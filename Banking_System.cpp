#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <iomanip>

using namespace std;

class Bank {
public:
    char name[40];
    char type[40];
    int pin, account_number, request;
    float balance;
    void new_account();
    void show_account() const;
    void modify_account();
    void report() const;
    void deposit(float);
    void withdraw(float);
    int return_accountnum() const;
    int return_balance() const;
    int return_pin () const;

};

void Bank::new_account() {
    srand(static_cast<unsigned int>(time(nullptr)));
    account_number = rand() % 900000 + 100000;
    cout << "\nYour Account Number: #" << account_number;
    cout << "\n\n Enter Your Name: ";
    cin.ignore();
    cin.getline(name, 40);
    cout << "\nEnter the type of the account (Checkings/Savings): ";
    cin >> type;
    cout << "\nEnter a pin for your account {5 digits}: ";
    cin >> pin;
    cout << "\nEnter a deposit: ";
    cin >> balance;
    cout << "\n Account was created successfully.";
}

void Bank::modify_account() {
    cout << "\n\n Enter Your New Name: ";
    cin.ignore();
    cin.getline(name, 40);
    cout << "\n Enter the new type of the account (Checkings/Savings): ";
    cin >> type;
    cout << "Enter a new pin for your account {5 digits}: ";
    cin >> pin;

}

void Bank::show_account() const {
    cout << "\nAccount No. : #" << account_number;
    cout << "\nAccount Holder Name : " << name;
    cout << "\nType of Account : " << type;
    cout << "\nBalance amount : $" << balance;
}

void Bank::report() const {
    cout << account_number << setw(10) << " " << name << setw(10) << " " << type << setw(10) << " " << balance << setw(10) << " " << request << endl;
}

void Bank::deposit(float n) {
    balance += n;
}

void Bank::withdraw(float n) {
    balance -= n;
}

int Bank::return_accountnum() const {
    return account_number;
}

int Bank::return_balance() const {
    return balance;
}

int Bank::return_pin() const
{
    return pin;
}

void create_account(); // creates a new account
void change_account(int); // modifies a specific user's account
void display_account(int); // displays a specific account
void delete_account(int);
void account_actions(int, int);
void display_all_accounts();
void sign_in_menu(); // function for sign-in menu
void verify_user(int, int);
void account_menu(int);
void request_account_deletion(int);
void verify_admin(int, string);
void admin_menu();


void create_account() {
    Bank obj;
    ofstream file;
    file.open("newdatabase.dat", ios::binary | ios::app);
    obj.new_account();
    file.write(reinterpret_cast<char *> (&obj), sizeof(Bank));
    file.close();
}

void change_account(int n) {
    Bank obj;
    bool flag = false;
    fstream file;
    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "File Could Not Be Opened.";
        return;
    }
    cout << "\nModifying Account\n";
    while (!file.eof() && flag == false) {
        file.read(reinterpret_cast<char *> (&obj), sizeof(Bank));
        if (obj.return_accountnum() == n) {
            obj.show_account();
            cout << "\nPlease enter your new details" << endl;
            obj.modify_account();
            int position = (-1) * static_cast<int>(sizeof(Bank)); // calculates the position where the object "obj" should be rewritten in the file "database". static_case<int> converts it into a integer.
            file.seekp(position, ios::cur); // this moves the file pointer by the value of position relative to the current position which is ios::cur.
            file.write(reinterpret_cast<char *> (&obj), sizeof(Bank)); // this is better over creating a copy of the file, deleting the original and renaming the copy to the original repeatedly.
            cout << "\nAccount was successfully modified.";
            flag = true;
        }
    }
    file.close();
    if (flag == false) {
        cout << "\nDatabase was not found." << endl;
    }
}

void delete_account(int n)
{
    Bank obj;
    ifstream file; // input file stream object, used to read data from the file
    ofstream file2; // output file stream , used to write data into the file
    file.open("newdatabase.dat", ios::binary);
    if (!file) {
        cout << "File Does Not Exist.";
        return;
    }
    file2.open("temp_newdatabase.dat", ios::binary); // creates a temp file that used to write
    file.seekg(0, ios::beg); // moves the file pointer to the beginning of the file
    while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank))) {
        if (obj.return_accountnum() != n) {
            file2.write(reinterpret_cast<char *> (&obj), sizeof(Bank));
        }
    }
    file.close();
    file2.close();
    remove("newdatabase.dat");
    rename("temp_newdatabase.dat", "newdatabase.dat");
    cout << "Account was successfully deleted.";
}

void display_account(int n)
{
    Bank obj;
    bool flag = false;
    ifstream file;
    file.open("newdatabase.dat", ios::binary);
    if (!file) {
        cout << "File Could Not Be Opened.";
        return;
    }

    cout << "\nAccount Details\n";
    while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank))) {
        if (obj.return_accountnum() == n) {
            obj.show_account();
            flag = true;
        }
    }
    file.close();
    if (flag == false) {
        cout << "\n\nAccount Number does not exist";
    }
}

