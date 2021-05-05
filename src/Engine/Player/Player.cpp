//
// Created by user on 08.02.2021.
//

#include "Player.h"

using namespace engine;

Player::Player(const GameMap& game_map): m_game_map(game_map) {
}

/// PUBLIC SET
void Player::set_position(float x, float y) {
    m_position = sf::Vector2f(x, y);

    const auto& paddings = m_game_map.get_paddings();
    m_sprite.setPosition({
      m_position.x - m_game_map.get_offset_x() + paddings.left,
      m_position.y - m_game_map.get_offset_y() + paddings.top
    });
}

void Player::move_input(Player& player) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player.move(GameMap::Direction::Left);
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player.move(GameMap::Direction::Right);
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player.move(GameMap::Direction::Top);
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player.move(GameMap::Direction::Bottom);
    }
}


void Player::collision(Player::Axis axis, float prev_pos) {
    /**
     *  using "getGlobalBounds" because we use "sprite.scale"
     * */
    const auto& bounds = m_sprite.getGlobalBounds();

    const auto& collision_blocks_y = m_game_map.get_collision_blocks_y(m_position, bounds);
    const auto& collision_blocks_x = m_game_map.get_collision_blocks_x(m_position, bounds);

    for (size_t i = collision_blocks_y.get_start(); i < collision_blocks_y.get_end(); i++) {
        for (size_t j = collision_blocks_x.get_start(); j < collision_blocks_x.get_end(); j++) {
            auto cell_at = m_game_map.at_tile(i, j);

            if(!m_game_map.is_collision_cell(cell_at)) continue;

            if(axis == Axis::X) {
                if(m_dx > 0) m_position.x = prev_pos;
                if(m_dx < 0) m_position.x = prev_pos;
            }

            if(axis == Axis::Y) {
                if(m_dy > 0) m_position.y = prev_pos;
                if(m_dy < 0) m_position.y = prev_pos;
            }
        }
    }
}

void Player::hitting_in_texture() {
    /**
     *    Explanation:
     *      when we move to the left / right (near the borders)
     *      and then we start to move forward,
     *      then we change the height of the player and get into the texture
     *
     *    Need execute if the width / height of the sprite changes
     * */

    const auto& bounds = m_sprite.getGlobalBounds();

    const auto& collision_blocks_y = m_game_map.get_collision_blocks_y(m_position, bounds);
    const auto& collision_blocks_x = m_game_map.get_collision_blocks_x(m_position, bounds);

    for (size_t i = collision_blocks_y.get_start(); i < collision_blocks_y.get_end(); ++i) {
        for (size_t j = collision_blocks_x.get_start(); j < collision_blocks_x.get_end(); ++j) {
            auto cell_at = m_game_map.at_tile(i, j);

            if(!m_game_map.is_collision_cell(cell_at)) continue;

            float x_block_position = j * m_game_map.get_block_size();
            float y_block_position = i * m_game_map.get_block_size();

            bool is_right = x_block_position >= m_position.x;
            bool is_left = !is_right;

            bool is_top = y_block_position <= m_position.y;
            bool is_bottom = !is_top;

            bool is_X = is_right || is_left;
            bool is_Y = is_top || is_bottom;

            float hitting_x;
            float hitting_y;

            if(is_X) {
                if(is_right)
                    hitting_x = m_game_map.get_hitting_in_texture(Direction::Right, m_position, bounds, j);
                else
                    hitting_x = m_game_map.get_hitting_in_texture(Direction::Left, m_position, bounds, j);
            }

            if(is_Y) {
                if(is_top)
                    hitting_y = m_game_map.get_hitting_in_texture(Direction::Top, m_position, bounds, i);
                else
                    hitting_y = m_game_map.get_hitting_in_texture(Direction::Bottom, m_position, bounds, i);
            }

            if(is_X && is_Y) {
                float result;
                if(hitting_x < 0 && hitting_y < 0) {
                    result = std::max(hitting_x, hitting_y);
                } else {
                    float fabs_hitting_x = fabs(hitting_x);
                    float min = std::min(fabs_hitting_x, fabs(hitting_y));

                    if(min == fabs_hitting_x)
                        result = hitting_x;
                    else
                        result = hitting_y;
                }

                if(result == hitting_x)
                    hitting_y = 0;
                else
                    hitting_x = 0;
            }

            if(hitting_x != 0) {
                if(hitting_x > 0) m_position.x += (hitting_x + 5);
                else m_position.x += (hitting_x - 5);
            }

            if(hitting_y != 0) {
                if(hitting_y > 0) m_position.y += (hitting_y + 2);
                else m_position.y += (hitting_y - 2);
            }
        }
    }
}

