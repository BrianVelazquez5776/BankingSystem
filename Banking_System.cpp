#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <iomanip>
#include <string.h>
#include <limits>

using namespace std;

struct TransactionRecord
{
    time_t timestamp;
    int account_number;
    char transaction_type;
    double amount;
};

void write_transaction_log(int account_number, char transaction_type, float amount)
{
    TransactionRecord record;
    record.timestamp = time(0);
    record.account_number = account_number;
    record.transaction_type = transaction_type;
    record.amount = amount;

    ofstream log_file("transaction_logs2.dat", ios::binary | ios::app);
    if (log_file.is_open())
    {
        log_file.write(reinterpret_cast<const char*>(&record), sizeof(TransactionRecord));
        log_file.close();
    }
    else
    {
        cout << "Unable to open transaction log file." << endl;
    }
}

class Bank {
public:
    char name[40];
    char type[20];
    int pin, account_number;
    char request = 'N', frozen = 'N';
    char answer1[40], answer2[40], answer3[40];
    float balance;
    void new_account();
    void show_account() const;
    void modify_account();
    void report() const;
    void deposit(float, int);
    void withdraw(float, int);
    int return_accountnum() const;
    int return_balance() const;
    char return_frozen() const;
    int return_pin () const;

};

void Bank::new_account()
{
    bool valid_type = false;
    srand(static_cast<unsigned int>(time(nullptr)));
    account_number = rand() % 900000 + 100000;
    cout << "\n Your Account Number: #" << account_number;
    cout << "\n Enter Your Name: ";
    cin.ignore();
    cin.getline(name, 40);
    while(strlen(name) == 0)
    {
        cout << "\n This field cannot be left empty. Please try again: ";
        cin.getline(name, 40);
    }

    cout << "\n Enter the type of the account (Checking/Savings): ";
    while (!valid_type)
    {
        cin >> type;
        for (int i = 0; type[i]; i++)
        {
            type[i] = tolower(type[i]);
        }
        if (strcmp(type, "checking") == 0 || strcmp(type, "savings") == 0)
        {
            valid_type = true;
        }
        else
        {
            cout << "\n Invalid account type. Please enter either (Checking or Savings): ";
        }
    }

    cout << "\n Enter a pin for your account {5 digits}: ";
    cin >> pin;
    while((pin < 10000) || (pin > 99999))
    {
        cout << "\n Invalid pin, please try again: ";
        cin >> pin;
    }

    cout << "\n Enter a deposit. (A minimum of $100 must be placed in in order to keep an account open.) : $";
    cin >> balance;
    while((balance < 100))
    {
        cout << "\n Invalid amount, please try again: $";
        cin >> balance;
    }

    cout << "\n For recovery purposes, please answer the following questions:";
    cout << "\n 1. What city were you born in? :";
    cin.ignore();
    cin.getline(answer1, 40);
    while(strlen(answer1) == 0)
    {
        cout << "\n This field cannot be left empty. Please try again: ";
        cin.getline(answer1, 40);
    }
    cout << "\n 2. What is the last name of your favorite elementary school teacher? :";
    cin.getline(answer2,40);
    while(strlen(answer2) == 0)
    {
        cout << "\n This field cannot be left empty. Please try again: ";
        cin.getline(answer2, 40);
    }
    cout << "\n 3. What was the first film you watched in theaters? :";
    cin.getline(answer3,40);
    while(strlen(answer3) == 0)
    {
        cout << "\n This field cannot be left empty. Please try again: ";
        cin.getline(answer3, 40);
    }
    cout << "\n Account was created successfully. (Press Enter)";
}

void Bank::modify_account()
{
    cout << " \n Enter a new pin for your account {5 digits}: ";
    cin >> pin;
    while((pin < 10000) || (pin > 99999))
    {
        cout << "\n Invalid pin, please try again: ";
        cin >> pin;
    }
    cout << "\n Successfully changed pin. (Press enter)";
}

void Bank::show_account() const
{
    cout << "\n Account No. : #" << account_number;
    cout << "\n Account Holder Name : " << name;
    cout << "\n Type of Account : " << type;
    cout << "\n Balance amount : $" << balance;
}

void Bank::report() const
{
    cout << setw(15) << left << account_number << setw(20) << name << setw(18) << type << setw(15) << balance << setw(15) << request << setw(15) << frozen << endl;
}

void Bank::deposit(float n, int option)
{
    balance += n;
    if (option == 1)
    {
        write_transaction_log(account_number, 'D', n);
    }
    else
    {
        write_transaction_log(account_number, 'R', n); // R receiving user
    }

}

