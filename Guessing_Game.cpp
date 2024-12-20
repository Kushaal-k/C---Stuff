#include <iostream> 
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int maxTry = 5;

int main(){

    srand(time(0));
    int randomNumber = rand()%100 + 1;

    int guess;
    int tryCount =0;

    char play = 'y';

    while(play != 'n' && play !='N')
    {
        if(tryCount >= maxTry){
            std::cout << "Your used all your trails." << std::endl;
            std::cout << "Wanna play again? " ;
            std::cin >> play ;
            if(play == 'y' || play == 'Y')
            {
                tryCount= 0;
                srand(time(0));
                randomNumber = rand()%100 + 1;
            }
            else 
                break;

        }

        std::cout << "Guess the number : " ;
        std::cin >> guess;
        tryCount++;

    
        if(guess == randomNumber)
        {
            std::cout << "You guessed it right !!!!!!" << std::endl;
            std::cout << "Wanna play again? " ;
            std::cin >> play ;

            if(play == 'y' || play == 'Y')
            {
                tryCount= 0;
                srand(time(0));
                randomNumber = rand()%100 + 1;
            }
            else 
                break;
           
        }
        else if(guess < 0 || guess > 100)
        {
            std::cout << "Guess the number between 1 to 100." << std::endl;
        }
        else if(randomNumber - guess >= 10)
        {
            std::cout << "Too Low!" << std::endl;
        }
        else if(guess - randomNumber >= 10)
        {
            std::cout << "Too High!" << std::endl;
        }
        else
        {
            std::cout << "Very Close!!" << std::endl;
        }
    }

        
    return 0;
}