/*
 * Scott Downey
 * scott.m.downey@gmail.com
 * C/C++ Programming IV : Advanced Programming with Objects, 116161, Raymond Mitchell III
 * August 11, 2016
 * BigInt.cpp
 * Windows 10
 * Visual Studio Community 2015
 *
 * This source code defines the implementation of the functions and members of class 'BigInt.'
 *
 */

#include <stdexcept>
#include <iterator>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#include "BigInt.h"
using Project1::BigInt;

//operator '+' overload (implemented in terms of '+=')
const BigInt Project1::operator+(const BigInt & lhs, const BigInt & rhs)
{
    BigInt newInt = lhs;
    newInt += rhs;

    return newInt;
}

//operator '-' overload (implemented in terms of '-=')
const BigInt Project1::operator-(const BigInt & lhs, const BigInt & rhs)
{
    BigInt newInt = lhs;
    newInt -= rhs;
    return newInt;
}

//operator '==' overload
bool Project1::operator==(const BigInt &first, const BigInt &second)
{
    return ((first.IS_NEGATIVE == second.IS_NEGATIVE) && (first.number == second.number));
}

//operator '!=' overload
bool Project1::operator!=(const BigInt &first, const BigInt &second)
{
    return !(first == second);
}

//operator '<' overload (implemented in terms of '>=')
bool Project1::operator<(const BigInt &first, const BigInt &second)
{
    return !(first >= second);
}

//operator '<=' overload (implemented in terms of '>' and '==')
bool Project1::operator<=(const BigInt &first, const BigInt &second)
{
    return !(first > second) || (first == second);
}

//operator '>' overload
bool Project1::operator>(const BigInt &first, const BigInt &second)
{
    //if one is negative and the other isn't
    if (!first.IS_NEGATIVE && second.IS_NEGATIVE)
        return true;

    //if both negative and different sizes
    if (first.IS_NEGATIVE && second.IS_NEGATIVE)
        return first.number.size() < second.number.size();

    //if both positive and different sizes
    return first.number.size() > second.number.size();
}

//operator '>=' overload (implemented in terms of '>' and '==')
bool Project1::operator>=(const BigInt & first, const BigInt & second)
{
    return ((first > second) || (first == second));
}

//output stream operator for 'BigInt'
ostream & Project1::operator<<(ostream & os, const BigInt & bigInt)
{
    ostream_iterator<int> streamIterator(os);

    if (bigInt.IS_NEGATIVE)
        os << "-";

    copy(bigInt.number.begin(), bigInt.number.end(), streamIterator);

    return os;
}

//input stream overload to use for type 'BigInt'
istream & Project1::operator>>(istream & is, BigInt & bigInt)
{
    string input;
    is >> input;

    BigInt tempInt(input);
    bigInt = tempInt;

    return is;
}

//default constructor for 'BigInt' (no args)
Project1::BigInt::BigInt()
{
    IS_NEGATIVE = false;
    number = { 0 };
}

//copy constructor for 'BigInt'
Project1::BigInt::BigInt(const BigInt & other)
{
    IS_NEGATIVE = other.IS_NEGATIVE;
    number = other.number;
}

//constructor for a 'BigInt' fron a type 'long long'
Project1::BigInt::BigInt(long long longInt)
{
    string longString;
    stringstream ss;

    //convert 'long long' into string
    ss << longInt;
    ss >> longString;

    //contruct 'BigInt' through string
    stringIntoVector(longString);
}

//constructor for 'BigInt' from a string
Project1::BigInt::BigInt(const string & str)
{
    stringIntoVector(str);
}

//overload assignment operator
const BigInt & Project1::BigInt::operator=(const BigInt & other)
{
    IS_NEGATIVE = other.IS_NEGATIVE;
    number.clear();
    copy(other.number.begin(), other.number.end(), back_inserter(number));
    return *this;
}

//overload '+=' operator - minusHelper and addHelper are used for implementation
const BigInt & Project1::BigInt::operator+=(const BigInt & other)
{
    //if both negative or both positive, add the values
    if (IS_NEGATIVE == other.IS_NEGATIVE)
    {
        addHelper(other);
    }
    //if one is negative and other is positive, subtract the values
    else
    {
        minusHelper(other);
    }
    return *this;
}


//overload '-=' operator - minusHelper and addHelper are used for implementation
const BigInt & Project1::BigInt::operator-=(const BigInt & other)
{
    //if both negative or both positive, subtract the values
    if (IS_NEGATIVE == other.IS_NEGATIVE)
    {
        minusHelper(other);
    }
    //if one is negative and other is positive, add the values
    else
    {
        addHelper(other);
    }
    return *this;
}

//private helper function to convert a string passed as argument
void Project1::BigInt::stringIntoVector(const string & str)
{
    //if negative, set flag to 'true' and copy all chars after the '-' char
    if (str.substr(0, 1) == "-")
    {
        //if negative zero, return as a positive zero value
        if (str.substr(1, 2) == "0")
            IS_NEGATIVE = false;
        else
            IS_NEGATIVE = true;

        copy(str.begin() + 1, str.end(), back_inserter(number));
    }
    //if '+' is used, set flag to 'false' and copy all chars after the '+' char
    else if (str.substr(0, 1) == "+")
    {
        IS_NEGATIVE = false;
        copy(str.begin() + 1, str.end(), back_inserter(number));
    }
    else
    {
        IS_NEGATIVE = false;
        copy(str.begin(), str.end(), back_inserter(number));
    }

    //transfrom the string chars to ints
    transform(number.begin(), number.end(), number.begin(), [](int n) { return n - '0'; });

    //check the number loaded into the vector is valid
    numberValidity(number);
}


