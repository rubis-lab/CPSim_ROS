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
    for(int ecu_id = 0; ecu_id < vectors::job_vectors_for_each_ECU.size(); ++ ecu_id )
    {
        if(vectors::job_vectors_for_each_ECU.at(ecu_id).size() != 0)
        {
            for(int task_id =0; task_id < vectors::job_vectors_for_each_ECU.at(ecu_id).size(); ++ task_id)
            for (auto job : vectors::job_vectors_for_each_ECU.at(ecu_id).at(task_id))
            {             
                construct_start_job_sets(ecu_id, job); // no is_read() condition, because construct_producer_job_sets needs this info aswell.
                if (job->get_is_write())
                    construct_finish_job_sets(ecu_id, job);
                construct_producer_job_sets(ecu_id, job);
            }
        }
    }
}
void OfflineGuider::update_job_precedence_graph()
{

}

// To affect us, a job must be higher priority, and from another task.
// Current jobs WCBP should start before or at same time as the high job's release time.
// comparing jobs real release should be <= lst of current job.
// Also their lst should be earlier than our est to be deterministic.
void OfflineGuider::construct_start_job_sets(int ecu_id, std::shared_ptr<Job>& current_job)
{

}

void OfflineGuider::construct_finish_job_sets(int ecu_id, std::shared_ptr<Job>& current_job)
{

}

// Constructs the job set that represents all the jobs that produce us.
// Make sure that start time job sets are constructed before calling this.
// Push back potential_producer items into deterministic set.
// if his lst < current job's est
// Otherwise push back into non-deterministic.

// Push back job_set start items into deterministic set.
// if his lst < current job's est.
// Otherwise push back into non-deterministic.
// Push back deterministic into deterministic if he isn't nullptr.
void OfflineGuider::construct_producer_job_sets(int ecu_id, std::shared_ptr<Job>& current_job)
{

}