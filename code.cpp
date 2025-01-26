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
double loggedInUserLoanAmount = 0.0;  // New loan amount variable
int loggedInUserLoanDuration = 0;     // New loan duration variable

// Function prototypes
void Register();
void Login();
void Logout();
void depositMoney();
void withdrawMoney();
void checkBalance();
void accountStatement();
void moneyTransfer();
void takeLoan();       // New function prototype for taking loan
void repayLoan();      // New function prototype for repaying loan

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
            cout << "6. Take Loan" << endl;
            cout << "7. Repay Loan" << endl;
            cout << "8. Logout" << endl;

            cout << "Enter your choice [1-8]: ";
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
                    takeLoan();
                    break;
                case 7:
                    repayLoan();
                    break;
                case 8:
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

// Function to take a loan
void takeLoan()
{
    if (loggedInUserLoanAmount > 0)
    {
        cout << "\nYou already have an active loan. Please repay existing loan first." << endl;
        return;
    }

    double loanAmount;
    int loanDuration;

    cout << "\nEnter loan amount: ";
    cin >> loanAmount;

    if (loanAmount <= 0)
    {
        cout << "Invalid loan amount!" << endl;
        return;
    }

    // Basic loan eligibility: max loan is 5 times current balance
    if (loanAmount > (loggedInUserBalance * 5))
    {
        cout << "Loan amount exceeds your eligibility!" << endl;
        return;
    }

    cout << "Enter loan duration in months (3-12 months): ";
    cin >> loanDuration;

    if (loanDuration < 3 || loanDuration > 12)
    {
        cout << "Loan duration must be between 3 and 12 months!" << endl;
        return;
    }

    // Simple interest calculation (10% annual interest)
    double interestRate = 0.10; 
    double totalInterest = loanAmount * interestRate * (loanDuration / 12.0);
    double totalRepayAmount = loanAmount + totalInterest;

    // Confirm loan
    char confirm; 
    cout << "Loan Details:" << endl; 
    cout << "Loan Amount: " << fixed << setprecision(2) << loanAmount<<endl; 
    cout<<"Loan Duration: "<<loanDuration<<" months"<<endl; 
    cout<<"Total Repayment Amount: "<<totalRepayAmount<<endl; 
    cout<<"Monthly Repayment: "<<totalRepayAmount/loanDuration<<endl; 
    cout<<"Confirm loan? (y/n): "; 
    cin>>confirm;

   if(confirm == 'y' || confirm == 'Y')
   {
       loggedInUserBalance += loanAmount; 
       loggedInUserLoanAmount = totalRepayAmount; 
       loggedInUserLoanDuration = loanDuration;

       updateBalance(); 
       logTransaction("Loan", loanAmount, "Loan Disbursed");

       cout<<"Loan approved and amount credited to your account!"<<endl; 
   }
   else
   {
       cout<<"Loan request cancelled."<<endl; 
   }
}

// Function to repay a loan
void repayLoan()
{
   if (loggedInUserLoanAmount <= 0)
   {
       cout<<"\nYou have no active loan to repay."<<endl; 
       return; 
   }

   double repaymentAmount; 
   cout<<"\nOutstanding Loan Amount: "<<fixed<<setprecision(2)<<loggedInUserLoanAmount<<endl; 
   cout<<"Enter amount to repay: "; 
   cin>>repaymentAmount;

   if(repaymentAmount <= 0 || repaymentAmount > loggedInUserBalance)
   {
       cout<<"Invalid repayment amount or insufficient balance!"<<endl; 
       return; 
   }

   // Repay the loan
   loggedInUserBalance -= repaymentAmount; 
   loggedInUserLoanAmount -= repaymentAmount;

   // Update balance in file
   updateBalance(); 
   logTransaction("Loan Repayment", repaymentAmount, "Loan Repayment");

   // Check if the loan is fully repaid
   if(loggedInUserLoanAmount <= 0)
   {
       loggedInUserLoanAmount = 0; 
       loggedInUserLoanDuration = 0; 
       cout<<"Congratulations! Loan fully repaid."<<endl; 
   }
   else
   {
       cout<<"Partial loan repayment successful. Remaining loan: "<<fixed<<setprecision(2)<<loggedInUserLoanAmount<<endl; 
   }
}

// Save user details to file
void saveUserDetails(const string &fullname, const string &email, const string &phone, const string &password, const string &pin) {
     ofstream userData("userData.txt", ios::app); 
     if (userData) { 
         userData<<fullname<<" "<<email<<" "<<phone<<" "<<password<<" "<<pin<<" "<<loggedInUserBalance<<" "<<loggedInUserLoanAmount<<" "<<loggedInUserLoanDuration<<endl; 
     } 
     userData.close(); 
}

