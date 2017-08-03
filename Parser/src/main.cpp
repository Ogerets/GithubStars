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
#include <future>
#include <vector>

using json = nlohmann::json;

class Search {
private:
    std::string company;
    std::string data;
    std::atomic<int> starCount;
    
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
       
        int num_cpus = std::thread::hardware_concurrency();
        int part = int(j.size())/num_cpus;
        int rem = j.size() % num_cpus;
        if (part > 0) {
            std::shared_future<bool> future;
            std::vector<std::future<bool>> futures(num_cpus);
            for (int i = 0; i < num_cpus; i++) {
                future = (std::async(std::launch::async, [](int beg, json j, int end, std::atomic_int &starCount) {
                    for (int i = beg; i < end; i++) {
                        auto it = j[i].find("stargazers_count");
                        if (it != j[i].end()) {
                            std::cout << *it << std::endl;
                            starCount += int(*it);
                        }
                    }
                    return true;
                }, i*part, j, (i+1)*part, ref(starCount)));
            }
            future.wait();
        }
        /*
        auto m = j.find("message");
        if ((m != j.end()) && (*m == "Not Found"))
            std::cout << "company not found!" << std::endl;
        */
    
        for (int i = part*num_cpus; i < num_cpus*part + rem; i++) {
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
        starCount = 0;
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
    std::cout << "Type company name:" << std::endl;
    std::cin >> s;
    //s = "fastlane";
    Search s1(s);
    s1.start();
    
    return 0;
}
