#include "Job.h"
#include "Utils.h"
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cmath>
/** 
 *  This file is the cpp file for the Job class.
 *  @file Job.cpp
 *  @brief cpp file for Engine-Job
 *  @author Seonghyeon Park
 *  @date 2020-08-20
 *  
 */

/**
 * @fn Job::Job()
 * @brief the function of basic constructor of Job
 * @author Seonghyeon Park
 *  @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Job::Job()
{

}

/**
 * @fn Job::Job()
 * @brief the function of basic constructor of Job
 * @author Seonghyeon Park
 *  @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Job::Job(std::shared_ptr<Task> task, int job_id, int hyper_period_start)
{
    this->set_task_id(task->get_task_id());
    this->set_task_name(task->get_task_name());
    this->set_period(task->get_period());
    this->set_deadline(task->get_deadline());
    this->set_offset(task->get_offset());
    this->set_priority(task->get_priority());
    this->set_callback_type(task->get_callback_type());
    this->set_fet(task->get_fet());
    this->set_is_read(task->get_is_read());
    this->set_is_write(task->get_is_write());
    this->set_ECU(task->get_ECU());
    this->set_producer(task->get_producer());
    this->set_consumer(task->get_consumer());

    
    m_job_id = job_id;
    if(task->get_callback_type() == 0) //timer callback 
        m_real_release_time = calculate_release_time(task->get_period(), task->get_offset(), hyper_period_start);
    else
        m_real_release_time = -1;
    m_real_deadline = -1;
    m_real_start_time = -1;
    m_simulated_release_time = -1;
    m_simulated_start_time = -1;
    m_simulated_finish_time = -1;
}

/**
 * @fn Job::~Job()
 * @brief the function of basic destroyer of Job
 * @author Seonghyeon Park
 *  @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Job::~Job()
{
    
}
bool Job::get_is_started()
{
    return m_is_started;
}
bool Job::get_is_finished()
{
    return m_is_finished;
}
bool Job::get_is_released()
{
    return m_is_released;
}
bool Job::get_is_running()
{
    return m_is_running;
}
bool Job::get_is_simulated()
{
    return m_is_simulated;
}
int Job::get_job_id()
{
    return m_job_id;
}

int Job::get_real_release_time()
{
    return m_real_release_time;
}
int Job::get_real_deadline()
{
    return m_real_deadline;
}
int Job::get_real_start_time()
{
    return m_real_start_time;
}

int Job::get_real_finish_time()
{
    return m_real_finish_time;
}  

int Job::get_real_execution_time()
{
    return m_real_execution_time;
}

double Job::get_simulated_release_time()
{
    return std::floor(m_simulated_release_time*10)/10;
}
double Job::get_simulated_deadline()
{
    return std::floor(m_simulated_deadline*10)/10; 
}
double Job::get_simulated_start_time()
{
    return std::floor(m_simulated_start_time*10)/10;
}
double Job::get_simulated_finish_time()
{
    return std::floor(m_simulated_finish_time*10)/10;
}
double Job::get_simulated_execution_time()
{
    return std::floor(m_simulated_execution_time*10)/10;
}

std::array<int, 2>& Job::get_wcbp()
{
    return m_worst_case_busy_period;
}


std::vector<std::shared_ptr<Job>>& Job::get_job_set_start_det()
{
    return m_job_set_start_det;
}

std::vector<std::shared_ptr<Job>>& Job::get_job_set_start_non_det()
{
    return m_job_set_start_non_det;
}
std::vector<std::shared_ptr<Job>>& Job::get_job_set_finish_det()
{
    return m_job_set_finish_det;
}

std::vector<std::shared_ptr<Job>>& Job::get_job_set_finish_non_det()
{
    return m_job_set_finish_non_det;
}
std::vector<std::shared_ptr<Job>>& Job::get_job_set_pro_con_det()
{
    return m_job_set_pro_con_det;
}

std::vector<std::shared_ptr<Job>>& Job::get_job_set_pro_con_non_det()
{
    return m_job_set_pro_con_non_det;
}
std::vector<std::shared_ptr<Job>>& Job::get_det_prdecessors()
{
    return m_det_predecessors;
}

std::vector<std::shared_ptr<Job>>& Job::get_det_successors()
{
    return m_det_successors;
}
std::vector<std::shared_ptr<Job>>& Job::get_non_det_prdecessors()
{
    return m_non_det_predecessors;
}

std::vector<std::shared_ptr<Job>>& Job::get_non_det_successors()
{
    return m_non_det_successors;
}

void Job::set_is_started(bool is_started)
{
    m_is_started = is_started;
}
void Job::set_is_finished(bool is_finished)
{
    m_is_finished = is_finished;
}
void Job::set_is_released(bool is_released)
{
    m_is_released = is_released;
}
void Job::set_is_running(bool is_running)
{
    m_is_running = is_running;
}
void Job::set_is_simulated(bool is_simulated)
{
    m_is_simulated = is_simulated;
}

void Job::set_real_release_time(int release_time)
{
    m_real_release_time = release_time;
}
void Job::set_real_deadline(int a_deadline)
{
    m_real_deadline = a_deadline;
}

void Job::set_real_start_time(int real_start_time)
{
    m_real_start_time = real_start_time;
}
void Job::set_real_finish_time(int real_finish_time)
{
    m_real_finish_time = real_finish_time;
}
void Job::set_real_execution_time(int original_execution_time)
{
    m_real_execution_time = original_execution_time;
}

void Job::set_simulated_release_time(double simulated_release_time)
{
    m_simulated_release_time = std::floor(simulated_release_time*10)/10;
}
void Job::set_simulated_deadline(double simulated_deadline)
{
    m_simulated_deadline = std::floor(simulated_deadline*10)/10;
}
void Job::set_simulated_start_time(double simulated_start_time)
{
    m_simulated_start_time = std::floor(simulated_start_time*10)/10;
}
void Job::set_simulated_finish_time(double simulated_finish_time)
{
    m_simulated_finish_time = std::floor(simulated_finish_time*10)/10;
}
void Job::set_simulated_execution_time(double simulated_execution_time)
{
    m_simulated_execution_time = std::floor(simulated_execution_time*10)/10;
}
void Job::set_job_set_start_det(std::vector<std::shared_ptr<Job>>& job_set_start_det)
{
    m_job_set_start_det = job_set_start_det;
}
void Job::set_job_set_start_non_det(std::vector<std::shared_ptr<Job>>& job_set_start_non_det)
{
    m_job_set_start_non_det = job_set_start_non_det;
}
void Job::set_job_set_finish_det(std::vector<std::shared_ptr<Job>>& job_set_finish_det)
{
    m_job_set_finish_det = job_set_finish_det;
}
void Job::set_job_set_finish_non_det(std::vector<std::shared_ptr<Job>>& job_set_finish_non_det)
{
    m_job_set_finish_non_det = job_set_finish_non_det;
}
void Job::set_job_set_pro_con_det(std::vector<std::shared_ptr<Job>>& job_set_pro_con_det)
{
    m_job_set_pro_con_det = job_set_pro_con_det;
}
void Job::set_job_set_pro_con_non_det(std::vector<std::shared_ptr<Job>>& job_set_pro_con_non_det)
{
    m_job_set_pro_con_non_det = job_set_pro_con_non_det;
}
void Job::set_det_predecessors(std::vector<std::shared_ptr<Job>>& predecessors)
{
    m_det_predecessors = predecessors;
}
void Job::set_det_successors(std::vector<std::shared_ptr<Job>>& successors)
{
    m_det_successors = successors;
}
void Job::set_non_det_predecessors(std::vector<std::shared_ptr<Job>>& predecessors)
{
    m_non_det_predecessors = predecessors;
}
void Job::set_non_det_successors(std::vector<std::shared_ptr<Job>>& successors)
{
    m_non_det_successors = successors;
}


int Job::calculate_release_time(int period, int offset, int hyper_period_start)
{
    /**
     * n-th Job release time can be calculated with [ period * (n-1) + task offset ]
     */
    return (period * (m_job_id - 1)) + offset + hyper_period_start;
}

