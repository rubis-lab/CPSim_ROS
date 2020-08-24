#include "Executor.h"
#include "Utils.h"
#include <ctime>
#include <cstdlib>
#include <climits>
#include <unordered_map>
#include "Logger.h"
#include <fstream>

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
            who_is_running->set_is_simulated_running(true);
            who_is_running->set_is_simulated_started(true);
            who_is_running->set_simulated_start_time(utils::current_time);
            who_is_running->set_simulated_finish_time(who_is_running->get_simulated_start_time() + who_is_running->get_simulated_execution_time());
            return true;
        }
        else // IF SIMULATOR IS BUSY,
        {
            if(who_is_running->get_simulated_finish_time() == utils::current_time)
            {
                who_is_running->set_is_simulated_finished(true);
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
                        who_is_running->set_is_simulated_running(true);
                        who_is_running->set_is_simulated_started(true);
                        who_is_running->set_simulated_start_time(utils::current_time);
                        who_is_running->set_simulated_finish_time(who_is_running->get_simulated_start_time() + who_is_running->get_simulated_execution_time());
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
        // IDLE
        if(is_busy == false)
        {

        }
        else // BUSY
        {
            
        }
    }
    
    return true;
}

void Executor::change_execution_time()
{
    for (auto job : vectors::job_vector_of_simulator)
    {
        job->set_simulated_execution_time(job->get_real_execution_time() * utils::simple_mapping_function);
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

void Executor::update_simulated_deadlines(int job_index)
{
    /**
     * EVERY JOB'S SUCCESSORS CHANGED, WHENEVER A JOB IS SIMULATED, SO THAT SIMULATED DEADLINE CAN BE CHAGNED
     */
    vectors::job_vector_of_simulator.at(job_index)->update_simulated_deadline();
}

bool Executor::check_deadline_miss()
{
    for(auto job : vectors::job_vector_of_simulator)
    {
        if(job->get_simulated_finish_time() > job->get_simulated_deadline())
        { 
            //std::cout << "Simulated finish time for job " << job->get_task_name() << ":" << job->get_job_id() << " was " << job->get_simulated_finish_time() << std::endl;
            //std::cout << "The simulated deadline was " << job->get_simulated_deadline() << std::endl;
            global_object::logger->log_which_job_was_deadline_miss(job);
            return true; //deadline miss occured
        } 
    }

    return false; // -1 for success.
}

bool Executor::simulatability_analysis()
{
    bool is_simulatable = !check_deadline_miss();
    return is_simulatable;
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
            delete_job_from_job_precedence_graph(job);        
        }
        else
        {
            continue;
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
        }
    }  
}