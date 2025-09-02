#include "member.h"
#include "constants.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdio>

static void writeMemberLine(std::ofstream& out, const Member& m) {
    out << m.id << "|" << m.name << "|" << m.contact << "|" << m.borrowedCount << "\n";
}

// Parse line into Member struct
Member parseMemberLine(const std::string& line) {
    Member m{};
    std::size_t p1 = line.find('|');
    std::size_t p2 = (p1 == std::string::npos) ? std::string::npos : line.find('|', p1 + 1);
    std::size_t p3 = (p2 == std::string::npos) ? std::string::npos : line.find('|', p2 + 1);

    if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos)
        throw std::runtime_error("Corrupted member record");

    m.id            = std::stoi(line.substr(0, p1));
    m.name          = line.substr(p1 + 1, p2 - p1 - 1);
    m.contact       = line.substr(p2 + 1, p3 - p2 - 1);
    m.borrowedCount = std::stoi(line.substr(p3 + 1));
    return m;
}

// Check if member ID already exists
bool memberExists(int id) {
    std::ifstream fin(MEMBER_FILE);
    if (!fin) return false;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Member m = parseMemberLine(line);
            if (m.id == id) return true;
        } catch (...) {}
    }
    return false;
}

bool isMemberValid(int id) {
    return memberExists(id);
}

bool updateMemberBorrowCount(int id, int delta) {
    std::ifstream fin(MEMBER_FILE);
    if (!fin) return false;
    std::ofstream temp("temp.txt");
    if (!temp) return false;

    std::string line;
    bool updated = false;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Member m = parseMemberLine(line);
            if (m.id == id) {
                m.borrowedCount += delta;
                if (m.borrowedCount < 0) m.borrowedCount = 0; // clamp
                updated = true;
            }
            writeMemberLine(temp, m);
        } catch (...) {
            temp << line << "\n";
        }
    }
    fin.close(); temp.close();
    std::remove(MEMBER_FILE);
    std::rename("temp.txt", MEMBER_FILE);
    return updated;
}

// Add new member
void addMember() {
    Member m{};
    m.id = getInt("\nEnter Member ID: ");

    if (memberExists(m.id)) {
        std::cout << "Error: Member with ID " << m.id << " already exists.\n";
        return;
    }

    m.name    = getLine("Enter Name: ");
    m.contact = getLine("Enter Contact: ");
    m.borrowedCount = 0;

    std::ofstream fout(MEMBER_FILE, std::ios::app);
    if (!fout) {
        std::cout << "Error: Could not open members file.\n";
        return;
    }
    writeMemberLine(fout, m);
    std::cout << "Member added successfully!\n";
}

// View all members
void viewMembers() {
    std::ifstream fin(MEMBER_FILE);
    if (!fin) {
        std::cout << "No members available.\n";
        return;
    }

    printHeader("Member List");
    std::cout << std::left << std::setw(10) << "ID"
              << std::setw(25) << "Name"
              << std::setw(20) << "Contact"
              << std::setw(15) << "Borrowed" << "\n";
    std::cout << "------------------------------------------------------------\n";

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Member m = parseMemberLine(line);
            std::cout << std::left << std::setw(10) << m.id
                      << std::setw(25) << m.name
                      << std::setw(20) << m.contact
                      << std::setw(15) << m.borrowedCount << "\n";
        } catch (...) {}
    }
}

// Search member by ID
void searchMember() {
    int searchId = getInt("\nEnter Member ID to search: ");

    std::ifstream fin(MEMBER_FILE);
    if (!fin) {
        std::cout << "No members available.\n";
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            Member m = parseMemberLine(line);
            if (m.id == searchId) {
                printHeader("Member Details");
                std::cout << "ID: " << m.id
                          << "\nName: " << m.name
                          << "\nContact: " << m.contact
                          << "\nBorrowed Books: " << m.borrowedCount << "\n";
                return;
            }
        } catch (...) {}
    }
    std::cout << "Member not found.\n";
}

// Update member details
void updateMember() {
    int updateId = getInt("\nEnter Member ID to update: ");

    std::ifstream fin(MEMBER_FILE);
    if (!fin) {
        std::cout << "No members available.\n";
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
            Member m = parseMemberLine(line);

            if (m.id == updateId) {
                std::string newName = getLine("Enter new Name (leave blank to keep \"" + m.name + "\"): ");
                if (!newName.empty()) m.name = newName;

                std::string newContact = getLine("Enter new Contact (leave blank to keep \"" + m.contact + "\"): ");
                if (!newContact.empty()) m.contact = newContact;

                updated = true;
            }
            writeMemberLine(temp, m);
        } catch (...) {
            temp << line << "\n";
        }
    }

    fin.close(); temp.close();
    std::remove(MEMBER_FILE);
    std::rename("temp.txt", MEMBER_FILE);

    std::cout << (updated ? "Member updated successfully!\n" : "Member not found.\n");
}

// Delete member
void deleteMember() {
    int deleteId = getInt("\nEnter Member ID to delete: ");

    std::ifstream fin(MEMBER_FILE);
    if (!fin) {
        std::cout << "No members available.\n";
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
            Member m = parseMemberLine(line);
            if (m.id != deleteId) {
                writeMemberLine(temp, m);
            } else {
                deleted = true;
            }
        } catch (...) {
            temp << line << "\n";
        }
    }

    fin.close(); temp.close();
    std::remove(MEMBER_FILE);
    std::rename("temp.txt", MEMBER_FILE);

    std::cout << (deleted ? "Member deleted successfully!\n" : "Member not found.\n");
}