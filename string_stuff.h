#ifndef string_stuff_h
#define string_stuff_h

#include <string>
#include <sstream>

class String_Stuff{
public:

    //Returns a string with all uppercase letters converted to lowercase.
    std::string make_string_lower_case(std::string str_input);

    template<class Number_Type>
    inline std::string num_to_string(Number_Type number){
        std::string message="";

        std::stringstream strstream("");
        strstream.clear();strstream.str("");strstream<<number;message=strstream.str();

        return message;
    }

    //Process newlines (\n) in a string that has been read from disk.
    std::string process_newlines(std::string str_input);

    //Returns the number of newlines in the passed string.
    int newline_count(std::string str_input);

    //Returns the length of the last line in a string.
    //Simply returns the length of the string if it has only one line.
    int length_of_last_line(std::string str_input);

    std::string erase_first_line(std::string str_input);
};

#endif
