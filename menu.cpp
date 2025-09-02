#include <iostream>
#include "menu.h"

using namespace std;

void showMainMenu() {
    cout << "\n==============================\n";
    cout << "        MAIN MENU             \n";
    cout << "==============================\n";
    cout << "1. Book Management\n";
    cout << "2. Member Management\n";
    cout << "3. Borrow/Return Menu\n";
    cout << "0. Exit\n";
    cout << "Enter choice: ";
}

void showBookMenu() {
    cout << "\n--------- BOOK MENU ---------\n";
    cout << "1. Add Book\n";
    cout << "2. View All Books\n";
    cout << "3. Search Book\n";
    cout << "4. Update Book\n";
    cout << "5. Delete Book\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter choice: ";
}

void showMemberMenu() {
    cout << "\n--------- MEMBER MENU ---------\n";
    cout << "1. Add Member\n";
    cout << "2. View All Members\n";
    cout << "3. Search Member\n";
    cout << "4. Update Member\n";
    cout << "5. Delete Member\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter choice: ";
}

void showBorrowMenu() {
    cout << "\n--------- BORROW/RETURN MENU ---------\n";
    cout << "1. Issue Book\n";
    cout << "2. Return Book\n";
    cout << "3. View Borrowed Books\n";
    cout << "4. Check Fines\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter choice: ";
}