#ifndef BORROW_H
#define BORROW_H

#include <string>

struct BorrowRecord {
    int bookId;
    int memberId;
    std::string issueDate;
    std::string dueDate;
    bool returned;
};

void issueBook();
void returnBook();
void viewBorrowedBooks();
void checkFines();
void borrowMenu();

#endif