#ifndef UTILS_H
#define UTILS_H

#include <string>

// Clear the input buffer (for cin issues)
void clearInputBuffer();

// Pause execution until user presses Enter
void waitForEnter();

// Get integer input safely (with validation)
int getInt(const std::string& prompt);

// Get string input safely (with spaces allowed)
std::string getLine(const std::string& prompt);

// Display a section header with formatting
void printHeader(const std::string& title);

#endif