void Bank::withdraw(float n, int option) {
    balance -= n;
    if (option == 1)
    {
        write_transaction_log(account_number, 'W', n);
    }
    else
    {
        write_transaction_log(account_number, 'S', n); // S for sender
    }
}

int Bank::return_accountnum() const
{
    return account_number;
}

int Bank::return_balance() const
{
    return balance;
}

int Bank::return_pin() const
{
    return pin;
}

char Bank::return_frozen () const
{
    return frozen;
}

void create_account(); // creates a new account
void change_account(int); // modifies a specific user's account
void display_account(int); // displays a specific account
void delete_account(int);
void account_actions(int, int);
void money_transfer(int);
void display_all_accounts();
void sign_in_menu(); // function for sign-in menu
void verify_user(int, int);
void account_menu(int);
void request_account_deletion(int);
void account_suspension(int);
void recover_account();
void verify_admin(int, string);
void admin_menu();
void write_transaction_log(int, char, float);
void display_transaction_logs(int);



void create_account()
{
    Bank obj;
    ofstream file;
    file.open("newdatabase.dat", ios::binary | ios::app);
    obj.new_account();
    file.write(reinterpret_cast<char *> (&obj), sizeof(Bank));
    file.close();
}

void change_account(int n)
{
    Bank obj;
    bool flag = false;
    fstream file;
    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "\n |-- File could not be opened. --|";
        return;
    }
    cout << "\n |-- Modifying Account --|";
    while (!file.eof() && flag == false)
    {
        file.read(reinterpret_cast<char *> (&obj), sizeof(Bank));
        if (obj.return_accountnum() == n)
        {
            obj.show_account();
            cout << "\n Please enter your new details." << endl;
            obj.modify_account();
            int position = (-1) * static_cast<int>(sizeof(Bank)); // calculates the position where the object "obj" should be rewritten in the file "database". static_case<int> converts it into a integer.
            file.seekp(position, ios::cur); // this moves the file pointer by the value of position relative to the current position which is ios::cur.
            file.write(reinterpret_cast<char *> (&obj), sizeof(Bank)); // this is better over creating a copy of the file, deleting the original and renaming the copy to the original repeatedly.
            cout << "\n Account was successfully modified.";
            flag = true;
        }
    }
    file.close();
    if (flag == false)
    {
        cout << "\n |-- File was not found. --|" << endl;
    }
}

void delete_account(int n)
{
    bool found_account = false;
    Bank obj;
    ifstream file; // input file stream object, used to read data from the file
    ofstream file2; // output file stream , used to write data into the file
    file.open("newdatabase.dat", ios::binary);
    if (!file)
    {
        cout << "\n |-- File could not be opened. --|";
        return;
    }
    file2.open("temp_newdatabase.dat", ios::binary); // creates a temp file that used to write
    file.seekg(0, ios::beg); // moves the file pointer to the beginning of the file
    while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank)))
    {
        if (obj.return_accountnum() != n)
        {
            file2.write(reinterpret_cast<char *> (&obj), sizeof(Bank));
        }
        else
        {
            found_account = true;
        }
    }
    file.close();
    file2.close();
    remove("newdatabase.dat");
    rename("temp_newdatabase.dat", "newdatabase.dat");
    if(!found_account)
    {
        cout << "\n Account was not found. (Press enter)";
    }
    else
    {
        cout << "\n Account was successfully deleted. (Press enter)";
    }
}

