#include "user.h"
#include "constants.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

static std::vector<User> users;

// Convert role enum to string
std::string roleToString(Role r) {
    switch (r) {
        case ADMIN: return "Admin";
        case LIBRARIAN: return "Librarian";
        case MEMBER: return "Member";
        default: return "Unknown";
    }
}

static User parseUserLine(const std::string& line) {
    User u{};
    std::stringstream ss(line);
    std::string idStr, uname, pass, roleStr;

    std::getline(ss, idStr, '|');
    std::getline(ss, uname, '|');
    std::getline(ss, pass, '|');
    std::getline(ss, roleStr, '|');

    if (idStr.empty() || uname.empty() || pass.empty() || roleStr.empty())
        throw std::runtime_error("Corrupted user record");

    u.id = std::stoi(idStr);
    u.username = uname;
    u.password = pass;
    u.role = static_cast<Role>(std::stoi(roleStr));
    return u;
}

// Load all users from file
void loadUsers() {
    users.clear();
    std::ifstream fin(USER_FILE);

    if (!fin) {
        // If no file exists, create default Admin
        std::ofstream fout(USER_FILE);
        fout << "1|admin|admin123|" << static_cast<int>(ADMIN) << "\n";
        users.push_back({1, "admin", "admin123", ADMIN});
        std::cout << "No users found. Default Admin created (username: admin, password: admin123).\n";
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        try {
            users.push_back(parseUserLine(line));
        } catch (...) {
            std::cout << "Warning: Skipping corrupted user record.\n";
        }
    }
}

// User login
bool login(User &loggedInUser) {
    std::string uname, pass;
    std::cout << "\n=== Login ===\n";
    std::cout << "Username: ";
    std::cin >> uname;
    std::cout << "Password: ";
    std::cin >> pass;

    for (auto &u : users) {
        if (u.username == uname && u.password == pass) {
            loggedInUser = u;
            std::cout << "\nLogin successful! Welcome, " << u.username
                      << " (" << roleToString(u.role) << ")\n";
            return true;
        }
    }

    std::cout << "Invalid username or password.\n";
    return false;
}