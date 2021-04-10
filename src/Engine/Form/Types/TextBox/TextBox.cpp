//
// Created by user on 07.04.2021.
//

#include "TextBox.h"

// PUBLIC SET
void TextBox::input(TextBox& text_box, const sf::RenderWindow& window, const sf::Event& event) {
//    // Установка Фокуса Input
//    bool select(Vector2i _mouse)
//    {
//        if((_mouse.x > x && _mouse.x < x + width) && (_mouse.y > y && _mouse.y < y + height)){ 	//Если нажат клаиша над объектом Input...
//            focus = true;																	   	// Фокус true
//            text += "|";																		// В конец строки добаляем | (что бы понимать что input в фокусе)
//        } else {																				//...Иначе если нажатие произошло не над объектом, то...
//            if(text.size() > 0){																// проверка последнего символа(иначе вылетает)
//                if(text[text.size()-1] == 124){													// если символ | ...
//                    text.erase(text.size()-1);														// ... то удаляем его
//                }
//            }
//            focus = false;																		// устанавливаем фокус false
//        }
//        return focus;
//    }

//    virtual void event ( const sf::Event & event )
//    {
//        if (event.type == sf::Event::TextEntered)
//        {
//            //Обработка ввода
//            m_textChanged = true ;
//            switch ( event.text.unicode )
//            {
//                case 0xD: //Return
//                    m_newText += L'\n' ;
//                    break ;
//                case 0x8://Backspace
//                    if ( !m_newText.isEmpty() )
//                        m_newText.erase(m_newText.getSize()-1) ;
//                    break ;
//                default :
//                {
//                    m_newText += static_cast<wchar_t>(event.text.unicode) ;
//                }
//            }
//        }
//    }

    if(event.type == sf::Event::TextEntered) {
        text_box.m_text_changed = true;
    }
}

TextBox& TextBox::set_focus() {
    ActionObject::set_focus();

    m_text.add_text("|");

    return (*this);
}

TextBox& TextBox::delete_focus() {
    ActionObject::delete_focus();

    m_text.remove_chars(0, 1);

    return (*this);
}
