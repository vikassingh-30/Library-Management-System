#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <bits/stdc++.h>

using namespace std;



#define DEBUG

#ifdef DEBUG
#define DBG() printf("%d\n", __LINE__)
#endif

#ifndef DEBUG
//#define DBG() ;
#endif

// Utility function to get current date in days
int getCurrentDate() {
    time_t now = time(0);
    return now / (60 * 60 * 24); // Convert to days
}

string formatDate(int date) {
    // Convert the integer to an 8-digit string (pad with leading zeros if necessary)
    ostringstream oss;
    oss << setw(8) << setfill('0') << date;
    string s = oss.str();

    // Extract day, month, year parts and insert slashes
    string formatted = s.substr(0, 2) + "/" + s.substr(2, 2) + "/" + s.substr(4, 4);
    return formatted;
}


time_t convertToTimeT(int date) {
    int day = date / 1000000;           // Extract first two digits (dd)
    int month = (date / 10000) % 100;   // Extract middle two digits (mm)
    int year = date % 10000;            // Extract last four digits (yyyy)

    struct tm tm = {0};  // Initialize struct to zero
    tm.tm_mday = day;
    tm.tm_mon = month - 1;  // Months are 0-based in struct tm
    tm.tm_year = year - 1900;  // Years since 1900

    return mktime(&tm); // Convert to time_t (seconds since epoch)
}

// Function to calculate difference in days
int daysDifference(int date1, int date2) {
    time_t t1 = convertToTimeT(date1);
    time_t t2 = convertToTimeT(date2);
    
    return abs(difftime(t1, t2) / (60 * 60 * 24));  // Convert seconds to days
}


// Book class
class Book {
public:
    int id;
    string title, author, publisher, isbn;
    int year;
    string status; // "Available", "Borrowed", "Reserved"

    Book(int id, string title, string author, string publisher, int year, string isbn,string status)
        : id(id), title(title), author(author), publisher(publisher), year(year), isbn(isbn), status("Available") {}

    void display() {
        cout << "Book ID: " << id << ", Title: " << title << ", Author: " << author 
             << ", Year: " << year << ", ISBN: " << isbn << ", Status: " << status << endl;
    }
};

// Account class
class Account {
public:
    int userId;
    vector<pair<int, int>> borrowedBooks; // <bookID, borrowedDate>
    int fine = 0;
    vector < tuple < int, int, int> > borrowed_History;
    //map <int, vector <string> > borrowed_History;

    Account(int id) : userId(id), fine(0) {}

    void borrowBook(int bookId,int date) {
        borrowedBooks.push_back({bookId, date});
    }

    void returnBook(int bookId, int maxDays, int fineRate) {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if (it->first == bookId) {
                int date = 0;
                cout << "Enter todays date in DD/MM/YYYY format without the slashes" << endl;
                
                while (!(cin >> date)) {
                    // If the input fails (e.g., a string was entered)
                    cout << "Invalid input. Please enter an integer: ";
                    cin.clear(); // Clear the error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                }

                if (date/1000000 > 31){
                    cout << "Invalid date entered (number of days in a month cannot be more than 31) \n";
                    return;
                }

                if ((date/10000)%100 > 12){
                    cout << "Invalid date entered (month number cannot be more than 12) \n";
                    return;
                }
                

                
                int borrowedDate = it->second;
                int daysBorrowed =  daysDifference(date,borrowedDate);
                if (daysBorrowed > maxDays) {
                    fine += (daysBorrowed - maxDays) * fineRate;
                }
                borrowed_History.push_back({bookId, borrowedDate , date});
                borrowedBooks.erase(it);
                return;
            }
        }
    }


    void payFine() {
        cout << "Outstanding fine: " << fine << " rupees." << endl;
        int amount = 0;
        cout << "Enter amount you want to pay: ";
        cin >> amount;
        if (amount > fine){
            cout << "Amount exceeds fine amount " << endl;
            return;
        }
        fine = fine - amount;
        cout << "Outstanding fine: " << fine << " rupees." << endl;
        if (fine == 0){
            cout << "You have paid all your fines\n";
            cout << "Good job :)\n";
        }
        return;
    }

