#include <iostream> 
#include <bits/stdc++.h>
#include <cstdlib>
#include <chrono>

using namespace std;

class Account
{
    private :
        string accountHolder;
        long accountNum;
        long accBalance;
        string accountType;

    public:
        Account(string accountHolder,long accBalance,string accountType)
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

        long getAccNum()
        {
            return accountNum;
        }

        long getBal()
        {
            return accBalance;
        }

        void setBal(long newBalance)
        {
            accBalance = newBalance;
        }

        void getDetails()
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
        vector<Account> accounts;
        int n = 1000;

    public:
        Bank()
        {
        }

        void createAcc(Account account)
        {
            accounts.push_back(account);
            std::cout << "Account Created!!" << std::endl;
            std::cout << "-----Account Details-----" << std::endl;
            account.getDetails();
        }

        Account* getUserbyAccNum(long accNum)
        {
            for(Account& account : accounts)
            {
                if(account.getAccNum() == accNum)
                {
                    return &account;
                }
            } 
            return nullptr;

        }

        void searchAcc(long accNum)
        {
            for(Account& account : accounts)
            {
                if(account.getAccNum() == accNum)
                {   
                    std::cout << "Account Found!!" << std::endl;
                    std::cout << "Do you want to see details? (Y/N)" << std::endl;
                    char ch;
                    std::cin >> ch;
                    if(ch == 'Y' || ch == 'y')
                        account.getDetails();
                    return;
                }
            }
            std::cout << "Account Not Found!!" << std::endl;
        }

        void deposit(int amount,long accNum) 
        {   
            Account* account = getUserbyAccNum(accNum);
            if(account)
            {
                account->setBal(account->getBal()+amount);
                std::cout << "Balance Updated!!" << std::endl;
            }
            else 
            {
                std::cout << "Account Not Found!!" << std::endl;
            }
        }

        void withdraw(int amount,long accNum)
        {
            Account* account = getUserbyAccNum(accNum);
            if(account != nullptr)
            {
                if(account->getBal() > amount)
                {
                    account->setBal(account->getBal() - amount);
                    std::cout << "Balance Updated!!" << std::endl;
                }
                else 
                {
                    std::cout << "Insufficient Balance!!" << std::endl;
                    return;
                }
                
            }
            else
            {
                std::cout << "Account Not Found!!" << std::endl;
            }
        }

        void checkBal(int accNum)
        {
            Account* account = getUserbyAccNum(accNum);

            if(account != nullptr)
            {
                std::cout << "Account Balance : " << account->getBal() << std::endl;
            }
            else
            {
                std::cout << "Account Not Found!!" << std::endl;
            }
        }

        void displayAccounts()
        {
            std::cout << "List of all the accounts : " << std::endl;
            for(Account account : accounts)
            {
                std::cout << account.getAccNum() << "  " << account.getBal() << std::endl;
            }
        }
};

int main(){
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
        std::cout << "Enter your choice : ";
        std::cin >> ch ;

        switch(ch)
        {
            case '1' :
            {
                string name,accType;
                char aT;
                long balance;

                std::cout << "Enter Your Name : " ;
                cin.ignore();
                getline(cin,name);

                std::cout << "Enter the type of account you want(C/S) : ";
                std::cin >> aT ;
                if(aT == 'c' || aT == 'C')
                    accType = "Current";
                else if(aT == 's' || aT == 'S')
                    accType = "Savings";
                else    
                {
                    std::cout << "Enter valid choice." <<std::endl;
                    break;
                }
                    

                std::cout << "Enter amount you want to deposit : " ;
                std::cin >> balance;
                if(balance < 0)
                {
                    std::cout << "Initial amount cannot be negative." << std::endl;
                    break;
                }


                do {
                    Account account(name, balance, accType);
                    if (!bank.getUserbyAccNum(account.getAccNum())) {
                        bank.createAcc(account);
                        break;
                    }
                } while (true);


                break;
            }
            case '2': 
            {
                long accNum,amount;

                std::cout << "Enter Account Number : " ;
                std::cin >> accNum;

                std:cout << "Enter amount to deposit : " ;
                std::cin >> amount;

                bank.deposit(amount,accNum);

                break;
            }
            case '3' :
            {
                long accNum,amount;

                std::cout << "Enter Account Number : " ;
                std::cin >> accNum;

                std::cout << "Enter amount to withdraw : " ;
                std::cin >> amount;

                bank.withdraw(amount,accNum);

                break;
            }
            case '4' :
            {
                long accNum;

                std::cout << "Enter Account Number : " ;
                std::cin >> accNum;

                Account* account = bank.getUserbyAccNum(accNum);
                
                if(account)
                {
                    account->getDetails();
                }
                else{
                    std::cout << "Account Not Found!!" << std::endl;
                } 

                break;
            }
            case '5':
            {
                std::cout << "Thanks for using the banking system." << std::endl;
                break;
            }
            default : "Enter valid choice.";
        }

        if(ch == '5')
            break;
    } while(true);
    
    return 0;
}