void Player::around_blocks(char cell, const t_around_block_cb& cb) {
    /** AFTER PLAYER UPDATE */

    const auto& around_blocks = m_game_map.get_around_blocks(m_position, m_sprite.getGlobalBounds());
    for(auto& around_block: around_blocks) {
        auto& block = around_block.second;

        auto x = block.first;
        auto y = block.second;

        switch (around_block.first) {
            case Direction::Top:
            case Direction::Bottom: {
                const auto& map_width = m_game_map.get_width();

                size_t x_l = (x - 1) > 0 ? (x - 1) : 0;
                size_t x_r = (x + 1) > map_width ? map_width : (x + 1);

                char32_t cell_c = m_game_map.at_tile(y, x);
                char32_t cell_l = m_game_map.at_tile(y, x_l);
                char32_t cell_r = m_game_map.at_tile(y, x_r);

                if(cell_c == cell) cb(make_pair(x, y));
                else if(cell_l == cell) cb(make_pair(x_l, y));
                else if(cell_r == cell) cb(make_pair(x_r, y));

                break;
            }
            case Direction::Right:
            case Direction::Left: {
                const auto& map_height = m_game_map.get_height();

                size_t y_t = (y - 1) > 0 ? (y - 1) : 0;
                size_t y_b = (y + 1) > map_height ? map_height : (y + 1);

                char32_t cell_c = m_game_map.at_tile(y, x);
                char32_t cell_t = m_game_map.at_tile(y_t, x);
                char32_t cell_b = m_game_map.at_tile(y_b, x);

                if(cell_c == cell) cb(make_pair(x, y));
                else if(cell_t == cell) cb(make_pair(x, y_t));
                else if(cell_b == cell) cb(make_pair(x, y_b));

                break;
            }
        }
    }
}

void Player::update(float elapsed_time) {
    if(m_dx == 0 && m_dy == 0) return;

    Axis current_axis = m_dx != 0 ? Axis::X : Axis::Y;

    this->calculate_boost(current_axis, elapsed_time);

    auto prev_x_pos = m_position.x;
    auto prev_y_pos = m_position.y;

    m_position.x += m_dx * elapsed_time;
    this->collision(Axis::X, prev_x_pos);

    m_position.y += m_dy * elapsed_time;
    this->collision(Axis::Y, prev_y_pos);

    this->calculate_steps_count(prev_x_pos, prev_y_pos);
    this->calculate_animation(current_axis, elapsed_time);

    const auto& paddings = m_game_map.get_paddings();
    m_sprite.setPosition({
       m_position.x - m_game_map.get_offset_x() + paddings.left,
       m_position.y - m_game_map.get_offset_y() + paddings.top
    });

    m_prev_axis = current_axis;
    this->stop_all();
}

void Player::run(bool condition) {
    m_run = condition;
}

void Player::move(Player::Direction dir) {
    switch (dir) {
        case GameMap::Right: {
            m_dx = m_speed;
            break;
        }
        case GameMap::Left: {
            m_dx = -m_speed;
            break;
        }
        case GameMap::Top: {
            m_dy = -m_speed;
            break;
        }
        case GameMap::Bottom: {
            m_dy = m_speed;
            break;
        }
    }
}

void Player::stop(Player::Direction dir) {
    switch (dir) {
        case GameMap::Left:
        case GameMap::Right: {
            m_dx = 0;
            break;
        }
        case GameMap::Bottom:
        case GameMap::Top: {
            m_dy = 0;
            break;
        }
    }
}

void Player::stop_all() {
    m_dx = m_dy = 0;
}

/// PUBLIC GET
bool Player::is_run() const {
    return m_run;
}

float Player::get_steps_count() const {
    return m_steps_count / m_game_map.get_block_size();
}

sf::FloatRect Player::get_bounds() const {
    return m_sprite.getGlobalBounds();
}

const sf::Vector2f& Player::get_position() const {
    return m_position;
}

const sf::Sprite& Player::get_sprite() const {
    return m_sprite;
}

/// PRIVATE SET
void Player::calculate_boost(Axis axis, float elapsed_time) {
    if(m_run) {
        float boost = m_boost * elapsed_time; /// a * t; a - ускорение, t - время

        switch (axis) {
            case Axis::X: {
                if(m_dx > 0) m_dx = m_dx + boost;
                if(m_dx < 0) m_dx = m_dx - boost;

                break;
            }
            case Axis::Y: {
                if(m_dy > 0) m_dy = m_dy + boost;
                if(m_dy < 0) m_dy = m_dy - boost;

                break;
            }
            default: {
                return;
            }
        }
    }
}

void Player::calculate_animation(Axis axis, float elapsed_time) {
    float boost = 0;
    if(m_run) {
        boost = m_animation_boost * elapsed_time; /// a * t; a - ускорение, t - время
    }

    float animation_road = (m_animation_change_rate * elapsed_time) + boost;

    float& frame = axis == Axis::X
      ? m_dx_current_frame
      : m_dy_current_frame;

    frame += animation_road;
    if(frame > m_max_animation_frames) frame -= m_max_animation_frames;

    if(axis == Axis::X) {
        /**
          *      обычное отображение - IntRect(10 + ((int)dx_current_frame * 100), 215, 100, 70)
          *      зеркальное - IntRect(10 + ((int)dx_current_frame * 100) + 100, 215, -100, 70)
          * */

        if(m_dx > 0) {
            m_rect_x.left = 11 + ((int)m_dx_current_frame * 100);
            m_rect_x.width = 95;
        }
        if(m_dx < 0) {
            m_rect_x.left = 11 + ((int)m_dx_current_frame * 100) + 100;
            m_rect_x.width = -95;
        }

        m_sprite.setTextureRect(m_rect_x);
        this->hitting_in_texture();
    } else {
        m_rect_y.left = 20 + (int(m_dy_current_frame) * 100);

        if(m_dy > 0) {
            m_rect_y.top = 5;
            m_rect_y.height = 95;
        }
        if(m_dy < 0) {
            m_rect_y.top = 5 + 95;
            m_rect_y.height = -95;
        }

        m_sprite.setTextureRect(m_rect_y);
        this->hitting_in_texture();
    }
}

void Player::calculate_steps_count(float prev_pos_x, float prev_pos_y) {
    m_steps_count += std::fabs(m_position.x - prev_pos_x) + std::fabs(m_position.y - prev_pos_y);
}