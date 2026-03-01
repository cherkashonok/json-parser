#include "IIIjson_parser.hpp"
#include <iostream>
#include <fstream>


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
    file = nullptr;
}
JSON::JSON (const std::string& name)  {
    file_name = name;
    file = new std::ofstream;
    dynamic_cast<std::ofstream*>(file)->open(file_name);
    if (!dynamic_cast<std::ofstream*>(file)->is_open())
        throw std::runtime_error("JSON file could not be opened");
}

JSON::~JSON () {
    if (file) {
        dynamic_cast<std::ofstream*>(file)->close();
        delete file;
    }
}

jsonObj& JSON::operator[] (const jsonObj& x) {
    if (x.getType() != string)
        throw std::runtime_error("key must be a string");

    return obj[x];
}


void JSON::writeJsonValue(const std::string& name, const jsonObj* val_obj) {
    if (val_obj->getType() == null)
        *file << "null";
    if (val_obj->getType() == string)
        *file << '"' << val_obj->getValue<std::string>() << '"';
    if (val_obj->getType() == number)
        *file << val_obj->getValue<double>();
    if (val_obj->getType() == boolean)
        *file << std::boolalpha << val_obj->getValue<bool>();
    if (val_obj->getType() == json)
        val_obj->getValue<JSON*>()->writeJSON(name);

    if (val_obj->getType() == array) {
        auto&& o = val_obj->getValue<std::vector<jsonObj>>();

        *file << "[ ";
        for (auto it = o.begin(); it != o.end(); ++it) {
            writeJsonValue(name, &(*it));
            if (it != o.end()-1)
                *file << ", ";
        }
        *file << " ]";
    }
}

void JSON::writeJSON(const std::string& name) {
    if (name == "std::cout") {
        file = &std::cout;
    } else if (name != file_name) {
        if (!file)
            file = new std::ofstream;
        else if (dynamic_cast<std::ofstream*>(file)->is_open())
            dynamic_cast<std::ofstream*>(file)->close();

        file_name = name;
        dynamic_cast<std::ofstream*>(file)->open(name);

        if (!dynamic_cast<std::ofstream*>(file)->is_open())
            throw std::runtime_error("JSON file could not be opened");

    }

    vloz++;
    *file << "{\n" << std::string(vloz, '\t');
    for (auto it = obj.begin(); it != obj.end();) {
        writeJsonValue(name, &it->first);
        *file << " : ";
        writeJsonValue(name, &it->second);

        if (++it != obj.end())
            *file << ",";
        *file << "\n" << std::string(vloz, '\t');
    }
    if (!--vloz)
        *file << "\r";
    *file << "}\n";
}

const std::map<jsonObj, jsonObj>& JSON::getObject() const noexcept {
    return obj;
}


int main () {
    jsonObj o;
    std::string input;
    std::getline(std::cin, input);
    if (const jsonParser p; p.parser.parse(input, o)) {
        auto x = o.getValue<JSON*>();
        x->writeJSON("std::cout");
        delete x;
    }

    // работает с крашами
    // JSON example;
    //
    // example["a"] = false;
    // example["b"] = 12424;
    // example["c"] = "germany";
    //
    // // настоящая головная боль:
    // example["d"] = std::vector<jsonObj>{123, 143, 154};
    // example["e"] = new JSON;
    // example["e"]["1"] = 20;
    // example["e"]["2"] = new JSON;
    // example["e"]["2"]["v"] = true;
    //
    //
    // //
    // // example["d"].printType();
    // // example["c"].printType();
    // // example["e"].printType();
    // // example["e"]["1"].printType();
    // example.writeJSON("std::cout");
}