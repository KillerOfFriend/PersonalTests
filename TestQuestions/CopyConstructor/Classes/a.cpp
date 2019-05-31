#include "a.h"

#include <iostream>

//---
A::A() {}
//---
A::A(const A &inOther)
{
    this->fIntNum = inOther.fIntNum;
    this->fDoubNum = inOther.fDoubNum;
}
//---
A::~A()
{
    fIntNum = 0;
    fDoubNum = 0.0;
}
//---
void A::setIntNum(const int inIntNum)
{ fIntNum = inIntNum; }
//---
void A::setDoubNum(const double inDoubNum)
{ fDoubNum = inDoubNum; }
//---
void A::printContent() const
{
    std::cout << fIntNum << std::endl << fDoubNum << std::endl;
}
//---
