#pragma once

#include <deque>
#include <vector>
#include <set>
#include <regex>
#include <iostream>
#include <fstream>
#include "MyTime.h"
#include "Computer.h"

enum IncomingEvent {
    ID1 = 1,
    ID2 = 2,
    ID3 = 3,
    ID4 = 4
};

enum OutgoingEvents {
    ID11 = 11,
    ID12 = 12,
    ID13 = 13
};

class ComputerClub {
    private:
        std::deque<std::string> clients_deque;
        std::vector<Computer*> computers;
        std::set<std::string> clients_in_club;
        MyTime* time_incoming_events;

    public:
        unsigned short computers_amount;
        MyTime* start_time;
        MyTime* end_time;
        unsigned short earnings_per_hour;

        ComputerClub() {
            this->time_incoming_events = nullptr;
            this->computers_amount = 0;
            this->earnings_per_hour = 0;
            this->start_time = nullptr;
            this->end_time = nullptr;
        }

        std::string delete_extra_spaces(std::string str) {
            std::string format_str;
            std::regex leading_spaces_reg("(^ +)");
            format_str = std::regex_replace(str, leading_spaces_reg, "");

            std::regex tailing_spaces_reg("( +$)");
            format_str = std::regex_replace(format_str, tailing_spaces_reg, ""); 

            std::regex extra_spaces_reg("( +)");
            format_str = std::regex_replace(format_str, extra_spaces_reg, " ");

            return format_str;
        }

        bool check_amount_computers_format(std::string str) {
            std::regex regular("(0|[1-9]{1}[0-9]*)");
            return std::regex_match(str.c_str(), regular);
        }

        bool check_time_format(std::string _time) {
            std::regex regular("([0-1]{1}[0-9]{1}|2[0-3]{1})(:)([0-5]{1}[0-9]{1})");
            return std::regex_match(_time.c_str(), regular);   
        }

        bool check_earnings_per_hour_format(std::string str) {
            std::regex regular("(0|[1-9]{1}[0-9]*)");
            return std::regex_match(str.c_str(), regular);
        }

        bool check_client_name_format(std::string str) {
            std::regex regular("([\\w-]+)");
            return std::regex_match(str.c_str(), regular);
        }

        bool check_incoming_events(std::string str) {
            std::regex regular("([1-4]{1})");
            return std::regex_match(str.c_str(), regular);
        }

        bool check_computer_number(std::string str) {
            unsigned short computer_number = std::stoul(str, nullptr, 10);
            if (1 <= computer_number && computer_number <= this->computers_amount) {
                return true;
            } else {
                return false;
            }
        }

        bool in_work_hours(std::string str) {
            MyTime* incoming_time = new MyTime(str);
            if (incoming_time->hour == this->start_time->hour) {
                if (incoming_time->minute >= this->start_time->minute) {
                    return true;
                } else {
                    return false;
                }
            } 
            if (incoming_time->hour == this->end_time->hour) {
                if (incoming_time->minute <= this->end_time->minute) {
                    return true;
                } else {
                    return false;
                }
            }
            if (this->start_time->hour < incoming_time->hour && incoming_time->hour < this->end_time->hour) {
                return true;
            } else {
                return false;
            }

        }

        void fill_computers() {
            for (int i = 0; i < this->computers_amount; ++i) { 
                this->computers.push_back(new Computer(i + 1, this->earnings_per_hour));
            }
        }

        bool exist_empty_computer() {
            for (int i = 0; i < this->computers.size(); ++i) {
                if (this->computers[i]->is_used == false) {
                    return true;
                }
            }
            return false;
        }

        int find_client_on_computer(std::string client) {
            for (int i = 0; i < this->computers.size(); ++i) {
                if (this->computers[i]->client == client) {
                    return i;
                }
            }
            return this->computers.size();
        }

        void print_in_console(std::string str) {
            std::cout << str << std::endl;
        }

