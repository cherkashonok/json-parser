#ifndef IIIJSON_PARSER_HPP
#define IIIJSON_PARSER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

class JSON;
class jsonValue;

enum jsonValueType {
    null,
    boolean,
    number,
    string,
    array,
    json
};


class jsonValue {
public:
    template<class T> jsonValue(const T& val);
    jsonValue(const char* val);
    jsonValue(JSON* val);
    jsonValue() {}

    template<class T> T getValue() const;
    jsonValueType getType() const;

    jsonValue& operator= (const jsonValue& right);
    bool operator< (const jsonValue& right) const;
    bool operator== (const jsonValue& right) const;
private:
    jsonValueType         type;
    std::string             j1;
    double                  j2;
    bool                    j3;
    std::vector<jsonValue>  j4;
    JSON*                   j5;
};

class JSON {
public:
    JSON (const std::string& file_name);
    JSON ();
    ~JSON ();

    jsonValue& operator[] (const jsonValue& x) {
        return obj[x];
    }

    void get_JSON(const std::string& name);

    void write_jsonValue(const std::string& name, const jsonValue* val_obj);
    void write_JSON(const std::string& name);

    const std::map<jsonValue, jsonValue>& getObject() const;
private:
    std::map<jsonValue, jsonValue> obj;
    std::string file_name;
    std::fstream file;
};


#endif 