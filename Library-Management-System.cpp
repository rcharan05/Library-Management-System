#include<bits/stdc++.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

typedef long long ll;

class Book{
    private:
        string title, author, publisher;
        int year;
        ll ISBN;
        string status; //It can be Available, Unavailable or Reserve+"userID" or Unavailable+"userID", where userID is of the reservee!
    public:
        Book(ll isbn=0, string t=" ", string a=" ", string p=" ", int y=0, string s="Available"){
            title=t;
            author=a;
            publisher=p;
            year=y;
            ISBN=isbn;
            status=s;
        }
        ll get_ISBN(){
            return ISBN;
        }
        void update_ISBN(ll isbn){
            ISBN=isbn;
        }
        string get_title(){
            return title;
        }
        void update_title(string t=""){
            title=t;
        }
        string get_author(){
            return author;
        }
        void update_author(string a=""){
            author=a;
        }
        string get_publisher(){
            return publisher;
        }
        void update_publisher(string p=""){
            publisher=p;
        }
        string get_status(){
            return status;
        }
        void update_status(string s="Available"){
            status=s;
        }
        int get_year(){
            return year;
        }
        void update_year(int y=0){
            year=y;
        }
        string serializeCSV() const{
            ostringstream oss;
            oss<<ISBN<<","<<title<<","<<author<<","<<publisher<<","<<year<<","<<status;
            return oss.str();
        }

        static Book deserializeCSV(const string& csvLine) {
            istringstream iss(csvLine);
            string token;
            vector<string> tokens;
            while(getline(iss, token, ',')) {
                tokens.push_back(token);
            }

            // Expecting exactly 6 columns

            if(tokens.size() != 6) {
                throw runtime_error("Invalid CSV format for Book.");
            }
            return Book(
                stoll(tokens[0]), tokens[1], tokens[2],
                tokens[3], stoi(tokens[4]), tokens[5]
            );
        }
};

class User{
    protected:
        ll userID, fine;
        string name, password;
        int role; //0: Librarian, 1:Student, 2:Professor
    public:
        User() : userID(0), name(""), password(""), role(1), fine(0) {}

        User(ll id, string n, string p="1", ll f=0, int r=1){
            userID=id;
            name=n;
            password=p;
            fine=f;
            role=r;
        }

        ll get_fine(){
            return fine;
        }

        void update_fine(ll f){
            fine=f;
        }

        int get_role(){
            return role;
        }
        ll get_userID(){
            return userID;
        }
        void update_userID(ll n){
            userID=n;
        }
        string get_name(){
            return name;
        }
        void update_name(string n){
            name=n;
        }
        string get_pwd(){
            return password;
        }
        void update_pwd(string p){
            password=p;
        }
        string serializeCSV() const{
            ostringstream oss;
            oss<<userID<<","<<name<<","<<password<<","<<fine<<","<<role;
            return oss.str();
        }

        virtual vector<pair<ll, time_t>> borrowBook(vector<pair<ll, time_t>> &v, map<ll, Book> &m) {
            throw runtime_error("Borrowing books is not implemented for this user type.");
        }
    
        virtual pair<ll, time_t> returnBook(vector<pair<ll, time_t>> &current_borrowed) {
            throw runtime_error("Returning books is not implemented for this user type.");
        }

        static User deserializeCSV(const string& csvLine) {
            istringstream iss(csvLine);
            string token;
            vector<string> tokens;
            while(getline(iss, token, ',')) {
                tokens.push_back(token);
            }

            // Expecting exactly 5 columns

            if(tokens.size() != 5) {
                throw runtime_error("Invalid CSV format for Book.");
            }
            return User(
                stoll(tokens[0]), tokens[1], tokens[2], stoll(tokens[3]), stoi(tokens[4])
            );
        }
};

class Account{
    vector <pair<ll, time_t>>current_borrowed;
    vector <pair<ll, time_t>> history;
    public:
        Account(){}

        Account(vector <pair<ll, time_t>> c, vector<pair<ll, time_t>> h){
            current_borrowed=c;
            history=h;
        }

        vector <pair<ll, time_t>> get_current(){
            return current_borrowed;
        }

        vector <pair<ll, time_t>> get_history(){
            return history;
        }

