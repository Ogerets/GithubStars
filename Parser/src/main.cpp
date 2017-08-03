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
#include <thread>

using json = nlohmann::json;


class Search {
private:
    std::string company;
    std::string data;
    int starCount = 0;
    
    static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
        static_cast<Search*>(up)->data.append(buf, size*nmemb);
        return size*nmemb;  //tell curl how many bytes we handled
    }
    
    void getReposFromGithub() {
        std::string request;
        request = "http://api.github.com/users/" + company + "/repos";
        std::cout << "request is: " << request << std::endl;
        
        CURL *curl;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Ogerets");
        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Search::writeCallback);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);    //tell curl to output its progress
        
        curl_easy_perform(curl);
        
        //std::cout << std::endl << data << std::endl;
        
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    
    void countStars() {
        json j = json::parse(data);
        for (int i = 0; i < j.size(); i++) {
            auto it = j[i].find("stargazers_count");
            if (it != j[i].end()) {
                std::cout << *it << std::endl;
                starCount += int(*it);
            }
        }
    }

public:
    Search(std::string company) {
        this->company = company;
    }
    
    void start() {
        getReposFromGithub();
        if (data != "")
            countStars();
        std::cout << std::endl << starCount << std::endl;
    }
};


int main()
{
    std::string s;
    //std::cout << "Type company name:" << std::endl;
    //std::cin >> s;
    s = "fastlane";
    Search *s1 = new Search(s);
    s1->start();
    
    return 0;
}
