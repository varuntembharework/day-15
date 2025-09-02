# 📚 Library-Management-System_CPP

A **console-based Library Management System** built in **C++** for managing books, members, borrowing/returning, and fines.  
This project demonstrates **modular design, file-based persistence, and clean C++ code structure**.  
Perfect for showcasing as a **portfolio project**.

---

## ✨ Features

✅ **User Authentication**
- Default admin account: `admin / admin123`
- Easy to extend with librarians or members

✅ **Book Management**
- Add, view, search, update, delete books
- Availability status automatically updated

✅ **Member Management**
- Add, view, search, update, delete members
- Track number of borrowed books

✅ **Borrow & Return System**
- Issue books with due dates
- Return books and calculate fines (per late day)
- View all borrowed books and pending fines

✅ **File-Based Storage**
- Data is saved in plain text files:
  - `users.txt`
  - `books.txt`
  - `members.txt`
  - `borrow.txt`

---

## 🛠️ Tech Stack

- **Language**: C++17  
- **Paradigm**: Procedural + Modular  
- **Persistence**: File I/O (`fstream`)  
- **Platform**: Cross-platform (Windows/Linux/macOS)

---

## 🚀 Getting Started

### 1️⃣ Clone the Repository
```bash
git clone https://github.com/your-username/Library-Management-System_CPP.git
cd Library-Management-System_CPP

### 2️⃣ Compile the Project
On Windows (MinGW):
g++ -std=c++17 -o library.exe main.cpp menu.cpp utils.cpp book.cpp member.cpp borrow.cpp user.cpp
library.exe

---

## 🔑 Default Login

Username: admin
Password: admin123

---

## 📂 Project Structure

Library-Management-System_CPP/
│── book.h / book.cpp        # Book management
│── member.h / member.cpp    # Member management
│── borrow.h / borrow.cpp    # Borrow & return system
│── user.h / user.cpp        # User authentication
│── utils.h / utils.cpp      # Utility functions
│── menu.h / menu.cpp        # Menu system
│── constants.h              # Global constants
│── main.cpp                 # Entry point
