#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <exception>
using namespace std;

class InvalidMarksException : public exception
{
    string message;

public:
    InvalidMarksException(const string &subject, int value)
    {
        message = "Invalid marks for " + subject + ": " + to_string(value) +
                  " (allowed range is 0-100)";
    }

    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

class Student
{
protected:
    string name;
    string usn;

public:
    Student(const string &n, const string &u) : name(n), usn(u) {}
    virtual ~Student() {}
    virtual void display() const = 0;
};

class Result : public Student
{
    int marks[3];
    int total;
    double percentage;

    static void check(const string &subject, int m)
    {
        if (m < 0 || m > 100)
            throw InvalidMarksException(subject, m);
    }

public:
    Result(const string &n, const string &u, int m1, int m2, int m3)
        : Student(n, u)
    {
        check("Subject 1", m1);
        check("Subject 2", m2);
        check("Subject 3", m3);
        marks[0] = m1;
        marks[1] = m2;
        marks[2] = m3;
        total = m1 + m2 + m3;
        percentage = total / 3.0;
    }

    int getTotal() const { return total; }
    string getName() const { return name; }

    string getGrade() const
    {
        if (percentage >= 90) return "A+";
        if (percentage >= 75) return "A";
        if (percentage >= 60) return "B";
        if (percentage >= 40) return "C";
        return "F";
    }

    bool isPass() const
    {
        return total >= 120;
    }

    void display() const override
    {
        cout << "\nName       : " << name << endl;
        cout << "USN        : " << usn << endl;
        cout << "Marks      : " << marks[0] << ", " << marks[1]
             << ", " << marks[2] << endl;
        cout << "Total      : " << total << endl;
        cout << "Percentage : " << fixed << setprecision(2) << percentage << "%" << endl;
        cout << "Grade      : " << getGrade() << endl;
        cout << "Result     : " << (isPass() ? "PASS" : "FAIL") << endl;
    }

    void saveToFile(ofstream &out) const
    {
        out << name << "," << usn << ","
            << marks[0] << "," << marks[1] << "," << marks[2] << ","
            << total << "," << getGrade() << "\n";
    }
};

int main()
{
    vector<Result> records;

    try
    {
        records.push_back(Result("R Bhanu Sai Ganesh", "NB25ECE148", 92, 88, 95));
        records.push_back(Result("Student Two", "NB25ECE901", 70, 65, 72));
        records.push_back(Result("Student Three", "NB25ECE902", 30, 35, 40));
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    try
    {
        records.push_back(Result("Student Four", "NB25ECE903", 80, 105, 60));
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\n===== STUDENT RESULT DETAILS =====";
    for (const auto &r : records)
        r.display();

    ofstream fout("students.txt");
    if (!fout)
    {
        cout << "Could not open students.txt" << endl;
        return 1;
    }

    for (const auto &r : records)
        r.saveToFile(fout);
    fout.close();

    const int limit = 250;
    auto above = [limit](const Result &r) { return r.getTotal() > limit; };

    int count = count_if(records.begin(), records.end(), above);
    cout << "\nStudents with total above " << limit << ": " << count << endl;

    vector<Result> toppers;
    copy_if(records.begin(), records.end(), back_inserter(toppers), above);

    for (const auto &t : toppers)
        cout << " - " << t.getName() << " (" << t.getTotal() << ")" << endl;

    cout << "\nRecords saved to students.txt successfully." << endl;
    return 0;
}