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
    null=1,
    boolean,
    number,
    string,
    array,
    json,
    trash_value
};


class jsonValue {
public:
    jsonValue() : type(trash_value) {}
    jsonValue(const char* val);
    jsonValue(JSON* val);
    template<class T> jsonValue(const T& val);

    template<class T> T getValue() const noexcept;

    jsonValueType getType() const noexcept;
    void printType() const noexcept;

    jsonValue& operator[] (const jsonValue& x) const;
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

    jsonValue& operator[] (const jsonValue& x);

    void getJSON(const std::string& name);

    void writeJsonValue(const std::string& name, const jsonValue* val_obj);
    void writeJSON(const std::string& name);

    const std::map<jsonValue, jsonValue>& getObject() const noexcept;
private:
    std::map<jsonValue, jsonValue> obj;
    inline static std::string file_name;
    inline static std::fstream file;
};


#endif 