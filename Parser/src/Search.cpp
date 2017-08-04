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
#include "CurlWrapper.hpp"
#include <future>
#include <vector>

using json = nlohmann::json;

Search::Search(std::string company) {
    this->company = company;
    this->starCount = 0;
}

void Search::startSearch() {
    std::string request;
    request = "http://api.github.com/users/" + this->company + "/repos";
    //std::cout << "request is: " << request << std::endl;
    
    CurlWrapper curl;
    curl.takeDataFrom(request);
    
    if (curl.getData() != "")
        countStars(curl.getData());
    else
        PrintError("data is missing");
}

//json parsing
void Search::countStars(std::string data) {
    
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

void Search::PrintError(std::string error) {
    std::cout << "ERROR: " << error << std::endl;
    okay = false;
}

bool Search::isOkay() {
    return okay;
}

std::string Search::getCompany() {
    return company;
}

int Search::getResult() {
    return starCount;
}

