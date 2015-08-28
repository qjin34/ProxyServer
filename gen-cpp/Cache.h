#ifndef cache_H
#define cache_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <map>
#include <string>
#include <queue>
#include <vector>

#include <unistd.h>
#include <sys/select.h>
#include <curl/curl.h>



#define MAX_CACHE 2048  // kB

using namespace std;

struct wd_in {
    size_t size;
    size_t len;
    char *data;
};

class Cache{
    protected:
        double current_size = 0;
        map<string,string> cache;
	int hit_count = 0;
	int miss_count = 0;        

    public:
        string getContent(string url);
        char *getBody(char *url);
        static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
    	virtual void addRecord(string url, string content) = 0;
};

class FIFO_Cache: public Cache{
    private:
        queue<string> orderQ;
    
    public:
        void addRecord(string url, string content);
};

class Random_Cache: public Cache{
    private:
        vector<string> rand_vector;
    
    public:
        void addRecord(string url, string content);
};

class MAXS_Cache: public Cache{
    private:
        map<double, string> sizeMap;

    public:
        void addRecord(string url, string content);
};

#endif
