#include <iostream>

using namespace std;

int Hello()
{ return printf("Hello"); }

int World()
{ return printf("World !"); }

int main()
{
    int a = Hello() + World();

    getchar();

    return 0;
}
