#include <iostream>

/*
 * ASG
 * Тест проверяет знание механизмов автоматического приведения типов и устройство иеханизма исключений
 * 1) т.к. входные данные типа int то в выражении a / (b / a) произойдёт автоматическое приведение к int и дробная часть будет утрачена (Потеря точности)
 * 2) Выбрасываемое исключение типа throw std::exception("Деление на ноль!"); не будет поймано catch (std::runtime_error &e)
 */

using namespace std;

int main()
{
    int a = 2;
    int b = 3;

    try
    {
        double c = 0.0;

        if (!a || !(b / a))
            throw std::exception("Деление на ноль!");

        c = a / (b / a);
        std::cout << c << std::endl;
    }
    catch (std::runtime_error &e)
    {
        std::cout << "Произошла ошибка:" << e.what() << std::endl;
    }

    getchar();

    return 0;
}
