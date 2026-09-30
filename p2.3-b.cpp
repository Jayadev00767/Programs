//p2.3.2 Call by pointer.

#include <iostream>
using namespace std;

void minMax(int a[], int n, int *min, int *max)
{
    *min = a[0];
    *max = a[0];

    for (int i = 1; i < n; i++)
    {
        if (a[i] < *min)
            *min = a[i];

        if (a[i] > *max)
            *max = a[i];
    }
}

int main()
{
    int data[] = {7, 2, 9, 4, 1};

    int min, max;

    minMax(data, 5, &min, &max);

    cout << "Minimum = " << min << endl;
    cout << "Maximum = " << max << endl;

    return 0;
}