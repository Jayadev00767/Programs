// swapping by call by reference
#include <iostream>
using namespace std;

// Function declaration (prototype)
void swap(int &x, int &y);

int main()
{
    int a = 10;
    int b = 20;

    cout << "Before swap: a = " << a << " b = " << b << endl;

    swap(a, b);

    cout << "After swap: a = " << a << " b = " << b << endl;

    return 0;
}

// Function definition using references
void swap(int &x, int &y)
{
    int t = x;
    x = y;
    y = t;
}