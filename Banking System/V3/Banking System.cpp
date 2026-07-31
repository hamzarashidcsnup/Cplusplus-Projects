#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

int available_acc_num = 10001;

void user_logs(string new_user, string password, int new_acc_num){
    ofstream storing_file("user & account numbers.txt", ios::app);
    if (storing_file.is_open()){
        storing_file << new_user << " " << password << " " << new_acc_num << endl;
        storing_file.close();
    }
    else{
        cout << "Couldn't open file for writing!" << endl;
    }
}
void transaction_logs(int acc_num, string transaction_type, double transaction_amount){
    ofstream transaction_file("Transactions log.txt", ios::app);
    if (transaction_file.is_open()){
        transaction_file << acc_num << " " << transaction_type << " " << transaction_amount << endl;
    }
}

class Transaction{
private:
    string transaction_type;
    double transaction_amount;

public:
    string get_transaction_type() const{
        return transaction_type;
    }
    double get_transaction_amount() const{
        return transaction_amount;
    }
    void set_transaction_type(string new_transaction_type){
        transaction_type = new_transaction_type;
    }
    void set_transaction_amount(double new_transaction_amount){
        transaction_amount = new_transaction_amount;
    }
};
class Account{
private:
    int acc_num;
    double balance = 0;
    vector<Transaction> history;

public:
    Account(){
        acc_num = available_acc_num;
        available_acc_num = available_acc_num + 1;
    }
    int get_acc_num() const{
        return acc_num;
    }
    double get_balance() const{
        return balance;
    }
    void set_acc_num(int new_acc_num){
        acc_num = new_acc_num;
    }
    void update_balance(double new_balance){
        balance = new_balance;
    }
    void add_to_transaction_history(Transaction transaction_obj){
        history.push_back(transaction_obj);
    }
    void deposit_money(){
        double deposit_amount;
        cout << "Enter the amount you want to deposit: ";
        cin >> deposit_amount;
        balance = balance + deposit_amount;
        cout << "Deposited " << deposit_amount << " in your account!" << endl;
        Transaction transaction_obj;
        transaction_obj.set_transaction_type("Deposit");
        transaction_obj.set_transaction_amount(deposit_amount);
        history.push_back(transaction_obj);
        transaction_logs(acc_num, "Deposit", deposit_amount);
    }
    void withdraw_money(){
        double withdraw_amount;
        cout << "Enter the amount you want to withdraw: ";
        cin >> withdraw_amount;
        if (withdraw_amount > balance){
            cout << "Error, insufficient funds!" << endl;
        }
        else if (withdraw_amount <= balance){
            balance = balance - withdraw_amount;
            cout << "Successfully withdrawn " << withdraw_amount << " from your account!" << endl;
            Transaction transaction_obj;
            transaction_obj.set_transaction_type("Withdraw");
            transaction_obj.set_transaction_amount(withdraw_amount);
            history.push_back(transaction_obj);
            transaction_logs(acc_num, "Withdraw", withdraw_amount);
        }
        else{
            cout << "Error, please try again!" << endl;
        }
    }
    void transfer_money(Account &other_acc_num){
        double transfer_amount;
        cout << "Enter the amount you want to transfer to other account: ";
        cin >> transfer_amount;
        if (this->acc_num == other_acc_num.get_acc_num()){
            cout << "Error! You can't transfer money into your own account" << endl;
            return;
        }
        if (transfer_amount <= 0){
            cout << "Error! The transfer amount should be greater than 0" << endl;
            return;
        }
        if (transfer_amount <= balance){
            balance = balance - transfer_amount;
            other_acc_num.update_balance(other_acc_num.get_balance() + transfer_amount);
            cout << "Transfer success!" << endl;
            Transaction transaction_obj;
            transaction_obj.set_transaction_type("Transfer");
            transaction_obj.set_transaction_amount(transfer_amount);
            history.push_back(transaction_obj);
            transaction_logs(acc_num, "Transfer", transfer_amount);
            Transaction transaction_obj_for_reciever;
            transaction_obj_for_reciever.set_transaction_type("Received");
            transaction_obj_for_reciever.set_transaction_amount(transfer_amount);
            other_acc_num.add_to_transaction_history(transaction_obj_for_reciever);
            transaction_logs(other_acc_num.get_acc_num(), "Received", transfer_amount);
        }
        else if (transfer_amount > balance){
            cout << "Insufficient funds." << endl;
        }
        else{
            cout << "Error, please try again." << endl;
        }
    }
    void print_history() const{
        if (history.empty()){
            cout << "No recent transactions yet" << endl;
        }
        for (int i = 0; i < history.size(); i++){
            cout << history[i].get_transaction_type() << " " << history[i].get_transaction_amount() << endl;
        }
    }
};
class Customer{
private:
    string name;
    string password;
    Account Accountobj;

public:
    Customer() {}
    string get_name() const{
        return name;
    }
    string get_password() const{
        return password;
    }
    Account &get_account(){
        return Accountobj;
    }
    void set_name(string newname){
        name = newname;
    }
    void set_password(string new_password){
        password = new_password;
    }
    void register_new_user(){
        cin.ignore(10000, '\n');
        cout << "Enter your name: (no spaces)";
        cin >> name;
        cout << "Enter your password: (no spaces)";
        cin >> password;
        cout << "Account successfully created for " << name << endl;
        cout << "Your account number is " << Accountobj.get_acc_num() << endl;
        user_logs(name, password, Accountobj.get_acc_num());
    }
    void show_details(){
        cout << "Name: " << name << endl;
        cout << "Account Number: " << Accountobj.get_acc_num() << endl;
        cout << "Balance: " << Accountobj.get_balance() << endl;
        cout << "Recent transactions:" << endl;
        Accountobj.print_history();
    }
};
int index_helper(vector<Customer> &Bank_database, int acc_num_to_be_searched){
    for (int i = 0; i < Bank_database.size(); i++){
        if (Bank_database[i].get_account().get_acc_num() == acc_num_to_be_searched){
            return i;
        }
    }
    return -1; // standard code for not found
}