void Register()
{
     string fullname, email, phone, password, pin; 

     cout<<"\nEnter Full Name: "; 
     cin.ignore(); 
     getline(cin, fullname); 

     if (fullname.length() < 4) { 
         cout<<"Full name must be at least 4 characters long!"<<endl; 
         return; 
     }

     cout<<"Enter Email: "; getline(cin, email); 

     if (!validateEmail(email)) { 
         cout<<"Invalid email format!"<<endl; return; 
     }

     cout<<"Enter Phone Number (11 digits): "; getline(cin, phone); 

     if (!validatePhoneNumber(phone)) { 
         cout<<"Invalid phone number!"<<endl; return; 
     }

     // Check if email or phone already exists
     ifstream userData("userData.txt"); 

     string fileFullname, fileEmail, filePhone, filePassword, filePin; 

     bool emailExists = false, phoneExists = false;

     while (userData >> fileFullname >> fileEmail >> filePhone >> filePassword >> filePin) { 

         if (fileEmail == email) { emailExists = true; } 

         if (filePhone == phone) { phoneExists = true; } 

     } 

     userData.close();

     if (emailExists) { 

         cout<<"Email already exists!"<<endl; return; 

     }

     if (phoneExists) { 

         cout<<"Phone number already exists!"<<endl; return;

     }

     // Password validation
     cout<<"Enter Password (minimum 5 characters): "; getline(cin, password); 

     if (!validatePassword(password)) { 

         cout<<"Password must be at least 5 characters long!"<<endl; return;

      }

      // PIN validation
      cout<<"Enter 4-digit PIN: "; getline(cin, pin); 

      if (!validatePin(pin)) { 

          cout<<"PIN must be exactly 4 digits!"<<endl; return;

      }

      // Save new user details
      saveUserDetails(fullname, email, phone, password, pin);

      // After successful registration
      strcpy(loggedInUserFullname, fullname.c_str());
      strcpy(loggedInUserEmail, email.c_str());
      strcpy(loggedInUserPhoneNumber, phone.c_str());
      strcpy(loggedInUserPass, password.c_str());
      strcpy(loggedInUserPin, pin.c_str());
      
      isLoggedIn = true;  
      loggedInUserBalance = 0.0;

      cout<<"Registration successful! You are now logged in."<<endl;

}

void Login() {
    
   string email,password;

   // User input for login credentials.
   cout<<"\nEnter Email: "; cin.ignore(); getline(cin,email);
   cout<<"Enter Password: "; getline(cin,password);

   // Open user data file for reading credentials.
   ifstream userData("userData.txt");
   
   string fileFullname,fileEmail,filePhone,filePassword,filePin;
   
   double fileBalance,fileLoanAmount=0.0 ;
   
   int fileLoanDuration=0 ;
   
   bool found=false;

   while(userData>>fileFullname>>fileEmail>>filePhone>>filePassword>>filePin>>fileBalance>>fileLoanAmount>>fileLoanDuration){
       if(fileEmail==email && filePassword==password){
           found=true;

           // Store user details after successful login.
           strcpy(loggedInUserFullname,fileFullname.c_str());
           strcpy(loggedInUserEmail,fileEmail.c_str());
           strcpy(loggedInUserPhoneNumber,filePhone.c_str());
           strcpy(loggedInUserPass,filePassword.c_str());
           strcpy(loggedInUserPin,filePin.c_str());
           loggedInUserBalance=fileBalance ;
           loggedInUserLoanAmount=fileLoanAmount ;
           loggedInUserLoanDuration=fileLoanDuration ;
           break ;
       }
   }
   
   userData.close();

   if(found){
       isLoggedIn=true ;
       cout<<"Login successful!"<<endl ;
   }else{
       cout<<"Invalid email or password!"<<endl ;
   }
}

void Logout() {
    
isLoggedIn=false ; 
    
cout<<"You have been logged out."<<endl ; 
    
}

void depositMoney() {

double amount ; 
    
cout<<"\nEnter the amount to deposit: "; 
    
cin>>amount ;

if(amount<=0){

cout<<"Invalid amount!"<<endl ; 
    
return ; 
    
}

loggedInUserBalance+=amount ; 
    
updateBalance(); 
    
logTransaction("Deposit",amount,""); 
    
cout<<"Amount deposited successfully!"<<endl ; 
    
}

void withdrawMoney() {

double amount ; 
    
string pin ; 
    
cout<<"\nEnter the amount to withdraw: "; 
    
cin>>amount ;

if(amount<=0 || amount>loggedInUserBalance){

cout<<"Invalid or insufficient balance!"<<endl ; 
    
return ; 
    
}

cout<<"Enter your 4-digit PIN: "; 
    
cin>>pin ;

if(pin!=loggedInUserPin){

cout<<"Incorrect PIN!"<<endl ; 
    
return ; 
    
}

loggedInUserBalance-=amount ; 
    
updateBalance(); 
    
logTransaction("Withdraw",amount,""); 
    
cout<<"Amount withdrawn successfully!"<<endl ; 
    
}

void checkBalance() {

cout<<"\nYour current balance is: "<<fixed<<setprecision(2)<<loggedInUserBalance<<endl ; 

}

