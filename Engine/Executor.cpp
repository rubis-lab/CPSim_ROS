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
    //run a job
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
    for (auto job : vectors::job_vector_of_simulator)
    {
        if(job->get_is_simulated_finished() == false || job->get_is_simulated_released() == false)
            job->initialize_simulated_deadline();
    }
    for (auto job : vectors::job_vector_of_simulator)
    {
        if(job->get_is_simulated_finished() == false || job->get_is_simulated_released() == false)
            job->update_simulated_deadline();
    } 
}

void Executor::move_ecus_jobs_to_simulator()
{
    vectors::job_vector_of_simulator.clear();
    for(int i = 0; i < vectors::job_vectors_for_each_ECU.size(); i++ )
    {
        for(int task_id = 0; task_id < vectors::job_vectors_for_each_ECU.at(i).size(); ++task_id)
        {
            vectors::job_vector_of_simulator.insert(vectors::job_vector_of_simulator.end(), vectors::job_vectors_for_each_ECU.at(i).at(task_id).begin(), vectors::job_vectors_for_each_ECU.at(i).at(task_id).end());
            vectors::job_vectors_for_each_ECU.at(i).at(task_id).clear();
        }
    }
}

void Executor::update_all(std::shared_ptr<Job> last_simulated_job)
{    
    /**
     * UPDATE THE SUCCESSORS' PREDECESSORS JOB SET
     * ACTUAL START TIME
     * ACTUAL FINISH TIME
     * UPDATED EST
     * UPDATED LST
     * UPDATED EFT
     * UPDATED LFT
     * 
     * JOBSET START TIME
     * JOBSET FINISH TIME
     * JOBSET PROCON
     * 
     * DET_PREDECESSOR
     * DET_SUCCESSOR
     */
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