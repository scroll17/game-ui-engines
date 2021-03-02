//
// Created by user on 09.02.2021.
//

#ifndef STUD_GAME_PROGRESSBAR_H
#define STUD_GAME_PROGRESSBAR_H


class ProgressBar final {
    private:
        int m_step;

        int m_start;
        int m_stop;

    public:
        ProgressBar(int step, int start = 0, int stop = 100): m_step(step), m_start(start), m_stop(stop) {};
        ~ProgressBar();


};


#endif //STUD_GAME_PROGRESSBAR_H
