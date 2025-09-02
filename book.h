#ifndef BOOK_H
#define BOOK_H

#include <string>

// Book structure
struct Book {
    int id;
    std::string title;
    std::string author;
    bool available; // true = available, false = issued
};

// Book management
void addBook();
void viewBooks();
void searchBook();
void updateBook();
void deleteBook();

// Utilities
Book parseBookLine(const std::string& line);
bool bookExists(int id);

// Used by borrow.cpp (toggle semantics to match current model)
bool isBookAvailable(int id);
void updateBookQuantity(int id, int delta); // delta < 0 => mark unavailable, >0 => available

#endif