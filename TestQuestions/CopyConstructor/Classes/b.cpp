#include "b.h"

#include <iostream>

//---
B::B() : A() {}
//---
B::B(const B &inOther)
{
    this->fUChar = inOther.fUChar;
    this->fStr = inOther.fStr;
}
//---
B::~B()
{
    fUChar = 0;
    fStr.clear();
}
//---
void B::setUChar(const unsigned char inUChar)
{ fUChar = inUChar; }
//---
void B::setStr(const std::string &inStr)
{ fStr = inStr; }
void B::printContent() const
{
    A::printContent();
    std::cout << fUChar << std::endl << fStr << std::endl;
}
//---