//helper function to test that the number that was loaded into the 'number' vector is valid
void Project1::BigInt::numberValidity(const vector<int> &num)
{
    //check to ensure at least a single valid int was entered in the string
    if (num.size() == 0)
        throw invalid_argument("String doesn't represent a valid integer.");

    //test the string to see if it's legal first?
    if (any_of(num.begin(), num.end(), [](int num) {return (num < 0 || num > 9); }))
        throw invalid_argument("String doesn't represent a valid integer.");

    //test if all zeroes were passed to vector (mutliple zeroes only - single zero is valid)
    if (num.size() > 1 && all_of(num.begin(), num.end(), [](int num) {return num == 0; }))
        throw invalid_argument("String doesn't represent a valid integer.");

    //test if any items are not a digit
    if (any_of(num.begin(), num.end(), [](int num) {return isdigit(num); }))
        throw invalid_argument("String doesn't represent a valid integer.");
}

//recursive function to finish the subtraction - basically moves the
//1 from the tenth/hundreths/thousands/etc. columns to take care of the negative
//numbers generated from the substraction (and makes them correct subtracted value for operation)
void Project1::BigInt::subFix(int currentIndex)
{
    //test to see if the digit is negative.  If it is, recursively call function until positive
    //digit is found
    if (number[currentIndex] < 0)
    {
        subFix(currentIndex - 1);

        //keep 'borrowing' a '1' from the current index and add to the previous digit up to the index
        //passed
        if (currentIndex != number.size() - 1)
        {
            number[currentIndex] -= 1;
            number[currentIndex + 1] += 10;
        }
    }
    //'borrow' a '1' from the first digit that is larger than zero and add it to the digit before
    else
    {
        number[currentIndex] -= 1;
        number[currentIndex + 1] += 10;
    }
}

//helper function for adding two 'BigInts' together
void Project1::BigInt::addHelper(const BigInt & other)
{
    //add the smaller of the two vectors to the larger. If 'other' vector is larger, store in a temporary
    //vector, add together and assign to this 'BigInt's number vector
    if (number.size() >= other.number.size())
        transform(other.number.rbegin(),
            other.number.rend(),
            number.rbegin(),
            number.rbegin(),
            plus<int>());

    else
    {
        vector<int> tempNumber(other.number);
        transform(number.rbegin(), number.rend(), tempNumber.rbegin(), tempNumber.rbegin(), plus<int>());
        number = tempNumber;
    }

    //loop through the vector to correct any additions that resulted in a value > 10
    for (int i = number.size() - 1; i >= 0; --i)
    {
        //if 10 can divided into the number once, the number is too big. Assign the remainder to the 
        //number at the current index value and add the tenth's place to the following index position
        if (number[i] / 10 == 1)
        {
            number[i] %= 10;

            //if the tenth's value needs to be added to the next value but the loop as the the beggining
            //of the vector, insert the tenth's digit at the beginning of the vector
            if (i == 0)
                number.insert(number.begin(), 1);
            else
                number[i - 1] += 1;
        }
    }
}

//helper function for subtracting two 'BigInts'
void Project1::BigInt::minusHelper(const BigInt & other)
{
    //subtract the smaller of the two vectors from the larger - ignoring the sign (neg/pos).  
    //If the lhs was smaller, store the rhs vector into a temporary vector, subtract and change
    //the sign of the lhs (otherwise, subtract as normal).  A subsequent loop is run after the 
    //subtraction of each digit to correct the result
    if ((number.size() > other.number.size()) ||
        (number.size() == other.number.size()) && (number >= other.number))
    {
        transform(other.number.rbegin(),
            other.number.rend(),
            number.rbegin(),
            number.rbegin(),
            [](int a, int b) { return b - a; });
    }
    else
    {
        vector<int> tempNumber(other.number);
        transform(number.rbegin(),
            number.rend(),
            tempNumber.rbegin(),
            tempNumber.rbegin(),
            [](int a, int b) { return b - a; });

        number = tempNumber;
        IS_NEGATIVE = !IS_NEGATIVE;
    }

    //loop to finish the subtraction through the 'subFix' function
    for (int i = number.size() - 1; i >= 0; --i)
    {
        if (number[i] < 0)
            subFix(i);
    }

    //if all digits in vector were reduced to zero, reset the vector to a single zero
    if (number.size() > 1 && all_of(number.begin(), number.end(), [](int num) {return num == 0; }))
    {
        number.clear();
        number = { 0 };
    }

    //clean up any leading zeros in the vector, provided the vector itself isn't a single zero
    if (number.size() > 1)
    {
        vector<int>::iterator it = find_if(number.begin(),
            number.end(),
            [](int num) {return num != 0; });

        number.erase(number.begin(), it);
    }
}
