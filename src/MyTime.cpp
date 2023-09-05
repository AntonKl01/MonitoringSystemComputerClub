#include "../include/MyTime.h"

MyTime::MyTime(std::string _time) {
    std::string delimiter = ":";
    unsigned short pos = 0;

    pos = _time.find(delimiter);
    std::string hour_token = _time.substr(0, pos);
    _time.erase(0, pos + delimiter.length());
    std::string minute_token = _time;

    hour = std::stoul(hour_token, nullptr, 10);
    minute = std::stoul(minute_token, nullptr, 10);
}

MyTime::MyTime(const MyTime &time) {
    this->hour = time.hour;
    this->minute = time.minute;
}

std::string MyTime::from_time_to_str() {
    std::string str_time;
    if (0 <= hour && hour <= 9) {
        if (0 <= minute && minute <= 9) {
            str_time = "0" + std::to_string(hour) + ":" + "0" + std::to_string(minute);
        } else {
            str_time = "0" + std::to_string(hour) + ":" + std::to_string(minute);
        }
    } else {
        if (0 <= minute && minute <= 9) {
            str_time = std::to_string(hour) + ":" + "0" + std::to_string(minute);
        } else {
            str_time = std::to_string(hour) + ":" + std::to_string(minute);
        }
    }

    return str_time;
}
