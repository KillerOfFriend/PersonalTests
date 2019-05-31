#ifndef A_H
#define A_H

class A
{
public:
    A();
    A(const A &inOther);
    ~A();

    void setIntNum(const int inIntNum);
    void setDoubNum(const double inDoubNum);

    virtual void printContent() const;

protected:
    int fIntNum = 0;
    double fDoubNum = 0.0;
};

#endif // A_H
