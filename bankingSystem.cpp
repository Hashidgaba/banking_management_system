#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <vector>
using namespace std;
// initialize all global variables used in this proejct
vector<string> Name;
vector<string> Email;
vector<string> password;
vector<string> Number;
bool isLoggedIn = false;
// initialize all functions which is used in this project
void Register();
int Login();
int forgotPass() {};
int depositMoney() {};
int withdrawMoney() {};
int balance() {};
int accountStatement() {};
int moneyTransfer() {};
int pinChange() {};
int deleteAccount() {};
int loanManagement() {};
int Logout();

int main()
{
    int choice;
    do
    {
        if (isLoggedIn)
        {
            cout << "==========Banking Mnanagement System==========" << endl;
            cout << "1. Deposit Money" << endl;
            cout << "2. Withdraw Money" << endl;
            cout << "3. Check Account Balance" << endl;
            cout << "4. Account Statement" << endl;
            cout << "5. Money Transfer" << endl;
            cout << "6. Delete My account" << endl;
            cout << "7. Loan management" << endl;
            cout << "8. Logout" << endl;

            cout << "Enter any choice [1-8]" << endl;
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
        // if user is not loggedIn
        else
        {
            int userChoice;
            cout << "First you have to Create Account" << endl;
            cout << "1. Register" << endl;
            cout << "2. Already Have an account, Login" << endl;
            cout << "3. Forgot Password" << endl;
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
    cout << "For you Registeration we need your details" << endl;
    cout << "Enter your Full Name" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer completely
    getline(cin, fullName);

    cout << "Enter your Email" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer completely
    getline(cin, userEmail);

    cout << "Enter your phone Number" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer completely
    getline(cin, userphoneNumber);

    cout << "Password" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer completely
    getline(cin, userpassword);

    for (int i = 0; i < Email.size(); i++)
    {
        if (Email[i] == userEmail)
        {
            cout << "This email is already exist" << endl;
            emailExist = true;
        }

        if (Number[i] == userphoneNumber)
        {
            cout << "This Number is already Exist" << endl;
            numberExist = true;
        }
    }

    if (!emailExist && !numberExist)
    {
        Name.push_back(fullName);
        Email.push_back(userEmail);
        Number.push_back(userphoneNumber);
        password.push_back(userpassword);
        cout << "Congratulations you are successfully Register" << endl;
        isLoggedIn = true;
    }
}

int Logout()
{
    cout << "Logout successfully" << endl;
    isLoggedIn = false;
}

int Login()
{
    string loginEmail;
    string loginPass;
    bool isUserexist = false;
    cout << "Enter your Email" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer completely
    getline(cin, loginEmail);

    cout << "Enter your password" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer completely
    getline(cin, loginPass);

    for (int i = 0; i < Email.size(); i++)
    {
        if (Email[i] == loginEmail && password[i] == loginPass)
        {
            isUserexist = true;
        }
    }

    if(isUserexist){
        cout << "Succesfully Login" << endl;
        isLoggedIn = true;
    }else{
        cout << "User not found" ;
    }
}