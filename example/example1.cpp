#include "IIIjson_parser.hpp"

int main () {
    JSON example;

    example["e"] = new JSON{ {"a1", 20}, {"a2", true} };
    example["d"] = {123, 143, 154};
    example["c"] = "germany";
    example["b"] = 12424;
    example["a"] = false;
    example["a0"] = nullptr;
    example.writeJSON("std::cout");
    example.writeJSON("example1.json");

    JSON f = {
        {"b3", {123, true, false, true, nullptr}},
        {"b4", new JSON{{"c", {1, 2, 3, 4, 5}}}},
        {"b2", 2142152.214323},
        {"b1", 124}
    };
    f.writeJSON("std::cout");
    f.writeJSON("example2.json");


    JSON e;
    e = *e.getJSON("example3.json");
    e.writeJSON("std::cout");
}