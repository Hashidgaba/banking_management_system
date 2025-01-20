#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Function prototypes
void registerUser();
bool loginUser();
void forgotPassword();
bool checkUserExists(const string& email, const string& password);
string retrievePassword(const string& email);
void logoutUser();
string trim(const string& str);  // Helper function to remove leading/trailing spaces

bool loggedIn = false;  // Track login status

int main() {
    int choice;
    do {
        cout << "\n=== Banking Management System ===\n";
        if (!loggedIn) {
            cout << "1. Register\n";
            cout << "2. Login\n";
        } else {
            cout << "1. Forgot Password\n";
            cout << "2. Logout\n";
        }
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (!loggedIn) {
                    registerUser();
                } else {
                    forgotPassword();
                }
                break;
            case 2:
                if (!loggedIn) {
                    if (loginUser()) {
                        loggedIn = true;
                        cout << "Login successful!\n";
                    } else {
                        cout << "Invalid email or password.\n";
                    }
                } else {
                    logoutUser();
                }
                break;
            case 3:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}

// Function to register a new user
void registerUser() {
    string email, phone, password;
    cout << "Enter your email: ";
    cin >> email;
    cout << "Enter your phone number: ";
    cin >> phone;
    cout << "Enter a password: ";
    cin >> password;

    ofstream outFile("users.txt", ios::app);
    if (!outFile) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    // Store user data in the format: email phone password
    outFile << email << " " << phone << " " << password << "\n";
    outFile.close();

    cout << "User registered successfully! You can now log in.\n";
}

// Function to handle user login
bool loginUser() {
    string email, password;
    cout << "Enter your email: ";
    cin >> email;
    cout << "Enter your password: ";
    cin >> password;

    return checkUserExists(email, password);
}

// Function for forgot password
void forgotPassword() {
    string email;
    cout << "Enter your registered email: ";
    cin >> email;

    string password = retrievePassword(email);
    if (password.empty()) {
        cout << "No account found with this email.\n";
    } else {
        cout << "Your password is: " << password << "\n";
    }
}

// Function to check if a user exists in the file with given email and password
bool checkUserExists(const string& email, const string& password) {
    ifstream inFile("users.txt");
    if (!inFile) {
        cerr << "Error opening file for reading.\n";
        return false;
    }

    string fileEmail, filePhone, filePassword;
    while (inFile >> fileEmail >> filePhone >> filePassword) {
        // Trim extra spaces and newlines from file data
        fileEmail = trim(fileEmail);
        filePassword = trim(filePassword);

        if (fileEmail == email && filePassword == password) {
            return true;
        }
    }

    return false;
}

// Function to retrieve password for a given email
string retrievePassword(const string& email) {
    ifstream inFile("users.txt");
    if (!inFile) {
        cerr << "Error opening file for reading.\n";
        return "";
    }

    string fileEmail, filePhone, filePassword;
    while (inFile >> fileEmail >> filePhone >> filePassword) {
        // Trim extra spaces and newlines from file data
        fileEmail = trim(fileEmail);
        filePassword = trim(filePassword);

        if (fileEmail == email) {
            return filePassword;
        }
    }

    return "";
}

// Function to log out the user
void logoutUser() {
    loggedIn = false;
    cout << "You have been logged out successfully.\n";
}

// Helper function to trim leading and trailing spaces
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');

    if (first == string::npos || last == string::npos) {
        return "";  // empty string if no non-space characters
    }

    return str.substr(first, (last - first + 1));
}
