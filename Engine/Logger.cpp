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

void Logger::log_transaction_status()
{
    std::ofstream transaction_status;
    transaction_status.open("Log/transaction_status.txt");
    std::string contents = "\n";

    for(auto transaction : vectors::transaction_vector)
    {
        contents += "Transaction ID : " + std::to_string(transaction.at(0)->get_transaction_id()) + "\n";
        for(auto task : transaction)
        {
            contents += "TASK NAME: " + task->get_task_name() + "\n";
            contents += "PERIOD : " + std::to_string(task->get_period()) + "\n";
            contents += "FET : " + std::to_string(task->get_fet()) + "\n";
            contents += "CallbackType: " + std::to_string(task->get_callback_type()) + "\n";
            contents += "Chain pos: " + std::to_string(task->get_chain_pos()) + "\n";
            contents += "ECU id: " + std::to_string(task->get_ECU_id()) + "\n";
            contents += "Priority: " + std::to_string(task->get_priority()) + "\n";
        }
        contents += "\n\n";
    }

    transaction_status.write(contents.c_str(), contents.size());
    transaction_status.close();
}
void Logger::log_system_status()
{
    std::ofstream system_info;
    system_info.open("Log/System_info.txt");
    std::string contents = "\n";
    system_info.write(contents.c_str(), contents.size());
    system_info.close();
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
void Logger::print_job_execution_on_ECU()
{
    std::ofstream execution_schedule;
    execution_schedule.open("Log/ECU_schedules.txt");
    std::string contents = "ECU Log\n";

    for (auto ecu : vectors::ecu_vector)
    {
        contents += "ECU ID : " + std::to_string(ecu->get_ECU_id()) + "\t";
        for(auto job : ecu->get_finished_jobset())
        {
            contents += job->get_task_name() + "," + std::to_string(job->get_job_id()) + "\t";
        }
        contents += "\n\n";
    }
    
    execution_schedule.write(contents.c_str(), contents.size());
    execution_schedule.close();
}
void Logger::print_job_execution_schedule()
{

}
void Logger::log_which_job_was_deadline_miss(std::shared_ptr<Job> deadline_job)
{

}