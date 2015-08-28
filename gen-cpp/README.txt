******************By Qiuxiang Jin, Georgia Institute of Technology************************

This is a proxy server project based on Apache Thrift. The project has been tested on Ubuntu Server 14.04 LTS. Other version of linux operating system might or might not work. 

1. To run the program, you need to install Apache Thrift on both the server and client machine. Go to “http://thrift.apache.org/docs/install/“ for details about how to install Apache thrift. 

2. Before building the project, please first go to both “Proxy_client_Repeat.cpp” and “Proxy_client_Random.cpp” in the “./gen-cpp” directory, and change the IP address on line 27 in both files to the public IP address of your server machine.

3. To build the project, go to the “./gen-cpp” directory and enter the “make” command. Three executables will be generated: “proxy_server”,”proxy_client_Random” and “proxy_client_Repeat”. 

4. The “proxy_server” runs on the server side. Both “proxy_client_Random” and “proxy_client_Random” run on the client side. “proxy_client_Random” is used to generate random workload, and “repeat_client_Random” is used to generate repeated workload.

5. The default cache size in the proxy server is 2048 KB, you can go to the line 19 in the “Cache.h” file and change it to the size you want.

6. The default caching policy is MAXS(Largest Size First), and you can go to line 26, 27 and 28 to uncomment the cache policy you prefer to use. The available choices are Random, FIFO and MAXS. 