        void view_history(){
            if(history.size()==0){
                cout<<"History is empty since you haven't returned any book yet!"<<endl;
            }
            else{
                for(auto &pair: history){
                    cout<<pair.first<<" Returned on ";
                    struct tm *localTime = localtime(&pair.second);
                    char buffer[80];
                    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
                    cout << buffer <<endl;
                }
            }
        }
        static pair<ll, time_t> deserializeCSV1(const string& csvLine) {
            istringstream iss(csvLine);
            string token;
            vector<string> tokens;
            while(getline(iss, token, ',')) {
                tokens.push_back(token);
            }

            // Expecting exactly 2 columns

            if(tokens.size() != 2) {
                throw runtime_error("Invalid CSV format for Book.");
            }
            pair <ll, time_t> p; 
            p={stoll(tokens[0]), stoll(tokens[1])};
            return p;
        }
        static string serializeCSV(pair<ll, time_t> p){
            ostringstream oss;
            oss<<p.first<<","<<p.second;
            return oss.str();
        } 
};

class Student : public User{
    const int maxBooks = 3;
    const int maxDays = 15;
    const double finePerDay = 10.0;
    Account account;
    public:
        Student(ll id, string name, string pwd, ll f=0) : User(id, name, pwd, f, 1){}

        void add_account(Account a){
            account=a;
        }

        void update_fine(ll f){
            fine=f;
        }

        void view_history(){
            account.view_history();
        }

        void pay_fine(){
            if(fine==0){
                cout<<"You have no fines! Keep up the good work"<<endl;
            }
            else{
                cout<<"Do you wish to pay your fine amount of Rs."<<fine<<"?"<<endl;
                cout<<"Enter y[yes]/n[no]: "<<endl;
                char c;
                while (!(cin >> c)) {  
                    cout << "Invalid input! Please enter a single character: ";
                    cin.clear(); 
                    cin.ignore(1000, '\n');  
                }
                //cin>>c;
                if(c=='y') fine=0;
                else if(c=='n') {}
                else cout<<"Invalid option chosen!"<<endl;
            }
        }
        vector<pair<ll, time_t>> borrowBook (vector<pair<ll, time_t>> &v, map<ll, Book> &m) override {
            cout<<"Which book do you wish to borrow? Enter its ISBN: "<<endl;
            ll isbn;
            while (!(cin >> isbn)) {  
                cout << "Invalid input! Please enter an integer: ";
                cin.clear(); 
                cin.ignore(1000, '\n');  
            }
            //cin>>isbn;
            if(fine>0){
                cout<<"Pay your fines to borrow more books!"<<endl;
                cout<<"Current fines are: "<<fine<<endl;
                return v;
            }
            else if(v.size()>=maxBooks){
                cout<<"You have already reached your limit!"<<endl;
                return v;
            }
            else if(m.find(isbn)==m.end()){
                cout<<"No such book exists!"<<endl;
                return v;
            }
            else if(m[isbn].get_status()=="Available"){
                time_t t=time(nullptr);
                v.push_back({isbn, t});
                m[isbn].update_status("Unavailable");
                return v;
            }
            else if(m[isbn].get_status()=="Unavailable"){
                cout<<"This book is unavailable! You will be able to borrow the book as soon as the book has been returned! Do you wish to reserve this y[yes]/n[no]?"<<endl;
                char c;
                while (!(cin >> c)) {  
                    cout << "Invalid input! Please enter a single character: ";
                    cin.clear(); 
                    cin.ignore(1000, '\n');  
                }
                //cin>>c;
                if(c=='y' || c=='Y'){
                    m[isbn].update_status(("Unavailable"+to_string(userID)));
                }
                else{
                    cout<<"Book isn't being reserved!"<<endl;
                }
                return v;
            }
            else if(m[isbn].get_status()==("Reserve"+to_string(userID))){
                time_t t=time(nullptr);
                v.push_back({isbn, t});
                m[isbn].update_status("Unavailable");
                return v;
            }
            else{
                cout<<"Book is currently not available and has been reserved already!"<<endl;
                return v;
            }
        }

        pair <ll, time_t> returnBook(vector<pair<ll, time_t>> &current_borrowed) override {
            if(current_borrowed.size()==0){
                cout<<"You haven't borrowed any book and thus can't return any!"<<endl;
                ll k1=0;
                return {k1, time(NULL)};
            }
            cout<<"List of books you own: "<<endl;
            for(auto &pair: current_borrowed){
                cout<<pair.first<<" Borrowed on ";
                struct tm *localTime = localtime(&pair.second);
                char buffer[80];
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
                cout << buffer <<endl;
            }
            cout<<"Enter ISBN you wish to return: "<<endl;
            ll isbn;
            while (!(cin >> isbn)) {  
                cout << "Invalid input! Please enter an integer: ";
                cin.clear(); 
                cin.ignore(1000, '\n');  
            }
            //cin>>isbn;
            for(auto &pair: current_borrowed){
                if(pair.first==isbn){
                    cout<<"Successfully returned"<<endl;
                    auto it=find(current_borrowed.begin(), current_borrowed.end(), pair);
                    current_borrowed.erase(it);
                    ll overdue=(time(nullptr)-pair.second)/86400;
                    if(overdue>maxDays){
                        fine=fine+finePerDay*(overdue-maxDays);
                        cout<<"You have returned the book "<<overdue-maxDays<<" days late!"<<endl;
                        cout<<"Fine of Rs"<<finePerDay*(overdue-maxDays)<<" has been added!"<<endl;
                    }
                    return {pair.first, time(nullptr)};
                }
            }
            cout<<"You haven't borrowed this ISBN!"<<endl;
            ll k=0;
            return {k, time(NULL)};
        }
};

