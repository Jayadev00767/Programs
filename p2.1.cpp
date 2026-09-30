//p2.1 function overloading:
#include <iostream>
using namespace std;

// Inline function
inline int square(int x)
{
    return x * x;
}

// Function overloading
double area(double r)
{
    return 3.14159 * r * r;   // Circle
}

int area(int l, int b)
{
    return l * b;             // Rectangle
}

double area(double b, double h)
{
    return 0.5 * b * h;       // Triangle
}

int main()
{
    cout << "Square(6) = " << square(6) << endl;

    cout << "Circle r=2 = " << area(2.0) << endl;

    cout << "Rectangle 4x5 = " << area(4, 5) << endl;

    cout << "Triangle b=3 h=8 = " << area(3.0, 8.0) << endl;

    return 0;
}