void display_account(int n)
{
    Bank obj;
    bool flag = false;
    ifstream file;
    file.open("newdatabase.dat", ios::binary);
    if (!file)
    {
        cout << "\n |-- File could not be opened. --|";
        return;
    }

    cout << "\n |-- Account Details --|";
    while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank))) {
        if (obj.return_accountnum() == n)
        {
            obj.show_account();
            flag = true;
        }
    }
    file.close();
    if (flag == false)
    {
        cout << "\n |-- Account number was not found. -- |";
    }
}
void account_actions(int n, int option)
{
    float test_balance, amount;
    bool flag = false;
    Bank obj;
    fstream file;
    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "\n |-- File could not be opened. --|" << endl;
        return;
    }
    while (!file.eof() && flag == false)
    {
        file.read(reinterpret_cast<char *> (&obj), sizeof(Bank));
        if (obj.return_accountnum() == n)
        {
            obj.show_account();
            if (option == 1)
            {
                cout << "\n |-- Depositing Amount --|" << endl;
                cout << "\n Please enter the amount you would like to deposit: $";
                while(!(cin >> amount))
                {
                    cout << "\n Invalid input type. Please enter again: $";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                obj.deposit(amount, 1);
            }
            if (option == 2)
            {
                cout << "\n |-- Withdrawing Amount --|" << endl;
                cout << "\n Please enter the amount you would like to withdraw. (Remember: An Account must have at least $100): $";
                while(!(cin >> amount))
                {
                    cout << "\n Invalid input type. Please enter again: $";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                test_balance = obj.return_balance() - amount;
                if (test_balance < 100)
                {
                    cout << "\n Unable to withdraw. Insufficient amount available. (Press enter) " << endl;
                    return;
                }
                else
                {
                    obj.withdraw(amount,1);
                }
            }
            int position = (-1) * static_cast<int>(sizeof(obj));
            file.seekp(position, ios::cur);
            file.write(reinterpret_cast<char *> (&obj), sizeof(Bank));
            cout << "\n |-- Balance was successfully updated --|" << endl;
            flag = true;
        }
    }
}

void money_transfer(int sender_id)
{
    int r_id;
    float amount, test_balance;
    bool s_flag = false, r_flag = false;
    Bank s_obj, r_obj;
    fstream file;
    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "\n |-- File could not be opened. --|" << endl;
        return;
    }

    while((!file.eof()) && (s_flag == false))
    {
        file.read(reinterpret_cast<char *> (&s_obj), sizeof(Bank));
        if(s_obj.return_accountnum() == sender_id)
        {
            s_obj.show_account();
            s_flag = true;
        }
    }

    if(!s_flag)
    {
        cout << "\n |-- Sender account was not found --|" << endl;
        file.close();
        return;
    }

    cout << "\n Please enter the account number of the receiving user: #";
    cin >> r_id;
    while((!file.eof()) && (r_flag == false))
    {
        file.read(reinterpret_cast<char *> (&r_obj), sizeof(Bank));
        if (r_obj.return_accountnum() == r_id)
        {
            cout << "\n Sending to " << r_obj.name << ".";
            r_flag = true;
        }
    }

    if(!r_flag)
    {
        cout << "\n Canceling Transaction...Receiving user not found.";
        file.close();
        return;
    }

    cout << "How much would you like to send to " << r_obj.name << "? $";
    while(!(cin >> amount))
    {
        cout << "\n Invalid input type. Please enter again: $";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    test_balance = s_obj.return_balance() - amount;
    if(test_balance < 100)
    {
        cout << "\n Sender has insufficient funds.";
    }
    else
    {
        r_obj.deposit(amount,2);
        int r_position = (-1) * static_cast<int>(sizeof(r_obj));
        file.seekp(r_position, ios::cur);
        file.write(reinterpret_cast<char *> (&r_obj), sizeof(Bank));
        cout << "\n |-- Funds have been transferred --|";
    }
    file.close();

    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "\n |-- File could not be opened. --|" << endl;
        return;
    }
    while((!file.eof()) && (s_flag == true))
    {
        file.read(reinterpret_cast<char *> (&s_obj), sizeof(Bank));
        if(s_obj.return_accountnum() == sender_id)
        {
            s_obj.withdraw(amount,2);
            int s_position = (-1) * static_cast<int>(sizeof(s_obj));
            file.seekp(s_position, ios::cur);
            file.write(reinterpret_cast<char *> (&s_obj), sizeof(Bank));
        }
    }
    file.close();

}



void display_all_accounts()
{
    Bank obj;
    ifstream file;
    file.open("newdatabase.dat", ios::binary);
    if (!file)
    {
        cout << "\n |-- File could not be opened. --|";
        return;
    }
    cout << "\n\n\t\t\t|-- Existing User Accounts --|\n\n";
    cout << "========================================================================================\n";
    cout << " AC #              Name              Type           Balance      Delete?        Frozen?\n";
    cout << "========================================================================================\n";
    while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank)))
    {
        obj.report();
    }
    file.close();
    cout << "\n (Press Enter)";
}

