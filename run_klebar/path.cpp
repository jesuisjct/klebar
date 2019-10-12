#include "path.h"

CPath::CPath(int nb_positions, int nb_axis)
{
    m_thread        = 0;
    m_nb_positions  = nb_positions;
    m_nb_axis       = nb_axis;
    m_positions     = new Position[nb_positions];
    for(int i = 0; i < nb_positions; i++)
    {   m_positions[i].axis_positions = new int[nb_axis];
    }
}

void CPath::set_position(int pos, int duration)
{
    if(pos >= 0 && pos < m_nb_positions) m_positions[pos].t = duration;
}

void CPath::start(int nb_runs)
{
    //printf("start\n");
    m_halt      = false;
    if(m_thread)    stop();
    m_thread    = new std::thread( [=] { run(nb_runs); } );
}

void CPath::stop()
{
    m_halt  = true;
    m_thread->join();
    delete m_thread;
    m_thread = 0;
}

void CPath::run(int nb_runs)
{
    int     n = 0;
    int     pos = 0;
    int     current_pos, target_pos;
    int     target_velocity;
    std::chrono::high_resolution_clock::time_point  time_initial;
    std::chrono::high_resolution_clock::time_point  time_current;
    int     duration;
    int     epsilon_ms = 2;


    time_initial = std::chrono::high_resolution_clock::now();

    for(;;)
    {   if(m_halt)   break;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        //printf("n=%d nb_runs=%d pos=%d\n", n, nb_runs, pos);
        if(nb_runs > 0) {   if (n > nb_runs) break;}
        time_current = std::chrono::high_resolution_clock::now();
        duration = (int) std::chrono::duration_cast<std::chrono::milliseconds>(time_current - time_initial).count();
        //printf("duration=%d m_pos[%d]=%d\n", duration, pos, m_positions[pos].t);
        if( duration > m_positions[pos].t - epsilon_ms) 
        {   pos++;
            if(pos >= m_nb_positions)
            {   pos = 0; 
                time_initial = std::chrono::high_resolution_clock::now();
                n++;
            }
        }
        for(int axis = 0; axis < m_nb_axis; axis++)
        {   current_pos = 0; //get_axis_position(axis);
            target_pos = m_positions[pos].axis_positions[axis];
            //target_velocity = (1000 *(target_pos - current_pos)) / duration;
            //set_velocity(axis, target_velocity);
        }
    }
}

void CPath::wait()
{   if(m_thread) m_thread->join();

}