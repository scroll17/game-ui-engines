//
// Created by user on 27.04.2021.
//

#include "GameMap.h"

using namespace engine;

GameMap::GameMap(size_t block_size): m_block_size(block_size) {

}

/// PUBLIC SET
void GameMap::calculate_offset(const sf::Vector2f& position) {
    this->calculate_offset_x(position);
    this->calculate_offset_y(position);
}

void GameMap::calculate_offset_x(const sf::Vector2f& position) {
    auto start_x = m_window_size.x / 2.f;
    auto end_x = float(m_tile_width * m_block_size) - start_x;
    if(position.x > start_x && position.x < end_x) {
        m_offset_x = position.x - start_x;
    }
}

void GameMap::calculate_offset_y(const sf::Vector2f& position) {
    auto start_y = m_window_size.y / 2.f;
    auto end_y = float(m_tile_height * m_block_size) - start_y;
    if(position.y > start_y && position.y < end_y) {
        m_offset_y = position.y - start_y;
    }
}

void GameMap::draw(sf::RenderWindow& window, pair<size_t, size_t>* player_current_block) {
    auto block_size = this->get_block_size();
    const auto& paddings = this->get_paddings();

    size_t start_x = 0;
    size_t end_x = this->get_width();

    size_t start_y = 0;
    size_t end_y = this->get_height();

    if(player_current_block != nullptr) {
        if(m_max_block_vising_x != 0) {
            size_t player_x = player_current_block->first;

            start_x = std::max(0, int(player_x - m_max_block_vising_x / 2));
            end_x = std::min(this->get_width(), player_x + m_max_block_vising_x / 2);
        }

        if(m_max_block_vising_y != 0) {
            size_t player_y = player_current_block->second;

            start_y = std::max(0, int(player_y - m_max_block_vising_y / 2));
            end_y = std::min(this->get_height(), player_y + m_max_block_vising_y / 2);
        }
    }

    float vising_offset_x = start_x * block_size;
    float vising_offset_y = start_y * block_size;

    for(size_t i = start_y; i < end_y; i++) {
        for(size_t j = start_x; j < end_x; j++) {
            auto cell = this->at_tile(i, j);

            auto draw_el = this->get_draw_element(cell);
            if(draw_el != nullptr) {
                const auto& el = draw_el->first;
                const auto& on_draw_el_cb = draw_el->second;

                on_draw_el_cb(el);
                el->setPosition(
                  (j * block_size) + paddings.left - vising_offset_x - this->get_offset_x(),
                  (i * block_size) + paddings.top - vising_offset_y - this->get_offset_y()
                );

                if(!m_on_draw_callbacks.empty()) {
                  const size_t size = m_on_draw_callbacks.size();
                  for(size_t pos = 0; pos < size; pos += 2) {
                     auto& callback = m_on_draw_callbacks.at(pos);
                     callback(i, j, cell, *this);
                  }
                }

                window.draw((*el));
            }

            if(!m_on_draw_callbacks.empty()) {
                const size_t size = m_on_draw_callbacks.size();
                for(size_t pos = 1; pos < size; pos += 2) {
                    auto& callback = m_on_draw_callbacks.at(pos);
                    callback(i, j, cell, *this);
                }
            }
        }
    }
}

size_t GameMap::on_draw(const GameMap::t_draw_cb& cb, const GameMap::t_draw_cb& after_cb) {
    m_on_draw_callbacks.push_back(cb);
    size_t pos = m_on_draw_callbacks.size() - 1;

    m_on_draw_callbacks.push_back(after_cb);

    return pos;
}

bool GameMap::remove_on_draw(size_t pos) {
    if(pos % 2 != 0) throw std::runtime_error("Invalid argument");

    if(pos > (m_on_draw_callbacks.size() - 2)) {
        throw data_types::Exception(data_types::Exception::NonExistentPosition);
    }

    auto it = std::begin(m_on_draw_callbacks);
    m_on_draw_callbacks.erase(it + pos);               // on callback
    m_on_draw_callbacks.erase(it + pos + 1);   // after callback

    return false;
}

bool GameMap::load_tile(const string *tile, size_t width, size_t height) {
    m_tile = tile;

    m_tile_width = width;
    m_tile_height = height;

    return true;
}

bool GameMap::register_collision_cell(char cell) {
    m_collision_cells.insert(cell);
    return true;
}

bool GameMap::register_collision_cells(string str) {
    auto it = std::begin(str);
    while (it != std::end(str)) {
        m_collision_cells.insert(*it);
        it++;
    }

    return true;
}

bool GameMap::register_draw_element(char cell, sf::RectangleShape& rec, const t_draw_el_cb& cb) {
    m_cell_draw_el[cell] = make_pair(&rec, cb);
    return true;
}

bool GameMap::set_windows_size(const sf::Vector2u& size) {
    m_window_size = size;
    return true;
}

bool GameMap::set_paddings(const Paddings& paddings) {
    m_paddings = paddings;
    return true;
}


bool GameMap::set_max_block_vising_x(int blocks) {
    m_max_block_vising_x = blocks;
    return true;
}

bool GameMap::set_max_block_vising_y(int blocks) {
    m_max_block_vising_y = blocks;
    return true;
}

bool GameMap::set_offset_x(float x) {
    m_offset_x = x;
    return true;
}

bool GameMap::set_offset_y(float y) {
    m_offset_y = y;
    return true;
}

/// PUBLIC GET
char GameMap::at_tile(size_t y, size_t x) const {
    if(m_tile == nullptr) throw std::runtime_error("before need load tile");

    if(y > m_tile_height) {
        throw std::runtime_error("invalid Y block");
    }
    if(x > m_tile_width) {
        throw std::runtime_error("invalid X block");
    }

    return m_tile[y][x];
}