void loading_accounts(vector<Customer> &Bank_database){
    ifstream storing_file("user & account numbers.txt");
    if (!storing_file.is_open())
        return;
    string loaded_name, loaded_password;
    int highest_acc_num = 10000, loaded_acc_num;

    while (storing_file >> loaded_name >> loaded_password >> loaded_acc_num){
        Customer Customerobj;
        Customerobj.set_name(loaded_name);
        Customerobj.set_password(loaded_password);
        Customerobj.get_account().set_acc_num(loaded_acc_num);
        Bank_database.push_back(Customerobj);
        if (loaded_acc_num > highest_acc_num){
            highest_acc_num = loaded_acc_num;
        }
    }
    storing_file.close();
    available_acc_num = highest_acc_num + 1;
}

void loading_transactions(vector<Customer> &Bank_database){
    ifstream transaction_file("Transactions log.txt");
    if (!transaction_file.is_open())
        return;
    int acc_num;
    string transaction_type;
    double transaction_amounts;

    while (transaction_file >> acc_num >> transaction_type >> transaction_amounts){
        int index = index_helper(Bank_database, acc_num);
        if (index != -1){
            Transaction transaction_obj;
            transaction_obj.set_transaction_type(transaction_type);
            transaction_obj.set_transaction_amount(transaction_amounts);
            Bank_database[index].get_account().add_to_transaction_history(transaction_obj);
            if (transaction_type == "Deposit" || transaction_type == "Received"){
                Bank_database[index].get_account().update_balance(Bank_database[index].get_account().get_balance() + transaction_amounts);
            }
            else if (transaction_type == "Withdraw" || transaction_type == "Transfer"){
                Bank_database[index].get_account().update_balance(Bank_database[index].get_account().get_balance() - transaction_amounts);
            }
        }
    }
    transaction_file.close();
}

int main(){
    vector<Customer> BankDatabase;
    loading_accounts(BankDatabase);
    loading_transactions(BankDatabase);
    int authentication_choice = 0;
    while (authentication_choice != 3){
        cout << "Welcome to the banking system! Please enter the number of operation you want to perform.\n1. Register a new account.\n2. Login to existing account.\n3. Exit" << endl;
        cin >> authentication_choice;
        if (cin.fail()){
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input, please try again!" << endl;
            continue;
        }
        if (authentication_choice == 1){
            Customer Customerobj;
            Customerobj.register_new_user();
            BankDatabase.push_back(Customerobj);
        }
        else if (authentication_choice == 2){
            int login_account_number;
            string login_password;
            cout << "Enter your account number: ";
            cin >> login_account_number;
            cout << "Enter your password: ";
            cin >> login_password;
            int index = index_helper(BankDatabase, login_account_number);
            if (index != -1 && BankDatabase[index].get_password() == login_password){
                cout << "Login Successful!\nWelcome, " << BankDatabase[index].get_name() << "!" << endl;
                int operation_choice = 0;
                while (operation_choice != 5){
                    cout << "Welcome to the main menu! Please enter the number of operation you want to perform.\n1. Deposit money.\n2. Withdraw money.\n3. Transfer money.\n4. Show account details.\n5. Logout." << endl;
                    cin >> operation_choice;
                    if (cin.fail()){
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid input, please try again!" << endl;
                        continue;
                    }
                    if (operation_choice == 1){
                        BankDatabase[index].get_account().deposit_money();
                    }
                    else if (operation_choice == 2){
                        BankDatabase[index].get_account().withdraw_money();
                    }
                    else if (operation_choice == 3){
                        int other_account_number;
                        cout << "Enter the account number you want to transfer money to: ";
                        cin >> other_account_number;
                        int otherindex = index_helper(BankDatabase, other_account_number);
                        if (otherindex != -1){
                            BankDatabase[index].get_account().transfer_money(BankDatabase[otherindex].get_account());
                        }
                        else{
                            cout << "Error! The account number you entered doesn't exist!" << endl;
                        }
                    }
                    else if (operation_choice == 4){
                        BankDatabase[index].show_details();
                    }
                    else if (operation_choice == 5){
                        cout << "Logged out successfully!" << endl;
                    }
                    else{
                        cout << "Invalid input, please try again!" << endl;
                    }
                }
            }
            else{
                cout << "Login failed! Incorrect account number or password." << endl;
            }
        }
    }
    cout << "Thank you for visiting. Goodbye!" << endl;
    return 0;
}