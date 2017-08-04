//
//  CurlWrapper.cpp
//  Parser
//
//  Created by Eugene on 8/4/17.
//  Copyright © 2017 Eugene. All rights reserved.
//

#include "CurlWrapper.hpp"

size_t CurlWrapper::writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    static_cast<CurlWrapper*>(up)->data.append(buf, size*nmemb);
    return size*nmemb;                          //tell curl how many bytes we handled
}

CurlWrapper::CurlWrapper() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
}

void CurlWrapper::takeDataFrom(std::string request) {
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Ogerets");
    curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlWrapper::writeCallback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);                            //tell curl to output its progress
    
    curl_easy_perform(curl);
}

std::string CurlWrapper::getData() {
    return data;
}

CurlWrapper::~CurlWrapper() {
    curl_easy_cleanup(curl);
    curl_global_cleanup();
};
