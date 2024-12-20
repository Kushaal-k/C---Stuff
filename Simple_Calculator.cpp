#include <iostream> 

using namespace std;

void addition(int a,int b)
{
    std::cout << a+b << std::endl;
}

void subtraction(int a,int b)
{
    std::cout << a-b << std::endl;
}

void multiplication(int a,int b)
{
    std::cout << a*b << std::endl;
}

void division(int a,int b)
{   
    if(b==0){
        throw "Division by zero is not possible.";
    }
    std::cout << a/b << std::endl;
}

int main(){
    
    int a,b;

    std::cout << "Enter two number : " << std::endl;
    std::cin >> a >> b;

    char op ;
    std::cout << "+ Addition" << std::endl;
    std::cout << "- Subtraction" << std::endl;
    std::cout << "* Multiplication" << std::endl;
    std::cout << "/ Division" << std::endl;
    std::cout << "Enter the operation : " ;
    std::cin >> op;

    switch(op)
    {
        case '+' : addition(a,b);
            break;
        case '-' : subtraction(a,b);
            break;
        case '*' : multiplication(a,b);
            break;
        case '/' : try{
                    division(a,b);
                    }
                   catch (const char* e) {
                        std::cout << "Error: " << e << std::endl;
                    }
                   break;
        default : std::cout << "Enter Valid Operation." << std::endl ;
    }


    return 0;
}