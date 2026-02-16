#include "IIIjson_parser.hpp"
// #include <boost/spirit/include/qi.hpp>
#include <iostream>
// namespace qi = boost::spirit::qi;


// class jsonValue 
// реализация getValue
template<> 
void* jsonValue::getValue<void*>() const {
    return nullptr; 
}

template<> 
std::string jsonValue::getValue<std::string>() const {
    return j1; 
}

template<>
double jsonValue::getValue<double>() const {
    return j2;
}

template<> 
int jsonValue::getValue<int>() const {
    return j2; 
}

template<> 
bool jsonValue::getValue<bool>() const {
    return j3; 
}

template<> 
std::vector<jsonValue> jsonValue::getValue<std::vector<jsonValue>>() const {
    return j4; 
}

template<> 
JSON* jsonValue::getValue<JSON*>() const {
    return j5; 
}

// реализация конструкторов
template<> 
jsonValue::jsonValue<std::string>(const std::string& val) {
    type = string;
    j1 = val;
}

template<>
jsonValue::jsonValue<double>(const double& val) {
    type = number;
    j2 = val;
}

template<> 
jsonValue::jsonValue<int>(const int& val) {
    type = number;
    j2 = val;
}

template<>
jsonValue::jsonValue<bool>(const bool& val) {
    type = boolean;
    j3 = val;
}

template<> 
jsonValue::jsonValue<std::vector<jsonValue>>(const std::vector<jsonValue>& val) {
    type = array;
    j4 = val;
}

jsonValue::jsonValue(const char* val) {
    type=string;
    j1 = std::string(val);
}

jsonValue::jsonValue(JSON* val) {
    type = json;
    j5 = val;
}

// остальной функционал jsonValue
jsonValueType jsonValue::getType() const {
    return type;
}

jsonValue& jsonValue::operator= (const jsonValue& right) {
    if (this == &right)
        return *this;

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

    return *this;
}

bool jsonValue::operator< (const jsonValue& right) const {
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

bool jsonValue::operator== (const jsonValue& right) const {
    if (type != right.type) return false;

    if (type == null) return    true;
    if (type == string) return  j1 == right.j1;
    if (type == number) return  j2 == right.j2;
    if (type == boolean) return j3 == right.j3;
    if (type == array) return   j4 == right.j4;
    if (type == json) return    j5 == right.j5;

    throw "not initialized object";
}


// class JSON
JSON::JSON () {
    std::cout << this << " had been constructed\n";

    file_name = "";
}
JSON::JSON (const std::string& file_name) : file_name(file_name) {
    std::cout << this << " had been constructed\n";

    file.open(file_name, std::ios::out | std::ios::in);
    if (!file.is_open())
        throw std::runtime_error("JSON file could not be opened");
}
JSON::~JSON () {
    file.close();

    std::cout << this << " had been destroyed\n";
}

// void JSON::get_JSON(const std::string& name) {
//     if (!file_name.empty() || name != file_name) {
//         if (file.is_open())
//             file.close();
//
//         file_name = name;
//         file.open(name, std::ios::app | std::ios::out | std::ios::in);
//
//         if (!file.is_open())
//             throw std::runtime_error("JSON file could not be opened");
//     }
// }

void JSON::write_jsonValue(const std::string& name, const jsonValue* val_obj) {
    if (val_obj->getType() == null)
        file << "null";
    if (val_obj->getType() == string)
        file << '"' << val_obj->getValue<std::string>() << '"';
    if (val_obj->getType() == number)
        file << val_obj->getValue<double>();
    if (val_obj->getType() == boolean)
        file << std::boolalpha << val_obj->getValue<bool>();
    if (val_obj->getType() == json)
        val_obj->getValue<JSON*>()->write_JSON(name);

    if (val_obj->getType() == array) {
        auto it = val_obj->getValue<std::vector<jsonValue>>().begin();
        auto end = val_obj->getValue<std::vector<jsonValue>>().end();

        file << "[ ";
        for (; it != end; ++it) {
            write_jsonValue(name, &(*it));
            if (it != end-1)
                file << ", ";
        }
        file << " ]";
    }
}

void JSON::write_JSON(const std::string& name) {
    if (!file_name.empty() || name != file_name) {
        if (file.is_open())
            file.close();

        file_name = name;
        file.open(name, std::ios::app | std::ios::out | std::ios::in);

        if (!file.is_open())
            throw std::runtime_error("JSON file could not be opened");
    }

    file << "{\n\t";
    for (auto it = obj.begin(); it != obj.end();) {
        jsonValue v1 = it->first; // ?
        write_jsonValue(name, &v1); // ?
        file << " : ";
        write_jsonValue(name, &it->second); // ?

        if (++it != obj.end())
            file << ",\n\t";
        else
            file << "\n";
    }
    file << "}";
}

const std::map<jsonValue, jsonValue>& JSON::getObject() const {
    return obj;
}


int main () {
    JSON example;

    example["fd"] = false;
    example["12424"] = 12424;
    example["sgdgsd"] = "gerr";

    // std::cout << example["sgdgsd"].getValue<std::string>() << std::endl;
    // std::cout << example["fd"].getValue<bool>() << std::endl;
    // std::cout << example["12424"].getValue<int>() << std::endl;
    //
    // std::cout << &example << " had " << example.getObject().size() << " elements\n";

    example.write_JSON("./example1.json");







    // std::string s;
    // std::getline(std::cin, s);
    // auto it = s.begin();
    // bool match = qi::parse(it, s.end(), qi::double_);
    // std::cout << std::boolalpha << match << std::endl;
    // if (it != s.end()) {
    //     std::cout << std::string(it, s.end()) << std::endl;
    // }
}