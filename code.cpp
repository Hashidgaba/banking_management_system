#include <iostream>
#include <fstream>
#include <string>
#include <regex>       
#include <cstring>     
#include <ctime>    // strcpy function ke liye required header file   
#include <iomanip>   
#include <sstream>     // Provides string streams, enabling conversion between strings and other data types. It's useful for formatting and parsing strings.
#include <limits>      // Defines limits for fundamental data types, such as the maximum and minimum values of integer or floating-point types.


using namespace std;

// Declare global variables
bool isLoggedIn = false;
char loggedInUserFullname[50];
char loggedInUserEmail[50];
char loggedInUserPhoneNumber[20];
char loggedInUserPass[20];
char loggedInUserPin[5];
double loggedInUserBalance = 0.0;

// Declare all functions
void Register();
void Login();
void Logout();
void depositMoney();
void withdrawMoney();
void checkBalance();
void accountStatement();
void moneyTransfer();

// Utility functions
bool validateEmail(const string &email);
bool validatePhoneNumber(const string &phone);
bool validatePassword(const string &password);
bool validatePin(const string &pin);
void saveUserDetails(const string &fullname, const string &email, const string &phone, const string &password, const string &pin);
void updateBalance();
void logTransaction(const string &type, double amount, const string &details);

// Main function
int main() {
    int choice;
    while (true) {
        try {
            if (isLoggedIn) {
                cout << "\n========== Banking Management System ==========" << endl;
                cout << "User: " << loggedInUserFullname << " | Email: " << loggedInUserEmail << " | Phone: " << loggedInUserPhoneNumber << endl;
                cout << "1. Deposit Money" << endl;
                cout << "2. Withdraw Money" << endl;
                cout << "3. Check Account Balance" << endl;
                cout << "4. Account Statement" << endl;
                cout << "5. Money Transfer" << endl;
                cout << "6. Logout" << endl;
                cout << "Enter your choice [1-6]: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                        depositMoney();
                        break;
                    case 2:
                        withdrawMoney();
                        break;
                    case 3:
                        checkBalance();
                        break;
                    case 4:
                        accountStatement();
                        break;
                    case 5:
                        moneyTransfer();
                        break;
                    case 6:
                        Logout();
                        break;
                    default:
                        throw invalid_argument("Invalid choice. Please try again.");
                }
            } else {
                cout << "\n========== Banking Management System ==========" << endl;
                cout << "1. Register" << endl;
                cout << "2. Login" << endl;
                cout << "3. Exit" << endl;
                cout << "Enter your choice [1-3]: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                        Register();
                        break;
                    case 2:
                        Login();
                        break;
                    case 3:
                        exit(0);
                        break;
                    default:
                        throw invalid_argument("Invalid choice. Please try again.");
                }
            }
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return 0;
}

