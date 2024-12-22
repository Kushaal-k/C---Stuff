#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include "lib\jdbc\mysql_driver.h"
#include "lib\jdbc\mysql_connection.h"


#include "lib\jdbc\cppconn\prepared_statement.h"
#include "lib\jdbc\cppconn\resultset.h"

using namespace std;

class Account
{
private:
    string accountHolder;
    long accountNum;
    long accBalance;
    string accountType;

public:
    Account(string accountHolder, long accBalance, string accountType)
    {
        //use the current time as seed 
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        srand(seed);

        int randNum = 1000000 + rand() % 9000000;

        this->accountHolder = accountHolder;
        this->accountNum = randNum;
        this->accBalance = accBalance;
        this->accountType = accountType;
    }

    long getAccNum() const
    {
        return accountNum;
    }

    long getBal() const
    {
        return accBalance;
    }

    string getAccountHolder() const
    {
        return accountHolder;
    }

    string getAccountType() const
    {
        return accountType;
    }

    void setBal(long newBalance)
    {
        accBalance = newBalance;
    }

    void getDetails() const
    {
        std::cout << "Account Holder Name : " << accountHolder << std::endl;
        std::cout << "Account Number : " << accountNum << std::endl;
        std::cout << "Account Type : " << accountType << std::endl;
        std::cout << "Balance : " << accBalance << std::endl;
    }
};

class Bank
{
private:
    sql::Connection* con;

public:
    Bank()
    {
        try
        {
            sql::Driver* driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", "root", "Thisisryuk12");
            con->setSchema("banking_system");
        }
        catch (sql::SQLException& e)
        {
            std::cout << "Error connecting to database: " << e.what() << std::endl;
            exit(1);
        }
    }

    ~Bank()
    {
        delete con;
    }

    void createAcc(Account& account)
    {
        try
        {
            sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO accounts(accountNum, accountHolder, accBalance, accountType) VALUES(?,?,?,?)");
            pstmt->setInt64(1, account.getAccNum());
            pstmt->setString(2, account.getAccountHolder());
            pstmt->setInt64(3, account.getBal());
            pstmt->setString(4, account.getAccountType());
            pstmt->executeUpdate();

            std::cout << "Account Created in Database!!" << std::endl;
            std::cout << "------Account Details------" << std::endl;
            account.getDetails();
            delete pstmt;
        }
        catch (sql::SQLException& e)
        {
            std::cout << "Error creating account: " << e.what() << std::endl;
        }
    }

    Account* getUserbyAccNum(long accNum)
    {
        try
        {
            sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM accounts WHERE accountNum = ?");
            pstmt->setInt64(1, accNum);
            sql::ResultSet* res = pstmt->executeQuery();

            if (res->next())
            {
                string accountHolder = res->getString("accountHolder");
                long accBalance = res->getInt64("accBalance");
                string accountType = res->getString("accountType");

                Account* account = new Account(accountHolder, accBalance, accountType);
                delete pstmt;
                delete res;
                return account;
            }

            delete pstmt;
            delete res;
        }
        catch (sql::SQLException& e)
        {
            std::cout << "Error retrieving account: " << e.what() << std::endl;
        }
        return nullptr;
    }

    void deposit(int amount, long accNum)
    {
        Account* account = getUserbyAccNum(accNum);
        if (account)
        {
            account->setBal(account->getBal() + amount);

            try
            {
                sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE accounts SET accBalance = ? WHERE accountNum = ?");
                pstmt->setInt64(1, account->getBal());
                pstmt->setInt64(2, account->getAccNum());
                pstmt->executeUpdate();

                std::cout << "Balance Updated in Database!!" << std::endl;
                delete pstmt;
            }
            catch (sql::SQLException& e)
            {
                std::cout << "Error updating balance: " << e.what() << std::endl;
            }

            delete account;
        }
        else
        {
            std::cout << "Account Not Found!!" << std::endl;
        }
    }

