#include <iostream>

#include "Vector.h"
#include "List.h"
#include "Redirect.h"


template<class T>
T fillObj(size_t m) {
    T temp;
    for(size_t i = 0; i < m; ++i) {
        temp.push_back(i);
    }
    return temp;
}

template<class T>
void test(std::string_view className) {

    const int n = 4; // Индекс элемента который выводим
    const int m = 10; // Количество элементов в контейнере

    std::cout << "1. Создаём объект класса " << className << "\n";
    T obj;

    std::cout << "2. Заполняем объект класса " << className << " числами от 0 до 9\n";
    obj = fillObj<T>(m);

    std::cout << "3. Выводим на экран содержимое объекта класса " << className << "\n";
    print(obj);

    std::cout << "4. Выводим на экран размер объекта класса " << className << "\n";
    std::cout << "obj.size() = " << obj.size() << "\n";

    std::cout << "5. Удаляем из объекта класса " << className << " \n";
    obj.erase(obj.begin() + 2);
    obj.erase(obj.begin() + 3);
    obj.erase(obj.begin() + 4);

    std::cout << "6. Выводим на экран содержимое объекта класса " << className << "\n";
    print(obj);

    std::cout << "7. Добавляем число 10 в начало объекта класса " << className << "\n";
    obj.insert(obj.begin(), 10);

    std::cout << "8. Выводим на экран содержимое объекта класса " << className << "\n";
    print(obj);

    std::cout << "9. Добавляем число 20 в середину объекта класса " << className << "\n";
    obj.insert((obj.begin() + (obj.size() / 2)), 20);

    std::cout << "10. Выводим на экран содержимое объекта класса " << className << "\n";
    print(obj);

    std::cout << "11. Добавляем число 30 в конец объекта класса " << className << "\n";
    obj.insert(obj.end(), 30);

    std::cout << "12. Выводим на экран содержимое объекта класса " << className << "\n";
    print(obj);

    T obj1(obj);
    T obj2(obj1);
    obj2 = obj;
    std::cout << className << " obj[" << n << "] = " << obj[n] << std::endl; // Проверка оператора индекса
    std::cout << std::endl;
}

int main() {
    test<Vector<int>>("Vector<int>");
    test<List<int>>("List<int>");
    test<Redirect<int>>("Redirect<int>");
    return 0;
}