class Professor : public User{
    const int maxBooks = 5;
    const int maxDays = 30;
    Account account;
    public:
        Professor(ll id, string name, string pwd): User(id, name, pwd, 0, 2){}

        void add_account(Account a){
            account=a;
        }

        void view_history(){
            account.view_history();
        }

        vector<pair<ll, time_t>> borrowBook(vector<pair<ll, time_t>> &v, map<ll, Book> &m) override{
            cout<<"Which book do you wish to borrow? Enter its ISBN: "<<endl;
            ll isbn;
            while (!(cin >> isbn)) {  
                cout << "Invalid input! Please enter an integer: ";
                cin.clear(); 
                cin.ignore(1000, '\n');  
            }
            //cin>>isbn;
            int t=0;
            for(auto &pair: v){
                if((time(nullptr)-pair.second)/86400>60){
                    t=1;
                    break;
                }
            }
            if(t==1){
                cout<<"You have a book beyond 60 days! Please return that to continue"<<endl;
                return v;
            }
            else if(v.size()>=maxBooks){
                cout<<"You have already reached your limit!"<<endl;
                return v;
            }
            else if(m.find(isbn)==m.end()){
                cout<<"No such book exists!"<<endl;
                return v;
            }
            else if(m[isbn].get_status()=="Available"){
                time_t t=time(nullptr);
                v.push_back({isbn, t});
                m[isbn].update_status("Unavailable");
                return v;
            }
            else if(m[isbn].get_status()=="Unavailable"){
                cout<<"This book is unavailable! You will be able to borrow the book as soon as the book has been returned! Do you wish to reserve this y[yes]/n[no]?"<<endl;
                char c;
                while (!(cin >> c)) {  
                    cout << "Invalid input! Please enter a single character: ";
                    cin.clear(); 
                    cin.ignore(1000, '\n');  
                }
                //cin>>c;
                if(c=='y' || c=='Y'){
                    m[isbn].update_status(("Unavailable"+to_string(userID)));
                }
                else{
                    cout<<"Book isn't being reserved!"<<endl;
                }
                return v;
            }
            else if(m[isbn].get_status()==("Reserve"+to_string(userID))){
                time_t t=time(nullptr);
                v.push_back({isbn, t});
                m[isbn].update_status("Unavailable");
                return v;
            }
            else{
                cout<<"Book is currently not available and has been reserved already!"<<endl;
                return v;
            }
        }

        pair <ll, time_t> returnBook(vector<pair<ll, time_t>> &current_borrowed) override {
            if(current_borrowed.size()==0){
                cout<<"You haven't borrowed any book and thus can't return any!"<<endl;
                ll k1=0;
                return {k1, time(NULL)};
            }
            cout<<"List of books you own: "<<endl;
            for(auto &pair: current_borrowed){
                cout<<pair.first<<" Borrowed on ";
                struct tm *localTime = localtime(&pair.second);
                char buffer[80];
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
                cout << buffer <<endl;
            }
            cout<<"Enter ISBN you wish to return: "<<endl;
            ll isbn;
            while (!(cin >> isbn)) {  
                cout << "Invalid input! Please enter an integer: ";
                cin.clear(); 
                cin.ignore(1000, '\n');  
            }
            //cin>>isbn;
            for(auto &pair: current_borrowed){
                if(pair.first==isbn){
                    cout<<"Successfully returned"<<endl;
                    auto it=find(current_borrowed.begin(), current_borrowed.end(), pair);
                    current_borrowed.erase(it);
                    ll overdue=(time(nullptr)-pair.second)/86400;
                    if(overdue>maxDays){
                        cout<<"You have returned the book "<<overdue-maxDays<<" days late!"<<endl;
                    }
                    return {pair.first, time(nullptr)};
                }
            }
            ll k=0;
            cout<<"You haven't borrowed this ISBN!"<<endl;
            return {k, time(NULL)};
        }
};

