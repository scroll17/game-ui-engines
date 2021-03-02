#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "iostream"
#include "pugixml.hpp"

#include "./Exception/Exception.h"
#include "./File/File.h"
#include "./File/FileReader/FileReader.h"
#include "./File/FileWriter/FileWriter.h"

#include "./utils/array.h"

using json = nlohmann::json;
using namespace std;

// TODO _ Расширение функционала родительских методов
//      void identify() {
//        Parent::identify(); // сначала выполняется вызов Parent::identify()
//        std::cout << "I am a Child!\n"; // затем уже вывод этого текста
//      }

// TODO _ Сокрытие методов родительского класса
//  class Child: public Parent {
//    private:
//	    using Parent::m_value;
//    public:
//      Child(int value): Parent(value) {}
//
//      // Parent::printValue является protected, поэтому доступ к нему не является открытым для всех объектов.
//      // Но мы можем это исправить с помощью "using-объявления"
//      using Parent::printValue; // обратите внимание, нет никаких скобок TODO _
//
//      int getValue() = delete; // делаем этот метод недоступным   TODO _
//  };

// TODO _ При работе с наследованием ваши деструкторы всегда должны быть виртуальными
//        virtual ~Child() {}

// TODO _ Игнорирование виртуальных функций
//      Parent &parent = child;
//      std::cout << parent.Parent::getName() << std::endl;


// TODO _ Абстрактные функции и классы
//      virtual int getValue() = 0; // чистая виртуальная функция
//      Таким образом, мы сообщаем компилятору: «Реализацией этой функции займутся дочерние классы».

// TODO _ Интерфейс — это класс, который не имеет переменных-членов и все методы которого являются чистыми виртуальными функциями
//  class IErrorLog {
//      public:
//          virtual bool openLog(const char *filename) = 0;
//          virtual bool closeLog() = 0;
//          virtual bool writeError(const char *errorMessage) = 0;
//          virtual ~IErrorLog() {}; // создаем виртуальный деструктор, чтобы вызывался соответствующий деструктор дочернего класса в случае, если удалим указатель на IErrorLog
//  };

using namespace sf;

int main()
{
    try {
        throw Exception(Exception::FileRead, "Testing standart error");
    } catch (Exception& ex) {
        ex.draw_error();
    }

    return 0;
}

int main_original()
{


//        try {
//            FileWriter f_w("./data/json/level_1.json");
//
//            f_w.open_file();
//            f_w.write("test");
//            f_w.close_file();
//
//            //cout << "name => " << f_r.to_json()["name"] << endl;
//        } catch (Exception& ex) {
//            ex.draw_error();
//        } catch (std::exception &ex) {
//            cerr << ex.what();
//        }

//    auto *button = new InterfaceForm::Button{ 50, 50, "text" };
//    // reinterpret_cast<void*>(boo)
//    button->onClick = []() {
//       cout << "Onclick" << endl;
//    };
//    button->onClick();
//    delete button;

//      TODO _exception strategy
//    try {
//       throw Exception(Exception::FileRead, "Testing standart error");
//      // ...
//    } catch (std::exception &exception) {
//        std::cerr << "Standard exception: " << exception.what() << '\n';
//    } catch (...) {
//
//    }


//    int array[4] = { 5, 8, 6, 4 };
//    cout << sizeof(array) << endl;
//
//    int value = 5;
//    int *ptr = new int(value);
//
//    const int *const ptr2 = &value;
//
//    test(&value);
//
//    void *ptr3;
//    ptr3 = &value;
//
//    cout << " b => " << *static_cast<int*>(ptr3);

    // assert(1 == 0);

    //Employee &l_value (Employee);

//    File f("../../././data/json/level_2.json");
//    cout << f.get_file_path() << endl;

//    cout << f.check_file_exists();

//    try {
//        f.read();
//
//        json j2;
//        f.iterate([&j2](char* chunk)  {
//            j2 << *chunk;
//            return 0;
//        });
//
//        json j = json::parse(f.get_data());
//
//
//        cout << j["name"] << endl;
//    } catch (Exception &ex) {
//        ex.out_to(cout);
//    }


//    Game g;
//
//    g.start();

//    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }

    return 0;
}