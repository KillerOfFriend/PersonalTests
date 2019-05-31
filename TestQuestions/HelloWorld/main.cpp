#include <iostream>

/*
 * ASG
 * Тест проверяет знание стандарта
 * В данном случаи стандарт не гарантирует какая часть выражения Hello() + World() будет выполнена первой.
 * Стандарт перекладывает решение о реализации последовательности вычислений на компилятор
 * Следовательно в некоторых компиляторах можно встретить результат "World !Hello"
 */

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
