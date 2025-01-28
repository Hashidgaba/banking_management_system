#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cstring>
#include <ctime>
#include <iomanip>
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
                cout << "Enter your choice [1-2]: ";

                cin >> choice;

                switch (choice) {
                    case 1:
                        Register();
                        break;
                    case 2:
                        Login();
                        break;
                    default:
                        throw invalid_argument("Invalid choice. Please try again.");
                }
            }
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
    }
    return 0;
}

// User register function
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

        // Check if email or phone already exists
        ifstream userData("userData.txt");
        if (!userData) {
            throw runtime_error("Error opening user data file!");
        }

        string fileFullname, fileEmail, filePhone, filePassword, filePin;
        double fileBalance;
        bool emailExists = false, phoneExists = false;

        while (userData >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance) {
            if (fileEmail == email) {
                emailExists = true;
            }
            if (filePhone == phone) {
                phoneExists = true;
            }
        }
        userData.close();

        if (emailExists) {
            throw invalid_argument("Email already exists!");
        }

        if (phoneExists) {
            throw invalid_argument("Phone number already exists!");
        }

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

        // After successful registration, directly move to the main menu and show user details
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

// Login function
void Login() {
    try {
        string email, password;
        cout << "\nEnter Email: ";
        cin.ignore();
        getline(cin, email);

        cout << "Enter Password: ";
        getline(cin, password);

        ifstream userData("userData.txt");
        if (!userData) {
            throw runtime_error("Error opening user data file!");
        }

        string fileFullname, fileEmail, filePhone, filePassword, filePin;
        double fileBalance;
        bool found = false;

        while (userData >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance) {
            if (fileEmail == email && filePassword == password) {
                found = true;
                strcpy(loggedInUserFullname, fileFullname.c_str());
                strcpy(loggedInUserEmail, fileEmail.c_str());
                strcpy(loggedInUserPhoneNumber, filePhone.c_str());
                strcpy(loggedInUserPass, filePassword.c_str());
                strcpy(loggedInUserPin, filePin.c_str());
                loggedInUserBalance = fileBalance;
                break;
            }
        }
        userData.close();

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
            throw invalid_argument("Invalid amount!");
        }

        loggedInUserBalance += amount;
        updateBalance();
        logTransaction("Deposit", amount, "");
        cout << "Amount deposited successfully!" << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Withdraw money function
void withdrawMoney() {
    try {
        double amount;
        string pin;
        cout << "\nEnter the amount to withdraw: ";
        cin >> amount;

        if (amount <= 0 || amount > loggedInUserBalance) {
            throw invalid_argument("Invalid or insufficient balance!");
        }

        cout << "Enter your 4-digit PIN: ";
        cin >> pin;

        if (pin != loggedInUserPin) {
            throw invalid_argument("Incorrect PIN!");
        }

        loggedInUserBalance -= amount;
        updateBalance();
        logTransaction("Withdraw", amount, "");
        cout << "Amount withdrawn successfully!" << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Check balance function
void checkBalance() {
    cout << "\nYour current balance is: " << fixed << setprecision(2) << loggedInUserBalance << endl;
}

// Account statement function
void accountStatement() {
    try {
        ifstream transactions("transactions.txt");
        if (!transactions) {
            throw runtime_error("Error opening transactions file!");
        }

        string line;
        cout << "\nAccount Statement:" << endl;
        while (getline(transactions, line)) {
            if (line.find(loggedInUserEmail) != string::npos) {
                cout << line << endl;
            }
        }
        transactions.close();
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Money transfer function
void moneyTransfer() {
    try {
        string recipientPhone;
        double amount;

        cout << "\nEnter recipient's phone number: ";
        cin >> recipientPhone;

        ifstream userData("userData.txt");
        if (!userData) {
            throw runtime_error("Error opening user data file!");
        }

        string fileFullname, fileEmail, filePhone, filePassword, filePin;
        double fileBalance;
        bool recipientFound = false;

        while (userData >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance) {
            if (filePhone == recipientPhone) {
                recipientFound = true;
                break;
            }
        }
        userData.close();

        if (!recipientFound) {
            throw invalid_argument("Recipient not found!");
        }

        cout << "Enter amount to transfer: ";
        cin >> amount;

        if (amount <= 0 || amount > loggedInUserBalance) {
            throw invalid_argument("Invalid amount or insufficient balance!");
        }

        // Deduct amount from sender and update recipient balance
        double recipientBalance = fileBalance + amount;
        loggedInUserBalance -= amount;

        // Update the balances in the file
        ifstream inputFile("userData.txt");
        ofstream tempFile("temp.txt");
        if (!inputFile || !tempFile) {
            throw runtime_error("Error updating user data!");
        }

        while (inputFile >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance) {
            if (filePhone == loggedInUserPhoneNumber) {
                tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " << filePassword << " "
                         << filePin << " " << loggedInUserBalance << endl;
            } else if (filePhone == recipientPhone) {
                tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " << filePassword << " "
                         << filePin << " " << recipientBalance << endl;
            } else {
                tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " << filePassword << " "
                         << filePin << " " << fileBalance << endl;
            }
        }
        inputFile.close();
        tempFile.close();

        // Replace old file with updated file
        remove("userData.txt");
        rename("temp.txt", "userData.txt");

        // Log the transaction
        logTransaction("Transfer", amount, "To: " + recipientPhone);
        cout << "Transaction successful! You transferred " << fixed << setprecision(2) << amount << " to " << recipientPhone << ".\n";
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Utility functions (unchanged)
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
        throw runtime_error("Error saving user details!");
    }
    userData << fullname << " " << email << " " << phone << " " << password << " " << pin << " 0.0" << endl;
    userData.close();
}

void updateBalance() {
    ifstream inputFile("userData.txt");
    ofstream tempFile("temp.txt");
    if (!inputFile || !tempFile) {
        throw runtime_error("Error updating balance!");
    }

    string fileFullname, fileEmail, filePhone, filePassword, filePin;
    double fileBalance;

    while (inputFile >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance) {
        if (filePhone == loggedInUserPhoneNumber) {
            tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " << filePassword << " "
                     << filePin << " " << loggedInUserBalance << endl;
        } else {
            tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " << filePassword << " "
                     << filePin << " " << fileBalance << endl;
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
    char *dt = ctime(&now);
    transactions << loggedInUserEmail << " | " << type << " | " << fixed << setprecision(2) << amount
                 << " | " << details << " | " << dt;
    transactions.close();
}