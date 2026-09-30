//p2.2  Default Argument:

#include <iostream>
#include <string>
using namespace std;

// Default argument: level = 1
void logMsg(const string &msg, int level = 1)
{
    const string tag[] = {"", "INFO", "WARN", "ERROR"};

    cout << "[" << tag[level] << "] " << msg << endl;
}

// Default argument: rate = 7.5
double interest(double principal, double years, double rate = 7.5)
{
    return principal * rate * years / 100.0;
}

int main()
{
    // Default level = 1 is used
    logMsg("System started");

    // level = 2 is provided by the caller
    logMsg("Low memory", 2);

    // Default rate = 7.5 is used
    cout << "Interest = "
         << interest(10000, 2) << endl;

    // Custom rate = 9.0 is provided
    cout << "Interest = "
         << interest(10000, 2, 9.0) << endl;

    return 0;
}