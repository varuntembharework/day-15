#include "utils.h"
#include <iostream>
#include <limits>
#include <iomanip>

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getInt(const std::string& prompt) {
    int value;
    for (;;) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputBuffer();
            return value;
        } else {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            clearInputBuffer();
        }
    }
}

std::string getLine(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void printHeader(const std::string& title) {
    std::cout << "\n============================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "============================================\n";
}