#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
static fs::path main_direc = "./data";

void save_data(std::string &username, std::string &password, int &balance, int &PIN) {
    if (!(fs::exists(main_direc))) {
        fs::create_directory(main_direc);
    }
    fs::path userfolder = main_direc / ("Userdata_" + username);
    if (!(fs::exists(userfolder))) {
        fs::create_directory(userfolder);
    }

    try {
        fs::path userdfile = userfolder / ("UserData_" + username + ".txt");
        std::ofstream userdata(userdfile);

        if (userdata.is_open()) {
            userdata << username << '\n';
            userdata << password << '\n';
            userdata << balance << '\n';
            userdata << PIN << '\n';
            userdata.close();
        }
    } catch(fs::filesystem_error &e) {
        std::cout << "Error: " << e.what();
    }
};

bool get_data(std::string &lusername, std::string &lpassword, int &lbalance, int &lPIN) {
    fs::path userfolder = main_direc / ("Userdata_" + lusername);
    fs::path userdfile = userfolder / ("UserData_" + lusername + ".txt");
    fs::path user_translogfile = userfolder / ("User_TransLog_" + lusername + ".txt");
    std::ifstream userdata(userdfile);
    if (userdata.is_open()) {
        std::string fileUser;
        userdata >> fileUser;
        userdata >> lpassword;
        userdata >> lbalance;
        userdata >> lPIN;
        userdata.close();
        return (fileUser == lusername);
    } return false;
};

bool get_usertranslog(std::string &lusername) {
    fs::path userfolder = main_direc / ("Userdata_" + lusername);
    fs::path userdfile = userfolder / ("UserData_" + lusername + ".txt");
    fs::path user_translogfile = userfolder / ("User_TransLog_" + lusername + ".txt");
    std::ifstream translogfile(user_translogfile);
    if (translogfile.is_open()) {
        translogfile.close();
    }
}

void transaction(std::string tusername, int &pin, int &current_balance, std::string tpassword) {
    int trans_pin, trans_balance, target_balance, target_pin;
    std::string target_name, target_pass;

    std::cout << "Enter the recipient's username: ";
    std::cin >> target_name;

    fs::path target_folder = main_direc / ("Userdata_" + target_name);
    fs::path target_dfile = target_folder / ("UserData_" + target_name + ".txt");
    std::ifstream targetuserdata(target_dfile);

    if (!(targetuserdata.is_open())) {
        std::cout << "No account found with this name. ";
    } else {
        targetuserdata >> target_name;
        targetuserdata >> target_pass;
        targetuserdata >> target_balance;
        targetuserdata >> target_pin;
        std::cout << "Enter your amount for transaction: ";
        std::cin >> trans_balance;

        if (trans_balance <= current_balance) {
            std::cout << "THIS ACTION CAN'T BE UNDO, SO BEWARE.\n";
            std::cout << "Enter your PIN for continue: ";
            std::cin >> trans_pin;

            if (trans_pin == pin) {
                current_balance -= trans_balance;
                target_balance += trans_balance;

                std::cout << "Transacted successfully\n";
                std::cout << "You have transacted " << trans_balance << "$ to " << target_name << '\n';
                std::cout << "Remaining balance: " << current_balance << "$\n";

                save_data(tusername, tpassword, current_balance, pin);
                save_data(target_name, target_pass, target_balance, target_pin);
                targetuserdata.close();
            } else {
                std::cout << "Inccorect PIN\n";
            }
        } else {
            std::cout << "Insufficient funds\n";
        }
    }
};

bool login(std::string &password) {
    std::string compassword;
    int tries = 0, wait_time = 30;

    while (true) {
        std::cout << "Enter your password: ";
        std::cin >> compassword;

        if (compassword == password) {
            std::cout << "Login successful!\n";
            return true;
        } else {
            std::cout << "Incorrect password.\n";
            tries++;

            if (tries >= 3) {
                std::cout << "Too many failed attempts. Please wait before retrying.\n";    
                for (int i = wait_time; i > 0; i--) {
                    std::cout << "\rRetry available in: " << i << " seconds..";
                    std::cout.flush();
                }

                std::cout << "\nYou may try again now. ";
                tries = 2;
                wait_time *= 2;
            }
        }
    }
};

void new_account() {
    std::string username, password;
    int balance = 10000, pin;

    while (true) {
        std::cout << "Set a username: ";
        std::cin >> username;

        while (true) {
            std::cout << "Set a password (min 8 characters): ";
            std::cin >> password;

            if (password.size() >= 8) {
                break;
            } std::cout << "Password must be at least 8 characters. Try again.\n";
        }

        std::cout << "Set a 4-digit PIN (for withdrawals, deposits, and security): ";
        std::cin >> pin;
        save_data(username, password, balance, pin);
        break;
    }
};

