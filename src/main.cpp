#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "iostream"

using json = nlohmann::json;
using namespace std;

int main()
{

    json j;

    j["test"] = 5;

    cout << j.dump();

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