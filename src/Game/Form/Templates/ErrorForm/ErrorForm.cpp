//
// Created by user on 09.02.2021.
//

#include "ErrorForm.h"

ErrorForm::ErrorForm() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    this->m_window = new sf::RenderWindow(
      sf::VideoMode(500, 200),
      "Error",
      sf::Style::Default,
       settings
    );
}

ErrorForm::ErrorForm(const sf::ContextSettings& settings) {
    this->m_window = new sf::RenderWindow(
      sf::VideoMode(500, 200),
      "Error",
      sf::Style::Default,
       settings
    );
}

ErrorForm::~ErrorForm() {
    delete m_window;
}

// PROTECTED SET
void ErrorForm::build() {

}

void ErrorForm::draw() {

}

void ErrorForm::pollEvent(const sf::Event &event) {

}