// swapping by call by value
#include <iostream>
using namespace std;

int main() 
{
    int a = 10;
    int b = 20;

    cout << "Before swap: a = " << a << ", b = " << b << endl;
    
    // Calling the function using call by value
    swap(a, b);

    cout << "After swap: a = " << a << ", b = " << b << endl;
}
void swap(int x, int y) 
{
    int t = x;
    x = y;
    y = t;
}