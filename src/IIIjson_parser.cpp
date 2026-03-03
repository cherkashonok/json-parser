#include <iostream>
#include <algorithm>

#include "IIIjson_parser.hpp"


// class jsonValue
// реализация getValue
template<>
void* jsonObj::getValue<void*>() const noexcept {
    return nullptr;
}

template<>
std::string jsonObj::getValue<std::string>() const noexcept {
    return j1;
}

template<>
double jsonObj::getValue<double>() const noexcept {
    return j2;
}

template<>
int jsonObj::getValue<int>() const noexcept {
    return j2;
}

template<>
bool jsonObj::getValue<bool>() const noexcept {
    return j3;
}

template<>
std::vector<jsonObj> jsonObj::getValue<std::vector<jsonObj>>() const noexcept {
    return j4;
}

template<>
JSON* jsonObj::getValue<JSON*>() const noexcept {
    return j5;
}

// реализация конструкторов и деструктора
jsonObj::jsonObj(const double& val) {
    type = number;
    j2 = val;
}

jsonObj::jsonObj(const int& val) {
    type = number;
    j2 = val;
}

jsonObj::jsonObj(const std::string& val) {
    type = string;
    j1 = val;
}

jsonObj::jsonObj(const char* val) {
    type = string;
    j1 = std::string(val);
}

jsonObj::jsonObj(const char& val) {
    type = string;
    j1 = val;
}

jsonObj::jsonObj(const bool& val) {
    type = boolean;
    j3 = val;
}

jsonObj::jsonObj(const std::vector<jsonObj>& val) {
    type = array;
    j4 = val;
}

jsonObj::jsonObj(JSON* val) {
    type = json;
    j5 = val;
}

jsonObj::jsonObj(const std::initializer_list<jsonObj>& x) {
    type = array;
    j4 = x;
}

// остальной функционал jsonValue
jsonType jsonObj::getType() const noexcept {
    return type;
}
void jsonObj::printType() const noexcept {
    if (type == null) std::cout << this << " type is 'null'" << std::endl;
    if (type == boolean) std::cout << this << " type is 'boolean'" << std::endl;
    if (type == number) std::cout << this << " type is 'number'" << std::endl;
    if (type == string) std::cout << this << " type is 'string'" << std::endl;
    if (type == array) std::cout << this << " type is 'array'" << std::endl;
    if (type == json) std::cout << this << " type is 'json'" << std::endl;
    if (type == trash_value) std::cout << this << " is not initialized" << std::endl;
}

jsonObj& jsonObj::operator[] (const jsonObj& x) const {
    if (type != json)
        throw std::runtime_error("jsonValue must have json type");

    if (x.getType() != string)
        throw std::runtime_error("key must be a string");
    return this->getValue<JSON*>()->operator[](x);
}

jsonObj& jsonObj::operator= (const jsonObj& right) {
    if (this != &right) {
        if (right.type == null) {
            type = right.type;
        } else if (right.type == string) {
            type = right.type;
            j1 = right.j1;
        } else if (right.type == number) {
            type = right.type;
            j2 = right.j2;
        } else if (right.type == boolean) {
            type = right.type;
            j3 = right.j3;
        } else if (right.type == array) {
            type = right.type;
            j4 = right.j4;
        } else if (right.type == json) {
            type = right.type;
            j5 = right.j5;
        }
    }

    return *this;
}

bool jsonObj::operator< (const jsonObj& right) const {
    if (this->type != right.type)
        return this->type < right.type;

    if (this->type == string)
        return this->j1 < right.j1;
    if (this->type == number)
        return this->j2 < right.j2;
    if (this->type == boolean)
        return this->j3 < right.j3;
    if (this->type == array)
        return this->j4 < right.j4;
    if (this->type == json)
        return this->j5->getObject() < right.j5->getObject();

    return true; // for null case
}

bool jsonObj::operator== (const jsonObj& right) const {
    if (type != right.type) return false;

    if (type == null) return    true;
    if (type == string) return  j1 == right.j1;
    if (type == number) return  j2 == right.j2;
    if (type == boolean) return j3 == right.j3;
    if (type == array) return   j4 == right.j4;
    if (type == json) return    j5 == right.j5;

    throw std::invalid_argument("argument is not initialized");
}


// class JSON
JSON::JSON () {
    file_name = "";
    file_o = nullptr;
}

JSON::JSON (const std::string& name) : JSON() {
    file_name = name;
}

JSON::JSON (const std::initializer_list<std::pair<jsonObj, jsonObj>>& x) : JSON() {
    for (auto& el : x) {
        if (el.first.getType() != string)
            throw std::invalid_argument("key must be a string");
        obj.insert(el);
    }
}

jsonObj& JSON::operator[] (const jsonObj& x) {
    if (x.getType() != string)
        throw std::runtime_error("key must be a string");

    return obj[x];
}

JSON* JSON::getJSON(const std::string& name=file_name) {
    std::string tmp;
    if (name == "std::cin") {
        std::getline(std::cin, tmp);
    } else {
        file_i.open(name);
        if (!file_i.is_open())
            throw std::runtime_error("JSON file could not be opened");

        std::string tmp_tmp;
        while (std::getline(file_i, tmp_tmp))
            tmp += tmp_tmp;

        file_i.close();
    }

    auto e = std::remove_if(tmp.begin(), tmp.end(), [](char c){ return c == '\n' || c == '\r'; });
    tmp.erase(e, tmp.end());

    jsonObj j_tmp;
    JSON* x = nullptr;
    if (const jsonParser p; p.parser.parse(tmp, j_tmp))
        x = j_tmp.getValue<JSON*>();
    return x;
}

void JSON::writeJsonValue(const std::string& name, const jsonObj* val_obj) {
    if (val_obj->getType() == null)
        *file_o << "null";
    if (val_obj->getType() == string)
        *file_o << '"' << val_obj->getValue<std::string>() << '"';
    if (val_obj->getType() == number)
        *file_o << val_obj->getValue<double>();
    if (val_obj->getType() == boolean)
        *file_o << std::boolalpha << val_obj->getValue<bool>();
    if (val_obj->getType() == json)
        val_obj->getValue<JSON*>()->writeJSON(name);

    if (val_obj->getType() == array) {
        auto&& o = val_obj->getValue<std::vector<jsonObj>>();

        *file_o << "[ ";
        for (auto it = o.begin(); it != o.end(); ++it) {
            writeJsonValue(name, &(*it));
            if (it != o.end()-1)
                *file_o << ", ";
        }
        *file_o << " ]";
    }
}

void JSON::writeJSON(const std::string& name=file_name) {
    if (name == "std::cout") {
        file_o = &std::cout;
    } else if (!vloz) {
        file_o = new std::ofstream;
        dynamic_cast<std::ofstream*>(file_o)->open(name);
        if (!dynamic_cast<std::ofstream*>(file_o)->is_open())
            throw std::runtime_error("JSON file could not be opened");
    }

    vloz++;
    *file_o << "{\n" << std::string(vloz, '\t');
    for (auto it = obj.begin(); it != obj.end();) {
        writeJsonValue(name, &it->first);
        *file_o << " : ";
        writeJsonValue(name, &it->second);

        if (++it != obj.end())
            *file_o << ",";
        *file_o << "\n" << std::string(vloz, '\t');
    }
    if (!--vloz)
        *file_o << "\r";
    *file_o << "}\n";

    if (name != "std::cout" && !vloz) {
        dynamic_cast<std::ofstream*>(file_o)->close();
        delete file_o;
        file_o = nullptr;
    }
}

const std::map<jsonObj, jsonObj>& JSON::getObject() const noexcept {
    return obj;
}