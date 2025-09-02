#ifndef USER_H
#define USER_H

#include <string>

enum Role { ADMIN = 0, LIBRARIAN = 1, MEMBER = 2 };

struct User {
    int id;
    std::string username;
    std::string password;
    Role role;
};

void loadUsers();
bool login(User &loggedInUser);
std::string roleToString(Role r);

#endif