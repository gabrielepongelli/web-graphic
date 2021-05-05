#include "../../includes/utilities/string_utilities.hpp"

std::string replaceAll(std::string s, char c, std::string r)
{
    int i = 0, j;
    while ((j = s.find(c, i)) >= 0)
    {
        s.replace(j, 1, r);
        i = j + r.length();
    }
    return s;
}

std::string bashEncode(std::string s)
{
    s = replaceAll(s, '\\', "\\\\");
    s = replaceAll(s, '\"', "\\\"");
    s = replaceAll(s, '\'', "\\\'");
    s = replaceAll(s, '\n', "\\n");
    s = replaceAll(s, '$', "\\x24");
    return s;
}