void sign_in_menu()
{
    int choice, account_number, pin, adminac_num;
    string admin_pass;
    do
    {
        cout << "\n 1. Sign In";
        cout << "\n 2. Create New Account";
        cout << "\n 3. Recover Account";
        cout << "\n 4. Exit";
        cout << "\n Enter Your Choice: ";
        while(!(cin >> choice))  // https://stackoverflow.com/questions/28380421/what-happens-if-we-input-a-character-in-a-switch-case-where-an-integer-is-requir
        {
            cout << "\n Invalid input type. Please enter again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice)
        {
            case 1:
                cout << "\n Enter your account number: #";
                while(!(cin >> account_number))
                {
                    cout << "\n Invalid input type. Please enter again: #";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cout << "\n Enter your PIN: ";
                while(!(cin >> pin))
                {
                    cout << "\n Invalid input type. Please enter again: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                verify_user(account_number, pin);
                break;
            case 2:
                create_account();
                break;
            case 3:
                recover_account();
                break;
            case 4:
                cout << "\n Exiting program.";
                exit(0);
            case 33050203:
                cout << "\n Please enter your Admin account number: #";
                while(!(cin >> adminac_num))
                {
                    cout << "\n Invalid input type. Please enter again: #";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cout << "\n Please enter your Admin password: ";
                cin >> admin_pass;
                verify_admin(adminac_num, admin_pass);
                break;
            default:
                cout << "\n Invalid choice, please try again. (Press enter)";
        }
        cin.ignore(); // Clear input buffer
        cin.get(); // Wait for user to press enter
    } while (choice != 4);
}

void verify_user(int ac_num, int pin)
{
    Bank obj;
    ifstream file;
    bool flag = false;
    file.open("newdatabase.dat", ios::binary);
    if (!file)
    {
        cout << "\n |-- File could not be opened. --|";
        return;
    }
    while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank)))
    {
        if ((obj.return_accountnum() == ac_num) && (obj.return_pin() == pin) && (obj.return_frozen() == 'N'))
        {
            cout << "\n Hello " << obj.name << "! Please select one of the following.";
            account_menu(ac_num);
            flag = true;

        }
    }
    file.close();
    if (flag == false)
    {
        cout << "\n |-- Please ensure the account number exists, the pin is correct, or even if the account is frozen. --|";
    }

}

void request_account_deletion(int id)
{
    Bank obj;
    fstream file;
    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "File could not be opened." << endl;
        return;
    }
    bool flag = false;
    cout << "\nPlease note that your account will be put on hold for 30 days before deletion.";
    while (file.read(reinterpret_cast<char *>(&obj), sizeof(Bank))) {

        if (obj.return_accountnum() == id)
        {
            obj.request = 'Y'; // Update the request variable
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

void account_suspension(int id)
{
    Bank obj;
    fstream file;
    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "\n |-- File could not be opened. --|" << endl;
        return;
    }
    bool flag = false;
    cout << "\nPlease note that suspending your account will make it inaccessible.";
    while (file.read(reinterpret_cast<char *>(&obj), sizeof(Bank)))
    {

        if (obj.return_accountnum() == id)
        {
            obj.frozen = 'Y'; // Update the request variable
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

void verify_admin(int admin_n, string admin_p)
{
    if((admin_n == 101) && (admin_p == "test"))
    {
        admin_menu();
    }
    else
    {
        cout << "\n Incorrect Credentials. Please try again. (Press enter)";
        return;
    }
}

bool verify_recovery(const Bank& obj)
{
    char answer[40];
    cout << "\n 1. What city were you born in? : ";
    cin.ignore();
    cin.getline(answer, 40);
    while(strlen(answer) == 0)
    {
        cout << "\n This field cannot be left empty. Please try again: ";
        cin.getline(answer, 40);
    }
    if(strcmp(answer, obj.answer1) != 0)
    {
        return false;
    }
    cout << "\n 2. What is the last name of your favorite elementary school teacher? : ";
    cin.getline(answer, 40);
    while(strlen(answer) == 0)
    {
        cout << "\n This field cannot be left empty. Please try again: ";
        cin.getline(answer, 40);
    }
    if (strcmp(answer, obj.answer2) != 0)
    {
        return false;
    }
    cout << "\n 3. What was the first film you watched in theaters? : ";
    cin.getline(answer, 40);
    while(strlen(answer) == 0)
    {
        cout << "\n This field cannot be left empty. Please try again: ";
        cin.getline(answer, 40);
    }
    if (strcmp(answer, obj.answer3) != 0)
    {
        return false;
    }
    return true;
}

void recover_account()
{
    int ac_num;
    bool flag = false;
    Bank obj;
    fstream file;
    file.open("newdatabase.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\n |-- File could not be opened. --|";
        return;
    }

    cout << "\n Please enter your account number: #";
    cin >> ac_num;
    while (file.read(reinterpret_cast<char *>(&obj), sizeof(Bank)))
    {
        if (obj.return_accountnum() == ac_num)
        {
            flag = true;
            cout << "\n Answer the security questions to recover your account:" << endl;
            if (verify_recovery(obj))
            {
                obj.modify_account();
                int position = (-1) * static_cast<int>(sizeof(Bank));
                file.seekp(position, ios::cur);
                file.write(reinterpret_cast<char *>(&obj), sizeof(Bank));
            }
            else
            {
                cout << "\n Recovery failed. Incorrect answers to security questions." << endl;
            }
            break;
        }
    }
}

void display_transaction_logs(int account_number = -1) // placeholder parameter used for admin account
{
    ifstream log_file("transaction_logs2.dat", ios::binary);
    if (log_file.is_open())
    {
        TransactionRecord record;
        if (account_number != -1)
        {
            cout << "\n Recent Transactions for Account Number: #" << account_number << endl;
        }
        else
        {
            cout << "\n\n\t     |-- Transaction Logs --|\n\n";
            cout << "==================================================\n";
            cout << "      Timestamp          AC #     Type   Amount\n";
            cout << "==================================================\n";
        }
        bool found_transactions = false;
        while (log_file.read(reinterpret_cast<char*>(&record), sizeof(TransactionRecord)))
        {
            if (account_number == -1 || record.account_number == account_number)
            {
                cout << put_time(localtime(&record.timestamp), "%Y-%m-%d %H:%M:%S") << setw(5) << " " << record.account_number << setw(5) << " " << record.transaction_type;
                if (record.transaction_type == 'W' || record.transaction_type == 'S')
                {
                    cout << setw(5) << " -" << "$" << record.amount << endl; // Add subtraction symbol for withdrawal
                }
                else
                {
                    cout << setw(5) << " " << "$" << record.amount << endl;
                }
                found_transactions = true;
            }
        }
        if (!found_transactions)
        {
            if (account_number != -1)
            {
                cout << "\n No recent transactions found for Account Number: #" << account_number << endl;
            }
            else
            {
                cout << "\n |-- No logs found --|" << endl;
            }
        }
        log_file.close();
    }
    else
    {
        cout << "\n |-- File could not be opened. --|" << endl;
        return;
    }
}

void account_menu(int id)
{
    int choice;
    do {
        cout << "\n 1. Account Details";
        cout << "\n 2. Deposit into Account";
        cout << "\n 3. Withdraw from Account";
        cout << "\n 4. Transfer to another user";
        cout << "\n 5. View your bank statements";
        cout << "\n 6. Request Account Deletion";
        cout << "\n 7. Freeze Account";
        cout << "\n 8. Log out";
        cout << "\n Enter Your Choice: ";
        while(!(cin >> choice))
        {
            cout << "\n Invalid input type. Please enter again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        switch (choice)
        {
            case 1:
                display_account(id);
                break;
            case 2:
                account_actions(id, 1);
                break;
            case 3:
                account_actions(id, 2);
                break;
            case 4:
                money_transfer(id);
                break;
            case 5:
                display_transaction_logs(id);
                break;
            case 6:
                request_account_deletion(id);
                sign_in_menu();
                break;
            case 7:
                account_suspension(id);
                break;
            case 8:
                cout << "\n Logging out..." << endl;
                sign_in_menu();
            default:
                cout << "\n Invalid choice, please try again. (Press enter)";
        }
        cin.ignore(); // Clear input buffer
        cin.get(); // Wait for user to press enter
    } while (choice != 8);
}

void admin_menu()
{
    cout << "\n Welcome to the Admin menu.";
    int choice, ac_num;
    do
    {
        cout << "\n 1. Delete a user account";
        cout << "\n 2. View all users";
        cout << "\n 3. View all user transaction logs";
        cout << "\n 4. Log out";
        cout << "\n Enter your choice:";
        while(!(cin >> choice))  // https://stackoverflow.com/questions/28380421/what-happens-if-we-input-a-character-in-a-switch-case-where-an-integer-is-requir
        {
            cout << "\n Invalid input type. Please enter again: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        switch(choice)
        {
            case 1:
                cout << "\n Please enter the user's account number: #";
                cin >> ac_num;
                delete_account(ac_num);
                break;
            case 2:
                display_all_accounts();
                break;
            case 3:
                display_transaction_logs();
                break;
            case 4:
                cout << "\n Logging out..." << endl;
                sign_in_menu();
                break;
            default:
                cout << "\n Invalid choice, please try again. (Press enter)";
        }
        cin.ignore(); // Clear input buffer
        cin.get(); // Wait for user to press enter
    }while(choice != 4);

}

int main()
{
    sign_in_menu(); // Call the sign-in menu first
    return 0;
}