int Job::calculate_absolute_deadline(int release_time, int r_deadline)
{
    return release_time + r_deadline;
}

void Job::initialize_simulated_deadline()
{
    if(this->get_is_write())
    {
        m_simulated_deadline = static_cast<double>(m_real_finish_time);
        if(m_simulated_deadline == 0)
        {
            //std::cout << "WE GOT A ZERO VALUE DEADLINE INSIDE THE INITIALIZE SIMULATED DEADLINE FUNCTION!" << std::endl;
        }
    }
    else
    {
        m_simulated_deadline = INT_MAX;
    }
}
void Job::update_simulated_deadline()
{
    if(m_is_simulated == false || m_is_released == false)
    {
        if(this->get_is_write())
        {
            
            m_simulated_deadline = static_cast<double>(m_real_finish_time);
            if(m_simulated_deadline == 0)
            {
                //std::cout << "WE GOT A ZERO VALUE DEADLINE INSIDE THE UPDATE SIMULATED DEADLINE FUNCTION!" << std::endl;
            }
            else
            {
                //std::cout << "WE GOT A NON ZERO VALUE INSIDE THE UPDATE SIMULATED DEADLINE FUNCTION!" << std::endl;
            }
        }
        else
        {
            m_simulated_deadline = min_simulated_deadline_det_successor();
        }
    }
}
double Job::min_simulated_deadline_det_successor()
{
    double min_value = INT_MAX;
    if(!m_det_successors.empty())
    {
        std::shared_ptr<Job> min_succ = m_det_successors.front();
        for(auto succ : m_det_successors)
        {
            if(succ->get_simulated_deadline() < min_value)
            {
                min_value = succ->get_simulated_deadline();
                min_succ = succ;
            }
        }
        add_history(min_succ);
        return min_value;
    }
    else
    {
        if(min_value == INT_MAX)
            return min_value;
    }
    std::cout << "FATAL ERROR" << std::endl;
    std::cin >> min_value;
} 

std::vector<std::shared_ptr<Job>> Job::get_history()
{
    return m_history_of_sim_deadline;
}

void Job::add_history(std::shared_ptr<Job> new_deadline)
{
    m_history_of_sim_deadline.push_back(new_deadline);
}