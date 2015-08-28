#include "Proxy.h"  // As an example

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

#include <iostream>
#include <string>
#include <sys/time.h>
#include <iostream>
#include <fstream> 
#include <time.h>       /* time */

#include <vector>

#define WEBSITE_NUM 50

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace std;

using namespace Test;

int main(int argc, char **argv) {
     boost::shared_ptr<TSocket> socket(new TSocket("54.173.141.220", 9090));
     boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

     ProxyClient client(protocol);
     transport->open();
     //client.ping(); //test the conection
    
    // reading a text file
     string line;
     vector<string> websiteList;
     int i;  
     ifstream myfile ("alexa10k.txt");
     if (myfile.is_open()){
         for(i = 0; i < WEBSITE_NUM;i++){
             getline (myfile,line);
             websiteList.push_back(line);
         }
         myfile.close();
     }
     else{
         cout << "Unable to open file" << endl;
         exit(0);
     }
    
     struct timeval start, end;   //Used for count time
     // Visited the website
     srand (time(NULL));
     int visit;
     int group;
     string result;
     gettimeofday(&start, NULL);
     for(int j = 0; j < 30; j++){  // Repeat group process for 30 times
        group = rand() % 10;     // Total 10 group, each group has 5 websites;
        for(int i = 0; i < 15; i++){ // Repeat the websites in each group for 15 times;
          visit = group * 5 + rand() % 5;
            client.getResult(result, websiteList[visit]);
            cout << "Visited: " << websiteList[visit] << " Size: "<< result.size()<< endl;
        }
     }
     gettimeofday(&end, NULL);
     
     transport->close();
     int total_second = end.tv_sec - start.tv_sec;
     cout << "Total time: " << total_second << "s" << endl;
    
     return 0;
}
