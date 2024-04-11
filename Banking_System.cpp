    #include <iostream> // Standard Input Output Stream
    #include <fstream> // Will be used for binary file handling.
    #include <cstdlib> // Allows to use the srand() and rand() functions to create a 6-digit number generator.
    #include <ctime> // Allows to create the timestamp and its specific format for keeping track when a transaction log happens
    #include <cctype> // Allows to make the account type all lowercase by using the tolower() function.
    #include <iomanip> // Allows to set the width of the report() by using setting a specific width with the setw() function to allow for static placement and left alignment on the display functions.
    #include <cstring> // Allows to use strcmp and strlen for input handling to ensure what the user is inputting is restricted to a certain degree.
    #include <limits> // Allow to create the while loops with !(cin >> variable) to force the user to input a correct value within the specific context.

    using namespace std;

    class Bank
            {
    public: // Reason for variables being public is because of it allows to pass on a specific user's name and even specific request.
        char name[40]; // name is a character assigned 40 bytes
        char type[20]; // type is a character assigned 20 bytes
        int pin, account_number;
        char request = 'N', frozen = 'N'; // Will later show up in the user display function for an admin see a quick glance certain conditions related to an account.
        char answer1[40], answer2[40], answer3[40]; // Like the name variable, these are assigned 40 bytes each.
        float balance;
        void new_account(); // Generates a unique 6-digit account number, asks for name, account type, pin, deposit, and three security questions.
        void show_account() const; // Displays the account number, name, account type, and current balance of a given user.
        void modify_account(); // Part of the account recovery function. Upon successful verification, this function will allow the user to create a new pin to log in with.
        void report() const; // Part of the display_accounts() function. This creates the format on the padding between each piece of in formation and in what order.
        void deposit(float, int); // Deposits a specified amount into a user's balance. Also contains a transaction log function to record such action.
        void withdraw(float, int); // Withdraws a specified amount into a user's balance. Also contains a transaction log function to record such action.
        int return_accountnum() const; // Mainly used as part of a condition to check if inside the file if there is any matching account number that the user has inputted.
        int return_balance() const; // Used to in part with an input amount to check if a user has enough to withdraw specified amount.
        char return_frozen() const; // Used in the verify user function to check if the user's account isn't currently frozen.
        int return_pin () const; // Used in the verify user function as a security measure.

    };

    class Transaction
    {
    public:
        time_t timestamp; // time_t helps represent a time. Specifically to create a transaction log.
        int account_number; // Takes note of the current user's account number
        char transaction_type; // Label of to show different transaction types. There is D, W, S, and R. (Deposit, Withdrawal, Sender, and Receiver)
        double amount; // Takes note of the specified about from the user.
    };

    void write_log(int acc_num, char transaction_type, float amount) // The reason this function
    {
        Transaction obj;
        obj.timestamp = time(nullptr); // Returns the current time
        obj.account_number = acc_num;
        obj.transaction_type = transaction_type;
        obj.amount = amount;

        // https://cplusplus.com/doc/tutorial/files/
        ofstream log_file("transaction_logs.dat", ios::binary | ios::app); // Opens only in writing mode. Creates object called log_file. Opens in binary mode which writes data without any formatting. Append mode adds data to the end of the file if it already exists.
        if (log_file.is_open()) // the .is_open() function checks if the file can be open. Doing !log_file could also as this condition. However since we only used ofstream, is_open() seems like the better option.
        {
            log_file.write(reinterpret_cast<const char*>(&obj), sizeof(Transaction)); // reinterpret_cast changes the memory address of "obj" into a constant character pointer. This is used to help pass whatever is written in "obj". Size of "transaction" which is a class, specifies how many bytes to write to ensure the contents of the "obj" are completely written.
            log_file.close();
        }
        else
        {
            cout << "Unable to open transaction log file." << endl; // This message happens if the file is unable to be open.
        }
    }

    void Bank::new_account()
    {
        bool valid_type = false; // Will be used to verify the account types
        // https://www.digitalocean.com/community/tutorials/random-number-generator-c-plus-plus
        srand(static_cast<unsigned int>(time(nullptr))); // srand() helps perform pseudo-random number generation which tries to mimic randomness. This function however requires a seed value, which in this case is whenever the current time is. This function however requires an unsigned int, which is why a static_cast is done so. The quality of this seed is not best though, but for these purposes it is fine.
        account_number = rand() % 900000 + 100000; // rand() is the one that actually generates these pseudo-random numbers with a range (900000) and an offset or starting point of our range (100000) leaving us with a possible range of (100000 to 999999) that can be assigned to the account_number variable.
        cout << "\n Your Account Number: #" << account_number;
        cout << "\n Enter Your Name: ";
        cin.ignore(); // Was having some issue where this statement was being skipped. Because when a user enters a '\n' gets in to the cin buffer. The getline statement gets that newline character causing it to be skipped. https://stackoverflow.com/questions/25475384/when-and-why-do-i-need-to-use-cin-ignore-in-c
        cin.getline(name, 40);
        while(strlen(name) == 0) // This while loop will continue as long as the input that user enters is empty.
        {
            cout << "\n This field cannot be left empty. Please try again: ";
            cin.getline(name, 40);
        }

        cout << "\n Enter the type of the account (Checking/Savings): ";
        while (!valid_type) // This while loop happens as long as the input isn't a correct account type.
        {
            cin >> type;
            for (int i = 0; type[i]; i++) // this for loop changes any input in lowercase by using the tolower() function.
            {
                type[i] = tolower(type[i]);
            }
            if (strcmp(type, "checking") == 0 || strcmp(type, "savings") == 0) // Uses string compare to see if the user has entered an existing account type. If correct, it leaves the while loop.
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
        while((pin < 10000) || (pin > 99999)) // This while repeats as long as a user hasn't entered a valid pin length.
        {
            cout << "\n Invalid pin, please try again: ";
            cin >> pin;
        }

        cout << "\n Enter a deposit. Deposits have a maximum of $100,000. (A minimum of $100 is a must to have an account open.): $";
        cin >> balance;
        while((balance < 100) || (balance > 100000)) // This while ensures that a valid deposit was made.
        {
            while(!(cin >> balance)) // While loops to ensure valid type
            {
                cout << "\n Invalid input type. Please enter again: $";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // same as the other one
            }
        }

        cout << "\n For recovery purposes, please answer the following questions:";
        cout << "\n 1. What city were you born in? :";
        cin.ignore();
        cin.getline(answer1, 40);
        while(strlen(answer1) == 0) // Same as the while loop for the name variable. Applies to the rest of the questions.
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
        cout << "\n Account was created successfully. (Double Enter)";
    }

    void Bank::modify_account() // Later used in the recovery process for an account after successfully answering the security questions.
    {
        cout << " \n Enter a new pin for your account {5 digits}: ";
        cin >> pin;
        while((pin < 10000) || (pin > 99999)) // Same as the original pin
        {
            cout << "\n Invalid pin, please try again: ";
            cin >> pin;
        }
        cout << "\n Successfully changed pin. (Press enter)";
    }

    void Bank::show_account() const // Simply displays the current users information upon request.
    {
        cout << "\n Account No. : #" << account_number;
        cout << "\n Account Holder Name : " << name;
        cout << "\n Type of Account : " << type;
        cout << "\n Balance amount : $" << balance;
    }

    void Bank::report() const // This is the format on how data will be displayed in the displaying the user accounts
    {
        cout << setw(15) << left << account_number << setw(20) << name << setw(18) << type << setw(15) << balance << setw(15) << request << setw(15) << frozen << endl;
    }

    void Bank::deposit(float amt, int option)
    {
        balance += amt;
        if (option == 1) // Option 1 is used for a normal deposit.
        {
            write_log(account_number, 'D', amt); // This calls the function that was defined above, writes the account number, type and how much.
        }
        else
        {
            write_log(account_number, 'R', amt); // Same thing as line above just the deposit is relabeled as a transfer type of "Receiving"
        }

    }

    void Bank::withdraw(float amt, int option)
    {
        balance -= amt;
        if (option == 1) // Fundamentally works the same as the deposit function.
        {
            write_log(account_number, 'W', amt);
        }
        else
        {
            write_log(account_number, 'S', amt); // This time the withdrawal function is relabeled as a transfer type of "Sender"
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
    void display_account(int); // displays a specific account
    void delete_account(int); // deletes an account
    void account_actions(int, int); // provides the option if a user wants to deposit or withdraw
    void money_transfer(int); // allows to transfer money from one user to another.
    void display_accounts(); // an admin only option to see all the users
    void main_menu(); // function for sign-in menu options
    void verify_user(int, int); // verifies if it is an existing user before going into the account_menu
    void account_menu(int); // a wide assortment of options a user can do
    void request_deletion(int); // a request a user can make so that an admin deletes their account
    void account_suspension(int); // a user can directly freeze their account if they suspect any malpractice
    void recover_account(); // if pin is unknown, a user can answer recovery questions in order to change their pin
    void verify_admin(int, string); // verifies if the admin login credentials are correct.
    void admin_menu(); // an assortment of actions a admin can do
    void write_log(int, char, float); // writes the transaction log in accordance to the type of action
    void display_logs(int); // displays a report of all user transactions from admin side and all a user's transactions from their side.

    void create_account()
    {
        Bank obj; // creates an object named "obj" from the class bank
        ofstream file;
        file.open("database.dat", ios::binary | ios::app); // Opens in binary mode which writes data without any formatting. Append mode adds data to the end of the file if it already exists.
        obj.new_account(); // calls the new_account function inside the bank class
        file.write(reinterpret_cast<char *> (&obj), sizeof(Bank)); // reinterpret_cast changes the memory address of "obj" into a constant character pointer. This is used to help pass whatever is written in "obj". Size of "bank" which is a class, specifies how many bytes to write to ensure the contents of the "obj" are completely written.
        file.close();
    }

    void delete_account(int account_number)
    {
        bool found_account = false;
        Bank obj;
        ifstream file;
        ofstream file2;
        file.open("database.dat", ios::binary); // opens "file object" in reading mode
        if(!file) // (!file) similar to is_open() to check if the file exists
        {
            cout << "\n |-- File could not be opened. --|";
            return;
        }
        file2.open("temp_database.dat", ios::binary); // creates a temp file that used to write
        file.seekg(0, ios::beg); // allows to change the location of the get position of the specific user that needs deletion. The offset is 0 and the position is the offset counted from the beginning of the stream
        while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank))) // this file.read() reads data from the file into "obj" continues until an error, or it has reached the end of the file.
        {
            if (obj.return_accountnum() != account_number) // if account (obj.return_account_num) does not match the given user (n), this means that it is not the account that needs deleting so it writes that data in the temp file. Once it matches with the given user, it will omit that data and set the flag to true.
            {
                file2.write(reinterpret_cast<char *> (&obj), sizeof(Bank));
            }
            else
            {
                found_account = true; // helps create the messages of whether it was successful or not.
            }
        }
        file.close();
        file2.close();
        remove("database.dat");
        rename("temp_database.dat", "database.dat");
        if(!found_account)
        {
            cout << "\n Account was not found. (Press enter)";
        }
        else
        {
            cout << "\n Account was successfully deleted. (Press enter)";
        }
    }

    void display_account(int account_number)
    {
        Bank obj;
        bool flag = false;
        ifstream file;
        file.open("database.dat", ios::binary);
        if (!file)
        {
            cout << "\n |-- File could not be opened. --|";
            return;
        }

        cout << "\n |-- Account Details --|";
        while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank))) // Same as the delete_account() function it reads until it can find a matching user.
        {
            if (obj.return_accountnum() == account_number) // if the given user's account number matches with existing ones (obj.return_accountnum()) it continues inside the if.
            {
                obj.show_account(); // calls the function that displays the data of that specific user.
                flag = true; // makes it true in order make an error message
            }
        }
        file.close();
        if (flag == false)
        {
            cout << "\n |-- Account number was not found. -- |";
        }
    }
    void account_actions(int account_number, int option)
    {
        float test_balance, amount;
        bool flag = false;
        Bank obj;
        fstream file;
        file.open("database.dat", ios::binary | ios::in | ios::out); // opens the file in binary, and both input and output modes
        if (!file)
        {
            cout << "\n |-- File could not be opened. --|" << endl;
            return;
        }
        while (!file.eof() && flag == false) // runs until end of file or if the flag becomes true
        {
            file.read(reinterpret_cast<char *> (&obj), sizeof(Bank)); // reads in order to check if there is matching user
            if (obj.return_accountnum() == account_number) // checks if it matches with user
            {
                obj.show_account(); // show how much the user currently has
                if (option == 1)
                {
                    cout << "\n |-- Depositing Amount --|" << endl;
                    cout << "\n Please enter the amount you would like to deposit: $";
                    while(!(cin >> amount)) // Does a while as long as it's an invalid type
                    {
                        cout << "\n Invalid input type. Please enter again: $";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // If a user types in such as "test" in the amount variable, this only will print the error message once and not 5 times. // https://stackoverflow.com/questions/28380421/what-happens-if-we-input-a-character-in-a-switch-case-where-an-integer-is-requir
                    }
                    obj.deposit(amount, 1); // this option parameter comes into play, tells the deposit function that it's a normal deposit.
                }
                if (option == 2)
                {
                    cout << "\n |-- Withdrawing Amount --|" << endl;
                    cout << "\n Please enter the amount you would like to withdraw. (Remember: An Account must have at least $100): $";
                    while(!(cin >> amount)) // While loops to ensure valid type
                    {
                        cout << "\n Invalid input type. Please enter again: $";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // same as the other one
                    }
                    test_balance = obj.return_balance() - amount; // calculates if does not make the user have less than $100
                    if (test_balance < 100)
                    {
                        cout << "\n Unable to withdraw. Insufficient amount available. (Press enter) " << endl;
                        return;
                    }
                    else
                    {
                        obj.withdraw(amount,1); // if successful, it will withdraw the amount and flag it as normal withdrawal.
                    }
                }
                int position = (-1) * static_cast<int>(sizeof(obj)); // Turns the size of the "obj" into an integer and creates a negative offset needed to position the file pointer at the start of the object that was just read.
                file.seekp(position, ios::cur); // the put position is set to this negative offset value in respect to ios::cur which is the current position.
                file.write(reinterpret_cast<char *> (&obj), sizeof(Bank)); // then it writes the updated balance.
                cout << "\n |-- Balance was successfully updated --|" << endl;
                flag = true; // breaks the while loop
            }
        }
    }

    void money_transfer(int sender_num)
    {
        int receiver_num;
        float amount, test_balance;
        bool s_flag = false, r_flag = false; // used to check if either sender or receiver account number was found.
        Bank s_obj, r_obj; // two separate options.
        fstream file;
        file.open("database.dat", ios::binary | ios::in | ios::out);
        if (!file)
        {
            cout << "\n |-- File could not be opened. --|" << endl;
            return;
        }

        while((!file.eof()) && (s_flag == false))
        {
            file.read(reinterpret_cast<char *> (&s_obj), sizeof(Bank));
            if(s_obj.return_accountnum() == sender_num) // verifies if the sender exists
            {
                s_obj.show_account(); // Shows the current balance the sending user
                s_flag = true; // breaks the while loop
            }
        }

        if(!s_flag) // sends this message if the sender's account number was not found.
        {
            cout << "\n |-- Sender account was not found --|" << endl;
            file.close();
            return;
        }

        file.close();
        file.open("database.dat", ios::binary | ios::in | ios::out);
        cout << "\n Please enter the account number of the receiving user: #"; // Similar idea for the receiving user.
        cin >> receiver_num;
        while((!file.eof()) && (r_flag == false))
        {
            file.read(reinterpret_cast<char *>(&r_obj), sizeof(Bank));
            if (r_obj.return_accountnum() == receiver_num) // checks if the recieving user exists
            {
                cout << "\n Sending to " << r_obj.name << "."; // displays the recieving user's name
                r_flag = true;
            }
        }

        if(!r_flag) // sends this message if the recieving user was not found.
        {
            cout << "\n Canceling Transaction...Receiving user not found.";
            file.close();
            return;
        }

        cout << " How much would you like to send to " << r_obj.name << "? $";
        while(!(cin >> amount)) // While loop to ensure valid input
        {
            cout << "\n Invalid input type. Please enter again: $";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        test_balance = s_obj.return_balance() - amount; // similar to the withdrawal option. Ensures that the sending user does end up with less than $100
        if(test_balance < 100)
        {
            cout << "\n Sender has insufficient funds.";
        }
        else
        {
            r_obj.deposit(amount,2); // if successful a deposit goes to the recieving user with option 2 to create the label of R.
            int r_position = (-1) * static_cast<int>(sizeof(r_obj)); // same as the deposit/withdrawal option, rewrites the balance of the recieving user.
            file.seekp(r_position, ios::cur);
            file.write(reinterpret_cast<char *> (&r_obj), sizeof(Bank));
            cout << "\n |-- Funds have been transferred --|";
        }
        file.close();

        file.open("database.dat", ios::binary | ios::in | ios::out); // file is opened again, in order to the withdrawal function to the sender and to update their new balance.
        if (!file)
        {
            cout << "\n |-- File could not be opened. --|" << endl;
            return;
        }
        while((!file.eof()) && (s_flag == true))
        {
            file.read(reinterpret_cast<char *> (&s_obj), sizeof(Bank));
            if(s_obj.return_accountnum() == sender_num)
            {
                s_obj.withdraw(amount,2);
                int s_position = (-1) * static_cast<int>(sizeof(s_obj));
                file.seekp(s_position, ios::cur);
                file.write(reinterpret_cast<char *> (&s_obj), sizeof(Bank));
                break;
            }
        }
        file.close();

    }

    void display_accounts()
    {
        Bank obj;
        ifstream file;
        file.open("database.dat", ios::binary);
        if (!file)
        {
            cout << "\n |-- File could not be opened. --|";
            return;
        }
        cout << "\n\n\t\t\t|-- Existing User Accounts --|\n\n"; // a simple table to show what the data means
        cout << "========================================================================================\n";
        cout << " AC #              Name              Type           Balance      Delete?        Frozen?\n";
        cout << "========================================================================================\n";
        while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank)))
        {
            obj.report(); // the formatted data is read until the end of the file.
        }
        file.close();
        cout << "\n (Press Enter)";
    }

    void main_menu()
    {
        int choice, acc_num, pin, adminac_num;
        string admin_pass;
        do // a do-while loop is done that way the list option are always shown and the user does not have to scroll up in the console
        {
            cout << "\n 1. Sign In"; // list of options
            cout << "\n 2. Create New Account";
            cout << "\n 3. Recover Account";
            cout << "\n 4. Exit";
            cout << "\n Enter Your Choice: ";
            while(!(cin >> choice)) // ensures it is the correct type for the choice
            {
                cout << "\n Invalid input type. Please enter again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            switch (choice)
            {
                case 1:
                    cout << "\n Enter your account number: #";
                    while(!(cin >> acc_num)) // checks input
                    {
                        cout << "\n Invalid input type. Please enter again: #";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    cout << "\n Enter your PIN: ";
                    while(!(cin >> pin)) // checks input
                    {
                        cout << "\n Invalid input type. Please enter again: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    verify_user(acc_num, pin); // verifies the user
                    break;
                case 2:
                    create_account(); // calls the account creator
                    break;
                case 3:
                    recover_account(); // calls the recovery option
                    break;
                case 4:
                    cout << "\n Exiting program.";
                    exit(0); // ends the program
                case 505: // unlisted option. Intentionally hidden from a user.
                    cout << "\n Please enter your Admin account number: #";
                    while(!(cin >> adminac_num)) // checks valid input
                    {
                        cout << "\n Invalid input type. Please enter again: #";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    cout << "\n Please enter your Admin password: ";
                    cin >> admin_pass;
                    verify_admin(adminac_num, admin_pass); // calls the verify_admin function
                    break;
                default:
                    cout << "\n Invalid choice, please try again. (Press enter)";
            }
            cin.ignore(); // Clear input buffer
            cin.get(); // Wait for user to press enter
        } while (choice != 4);
    }

    void verify_user(int account_number, int pin)
    {
        Bank obj;
        ifstream file;
        bool flag = false;
        file.open("database.dat", ios::binary);
        if (!file)
        {
            cout << "\n |-- File could not be opened. --|";
            return;
        }
        while (file.read(reinterpret_cast<char *> (&obj), sizeof(Bank)))
        {
            if ((obj.return_accountnum() == account_number) && (obj.return_pin() == pin) && (obj.return_frozen() == 'N')) // sees if the account number, pin, and if the account is not frozen.
            {
                cout << "\n Hello " << obj.name << "! Please select one of the following."; // greets the user if successful
                account_menu(account_number); // calls the account menu
                flag = true; // indicates that was successful verification

            }
        }
        file.close();
        if (flag == false) // if fails, sends this message
        {
            cout << "\n |-- Please ensure the account number exists, the pin is correct, or even if the account is frozen. --|";
        }

    }

    void request_deletion(int account_number)
    {
        Bank obj;
        fstream file;
        file.open("database.dat", ios::binary | ios::in | ios::out);
        if (!file)
        {
            cout << "File could not be opened." << endl;
            return;
        }
        bool flag = false;
        cout << "\nPlease note that your account will be put on hold for 30 days before deletion.";
        while (file.read(reinterpret_cast<char *>(&obj), sizeof(Bank))) {

            if (obj.return_accountnum() == account_number)
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

    void account_suspension(int account_number)
    {
        Bank obj;
        fstream file;
        file.open("database.dat", ios::binary | ios::in | ios::out);
        if (!file)
        {
            cout << "\n |-- File could not be opened. --|" << endl;
            return;
        }
        bool flag = false;
        cout << "\nPlease note that suspending your account will make it inaccessible.";
        while (file.read(reinterpret_cast<char *>(&obj), sizeof(Bank)))
        {

            if (obj.return_accountnum() == account_number)
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

    void verify_admin(int admin_num, string admin_pin)
    {
        if((admin_num == 33050203) && (admin_pin == "@dm1n!")) // specific credentials for the admin user to match.
        {
            admin_menu(); // if successful, will call account menu
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
        while(strlen(answer) == 0) // Ensures the answer fields where not left empty
        {
            cout << "\n This field cannot be left empty. Please try again: ";
            cin.getline(answer, 40);
        }
        if(strcmp(answer, obj.answer1) != 0) //this is case-sensitive, it checks if the answer the user put in the create_account() function matches the function they answered now. Applies for the three answers.
        {
            return false; // if both answers do not match, this recovery fails and quick ends.
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
        return true; // If all three are successful, the user can change their pin
    }

    void recover_account()
    {
        int account_number;
        bool flag = false;
        Bank obj;
        fstream file;
        file.open("database.dat", ios::binary | ios::in | ios::out);
        if (!file) {
            cout << "\n |-- File could not be opened. --|";
            return;
        }

        cout << "\n Please enter your account number: #";
        cin >> account_number;
        while (file.read(reinterpret_cast<char *>(&obj), sizeof(Bank)))
        {
            if (obj.return_accountnum() == account_number)
            {
                flag = true;
                cout << "\n Answer the security questions to recover your account:" << endl;
                if (verify_recovery(obj)) //If this results into true
                {
                    obj.modify_account(); //this allows to change the pin
                    int position = (-1) * static_cast<int>(sizeof(Bank));
                    file.seekp(position, ios::cur);
                    file.write(reinterpret_cast<char *>(&obj), sizeof(Bank)); // rewrites for the new pin
                }
                else
                {
                    cout << "\n Recovery failed. Incorrect answers to security questions." << endl;
                }
                break;
            }
        }
    }

    void display_logs(int account_number = -1) // account_number here is a default parameter to allow the admin account view all the users
    {
        ifstream log_file("transaction_logs.dat", ios::binary);
        if (log_file.is_open())
        {
            Transaction obj;
            if (account_number != -1) //Basically if it is not the admin, it will print out the user's unique account_number
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
            while (log_file.read(reinterpret_cast<char*>(&obj), sizeof(Transaction)))
            {
                if (account_number == -1 || obj.account_number == account_number) //Whether admin or user, it still prints the data in the same format.
                {
                    cout << put_time(localtime(&obj.timestamp), "%Y-%m-%d %H:%M:%S") << setw(5) << " " << obj.account_number << setw(5) << " " << obj.transaction_type; //put_time uses that time(nullptr) that we made and a specific format inorder to print the time stamp.
                    if (obj.transaction_type == 'W' || obj.transaction_type == 'S')
                    {
                        cout << setw(5) << " -" << "$" << obj.amount << endl; // Add subtraction symbol for withdrawal case
                    }
                    else
                    {
                        cout << setw(5) << " " << "$" << obj.amount << endl;
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

    void account_menu(int acc_num)
    {
        int choice;
        do // a do-while loop is done that way the list option are always shown and the user does not have to scroll up in the console
        {
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
                    display_account(acc_num);
                    break;
                case 2:
                    account_actions(acc_num, 1);
                    break;
                case 3:
                    account_actions(acc_num, 2);
                    break;
                case 4:
                    money_transfer(acc_num);
                    break;
                case 5:
                    display_logs(acc_num);
                    break;
                case 6:
                    request_deletion(acc_num);
                    main_menu();
                    break;
                case 7:
                    account_suspension(acc_num);
                    main_menu();
                    break;
                case 8:
                    cout << "\n Logging out..." << endl;
                    main_menu();
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
        int choice, acc_num;
        do // a do-while loop is done that way the list option are always shown and the user does not have to scroll up in the console
        {
            cout << "\n 1. Delete a user account";
            cout << "\n 2. View all users";
            cout << "\n 3. View all user transaction logs";
            cout << "\n 4. Log out";
            cout << "\n Enter your choice:";
            while(!(cin >> choice))
            {
                cout << "\n Invalid input type. Please enter again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            switch(choice)
            {
                case 1:
                    cout << "\n Please enter the user's account number: #";
                    cin >> acc_num;
                    delete_account(acc_num);
                    break;
                case 2:
                    display_accounts();
                    break;
                case 3:
                    display_logs();
                    break;
                case 4:
                    cout << "\n Logging out..." << endl;
                    main_menu();
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
        main_menu(); // Call the sign-in menu first
        return 0;
    }
