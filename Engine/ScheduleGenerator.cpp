#include <fstream>
#include <climits>
#include "ScheduleGenerator.h"

/**
 *  This file is the cpp file for the ScheduleSimulator class.
 *  @file ScheduleGenerator.cpp
 *  @brief cpp file for Engine-ScheduleGenerator
 *  @author Seonghyeon Park
 *  @date 2020-08-20
 */

/**
 * @fn ScheduleGenerator::ScheduleGenerator()
 * @brief the function of basic constructor of ScheduleSimulator
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return ScheduleSimulator
 * @bug none
 * @warning none
 * @todo none
 */
ScheduleGenerator::ScheduleGenerator()
{
    m_is_offline = true;
}

/**
 * @fn ScheduleGenerator::~ScheduleGenerator()
 * @brief the function of basic destroyer of ScheduleGenerator
 * @author Seonghyeon Park
 * @date 2020-04-01
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
ScheduleGenerator::~ScheduleGenerator()
{

}
/**
 * @fn void ScheduleGenerator::generate_schedule_offline()
 * @brief the function
 * @author Seonghyeon Park
 * @date 2020-08-21
 * @details 
 *  - it generates a schedule in a hyper period at offline state.
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
void ScheduleGenerator::generate_schedule_offline()
{
    unsigned long long offline_current_time = 0;
    generate_job_instances_for_hyper_period();

    while(offline_current_time < (0 + utils::hyper_period))
    {
        bool is_any_job_released = false;
        for(auto ecu : vectors::ecu_vector)
        {
            // std::cout << ecu->get_ECU_id() << "ECUs ";
            // std::cout << "time : "<< offline_current_time;
            // std::cout << " size : " << ecu->get_released_jobset().size() << std::endl;
            //
            for(auto job : ecu->get_released_jobset())
            {
                // IF Job is a timer callback, it can be directly added to ReadySet.
                if(job->get_callback_type() == 0)
                {
                    if(job->get_real_release_time() == offline_current_time)
                    {
                        ecu->add_job_to_ready_set(job);
                        ecu->delete_job_from_released_jobset(std::move(job));
                    }
                }
                else // IF Job is a subscriber callback, it should wait until readySet is empty.
                {
                    if(ecu->get_ecu_ready_set().size() == 0)
                    {
                        if(job->get_real_release_time() == offline_current_time)
                        {
                            ecu->add_job_to_ready_set(job);
                            ecu->delete_job_from_released_jobset(std::move(job));
                            ecu->copy_pending_jobset_to_ready_set();
                            ecu->flush_pending_jobset();
                        }
                    }
                    else
                    {
                        if(job->get_real_release_time() == offline_current_time)
                        {
                            ecu->add_job_to_pending_jobset(job);
                            ecu->delete_job_from_released_jobset(std::move(job));
                        }
                        else
                        {
                            continue;   
                        }
                    }
                }
            }
            // 
            int minimum_priority_value;
            int minimum_idx = 0;
            for(auto job : ecu->get_ecu_ready_set())
            {
                // select a highest job in the ReadySet
                // for that, we must find minimum priority value.
                if()
                {
                    s
                }
            }
        }
        // std::cout << std::endl;

        offline_current_time ++;
    }
    
}
/**
 * @fn void ScheduleGenerator::generate_schedule_online()
 * @brief the function
 * @author Seonghyeon Park
 * @date 2020-08-21
 * @details 
 *  - it generates a job at next hyper period at online state.
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
void ScheduleGenerator::generate_schedule_online()
{
    
}
void ScheduleGenerator::busy_period_analysis(std::vector<std::shared_ptr<Job>>& job_queue, int start, int& end, int ecu_id, bool setWorstCase)
{
    

}

void ScheduleGenerator::update_job_vector()
{
    for(int i = 0; i < vectors::ecu_vector.size(); i++)
    {
        std::vector<std::vector<std::shared_ptr<Job>>> vector_space_for_ecu;
        vectors::job_vectors_for_each_ECU.push_back(vector_space_for_ecu);
    }
    /**
     * Task Vector Initialization
     */
    for(int ecu_num =0; ecu_num < vectors::ecu_vector.size(); ecu_num++)
    {
        for(int i = 0; i < vectors::ecu_vector.at(ecu_num)->get_num_of_task(); i++)
        {
            std::vector<std::shared_ptr<Job>> vector_space_for_task_in_this_ecu;
            vectors::job_vectors_for_each_ECU.at(ecu_num).push_back(vector_space_for_task_in_this_ecu);
        }

    }
}

void ScheduleGenerator::generate_job_instances_for_hyper_period()
{
    //Generates Job instances
    int num_of_job_of_this_task = 0;
    
    for(int task_idx = 0; task_idx < vectors::task_vector.size(); task_idx++)
    {
        if(vectors::task_vector.at(task_idx)->get_callback_type() != 0)
        {
            // if task is not timer_callback, do nothing.
        }
        else
        {
            int task_id = vectors::task_vector.at(task_idx)->get_task_id();
            /**
             * number_of_jobs of this task in this hyper_period if offset is 0
             */
            int num_of_jobs = utils::hyper_period / vectors::task_vector.at(task_idx)->get_period();
            for(int job_id = 1; job_id <= num_of_jobs; job_id++)
            {
                std::shared_ptr<Job> job = std::make_shared<Job>(vectors::task_vector.at(task_id), job_id, 0);
                vectors::ecu_vector.at(job->get_ECU_id())->add_job_to_released_jobset(std::move(job));
            }   
        }
    }
    
}