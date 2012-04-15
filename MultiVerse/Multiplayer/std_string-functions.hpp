#ifndef __STD_STRING_FUNCTIONS_HPP__
#define __STD_STRING_FUNCTIONS_HPP__

#include <cstdlib>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

string Bool(bool b)
{
  return b ? "true" : "false";
}

template <class Number>
string ToString(const Number & n)
{
    stringstream stream1;
    stream1 << n;
    return stream1.str();
}

int StringToInt(string s)
{
    int Number;
    Number = atoi(s.c_str());

    return Number;
}

float StringToFloat(string s)
{
    float Number;
    Number = atof(s.c_str());

    return Number;
}

enum {TO_UPPER,TO_LOWER};
string CaseShift(const string & Str,int Case)
{
    string Data = Str;

    if(Case == TO_UPPER)
    transform(Data.begin(),Data.end(),Data.begin(),::toupper);
    else if(Case == TO_LOWER)
    transform(Data.begin(),Data.end(),Data.begin(),::tolower);

    return Data;
}

#endif
