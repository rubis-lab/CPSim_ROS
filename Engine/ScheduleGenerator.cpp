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
    //with a hyper period.
    while(utils::current_time < (0 + utils::hyper_period))
    {
        for(auto ecu : vectors::ecu_vector)
        {
            std::cout << ecu->get_ecu_ready_set().size() << std::endl;
        }
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