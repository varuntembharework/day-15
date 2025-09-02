#include "borrow.h"
#include "book.h"
#include "member.h"
#include "constants.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdio>

// ================= Utility Functions =================

static std::string pad2(int n) {
    std::ostringstream os;
    os << std::setw(2) << std::setfill('0') << n;
    return os.str();
}

// Get current date as string (YYYY-MM-DD)
static std::string currentDate() {
    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);
    std::ostringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << pad2(1 + ltm->tm_mon) << "-"
       << pad2(ltm->tm_mday);
    return ss.str();
}

// Calculate due date (BORROW_DAYS later)
static std::string calcDueDate() {
    std::time_t now = std::time(nullptr);
    now += static_cast<long>(BORROW_DAYS) * 24 * 60 * 60;
    std::tm *ltm = std::localtime(&now);
    std::ostringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << pad2(1 + ltm->tm_mon) << "-"
       << pad2(ltm->tm_mday);
    return ss.str();
}

// Calculate days between two dates (d2 - d1)
static int daysBetween(const std::string &d1, const std::string &d2) {
    std::tm t1{}; std::tm t2{};
    std::sscanf(d1.c_str(), "%d-%d-%d", &t1.tm_year, &t1.tm_mon, &t1.tm_mday);
    std::sscanf(d2.c_str(), "%d-%d-%d", &t2.tm_year, &t2.tm_mon, &t2.tm_mday);
    t1.tm_year -= 1900; t2.tm_year -= 1900;
    t1.tm_mon  -= 1;    t2.tm_mon  -= 1;
    std::time_t time1 = std::mktime(&t1);
    std::time_t time2 = std::mktime(&t2);
    return static_cast<int>(std::difftime(time2, time1) / (60 * 60 * 24));
}

static BorrowRecord parseBorrowRecord(const std::string &line) {
    std::stringstream ss(line);
    std::string bId, mId, issue, due, ret;
    std::getline(ss, bId, '|');
    std::getline(ss, mId, '|');
    std::getline(ss, issue, '|');
    std::getline(ss, due, '|');
    std::getline(ss, ret);

    BorrowRecord br{};
    br.bookId   = std::stoi(bId);
    br.memberId = std::stoi(mId);
    br.issueDate= issue;
    br.dueDate  = due;
    br.returned = (ret == "1");
    return br;
}

static void writeBorrowRecord(std::ofstream &fout, const BorrowRecord &br) {
    fout << br.bookId << "|"
         << br.memberId << "|"
         << br.issueDate << "|"
         << br.dueDate << "|"
         << (br.returned ? 1 : 0) << "\n";
}

static int activeBorrowCountForMember(int memberId) {
    std::ifstream fin(BORROW_FILE);
    if (!fin) return 0;
    int count = 0;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            BorrowRecord br = parseBorrowRecord(line);
            if (br.memberId == memberId && !br.returned) ++count;
        } catch (...) {}
    }
    return count;
}

// ================= Core Functions =================

void issueBook() {
    int bookId  = getInt("\nEnter Book ID to issue: ");
    int memberId= getInt("Enter Member ID: ");

    if (!isBookAvailable(bookId)) {
        std::cout << "Book not available or does not exist.\n";
        return;
    }
    if (!isMemberValid(memberId)) {
        std::cout << "Invalid Member ID.\n";
        return;
    }

    // Enforce borrow limit
    int current = activeBorrowCountForMember(memberId);
    if (current >= MAX_BORROW_LIMIT) {
        std::cout << "Borrow limit reached (" << MAX_BORROW_LIMIT << ").\n";
        return;
    }

    // Create borrow record
    BorrowRecord br{bookId, memberId, currentDate(), calcDueDate(), false};
    std::ofstream fout(BORROW_FILE, std::ios::app);
    if (!fout) {
        std::cout << "Error: cannot open borrow file.\n";
        return;
    }
    writeBorrowRecord(fout, br);

    // Update book availability (mark issued) and member count
    updateBookQuantity(bookId, -1);
    updateMemberBorrowCount(memberId, +1);

    std::cout << "Book issued successfully! Due date: " << br.dueDate << "\n";
}

void returnBook() {
    int bookId   = getInt("\nEnter Book ID to return: ");
    int memberId = getInt("Enter Member ID: ");

    std::ifstream fin(BORROW_FILE);
    if (!fin) {
        std::cout << "No borrow records found.\n";
        return;
    }
    std::ofstream temp("temp.txt");
    if (!temp) {
        std::cout << "Error: Could not create temp file.\n";
        return;
    }

    std::string line;
    bool returned = false;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            BorrowRecord br = parseBorrowRecord(line);
            if (br.bookId == bookId && br.memberId == memberId && !br.returned) {
                int lateDays = daysBetween(br.dueDate, currentDate());
                br.returned = true;
                writeBorrowRecord(temp, br);
                returned = true;

                updateBookQuantity(bookId, +1);
                updateMemberBorrowCount(memberId, -1);

                std::cout << "Book returned successfully.\n";
                if (lateDays > 0) {
                    std::cout << "Late by " << lateDays
                              << " days. Fine: Rs." << lateDays * FINE_PER_DAY << "\n";
                }
            } else {
                writeBorrowRecord(temp, br);
            }
        } catch (...) {
            temp << line << "\n";
        }
    }

    fin.close(); temp.close();
    std::remove(BORROW_FILE);
    std::rename("temp.txt", BORROW_FILE);

    if (!returned) std::cout << "Record not found or already returned.\n";
}

void viewBorrowedBooks() {
    std::ifstream fin(BORROW_FILE);
    if (!fin) {
        std::cout << "No borrowed records found.\n";
        return;
    }

    printHeader("Borrowed Books");
    std::cout << std::left << std::setw(10) << "BookID"
              << std::setw(10) << "MemberID"
              << std::setw(15) << "Issue Date"
              << std::setw(15) << "Due Date"
              << std::setw(10) << "Returned" << "\n";
    std::cout << "-------------------------------------------------------------\n";

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            BorrowRecord br = parseBorrowRecord(line);
            std::cout << std::left << std::setw(10) << br.bookId
                      << std::setw(10) << br.memberId
                      << std::setw(15) << br.issueDate
                      << std::setw(15) << br.dueDate
                      << std::setw(10) << (br.returned ? "Yes" : "No") << "\n";
        } catch (...) {}
    }
}

void checkFines() {
    std::ifstream fin(BORROW_FILE);
    if (!fin) {
        std::cout << "No records found.\n";
        return;
    }

    printHeader("Fines");
    std::string line;
    bool anyFine = false;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            BorrowRecord br = parseBorrowRecord(line);
            if (!br.returned) {
                int lateDays = daysBetween(br.dueDate, currentDate());
                if (lateDays > 0) {
                    std::cout << "Member ID " << br.memberId
                              << " owes Rs." << lateDays * FINE_PER_DAY
                              << " for Book ID " << br.bookId << "\n";
                    anyFine = true;
                }
            }
        } catch (...) {}
    }
    if (!anyFine) std::cout << "No fines pending.\n";
}

void borrowMenu() {
    int choice;
    do {
        std::cout << "\n===== Borrow Menu =====\n"
                  << "1. Issue Book\n"
                  << "2. Return Book\n"
                  << "3. View Borrowed Books\n"
                  << "4. Check Fines\n"
                  << "0. Back\n";
        choice = getInt("Enter choice: ");

        switch (choice) {
            case 1: issueBook(); break;
            case 2: returnBook(); break;
            case 3: viewBorrowedBooks(); break;
            case 4: checkFines(); break;
            case 0: std::cout << "Returning to main menu...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}