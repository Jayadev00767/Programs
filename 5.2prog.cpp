#include <iostream>
using namespace std;

class Distance {
    int feet, inch;

public:
    void set(int f, int i) {
        feet = f;
        inch = i;
    }

    void show() {
        cout << feet << "ft " << inch << "in\n";
    }

    friend Distance add(Distance a, Distance b);
};

Distance add(Distance a, Distance b) {
    Distance r;
    int total = (a.feet + b.feet) * 12 + a.inch + b.inch;
    r.feet = total / 12;
    r.inch = total % 12;
    return r;
}

int main() {
    Distance d1, d2, d3;
    d1.set(5, 8);
    d2.set(3, 7);

    d3 = add(d1, d2);
    d3.show();

    return 0;
}