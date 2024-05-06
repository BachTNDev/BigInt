// Bach Nguyen
// CECS 325-02
// Prog 6 -  BigInt class (arbitrarily large integers)
// Due Date: 05/09/2024
// I certify that this program is my own original work. I did not copy any part of this program from
// any other source. I further certify that I typed each and every line of code in this program.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip> 
#include <limits.h>

using namespace std;

class BigInt
{
    private:
        vector<char> v;
        BigInt fiboHelper(BigInt n, BigInt a = 0, BigInt b = 1)
        {
            if (n == BigInt(0))
                return a;
            else if (n == BigInt(1))
                return b;
            else
                return fiboHelper(n - BigInt(1), b, a + b);
        }

    public:
        BigInt(){}
        BigInt(int n);
        BigInt(string str);
        bool operator==(BigInt b);
        bool operator>=(BigInt b);
        bool operator<(BigInt b);
        bool operator<=(BigInt b);
        bool operator>(BigInt b);
        BigInt operator+(BigInt b);
        BigInt operator- (BigInt b);
        BigInt operator- (int n);
        BigInt operator* (BigInt b);
        BigInt multiplyByRepeatedAddition(BigInt a, BigInt b);
        BigInt operator/ (BigInt b);
        BigInt operator% (BigInt b);
        BigInt operator++(int);
        BigInt operator++( );
        void print();
        int size();
        BigInt fibo(); // calls fiboHelper
        BigInt fact();
        friend ostream& operator<<(ostream& out, const BigInt& b);
        friend BigInt operator+ (int, BigInt);
};

BigInt::BigInt(int n)
{
    if (n == 0)
    {
        v.push_back(0);
    }
    else
    {
        while (n > 0)
        {
            // turns int n into char
            // adds into v, digits right to left
            char digit = n%10;
            v.push_back(digit);
            n/= 10;
        }
    }
}

BigInt::BigInt(string str)
{
    // adds each char in string into v
    for (int i = str.size()-1; i >=0; i--)
    {
        v.push_back(str[i] - '0');
    }
}

bool BigInt::operator==(BigInt b)
{
    return v == b.v;
}

bool BigInt::operator<(BigInt b)
{
    if (v.size() != b.v.size())
        return v.size() < b.v.size();

    for (int i = v.size() - 1; i >= 0; --i)  
    {
        if (v[i] != b.v[i])
            return v[i] < b.v[i];
    }

    return false;
}

bool BigInt::operator>=(BigInt b)
{
    return !(*this < b);
}

bool BigInt::operator<=(BigInt b)
{
    return *this < b || *this == b;
}

bool BigInt::operator>(BigInt b)
{
    return !(*this <= b);
}

BigInt BigInt::operator+(BigInt b)
{
    BigInt result;
    int carry = 0;
    int i = 0;
    while (i < v.size() || i < b.v.size() || carry != 0)
    {
        if (i < v.size())
            carry += int(v[i]);
        if (i < b.v.size())
            carry += int(b.v[i]);
        result.v.push_back(carry % 10);
        carry /= 10;
        i++;
    }
    return result;
}


