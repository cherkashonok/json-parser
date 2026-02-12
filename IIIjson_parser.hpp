#ifndef IIIJSON_PARSER_HPP
#define IIIJSON_PARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

class JSON;
class jsonValue;

enum jsonValueType {
    null,
    string,
    number,
    boolean,
    array,
    json
};


class jsonValue {
private:
    jsonValueType         type;
    std::string             j1;
    int                     j2;
    bool                    j3;
    std::vector<jsonValue>  j4;
    JSON*                   j5;
public:
    template<class T> jsonValue(const T& val);
    jsonValue(const char* val);
    jsonValue(JSON* val);
    jsonValue() {}

    template<class T> T getValue() const { 
        throw "unknown type"; 
    }

    jsonValue& operator= (const jsonValue& right);
    bool operator< (const jsonValue& right) const;
    bool operator== (const jsonValue& right) const;
};

class JSON {
private:
    std::map<jsonValue, jsonValue> obj;
public:
    JSON (const std::string& file_name);
    JSON () {}

    jsonValue& operator[] (const jsonValue& x) {
        return obj[x];
    }

    void get_JSON_from_file(const std::string& name);
    void write_JSON_to_file(const std::string& name);
};


#endif 