#include <iostream>
#include "Classes/b.h"

/*
 * ASG
 * Тест проверяет понимание ООП и работу с конструктором копирования
 * ! У класса B в констукторе копирования требуется добавить вызов конструктора копирования предка : A(inOther)!
 */

int main()
{
    std::string Str("Hellow World!");

    B First = B();

    First.setIntNum(10);
    First.setDoubNum(15.5);

    First.printContent();

    //---

    B Second = B(First);
    Second.setUChar('Q');
    Second.setStr(Str);

    Second.printContent(); // <<- Какой будет вывод\Почему?

    //---

    getchar();

    return 0;
}