    bool hasUnpaidFine() {
        return fine > 0;
    }

    

    void showBorrowedBooksDuration() {
        int today;
        cout << "Enter today's date in XX/XX/XXXX format without the slashes: ";
        cin >> today;

        cout << "Borrowed Books Duration: " << endl;
        for (auto &book : borrowedBooks) {
            int bookId = book.first;
            int borrowedDate = book.second;
            int days = daysDifference(today, borrowedDate);
            cout << "Book ID: " << bookId << " | Borrowed for: " << days << " days" << endl;
        }
    }

    bool canBorrowMoreBooksFaculty(int today) {

        for (auto &book : borrowedBooks) {
            int borrowedDate = book.second;
            if (daysDifference(today, borrowedDate) >= 60) {
                //cout << "You have a book that has been overdue for 60 days or more. Return it before borrowing a new book." << endl;
                return false;
            }
        }
        return true;
    }

    bool canBorrowMoreBooksStudent(int today) {

        for (auto &book : borrowedBooks) {
            int borrowedDate = book.second;
            if (daysDifference(today, borrowedDate) >= 15) {
                //cout << "You have a book that has been overdue for 60 days or more. Return it before borrowing a new book." << endl;
                return false;
            }
        }
        return true;
    }
};

// Base User class
class User {
protected:
    string name;
    int id;
    string passwd;
    Account account;
    int maxBooks;
    int maxDays;
    int fineRate;


public:
    User(string name, int id, string passwd,int maxBooks, int maxDays, int fineRate) 
        : name(name), id(id), passwd(passwd), account(id), maxBooks(maxBooks), maxDays(maxDays), fineRate(fineRate) {}

    virtual void display() {
        cout << "User ID: " << id << ", Name: " << name << endl;
    }

    int compare_passwd(const string& passwd2) const { // Marked as const
        return passwd == passwd2;
    }

    int compare_name(const string& name2) const { // Marked as const
        return name == name2;
    }

    int fineAmount(){
        return account.fine;
    }

    int getId() const { return id; }  // Getter for id

    string getPasswd() const { return passwd; }  // Getter for passwd

    string getName() const { return name; }  // Getter for name

    int getFine() const { return fineRate; }

    Account& getacc() {return account;}


    bool canBorrow(int date) {
        return (account.borrowedBooks.size() < maxBooks) && !account.hasUnpaidFine() && account.canBorrowMoreBooksStudent(date);
    }

    bool canBorrowFaculty(int date){
        return account.canBorrowMoreBooksFaculty(date) && account.borrowedBooks.size() <= 5;
    }

    void viewBorrowHistory(){
        for (const auto& t : account.borrowed_History) {
            cout << "Book Id: " << get<0>(t) << endl;
            cout << "Borrowed on: " << formatDate(get<1>(t)) << endl;
            cout << "Returned on: " << formatDate(get<2>(t)) << endl;
            cout << "\n";
        }
    }

    vector <tuple <int , int ,int > > & getBorrowHistory(){
        return account.borrowed_History;
    }

    void setBorrowHistory(vector <tuple <int,int,int> > & hist_loaded){
        account.borrowed_History = hist_loaded;
    }

    void borrowBk(Book &book,int date) {
        if (canBorrow(date) && book.status == "Available") {
            account.borrowBook(book.id,date);
            book.status = "Borrowed";
            cout << name << " borrowed the book: " << book.title << endl;
        } else {
            cout << "Cannot borrow book!" << endl;
        }
    }

    void returnBk(Book &book) {
        account.returnBook(book.id, maxDays, fineRate);
        book.status = "Available";
        cout << name << " returned the book: " << book.title << endl;
    }

    void checkFine() {
        cout << "Outstanding fine: " << account.fine << " rupees." << endl;
    }

    void payFine() {
        account.payFine();
    }