Range GameMap::get_collision_blocks_x(const sf::Vector2f& position, const sf::FloatRect& bounds) const {
    /**
     *    left_x_block:
     *      the block on which the player "left" border is located (by axis X);
     *      - using floor() to round down;
     *    right_x_block:
     *      the block on which the "right" player is located (by axis X);
     *      - using ceil() to round up;
     * */
    int left_x_block = floor(position.x / m_block_size);
    int right_x_block = ceil((position.x + bounds.width) / m_block_size);

    return Range(left_x_block, right_x_block);
}

Range GameMap::get_collision_blocks_y(const sf::Vector2f& position, const sf::FloatRect& bounds) const {
    /**
     *    top_y_block:
     *      the block on which the player "upper" border is located (by axis Y);
     *    bottom_y_block:
     *      the block on which the "lower" player is located (by axis Y);
     * */
    int top_y_block = floor(position.y / m_block_size);
    int bottom_y_block = ceil((position.y + bounds.height) / m_block_size);

    return Range(top_y_block, bottom_y_block);
}

float GameMap::get_hitting_in_texture(
   const GameMap::Direction direction,
   const sf::Vector2f& position,
   const sf::FloatRect& bounds,
   size_t block
) const {
    // X - start of contact block
    // Y - start of contact block
    float block_position = block * m_block_size;

    switch (direction) {
        case Direction::Right: {
            float x = position.x + bounds.width;           // position with width
            float y = block_position;                      // start of contact block by X

            float result = y - x;
            if(result > 0) return 0.f;                     // must be negative because block after player position

            return result;
        }
        case Direction::Left: {
            float x = position.x;                     // player position (start of sprite)
            float y = block_position + m_block_size;  // end of contact block by X

            float result = y - x;
            if(result < 0) return 0.f;

            return result;
        }
        case Direction::Top: {
            float x = position.y;                      // player position (start of sprite)
            float y = block_position + m_block_size;   // end of contact block by Y

            float result = y - x;
            if(result < 0) return 0.f;

            return result;
        }
        case Direction::Bottom: {
            float x = position.y + bounds.height;           // position with height
            float y = block_position;                       // start of contact block by Y

            float result = y - x;
            if(result > 0) return 0.f;                      // must be negative because block after player position

            return result;
        }
    }

    return 0.f;
}

pair<size_t, size_t> GameMap::get_current_block(const sf::Vector2f& position, const sf::FloatRect& bounds) const {
    return make_pair<size_t, size_t>(
       int((position.x + (bounds.width / 2.f)) / m_block_size),
       int((position.y + (bounds.height / 2.f)) / m_block_size)
    );
}


pair<size_t, size_t> GameMap::get_adjacent_block(
    const GameMap::Direction dir,
    const sf::Vector2f& position,
    const sf::FloatRect& bounds
) const {
    int start_y = std::floor(position.y / m_block_size);
    int end_y = std::floor((position.y + bounds.height) / m_block_size);
    int center_y = (start_y + end_y) / 2;

    int start_x = std::floor(position.x / m_block_size);
    int end_x = std::floor((position.x + bounds.width) / m_block_size);
    int center_x = (start_x + end_x) / 2;

    switch (dir) {
        case Direction::Top: {
            int top_block = start_y - 1;

            return pair(
               center_x,
               top_block > 0 ? top_block : 0
            );
        }
        case Direction::Right: {
            int right_block = end_x + 1;

            return pair(
               right_block > m_tile_width ? m_tile_width : right_block,
               center_y
            );
        }
        case Direction::Bottom: {
            int bottom_block = end_y + 1;

            return pair(
               center_x,
               bottom_block > m_tile_height ? m_tile_height : bottom_block
            );
        }
        case Direction::Left: {
            int left_block = start_x - 1;

            return pair(
               left_block > 0 ? left_block : 0,
               center_y
            );
        }
        default:
            throw std::runtime_error("unknown param");
    }
}

map<GameMap::Direction, pair<size_t, size_t>>
GameMap::get_around_blocks(const sf::Vector2f& position, const sf::FloatRect& bounds) const {
    array<Direction, 4> directions {
       Direction::Top,
       Direction::Right,
       Direction::Bottom,
       Direction::Left
    };

    map<Direction, pair<size_t, size_t>> around_blocks {};
    for (auto& direction : directions) {
        around_blocks[direction] = get_adjacent_block(direction, position, bounds);
    }

    return around_blocks;
}

size_t GameMap::get_width() const {
    return m_tile_width;
}

size_t GameMap::get_height() const {
    return m_tile_height;
}

size_t GameMap::get_block_size() const {
    return m_block_size;
}

size_t GameMap::get_max_block_vising_x() const {
    return m_max_block_vising_x;
}

size_t GameMap::get_max_block_vising_y() const {
    return m_max_block_vising_y;
}

bool GameMap::is_collision_cell(char cell) const {
    bool is_collision = false;
    for(auto& collision_cell: m_collision_cells) {
        if (collision_cell == cell) {
            is_collision = true;
        }

        if (is_collision) break;
    }

    return is_collision;
}


float GameMap::get_offset_x() const {
    return m_offset_x;
}

float GameMap::get_offset_y() const {
    return m_offset_y;
}

const GameMap::t_draw_el* GameMap::get_draw_element(char cell) const {
    auto it = m_cell_draw_el.find(cell);
    if(it != std::end(m_cell_draw_el)) {
        return &(it->second);
    }

    return nullptr;
}

const Paddings& GameMap::get_paddings() const {
    return m_paddings;
}

const set<char>& GameMap::get_collision_cells() const {
    return m_collision_cells;
}