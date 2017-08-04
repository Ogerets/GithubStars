//
//  CurlWrapper.hpp
//  Parser
//
//  Created by Eugene on 8/4/17.
//  Copyright © 2017 Eugene. All rights reserved.
//

#ifndef CurlWrapper_hpp
#define CurlWrapper_hpp

#include <iostream>
#include <curl/curl.h>

class CurlWrapper {
    
private:
    CURL *curl;
    std::string data;
    
    static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);
    
public:
    CurlWrapper();
    
    void takeDataFrom(std::string request);
    std::string getData();
    
    ~CurlWrapper();
};

#endif /* CurlWrapper_hpp */
