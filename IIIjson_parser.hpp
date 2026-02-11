// version 1.1.1
#ifndef IIIJSON_PARSER_HPP
#define IIIJSON_PARSER_HPP

#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

class JSON;
class jsonValue;


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
    jsonValueType        type;
    std::string            j1;
    int64_t                j2;
    bool                   j3;
    std::vector<jsonValue> j4;
    JSON                   j5;
public:
    template<class T>
    jsonValue(const T& val) {
        switch (type) {
            case string:
                j1 = val; break;
            case integer:
                j2 = val; break;
            case boolean:
                j3 = val; break;
            case array:
                j4 = val; break;
            case json:
                j5 = val;
        }
    }

    template<class T>
    jsonValue(const T& val, jsonValueType type) : type(type) {
        jsonValue(val);
    }

    template<class T>
    T getValue() {
        switch (type) {
            case null: 
                return nullptr; break;
            case string:
                return j1; break
            case integer:
                return j2; break
            case boolean:
                return j3; break
            case array:
                return j4; break
            case json:
                return j5;
        }
    }

    jsonValue (const jsonValue& parser) = delete;
    jsonValue& operator= (const jsonValue& parser) = delete;
};


// nf
class JSON {
private:
    std::map<jsonValue, jsonValue> obj;
public:
    JSON (const std::string& file_name);

    JSON (const JSON& parser) = delete;
    JSON& operator= (const JSON& parser) = delete;
};

// nf
JSON get_JSON_from_file(const std::string& name);
void write_JSON_to_file(const std::string& name);

// спиздить или разработать больше функционала

#endif