#include "Executor.h"
#include "Utils.h"
#include <ctime>
#include <cstdlib>
#include <climits>
#include <unordered_map>
#include "Logger.h"
#include <fstream>
#define EPSILON 0.000001
#define ABS(X) ((X) < 0 ? -(X) : (X))

/**
 *  This file is the cpp file for the Executor class.
 *  @file Executor.cpp
 *  @brief cpp file for Engine-Executor
 *  @author Alex Noble
 *  @date 2020-06-04
 */

/**
 * @fn Executor::Executor()
 * @brief the function of basic constructor of Executor
 * @author Seonghyeon Park
 * @date 2020-03-31
 * @details 
 *  - None
 * @param none
 * @return Executor
 * @bug none
 * @warning none
 * @todo none
 */
Executor::Executor()
{

}

/**
 * @fn Executor::~Executor()
 * @brief the function of basic destroyer of Executor
 * @author Seonghyeon Park
 * @date 2020-03-31
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Executor::~Executor()
{

}

int Executor::get_current_hyper_period_index()
{
    return m_current_hyper_period_index;
}

int Executor::get_current_hyper_period_start()
{
    return m_current_hyper_period_start;
}

int Executor::get_current_hyper_period_end()
{
    return m_current_hyper_period_end;
}
int Executor::get_simulator_scheduler_mode()
{
    return m_simulator_scheduler_mode;
}

void Executor::set_current_hyper_period_index(int current_hyper_period_index)
{
    m_current_hyper_period_index = current_hyper_period_index;
}

void Executor::set_current_hyper_period_start(int current_hyper_period_start)
{
    m_current_hyper_period_start = current_hyper_period_start;
}

void Executor::set_current_hyper_period_end(int current_hyper_period_end)
{
    m_current_hyper_period_end = current_hyper_period_end;
}
void Executor::set_simulator_scheduler_mode(int mode)
{
    m_simulator_scheduler_mode = mode;
}

/**
 * @fn void Executor::run_simulation()
 * @brief the function for running simulation engine
 * @author Alex Noble
 * @date 2020-06-04
 * @details 
 *  This function is essential function for running simulation.\n
 *  It has a loop that iterately runs a process for the simulation steps.\n
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo implement this today's night
 */
