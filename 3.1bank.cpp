#include <iostream>
#include <string>

using namespace std;

class BankAccount
{
private:
    string owner;
    double balance;

public:
    // Open account
    void open(const string &name, double initial)
    {
        owner = name;
        balance = (initial > 0) ? initial : 0;
    }

    // Deposit money
    void deposit(double amount)
    {
        if (amount > 0)
            balance += amount;
    }

    // Withdraw money
    bool withdraw(double amount)
    {
        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            return true;
        }

        return false;
    }

    // Get balance
    double getBalance() const
    {
        return balance;
    }

    // Get owner name
    string getOwner() const
    {
        return owner;
    }
};

int main()
{
    BankAccount a;

    a.open("Asha", 1000);

    a.deposit(500);

    if (!a.withdraw(2000))
    {
        cout << "Withdraw denied (insufficient balance)" << endl;
    }

    a.withdraw(500);

    cout << a.getBalance() << endl;
    cout << a.getOwner() << endl;

    return 0;
}