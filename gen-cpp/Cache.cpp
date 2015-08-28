#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>       /* time */

#include <sys/select.h>
#include <curl/curl.h>
#include "Cache.h"



using namespace std;


string Cache::getContent(string url){
        
    // When hit
    if(cache.count(url)){
	hit_count++;
        cout << "Hit: " << hit_count << "   Miss: " << miss_count  << endl;
        return cache[url];
    }
        
    // When miss
    miss_count++;
    cout << "Hit: " << hit_count << "   Miss: "<< miss_count << endl;
    char *query = new char[url.size()+1]; // Convert the string to char array
    strcpy(query, url.c_str());
        
    // Get the result
    char *result = getBody(query);
    string return_value(result);
    free(query);
    free(result);
    
    //cout << return_value << endl;
    //Put the result into cache;
    addRecord(url, return_value);
    return return_value;
}

size_t Cache::write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    struct wd_in *wdi = (struct wd_in *) userp;
    
    while(wdi->len + (size * nmemb) >= wdi->size) {
        /* check for realloc failing in real code. */
        wdi->data = (char *)realloc(wdi->data, wdi->size*2);
        wdi->size*=2;
    }
    
    memcpy(wdi->data + wdi->len, buffer, size * nmemb);
    wdi->len+=size*nmemb;
    
    return size * nmemb;
}

char* Cache::getBody(char *url){
    CURL *curl;
    CURLcode res;
    struct wd_in wdi;
    memset(&wdi, 0, sizeof(wdi));
    curl = curl_easy_init();   /* Get a curl handle.  Each thread will need a unique handle. */
       
    if(NULL != curl) {
        wdi.size = 1024;
        wdi.data = (char *)malloc(wdi.size);         /* Check for malloc failure in real code. */
        curl_easy_setopt(curl, CURLOPT_URL, url);    /* Set the URL for the operation. */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        /* "write_data" function to call with returned data. */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wdi);   /* userp parameter passed to write_data. */
        res = curl_easy_perform(curl);        /* Actually perform the query. */
        curl_easy_cleanup(curl);              /* Clean up after ourselves. */
    }
    else {
        fprintf(stderr, "Error: could not get CURL handle.\n");
        exit(EXIT_FAILURE);
    }
    return wdi.data;
}


void FIFO_Cache::addRecord(string url, string content){
    
    if(content.size()/1024.0 > MAX_CACHE){
        cout << url << " is too large to put into cache!" << endl;
        return;
    }
            
    // When the space is not enough, evict some content
    while(content.size()/1024.0 + current_size > MAX_CACHE){
        current_size -= cache[orderQ.front()].size()/1024.0;
        cache.erase(orderQ.front());
        cout << "Evict: " << orderQ.front() << endl;
        cout << "Current cache size: " << current_size << " / " << MAX_CACHE << "(KB)" << endl;
        orderQ.pop();
    }
           
    cache[url] = content;
    orderQ.push(url);
    current_size += content.size()/1024.0;
    cout << "Add: " << url << endl;
    cout << "Current cache size: " << current_size << " / " << MAX_CACHE << "(KB) " <<endl;
}


void Random_Cache::addRecord(string url, string content){
    
    if(content.size()/1024.0 > MAX_CACHE){
        cout << url << " is too large to put into cache!" << endl;
        return;
    }
    
    /* initialize random seed: */
    srand (time(NULL));
    int i;
    int evict = 0;
    
    // When the space is not enough, evict some content
    while(content.size()/1024.0 + current_size > MAX_CACHE){
        evict = 1;
        i = rand() % rand_vector.size();   /* generate a random number between 0 to vector.size() - 1: */
        current_size -= cache[rand_vector[i]].size()/1024.0;
        cache.erase(rand_vector[i]);
        cout << "Evict: " << rand_vector[i] << endl;
        cout << "Current cache size: " << current_size << " / " << MAX_CACHE << "(KB)" << endl;
        if(content.size()/1024.0 + current_size > MAX_CACHE){
            rand_vector.erase(rand_vector.begin()+i); //Only when the room is still small do we need erase
        }
    }
    
    cache[url] = content;       // Put the content in the cache
    
    if(evict == 1){              // When evict happens, swap
        rand_vector[i] = url;
    }else{                      // Otherwise put the new element to end of the vector
        rand_vector.push_back(url);
    }
    
    current_size += content.size()/1024.0;
    cout << "Add: " << url << endl;
    cout << "Current cache size: " << current_size << " / " << MAX_CACHE << "(KB) " <<endl;
}

void MAXS_Cache::addRecord(string url, string content){
    
    if(content.size()/1024.0 > MAX_CACHE){
        cout << url << " is too large to put into cache!" << endl;
        return;
    }
    
    
    // When the space is not enough, evict some content
    while(content.size()/1024.0 + current_size > MAX_CACHE){
        current_size -= cache[sizeMap.rbegin()->second].size()/1024.0;
        cache.erase(sizeMap.rbegin()->second);
        cout << "Evict: " << sizeMap.rbegin()->second << endl;
        cout << "Evict Size: " << sizeMap.rbegin()->first << endl;
        cout << "Current cache size: " << current_size << " / " << MAX_CACHE << "(KB)" << endl;
        sizeMap.erase(sizeMap.rbegin()->first);
    }
    
    cache[url] = content;
    
    double fileSize = content.size()/1024.0;
    while(sizeMap.count(fileSize)){                 // Collision Avoidance
        srand (time(NULL));
        fileSize -= 0.001 * (double)rand()/RAND_MAX;
    }
    
    sizeMap[fileSize] = url;
    
    current_size += content.size()/1024.0;
    cout << "Add: " << url << endl;
    cout << "Current cache size: " << current_size << " / " << MAX_CACHE << "(KB) " <<endl;

}








