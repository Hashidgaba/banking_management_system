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
int depositMoney();
int withdrawMoney();
int balance();
int accountStatement();
int moneyTransfer();
int pinChange() {};
int deleteAccount() {};
int loanManagement() {};
int Logout();

//Deposit Money Implementation;
int depositMoney() {
    if (!isLoggedIn) {
        cout << "Please login first!" << endl;
        return 0;
    }
    
    double amount;
    string email;
    cout << "Enter the amount to deposit: ";
    cin >> amount;
    
    if (amount <= 0) {
        cout << "Invalid amount. Please enter a positive number." << endl;
        return 0;
    }

    cout << "Enter your email to confirm: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, email);

    // Read the current balance from transactions file
    ifstream balanceFile(email + "_balance.txt");
    double currentBalance = 0.0;
    if (balanceFile.is_open()) {
        balanceFile >> currentBalance;
        balanceFile.close();
    }

    // Update balance
    currentBalance += amount;

    // Save the new balance
    ofstream newBalanceFile(email + "_balance.txt");
    if (newBalanceFile.is_open()) {
        newBalanceFile << fixed << setprecision(2) << currentBalance;
        newBalanceFile.close();

        // Record the transaction
        ofstream transactionFile(email + "_transactions.txt", ios::app);
        if (transactionFile.is_open()) {
            time_t now = time(0);
            string date = ctime(&now);
            transactionFile << date.substr(0, date.length()-1) << " - Deposit: +" 
                          << fixed << setprecision(2) << amount 
                          << " - Balance: " << currentBalance << endl;
            transactionFile.close();
        }

        cout << "Successfully deposited $" << fixed << setprecision(2) << amount << endl;
        cout << "Current balance: $" << currentBalance << endl;
    } else {
        cout << "Error processing deposit. Please try again." << endl;
    }

    return 0;
}
//check balance 
int balance() {
    if (!isLoggedIn) {
        cout << "Please login first!" << endl;
        return 0;
    }

    string email;
    cout << "Enter your email to check your balance: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, email);

    ifstream balanceFile(email + "_balance.txt");
    if (balanceFile.is_open()) {
        double currentBalance;
        balanceFile >> currentBalance;
        balanceFile.close();

        cout << "Your current balance is: $" << fixed << setprecision(2) << currentBalance << endl;
    } else {
        cout << "No balance information found for this email. Please make a deposit first." << endl;
    }

    return 0;
}
//withdraw money
int withdrawMoney() {
    if (!isLoggedIn) {
        cout << "Please login first!" << endl;
        return 0;
    }

    double amount;
    string email;
    cout << "Enter the amount to withdraw: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Invalid amount. Please enter a positive number." << endl;
        return 0;
    }

    cout << "Enter your email to confirm: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, email);

    // Read the current balance from the balance file
    ifstream balanceFile(email + "_balance.txt");
    double currentBalance = 0.0;
    if (balanceFile.is_open()) {
        balanceFile >> currentBalance;
        balanceFile.close();
    } else {
        cout << "No balance information found for this email. Please make a deposit first." << endl;
        return 0;
    }

    // Check if there is enough balance
    if (amount > currentBalance) {
        cout << "Insufficient balance. Your current balance is: $" << fixed << setprecision(2) << currentBalance << endl;
        return 0;
    }

    // Update the balance
    currentBalance -= amount;

    // Save the new balance
    ofstream newBalanceFile(email + "_balance.txt");
    if (newBalanceFile.is_open()) {
        newBalanceFile << fixed << setprecision(2) << currentBalance;
        newBalanceFile.close();

        // Record the transaction
        ofstream transactionFile(email + "_transactions.txt", ios::app);
        if (transactionFile.is_open()) {
            time_t now = time(0);
            string date = ctime(&now);
            transactionFile << date.substr(0, date.length() - 1) << " - Withdrawal: -$"
                            << fixed << setprecision(2) << amount
                            << " - Balance: $" << currentBalance << endl;
            transactionFile.close();
        }

        cout << "Successfully withdrew $" << fixed << setprecision(2) << amount << endl;
        cout << "Current balance: $" << currentBalance << endl;
    } else {
        cout << "Error processing withdrawal. Please try again." << endl;
    }

    return 0;
}
//Account Statements
int accountStatement() {
    if (!isLoggedIn) {
        cout << "Please login first!" << endl;
        return 0;
    }

    string email;
    cout << "Enter your email to view account statement: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, email);

    string transactionFileName = email + "_transactions.txt";

    ifstream transactionFile(transactionFileName);
    if (!transactionFile.is_open()) {
        cout << "No transaction history found for this email." << endl;
        return 0;
    }

    cout << "========== Account Statement ==========" << endl;
    string line;
    while (getline(transactionFile, line)) {
        cout << line << endl;
    }
    transactionFile.close();
    cout << "=======================================" << endl;

    return 0;
}
// Money Transfer
int moneyTransfer() {
    if (!isLoggedIn) {
        cout << "Please login first!" << endl;
        return 0;
    }

    string senderEmail, recipientEmail;
    double amount;

    cout << "Enter your email: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, senderEmail);

    cout << "Enter recipient's email: ";
    getline(cin, recipientEmail);

    cout << "Enter the amount to transfer: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Invalid amount. Please enter a positive number." << endl;
        return 0;
    }

    // Read the sender's current balance
    double senderBalance = 0.0;
    ifstream senderBalanceFile(senderEmail + "_balance.txt");
    if (senderBalanceFile.is_open()) {
        senderBalanceFile >> senderBalance;
        senderBalanceFile.close();
    } else {
        cout << "Sender account not found." << endl;
        return 0;
    }

    // Check if the sender has enough balance
    if (senderBalance < amount) {
        cout << "Insufficient balance. Transfer failed." << endl;
        return 0;
    }

    // Read the recipient's current balance
    double recipientBalance = 0.0;
    ifstream recipientBalanceFile(recipientEmail + "_balance.txt");
    if (recipientBalanceFile.is_open()) {
        recipientBalanceFile >> recipientBalance;
        recipientBalanceFile.close();
    } else {
        cout << "Recipient account not found." << endl;
        return 0;
    }

    // Update balances
    senderBalance -= amount;
    recipientBalance += amount;

    // Save updated balances
    ofstream updateSenderBalance(senderEmail + "_balance.txt");
    ofstream updateRecipientBalance(recipientEmail + "_balance.txt");

    if (updateSenderBalance.is_open() && updateRecipientBalance.is_open()) {
        updateSenderBalance << fixed << setprecision(2) << senderBalance;
        updateRecipientBalance << fixed << setprecision(2) << recipientBalance;

        updateSenderBalance.close();
        updateRecipientBalance.close();

        // Log the transactions
        ofstream senderTransactionFile(senderEmail + "_transactions.txt", ios::app);
        ofstream recipientTransactionFile(recipientEmail + "_transactions.txt", ios::app);
        time_t now = time(0);
        string date = ctime(&now);
        date = date.substr(0, date.length() - 1); // Remove newline

        if (senderTransactionFile.is_open()) {
            senderTransactionFile << date << " - Transfer to " << recipientEmail
                                  << ": -" << fixed << setprecision(2) << amount
                                  << " - Balance: " << senderBalance << endl;
            senderTransactionFile.close();
        }

        if (recipientTransactionFile.is_open()) {
            recipientTransactionFile << date << " - Transfer from " << senderEmail
                                     << ": +" << fixed << setprecision(2) << amount
                                     << " - Balance: " << recipientBalance << endl;
            recipientTransactionFile.close();
        }

        cout << "Successfully transferred $" << fixed << setprecision(2) << amount << " to " << recipientEmail << endl;
    } else {
        cout << "Error processing the transfer. Please try again." << endl;
    }

    return 0;
}


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
