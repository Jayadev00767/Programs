#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int rollNo;
    float marks;
    static int count;

public:
    Student(string n, int r, float m) {
        name = n;
        rollNo = r;
        marks = m;
        count++;
    }

    void display() {
        cout << name << " | Roll: " << rollNo << " | Marks: " << marks << endl;
    }

    void display(string title) {
        cout << title << ": ";
        display();
    }

    static int getCount() {
        return count;
    }

    bool operator>(Student s) {
        return marks > s.marks;
    }

    friend void showResult(Student s);
};

int Student::count = 0;

void showResult(Student s) {
    if (s.marks >= 40)
        cout << s.name << " has Passed" << endl;
    else
        cout << s.name << " has Failed" << endl;
}

int main() {
    Student s1("Aarav", 101, 88.5);
    Student s2("Diya", 102, 92);

    s1.display();
    s2.display("Student 2");

    cout << "Total students: " << Student::getCount() << endl;

    showResult(s1);
    showResult(s2);

    if (s1 > s2)
        cout << "Aarav has higher marks" << endl;
    else
        cout << "Diya has higher marks" << endl;

    return 0;
}