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
            std::cout << "Account Details : " << std::endl;
            account.getDetails();
        }

        Account* getUserbyAccNum(long accNum)
        {
            for(Account account : accounts)
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
        }

        long checkBal(int accNum)
        {

        }

        void displayAccounts()
        {

        }
};

int main(){
    Bank bank;

    
    return 0;
}