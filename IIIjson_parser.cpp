#include "IIIjson_parser.hpp"


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
    return sizeof(*this) < sizeof(right);
}

bool jsonValue::operator== (const jsonValue& right) const {
    if (type != right.type) return false;

    if (type == null) return    true;
    if (type == string) return  j1 == right.j1;
    if (type == number) return  j2 == right.j2;
    if (type == boolean) return j3 == right.j3;
    if (type == array) return   j4 == right.j4;
    if (type == json) return    j5 == right.j5;
}


// class JSON
JSON::JSON (const std::string& file_name) {

}

void JSON::get_JSON_from_file(const std::string& name) {

}

void JSON::write_JSON_to_file(const std::string& name) {

}



int main () {
    jsonValue t1(30);
    std::cout << t1.getValue<int>() << "\n";


    JSON example;

    example.get_JSON_from_file("example.json");

    example[30] = "gerr";
    example["fd"] = true;
    example[true] = 12424;

    std::cout << example[30].getValue<std::string>() << "\n";
    std::cout << example["fd"].getValue<bool>() << "\n";
    std::cout << example[true].getValue<int>() << "\n";
}