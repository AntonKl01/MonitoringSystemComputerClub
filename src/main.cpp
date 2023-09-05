#include "../include/ComputerClub.h"

int main(int argc, char* argv[]) {
    if (argc == 2) {
        ComputerClub computer_club;
        computer_club.read_file(std::string(argv[1]));
    } else {
        std::cout << "Введен более или менее чем один тестовый файл" << std::endl;
    }
    return 0;
}

