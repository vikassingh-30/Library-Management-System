Library Management System
https://img.shields.io/badge/c++-%252300599C.svg?style=for-the-badge&logo=c%252B%252B&logoColor=white

A comprehensive console-based Library Management System implemented in C++ that handles book management, user accounts, borrowing/returning operations, and fine calculations.

Features
User Management
Three user roles: Students, Faculty, and Librarians

Registration system with password authentication

Role-specific privileges:

Students: Borrow up to 3 books for 15 days

Faculty: Borrow up to 5 books for 30 days

Librarians: Full administrative access

Book Operations
Add/remove books with validation

Modify book details (title, author, ISBN, etc.)

Track book status (Available, Borrowed, Reserved)

Display all books in the library

Transaction System
Borrow and return books with date tracking

Automatic fine calculation (10₹/day for students)

View borrowing history and current borrowed books

Fine payment system

Data Persistence
Save/load books to books.txt

Save/load user accounts to:

students.txt

faculty.txt

librarians.txt
