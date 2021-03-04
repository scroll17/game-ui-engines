//
// Created by user on 09.02.2021.
//

#include "ProgressBar.h"

ProgressBar::ProgressBar(const sf::Vector2f& size, float step_count):
    Element({ 0, size.y }),
    m_curr_step(0.f),
    m_step_count(step_count),
    m_step_in_pixels(size.x / step_count),
    m_original_width(size.x)
{
    m_rectangle = new sf::RectangleShape();
}

ProgressBar::ProgressBar(float height, float start_width, float end_width, float step_count):
   Element({ start_width, height }),
   m_curr_step(0.f),
   m_step_count(step_count),
   m_step_in_pixels((end_width - start_width) / step_count),
   m_original_width(end_width - start_width)
{
   m_rectangle = new sf::RectangleShape();
}

ProgressBar::~ProgressBar() {
    delete m_rectangle;
}

/// PROTECTED GET
bool ProgressBar::is_end() const {
    return m_step_count == m_curr_step;
}

/// PROTECTED SET
ProgressBar& ProgressBar::build() {
    if(!m_need_build) return (*this);

    m_rectangle->setSize(this->get_size());
    m_rectangle->setFillColor(m_color);

    m_rectangle->setOutlineThickness(m_border_width);
    m_rectangle->setOutlineColor(m_border_color);

    m_rectangle->setPosition(this->get_position());
    m_rectangle->setOrigin(this->get_origin());

    this->turn_on_building();
    return (*this);
}

void ProgressBar::call_callback() {
    if(m_callback_was_called) return;

    m_callback_was_called = true;
    m_callback();
}

/// PUBLIC VOID
void ProgressBar::draw(sf::RenderWindow& window) const {
    window.draw(*m_rectangle);
}

/// PUBLIC SET
ProgressBar& ProgressBar::set_color(const sf::Color& color) {
    m_color = color;

    this->turn_off_building();
    return (*this);
}

bool ProgressBar::next() {
    if(this->is_end()) {
        this->call_callback();
        return false;
    }

    if((m_curr_step + 1) > m_step_count) {
        float diff = m_step_count - m_curr_step;

        m_curr_step += diff;
        this->set_width(this->get_width() + (m_step_in_pixels * diff));
    } else {
        m_curr_step++;
        this->set_width(this->get_width() + m_step_in_pixels);
    }

    this->build();
    return (m_curr_step != m_step_count);
}

bool ProgressBar::next(float step) {
    if(this->is_end()) {
        this->call_callback();
        return false;
    }

    float next_step = m_curr_step + step;
    float diff = (next_step > m_step_count ? m_step_count : next_step) - m_curr_step;

    m_curr_step += diff;
    this->set_width(this->get_width() + (m_step_in_pixels * diff));

    this->build();
    return (m_curr_step != m_step_count);
}

void ProgressBar::ready() {
    if(this->is_end()) return this->call_callback();

    this->next(m_step_count - m_curr_step);
}

void ProgressBar::after_ready(const ProgressBar::t_callback& after_cb) {
    m_callback = after_cb;
}

