#include <fstream>
using namespace std;

ifstream cin("fibo0.in");
ofstream cout("fibo0.out");

int main()
{
    int x, y, k = 0;
    while(cin >> x >> y)
    {
        if(y % x == 0)
            k++;
    }
    cout << k;
    return 0;
}
