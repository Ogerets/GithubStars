//
//  Search.hpp
//  Parser
//
//  Created by Eugene on 8/4/17.
//  Copyright © 2017 Eugene. All rights reserved.
//

#ifndef Search_hpp
#define Search_hpp

#include <iostream>

class Search {
private:
    std::string company;
    std::string data;
    std::atomic<int> starCount;
    bool isOkay = true;
    
    static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);    //curl writedata func
    
    void PrintError(std::string error);
    void getReposFromGithub();                                                      //curl making request
    void countStars();                                                              //json parsing
    
public:
    Search(std::string company);
    
    void start();
};


#endif /* Search_hpp */
