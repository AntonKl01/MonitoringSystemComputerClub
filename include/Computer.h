#pragma once

#include "MyTime.h"

class Computer{
    public:
        unsigned short computer_number;
        bool is_used;
        std::string client;
        MyTime* start_time;
        MyTime* end_time;
        MyTime* working_time;
        unsigned int earnings_per_hour;
        unsigned int all_earnings;
        Computer(unsigned short _computer_number, unsigned int _earnings);
        void compute_working_time();
        void compute_earnings();
};