void account_actions(int n, int option) {
    float test_balance, amount;
    bool flag = false;
    Bank obj;
    fstream file;
    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "File could not be opened." << endl;
        return;
    }
    while (!file.eof() && flag == false) {
        file.read(reinterpret_cast<char *> (&obj), sizeof(Bank));
        if (obj.return_accountnum() == n)
        {
            obj.show_account();
            if (option == 1) {
                cout << "\nDepositing Amount." << endl;
                cout << "Please enter the amount you would like to deposit: $";
                cin >> amount;
                obj.deposit(amount);
            }
            if (option == 2) {
                cout << "\nWithdrawing Amount." << endl;
                cout << "Please enter the amount you would like to withdraw: $";
                cin >> amount;
                test_balance = obj.return_balance() - amount;
                if (test_balance < 100) {
                    cout << "Unable to withdraw. Insufficient amount available. " << endl;
                    return;
                } else {
                    obj.withdraw(amount);
                }

            }
            int position = (-1) * static_cast<int>(sizeof(obj));
            file.seekp(position, ios::cur);
            file.write(reinterpret_cast<char *> (&obj), sizeof(Bank));
            cout << "Balance was successfully updated." << endl;
            flag = true;
        }
    }
}

void display_all_accounts()
{
    Bank obj;
    ifstream file;
    file.open("newdatabase.dat", ios::binary);
    if (!file) {
        cout << "File Could Not Be Opened";
        return;
    }
    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "==================================================================================\n";
    cout << "A/c no.          NAME            Type            Balance\n";
    cout << "==================================================================================\n";
    while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank))) {
        obj.report();
    }
    file.close();
}

void sign_in_menu()
{
    int choice, account_number, pin, adminac_num;
    string admin_pass;
    do {
        cout << "\n Welcome to the ATM Management System";
        cout << "\n 1. Sign In";
        cout << "\n 2. Create New Account";
        cout << "\n 3. Exit";
        cout << "\n Enter Your Choice : ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter your account number: ";
                cin >> account_number;
                cout << "Enter your PIN: ";
                cin >> pin;
                verify_user(account_number, pin);
                break;
            case 2:
                create_account();
                break;
            case 3:
                cout << "Exiting program.";
                exit(0);
            case 33050203:
                cout << "Please enter your Admin account number: #";
                cin >> adminac_num;
                cout << "Please enter your Admin password: ";
                cin >> admin_pass;
                verify_admin(adminac_num, admin_pass);
                break;
            default:
                cout << "Error: Invalid input. Try again.";
        }
        cin.ignore(); // Clear input buffer
        cin.get(); // Wait for user to press enter
    } while (choice != 3);
}

void verify_user(int ac_num, int pin)
{
    Bank obj;
    ifstream file;
    bool flag = false;
    file.open("newdatabase.dat", ios::binary);
    if (!file)
    {
        cout << "File Could Not Be Opened.";
        return;
    }
    while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank))) {
        if ((obj.return_accountnum() == ac_num) && (obj.return_pin() == pin))
        {
            cout << "\nHello " << obj.name << "! Please select one of the following.";
            account_menu(ac_num);
            flag = true;

        }
    }
    file.close();
    if (flag == false)
    {
        cout << "\n\nAccount Number does not exist";
    }

}

void request_account_deletion(int id)
{
    Bank obj;
    fstream file;
    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "File could not be opened." << endl;
        return;
    }
    bool flag = false;
    cout << "\nPlease note that your account will be put on hold for 30 days before deletion.";
    while (file.read(reinterpret_cast<char *>(&obj), sizeof(Bank))) {

        if (obj.return_accountnum() == id)
        {
            obj.request = 1; // Update the request variable
            int position = (-1) * static_cast<int>(sizeof(obj));
            file.seekp(position, ios::cur);
            file.write(reinterpret_cast<char *>(&obj), sizeof(Bank)); // Write the modified object back to the file
            flag = true;
            break;
        }
    }
    file.close();
    if (!flag)
    {
        cout << "\nAccount Number does not exist";
    }
}

void account_menu(int id)
{
    Bank obj;
    int choice;
    do {
        cout << "\n 1. Account Details";
        cout << "\n 2. Modify User Details";
        cout << "\n 3. Request Account Deletion";
        cout << "\n 4. Deposit into Account";
        cout << "\n 5. Withdraw from Account";
        cout << "\n 6. Log out";
        cout << "\n Enter Your Choice : ";
        cin >> choice;
        switch (choice)
        {
            case 1:
                display_account(id);
                break;
            case 2:
                change_account(id);
                break;
            case 3:
                request_account_deletion(id);
                break;
            case 4:
                account_actions(id, 1);
                break;
            case 5:
                account_actions(id, 2);
                break;
            case 6:
                cout << "Logging out...";
                sign_in_menu();
            default:
                cout << "Error: Invalid input. Try again.";
            }
            cin.ignore(); // Clear input buffer
            cin.get(); // Wait for user to press enter
        } while (choice != 7);
}

void verify_admin(int admin_n, string admin_p)
{
    if((admin_n == 101) && (admin_p == "test"))
    {
        admin_menu();
    }
    else
    {
        cout << "Oops... Looks like something went wrong.";
        return;
    }
}

void admin_menu()
{
    cout << "\nWelcome to the Admin menu.";
    int choice, ac_num;
    do
    {
        cout << "\n 1. Delete a user account";
        cout << "\n 2. View all users";
        cout << "\n 3. Log out";
        cout << "\n Enter your choice:";
        cin >> choice;

        switch(choice)
        {
            case 1:
                cout << "Please enter the user's account number: #";
                cin >> ac_num;
                delete_account(ac_num);
                break;
            case 2:
                display_all_accounts();
                break;
            case 3:
                cout << "\nLogging out...";
                sign_in_menu();
                break;
            default:
                cout << "Error: Invalid input. Try again.";
        }
        cin.ignore(); // Clear input buffer
        cin.get(); // Wait for user to press enter
    }while(choice != 7);

}



int main()
{
    sign_in_menu(); // Call the sign-in menu first
    return 0;
}