bool Executor::run_simulation()
{
    // EDF
    if(m_simulator_scheduler_mode == 0)
    {
        // IF SIMULATOR IS IDLE
        if(is_busy == false)
        {
            if(m_simulation_ready_queue.size() != 0)
            {
                is_busy = true;
                int min_deadline = INT_MAX;
                int min_idx = 0;
                for(int job_idx = 0; job_idx < m_simulation_ready_queue.size(); job_idx++)
                {
                    if(m_simulation_ready_queue.at(job_idx)->get_simulated_deadline() < min_deadline)
                    {
                        min_idx = job_idx;
                        min_deadline = m_simulation_ready_queue.at(job_idx)->get_simulated_deadline();
                    }
                }
                who_is_running = m_simulation_ready_queue.at(min_idx);
                delete_job_from_simulation_ready_queue(who_is_running);
                who_is_running->set_is_simulated_running(true);
                who_is_running->set_is_simulated_started(true);
                who_is_running->set_simulated_start_time(utils::current_time);
                who_is_running->set_simulated_finish_time(who_is_running->get_simulated_start_time() + who_is_running->get_simulated_execution_time());
            }
            return true;
        }
        else // IF SIMULATOR IS BUSY,
        {
            if(ABS(who_is_running->get_simulated_finish_time() - utils::current_time) < EPSILON)
            {
                who_is_running->set_is_simulated_running(false);
                who_is_running->set_is_simulated_finished(true);
                for(auto job : vectors::job_precedence_graph)
                {
                    job->delete_job_from_predecessors(who_is_running);
                }
                if(who_is_running->get_simulated_deadline() < who_is_running->get_simulated_finish_time())
                {
                    return false;
                }
                else
                {
                    if(m_simulation_ready_queue.size() != 0)
                    {
                        int min_deadline = INT_MAX;
                        int min_idx = 0;
                        for(int job_idx = 0; job_idx < m_simulation_ready_queue.size(); job_idx++)
                        {
                            if(m_simulation_ready_queue.at(job_idx)->get_simulated_deadline() < min_deadline)
                            {
                                min_idx = job_idx;
                                min_deadline = m_simulation_ready_queue.at(job_idx)->get_simulated_deadline();
                            }
                        }

                        who_is_running = m_simulation_ready_queue.at(min_idx);
                        delete_job_from_simulation_ready_queue(who_is_running);
                        who_is_running->set_is_simulated_running(true);
                        who_is_running->set_is_simulated_started(true);
                        who_is_running->set_simulated_start_time(utils::current_time);
                        who_is_running->set_simulated_finish_time(who_is_running->get_simulated_start_time() + who_is_running->get_simulated_execution_time());
                        return true;
                    }   
                    else
                    {
                        who_is_running = nullptr;
                        is_busy = false;
                        return true;
                    }
                }
                
            }
            else
            {
                return true;
            }
            
        }
    }
    else // ROS2
    {
        if(is_busy == false)
        {
            if(m_ready_set.size() != 0)
            {
                //Select highest job in the ready set
                is_busy = true;
                int min_val = INT_MAX;
                int min_idx = 0;
                for(int job_idx = 0; job_idx < m_ready_set.size(); job_idx++)
                {
                    if(m_ready_set.at(job_idx)->get_priority() < min_val)
                    {
                        min_idx = job_idx;
                        min_val = m_ready_set.at(job_idx)->get_priority(); 
                    }
                }
                who_is_running = m_ready_set.at(min_idx);
                delete_job_from_simulation_ready_set(who_is_running);
                who_is_running->set_simulated_start_time(utils::current_time);
                who_is_running->set_simulated_finish_time(who_is_running->get_simulated_start_time() + who_is_running->get_simulated_execution_time());
                who_is_running->set_is_simulated_started(true);
                who_is_running->set_is_simulated_running(true);
            }
            return true;
        }
        else // BUSY
        {
            if(ABS(who_is_running->get_simulated_finish_time() - utils::current_time) < EPSILON)
            {
                who_is_running->set_is_simulated_running(false);
                who_is_running->set_is_simulated_finished(true);
                std::cout << who_is_running->get_task_name() << who_is_running->get_job_id() << std::endl;
                if(who_is_running->get_simulated_deadline() < who_is_running->get_simulated_finish_time())
                {
                    return false;
                }
                else
                {
                    if(m_ready_set.size() != 0)
                    {
                        int min_val = INT_MAX;
                        int min_idx = 0;
                        for(int job_idx = 0; job_idx < m_ready_set.size(); job_idx++)
                        {
                            if(m_ready_set.at(job_idx)->get_priority() < min_val)
                            {
                                min_idx = job_idx;
                                min_val = m_ready_set.at(job_idx)->get_priority(); 
                            }
                        }

                        who_is_running = m_ready_set.at(min_idx);
                        delete_job_from_simulation_ready_set(who_is_running);
                        who_is_running->set_is_simulated_running(true);
                        who_is_running->set_is_simulated_started(true);
                        who_is_running->set_simulated_start_time(utils::current_time);
                        who_is_running->set_simulated_finish_time(who_is_running->get_simulated_start_time() + who_is_running->get_simulated_execution_time());
                        return true;
                    }   
                    else
                    {
                        who_is_running = nullptr;
                        is_busy = false;
                        return true;
                    }
                }
            }
            else
            {
                return true;
            }
            
        }
        
    }
}

