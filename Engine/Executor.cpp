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
        if(job->get_is_simulated() == false || job->get_is_released() == false)
            job->initialize_simulated_deadline();
    }
    for (auto job : vectors::job_vector_of_simulator)
    {
        if(job->get_is_simulated() == false || job->get_is_released() == false)
            job->update_simulated_deadline();
    } 
}

void Executor::assign_predecessors_successors()
{
    // Does this sort have any purpose? Pls let me know :)
    std::sort(vectors::job_vector_of_simulator.begin(), vectors::job_vector_of_simulator.end(), utils::first_release);
    std::unordered_map<std::string, bool> duplication_check_det_pred;
    std::unordered_map<std::string, bool> duplication_check_non_det_pred;

    for (auto job : vectors::job_vector_of_simulator)
    {
        job->set_is_simulated(false); // is_simulated true means is finished in sim.
        job->set_is_released(false);
        job->get_det_prdecessors().clear();
        job->get_non_det_prdecessors().clear();
        job->get_det_successors().clear();
        job->get_non_det_successors().clear();
    }

    for(auto job : vectors::job_vector_of_simulator)
    {
        duplication_check_det_pred.clear();

        duplication_check_det_pred[std::to_string(job->get_task_id()) + ":" + std::to_string(job->get_job_id())] = true;
        for (auto other_job : vectors::job_vector_of_simulator) // For both CPU and GPU jobs.
        {
            if (job == other_job) continue;
            std::string identifier = std::to_string(other_job->get_task_id()) + ":" + std::to_string(other_job->get_job_id());
            if (job->get_task_id() == other_job->get_task_id() && other_job->get_job_id() < job->get_job_id())
            {
                job->get_det_prdecessors().push_back(other_job);
                other_job->get_det_successors().push_back(job);
                duplication_check_det_pred[identifier] = true;
            }
        }
        // det_predecessors are:
        // 1. Jobs with same task ID and Earlier job id.
        // 2. If our job is a Read job, then all jobs that affect our start time deterministically are det_predecessors.
        // 3. If our job is a Write job, then all jobs that affect our finish time deterministically are det_predecessors.
        // 4. All jobs in get_job_set_pro_con_det are det_predecessors.
        // Get all deterministic predecessors and deterministic successors:
        if (job->get_is_read()) // Technically only affects CPU jobs.
        {
            for (auto other_job : job->get_job_set_start_det()) // This job set is empty on GPU Policy Jobs, don't worry.
            {
                std::string identifier = std::to_string(other_job->get_task_id()) + ":" + std::to_string(other_job->get_job_id());
                if (!duplication_check_det_pred[identifier])
                {
                    job->get_det_prdecessors().push_back(other_job);
                    other_job->get_det_successors().push_back(job);
                    duplication_check_det_pred[identifier] = true;
                }
            }
        }
        else if (job->get_is_write()) // Technically only affects CPU jobs.
        {
            for (auto other_job : job->get_job_set_finish_det()) // This job set is empty on GPU Policy Jobs, don't worry.
            {
                std::string identifier = std::to_string(other_job->get_task_id()) + ":" + std::to_string(other_job->get_job_id());
                if (!duplication_check_det_pred[identifier])
                {
                    job->get_det_prdecessors().push_back(other_job);
                    other_job->get_det_successors().push_back(job);
                    duplication_check_det_pred[identifier] = true;
                }
            }
        }

        for (auto other_job : job->get_job_set_pro_con_det()) // For GPU Sync, this should be the init. For GPU Init, same as normal CPU job.
        {
            std::string identifier = std::to_string(other_job->get_task_id()) + ":" + std::to_string(other_job->get_job_id());
            if (!duplication_check_det_pred[identifier])
            {
                job->get_det_prdecessors().push_back(other_job);
                other_job->get_det_successors().push_back(job);
                duplication_check_det_pred[identifier] = true;
            }
        }

        // non_det_predecessors are:
        // 1. If our job is read: then all jobs in get_job_set_start_non_det()
        // 2. If our job is write: then all jobs in get_job_set_finish_non_det()
        // 3. All jobs in get_job_set_pro_con_non_det().
        // Get all non-deterministic predecessors and non-deterministic successors:
        if (job->get_is_read())
        {
            for (auto other_job : job->get_job_set_start_non_det()) // Empty on GPU.
            {
                std::string identifier = std::to_string(other_job->get_task_id()) + ":" + std::to_string(other_job->get_job_id());
                if (!duplication_check_det_pred[identifier] && !duplication_check_non_det_pred[identifier])
                {
                    job->get_non_det_prdecessors().push_back(other_job);
                    other_job->get_non_det_successors().push_back(job);
                    duplication_check_non_det_pred[identifier] = true;
                }
            }
        }
        else if (job->get_is_write())
        {
            for (auto other_job : job->get_job_set_finish_non_det()) // Empty on GPU.
            {
                std::string identifier = std::to_string(other_job->get_task_id()) + ":" + std::to_string(other_job->get_job_id());
                if (!duplication_check_det_pred[identifier] && !duplication_check_non_det_pred[identifier])
                {
                    job->get_non_det_prdecessors().push_back(other_job);
                    other_job->get_non_det_successors().push_back(job);
                    duplication_check_non_det_pred[identifier] = true;
                }
            }
        }

        for (auto other_job : job->get_job_set_pro_con_non_det()) // For GPU Sync, empty. For GPU Init..Same as normal CPU job..
        {
            std::string identifier = std::to_string(other_job->get_task_id()) + ":" + std::to_string(other_job->get_job_id());
            if (!duplication_check_det_pred[identifier] && !duplication_check_non_det_pred[identifier])
            {
                job->get_det_prdecessors().push_back(other_job);
                other_job->get_non_det_successors().push_back(job);
                duplication_check_non_det_pred[identifier] = true;
            }
        }
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

// Recursive function to update time ranges of jobs.
void Executor::update_ecu_schedule(std::shared_ptr<Job> source_job, OldData old_data)
{
    
}

void Executor::update_simulated_deadlines(int job_index)
{
    /**
     * EVERY JOB'S SUCCESSORS CHANGED, WHENEVER A JOB IS SIMULATED, SO THAT SIMULATED DEADLINE CAN BE CHAGNED
     */
    vectors::job_vector_of_simulator.at(job_index)->update_simulated_deadline();
}

void Executor::update_jobset(std::shared_ptr<Job> simulated_job)
{
    for (int i = 0; i < simulated_job->get_det_successors().size(); i++)
    {
        for (int j = 0; j < simulated_job->get_det_successors().at(i)->get_det_prdecessors().size(); j++)
        {
            if (simulated_job->get_det_successors().at(i)->get_det_prdecessors().at(j) == simulated_job)
            {
                // Remove simulated_job from the successor's deterministic predecessor list.
                simulated_job->get_det_successors().at(i)->get_det_prdecessors().at(j) = std::move(simulated_job->get_det_successors().at(i)->get_det_prdecessors().back());
                simulated_job->get_det_successors().at(i)->get_det_prdecessors().pop_back();
                break;
            }
        }
    }

    for (int i = 0; i < simulated_job->get_non_det_successors().size(); i++)
    {
        for (int j = 0; j < simulated_job->get_non_det_successors().at(i)->get_non_det_prdecessors().size(); j++)
        {
            if (simulated_job->get_non_det_successors().at(i)->get_non_det_prdecessors().at(j) == simulated_job)
            {
                // Remove simulated_job from the successor's non-deterministic predecessor list.
                simulated_job->get_non_det_successors().at(i)->get_non_det_prdecessors().at(j) = std::move(simulated_job->get_non_det_successors().at(i)->get_non_det_prdecessors().back());
                simulated_job->get_non_det_successors().at(i)->get_non_det_prdecessors().pop_back();
                break;
            }
        }
    }
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