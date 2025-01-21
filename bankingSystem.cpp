#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <vector>
using namespace std;

bool isLoggedIn = false; // Track login status

// Function prototypes
void Register();
int Login();
int forgotPass();
int depositMoney() {};
int withdrawMoney() {};
int balance() {};
int accountStatement() {};
int moneyTransfer() {};
int pinChange() {};
int deleteAccount() {};
int loanManagement() {};
int Logout();

// File name for storing user data
const string userDataFile = "users.txt";

int main()
{
    int choice;
    do
    {
        if (isLoggedIn)
        {
            cout << "==========Banking Management System==========" << endl;
            cout << "1. Deposit Money" << endl;
            cout << "2. Withdraw Money" << endl;
            cout << "3. Check Account Balance" << endl;
            cout << "4. Account Statement" << endl;
            cout << "5. Money Transfer" << endl;
            cout << "6. Delete My Account" << endl;
            cout << "7. Loan Management" << endl;
            cout << "8. Logout" << endl;

            cout << "Enter any choice [1-8]: ";
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
                balance();
                break;

            case 4:
                accountStatement();
                break;

            case 5:
                moneyTransfer();
                break;

            case 6:
                deleteAccount();
                break;

            case 7:
                loanManagement();
                break;

            case 8:
                Logout();
                break;
            }
        }
        else
        {
            int userChoice;
            cout << "First you have to Create an Account" << endl;
            cout << "1. Register" << endl;
            cout << "2. Already Have an Account? Login" << endl;
            cout << "3. Forgot Password" << endl;
            cout << "Enter your choice: ";
            cin >> userChoice;
            if (userChoice == 1)
            {
                Register();
            }
            else if (userChoice == 2)
            {
                Login();
            }
            else
            {
                forgotPass();
            }
        }
    } while (choice != 9);

    return 0;
}

// Register New user
void Register()
{
    string fullName;
    string userEmail;
    string userphoneNumber;
    string userpassword;
    bool emailExist = false;
    bool numberExist = false;

    cout << "For your registration, we need your details" << endl;
    cout << "Enter your Full Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
    getline(cin, fullName);

    cout << "Enter your Email: ";
    getline(cin, userEmail);

    cout << "Enter your phone Number: ";
    getline(cin, userphoneNumber);

    cout << "Enter your Password: ";
    getline(cin, userpassword);

    ifstream inputFile("users.txt");
    if (inputFile.is_open()) {
        string fileName, fileEmail, filePhone, filePassword;
        while (inputFile >> fileName >> fileEmail >> filePhone >> filePassword) {
            if (fileEmail == userEmail) {
                emailExist = true;
            }
            if (filePhone == userphoneNumber) {
                numberExist = true;
            }
        }
        inputFile.close();
    }

    if (emailExist) {
        cout << "This email is already registered." << endl;
    }
    if (numberExist) {
        cout << "This phone number is already registered." << endl;
    }

    if (!emailExist && !numberExist) {
        ofstream outputFile("users.txt", ios::app);
        if (outputFile.is_open()) {
            outputFile << fullName << " " << userEmail << " " << userphoneNumber << " " << userpassword << "\n";
            outputFile.close();
            cout << "Congratulations, you are successfully registered!" << endl;
            isLoggedIn = true;
        } else {
            cerr << "Error: Unable to open the file for writing." << endl;
        }
    }
}


// Login
int Login()
{
    string loginEmail;
    string loginPass;
    bool isUserExist = false;

    cout << "Enter your Email: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer completely
    getline(cin, loginEmail);

    cout << "Enter your Password: ";
    getline(cin, loginPass);

    ifstream inFile(userDataFile);
    string fileFullName, fileEmail, filePhone, filePassword;

    while (inFile >> fileFullName >> fileEmail >> filePhone >> filePassword)
    {
        if (fileEmail == loginEmail && filePassword == loginPass)
        {
            isUserExist = true;
            break;
        }
    }
    inFile.close();

    if (isUserExist)
    {
        cout << "Successfully Logged In." << endl;
        isLoggedIn = true;
    }
    else
    {
        cout << "Invalid email or password." << endl;
    }
}

// Forgot Password
int forgotPass()
{
    string userEmail;
    cout << "Enter your registered Email: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer completely
    getline(cin, userEmail);

    ifstream inFile(userDataFile);
    string fileFullName, fileEmail, filePhone, filePassword;

    while (inFile >> fileFullName >> fileEmail >> filePhone >> filePassword)
    {
        if (fileEmail == userEmail)
        {
            cout << "Your password is: " << filePassword << endl;
            inFile.close();
            return 0;
        }
    }
    inFile.close();

    cout << "No account found with the given email." << endl;
    return 0;
}

// Logout
int Logout()
{
    cout << "Logout successfully." << endl;
    isLoggedIn = false;
    return 0;
}
