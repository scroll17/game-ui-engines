//
// Created by user on 03.05.2021.
//

#ifndef STUD_GAME_STRATEGIESCONTROLLER_H
#define STUD_GAME_STRATEGIESCONTROLLER_H

#include "vector"
#include "functional"

using namespace std;

class StrategiesController {
    using t_cb = function<bool()>;

    private:
        bool m_active = true;

        size_t m_current_strategy = 0;
        vector<t_cb> m_strategies {};

    public:
        StrategiesController() = default;
        ~StrategiesController() = default;

        size_t register_strategy(const t_cb& cb);

        void next(size_t pos);

        void start();
        void start(size_t pos);
        void stop();

        [[nodiscard]] size_t get_current_strategy() const;
};


#endif //STUD_GAME_STRATEGIESCONTROLLER_H
