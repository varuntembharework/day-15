#ifndef MEMBER_H
#define MEMBER_H

#include <string>

struct Member {
    int id;
    std::string name;
    std::string contact;
    int borrowedCount; // how many books currently borrowed
};

// Member management functions
void addMember();
void viewMembers();
void searchMember();
void updateMember();
void deleteMember();

// Utilities
Member parseMemberLine(const std::string& line);
bool memberExists(int id);
bool isMemberValid(int id);

// Update borrowedCount by delta (+1 on issue, -1 on return)
bool updateMemberBorrowCount(int id, int delta);

#endif