#include <algorithm>
#include <string>
#include <iostream>

#include "IIIjson_parser.hpp"


int main() {
    jsonValue t1(30, integer);
    std::cout << t1.getValue<int>() << "\n";
}