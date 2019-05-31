#include <iostream>
#include <memory>

using namespace std;

/*
 * AAL
 * Тест на знание особенностей умных указателей. Содержит не критичные ошибки, позволяющие компилировать код при конкретизации шаблона целочисленным типом
 *  Ошибки:
 * 1) Шаблон вне зависимости от конкретизации возвращает testType. При этом retVal является объектом типа, которым конкретизирован шаблон.
 * Следовательно данный шаблон бутер компилироваться только при конкретизации целочисленным типом т.к. return retVal; пытается вернёться как testType == std::uintmax_t.
 * 2) T retVal; Не инициализирован, а следовательно, если функция foo во всех случаях вернёт false, на выходе вернётся мусор.
 * 3) Не корректное освобождение памяти в строчке delete testX. Переменной testX выделяется память под массив, а следовательно и удалять нужно как массив delete [] testX.
 */

bool foo(std::uint32_t inValue)
{
    bool Res = rand() % 2;
    return Res;
}
//--
std::uint32_t testFoo (size_t testSize) {
    std::uint32_t retVal = 0;
    std::shared_ptr <std::uint32_t []> ptrTest (new std::uint32_t [testSize]);

    // Заполняем массив данными

    for (size_t i = 0; i < testSize; i++){
        std::uint32_t *testX = ptrTest.get();
        if (foo (testX[i])) retVal = testX [i];
    }

    return retVal;
}

int main()
{
    std::uint32_t Res = testFoo(10);
    std::cout << Res << std::endl;

    getchar();

    return 0;
}
