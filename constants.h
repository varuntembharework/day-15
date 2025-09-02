#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// File names for persistent storage (single source of truth)
constexpr const char* BOOK_FILE   = "books.txt";
constexpr const char* MEMBER_FILE = "members.txt";
constexpr const char* USER_FILE   = "users.txt";
constexpr const char* BORROW_FILE = "borrow.txt"; // unified

// Borrowing rules
constexpr int MAX_BORROW_LIMIT = 5;
constexpr int BORROW_DAYS      = 14; // default due period
constexpr int FINE_PER_DAY     = 2;  // fine amount per day late

#endif