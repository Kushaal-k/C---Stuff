#include <iostream>
#include <cstdlib>
#include <chrono>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <vector>
#include <limits>

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

class Account
{
private:
    string accountHolder;
    long accountNum;
    long accBalance;
    string accountType;

public:
    // Constructor for new account creation
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

    // Constructor for existing account (from database)
    Account(string accountHolder, long accountNum, long accBalance, string accountType)
        : accountHolder(accountHolder), accountNum(accountNum),
        accBalance(accBalance), accountType(accountType) {
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
                long accountNumber = res->getInt64("accountNum");
                long accBalance = res->getInt64("accBalance");
                string accountType = res->getString("accountType");

                Account* account = new Account(accountHolder, accountNumber, accBalance, accountType);
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

    void deposit(long amount, long accNum)
    {
        if (amount <= 0) {
            std::cout << "Deposit amount must be positive!" << std::endl;
            return;
        }

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
                std::cout << "New Balance: " << account->getBal() << std::endl;
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

    void withdraw(long amount, long accNum)
    {
        if (amount <= 0) {
            std::cout << "Withdrawal amount must be positive!" << std::endl;
            return;
        }

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
                    std::cout << "New Balance: " << account->getBal() << std::endl;
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

    void checkBal(long accNum)
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

            std::cout << "List of all accounts:" << std::endl;
            std::cout << "Account Number  |  Balance" << std::endl;
            std::cout << "------------------------" << std::endl;
            while (res->next())
            {
                long accountNum = res->getInt64("accountNum");
                long balance = res->getInt64("accBalance");
                std::cout << accountNum << "  |  " << balance << std::endl;
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

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Bank bank;

    do
    {
        std::cout << "\n--------Banking System--------" << std::endl;
        std::cout << "1. Create Account" << std::endl;
        std::cout << "2. Deposit" << std::endl;
        std::cout << "3. Withdraw" << std::endl;
        std::cout << "4. Account Details" << std::endl;
        std::cout << "5. Display All Accounts" << std::endl;
        std::cout << "6. Exit" << std::endl;

        char ch;
        std::cout << "Enter your choice: ";
        std::cin >> ch;
        clearInputBuffer();

        switch (ch)
        {
        case '1':
        {
            string name, accType;
            char aT;
            long balance;

            std::cout << "Enter Your Name: ";
            getline(cin, name);

            std::cout << "Enter the type of account you want(C/S): ";
            std::cin >> aT;
            clearInputBuffer();

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
            if (!(std::cin >> balance)) {
                std::cout << "Invalid amount entered!" << std::endl;
                clearInputBuffer();
                break;
            }
            clearInputBuffer();

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
            if (!(std::cin >> accNum)) {
                std::cout << "Invalid account number!" << std::endl;
                clearInputBuffer();
                break;
            }

            std::cout << "Enter amount to deposit: ";
            if (!(std::cin >> amount)) {
                std::cout << "Invalid amount!" << std::endl;
                clearInputBuffer();
                break;
            }
            clearInputBuffer();

            bank.deposit(amount, accNum);
            break;
        }
        case '3':
        {
            long accNum, amount;

            std::cout << "Enter Account Number: ";
            if (!(std::cin >> accNum)) {
                std::cout << "Invalid account number!" << std::endl;
                clearInputBuffer();
                break;
            }

            std::cout << "Enter amount to withdraw: ";
            if (!(std::cin >> amount)) {
                std::cout << "Invalid amount!" << std::endl;
                clearInputBuffer();
                break;
            }
            clearInputBuffer();

            bank.withdraw(amount, accNum);
            break;
        }
        case '4':
        {
            long accNum;

            std::cout << "Enter Account Number: ";
            if (!(std::cin >> accNum)) {
                std::cout << "Invalid account number!" << std::endl;
                clearInputBuffer();
                break;
            }
            clearInputBuffer();

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
            bank.displayAccounts();
            break;
        }
        case '6':
        {
            std::cout << "Thanks for using the banking system." << std::endl;
            break;
        }
        default:
            std::cout << "Enter valid choice." << std::endl;
        }

        if (ch == '6')
            break;
    } while (true);

    return 0;
}