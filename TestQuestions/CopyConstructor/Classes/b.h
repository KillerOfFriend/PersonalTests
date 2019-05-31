#ifndef B_H
#define B_H

#include <string>

#include "Classes/a.h"

class B : public A
{
public:
    B();
    B(const B &inOther);
    ~B();

    void setUChar(const unsigned char inUChar);
    void setStr(const std::string &inStr);

    virtual void printContent() const;

protected:
    unsigned char fUChar = 'A';
    std::string fStr = "String";
};

#endif // B_H