BigInt BigInt::operator- (BigInt b)
{
    BigInt result;
    int borrow = 0;
    for (int i = 0; i < v.size(); i++)
    {
        int diff = int(v[i]) - borrow;
        if (i < b.size())
            diff -= (int(b.v[i]));
        if (diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        else
            borrow = 0;
        result.v.push_back(diff);
    }
    while (result.v.size() > 1 && result.v.back() == 0)
        result.v.pop_back();
    return result;
}

// Subtraction with int
BigInt BigInt::operator-(int n)
{
    return *this - BigInt(n);
}

BigInt BigInt::multiplyByRepeatedAddition(BigInt a, BigInt b) {
    // Base case: if a is zero, return zero
    if (a == BigInt(0) || b == BigInt(0))
        return BigInt(0);

    // Base case: if a is 1, return b
    if (a == BigInt(1))
        return b;

    // Recursive case: multiply b by (a - 1) and add b to the result
    return b + multiplyByRepeatedAddition(a - BigInt(1), b);
}

BigInt BigInt::operator*(BigInt b) {
    // Determine which number is smaller
    BigInt smaller = (*this < b) ? *this : b;
    BigInt larger = (*this < b) ? b : *this;

    // Multiply smaller by larger using repeated addition
    return multiplyByRepeatedAddition(smaller, larger);
}

// Division
BigInt BigInt::operator/(BigInt b)
{
    // Initialize quotient
    BigInt quotient(0);

    // Keep subtracting b from *this until *this is less than b
    while (*this >= b)
    {
        *this = *this - b;
        quotient = quotient + BigInt(1);
    }

    return quotient;
}

// Modulo
BigInt BigInt::operator%(BigInt b)
{
    // Keep subtracting b from *this until *this is less than b
    while (*this >= b)
    {
        *this = *this - b;
    }

    return *this;
}


// Postfix increment
BigInt BigInt::operator ++(int)
{
    BigInt temp = *this;
    *this = *this + BigInt(1);
    return temp;
}

// Prefix increment
BigInt BigInt::operator++()
{
    *this = *this + 1;
    return *this;
}

// Print function
void BigInt::print()
{
    for (int i = v.size() - 1; i >= 0; i--)
        cout << int(v[i]);
}

// Size function
int BigInt::size()
{
    return v.size();
}

// Fibonacci
BigInt BigInt::fibo()
{
    return fiboHelper(*this);
}

// Factorial
BigInt BigInt::fact()
{
    BigInt result(1);
    for (BigInt i = 2; i <= *this; i++)
        result = result * i;
    return result;
}

// Output operator
ostream &operator<<(ostream &out, const BigInt &b)
{
    if (b.v.size() <= 12)
    {
        for (int i = b.v.size() - 1; i >= 0; i--)
            out << int(b.v[i]);
    }
    else
    {
        out << fixed << setprecision(7);
        out << int(b.v.back()); // Output the first digit
        out << '.'; // Decimal point
        for (int i = b.v.size() - 2; i >= b.v.size() - 7; i--) // Output next 6 digits
            out << int(b.v[i]);
        out << "e" << b.v.size() - 1; // Exponent is size - 1
    }
    return out;
}
// Addition with int
BigInt operator+(int n, BigInt b)
{
    return b + BigInt(n);
}

void testUnit()
{
    int space = 10;
    cout << "\a\nTestUnit:\n"<<flush;
    system("whoami");
    system("date");
    // initialize variables
    BigInt n1(25);
    BigInt s1("25");
    BigInt n2(1234);
    BigInt s2("1234");
    BigInt n3(n2);
    BigInt fibo(12345);
    BigInt fact(50);
    BigInt imax = INT_MAX;
    BigInt big("9223372036854775807");

    // display variables
    cout << "n1(int)    :"<<setw(space)<<n1<<endl;
    cout << "s1(str)    :"<<setw(space)<<s1<<endl;
    cout << "n2(int)    :"<<setw(space)<<n2<<endl;
    cout << "s2(str)    :"<<setw(space)<<s2<<endl;
    cout << "n3(n2)     :"<<setw(space)<<n3<<endl;
    cout << "fibo(12345):"<<setw(space)<<fibo<<endl;
    cout << "fact(50)   :"<<setw(space)<<fact<<endl;
    cout << "imax       :"<<setw(space)<<imax<<endl;
    cout << "big        :"<<setw(space)<<big<<endl;
    cout << "big.print(): "; big.print(); cout << endl;
    cout << endl;

    cout << n2 << "/"<< n1<< " = "<< n2/n1 <<" rem "<<n2%n1<<endl;
    cout << "fibo("<<fibo<<") = "<<fibo.fibo() << endl;
    cout << "fact("<<fact<<") = "<<fact.fact() << endl;

    cout << "10 + n1 = " << 10+n1 << endl;
    cout << "n1 + 10 = " << n1+10 << endl;

    cout << "(n1 == s1)? --> "<<((n1==s1)?"true":"false")<<endl;
    cout << "n1++ = ? --> before:"<<n1++<<" after:"<<n1<<endl;
    cout << "++s1 = ? --> before:"<<++s1<<" after:"<<s1<<endl;

    cout << "s2 * big = ? --> "<< s2 * big<<endl;
    cout << "big * s2 = ? --> "<< big * s2<<endl;
}

int main()
{
    testUnit();
    return 0;
}