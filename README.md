# Library-Management-System
A Library Management System built in C++ using OOP principles. Supports role-based actions for Students, Faculty, and Librarians including borrowing, returning, fine handling, and book/user management with persistent storage via file handling.

Library Management System (LMS) in C++: 

Overview:
This project implements a Library Management System in C++ and demonstrates key Object-Oriented Programming (OOP) concepts including abstraction, inheritance, polymorphism, and encapsulation. The system supports multiple user roles – Librarian, Student, and Professor – and persists data using CSV files. It includes features such as book borrowing, returning, fine calculation, reservation of unavailable books, and role-based management of users and book records.

Features and Functionality
User Roles and Permissions:

Librarian (Role 0):
Manage users: add new users (Professor, Student, or additional Librarian) or remove existing users.
Manage books: add new books, update existing book details, or remove books.
Student (Role 1):
Borrow up to 3 books at a time.
Maximum borrowing period of 15 days.
Fines applied at 10 rupees per day for overdue returns.
View borrowing history.
Pay fines when applicable.
Professor (Role 2, implemented as "Professor" in the code):
Borrow up to 5 books at a time.
Maximum borrowing period of 30 days.
No fines for overdue returns; however, borrowing is restricted if any book is overdue by more than 60 days.
View borrowing history.
Book Management:

Each book has attributes: ISBN, title, author, publisher, year, and status.
The status indicates if the book is available, unavailable, or reserved (using a convention that concatenates "Reserve" with the reserving user’s ID).
Books can be added, updated, or removed by the Librarian.
Borrowing, Returning, and Reservation:

Users can borrow books if they meet the role-based limits and if there are no outstanding fines (for Students).
When a book is unavailable, users are prompted with an option to reserve it.
Upon returning a book, the system calculates overdue days and applies fines (for Students).
Reservation status is updated so that a reserved book can be borrowed once returned.
Polymorphism:

The base class "User" declares virtual functions "borrowBook" and "returnBook".
These functions are overridden in the "Student" and "Professor" classes using the "override" keyword to provide role-specific behavior.
This design demonstrates runtime polymorphism, ensuring that the correct behavior is invoked based on the user’s role.
Password Management and Security:

All user accounts are initialized with a default password ("1").
On the first login, if the password is still set to "1", the user is prompted to change it to a more secure password.
This feature encourages better password practices, although in a real-world application passwords would be hashed rather than stored in plain text.
Input Validation:

Extensive input validation is implemented for all user inputs.
The program repeatedly prompts the user for correct input (e.g., ensuring integers are provided for IDs and menu choices, and validating single-character responses).
File I/O and Data Persistence:

CSV Files:
books_csv.csv: Contains all book records. The header line includes: ISBN, Title, Author, Publisher, Year, Status. Purpose: Stores the details of all books and their current status (e.g., Available, Unavailable, or Reservation information).
credentials.csv: Contains user login information. The header line includes: UserID, Name, Password, Fine, Role. Purpose: Holds login credentials and role information for all users (Role: 0 for Librarian, 1 for Student, 2 for Professor).
User-Specific Files: For each user, two CSV files are generated during runtime:
<UserID>borrowed.csv: Contains the current list of borrowed books along with the timestamp of borrowing.
<UserID>history.csv: Maintains a history of returned books with similar details as the borrowed file.
The system reads from these files upon startup and writes updates upon exit, ensuring that the library’s state is maintained across sessions.
Additional Features:

Reservation: If a desired book is unavailable, users have the option to reserve it. When the book is returned, if it is marked as reserved by the same user, it becomes available to them.
Clear Screen (Commented): The program contains ANSI escape codes to clear the screen; however, these have been commented out to ensure compatibility across different operating systems.
Robust Error Handling: Basic error checking is implemented for file operations (e.g., inability to open a file) and during CSV parsing.
Modular Code Structure: Although implemented in a single file for the assignment, the code is organized into classes for Book, User, Account, Student, Professor, and Librarian to reflect a clear separation of concerns.
System Initialization
First-Time Setup:

The "first_time()" function is responsible for initializing the system by creating CSV files with default data.   --> COMMENT AFTER FIRST RUN (LINE 652)
It creates:
10 book records with sample details (titles, authors, publication years, etc.).
User credentials for:
1 Librarian (UserID 0)
5 Students (UserIDs 1 to 5)
3 Professors (UserIDs 6 to 8)
Default Login Credentials:
Librarian: UserID 0, Password "1"
Student: UserID 1, Password "1"
Professor: UserID 6, Password "1"
IMPORTANT: After the initial run, you must comment out or remove the call to "first_time()" in the main() function to prevent overwriting the CSV files with default data.
Compiling the Software
Open a terminal or command prompt and navigate to the directory containing the source file "codingass.cpp".

Compile the code using a C++ compiler that supports C++11 or later. For example, if using g++:

g++ -std=c++11 -o LibrarySystem codingass.cpp

This command compiles the source code and produces an executable named "LibrarySystem".

Executing the Software
On Unix/Linux/MacOS: ./LibrarySystem

On Windows: ./LibrarySystem.exe

When the program is executed, it will:

Prompt for the UserID and password.
If the password is still the default ("1"), prompt the user to change it.
Display a menu based on the logged-in user’s role, allowing operations such as borrowing/returning books, paying fines, viewing borrowing history, and managing users or books (for Librarian).

Summary:

This Library Management System meets the assignment requirements by:

Demonstrating key OOP concepts (abstraction, inheritance, polymorphism, encapsulation).
Providing role-based functionality for different user types.
Persisting data using CSV files for books, user credentials, and borrowing history.
Implementing robust input validation and error handling.
Including additional features such as reservation, password management, and a clear modular design.
