//
//  main.cpp
//  Parser
//
//  Created by Eugene on 8/1/17.
//  Copyright © 2017 Eugene. All rights reserved.
//

#include <iostream>
#include "Search.hpp"

int main()
{
    std::string company;
    std::cout << "Type company name:" << std::endl;
    std::cin >> company;
    //company = "fastlane";
    
    Search s1(company);
    s1.startSearch();
    
    if (s1.isOkay())
        std::cout << std::endl << s1.getCompany() + "'s total number of stars: " << s1.getResult() << std::endl;
    
    return 0;
}
