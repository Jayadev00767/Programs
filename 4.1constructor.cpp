#include <iostream>
using namespace std;

class Tracer
{
    int id;

public:
    Tracer(int i) : id(i)
    {
        cout << "Construct #" << id << endl;
    }

    ~Tracer()
    {
        cout << "Destruct #" << id << endl;
    }
};

int main()
{
    cout << "Enter block In" << endl;

    {
        Tracer a(1), b(2);
        cout << "...working..." << endl;
    }

    cout << "Left block In" << endl;

    return 0;
}