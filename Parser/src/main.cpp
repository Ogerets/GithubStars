//
//  main.cpp
//  Parser
//
//  Created by Eugene on 8/1/17.
//  Copyright © 2017 Eugene. All rights reserved.
//

#include <iostream>
#include "json.hpp"
#include <curl/curl.h>

using json = nlohmann::json;

std::string data; //will hold the url's contents

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
    //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer
    
    for (int c = 0; c<size*nmemb; c++) {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

int main()
{
    std::string s;
    std::cout << "Type company name:" << std::endl;
    std::cin >> s;
    s = "http://api.github.com/users/" + s + "/repos";
    std::cout << s << std::endl;
    
    CURL* curl; //our curl object
    
    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Ogerets");
    curl_easy_setopt(curl, CURLOPT_URL, s.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
    
    curl_easy_perform(curl);
    
    //std::cout << std::endl << data << std::endl;
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    int count = 0;
    json j = json::parse(data);
    for (int i = 0; i < j.size(); i++) {
        auto it = j[i].find("stargazers_count");
        if (it != j[i].end()) {
            std::cout << *it << std::endl;
            count += int(*it);
        }
    }
    
    
    std::cout << std::endl;
    std::cout << std::endl << count << std::endl;
    return 0;
}
