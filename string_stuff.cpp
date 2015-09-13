#include "string_stuff.h"

#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;

string String_Stuff::make_string_lower_case(string str_input){
    to_lower(str_input);

    return str_input;
}

string String_Stuff::process_newlines(string str_input){
    string newline="\\";
    newline+="n";

    replace_all(str_input,newline,"\xA");

    return str_input;
}

int String_Stuff::newline_count(string str_input){
    int newlines=0;
    string newline="\xA";

    for(int i=0;i<str_input.length();i++){
        if(str_input[i]==newline[0]){
            newlines++;
        }
    }

    return newlines;
}

int String_Stuff::length_of_last_line(std::string str_input){
    vector<string> lines;
    split(lines,str_input,is_any_of("\xA"));

    return lines[lines.size()-1].length();
}

std::string String_Stuff::erase_first_line(std::string str_input){
    string newline="\xA";

    for(int i=0;i<str_input.length();i++){
        if(str_input[i]==newline[0]){
            str_input.erase(0,1);

            break;
        }
        else{
            str_input.erase(0,1);

            i--;
        }
    }

    return str_input;
}
