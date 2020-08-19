#include "Logger.h"
#include "Utils.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>
#include <climits>

/**
 *  This file is the cpp file for the Logger class.
 *  @file Logger.cpp
 *  @brief cpp file for Engine-Logger
 *  @author Seonghyeon Park
 *  @date 2020-08-19
 */


/**
 * @fn Logger::Logger()
 * @brief the function of basic constructor of Logger
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Logger::Logger()
{
    
}

/**
 * @fn Logger::~Logger()
 * @brief the function of basic destructor of Logger
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Logger::~Logger()
{

}

/**
 * @fn void start_logging()
 * @brief this function starts the logging of simulation events
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */

std::vector<std::shared_ptr<Job>> Logger::get_execution_order_buffer()
{
    return m_execution_order_buffer;
}
void Logger::set_execution_order_buffer(std::vector<std::shared_ptr<Job>> execution_order_buffer)
{
    m_execution_order_buffer = execution_order_buffer;
}
void Logger::add_current_simulated_job(std::shared_ptr<Job> current_job_instance)
{
    m_execution_order_buffer.push_back(current_job_instance);
    m_current_time_buffer.push_back(utils::current_time);
}

void Logger::log_task_vector_status()
{

}
void Logger::log_job_vector_of_each_ECU_status()
{

}
void Logger::log_job_vector_of_simulator_status()
{

}
void Logger::print_offline_guider_status()
{

}
void Logger::print_job_execution_on_ECU(std::vector<std::shared_ptr<Job>> b, std::vector<std::shared_ptr<Job>> w , int ecu_id)
{

}
void Logger::print_job_execution_schedule()
{

}
void Logger::log_which_job_was_deadline_miss(std::shared_ptr<Job> deadline_job)
{

}