class Librarian : public User{
    public:
        Librarian(ll id, string name, string pwd) : User(id, name, pwd, 0){}
};

//creating for the first time. File clears when called everytime

void saveBooksCSV(const map<ll, Book> &m, string filename){
    ofstream ofs(filename);
    if(ofs.is_open()){
        ofs<<"ISBN,Title,Author,Publisher,Year,Status\n";
        for(const auto &pair: m){
            ofs<<pair.second.serializeCSV()<<"\n";
        }
        ofs.close();
    }
    else{
        cerr<<"Error in opening file Books"<<endl;
    }
}

void saveUsersCSV(const map<ll, User> &m, string filename){
    ofstream ofs(filename);
    if(ofs.is_open()){
        ofs<<"UserID,Name,Password,Fine,Role\n";
        for(const auto &pair: m){
            ofs<<pair.second.serializeCSV()<<"\n";
        }
        ofs.close();
    }
    else{
        cerr<<"Error in opening file Users"<<endl;
    }
}

void saveBorrowedCSV(const vector<pair<ll, time_t>> &v, string filename){
    ofstream ofs(filename);
    if(ofs.is_open()){
        for(const auto &pair: v){
            ofs<<Account::serializeCSV(pair)<<"\n";
        }
    }
    else{
        cerr<<"Error in opening file"<<endl;
    }
}

map <ll, Book> loadBooksCSV(const string& filename){
    map<ll, Book> books;
    ifstream ifs(filename);
    if(ifs.is_open()){
        string line;
        bool header=true;
        while(getline(ifs, line)){
            //to skip header (first line in file)
            if(header){ 
                header=false;
                continue;
            }
            if(line.empty()) continue;
            try{
                Book b = Book::deserializeCSV(line);
                books[b.get_ISBN()]=b;
            } catch(const exception &e){
                cerr<<"Error parsing the line in file: "<<line<<endl;
            }
        }
        ifs.close();
    }
    else{
        cerr<<"Error opening file"<<endl;
    }
    return books;
}


map <ll, User> loadCredCSV(const string& filename){
    map<ll, User> users;
    ifstream ifs(filename);
    if(ifs.is_open()){
        string line;
        bool header=true;
        while(getline(ifs, line)){
            //to skip header (first line in file)
            if(header){ 
                header=false;
                continue;
            }
            if(line.empty()) continue;
            try{
                User b = User::deserializeCSV(line);
                users[b.get_userID()]=b;
            } catch(const exception &e){
                cerr<<"Error parsing the line in file: "<<line<<endl;
            }
        }
        ifs.close();
    }
    else{
        cerr<<"Error opening file"<<endl;
    }
    return users;
}

vector<pair<ll, time_t>> loadBorrowedCSV(const string& filename){
    vector<pair<ll, time_t>> Borrowed;
    ifstream ifs(filename);
    if(ifs.is_open()){
        string line;
        while(getline(ifs, line)){
            if(line.empty()) continue;
            try{
                pair <ll, time_t> p;
                p = Account::deserializeCSV1(line);
                Borrowed.push_back(p);
            } catch(const exception &e){
                cerr<<"Error parsing the line in file: "<<line<<endl;
            }
        }
        ifs.close();
    }
    else{
        ofstream ofs(filename);
        ofs.close();
        Borrowed={};
    }
    return Borrowed;
}

void first_time(){
    map <ll, Book> books;
    books[123]=Book(123, "Intro to Machine Learning 1", "Piyush Rai", "IIT Kanpur", 2018);
    books[124]=Book(124, "Intro to Machine Learning 2", "Piyush Rai", "IIT Kanpur", 2018);
    books[125]=Book(125, "Intro to Machine Learning 3", "Piyush Rai", "IIT Kanpur", 2019);
    books[126]=Book(126, "Intro to Machine Learning 4", "Piyush Rai", "IIT Kanpur", 2019);
    books[127]=Book(127, "Intro to Machine Learning 5", "Piyush Rai", "IIT Kanpur", 2019);
    books[128]=Book(128, "Intro to Machine Learning 6", "Piyush Rai", "IIT Kanpur", 2020);
    books[129]=Book(129, "Intro to Machine Learning 7", "Piyush Rai", "IIT Kanpur", 2020);
    books[130]=Book(130, "Intro to Machine Learning 8", "Piyush Rai", "IIT Kanpur", 2021);
    books[131]=Book(131, "Intro to Machine Learning 9", "Piyush Rai", "IIT Kanpur", 2021);
    books[132]=Book(132, "Intro to Machine Learning 10", "Piyush Rai", "IIT Kanpur", 2022);

    string filename="books_csv.csv";

    saveBooksCSV(books, filename);
    cout<<"Saving done!"<<endl;

    map <ll, User> userslist;
    filename="credentials.csv";
    
    userslist[0]=User(0, "Charan", "IITK", 0, 0); //1 Librarian

    //5 students
    userslist[1]=User(1, "Spandan", "1", 0, 1);
    userslist[2]=User(2, "Nikhil", "1", 0, 1);
    userslist[3]=User(3, "Achyut", "1", 0, 1);
    userslist[4]=User(4, "Poorvie", "1", 0, 1);
    userslist[5]=User(5, "Aaryaman", "1", 0, 1);

    //3 Professors
    userslist[6]=User(6, "Ashutosh Modi", "1", 0, 2);
    userslist[7]=User(7, "Piyush Rai", "1", 0, 2);
    userslist[8]=User(8, "Indranil Saha", "1", 0, 2);

    saveUsersCSV(userslist, filename);

    cout<<"Saving Done!"<<endl;
}

