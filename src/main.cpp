#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "iostream"
#include "pugixml.hpp"

#include "./Game/Game.h"
#include "./Exception/Exception.h"
#include "./File/File.h"
#include "./File/FileReader/FileReader.h"
#include "./File/FileWriter/FileWriter.h"

#include "./utils/array.h"

#include "./Game/Interface/Types/Element/Element.h"

#include "./Game/Interface/Types/Button/Button.h"
#include "./Game/Interface/Types/Text/Text.h"

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

    // Объект, который, собственно, является главным окном приложения
    RenderWindow window(VideoMode(500, 200), "SFML Works!");

    ::Text text { "чтобы вызывался соответствующий деструктор" };
    text
      .correct_position(true)
      .border_with_position(true)
      .set_window_size(window.getSize())
      .to_center(Element::XY)
      .move(Element::Y, -20)
      .build();

    Button button { {100, 50}, "Click" };
    button
        .correct_position(true)
        .border_with_position(true)
        .set_window_size(window.getSize())
        .set_border_width(2)
        .to_center(Element::X)
        .set_after(Element::Y, text)
        .move(Element::Y, 20)
        .build();

    button.on_click([](Button& button) {
        cout << "button click" << endl;

        button.set_text_color(sf::Color::Green);
        button.build();
    }, [&window](Button& button) {
        cout << "after button click" << endl;

        button.set_text_color(sf::Color::Black);
        button.build();

//        window.close();
    });

    button.on_hover([](Button& button){
        cout << "button hover" << endl;

        button.set_border_width(2);
        button.build();
    }, [](Button& button) {
        cout << "after button hover" << endl;

        button.set_border_width(0);
        button.build();
    });

//    Element el { {2, 50} };
//    el
//        .border_with_position(true)
//        .correct_position(true)
//        .set_window_size(window.getSize())
//        .to_center(Element::X)
//        //.set_border_width(2)
//        .set_border_color(sf::Color::Black);

//    Element el2 { {50, 50} };
//    el2
//      .border_with_position(true)
//      .correct_position(true)
//      .set_window_size(window.getSize())
//      .set_border_width(2)
//      .to_center()
//      .to(Element::Right)
//      .to(Element::Down)
//      .set_border_color(sf::Color::Blue);

    auto prev_pos(sf::Mouse::getPosition(window));
    auto curr_pos(sf::Mouse::getPosition(window));

    // Главный цикл приложения: выполняется, пока открыто окно
    while (window.isOpen())
    {
        // Обрабатываем очередь событий в цикле
        Event event;
        while (window.pollEvent(event))
        {

            if(event.type == sf::Event::MouseMoved) {
                prev_pos = curr_pos;
                curr_pos = sf::Mouse::getPosition(window);
            }

            Button::perform_button_actions(button, window, event, prev_pos);

            //button.perform_button_actions(window, event, prev_pos);

            // Пользователь нажал на «крестик» и хочет закрыть окно?
            if (event.type == Event::Closed)
                window.close(); // тогда закрываем его
        }
        // Установка цвета фона
        window.clear(Color(250, 220, 100, 0));


        text.draw(window);
        button.draw(window);

        //window.draw(m_text);

        // Отрисовка окна
        window.display();
    }

//    button.remove_click(0);

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