void accountStatement() {

ifstream transactions("transactions.txt"); 
    
string line ; 
    
cout<<"\nAccount Statement:"<<endl ; 

while(getline(transactions,line)) {

if(line.find(loggedInUserEmail)!=string::npos) {

cout<<line<<endl ; 

} 

} 

transactions.close(); 

}

void moneyTransfer() {

string recipientPhone ; 
    
double amount ;

cout<<"\nEnter recipient's phone number: "; 
    
cin>>recipientPhone ;

ifstream userData("userData.txt"); 
    
if(!userData) {

cout<<"Error opening user data file! Unable to process transfer.\n"; 
        
return ; 
        
}

string fileFullname,fileEmail,filePhone,filePassword,filePin ; 
    
double fileBalance ; 
    
bool recipientFound=false ;

while(userData>>fileFullname>>fileEmail>>filePhone>>filePassword>>filePin>>fileBalance) {

if(filePhone==recipientPhone) {

recipientFound=true ; 
        
break ; 
        
} 
        
} 
        
userData.close();

if(!recipientFound) {

cout<<"Recipient with phone number "<<recipientPhone <<" not found.\n"; 
        
return ; 
        
}

cout<<"Enter amount to transfer: "; 
        
cin>>amount ;

if(amount<=0 || amount>loggedInUserBalance) {

cout<<"Invalid amount or insufficient balance.\n"; 
        
return ; 
        
}

// Deduct from sender and update recipient balance.
double recipientBalance=fileBalance+amount ;
loggedInUserBalance-=amount;

// Update balances in the file.
ifstream inputFile("userData.txt");
ofstream tempFile("temp.txt");
while(inputFile>>fileFullname>>fileEmail>>filePhone>>filePassword>>filePin>>fileBalance) {

if(filePhone==loggedInUserPhoneNumber) {

tempFile<<fileFullname <<" "<<fileEmail <<" "<<filePhone <<" "<<filePassword <<" "
         		<<filePin <<" "<<loggedInUserBalance<<endl ;

} else if(filePhone==recipientPhone) {

tempFile<<fileFullname <<" "<<fileEmail <<" "<<filePhone <<" "<<filePassword <<" "
         		<<filePin <<" "<<recipientBalance<<endl ;

} else {

tempFile<<fileFullname <<" "<<fileEmail <<" "<<filePhone <<" "<<filePassword <<" "
         		<<filePin <<" "<<fileBalance<<endl ;

}
}
inputFile.close(); tempFile.close();

// Replace old file with updated one.
remove("userData.txt");
rename("temp.txt", "userData.txt");

// Log transaction.
logTransaction("Transfer", amount,"To: "+recipientPhone);
cout<<"Transaction successful! You transferred "<<fixed<<setprecision(2)<<amount <<" to "
		<<recipientPhone<<".\n";
}

// Validation functions

bool validateEmail(const string &email) {
	const regex pattern(R"((\w+)(\.{1}\w+)*@(\w+)(\.\w+)+)");
	return regex_match(email, pattern);
}

bool validatePhoneNumber(const string &phone) {
	if(phone.length()!=11)return false ;
	for(char c : phone){
		if(!isdigit(c))return false ;
	}
	return true ;
}

bool validatePassword(const string &password) {
	return password.length()>=5 ;
}

bool validatePin(const string &pin) {
	if(pin.length()!=4)return false ;
	for(char c : pin){
		if(!isdigit(c))return false ;
	}
	return true ;
}

// Function to log transactions
void logTransaction(const string &type,double amount,const string &details){
	ofstream transactionLog("transactions.txt",ios::app);
	if(transactionLog){
		time_t now=time(0);
		tm *ltm=localtime(&now);
		transactionLog<<(ltm->tm_year+1900)<<"/"<<(ltm->tm_mon+1)<<"/"<<(ltm->tm_mday)<<"\t"
			<<(ltm->tm_hour)<<":"<<(ltm->tm_min)<<":"<<(ltm->tm_sec)<<"\t"<<type<<"\t"
			<<fixed<<setprecision(2)<<amount<<"\t"<<details<<"\n";
	}
	transactionLog.close();
}

// Function to update balance in the data file after transactions
void updateBalance() {
	ifstream userData("userData.txt");
	ofstream tempFile("temp.txt");
	string line;

	while(getline(userData,line)){
		if(line.find(loggedInUserEmail)!=string::npos){
			tempFile<<(loggedInUserFullname)<<"\t"<<(loggedInUserEmail)<<"\t"<<(loggedInUserPhoneNumber)
				<<"\t"<<(loggedInUserPass)<<"\t"<<(loggedInUserPin)<<"\t"
				<<(loggedInUserBalance)<<"\t"<<(loggedInUserLoanAmount)<<"\t"<<(loggedInUserLoanDuration)<<'\n';
		}else{
			tempFile<<(line)<<'\n';
		}
	}
	userData.close();
	tempFile.close();

	remove("userData.txt");
	rename("temp.txt","userData.txt");
}