void Executor::assign_deadline_for_simulated_jobs()
{
    for (auto job : vectors::job_precedence_graph)
    {
        if(job->get_is_simulated_finished() == false || job->get_is_simulated_released() == false)
            job->initialize_simulated_deadline();
    }
    for (auto job : vectors::job_precedence_graph)
    {
        if(job->get_is_simulated_finished() == false || job->get_is_simulated_released() == false)
            job->update_simulated_deadline();
    } 
}
void Executor::assign_deadline_for_simulated_jobs_ros2()
{
    for (auto job : vectors::released_set)
    {
        if(job->get_is_simulated_finished() == false || job->get_is_simulated_released() == false)
            job->initialize_simulated_deadline();
    }
    for (auto job : vectors::released_set)
    {
        if(job->get_is_simulated_finished() == false || job->get_is_simulated_released() == false)
            job->update_simulated_deadline();
    } 
}
void Executor::check_job_precedence_graph()
{
    for(auto job : vectors::job_precedence_graph)
    {        
        // READ CONSTRAINED JOB MUST WAIT ITS REAL START TIME
        if(job->get_is_read() == true)
        {
            if(job->get_real_start_time() > utils::current_time)
            {
                continue;
            }
        }
        
        // SELECT A JOB WHICH DON'T HAVE ANY PREDECESSOR
        if(job->get_det_prdecessors().size() == 0)
        {
            job->set_simulated_release_time(utils::current_time);
            job->set_is_simulated_released(true);
            m_simulation_ready_queue.push_back(job);      
        }
        else
        {
            continue;
        }
    }

    bool is_all_deleted = false;
    while(is_all_deleted == false)
    {
        int size = vectors::job_precedence_graph.size();
        int detect_cnt = 0;
        for(int job_idx = 0; job_idx < size; job_idx++)
        {
            if(vectors::job_precedence_graph.at(job_idx)->get_is_simulated_released() == true)
            {
                delete_job_from_job_precedence_graph(vectors::job_precedence_graph.at(job_idx));
                break;
            }
            detect_cnt++;
        }
        if(detect_cnt == size)
        {
            is_all_deleted = true;
        }
    }
}
void Executor::delete_job_from_job_precedence_graph(std::shared_ptr<Job> job)
{
    for(int idx = 0; idx < vectors::job_precedence_graph.size(); idx++)
    {
        if(vectors::job_precedence_graph.at(idx) == job)
        {
            vectors::job_precedence_graph.erase(vectors::job_precedence_graph.begin() + idx);
            break;
        }
    }  
}
void Executor::delete_job_from_simulation_ready_set(std::shared_ptr<Job> job)
{
    for(int idx = 0; idx < m_ready_set.size(); idx++)
    {
        if(m_ready_set.at(idx) == job)
        {
            m_ready_set.erase(m_ready_set.begin() + idx);
            break;
        }
    }    
}
void Executor::delete_job_from_simulation_ready_queue(std::shared_ptr<Job> job)
{
    for(int idx = 0; idx < m_simulation_ready_queue.size(); idx++)
    {
        if(m_simulation_ready_queue.at(idx) == job)
        {
            m_simulation_ready_queue.erase(m_simulation_ready_queue.begin() + idx);
            break;
        }
    }    
}
void Executor::delete_job_from_released_set(std::shared_ptr<Job> job)
{
    for(int idx = 0; idx < vectors::released_set.size(); idx++)
    {
        if(vectors::released_set.at(idx) == job)
        {
            vectors::released_set.erase(vectors::released_set.begin() + idx);
            break;
        }
    }    
}
void Executor::delete_job_from_simulation_pending_set(std::shared_ptr<Job> job)
{
    for(int idx = 0; idx < m_pending_set.size(); idx++)
    {
        if(m_pending_set.at(idx) == job)
        {
            m_pending_set.erase(m_pending_set.begin() + idx);
            break;
        }
    }    
}
void Executor::update_simulation_pending_set()
{
    m_ready_set.insert(m_ready_set.end(), m_pending_set.begin(), m_pending_set.end());
    m_pending_set.clear();
}

void Executor::check_ros2_ready_set()
{
    // WE GENERATED ALL JOB INSTANCES IN THIS HYPER PERIOD. ( IN RELEASED_SET)
    // IN ROS2 SCHEDULE, WE NEED TO KEEP THE FUNCTIONAL TEMPORAL CORRECTNESS.
    // SO, WE NEED TO RELEASE TIMER JOB WHEN THEIR READ CONSTRAINTS SATISFIED.
    // FOR, SUBSCRIBERS, WE DIDN'T MAKE PREDECESSOR RELATIONS.
    //
    // CALLBACKS SCHEDULE MUST BE SAME WITH ITS REAL CYBER SYSTEM.
    // TIMER CALLBACK MUST BE RELEASED AT ITS REAL RELEASE TIME
    // SUBSCRIBER CALLBACK BE RELEASED AT ITS PRODUCER JOBS SIMULATED_FINISH_TIME
    for(auto job : vectors::released_set)
    {        
        // READ CONSTRAINED JOB MUST WAIT ITS REAL START TIME
        if(job->get_callback_type() == 0)
        {
            if(ABS(job->get_real_release_time() - utils::current_time) < EPSILON)
            {
                job->set_simulated_release_time(utils::current_time);
                job->set_is_simulated_released(true);
                if(m_ready_set.size() == 0)
                {
                    if(m_pending_set.size() != 0)
                        update_simulation_pending_set();
                }
                m_ready_set.push_back(job);
            }
        }
        else
        {
            if(job->get_producer_job()->get_is_simulated_finished() == true)
            {
                job->set_is_simulated_released(true);
                job->set_simulated_release_time(job->get_producer_job()->get_simulated_finish_time());
                if(m_ready_set.size() == 0)
                {
                    m_ready_set.push_back(job);
                    if(m_pending_set.size() != 0)
                        update_simulation_pending_set();
                }
                else
                {
                    m_pending_set.push_back(job);
                }
            }
        }
    }

    bool is_all_deleted = false;
    while(is_all_deleted == false)
    {
        int size = vectors::released_set.size();
        int detect_cnt = 0;
        for(int job_idx = 0; job_idx < size; job_idx++)
        {
            if(vectors::released_set.at(job_idx)->get_is_simulated_released() == true)
            {
                delete_job_from_released_set(vectors::released_set.at(job_idx));
                break;
            }
            detect_cnt++;
        }
        if(detect_cnt == size)
        {
            is_all_deleted = true;
        }
    }
}