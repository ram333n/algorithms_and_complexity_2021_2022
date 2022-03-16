#include "data_types.h"

using namespace std;

int main() {
    cout << (RationalNumber(1, 2) < RationalNumber(1, 3)) << endl;
    cout << (RationalNumber(1, -2) < RationalNumber(1, 3)) << endl;
    cout << (RationalNumber(-1, -2) == RationalNumber(2, 4)) << endl;
    cout << (RationalNumber(0, 1) == RationalNumber()) << endl;
    cout << (RationalNumber(0, 1) == RationalNumber(0,-435543)) << endl;
    cout << RationalNumber(44, -104);
    return 0;
}