
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
    
     ifstream myfile ("alexa10k.txt");
     if (myfile.is_open()){
         for(int i = 0; i < WEBSITE_NUM; i++){
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
     string result;
     gettimeofday(&start, NULL);
     for(int i = 0; i < 450; i++){
         visit = rand() % websiteList.size();
     	 client.getResult(result, websiteList[visit]);
         cout << "Visited: " << websiteList[visit] << " Size: "<< result.size()<< endl;
     }
     gettimeofday(&end, NULL);
     
     transport->close();
     int total_second = end.tv_sec - start.tv_sec;
     cout << "Total time: " << total_second << "s" << endl;
    
     return 0;
}