    void withdraw(int amount, long accNum)
    {
        Account* account = getUserbyAccNum(accNum);
        if (account != nullptr)
        {
            if (account->getBal() >= amount)
            {
                account->setBal(account->getBal() - amount);

                try
                {
                    sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE accounts SET accBalance = ? WHERE accountNum = ?");
                    pstmt->setInt64(1, account->getBal());
                    pstmt->setInt64(2, account->getAccNum());
                    pstmt->executeUpdate();

                    std::cout << "Balance Updated in Database!!" << std::endl;
                    delete pstmt;
                }
                catch (sql::SQLException& e)
                {
                    std::cout << "Error updating balance: " << e.what() << std::endl;
                }

            }
            else
            {
                std::cout << "Insufficient Balance!!" << std::endl;
            }

            delete account;
        }
        else
        {
            std::cout << "Account Not Found!!" << std::endl;
        }
    }

    void checkBal(int accNum)
    {
        Account* account = getUserbyAccNum(accNum);
        if (account != nullptr)
        {
            std::cout << "Account Balance: " << account->getBal() << std::endl;
            delete account;
        }
        else
        {
            std::cout << "Account Not Found!!" << std::endl;
        }
    }

    void displayAccounts()
    {
        try
        {
            sql::Statement* stmt = con->createStatement();
            sql::ResultSet* res = stmt->executeQuery("SELECT * FROM accounts");

            std::cout << "List of all the accounts: " << std::endl;
            while (res->next())
            {
                long accountNum = res->getInt64("accountNum");
                long balance = res->getInt64("accBalance");
                std::cout << accountNum << "  " << balance << std::endl;
            }

            delete stmt;
            delete res;
        }
        catch (sql::SQLException& e)
        {
            std::cout << "Error retrieving account list: " << e.what() << std::endl;
        }
    }
};

int main() {
    Bank bank;

    do
    {
        std::cout << "--------Banking System--------" << std::endl;
        std::cout << "1. Create Account" << std::endl;
        std::cout << "2. Deposit" << std::endl;
        std::cout << "3. Withdraw" << std::endl;
        std::cout << "4. Account Details" << std::endl;
        std::cout << "5. Exit" << std::endl;

        char ch;
        std::cout << "Enter your choice: ";
        std::cin >> ch;

        switch (ch)
        {
        case '1':
        {
            string name, accType;
            char aT;
            long balance;

            std::cout << "Enter Your Name: ";
            cin.ignore();
            getline(cin, name);

            std::cout << "Enter the type of account you want(C/S): ";
            std::cin >> aT;
            if (aT == 'c' || aT == 'C')
                accType = "Current";
            else if (aT == 's' || aT == 'S')
                accType = "Savings";
            else
            {
                std::cout << "Enter valid choice." << std::endl;
                break;
            }

            std::cout << "Enter amount you want to deposit: ";
            std::cin >> balance;
            if (balance < 0)
            {
                std::cout << "Initial amount cannot be negative." << std::endl;
                break;
            }

            Account account(name, balance, accType);
            bank.createAcc(account);
            break;
        }
        case '2':
        {
            long accNum, amount;

            std::cout << "Enter Account Number: ";
            std::cin >> accNum;

            std::cout << "Enter amount to deposit: ";
            std::cin >> amount;

            bank.deposit(amount, accNum);
            break;
        }
        case '3':
        {
            long accNum, amount;

            std::cout << "Enter Account Number: ";
            std::cin >> accNum;

            std::cout << "Enter amount to withdraw: ";
            std::cin >> amount;

            bank.withdraw(amount, accNum);
            break;
        }
        case '4':
        {
            long accNum;

            std::cout << "Enter Account Number: ";
            std::cin >> accNum;

            Account* account = bank.getUserbyAccNum(accNum);

            if (account)
            {
                account->getDetails();
                delete account;
            }
            else
            {
                std::cout << "Account Not Found!!" << std::endl;
            }
            break;
        }
        case '5':
        {
            std::cout << "Thanks for using the banking system." << std::endl;
            break;
        }
        default:
            std::cout << "Enter valid choice." << std::endl;
        }

        if (ch == '5')
            break;
    } while (true);

    return 0;
}
