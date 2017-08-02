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
int count = 0;
bool m = false;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer
    
    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

int main()
{
    std::string s;
    //char *s;
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
    
    json j = json::parse(data);
    
    
    //int count = 0;
    std::cout << std::endl;
    json::parser_callback_t cb = [](int depth, json::parse_event_t event, json & parsed)
    {
        // skip object elements with key "Thumbnail"
        if (event == json::parse_event_t::key and parsed == json("stargazers_count"))
        {
            //count ;
            //s = parsed();
            m = true;
            return false;
        }
        else
        {
            if (m == true) {
                m = false;
                int neww = std::stoi(parsed.dump());
                std::cout << neww << std::endl;
                count += neww;
            }
            return true;
        }
    };
    
    // parse (with callback) and serialize JSON
    json j_filtered = json::parse(data, cb);
    //std::cout << s << std::endl;
    std::cout << std::endl << count << std::endl;
    //std::cout << std::setw(4) << j_filtered << '\n';
    //int foo_present = j.count("stargazers_count");
    
    //std::cout << j << std::endl;
    /*
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::cout << *it << '\n' << '\n';
    }*/
    
    //std::cout << foo_present << std::endl;
    
    return 0;
}

/*int main(int argc, const char * argv[]) {
    std::cout << "Hello, ssss!\n";
    std::string s = "{ \"happy\": true, \"pi\": 3.141 }";
    json j = json::parse(s);
 
    return 0;
}*/
