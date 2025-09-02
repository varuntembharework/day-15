#include "book.h"
#include "constants.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdio>

// Parse line into Book struct (id|title|author|0/1)
Book parseBookLine(const std::string& line) {
    Book b{};
    std::size_t p1 = line.find('|');
    std::size_t p2 = (p1 == std::string::npos) ? std::string::npos : line.find('|', p1 + 1);
    std::size_t p3 = (p2 == std::string::npos) ? std::string::npos : line.find('|', p2 + 1);

    if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos)
        throw std::runtime_error("Corrupted book record");

    b.id        = std::stoi(line.substr(0, p1));
    b.title     = line.substr(p1 + 1, p2 - p1 - 1);
    b.author    = line.substr(p2 + 1, p3 - p2 - 1);
    b.available = (line.substr(p3 + 1) == "1");
    return b;
}

static void writeBookLine(std::ofstream& out, const Book& b) {
    out << b.id << "|" << b.title << "|" << b.author << "|" << (b.available ? "1" : "0") << "\n";
}

// Check if book ID already exists
bool bookExists(int id) {
    std::ifstream fin(BOOK_FILE);
    if (!fin) return false;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Book b = parseBookLine(line);
            if (b.id == id) return true;
        } catch (...) {
            // skip bad line
        }
    }
    return false;
}

// Is book available (exists and available flag is true)
bool isBookAvailable(int id) {
    std::ifstream fin(BOOK_FILE);
    if (!fin) return false;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Book b = parseBookLine(line);
            if (b.id == id) return b.available;
        } catch (...) {}
    }
    return false;
}

// Update availability using +/- delta semantics
void updateBookQuantity(int id, int delta) {
    std::ifstream fin(BOOK_FILE);
    if (!fin) {
        std::cout << "Error: books file not found.\n";
        return;
    }
    std::ofstream temp("temp.txt");
    if (!temp) {
        std::cout << "Error: Could not create temp file.\n";
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Book b = parseBookLine(line);
            if (b.id == id) {
                b.available = (delta > 0); // simple toggle model
            }
            writeBookLine(temp, b);
        } catch (...) {
            // keep original line if corrupted
            temp << line << "\n";
        }
    }
    fin.close(); temp.close();
    std::remove(BOOK_FILE);
    std::rename("temp.txt", BOOK_FILE);
}

// Add new book
void addBook() {
    Book b{};
    b.id = getInt("\nEnter Book ID: ");

    if (bookExists(b.id)) {
        std::cout << "Error: Book with ID " << b.id << " already exists.\n";
        return;
    }

    b.title  = getLine("Enter Title: ");
    b.author = getLine("Enter Author: ");
    b.available = true;

    std::ofstream fout(BOOK_FILE, std::ios::app);
    if (!fout) {
        std::cout << "Error: Could not open books file.\n";
        return;
    }
    writeBookLine(fout, b);
    std::cout << "Book added successfully!\n";
}

// View all books
void viewBooks() {
    std::ifstream fin(BOOK_FILE);
    if (!fin) {
        std::cout << "No books available.\n";
        return;
    }

    printHeader("Book List");
    std::cout << std::left << std::setw(10) << "ID"
              << std::setw(30) << "Title"
              << std::setw(25) << "Author"
              << std::setw(15) << "Status" << "\n";
    std::cout << "------------------------------------------------------------------\n";

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Book b = parseBookLine(line);
            std::cout << std::left << std::setw(10) << b.id
                      << std::setw(30) << b.title
                      << std::setw(25) << b.author
                      << std::setw(15) << (b.available ? "Available" : "Issued") << "\n";
        } catch (...) {}
    }
}

// Search book by ID
void searchBook() {
    int searchId = getInt("\nEnter Book ID to search: ");

    std::ifstream fin(BOOK_FILE);
    if (!fin) {
        std::cout << "No books available.\n";
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Book b = parseBookLine(line);
            if (b.id == searchId) {
                printHeader("Book Details");
                std::cout << "ID: " << b.id
                          << "\nTitle: " << b.title
                          << "\nAuthor: " << b.author
                          << "\nStatus: " << (b.available ? "Available" : "Issued") << "\n";
                return;
            }
        } catch (...) {}
    }
    std::cout << "Book not found.\n";
}

// Update book details
void updateBook() {
    int updateId = getInt("\nEnter Book ID to update: ");

    std::ifstream fin(BOOK_FILE);
    if (!fin) {
        std::cout << "No books available.\n";
        return;
    }
    std::ofstream temp("temp.txt");
    if (!temp) {
        std::cout << "Error: Could not create temp file.\n";
        return;
    }

    std::string line;
    bool updated = false;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Book b = parseBookLine(line);

            if (b.id == updateId) {
                std::string newTitle = getLine("Enter new Title (leave blank to keep \"" + b.title + "\"): ");
                if (!newTitle.empty()) b.title = newTitle;

                std::string newAuthor = getLine("Enter new Author (leave blank to keep \"" + b.author + "\"): ");
                if (!newAuthor.empty()) b.author = newAuthor;

                updated = true;
            }
            writeBookLine(temp, b);
        } catch (...) {
            temp << line << "\n";
        }
    }

    fin.close(); temp.close();
    std::remove(BOOK_FILE);
    std::rename("temp.txt", BOOK_FILE);

    std::cout << (updated ? "Book updated successfully!\n" : "Book not found.\n");
}

// Delete book
void deleteBook() {
    int deleteId = getInt("\nEnter Book ID to delete: ");

    std::ifstream fin(BOOK_FILE);
    if (!fin) {
        std::cout << "No books available.\n";
        return;
    }
    std::ofstream temp("temp.txt");
    if (!temp) {
        std::cout << "Error: Could not create temp file.\n";
        return;
    }

    std::string line;
    bool deleted = false;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Book b = parseBookLine(line);
            if (b.id != deleteId) {
                writeBookLine(temp, b);
            } else {
                deleted = true;
            }
        } catch (...) {
            temp << line << "\n";
        }
    }

    fin.close(); temp.close();
    std::remove(BOOK_FILE);
    std::rename("temp.txt", BOOK_FILE);

    std::cout << (deleted ? "Book deleted successfully!\n" : "Book not found.\n");
}