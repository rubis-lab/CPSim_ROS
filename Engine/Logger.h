#ifndef LOGGER_H__
#define LOGGER_H__
#include <iostream>
#include <vector>
#include "Job.h"


/** This file is engine code of CPSim-Re engine
 * @file Logger.h
 * @class Logger
 * @author Seonghyeon Park
 * @date 2020-08-18
 * @brief Codes for Engine-Logger 
*/
class Logger{
private:
    std::vector<std::shared_ptr<Job>> m_execution_order_buffer;
    std::vector<double> m_current_time_buffer;
    
public:
    /**
     * Constructors and Destructors
     */
    Logger();
    ~Logger();

    /**
     * Getter & Setter
     */

    std::vector<std::shared_ptr<Job>> get_execution_order_buffer();
    void set_execution_order_buffer(std::vector<std::shared_ptr<Job>>);
    void add_current_simulated_job(std::shared_ptr<Job>);
    void start_logging();
    void log_task_vector_status();
    void log_job_vector_of_each_ECU_status();
    void log_job_vector_of_simulator_status();
    void log_which_job_was_deadline_miss(std::shared_ptr<Job>);
    void print_job_execution_on_ECU(std::vector<std::shared_ptr<Job>>, std::vector<std::shared_ptr<Job>>, int);
    void print_job_execution_schedule();
    void print_offline_guider_status();
};

#endif