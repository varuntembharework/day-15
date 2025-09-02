#include <iostream>
#include "menu.h"
#include "user.h"
#include "book.h"
#include "member.h"
#include "borrow.h"
#include "utils.h"

using namespace std;

int main() {
    // Load users and login
    loadUsers();
    User loggedInUser;
    if (!login(loggedInUser)) {
        cout << "Exiting program...\n";
        return 0;
    }

    int mainChoice;
    do {
        showMainMenu();
        cin >> mainChoice;
        clearInputBuffer();

        switch (mainChoice) {
            case 1: { // Book Menu
                int bookChoice;
                do {
                    showBookMenu();
                    cin >> bookChoice;
                    clearInputBuffer();

                    switch (bookChoice) {
                        case 1: addBook(); break;
                        case 2: viewBooks(); break;
                        case 3: searchBook(); break;
                        case 4: updateBook(); break;
                        case 5: deleteBook(); break;
                        case 0: cout << "Returning to main menu...\n"; break;
                        default: cout << "Invalid choice.\n";
                    }
                } while (bookChoice != 0);
                break;
            }
            case 2: { // Member Menu
                int memberChoice;
                do {
                    showMemberMenu();
                    cin >> memberChoice;
                    clearInputBuffer();

                    switch (memberChoice) {
                        case 1: addMember(); break;
                        case 2: viewMembers(); break;
                        case 3: searchMember(); break;
                        case 4: updateMember(); break;
                        case 5: deleteMember(); break;
                        case 0: cout << "Returning to main menu...\n"; break;
                        default: cout << "Invalid choice.\n";
                    }
                } while (memberChoice != 0);
                break;
            }
            case 3: { // Borrow Menu
                int borrowChoice;
                do {
                    showBorrowMenu();
                    cin >> borrowChoice;
                    clearInputBuffer();

                    switch (borrowChoice) {
                        case 1: issueBook(); break;
                        case 2: returnBook(); break;
                        case 3: viewBorrowedBooks(); break;
                        case 4: checkFines(); break;
                        case 0: cout << "Returning to main menu...\n"; break;
                        default: cout << "Invalid choice.\n";
                    }
                } while (borrowChoice != 0);
                break;
            }
            case 0:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (mainChoice != 0);

    return 0;
}