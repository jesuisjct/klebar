#include <cstdio>
#include <chrono>
#include <ctime>
#include <thread>


class CPath
{
public:
         CPath(int nb_axis, int nb_positions);
    void set_position(int n, int t);
    void set_axis_position(int n, int axis_position);

    void start(int nb_runs);
    void stop();
    void wait();

private:
    void    run(int nb_runs);
    int     m_nb_axis;
    int     m_nb_positions;
    bool    m_halt;

    std::thread *m_thread;

    struct Position
    {   int  t;
        int  *axis_positions;
    } *m_positions;


};