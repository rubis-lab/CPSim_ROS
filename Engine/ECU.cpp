#include "ECU.h"
#include "Utils.h"

/**
 *  This file is the cpp file for the ECU class.
 *  @file ECU.cpp
 *  @brief cpp file for Engine-ECU
 *  @author Seonghyeon Park
 *  @date 2020-08-22
 */

/**
 * @fn ECU::ECU()
 * @brief the function of basic constructor of ECU
 * @author Seonghyeon Park
 * @date 2020-08-22
 * @details 
 *  - None
 * @param none
 * @return ECU
 * @bug none
 * @warning none
 * @todo none
 */
ECU::ECU()
{
    /**
     * This is basic constructor.
     */
}

/**
 * @fn ECU::ECU()
 * @brief the function of fundemental informed constructor of ECU
 * @author Seonghyeon Park
 * @date 2020-08-22
 * @details 
 *  This constructor has all properties that define each ECU.
 * @param performance
 * @param scheduling_policy
 * @param ecu_id
 * @return ECU
 * @bug none
 * @warning none
 * @todo none
 */
ECU::ECU(int performance, std::string scheduling_policy)
{
    m_performance = performance;
    m_scheduling_policy = scheduling_policy;
    m_num_of_task = 0;
    m_ecu_id = vectors::ecu_vector.size();
}

ECU::ECU(int performance, std::string scheduling_policy, int ecu_id)
{
    m_performance = performance;
    m_scheduling_policy = scheduling_policy;
    m_num_of_task = 0;
    m_ecu_id = ecu_id;
}

/**
 * @fn ECU::~ECU()
 * @brief the function of basic destructor of ECU
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
ECU::~ECU()
{
    /**
     * This is basic destructor.
     */
}

/**
 * @fn int get_ECU_id()
 * @brief getter for ECU ID
 * @author Seonghyeon Park
 * @date 2020-08-22
 * @details 
 *  - None
 * @param none
 * @return ECU ID
 * @bug none
 * @warning none
 * @todo none
 */
int ECU::get_ECU_id()
{
    return m_ecu_id;
}

/**
 * @fn int get_performance()
 * @brief getter for ECU Performance
 * @author Seonghyeon Park
 * @date 2020-048-22
 * @details 
 *  - None
 * @param none
 * @return ECU Performance
 * @bug none
 * @warning none
 * @todo none
 */
int ECU::get_performance()
{
    return m_performance;
}

int ECU::get_num_of_task()
{
    return m_num_of_task;
}

/**
 * @fn std::string get_scheduling_policy()
 * @brief getter for scheduling policy
 * @author Seonghyeon Park
 * @date 2020-08-22
 * @details 
 *  - None
 * @param none
 * @return scheduling policy
 * @bug none
 * @warning none
 * @todo none
 */
std::string ECU::get_scheduling_policy()
{
    return m_scheduling_policy;
}

std::vector<std::shared_ptr<Job>> ECU::get_ecu_ready_set()
{
    return m_ready_set;
}
std::vector<std::shared_ptr<Job>> ECU::get_released_jobset()
{
    return m_released_jobset;
}
std::vector<std::shared_ptr<Task>> ECU::get_task_set()
{
    return m_task_set;
}

void ECU::set_ECU_id(int id)
{
    m_ecu_id = id;
}
void ECU::set_performance(int performance)
{
    m_performance = performance;
}
void ECU::set_num_of_task(int num)
{
    m_num_of_task = num;
}
void ECU::set_scheduling_policy(std::string sched_policy)
{
    m_scheduling_policy = sched_policy;
}
void ECU::set_ecu_ready_set(std::vector<std::shared_ptr<Job>> ready_set)
{
    m_ready_set = ready_set;
}
void ECU::set_released_jobset(std::vector<std::shared_ptr<Job>> released_jobset)
{
    m_released_jobset = released_jobset;
}
void ECU::set_task_set(std::vector<std::shared_ptr<Task>> task_set)
{
    m_task_set = task_set;
}

void ECU::add_task_to_ecu(std::shared_ptr<Task> task)
{
    m_task_set.push_back(task);
}

void ECU::add_job_to_ready_set(std::shared_ptr<Job> job)
{
    m_ready_set.push_back(job);
}
void ECU::add_job_to_released_jobset(std::shared_ptr<Job> job)
{
    m_released_jobset.push_back(job);
}
void ECU::add_job_to_pending_jobset(std::shared_ptr<Job> job)
{
    m_pending_jobset.push_back(job);
}

void ECU::delete_job_from_released_jobset(std::shared_ptr<Job> job)
{
    for(int idx = 0; idx < m_released_jobset.size(); idx++)
    {
        if(m_released_jobset.at(idx) == job)
        {
            m_released_jobset.erase(m_released_jobset.begin() + idx);
        }
    }
}
void ECU::delete_job_from_ready_set(std::shared_ptr<Job> job)
{
    for(int idx = 0; idx < m_ready_set.size(); idx++)
    {
        if(m_ready_set.at(idx) == job)
        {
            m_ready_set.erase(m_ready_set.begin() + idx);
        }
    }  
}
void ECU::copy_pending_jobset_to_ready_set()
{
    m_ready_set.insert(m_ready_set.end(), m_pending_jobset.begin(), m_pending_jobset.end());
}
void ECU::flush_pending_jobset()
{
    m_pending_jobset.clear();
}