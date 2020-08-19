#include <fstream>
#include <climits>
#include "ScheduleGenerator.h"

/**
 *  This file is the cpp file for the ScheduleSimulator class.
 *  @file ScheduleSimulator.cpp
 *  @brief cpp file for Engine-ScheduleSimulator
 *  @author Seonghyeon Park
 *  @date 2020-03-31
 */

/**
 * @fn ScheduleSimulator::ScheduleSimulator()
 * @brief the function of basic constructor of ScheduleSimulator
 * @author Seonghyeon Park
 * @date 2020-04-01
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
 * @fn ScheduleSimulator::~ScheduleSimulator()
 * @brief the function of basic destroyer of ScheduleSimulator
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
 * @fn void ScheduleSimulator::simulate_scheduling_on_Real()
 * @brief this function simulates a scheduling scenario of Real Cyber System.
 * @author Seonghyeon Park
 * @date 2020-04-01
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo will be implemented tonight.
 */
void ScheduleGenerator::simulate_scheduling_on_real(double global_hyper_period_start_point)
{
    // LK, CC, SENSING
    // LOOP THROUGH ALL THE .SO FUNCTIONS
    // --- We execute all of them x times.
    // --- We save the execution time





    if(m_is_offline == true)
    {
        /**
         *  Generate scheduling simulation to refer to.
         *  Generated Scheduling Simulation Result will be stored to the utils:: 
         */
        m_hyper_period = utils::hyper_period;
        for(auto task : vectors::task_vector) // Incase simulate is run multiple times on same task set.
            task->set_priority(0);

        // Assign CPU Task Priorities
        for(auto task : vectors::task_vector)
        {
            for(auto other_task : vectors::task_vector)
            {
                if(task == other_task) continue;
                if(task->get_period() > other_task->get_period())
                    task->set_priority(task->get_priority() + 1);
            }
        }
    }
    else
    {
        update_job_vector();
    }
    
    
    /** 
     * Job instances generation for one HP
     */
    // Get rid of previous simulation loops stuff.
    //for(auto iter = vectors::task_vector.begin(); iter != vectors::task_vector.end(); iter ++ )
    for(int i = 0; i < vectors::task_vector.size(); ++i)
    {   // #CRASH#
        // Crash occurs in here when GPU is enabled.
        // I think it is because
        // Init and Sync jobs are two different tasks, but they have the same task ID as eachother.
        // I am not sure.
        // What is the best way to fix this?
        //int task_idx = iter->get()->get_task_id();
        int task_id = vectors::task_vector.at(i)->get_task_id();
        /**
         * number_of_jobs of this task in this hyper_period if offset is 0
         */
        int num_of_jobs = m_hyper_period / vectors::task_vector.at(i)->get_period();//iter->get()->get_period();
        for(int job_id = 1; job_id <= num_of_jobs; job_id++)
        {
            //std::shared_ptr<Job> job = std::make_shared<Job>(*iter, job_id);
            std::shared_ptr<Job> job = std::make_shared<Job>(vectors::task_vector.at(task_id), job_id, global_hyper_period_start_point);
            job->m_casted_func = vectors::task_vector.at(task_id)->m_casted_func;
            vectors::job_vectors_for_each_ECU.at(vectors::task_vector.at(i)->get_ECU()->get_ECU_id()).at(vectors::task_vector.at(i)->get_vector_idx()).push_back(std::move(job));
            //vectors::job_vectors_for_each_ECU.at(iter->get()->get_ECU()->get_ECU_id()).at(task_idx).push_back(std::move(job));
        }
    }

    for(int ecu_id = 0; ecu_id < vectors::job_vectors_for_each_ECU.size(); ++ecu_id) 
        for(int task_id = 0; task_id < vectors::job_vectors_for_each_ECU.at(ecu_id).size(); ++task_id)
            for(auto job : vectors::job_vectors_for_each_ECU.at(ecu_id).at(task_id))
            {
                job->set_is_started(false);
                job->set_is_finished(false);
                job->set_is_preempted(false);
                job->set_is_resumed(false);
            }
    
    int offset = global_hyper_period_start_point;
    // ACCOUNT FOR GPU JOBS.
    // Schedule GPU JOBS.

    // Effectively, the result of this function is:
    // set_is_released(true)
    // bpet
    // wpet
    // est
    // lst
    // wcbp
    // eft
    // lft
    /**
     * Generate schedule each ECUs
     */
    for(int ecu_id = 0; ecu_id < vectors::job_vectors_for_each_ECU.size(); ++ecu_id)
    {
        bool is_idle = true;
        bool is_best = true;
        for (int i = 0; i < 2; i++) // First BCET, then WCET.
        {
            //for (int ecu_id = 0; ecu_id < vectors::job_vectors_for_each_ECU.size(); ++ecu_id)
            for(int task_id = 0; task_id < vectors::job_vectors_for_each_ECU.at(ecu_id).size(); ++task_id)
                for (auto job : vectors::job_vectors_for_each_ECU.at(ecu_id).at(task_id))
                {
                    job->set_is_started(false);
                    job->set_is_finished(false);
                    job->set_is_preempted(false);
                    job->set_is_resumed(false);
                }
            int current_time_point = global_hyper_period_start_point;



            //int current_time_point = -1;
            // Find first free time slot.
            //for (int i = 0; i < m_hyper_period; i++)
            //    if (freeTimeSlots[i])
            //    {
            //        current_time_point = global_hyper_period_start_point + i;
            //        break;
            //    }
            //if (current_time_point == -1)
            //{
                //std::cout << "COULD NOT FIND A TIME SLOT FOR CPU JOBS.\nSKIPPING ITERATION OF THIS FOR LOOP." << std::endl;
            //    continue;
            //}
            int busy_period_start_point = 0;
            //check_released_jobs_at_the_time_point
            while(current_time_point < (global_hyper_period_start_point + m_hyper_period))
            {
                
                busy_period_start_point = current_time_point;
                
                int sum_of_execution = 0;
                std::vector<std::shared_ptr<Job>> job_queue; 
                
                /*
                * if released job exist, then put it in the job_queue.
                * and, set busy_period_start as job's release time.
                */    
                for(int task_id = 0; task_id < vectors::job_vectors_for_each_ECU.at(ecu_id).size(); ++task_id)
                    for(auto job : vectors::job_vectors_for_each_ECU.at(ecu_id).at(task_id))
                    {
                        //if(job->get_priority_policy() != PriorityPolicy::CPU) continue; // Only account for CPU jobs, maintain backwards compatability.
                        if((!job->get_is_finished()) && (job->get_actual_release_time() <= current_time_point))
                        {
                            job->set_is_released(true);
                            job_queue.push_back(job);
                            is_idle = false;
                        }
                    }
                if(is_idle)
                {
                    current_time_point += 1; // Skip to next iteration of loop.
                }
                else
                {
                    busy_period_analysis(job_queue, busy_period_start_point, sum_of_execution, ecu_id, !is_best);
                    current_time_point += sum_of_execution;
                    is_idle = true;
                }
                // Skip to next available timeslot (not blocked by GPU).
                /*bool found = false;
                for (int i = (current_time_point - offset); i < m_hyper_period; i++)
                    if (freeTimeSlots[i])
                    {
                        current_time_point = global_hyper_period_start_point + i;
                        found = true;
                        break;
                    }
                if (!found)
                {
                    //std::cout << "Could not find available time slot to continue execution.\nBreaking out of while loop." << std::endl;
                    break;
                }*/
                if(current_time_point > (global_hyper_period_start_point + m_hyper_period))
                {
                    //std::cout << "CURRENT TIME OVER " << std::endl;
                }
            }
            is_best = !is_best;
        }
        global_object::logger->print_job_execution_on_ECU(m_execution_order_b, m_execution_order_w, ecu_id);
        m_execution_order_b.clear();
        m_execution_order_w.clear();
    }
    global_object::logger->log_job_vector_of_each_ECU_status();
    m_is_offline = false;
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