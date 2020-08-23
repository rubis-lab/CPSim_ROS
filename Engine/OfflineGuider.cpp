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
    // for(int ecu_id = 0; ecu_id < vectors::ecu_vector.size(); ++ ecu_id)
    // {
    //     //
    //     for (auto job : vectors::job_vectors_for_each_ECU.at(ecu_id).at(task_id))
    //     {             
    //         construct_start_job_sets(ecu_id, job); // no is_read() condition, because construct_producer_job_sets needs this info aswell.
    //         if (job->get_is_write())
    //             construct_finish_job_sets(ecu_id, job);
    //         construct_producer_job_sets(ecu_id, job);
    //     }
    // }
    // For each ECU, we have all job which were finished.
    // In that ECUs, we set all jobs producer as edges.
    for(auto ecu : vectors::ecu_vector)
    {
        for(auto job : ecu->get_finished_jobset())
        {
            // IF THIS JOB IS TIMER JOB, START Transaction Analysis
            if(job->get_callback_type() == 0)
            {
                if(job->get_consumer_job() != 0)
                {
                    job->add_job_to_successors(job->get_consumer_job());
                    for(auto consumer : ecu->get_finished_jobset())
                    {
                        if(consumer->get_producer_job() == job)
                        {
                            consumer->add_job_to_predecessors(job);
                        }
                    }
                }
            }
        }
    }
}
void OfflineGuider::update_job_precedence_graph()
{
    // We will update job precedence graph at online state.
    // Cut the finished job, and add new job to the graph.

}
void OfflineGuider::recurrent_transaction_analysis(std::shared_ptr<Job> job)
{
    if(job->get_consumer_job() != nullptr)
    {
        vectors::ecu_vector.at(job->get_ECU_id())->fini1 
        recurrent_transaction_analysis(job->get_consumer_job());
    }
    else
    {
        // NO CONSUMER
    }
    
}