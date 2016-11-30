/*
 * Scott Downey
 * scott.m.downey@gmail.com
 * C/C++ Programming IV : Advanced Programming with Objects, 116161, Raymond Mitchell III
 * August 11, 2016
 * BigInt.h
 * Windows 10
 * Visual Studio Community 2015
 *
 * This source code declares the functions and members of class 'BigInt.'
 *
 */

#ifndef BIGINT_H
#define BIGINT_H

#include <vector>
#include <string>
#include <iosfwd>
using namespace std;

namespace Project1
{
    class BigInt
    {
        friend const BigInt operator+(const BigInt &, const BigInt &);
        friend const BigInt operator-(const BigInt &, const BigInt &);
        friend bool operator==(const BigInt &, const BigInt &);
        friend bool operator!=(const BigInt &, const BigInt &);
        friend bool operator<(const BigInt &, const BigInt &);
        friend bool operator<=(const BigInt &, const BigInt &);
        friend bool operator>(const BigInt &, const BigInt &);
        friend bool operator>=(const BigInt &, const BigInt &);
        friend ostream &operator<<(ostream &, const BigInt &);
        friend istream &operator>>(istream &, BigInt &);

    public:
        BigInt();
        BigInt(const BigInt &);
        BigInt(long long);
        // Throws invalid_argument if string malformed (contains
        // anything other than a legally formatted number)
        BigInt(const string &);

        const BigInt &operator=(const BigInt &);
        const BigInt &operator+=(const BigInt &);
        const BigInt &operator-=(const BigInt &);

    private:
        //represents sign of the number (equal to true if negative)
        bool IS_NEGATIVE;

        //holds the number - each vector index is a digit of the number
        vector<int> number;

        //various helper functions to implement the public interface
        void stringIntoVector(const string &str);
        void numberValidity(const vector<int> &num);
        void subFix(int currentIndex);
        void addHelper(const BigInt &);
        void minusHelper(const BigInt &);
    };
}

#endif
