//swapping by pointers
#include <iostream>
using namespace std;

// Function definition using pointers
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int main()
{
    int a = 10;
    int b = 20;

    cout << "before swap : " << "a = " << a << " b = " << b << endl;

    swap(&a, &b);

    cout << "after swap : " << "a = " << a << " b = " << b << endl;

    return 0;
}