// User register function - Fixed the file reading logic
void Register() {
    try {
        string fullname, email, phone, password, pin;
        cout << "\nEnter Full Name: ";
        cin.ignore();
        getline(cin, fullname);

        if (fullname.length() < 4) {
            throw invalid_argument("Full name must be at least 4 characters long!");
        }

        cout << "Enter Email: ";
        getline(cin, email);
        if (!validateEmail(email)) {
            throw invalid_argument("Invalid email format!");
        }

        cout << "Enter Phone Number (11 digits): ";
        getline(cin, phone);
        if (!validatePhoneNumber(phone)) {
            throw invalid_argument("Invalid phone number!");
        }

        // Check if email or phone already exists - Fixed logic
        string line;
        ifstream userCheck("userData.txt");
        while (getline(userCheck, line)) {
            string existingFullname, existingEmail, existingPhone, existingPassword, existingPin;
            double existingBalance;
            stringstream ss(line);
            ss >> existingFullname >> existingEmail >> existingPhone >> existingPassword >> existingPin >> existingBalance;
            
            if (existingEmail == email) {
                userCheck.close();
                throw invalid_argument("Email already exists!");
            }
            if (existingPhone == phone) {
                userCheck.close();
                throw invalid_argument("Phone number already exists!");
            }
        }
        userCheck.close();

        cout << "Enter Password (minimum 5 characters): ";
        getline(cin, password);
        if (!validatePassword(password)) {
            throw invalid_argument("Password must be at least 5 characters long!");
        }

        cout << "Enter 4-digit PIN: ";
        getline(cin, pin);
        if (!validatePin(pin)) {
            throw invalid_argument("PIN must be exactly 4 digits!");
        }

        // Save new user details
        saveUserDetails(fullname, email, phone, password, pin);

        strcpy(loggedInUserFullname, fullname.c_str());
        strcpy(loggedInUserEmail, email.c_str());
        strcpy(loggedInUserPhoneNumber, phone.c_str());
        strcpy(loggedInUserPass, password.c_str());
        strcpy(loggedInUserPin, pin.c_str());
        loggedInUserBalance = 0.0;

        isLoggedIn = true;
        cout << "Registration successful! You are now logged in." << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Login function - Fixed
void Login() {
    try {
        string email, password;
        cout << "\nEnter Email: ";
        cin.ignore();
        getline(cin, email);

        cout << "Enter Password: ";
        getline(cin, password);

        string line;
        ifstream userCheck("userData.txt");
        bool found = false;

        while (getline(userCheck, line)) {
            string fileFullname, fileEmail, filePhone, filePassword, filePin;
            double fileBalance;
            stringstream ss(line);
            ss >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance;

            if (fileEmail == email && filePassword == password) {
                strcpy(loggedInUserFullname, fileFullname.c_str());
                strcpy(loggedInUserEmail, fileEmail.c_str());
                strcpy(loggedInUserPhoneNumber, filePhone.c_str());
                strcpy(loggedInUserPass, filePassword.c_str());
                strcpy(loggedInUserPin, filePin.c_str());
                loggedInUserBalance = fileBalance;
                found = true;
                break;
            }
        }
        userCheck.close();

        if (!found) {
            throw invalid_argument("Invalid email or password!");
        }

        isLoggedIn = true;
        cout << "Login successful!" << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Logout function
void Logout() {
    isLoggedIn = false;
    cout << "You have been logged out." << endl;
}

// Deposit money function
void depositMoney() {
    try {
        double amount;
        cout << "\nEnter the amount to deposit: ";
        cin >> amount;

        if (amount <= 0) {
            throw invalid_argument("Invalid amount! Amount must be greater than 0.");
        }

        loggedInUserBalance += amount;
        updateBalance();
        logTransaction("Deposit", amount, "");
        cout << "Amount " << fixed << setprecision(2) << amount << " deposited successfully!" << endl;
        cout << "Current balance: " << loggedInUserBalance << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Withdraw money function
void withdrawMoney() {
    try {
        double amount;
        string pin;
        cout << "\nEnter the amount to withdraw: ";
        cin >> amount;

        if (amount <= 0) {
            throw invalid_argument("Invalid amount! Amount must be greater than 0.");
        }

        if (amount > loggedInUserBalance) {
            throw invalid_argument("Insufficient balance! Available balance: " + to_string(loggedInUserBalance));
        }

        cin.ignore();
        cout << "Enter your 4-digit PIN: ";
        getline(cin, pin);

        if (pin != loggedInUserPin) {
            throw invalid_argument("Incorrect PIN!");
        }

        loggedInUserBalance -= amount;
        updateBalance();
        logTransaction("Withdraw", amount, "");
        cout << "Amount " << fixed << setprecision(2) << amount << " withdrawn successfully!" << endl;
        cout << "Current balance: " << loggedInUserBalance << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Check balance function
void checkBalance() {
    cout << "\nYour current balance is: $" << fixed << setprecision(2) << loggedInUserBalance << endl;
}

// Account statement function
void accountStatement() {
    try {
        ifstream transactions("transactions.txt");
        if (!transactions) {
            throw runtime_error("No transaction history found!");
        }

        string line;
        bool found = false;
        cout << "\n==================== Account Statement ====================" << endl;
        
        while (getline(transactions, line)) {
            if (line.find(loggedInUserEmail) != string::npos) {
                cout << line << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No transactions found for this account." << endl;
        }
        
        cout << "========================================================" << endl;
        transactions.close();
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Money transfer function
void moneyTransfer() {
    try {
        string recipientPhone, pin;
        double amount;

        cout << "\nEnter recipient's phone number: ";
        cin >> recipientPhone;

        if (recipientPhone == loggedInUserPhoneNumber) {
            throw invalid_argument("Cannot transfer money to your own account!");
        }

        string line;
        ifstream userCheck("userData.txt");
        bool recipientFound = false;
        string recipientFullname, recipientEmail;
        double recipientBalance = 0.0;

        while (getline(userCheck, line)) {
            string fileFullname, fileEmail, filePhone, filePassword, filePin;
            double fileBalance;
            stringstream ss(line);
            ss >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance;

            if (filePhone == recipientPhone) {
                recipientFound = true;
                recipientFullname = fileFullname;
                recipientEmail = fileEmail;
                recipientBalance = fileBalance;
                break;
            }
        }
        userCheck.close();

        if (!recipientFound) {
            throw invalid_argument("Recipient not found!");
        }

        cout << "Enter amount to transfer: ";
        cin >> amount;

        if (amount <= 0) {
            throw invalid_argument("Invalid amount! Amount must be greater than 0.");
        }

        if (amount > loggedInUserBalance) {
            throw invalid_argument("Insufficient balance! Available balance: " + to_string(loggedInUserBalance));
        }

        cin.ignore();
        cout << "Enter your 4-digit PIN to confirm transfer: ";
        getline(cin, pin);

        if (pin != loggedInUserPin) {
            throw invalid_argument("Incorrect PIN!");
        }

        // Update balances
        loggedInUserBalance -= amount;
        recipientBalance += amount;

        // Update both accounts in the file
        ifstream inputFile("userData.txt");
        ofstream tempFile("temp.txt");
        
        while (getline(inputFile, line)) {
            string fileFullname, fileEmail, filePhone, filePassword, filePin;
            double fileBalance;
            stringstream ss(line);
            ss >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance;

            if (filePhone == loggedInUserPhoneNumber) {
                tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " 
                        << filePassword << " " << filePin << " " << loggedInUserBalance << endl;
            }
            else if (filePhone == recipientPhone) {
                tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " 
                        << filePassword << " " << filePin << " " << recipientBalance << endl;
            }
            else {
                tempFile << line << endl;
            }
        }
        
        inputFile.close();
        tempFile.close();
        remove("userData.txt");
        rename("temp.txt", "userData.txt");

        // Log transactions for both accounts
        logTransaction("Transfer Sent", amount, "To: " + recipientPhone);
        
        // Log transaction for recipient
        ofstream transactions("transactions.txt", ios::app);
        time_t now = time(0);
        char* dt = ctime(&now);
        transactions << recipientEmail << " | Transfer Received | " << fixed << setprecision(2) 
                    << amount << " | From: " << loggedInUserPhoneNumber << " | " << dt;
        transactions.close();

        cout << "Transfer successful!" << endl;
        cout << "Amount " << fixed << setprecision(2) << amount << " transferred to " << recipientFullname << endl;
        cout << "Your current balance: " << loggedInUserBalance << endl;

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Utility functions
bool validateEmail(const string &email) {
    const regex pattern(R"((\w+)(\.{1}\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, pattern);
}

bool validatePhoneNumber(const string &phone) {
    if (phone.length() != 11) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool validatePassword(const string &password) {
    return password.length() >= 5;
}

bool validatePin(const string &pin) {
    if (pin.length() != 4) return false;
    for (char c : pin) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void saveUserDetails(const string &fullname, const string &email, const string &phone, const string &password, const string &pin) {
    ofstream userData("userData.txt", ios::app);
    if (!userData) {
        throw runtime_error("Error opening file for saving user details!");
    }
    userData << fullname << " " << email << " " << phone << " " << password << " " << pin << " 0.0" << endl;
    userData.close();
}

void updateBalance() {
    string line;
    ifstream inputFile("userData.txt");
    ofstream tempFile("temp.txt");
    
    if (!inputFile || !tempFile) {
        throw runtime_error("Error updating balance!");
    }

    while (getline(inputFile, line)) {
        string fileFullname, fileEmail, filePhone, filePassword, filePin;
        double fileBalance;
        stringstream ss(line);
        ss >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance;

        if (filePhone == loggedInUserPhoneNumber) {
            tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " 
                    << filePassword << " " << filePin << " " << loggedInUserBalance << endl;
        } else {
            tempFile << line << endl;
        }
    }

    inputFile.close();
    tempFile.close();
    remove("userData.txt");
    rename("temp.txt", "userData.txt");
}

void logTransaction(const string &type, double amount, const string &details) {
    ofstream transactions("transactions.txt", ios::app);
    if (!transactions) {
        throw runtime_error("Error logging transaction!");
    }
    time_t now = time(0);
    char* dt = ctime(&now);
    transactions << loggedInUserEmail << " | " << type << " | " << fixed << setprecision(2) 
                << amount << " | " << details << " | " << dt;
    transactions.close();
}