    void disp() {

        cout << "=================================================\n";
        if (account.borrowedBooks.empty()) {
            cout << "No books borrowed.\n";
        }
        else
        {
            cout << "Borrowed Books\n";
            for (auto &entry : account.borrowedBooks) {
                cout << "Book ID: " << entry.first << ", Borrowed on: " << formatDate(entry.second) << "\n";
            }
        }
        cout << "=================================================\n";
        return;
    }
           
};

// Student class
class Student : public User {
public:
    Student(string name, int id, string passwd) : User(name, id, passwd, 3, 15, 10) {}
};

// Faculty class
class Faculty : public User {
public:
    Faculty(string name, int id, string passwd) : User(name, id, passwd, 5, 30, 0) {} // No fine for faculty
};

// Librarian class
class Librarian : public User {
public:

    Librarian(string name, int id, string passwd) : User(name, id, passwd, 0, 0, 0){};

    void addBk(vector<Book> &books, int id, string title, string author, string publisher, int year, string isbn,string status) {
        books.push_back(Book(id, title, author, publisher, year, isbn, status));
        cout << "Book added successfully!" << endl;
    }

    void removeBook(vector<Book> &books, int bookId) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->id == bookId) {
                books.erase(it);
                cout << "Book removed successfully!" << endl;
                return;
            }
        }

        cout << "Book not found!" << endl;
    }

    void saveBooks(vector <Book> &books) {
        ofstream file("books.txt"); // Open in overwrite mode
        if (!file) {
            cout << "Error: Could not open books.txt for writing!" << endl;
            return;
        }

        for (auto &book : books) {
            file << book.id << "," << book.title << "," << book.author << "," 
                << book.publisher << "," << book.year << "," << book.isbn << "," 
                << book.status << endl;
        }

        file.close();
    }

};

// Library class
class Library {
private:

    vector<Book> books;
    vector<Student> students;
    vector<Faculty> faculty;
    vector<Librarian> librarians;
    Librarian librarian; // Admin Librarian

public:

    Library() : librarian("Admin", 0, "admin") {
        LoadUsers();
        LoadBooks();
    }
    ~Library() {
        saveUsers();
        saveBooks();
    }
    // Function to find a student by ID
    Student* findStudent(int id) {
        for (auto &student : students) {
            if (student.getId() == id) return &student;
        }
        return nullptr;
    }

    // Function to find a faculty by ID
    Faculty* findFaculty(int id) {
        for (auto &fac : faculty) {
            if (fac.getId() == id) return &fac;
        }
        return nullptr;
    }

    // Function to find a librarian by ID
    Librarian* findLibrarian(int id) {
        for (auto &lib : librarians) {
            if (lib.getId() == id) return &lib;
        }
        return nullptr;
    }

