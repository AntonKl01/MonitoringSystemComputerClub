#include "../include/Computer.h"

Computer::Computer(unsigned short _computer_number, unsigned int _earnings) {
    this->computer_number = _computer_number;
    this->earnings_per_hour = _earnings;
    this->all_earnings = 0;
    this->client = "";
    this->start_time = nullptr;
    this->end_time = nullptr;
    this->working_time = new MyTime("00:00");
    this->is_used = false;
}

void Computer::compute_working_time() {
    unsigned short hour_in_minute = (end_time->hour - start_time->hour) * 60;
    unsigned short minute_diff = end_time->minute - start_time->minute;
    unsigned short time_in_minute = hour_in_minute + minute_diff;

    unsigned short hour = time_in_minute / 60;
    unsigned short minute = time_in_minute % 60;
    working_time->hour = working_time->hour + hour;
    working_time->minute = working_time->minute + minute;
    if (working_time->minute >= 60) {
        working_time->hour = working_time->hour + working_time->minute / 60;
        working_time->minute = working_time->minute % 60;
    }
}

void Computer::compute_earnings() {
    unsigned short count = 0;
    unsigned short hour = start_time->hour;
    unsigned short minute = start_time->minute;

    while (hour <= end_time->hour) {
        if (hour == end_time->hour) {
            if (minute >= end_time->minute) {
                break;
            } 
        }
        count += 1;
        hour = hour + 1;
    }

    all_earnings = all_earnings + count * earnings_per_hour;
    
}
