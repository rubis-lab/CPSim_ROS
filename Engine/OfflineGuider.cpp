#include "OfflineGuider.h"
#include <memory>

/**
 *  This file is the cpp file for the OfflineGuider class.
 *  @file OfflineGuider.cpp
 *  @brief cpp file for Engine-OfflineGuider
 *  @author Alex Noble
 *  @date 2020-06-04
 */

/**
 * @fn OfflineGuider::OfflienGuider()
 * @brief the function of basic constructor of OfflineGuider
 * @author Seonghyeon Park
 * @date 2020-04-01
 * @details 
 *  - None
 * @param none
 * @return OfflineGuider
 * @bug none
 * @warning none
 * @todo none
 */
OfflineGuider::OfflineGuider()
{
    /**
        This is basic constructor.
     */
}

/**
 * @fn OfflineGuider::~OfflienGuider()
 * @brief the function of basic destroyer of OfflineGuider
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
OfflineGuider::~OfflineGuider()
{
    /**
        This is basic destructor.
     */
}

/**
 * @fn void OfflineGuider::construct_job_precedence_graph()
 * @brief this function construct a data structure that represent JPG
 * @author Alex Noble
 * @date 2020-06-03
 * @details 
 *  JPG means Job-level Precedence Graph which is a data structure\n
 *  that consists of nodes(Job instances) and edges(Relation between Jobs).
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo to be implemented tonight.
 */
void OfflineGuider::construct_job_precedence_graph() 
{
    // We will construct job precedence graph at offline state.
    // We construct edges only when a job is related with its data producer-consumer relationship.

    // 1, We gonna merge every ECU's Finished Jobset 
    for(auto ecu : vectors::ecu_vector)
    {
        for(auto job : ecu->get_finished_jobset())
        {
            vectors::job_precedence_graph.push_back(job);
        }
    }
     
    // 2, Recurrently, 
    for(auto job : vectors::job_precedence_graph)
    {   
        recurrent_transaction_analysis(job);
    }
}
void OfflineGuider::update_job_precedence_graph()
{

}
void OfflineGuider::recurrent_transaction_analysis(std::shared_ptr<Job> job)
{
    for(int task_idx = 0; task_idx < vectors::transaction_vector.at(job->get_transaction_id()).size(); task_idx++)
    {
        if(job->get_chain_pos() > task_idx)
        {
            for(auto find_job : vectors::job_precedence_graph)
            {
                if(find_job->get_transaction_id() == vectors::transaction_vector.at(job->get_transaction_id()).at(task_idx)->get_transaction_id() && 
                   find_job->get_job_id() == job->get_job_id())
                {
                    if(find_job == job)
                    {
                        continue;
                    }
                    job->add_job_to_predecessors(find_job);
                }
            }
            
        }
        else if(job->get_chain_pos() < task_idx)
        {
            for(auto find_job : vectors::job_precedence_graph)
            {
                if(find_job->get_transaction_id() == vectors::transaction_vector.at(job->get_transaction_id()).at(task_idx)->get_transaction_id() && 
                   find_job->get_job_id() == job->get_job_id())
                {
                    if(find_job == job)
                    {
                        continue;
                    }
                    job->add_job_to_successors(find_job);
                }
            }
        }
    }
    for(auto find_job : vectors::job_precedence_graph)
    {
        if( (job->get_transaction_id() == find_job->get_transaction_id()) && (job->get_job_id() < find_job->get_job_id()) && (job->get_task_id() == find_job->get_task_id()))
        {
            job->add_job_to_successors(find_job);
            find_job->add_job_to_predecessors(job);
        }
    }
}