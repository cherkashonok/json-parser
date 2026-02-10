// version 1.0
#ifndef IIIJSON_PARSER_HPP
#define IIIJSON_PARSER_HPP

#include <algorithm>
#include <fstream>
#include <map>
#include <string>


class IIIjsonParser
{
private:

public:
    IIIjsonParser (const std::string& file_name);
    IIIjsonParser (const IIIjsonParser& parser) = delete;
    IIIjsonParser& operator= (const IIIjsonParser& parser) = delete;
    ~IIIjsonParser ();
};


#endif