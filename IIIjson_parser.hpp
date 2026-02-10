// version 1.0
#ifndef IIIJSON_PARSER_HPP
#define IIIJSON_PARSER_HPP

#include <algorithm>
#include <fstream>
#include <map>
#include <string>


enum jsonValueType {
    null,
    string,
    integer,
    boolean,
    array,
    json
};

class jsonValue {
private:

public:

};

class json {
private:
    std::map<jsonValue, jsonValue> obj;
public:
    json (const std::string& file_name);
    json (const json& parser) = delete;
    json& operator= (const json& parser) = delete;
    json ();
};


#endif