class your_ATM {
private:
    int balance, pin;
    std::string atm_name, pass;
public:
    your_ATM(int b, std::string atm_n, int p, std::string pa) : balance(b), atm_name(atm_n), pin(p), pass(pa) {};

    auto Main() {
        system("cls");
        char ops;
        int cpin, withdraw_Money;
        std::string new_password;

        std::vector<std::string> menu = {
            "=====================================",
            "               ATM MENU              ",
            "=====================================",
            " [1] Check Balance",
            " [2] Transfer Funds",
            " [3] Withdraw Money",
            " [4] Deposit Money",
            " [5] Change Password",
            " [6] View Transaction History",
            " [7] Log out",
        };

        std::cout << atm_name << " account\n";
        for (std::string i : menu) {
            std::cout << i << '\n';
        }

        while (true) {
            std::cout << "Type in your options(1-7), m to show the menu again, q to quit and 'l' to clear screen: ";
            std::cin >> ops;

            if (ops == 'q' || ops == 'Q') {
                std::cout << "Data saved";
                break;
            }

            switch(ops) {
                case('m'):
                case('M'):
                    for (std::string i : menu) {
                        std::cout << i << '\n';
                    }

                case('l'):
                case('L'):
                    system("cls");
                    std::cout << "---Main menu---\n";
                    for (std::string i : menu) {
                        std::cout << i << '\n';
                    }

                    std::cout << "Type in your options(1-7), m to show the menu again, q to quit and 'l' to clear screen: ";

                case('1'):
                    std::cout << "Current balance: " << balance << "$\n";
                    break;

                case('2'):
                    transaction(atm_name, pin, balance, pass);
                    break;

                case('3'):
                    std::cout << "Enter your PIN: ";
                    std::cin >> cpin;

                    if (cpin != pin) {
                        std::cout << "Inccorect PIN. ";
                    } else {
                        std::cout << "Current balance: " << balance << "$\n";
                        std::cout << "Enter the amount you want to withdraw: ";
                        std::cin >> withdraw_Money;

                        if (withdraw_Money > balance) {
                            std::cout << "Insufficient funds";
                            break;
                        } else {
                            balance -= withdraw_Money;

                            std::cout << "Withdrawed successfully\n";
                            std::cout << "You withdrawed: " << withdraw_Money << "$\n";
                            std::cout << "Current balance: " << balance << "$\n";

                            save_data(atm_name, pass, balance, pin);
                            break;
                        }
                    }

                case('4'):
                    std::cout << "4\n";
                    break;

                case('5'):
                    std::cout << "Please type in your PIN to do this action:";
                    std::cin >> cpin;

                    if (cpin == pin) {
                        std::cout << "Please type in your new password (min 8 characters): ";
                        std::cin >> new_password;

                        if (new_password.size() < 8) {
                            std::cout << "Password length must be atleast 8 characters.\n";
                        } else {
                            if (new_password != pass) {
                                std::cout << "Password changed successfully\n";
                                save_data(atm_name, new_password, balance, pin);
                                break;
                            } else {
                                std::cout << "New password can't be the same as the old password.\n";
                                break;
                            }
                        }
                    } else {
                        std::cout << "Inccorect PIN\n";
                        break;
                    }

                case('6'):
                    std::cout << "===TRANSACTION HISTORY===\n";
                    break;

                case('7'):
                    std::cout << "7";
                    break;
            }
        }
    }
};

int main() {
    std::string lusername, lpassword;
    int lbalance, lpin;
    char ops;

    system("cls");  
    std::cout << "=== Welcome to ATM System ===" << std::endl;

    while (true) {
        std::cout << "|| Log in: l || Sign up: y || Quit: q" << std::endl;
        std::cout << "Choose: ";
        std::cin >> ops;

        switch (ops) {
            case 'l':
            case 'L': 
                while (true) {
                    std::cout << "Enter your username: ";
                    std::cin >> lusername;

                    if (!(get_data(lusername, lpassword, lbalance, lpin))) {
                        std::cout << "No account found with this name\n";
                    } else {
                        bool logged = login(lpassword);
                        if (logged == true) {
                            your_ATM myClass(lbalance, lusername, lpin, lpassword);
                            myClass.Main();
                            break;
                        }
                    }
             } break;

            case 'y':
            case 'Y': 
                std::cout << "Creating a new account for you . . .\n";
                new_account();
                break;

            case 'q':
            case 'Q':
                std::cout << "Goodbye!\n";
                break;

            default:
                std::cout << "Invalid option\n";
        } break;
    }

    return 0;
}