    void addBook() {
        int id, year;
        string title, author, publisher, isbn;
        string status;

        cout << "Enter book ID: " << endl;
        
        while (!(cin >> id)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
    
        // Check if ID already exists
        for (const auto &book : books) {
            if (book.id == id) {
                cout << "Error: A book with this ID already exists!\n";
                return;
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter title: " << endl;
        getline(cin, title);

        //cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter author: " << endl;
        getline(cin, author);

        //cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter publisher: " << endl;

        getline(cin, publisher);
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter year: " << endl;

        
        while (!(cin >> year)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter ISBN: " << endl;
        getline(cin,isbn);
    
        

        // Check if ISBN already exists
        for (const auto &book : books) {
            if (book.isbn == isbn) {
                cout << "Error: A book with this ISBN already exists!\n";
                return;
            }
        }
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter status of book (Available/Reserved): " << endl;
        getline(cin,status);

        librarian.addBk(books, id, title, author, publisher, year, isbn, status);
        librarian.saveBooks(books);
        //3
        cout << "Book added successfully!\n";
    }
    

    void removeBook() {
        int bookId;
        cout << "Enter Book ID to remove: ";
        cin >> bookId;

        // Check if ID already exists
        for (const auto &book : books) {
            if (book.id == bookId) {
                librarian.removeBook(books, bookId);
                librarian.saveBooks(books);
                
                return;
            }
        }
        cout << "Error: No book with the provided id has been found\n";
    }

    void displayBooks() {
        for (auto &book : books) {
            book.display();
        }
    }

    void displayStudents() {
        for (auto &uzer : students){
            cout << "Name : " << uzer.getName() << endl;
            cout << "Id : " << uzer.getId() << endl;
        }
    }

    void displayFaculty() {
        for (auto &uzer : faculty){
            cout << "Name : " << uzer.getName() << endl;
            cout << "Id : " << uzer.getId() << endl;
        }
    }


    void saveUsers() {
        ofstream studentFile("students.txt");
        ofstream facultyFile("faculty.txt");
        ofstream librarianFile("librarians.txt");

        if (!studentFile || !facultyFile || !librarianFile) {
            cout << "Error opening user files for writing!" << endl;
            return;
        }

        for (auto &student : students) {
            studentFile << student.getId() << " " << student.getName() << " " << student.getPasswd() << " " << student.fineAmount() << " |";
            if (student.getBorrowHistory().size() > 0){
                for (auto i: student.getBorrowHistory()){
                    studentFile << get<0>(i) << "|" << get<1>(i) << "|" << get<2>(i) << "|";
                }
                studentFile << endl;
            }
        }

        for (auto &fac : faculty) {
            facultyFile << fac.getId() << " " << fac.getName() << " " << fac.getPasswd() << " |" ;
            if (fac.getBorrowHistory().size() > 0){
                for (auto i: fac.getBorrowHistory()){
                    facultyFile << get<0>(i) << "|" << get<1>(i) << "|" << get<2>(i) << "|";
                }
                facultyFile << endl;
            }
        }

        for (auto &lib : librarians) {
            librarianFile << lib.getId() << " " << lib.getName() << " " << lib.getPasswd() << endl;
        }

        studentFile.close();
        facultyFile.close();
        librarianFile.close();
    }


    void LoadUsers() {
        ifstream studentFile("students.txt");
        ifstream facultyFile("faculty.txt");
        ifstream librarianFile("librarians.txt");
    
        int id;
        string name, passwd;
        int fine;
        
        // Load students with borrowing history
        if (studentFile) {
            string line;
            while (getline(studentFile, line)) {
                if (line.empty()) continue;
                // Create a stringstream from the line
                stringstream ss(line);
                // Read the basic fields (ID, name, passwd, fine)
                ss >> id >> name >> passwd >> fine;
                // Read the remainder of the line which contains the borrow history
                string historyPart;
                getline(ss, historyPart);
                // Remove any leading spaces
                if (!historyPart.empty() && historyPart[0] == ' ')
                    historyPart.erase(0, historyPart.find_first_not_of(" "));
                // Now historyPart is a string like: "101|20230101|20230115|102|20230201|0|"
                vector<string> tokens;
                string token;
                stringstream ts(historyPart);
                while(getline(ts, token, '|')) {
                    if(!token.empty())
                        tokens.push_back(token);
                }
                vector<tuple<int,int,int>> history;
                // Each record consists of three tokens, so iterate in steps of three
                for (size_t i = 0; i + 2 < tokens.size(); i += 3) {
                    int bookId = stoi(tokens[i]);
                    int borrowDate = stoi(tokens[i+1]);
                    int returnDate = stoi(tokens[i+2]);
                    history.push_back(make_tuple(bookId, borrowDate, returnDate));
                }
                // Create the student object and set its fields
                Student s(name, id, passwd);
                s.getacc().fine = fine;  // assuming getacc() returns a reference to the Account
                s.setBorrowHistory(history);  // User class must provide setBorrowHistory()
                // Add to the students vector
                students.push_back(s);
            }
        }
    
        // Load faculty similarly (assuming faculty file uses the same format)
        if (facultyFile) {
            string line;
            while (getline(facultyFile, line)) {
                if (line.empty()) continue;
                stringstream ss(line);
                ss >> id >> name >> passwd;
                string historyPart;
                getline(ss, historyPart);
                if (!historyPart.empty() && historyPart[0] == ' ')
                    historyPart.erase(0, historyPart.find_first_not_of(" "));
                vector<string> tokens;
                string token;
                stringstream ts(historyPart);
                while(getline(ts, token, '|')) {
                    if(!token.empty())
                        tokens.push_back(token);
                }
                vector<tuple<int,int,int>> history;
                for (size_t i = 0; i + 2 < tokens.size(); i += 3) {
                    int bookId = stoi(tokens[i]);
                    int borrowDate = stoi(tokens[i+1]);
                    int returnDate = stoi(tokens[i+2]);
                    history.push_back(make_tuple(bookId, borrowDate, returnDate));
                }
                Faculty f(name, id, passwd);
                f.getacc().fine = fine;
                f.setBorrowHistory(history);
                faculty.push_back(f);
            }
        }
    
        // Load librarians (no borrowing history expected)
        if (librarianFile) {
            while (librarianFile >> id >> name >> passwd) {
                librarians.emplace_back(name, id, passwd);
            }
        }
    }
    

    int addUser() {
        int id, role;
        string name, passwd, passwd2;

        cout << "Enter 1 for Student, 2 for Faculty, 3 for Librarian: ";
        //cin >> role;

        while (!(cin >> role)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }

        if (role < 1 || role > 3) {
            cout << "Invalid role!" << endl;
            return 0;
        }

        cout << "Enter User ID: ";
        //cin >> id;

        while (!(cin >> id)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }

        if (findStudent(id) || findFaculty(id) || findLibrarian(id)) {
            cout << "Error: User ID already exists!\n";
            return 0;
        }

        cout << "Enter Name (no spaces): ";
        cin >> name;
        cout << "Enter Password (no spaces): ";
        cin >> passwd;
        cout << "Reenter Password: ";
        cin >> passwd2;

        if (passwd != passwd2) {
            cout << "Passwords do not match." << endl;
            return 0;
        }

        if (role == 1) {
            students.emplace_back(name, id, passwd);
            Student *strrr = findStudent(id);
            (strrr->getacc()).fine=0;
        }
        else if (role == 2) faculty.emplace_back(name, id, passwd);
        else if (role == 3) librarians.emplace_back(name, id, passwd);

        cout << "User added successfully!" << endl;
        return 1;
    }

    void borrowBook(int date,int userId) {
        int bookId;
        cout << "Enter Book ID: ";
        //cin >> userId >> bookId;

        /*
        while (!(cin >> userId)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
        */

        while (!(cin >> bookId)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }

        User* user = findStudent(userId);
        if (!user) user = findFaculty(userId);
        if (!user) {
            cout << "User not found!" << endl;
            return;
        }

        for (auto &book : books) {
            if (book.id == bookId) {
                user->borrowBk(book,date);
                return;
            }
        }

        cout << "Book not found!" << endl;
    }

    void returnBook(int userId) {
        int bookId;
        cout << "Enter Book ID: ";
        //cin >> userId;

        /*
        while (!(cin >> userId)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
        */

        //cin >> bookId;

        while (!(cin >> bookId)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }

        User* user = findStudent(userId);
        if (!user) user = findFaculty(userId);
        if (!user) {
            cout << "User not found!" << endl;
            return;
        }

        for (auto &book : books) {
            if (book.id == bookId) {
                for (auto i: (user->getacc()).borrowedBooks){
                    if (i.first == book.id){
                        user->returnBk(book);
                        return;
                    }
                }
                cout << "Book has not been borrowed by you " << endl;
                return;
            }
        }

        cout << "Book not found!" << endl;
    }

    void saveBooks() {
        ofstream file("books.txt");
        if (!file) {
            cout << "Error opening books.txt for writing!" << endl;
            return;
        }

        for (auto &book : books) {
            file << book.id << ","
                 << book.title << ","
                 << book.author << ","
                 << book.publisher << ","
                 << book.year << ","
                 << book.isbn << ","
                 << book.status << endl;
        }

        file.close();
    }

    void LoadBooks() {
        ifstream inFile("books.txt");

        if (!inFile) {
            cerr << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string id, year;
            string title, author, publisher, isbn, status;
            getline(ss,id,',');
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, publisher, ',');
            getline(ss, year, ',');
            getline(ss, isbn, ',');
            getline(ss, status, ',');

            books.emplace_back(stoi(id), title, author, publisher, stoi(year), isbn, status);
        }

        inFile.close();
    }

    void modifyBookDetails() {
        int bookId;
        cout << "Enter the Book ID of the book you want to modify: ";
        //cin >> bookId;
        
        while (!(cin >> bookId)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
    
        // Find the book by ID
        for (auto &book : books) {
            if (book.id == bookId) {
                cout << "\nCurrent Book Details:\n";
                book.display();
    
                cout << "\nEnter new details (leave blank to keep unchanged):\n";
    
                string newTitle, newAuthor, newPublisher, newIsbn, newStatus;
                int newYear;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "New Title: " << endl;
                getline(cin, newTitle);
                cout << "New Author: " << endl;
                getline(cin, newAuthor);
                cout << "New Publisher: " << endl;
                getline(cin, newPublisher);
                cout << "New Year: " << endl;
                string yearInput;
                getline(cin, yearInput);
                cout << "New ISBN: " << endl;
                getline(cin, newIsbn);
                cout << "New Status (Available/Borrowed): " << endl;
                getline(cin, newStatus);
    
                // Apply only non-empty inputs
                if (!newTitle.empty()) book.title = newTitle;
                if (!newAuthor.empty()) book.author = newAuthor;
                if (!newPublisher.empty()) book.publisher = newPublisher;
                if (!yearInput.empty()) book.year = stoi(yearInput);
                if (!newIsbn.empty()) book.isbn = newIsbn;
                if (!newStatus.empty()) book.status = newStatus;
                
                cout << "\nBook details updated successfully!\n";
    
                // Save updated book list to file
                librarian.saveBooks(books);
                return;
            }
        }
    
        cout << "Book with ID " << bookId << " not found.\n";
    }
    
    void deleteStudent() {
        int studentId;
        cout << "Enter the Student ID to delete: ";
        //cin >> studentId;

        while (!(cin >> studentId)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
    
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getId() == studentId) {
                if (it->getacc().borrowedBooks.size() == 0){
                    students.erase(it);
                    cout << "Student with ID " << studentId << " deleted successfully!\n";
                    saveUsers();  // Save updated student list
                    return;
                }
                else{
                    cout << "The selected student has borrowed books from the library\n";
                    cout << "Hence, deletion is prohibited\n";
                    return;
                }
            }
        }
        cout << "Student with ID " << studentId << " not found.\n";
    }
    
    void deleteFaculty() {
        int facultyId;
        cout << "Enter the Faculty ID to delete: ";
        //cin >> facultyId;

        while (!(cin >> facultyId)) {
            // If the input fails (e.g., a string was entered)
            cout << "Invalid input. Please enter an integer: ";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
    
        for (auto it = faculty.begin(); it != faculty.end(); ++it) {
            if (it->getId() == facultyId) {
                if (it->getacc().borrowedBooks.size() == 0){
                    faculty.erase(it);
                    cout << "Faculty with ID " << facultyId << " deleted successfully!\n";
                    saveUsers();  // Save updated faculty list
                    return;
                }
                else{
                    cout << "The selected faculty has borrowed books from the library\n";
                    cout << "Hence, deletion is prohibited\n";
                    return;
                }
            }
        }
        cout << "Faculty with ID " << facultyId << " not found.\n";
    }
    

    User* login(string name, int id, string passwd) {
        User* user = findStudent(id);
        if (!user) user = findFaculty(id);
        if (!user) user = findLibrarian(id);

        if (user && user->compare_passwd(passwd) && user->compare_name(name)) {
            return user;
        }

        return nullptr;
    }
};


// Main function
int main() {
    Library lib;
    int role;
    string nw;
    int id;
    //lib.LoadBooks();
    //DBG();
    User * user;
    while(1){
        //DBG();
        cout << "Welcome to the Library Management system\n ";
        cout << "Are you already registered? (Y/N) "<< endl;
        cin >> nw;

        if (nw == "N" || nw == "n" || nw == "NO" || nw == "no"){
            lib.addUser(); 
            cout << "Please login again using your new credentials to perform any task " << endl;
            continue;
        }
        else if (nw == "Y" || nw == "y" || nw == "YES" || nw == "yes"){
            cout << "Proceeding to login page " << endl;

            string passwd;
            string name;

            cout << "Enter your id " << endl;
            //cin >> id;

            while (!(cin >> id)) {
                // If the input fails (e.g., a string was entered)
                cout << "Invalid input. Please enter an integer: ";
                cin.clear(); // Clear the error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            }

            cout << "Enter your name " << endl;
            cin >> name;

            cout << "Enter password " << endl;
            cin >> passwd;

            user = lib.login(name,id,passwd);

            if (user != nullptr){
                cout << "You have successfully logged in" << endl;
                lib.saveUsers();
            }
            else{
                cout << "Login has failed. Please try again next time with correct credentials. " << endl;
                continue;
            }
            
        }

        else{
            cout << "Invalid input detected" << endl;
            continue;
        }

        cout << "Welcome " << user->getName() << endl;
        id = user->getId();

        int f = 1;
        if (lib.findStudent(id)){
            while(1){
                
                
                int choice = 0;

                cout << "=================================================\n";
                cout << "press 1 to return a book " << endl;
                cout << "press 2 to borrow a book " << endl;
                cout << "press 3 to view and pay fines" << endl;
                cout << "press 4 to view the books which you have borrowed" << endl;
                cout << "press 5 to view the list of available books" << endl;
                cout << "press 6 to view your borrowing history " << endl;
                cout << "press anything else to logout" << endl;
                cout << "=================================================\n";

                //cin >> choice;

                while (!(cin >> choice)) {
                    // If the input fails (e.g., a string was entered)
                    cout << "Invalid input. Please enter an integer: ";
                    cin.clear(); // Clear the error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                }

                if (choice == 1){
                    lib.returnBook(user->getId());
                    continue;
                }
                else if(choice == 2){
                    int date;
                    
                    cout << "Enter todays date in the format DD/MM/YYYY without the slashes \n";

                    while (!(cin >> date)) {
                        // If the input fails (e.g., a string was entered)
                        cout << "Invalid input. Please enter an integer: ";
                        cin.clear(); // Clear the error flags
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    }

                    if (user->canBorrow(date))
                        lib.borrowBook(date,user->getId());
                    else{
                        cout << "You either have some pending dues or some book has been held for longer than the maximum\n";
                        cout << "borrowing period. Please try again after clearing the issue. \n";
                    }
                    continue;
                }
                else if(choice == 3){
                    int ch = 0;
                    cout << "Enter 1 to view fines " << endl;
                    cout << "Enter 2 to pay your fines " << endl;

                    cin >> ch;

                    if (ch == 1){
                        user->checkFine();
                        continue;
                    }
                    else if (ch == 2){
                        user->payFine();
                        continue;
                    }
                    else{
                        cout << "Invalid input " << endl;
                        continue;
                    }
                }

                else if (choice == 4){
                    cout << "These are the books which you have borrowed " << endl;
                    cout << endl;
                
                    user->disp();
                    continue;
                }

                else if (choice == 5){
                    cout << "These are the books you can choose from " << endl;
                    cout << "=================================================\n" << endl;

                    lib.displayBooks();
                    
                    cout << "=================================================\n" << endl;
                    continue;
                   
                }

                else if (choice == 6){
                    cout << "=================================================\n";
                    user->viewBorrowHistory();
                    cout << "=================================================\n";
                    continue;
                }

                else{
                    cout << "Thank you for using our services. Goodbye" << endl;
                    f = 0;
                    break;
                }
            }
            if (f == 0){
                break;
            }
        }

        else if (lib.findFaculty(id)){
            while(1){
                
                
                int choice = 0;

                cout << "=================================================\n";
                cout << "press 1 to return a book " << endl;
                cout << "press 2 to borrow a book " << endl;
                //cout << "press 3 to view if any books are overdue" << endl;
                cout << "press 3 to view the books which you have borrowed" << endl;
                cout << "press 4 to view the list of available books" << endl;
                cout << "press 5 to view your borrowing history " << endl;
                cout << "press anything else to logout" << endl;
                cout << "=================================================\n";

                //cin >> choice;

                while (!(cin >> choice)) {
                    // If the input fails (e.g., a string was entered)
                    cout << "Invalid input. Please enter an integer: ";
                    cin.clear(); // Clear the error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                }

                if (choice == 1){
                    lib.returnBook(user->getId());
                }
                else if(choice == 2){
                    int date;
                    
                    cout << "Enter todays date in the format DD/MM/YYYY without the slashes \n";

                    while (!(cin >> date)) {
                        // If the input fails (e.g., a string was entered)
                        cout << "Invalid input. Please enter an integer: ";
                        cin.clear(); // Clear the error flags
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    }


                    if (user->canBorrowFaculty(date)){
                        lib.borrowBook(date,user->getId());
                    }

                    else{
                        cout << "You either have some pending dues or some book has been held for longer than the maximum\n";
                        cout << "borrowing period. Please try again after clearing the issue. \n";
                    }
                    continue;
                    
                }

                else if (choice == 3){
                    cout << "These are the books which you have borrowed " << endl;
                    cout << endl;
                
                    user->disp();
                    continue;
                }

                else if (choice == 4){
                    cout << "These are the books you can choose from " << endl;
                    cout << "=================================================\n" << endl;
                    lib.displayBooks();
                    cout << "=================================================\n" << endl;

                    continue;
                }

                else if (choice == 5){
                    cout << "=================================================\n";
                    user->viewBorrowHistory();

                    cout << "=================================================\n";
                    continue;
                    
                }

                else{

                    cout << "Thank you for using our services. Goodbye" << endl;
                    f = 0;
                    break;
                }
            }

            if (f == 0){
                break;
            }
        }

        else if(lib.findLibrarian(id)){
            //int f = 1;
            while(1){

                int choice = 0;

                cout << "=================================================\n";
                cout << "Enter 1 to add a new book to the library\n";
                cout << "Enter 2 to delete a book from the library\n";
                cout << "Enter 3 to view all the books in the library\n";
                cout << "Enter 4 to modify book details\n";
                cout << "Enter 5 to view students\n";
                cout << "Enter 6 to add a new user\n";
                cout << "Enter 7 to delete student\n";
                cout << "Enter 8 to view faculty\n";
                //cout << "Enter 9 to add new faculty\n";
                cout << "Enter 9 to delete faculty\n";
                cout << "Enter anything else to logout\n";
                cout << "=================================================\n";

                //cin >> choice;

                while (!(cin >> choice)) {
                    // If the input fails (e.g., a string was entered)
                    cout << "Invalid input. Please enter an integer: ";
                    cin.clear(); // Clear the error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                }

                if (choice == 1){
                    lib.addBook();
                }

                else if(choice == 2){
                    lib.removeBook();
                }

                else if(choice == 3){
                    lib.displayBooks();
                }

                else if(choice == 4){
                    lib.modifyBookDetails();
                }

                else if(choice == 5){
                    lib.displayStudents();
                }

                else if(choice == 6){
                    lib.addUser();
                }

                else if(choice == 7){
                    lib.deleteStudent();
                }

                else if(choice == 8){
                    lib.displayFaculty();
                }

                else if(choice == 9){
                    lib.deleteFaculty();
                }

                else{
                    cout << "Thank you for using our services. Goodbye" << endl;
                    f = 0;
                    break;
                }
            }

            if (f == 0){
                break;
            }

        }

        else{
            cout << "Wrong id entered.\n";
            cout << "User credentials not found. Please try again\n";
        }
    }
    return 0;
}
