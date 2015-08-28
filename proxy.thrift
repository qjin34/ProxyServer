#!/usr/local/bin/thrift --gen cpp

namespace cpp Test

service Proxy{
   i32 ping(),    /*Used for testing*/
   string getResult(1:string url)  /*Used for get the content*/
}

