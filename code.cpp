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

// declare all functions
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

int main()
{
    int choice;
    do
    {
        if (isLoggedIn)
        {
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

            switch (choice)
            {
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
                cout << "Invalid choice. Try again." << endl;
            }
        }
        else
        {
            cout << "\n========== Banking Management System ==========" << endl;
            cout << "1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "Enter your choice [1-2]: ";

            cin >> choice;

            switch (choice)
            {
            case 1:
                Register();
                break;
            case 2:
                Login();
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
            }
        }
    } while (true);

    return 0;
}

void Register()
{
    string fullname, email, phone, password, pin;
    cout << "\nEnter Full Name: ";

    cin.ignore();
    getline(cin, fullname);

    if (fullname.length() < 4)
    {
        cout << "Full name must be at least 4 characters long!" << endl;
        return;
    }

    cout << "Enter Email: ";
    getline(cin, email);
    if (!validateEmail(email))
    {
        cout << "Invalid email format!" << endl;
        return;
    }

    cout << "Enter Phone Number (11 digits): ";
    getline(cin, phone);
    if (!validatePhoneNumber(phone))
    {
        cout << "Invalid phone number!" << endl;
        return;
    }
     // Check if email or phone already exists
    ifstream userData("userData.txt");
    string fileFullname, fileEmail, filePhone, filePassword, filePin;
    double fileBalance;
    bool emailExists = false, phoneExists = false;

    while (userData >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance)
    {
        if (fileEmail == email)
        {
            emailExists = true;
        }
        if (filePhone == phone)
        {
            phoneExists = true;
        }
    }
    userData.close();

    if (emailExists)
    {
        cout << "Email already exists!" << endl;
        return;
    }

    if (phoneExists)
    {
        cout << "Phone number already exists!" << endl;
        return;
    }

    cout << "Enter Password (minimum 5 characters): ";
    getline(cin, password);
    if (!validatePassword(password))
    {
        cout << "Password must be at least 5 characters long!" << endl;
        return;
    }

    cout << "Enter 4-digit PIN: ";
    getline(cin, pin);
    if (!validatePin(pin))
    {
        cout << "PIN must be exactly 4 digits!" << endl;
        return;
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
}


void Login() {
    string email, password;
    cout << "\nEnter Email: ";
    cin.ignore();
    getline(cin, email);

    cout << "Enter Password: ";
    getline(cin, password);

    ifstream userData("userData.txt");
    string fileFullname, fileEmail, filePhone, filePassword, filePin;
    double fileBalance;
    bool found = false;

    while (userData >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance) {
        if (fileEmail == email && filePassword == password) {
            found = true;
            // User details ko store karna
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

    if (found) {
        isLoggedIn = true;
        cout << "Login successful!" << endl;
    } else {
        cout << "Invalid email or password!" << endl;
    }
}


void Logout()
{
    isLoggedIn = false;
    cout << "You have been logged out." << endl;
}
//this func is used to add money
void depositMoney()
{
    double amount;
    cout << "\nEnter the amount to deposit: ";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "Invalid amount!" << endl;
        return;
    }

    loggedInUserBalance += amount;
    updateBalance();
    logTransaction("Deposit", amount, "");
    cout << "Amount deposited successfully!" << endl;

}

void withdrawMoney()
{
    double amount;
    string pin;
    cout << "\nEnter the amount to withdraw: ";
    cin >> amount;

    if (amount <= 0 || amount > loggedInUserBalance)
    {
        cout << "Invalid or insufficient balance!" << endl;
        return;
    }

    cout << "Enter your 4-digit PIN: ";
    cin >> pin;

    if (pin != loggedInUserPin)
    {
        cout << "Incorrect PIN!" << endl;
        return;
    }

    loggedInUserBalance -= amount;
    updateBalance();
    logTransaction("Withdraw", amount, "");
    cout << "Amount withdrawn successfully!" << endl;
}

void checkBalance()
{
    cout << "\nYour current balance is: " << fixed << setprecision(2) << loggedInUserBalance << endl;
}

void accountStatement()
{
    ifstream transactions("transactions.txt");
    string line;
    cout << "\nAccount Statement:" << endl;
    while (getline(transactions, line))
    {
        if (line.find(loggedInUserEmail) != string::npos)
        {
            cout << line << endl;
        }
    }
    transactions.close();
}
void moneyTransfer()
{
    string recipientPhone;
    double amount;

    cout << "\nEnter recipient's phone number: ";
    cin >> recipientPhone;

    // Open the file to check if the recipient exists
    ifstream userData("userData.txt");
    if (!userData)
    {
        cout << "Error opening user data file! Unable to process transfer.\n";
        return;
    }

    string fileFullname, fileEmail, filePhone, filePassword, filePin;
    double fileBalance;
    bool recipientFound = false;

    while (userData >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance)
    {
        if (filePhone == recipientPhone)
        {
            recipientFound = true;
            break;
        }
    }
    userData.close();

    if (!recipientFound)
    {
        cout << "Recipient with phone number " << recipientPhone << " not found.\n";
        return;
    }

    // Ask user for transfer amount
    cout << "Enter amount to transfer: ";
    cin >> amount;

    if (amount <= 0 || amount > loggedInUserBalance)
    {
        cout << "Invalid amount or insufficient balance.\n";
        return;
    }

    // Deduct amount from sender and update recipient balance
    double recipientBalance = fileBalance + amount;
    loggedInUserBalance -= amount;

    // Update the balances in the file
    ifstream inputFile("userData.txt");
    ofstream tempFile("temp.txt");
    while (inputFile >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin >> fileBalance)
    {
        if (filePhone == loggedInUserPhoneNumber)
        {
            tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " << filePassword << " "
                     << filePin << " " << loggedInUserBalance << endl;
        }
        else if (filePhone == recipientPhone)
        {
            tempFile << fileFullname << " " << fileEmail << " " << filePhone << " " << filePassword << " "
                     << filePin << " " << recipientBalance << endl;
        }
        else
        {
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
}

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
    if (userData) {
        userData << fullname << " " << email << " " << phone << " " << password << " " << pin << " 0.0" << endl;
    }
    userData.close();
}

void updateBalance() {
    ifstream inputFile("userData.txt");
    ofstream tempFile("temp.txt");

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
    if (transactions) {
        time_t now = time(0);
        char *dt = ctime(&now);
        transactions << loggedInUserEmail << " | " << type << " | " << fixed << setprecision(2) << amount
                     << " | " << details << " | " << dt;
    }
    transactions.close();
}


