#include <iostream>

/*
 * AAL
 * Тест на знание шаблонов. Содержит не критичные ошибки, позволяющие компилировать код при конкретизации шаблона целочисленным типом
 *  Ошибки:
 * 1) Шаблон вне зависимости от конкретизации возвращает testType. При этом retVal является объектом типа, которым конкретизирован шаблон.
 * Следовательно данный шаблон бутер компилироваться только при конкретизации целочисленным типом т.к. return retVal; пытается вернёться как testType == std::uintmax_t.
 * 2) T retVal; Не инициализирован, а следовательно, если функция foo во всех случаях вернёт false, на выходе вернётся мусор.
 * 3) Не корректное освобождение памяти в строчке delete testX. Переменной testX выделяется память под массив, а следовательно и удалять нужно как массив delete [] testX.
 */

using namespace std;

typedef std::uintmax_t testType;

template <typename T>
bool foo(T inValue)
{
    bool Result = rand() % 2;
    return Result;
}

template <typename T>
testType testFoo(size_t testSize) {
    T retVal;
    T* testX = new T [testSize];

    //...
    // Заполняем массив данными
    //...

    // Обходим заполенный массив
    for (size_t i = 0; i < testSize; i++) {
        if (foo<T> (testX[i])) retVal = testX[i];
    }

    delete testX;
    return retVal;
}

int main()
{
    auto Res = testFoo<int>(10);
    //auto Res = testFoo<std::string>(20);
    std::cout << Res << std::endl;

    getchar();

    return 0;
}
