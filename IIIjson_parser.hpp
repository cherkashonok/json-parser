#ifndef IIIJSON_PARSER_HPP
#define IIIJSON_PARSER_HPP

#include <string>
#include <map>
#include <vector>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>

namespace ascii = boost::spirit::qi::ascii;
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;
using phoenix::ref;
using qi::alnum;
using qi::char_;
using qi::lit;
using qi::double_;
using ascii::space;
using qi::lexeme;
using qi::_1;
using ascii::space_type;

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


struct jsonParser : qi::grammar<std::string::iterator, space_type> {
    jsonParser(JSON& obj) : base_type{json}, obj{obj} {
        null = lit("null");
        boolean = lit("true") | lit("false");
        number = double_;
        array = char_('[') >> -(value >> *(char_(',') >> value)) >> char_(']');

        key = lexeme[char_('"') >> +alnum >> char_('"')];
        value = json | array | key | number | boolean | null;

        jsonObj = key >> char_(':') >> value;
        json = char_('{') >> -(jsonObj >> *(char_(',') >> jsonObj)) >> char_('}');
    }

    JSON& obj;
    // qi::rule<std::string::iterator, JSON(), space_type> json;
    // qi::rule<std::string::iterator, std::pair<jsonValue, jsonValue>(), space_type> jsonObj;
    //
    // qi::rule<std::string::iterator, jsonValue(), space_type> value;
    // qi::rule<std::string::iterator, jsonValue(), space_type> key;
    //
    // qi::rule<std::string::iterator, jsonValue(), space_type> array;
    qi::rule<std::string::iterator, jsonValue(), space_type> number;
    qi::rule<std::string::iterator, jsonValue(), space_type> boolean;
    qi::rule<std::string::iterator, jsonValue(), space_type> null;

    qi::rule<std::string::iterator, space_type> json;
    qi::rule<std::string::iterator, space_type> jsonObj;

    qi::rule<std::string::iterator, space_type> value;
    qi::rule<std::string::iterator, space_type> key;

    qi::rule<std::string::iterator, space_type> array;
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
    JSON (const std::string& name);
    JSON ();
    ~JSON ();

    jsonValue& operator[] (const jsonValue& x);

    void getJSON();

    void writeJsonValue(const std::string& name, const jsonValue* val_obj);
    void writeJSON(const std::string& name);

    const std::map<jsonValue, jsonValue>& getObject() const noexcept;
private:
    std::map<jsonValue, jsonValue> obj;

    inline static std::string file_name;
    inline static std::ostream* file;
    inline static int vloz = 0;
};


#endif 