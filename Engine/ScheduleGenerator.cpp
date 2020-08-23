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

bool ScheduleGenerator::get_is_globally_busy()
{
    return m_is_globally_busy;
}

void ScheduleGenerator::set_is_globally_busy(bool is_g_busy)
{
    m_is_globally_busy = is_g_busy;
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
        for(auto ecu : vectors::ecu_vector)
        {
            // std::cout << ecu->get_ECU_id() << "ECUs ";
            // std::cout << "time : "<< offline_current_time;
            // std::cout << " size : " << ecu->get_released_jobset().size() << std::endl;
            // check any job being released at this moment
            for(auto job : ecu->get_released_jobset())
            {
                // IF Job is a timer callback, it can be directly added to ReadySet.
                if(job->get_callback_type() == 0)
                {
                    if(job->get_real_release_time() == offline_current_time)
                    {
                        job->set_is_real_released(true);
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
                            job->set_is_real_released(true);
                            ecu->add_job_to_ready_set(job);
                            ecu->delete_job_from_released_jobset(std::move(job));
                            if(ecu->get_pending_jobset().size() != 0)
                            {
                                ecu->copy_pending_jobset_to_ready_set();
                                ecu->flush_pending_jobset();    
                            }
                        }
                    }
                    else
                    {
                        if(job->get_real_release_time() == offline_current_time)
                        {
                            job->set_is_real_released(true);
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
            // IF ECU IS IDLE NOW,
            if(ecu->get_is_busy() == false)
            {
                // IF ready_set is not empty, we should start a highest job. 
                if(ecu->get_ecu_ready_set().size() != 0)
                {
                    // select a highest job in the ReadySet
                    // for that, we must find minimum priority value. 
                    ecu->set_is_busy(true);
                    ecu->set_l_busy_period_start_time(offline_current_time);
                    int minimum_priority_value = INT_MAX;
                    int minimum_idx = 0;
                    for(int job_idx = 0; job_idx < ecu->get_ecu_ready_set().size(); job_idx++)
                    {
                        if(ecu->get_ecu_ready_set().at(job_idx)->get_priority() < minimum_priority_value)
                        {
                            minimum_priority_value = ecu->get_ecu_ready_set().at(job_idx)->get_priority();
                            minimum_idx = job_idx;
                        }
                    }
                    
                    // -------------------------------------------------------------------------------------
                    // run a highest job, and if the job has subscriber, then set its release time as job's finish time and add it to released job set
                    ecu->set_who_is_running(ecu->get_ecu_ready_set().at(minimum_idx));
                    ecu->get_who_is_running()->set_real_start_time(offline_current_time);
                    ecu->get_who_is_running()->set_real_finish_time(ecu->get_who_is_running()->get_real_start_time() + ecu->get_who_is_running()->get_fet());
                    ecu->get_who_is_running()->set_is_real_started(true);
                    ecu->get_who_is_running()->set_is_real_running(true);

                    std::shared_ptr<Job> consumer_job = std::make_shared<Job>(ecu->get_who_is_running()->get_consumer(),ecu->get_who_is_running()) ;
                    ecu->get_who_is_running()->set_consumer_job(consumer_job);
                    consumer_job->set_producer_job(ecu->get_who_is_running());
                    vectors::ecu_vector.at(consumer_job->get_ECU_id())->add_job_to_released_jobset(std::move(consumer_job));
                }
                else // IF ready_set is empty, we do nothing.
                {
                    // BECAUSE, WE ALREADY ADD PENDING JOBS TO READY SET.
                }

            }
            else // IF ECU IS BUSY NOW, 
            {
                // IF RUNNING JOB IS FINISHED
                if(ecu->get_who_is_running()->get_real_finish_time() == offline_current_time)
                {
                    ecu->get_who_is_running()->set_is_real_finished(true);
                    ecu->get_who_is_running()->set_is_real_running(false);

                    ecu->add_job_to_finished_jobset(ecu->get_who_is_running());
                    ecu->set_who_is_running(nullptr);
                    // IF READY SET IS EMPTY AFTER THE JOB FINISHED, THEN FINISH THE BUSY PERIOD
                    if(ecu->get_ecu_ready_set().size() == 0)
                    {   
                        ecu->set_is_busy(false);
                        ecu->set_l_busy_period_finish_time(offline_current_time);
                    }
                    else
                    {
                        // DO NOTHING
                    }
                    
                }
                else
                {
                   // DO NOTHING
                }
            }
        }
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