int main(){

    //comment below function after running once

    first_time();


    string n;
    ll i=0;
    int r;
    string password;
    ll userID;
    map<ll, Book> loadedBooks = loadBooksCSV("books_csv.csv");
    map <ll, User> loadedUsers = loadCredCSV("credentials.csv");
    while(i==0){
        cout<<"Enter UserID: ";
        while (!(cin >> userID)) {  
            cout << "Invalid input! Please enter an integer: ";
            cin.clear(); 
            cin.ignore(1000, '\n');  
        }
        //cin>>userID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"Enter Password: ";
        getline(cin, password);
        if(loadedUsers[userID].get_pwd()==password){
            cout<<"Login successful";
            if(password=="1"){
                cout<<"You are logging in for the first time. We highly advice that you change and set your own password."<<endl;
                int j=0;
                while(j==0){
                    cout<<"Enter you're new password: ";
                    cin.clear();
                    getline(cin, password);
                    if(password=="1" || password.empty()){
                        cout<<"Too weak! Try again"<<endl;
                    }
                    else{
                        loadedUsers[userID].update_pwd(password);
                        cout<<"You're password has been successfully updated! Thankyou"<<endl;
                        j=1;
                    }
                }
            }
            r=loadedUsers[userID].get_role();
            n=loadedUsers[userID].get_name();
            i=1;
        }
        else cout<<"Wrong credentials! Try again"<<endl; 
    }
    i=0;
    while(r==0){
        if(i==0){
            cout<<"Welcome Back Librarian, "<<n<<endl;
            i=1;
        }
        cout<<"Choose what operation you wish to do today, "<<endl;
        cout<<"1. Manage user"<<endl;
        cout<<"2. Manage Books"<<endl;
        cout<<"Choose option number 1/2: ";
        int option;
        while (!(cin >> option)) {  
            cout << "Invalid input! Please enter an integer: ";
            cin.clear(); 
            cin.ignore(1000, '\n');  
        }
        //cin>>option;
        //cout<<"\033[2J\033[1;1H";    -> This was specefic to windows to clear output screen, so to avoid errors while running on other OS, I've commented this
        if(option==1){
            cout<<"1. Add new user"<<endl<<"2. Remove user"<<endl<<"Choose option number 1/2: ";
            int option;
            while (!(cin >> option)) {  
                cout << "Invalid input! Please enter an integer: ";
                cin.clear(); 
                cin.ignore(1000, '\n');  
            }
            //cin>>option;
            if(option==1){
                cout<<"Do you wish to add Professor(p) or Student(s) or another Librarian(l)?"<<endl;
                char c;
                while (!(cin >> c)) {  
                    cout << "Invalid input! Please enter a single character s or p or l: ";
                    cin.clear(); 
                    cin.ignore(1000, '\n');  
                }
                //cin>>c;
                if(c=='p'||c=='P'){
                    cout<<"Enter the following details about the Professor,"<<endl;
                    cout<<"Enter the unique userID: "<<endl;
                    ll userID;
                    string name;
                    while (!(cin >> userID)) {  
                        cout << "Invalid input! Please enter an integer: ";
                        cin.clear(); 
                        cin.ignore(1000, '\n');  
                    }
                    //cin>>userID;
                    if(loadedUsers.find(userID)!=loadedUsers.end() && loadedUsers[userID].get_name()!=""){
                        cout<<"This userID already exists!"<<endl;
                    }
                    else{
                        cin.ignore();
                        cout<<"Enter the name of the Professor: "<<endl;
                        getline(cin, name);
                        cout<<"Thankyou for adding the details, the user will be added!"<<endl;
                        loadedUsers[userID]=User(userID, name, "1", 0, 2);
                    }
                }
                else if(c=='s'||c=='S'){
                    cout<<"Enter the following details about the Student,"<<endl;
                    cout<<"Enter the unique userID: "<<endl;
                    string name;
                    ll userID;
                    while (!(cin >> userID)) {  
                        cout << "Invalid input! Please enter an integer: ";
                        cin.clear(); 
                        cin.ignore(1000, '\n');  
                    }
                    //cin>>userID;
                    if(loadedUsers.find(userID)!=loadedUsers.end() && loadedUsers[userID].get_name()!=""){
                        cout<<"This userID already exists!"<<endl;
                    }
                    else{
                        cin.ignore();
                        cout<<"Enter the name of the Student: "<<endl;
                        getline(cin, name);
                        cout<<"Thankyou for adding the details, the user will be added!"<<endl;
                        loadedUsers[userID]=User(userID, name, "1", 0, 1);
                    }
                }
                else if(c=='l' || c=='L'){
                    cout<<"Enter the following details about the Librarian,"<<endl;
                    cout<<"Enter the unique userID: "<<endl;
                    string name;
                    ll userID;
                    while (!(cin >> userID)) {  
                        cout << "Invalid input! Please enter an integer: ";
                        cin.clear(); 
                        cin.ignore(1000, '\n');  
                    }
                    //cin>>userID;
                    if(loadedUsers.find(userID)!=loadedUsers.end() && loadedUsers[userID].get_name()!=""){
                        cout<<"This userID already exists!"<<endl;
                    }
                    else{
                        cin.ignore();
                        cout<<"Enter the name of the Librarian: "<<endl;
                        getline(cin, name);
                        cout<<"Thankyou for adding the details, the user will be added!"<<endl;
                        loadedUsers[userID]=User(userID, name, "1", 0, 0);
                    }
                }
                else{
                    cout<<"Wrong option, heading back to start of program"<<endl;
                    continue;
                }
            }
            else if(option==2){
                int i=0;
                while(i==0){
                    cout<<"Enter the userID of user you wish to remove: "<<endl;
                    ll useID;
                    while (!(cin >> useID)) {  
                        cout << "Invalid input! Please enter an integer: ";
                        cin.clear(); 
                        cin.ignore(1000, '\n');  
                    }
                    //cin>>userID;

                    if(useID==userID){
                        cout<<"You can't remove yourself!"<<endl;
                    }
                    else{
                        int val=loadedUsers.erase(useID);
                        if(val==1){
                            cout<<"User successfully removed!"<<endl;
                            i=1;
                        }
                        else cout<<"Invalid userID was provided. No such user exists!"<<endl;
                    }
                }
            }
            else{
                cout<<"Invalid option chosen!"<<endl;
            }
        }
        else if(option==2){
            cout<<"List of Books: "<<endl;
            for(auto &pair: loadedBooks){
                cout<<pair.second.get_ISBN()<<" "<<pair.second.get_title()<<" "<<pair.second.get_author()<<" "<<pair.second.get_publisher()<<" "<<pair.second.get_year()<<endl;
            }
            cout<<"1. Add new book"<<endl<<"2. Remove a book"<<endl<<"3. Update book details"<<endl<<"Choose option number 1/2/3: ";
            int option;
            while (!(cin >> option)) {  
                cout << "Invalid input! Please enter an integer: ";
                cin.clear(); 
                cin.ignore(1000, '\n');  
            }
            //cin>>option;
            if(option==2){
                int i=0;
                while(i==0){
                    cout<<"Enter ISBN of book: "<<endl;
                    ll isbn;
                    while (!(cin >> isbn)) {  
                        cout << "Invalid input! Please enter an integer: ";
                        cin.clear(); 
                        cin.ignore(1000, '\n');  
                    }
                    //cin>>isbn;
                    int val=loadedBooks.erase(isbn);
                    if(val==1){
                        cout<<"Book erased successfully!"<<endl;
                        i=1;
                    }
                    else cout<<"Book with given ISBN doesn't exist! Try again"<<endl;
                }
            }
            else if(option==1){
                cout<<"Enter book details please,"<<endl;
                cout<<"ISBN: "<<endl;
                ll isbn;
                while (!(cin >> isbn)) {  
                    cout << "Invalid input! Please enter an integer: ";
                    cin.clear(); 
                    cin.ignore(1000, '\n');  
                }
                string t, a, p;
                int y;
                //cin>>isbn;

                //Improvement: Check if ISBN already exists
                if(loadedBooks.find(isbn)!=loadedBooks.end()) cout<<"Book already exists with given isbn! Choose update option to change details!"<<endl;
                else{
                    cout<<"Title: "<<endl;
                    cin.clear();
                    getline(cin, t);
                    cout<<"Author: "<<endl;
                    cin.clear();
                    getline(cin, a);
                    cout<<"Publisher: "<<endl;
                    cin.clear();
                    getline(cin, p);
                    cout<<"Year: "<<endl;
                    while (!(cin >> y)) {  
                        cout << "Invalid input! Please enter an integer: ";
                        cin.clear(); 
                        cin.ignore(1000, '\n');  
                    }
                    //cin>>y;
                    loadedBooks[isbn]=Book(isbn, t, a, p, y);
                    cout<<"Succeessfully added the given book"<<endl;
                }
            }
            else if(option==3){
                cout<<"Enter following details about the book you wish to update the details of, "<<endl;
                cout<<"ISBN: "<<endl;
                ll isbn;
                string t, a, p;
                int y;
                while (!(cin >> isbn)) {  
                    cout << "Invalid input! Please enter an integer: ";
                    cin.clear(); 
                    cin.ignore(1000, '\n');  
                }
                //cin>>isbn;
                //Improvement: Check if ISBN already exists in map and only then can update
                if(loadedBooks.find(isbn)==loadedBooks.end()) cout<<"Book doesn't exist! Choose add book option to add a book of given ISBN"<<endl;
                else{
                    cout<<"Title: "<<endl;
                    cin.clear();
                    getline(cin, t);
                    cout<<"Author: "<<endl;
                    cin.clear();
                    getline(cin, a);
                    cout<<"Publisher: "<<endl;
                    cin.clear();
                    getline(cin, p);
                    cout<<"Year: "<<endl;
                    while (!(cin >> y)) {  
                        cout << "Invalid input! Please enter an integer: ";
                        cin.clear(); 
                        cin.ignore(1000, '\n');  
                    }
                    //cin>>y;
                    loadedBooks[isbn]=Book(isbn, t, a, p, y);
                    cout<<"Succeessfully updated the given book"<<endl;
                }
            }
        }
        else{
            cout<<"Inavlid input! Choose a valid option"<<endl;
        }
        cout<<"Do you wish to perform another operation? y[YES]/any character[EXIT]: ";
        char cc;
        while (!(cin >> cc)) {  
            cout << "Invalid input! Please enter a character: ";
            cin.clear(); 
            cin.ignore(1000, '\n');  
        }
        //cin>>cc;
        if(cc=='y') {}
        else r=3;
    }
    while(r==1){
        if(i==0){
            cout<<"Welcome Back, "<<n<<endl;
            i=1;
        }
        Student currentStudent=Student(userID, n, password, loadedUsers[userID].get_fine());
        cout<<"Choose what operation you wish to do today, "<<endl;
        cout<<"1. Borrow/Return books"<<endl;
        cout<<"2. Pay fines"<<endl;
        cout<<"3. View History"<<endl;
        cout<<"Choose option number 1/2/3: ";
        int option;
        while (!(cin >> option)) {  
            cout << "Invalid input! Please enter an integer: ";
            cin.clear(); 
            cin.ignore(1000, '\n');  
        }
        //cin>>option;
        //cout<<"\033[2J\033[1;1H";     -> This was specefic to windows to clear output screen, so to avoid errors while running on other OS, I've commented this
        string filename1=to_string(userID)+"borrowed"+".csv";
        string filename2=to_string(userID)+"history"+".csv";
        vector <pair<ll, time_t>>current_borrowed = loadBorrowedCSV(filename1);
        vector <pair<ll, time_t>>history = loadBorrowedCSV(filename2);
        currentStudent.add_account(Account(current_borrowed, history));
        if(option==1){
            cout<<"1. Borrow a book"<<endl<<"2. Return book"<<endl<<"Choose option number 1/2: ";
            int option;
            while (!(cin >> option)) {  
                cout << "Invalid input! Please enter an integer: ";
                cin.clear(); 
                cin.ignore(1000, '\n');  
            }
            //cin>>option;
            if(option==1){
                cout<<"List of Books: "<<endl;
                for(auto &pair: loadedBooks){
                    cout<<pair.second.get_ISBN()<<" "<<pair.second.get_title()<<" "<<pair.second.get_author()<<" "<<pair.second.get_publisher()<<" "<<pair.second.get_year()<<endl;
                }
                current_borrowed=currentStudent.borrowBook(current_borrowed, loadedBooks);
            }
            else if(option==2){
                pair<ll, time_t> p=currentStudent.returnBook(current_borrowed);
                ll k=0;
                pair <ll, time_t> p1={k, time(NULL)};
                if(p==p1){
                    //then return unsucessfull
                }
                else{
                    history.push_back(p);
                    if(loadedBooks[p.first].get_status()=="Unavailable") loadedBooks[p.first].update_status("Available");
                    else loadedBooks[p.first].update_status("Reserve"+(loadedBooks[p.first].get_status().substr(11)));
                }
            }
            else{
                cout<<"Inavlid option chosen!"<<endl;
            }
        }
        else if(option==3){
            currentStudent.view_history();
        }
        else if(option==2){
            currentStudent.pay_fine();
        }
        else{
            cout<<"Invalid option chosen!"<<endl;
        }
        cout<<"Do you wish to perform another operation? y[YES]/any character[EXIT]: ";
        char c;
        while (!(cin >> c)) {  
            cout << "Invalid input! Please enter a single character: ";
            cin.clear(); 
            cin.ignore(1000, '\n');  
        }
        //cin>>c;
        //load back in all student data
        saveBorrowedCSV(current_borrowed, filename1);
        saveBorrowedCSV(history, filename2);
        loadedUsers[userID].update_fine(currentStudent.get_fine());
        if(c=='y') {}
        else r=3;
    }
    i=0;
    while(r==2){
        if(i==0){
            cout<<"Welcome Back, "<<n<<endl;
            i=1;
        }
        Professor currentProf=Professor(userID, n, password);
        cout<<"Choose what operation you wish to do today, "<<endl;
        cout<<"1. Borrow/Return books"<<endl;
        cout<<"2. View History"<<endl;
        cout<<"Choose option number 1/2: ";
        int option;
        while (!(cin >> option)) {  
            cout << "Invalid input! Please enter an integer: ";
            cin.clear(); 
            cin.ignore(1000, '\n');  
        }
        //cin>>option;
        //cout<<"\033[2J\033[1;1H";   -> This was specefic to windows to clear output screen, so to avoid errors while running on other OS, I've commented this
        string filename1=to_string(userID)+"borrowed"+".csv";
        string filename2=to_string(userID)+"history"+".csv";
        vector <pair<ll, time_t>>current_borrowed = loadBorrowedCSV(filename1);
        vector <pair<ll, time_t>>history = loadBorrowedCSV(filename2);
        currentProf.add_account(Account(current_borrowed, history));
        if(option==1){
            cout<<"1. Borrow a book"<<endl<<"2. Return book"<<endl<<"Choose option number 1/2: ";
            int option;
            while (!(cin >> option)) {  
                cout << "Invalid input! Please enter an integer: ";
                cin.clear(); 
                cin.ignore(1000, '\n');  
            }
            //cin>>option;
            if(option==1){
                cout<<"List of Books: "<<endl;
                for(auto &pair: loadedBooks){
                    cout<<pair.second.get_ISBN()<<" "<<pair.second.get_title()<<" "<<pair.second.get_author()<<" "<<pair.second.get_publisher()<<" "<<pair.second.get_year()<<endl;
                }
                current_borrowed=currentProf.borrowBook(current_borrowed, loadedBooks);
            }
            else if(option==2){
                pair<ll, time_t> p=currentProf.returnBook(current_borrowed);
                ll k=0;
                pair <ll, time_t> p1={k, time(NULL)};
                if(p==p1){
                    //then return unsucessfull
                }
                else{
                    history.push_back(p);
                    if(loadedBooks[p.first].get_status()=="Unavailable") loadedBooks[p.first].update_status("Available");
                    else loadedBooks[p.first].update_status("Reserve"+(loadedBooks[p.first].get_status().substr(11)));
                }
            }
            else{
                cout<<"Invalid option chosen!"<<endl;
            }
        }
        else if(option==2){
            currentProf.view_history();
        }
        else{
            cout<<"Invalid option chosen!"<<endl;
        }
        cout<<"Do you wish to perform another operation? y[YES]/any character[EXIT]: ";
        char c;
        while (!(cin >> c)) {  
            cout << "Invalid input! Please enter a single character: ";
            cin.clear(); 
            cin.ignore(1000, '\n');  
        }
        //cin>>c;
        //load back in all student data
        saveBorrowedCSV(current_borrowed, filename1);
        saveBorrowedCSV(history, filename2);
        if(c=='y') {}
        else r=3;
    }

    //load back all data --> i.e. Users and Books

    saveBooksCSV(loadedBooks, "books_csv.csv");
    saveUsersCSV(loadedUsers, "credentials.csv");

    return 0;
}