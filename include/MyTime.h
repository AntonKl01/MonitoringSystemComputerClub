#pragma once

#include <string>

class MyTime{
    public:
        unsigned short hour;
        unsigned short minute;
        MyTime(std::string _time);
        MyTime(const MyTime &_time);
        std::string from_time_to_str();
};