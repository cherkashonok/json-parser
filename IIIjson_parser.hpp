#ifndef IIIJSON_PARSER_HPP
#define IIIJSON_PARSER_HPP

#include <string>
#include <map>
#include <vector>
#include "peglib.h"


class JSON;
class jsonObj;

enum jsonType {
    null=1,
    boolean,
    number,
    string,
    array,
    json,
    trash_value
};

class jsonObj {
public:
    jsonObj() : type(trash_value) {}
    jsonObj(std::nullptr_t) : type(null) {}
    jsonObj(const double& val);
    jsonObj(const int& val);
    jsonObj(const std::string& val);
    jsonObj(const char* val);
    jsonObj(const char& val);
    jsonObj(const bool& val);
    jsonObj(const std::vector<jsonObj>& val);
    jsonObj(JSON* val);
    // template<class T> jsonObj(const T& val);

    template<class T> T getValue() const noexcept;
    jsonType getType() const noexcept;

    void printType() const noexcept;

    jsonObj& operator[] (const jsonObj& x) const;
    jsonObj& operator= (const jsonObj& right);
    bool operator< (const jsonObj& right) const;
    bool operator== (const jsonObj& right) const;
private:
    jsonType              type;
    std::string             j1;
    double                  j2;
    bool                    j3;
    std::vector<jsonObj>    j4;
    JSON*                   j5;
};

class JSON {
public:
    JSON (const std::string& name);
    JSON ();
    ~JSON ();

    jsonObj& operator[] (const jsonObj& x);

    void getJSON();

    void writeJsonValue(const std::string& name, const jsonObj* val_obj);
    void writeJSON(const std::string& name);

    const std::map<jsonObj, jsonObj>& getObject() const noexcept;
private:
    std::map<jsonObj, jsonObj> obj;

    inline static std::string file_name;
    inline static std::ostream* file;
    inline static int vloz = 0;
};

struct jsonParser {
    jsonParser () {
          peg::parser p(R"(

               json        <- '{' ' '* jsonObj? (' '* ',' ' '* jsonObj ' '*)* '}'
               jsonObj     <- ' '* key ' '* ':' ' '* value ' '*
               value       <- null / bool / key / number / array / json
               key         <- '"' char* '"'
               char        <- [a-zA-Z0-9_ ]
               array       <- '[' ' '* value? (' '* ',' ' '* value ' '*)* ']'
               number      <- [+-]?[0-9]+([.][0-9]+)?
               bool        <- 'true' / 'false'
               null        <- 'null'

           )");

        p["null"] = [](const peg::SemanticValues &vs) {
            return jsonObj(nullptr);
        };

        p["bool"] = [](const peg::SemanticValues &vs) {
            switch (vs.choice()) {
                case 0:  return jsonObj(true);
                default: return jsonObj(false);
            }
        };

        p["number"] = [](const peg::SemanticValues &vs) {
            return jsonObj(vs.token_to_number<double>());
        };

        p["array"] = [](const peg::SemanticValues &vs) {
            std::vector<jsonObj> x;
            for (auto& el : vs)
                x.push_back(std::any_cast<jsonObj>(el));
            return jsonObj(x);
        };

        p["char"] = [](const peg::SemanticValues &vs) {
            return any_cast<std::string>(vs.token_to_string());
        };

        p["key"] = [](const peg::SemanticValues &vs) {
            std::string x;
            for (auto& el : vs) {
                auto tmp = any_cast<std::string>(el);
                if (tmp[0] != '"')
                    x += tmp;
            }
            return jsonObj(x);
        };

        p["value"] = [](const peg::SemanticValues &vs) {
            return any_cast<jsonObj>(vs[0]);
        };

        p["jsonObj"] = [](const peg::SemanticValues &vs) {
            return std::pair{std::any_cast<jsonObj>(vs[0]), std::any_cast<jsonObj>(vs[1])};
        };

        p["json"] = [](const peg::SemanticValues &vs) {
            auto x = new JSON;
            for (auto& el : vs) {
                auto pr = any_cast<std::pair<jsonObj, jsonObj>>(el);
                x->operator[](pr.first) = pr.second;
            }
            return jsonObj(x);
        };

        p.set_logger([](size_t line, size_t col, const std::string& msg) {
            printf("Error on line %zi:%zi -> %s\n", line, col, msg.c_str());
        });

        parser = p;
    }
    peg::parser parser;
};

#endif
