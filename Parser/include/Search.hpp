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
    bool okay = true;
    
    static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);    //curl writedata func
    
    void PrintError(std::string error);
    void getReposFromGithub();                                                      //curl making request
    void countStars();                                                              //json parsing
    
public:
    Search(std::string company);
    
    void startSearch();
    bool isOkay();
    std::string getCompany();
    int getResult();
};


#endif /* Search_hpp */