        void read_file(std::string file_name) {
            std::ifstream fin;
            fin.open(file_name); 

            if (!fin.is_open()) {
                std::cout << "Не удалось открыть файл " + file_name  << std::endl; // + file_name
                return;   
            } else {

                // ОБРАБОТКА ПЕРВОЙ СТРОКИ -- КОЛИЧЕСТВА КОМПЬЮТЕРОВ
                std::string computers_amount;
                std::getline(fin, computers_amount);
                // ====обработка лишних пробелов====
                std::string format_computers_amount = delete_extra_spaces(computers_amount);
                if (format_computers_amount.length() != computers_amount.length()) {
                    print_in_console(computers_amount);
                    return;
                }
                // ====обработка того, что считанное число -- целое, положительное, без ведущих нуле====
                if (check_amount_computers_format(computers_amount)) {
                    this->computers_amount = std::stoul(computers_amount, nullptr, 10);
                } else {
                    print_in_console(computers_amount);
                    return;
                }

                // ОБРАБОТКА ВТОРОЙ СТРОКИ -- ВРЕМЯ НАЧАЛА И ОКОНЧАНИЯ РАБОТЫ
                std::string start_end_time;
                std::getline(fin, start_end_time);
                std::string flexible_start_end_time = start_end_time;
                // ====обработка лишних пробелов====
                std::string format_start_end_time = delete_extra_spaces(start_end_time);
                if (format_start_end_time.length() != start_end_time.length()) {
                    print_in_console(start_end_time);
                    return;
                }
                // ====обработка случая, когда разделителем является не пробел====
                std::string delimiter = " ";
                unsigned short pos = flexible_start_end_time.find(delimiter);
                if (pos == std::string::npos) {
                    print_in_console(start_end_time);
                    return;
                }
                std::string start_time = flexible_start_end_time.substr(0, pos);
                flexible_start_end_time.erase(0, pos + delimiter.length());
                std::string end_time = flexible_start_end_time;
                // ====обработка корректности формата времени====
                if (check_time_format(start_time) && check_time_format(end_time)) {
                    this->start_time = new MyTime(start_time);
                    this->end_time = new MyTime(end_time);   
                } else {
                    print_in_console(start_end_time);
                    return;
                }

                // ОБРАБОТКА ТРЕТЬЕЙ СТРОКИ -- ПОЧАСОВОЙ ОПЛАТЫ ЗА КОМПЬЮТЕР
                std::string earnings_per_hour;
                std::getline(fin, earnings_per_hour);
                // ====обработка лишних пробелов====
                std::string format_earnings_per_hour = delete_extra_spaces(earnings_per_hour);
                if (format_earnings_per_hour.length() != earnings_per_hour.length()) {
                    print_in_console(earnings_per_hour);
                    return;
                }
                // ====обработка того, что считанное число -- целое, положительное, без ведущих нулей====
                if (check_earnings_per_hour_format(earnings_per_hour)) {
                    this->earnings_per_hour = std::stoul(earnings_per_hour, nullptr, 10);
                } else {
                    print_in_console(earnings_per_hour);
                    return;
                }

                // СОЗДАНИЕ КОМПЬЮТЕРОВ В КОМПЬЮТЕРНОМ КЛУБЕ
                fill_computers();

                std::cout << this->start_time->from_time_to_str() << std::endl;

                std::string incoming_events;
                // ОБРАБОТКА ОСНОВНЫХ ВХОДЯЩИХ СОБЫТИЙ
                while(std::getline(fin, incoming_events)) { //!fin.eof()
                    std::string flexible_incoming_events = incoming_events;
                    // ====обработка лишних пробелов====
                    std::string format_incoming_events = delete_extra_spaces(incoming_events);
                    if (format_incoming_events.length() != incoming_events.length()) {
                        print_in_console(incoming_events);
                        return;
                    }
                    // ====разбиваем входящую строку на токены, проверяя корректность разделителя====
                    std::string delimiter = " ";
                    size_t pos = 0;
                    std::vector<std::string> incoming_tokens;
                    std::string token;
                    for (short i = 0; i < 2; ++i) {
                        pos = flexible_incoming_events.find(delimiter);
                        if (pos == std::string::npos) {
                            print_in_console(incoming_events);
                            return;
                        }
                        token = flexible_incoming_events.substr(0, pos);
                        incoming_tokens.push_back(token);
                        flexible_incoming_events.erase(0, pos + delimiter.length());
                    }

                    // ====обработка корректности формата уже прочитанных данных(времени и id входящего события)====
                    if (!check_time_format(incoming_tokens[0])) {
                        print_in_console(incoming_events);
                        return;
                    } 
                    if (!check_incoming_events(incoming_tokens[1])) {
                        print_in_console(incoming_events);
                        return;
                    }

                    // ====продолжаем разбивать входящую строку на токены====
                    pos = flexible_incoming_events.find(delimiter);
                    if (incoming_tokens[1] != std::to_string(IncomingEvent::ID2)) {
                        incoming_tokens.push_back(flexible_incoming_events);
                    } else if (incoming_tokens[1] == std::to_string(IncomingEvent::ID2) && pos == std::string::npos) {
                        print_in_console(incoming_events);
                        return;
                    } else if (incoming_tokens[1] == std::to_string(IncomingEvent::ID2) && pos != std::string::npos) {
                        token = flexible_incoming_events.substr(0, pos);
                        incoming_tokens.push_back(token);
                        flexible_incoming_events.erase(0, pos + delimiter.length());
                        incoming_tokens.push_back(flexible_incoming_events);
                    }

                    std::string incoming_event_time;
                    unsigned short incoming_event_id;
                    std::string client_name;
                    unsigned short computer_number=0;

                    // ====обработка корректности формата оставшихся данных(имени клиента, номера компьютера)====
                    if (!check_client_name_format(incoming_tokens[2])) {
                        print_in_console(incoming_events);
                        return;   
                    }
                    if (incoming_tokens[1] == std::to_string(IncomingEvent::ID2) && !check_computer_number(incoming_tokens[3])) {
                        print_in_console(incoming_events);
                        return;      
                    } else if (incoming_tokens[1] == std::to_string(IncomingEvent::ID2) && check_computer_number(incoming_tokens[3])) {
                        computer_number = std::stoul(incoming_tokens[3], nullptr, 10);
                    }
                    incoming_event_time = incoming_tokens[0];
                    incoming_event_id = std::stoul(incoming_tokens[1], nullptr, 0);
                    client_name = incoming_tokens[2];

                    // СРАВНЕНИЕ ВРЕМЕНИ ТУКУЩЕГО СОБЫТИЯ С ВРЕМЕНЕМ ПРЕДЫДУЩЕГО
                    if (this->time_incoming_events == nullptr) {
                        this->time_incoming_events = new MyTime(incoming_event_time);
                    } else {
                        MyTime* current_time = new MyTime(incoming_event_time);
                        if (this->time_incoming_events->hour == current_time->hour) {
                            if (this->time_incoming_events->minute <= current_time->minute) {
                                delete this->time_incoming_events;
                                this->time_incoming_events = new MyTime(*current_time);
                                delete current_time; 
                            } else {
                                print_in_console(incoming_events);
                                return;
                            }
                        } else if(this->time_incoming_events->hour < current_time->hour) {
                            delete this->time_incoming_events;
                            this->time_incoming_events = new MyTime(*current_time);
                            delete current_time;
                        } else {
                            print_in_console(incoming_events);
                            return; 
                        }
                    }

                    std::cout << incoming_events << std::endl;

                    // ГЕНЕРАЦИЯ ИСХОДЯЩИХ СОБЫТИЙ
                    switch (incoming_event_id) {

                    case IncomingEvent::ID1:
                        if (!in_work_hours(incoming_event_time)) {
                            std::cout << incoming_event_time + " " << std::to_string(OutgoingEvents::ID13) << " NotOpenYet" << std::endl;
                        } else if (!clients_in_club.contains(client_name)) {
                            clients_in_club.insert(client_name);
                            clients_deque.push_back(client_name);
                        } else if (clients_in_club.contains(client_name)) {
                            std::cout << incoming_event_time + " " << std::to_string(OutgoingEvents::ID13) << " YouShallNotPass" << std::endl;
                        } 
                        break;

                    case IncomingEvent::ID2:
                        if (!clients_in_club.contains(client_name)) {
                            std::cout << incoming_event_time + " " << std::to_string(OutgoingEvents::ID13) << " ClientUnknown" << std::endl;
                        } else {
                            if (computers[computer_number - 1]->is_used == false) {
                                int past_client_computer = find_client_on_computer(client_name);
                                if (past_client_computer == computers.size()) {
                                    // В СЛУЧАЕ, ЕСЛИ ДО ЭТОГО КЛИЕНТ НЕ СИДЕЛ ЗА КОМПЬЮТЕРОМ
                                    for (auto it = clients_deque.begin(); it != clients_deque.end(); ++it) {
                                        if (*it == client_name) {
                                            clients_deque.erase(it);
                                            break;
                                        }
                                    } 

                                    computers[computer_number - 1]->is_used = true;
                                    computers[computer_number - 1]->client = client_name;
                                    computers[computer_number - 1]->start_time = new MyTime(incoming_event_time);
                                } else if (computers[past_client_computer]->computer_number != computer_number) {
                                    // В СЛУЧАЕ, ЕСЛИ ДО ЭТОГО КЛИЕНТ СИДЕЛ ЗА КОМПЬЮТЕРОМ
                                    computers[past_client_computer]->is_used = false;
                                    computers[past_client_computer]->client = "";
                                    computers[past_client_computer]->end_time = new MyTime(incoming_event_time);

                                    computers[past_client_computer]->compute_working_time();
                                    computers[past_client_computer]->compute_earnings();
                                    
                                    delete computers[past_client_computer]->start_time;
                                    delete computers[past_client_computer]->end_time;
                                    computers[past_client_computer]->start_time = nullptr;
                                    computers[past_client_computer]->end_time = nullptr;
                                    
                                    computers[computer_number - 1]->is_used = true;
                                    computers[computer_number - 1]->client = client_name;
                                    computers[computer_number - 1]->start_time = new MyTime(incoming_event_time);
                                }
                            } else {
                                std::cout << incoming_event_time + " " << std::to_string(OutgoingEvents::ID13) << " PlaceIsBusy" << std::endl; 
                            }
                            break;

                    case IncomingEvent::ID3:
                        if (!clients_in_club.contains(client_name)) {
                            std::cout << incoming_event_time + " " << std::to_string(OutgoingEvents::ID13) << " ClientUnknown" << std::endl;
                        } else if (exist_empty_computer()) {
                            std::cout << incoming_event_time + " " << std::to_string(OutgoingEvents::ID13) << " ICanWaitNoLonger!" << std::endl; 
                        } else if (clients_deque.size() > this->computers_amount) {
                            clients_in_club.erase(client_name);
                            clients_deque.pop_back();
                            std::cout << incoming_event_time + " " << std::to_string(OutgoingEvents::ID11) << " " << client_name << std::endl;
                        }
                        break;

                    case IncomingEvent::ID4:
                        if (!clients_in_club.contains(client_name)) {
                            std::cout << incoming_event_time + " " << std::to_string(OutgoingEvents::ID13) << " ClientUnknown" << std::endl;
                        } else {
                            clients_in_club.erase(client_name);
                            int computer_client = find_client_on_computer(client_name);
                            if (computer_client != computers.size()) {
                                // В СЛУЧАЕ, ЕСЛИ УХОДЯЩИЙ КЛИЕНТ СИДЕЛ ЗА КОМПЬЮТЕРОМ
                                computers[computer_client]->is_used = false;
                                computers[computer_client]->client = "";
                                computers[computer_client]->end_time = new MyTime(incoming_event_time);
                                
                                computers[computer_client]->compute_working_time();
                                computers[computer_client]->compute_earnings();

                                delete computers[computer_client]->start_time;
                                delete computers[computer_client]->end_time;
                                computers[computer_client]->start_time = nullptr;
                                computers[computer_client]->end_time = nullptr;

                                if (!clients_deque.empty()) {
                                    std::string first_client = clients_deque.front();
                                    clients_deque.pop_front();
                                    computers[computer_client]->is_used = true;
                                    computers[computer_client]->client = first_client;
                                    computers[computer_client]->start_time = new MyTime(incoming_event_time);
                                    std::cout << incoming_event_time + " " << std::to_string(OutgoingEvents::ID12) << " " << first_client << " " << std::to_string(computer_client + 1) << std::endl;
                                }
                            } else {
                                // В СЛУЧАЕ, ЕСЛИ КЛИЕНТ ПРОСТО ПРИШЕЛ И УШЕЛ
                                for (auto it = clients_deque.begin(); it != clients_deque.end(); ++it) {
                                    if (*it == client_name) {
                                        clients_deque.erase(it);
                                        break;
                                    }
                                } 
                            }
                            
                        }
                        break;
                        
                    default:
                        break;
                    } // Конец switch

                } // Конец while

            } // Конец ветки else по файлу

            fin.close();

            // ВЫГОНЯЕМ ВСЕХ ОСТАВШИХСЯ ИЗ КЛУБА
            for (auto client : clients_in_club) {
                int computer_client = find_client_on_computer(client);
                if (computer_client != computers.size()) {
                    computers[computer_client]->is_used = false;
                    computers[computer_client]->client = "";
                    computers[computer_client]->end_time = new MyTime(*this->end_time);
                    
                    computers[computer_client]->compute_working_time();
                    computers[computer_client]->compute_earnings();

                    delete computers[computer_client]->start_time;
                    delete computers[computer_client]->end_time;
                    computers[computer_client]->start_time = nullptr;
                    computers[computer_client]->end_time = nullptr;
                }
                std::cout << this->end_time->from_time_to_str() << " " << std::to_string(OutgoingEvents::ID11) << " " << client << std::endl;
            }
            clients_in_club.clear();

            std::cout << this->end_time->from_time_to_str() << std::endl;

            for (int i = 0; i < computers.size(); ++i) {
                std::cout << this->computers[i]->computer_number << " " << this->computers[i]->all_earnings << " " << computers[i]->working_time->from_time_to_str() << std::endl;
            }

        } // Конец процедуры
    }
};
