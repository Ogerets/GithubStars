//
//  Search.cpp
//  Parser
//
//  Created by Eugene on 8/4/17.
//  Copyright © 2017 Eugene. All rights reserved.
//

#include "Search.hpp"
#include <iostream>
#include "json.hpp"
#include <curl/curl.h>
#include <future>
#include <vector>

using json = nlohmann::json;

//curl writedata func
size_t Search::writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    static_cast<Search*>(up)->data.append(buf, size*nmemb);
    return size*nmemb;                          //tell curl how many bytes we handled
}

void Search::PrintError(std::string error) {
    std::cout << "ERROR: " << error << std::endl;
    isOkay = false;
}

//curl making request
void Search::getReposFromGithub() {
    std::string request;
    request = "http://api.github.com/users/" + this->company + "/repos";
    std::cout << "request is: " << request << std::endl;
    
    CURL *curl;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Ogerets");
        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Search::writeCallback);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);    //tell curl to output its progress
        
        curl_easy_perform(curl);
    }
    else
        PrintError("curl init falied");
    
    //std::cout << std::endl << data << std::endl;
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

//json parsing
void Search::countStars() {
    json j = json::parse(data);
    
    auto m = j.find("message");
    if ((m != j.end()) && (*m == "Not Found")) {
        PrintError("Company \"" + company + "\" not found");
        return;
    }
    
    int num_cpus = std::thread::hardware_concurrency();
    int part = int(j.size())/num_cpus;
    int rem = j.size() % num_cpus;
    if (part > 0) {
        std::shared_future<bool> future;
        for (int i = 0; i < num_cpus; i++) {
            future = (std::async(std::launch::async, [](int beg, json j, int end, std::atomic_int &starCount) {
                for (int i = beg; i < end; i++) {
                    auto it = j[i].find("stargazers_count");
                    if (it != j[i].end()) {
                        //std::cout << *it << std::endl;
                        starCount += int(*it);
                    }
                }
                return true;
            }, i*part, j, (i+1)*part, ref(starCount)));
        }
        future.wait();
    }
    
    for (int i = part*num_cpus; i < num_cpus*part + rem; i++) {
        auto it = j[i].find("stargazers_count");
        if (it != j[i].end()) {
            //std::cout << *it << std::endl;
            this->starCount += int(*it);
        }
    }
}

Search::Search(std::string company) {
    this->company = company;
    this->starCount = 0;
}

void Search::start() {
    getReposFromGithub();
    if (data != "")
        countStars();
    else
        PrintError("data is missing");
    
    if (isOkay)
        std::cout << std::endl << company + "'s total number of stars: " << starCount << std::endl;
}
