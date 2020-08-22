#include "Task.h"
#include "Utils.h"
#include <fstream>

/**
 *  This file is the cpp file for the Task class.
 *  @file Task.cpp
 *  @brief cpp file for Engine-Task
 *  @author Seonghyeon Park
 *  @date 2020-08-19
 */

/**
 * @fn Task::Task()
 * @brief the function of basic constructor of Task
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
Task::Task()
{
    
}

/**
 * @fn void Task::Task()
 * @brief the function of fundemental informed constructor of Task
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @details 
 *  This constructor has all properties that define each tasks in ECU. 
 * @param task_name
 * @param task_id
 * @param period
 * @param deadline
 * @param offset
 * @param priority
 * @param callback_type
 * @param fixed_execution_time
 * @param is_read
 * @param is_write
 * @param producer
 * @param consumer
 * @return Task
 * @bug none
 * @warning none
 * @todo none
 */
Task::Task(std::string task_name, int period, int deadline, int priority, int callback_type,
            int fet, int offset, bool is_read, bool is_write, int ecu_id)
{
    /**
     * Member variable initializaion
     */
    m_task_name = task_name;
    m_task_id = vectors::task_vector.size();
    m_period = period;
    m_deadline = deadline;
    m_offset = offset;
    m_priority = priority;
    m_callback_type = callback_type;
    m_fet = fet;
    m_is_read = is_read;
    m_is_write = is_write;
    m_ecu_id = ecu_id;
}

Task::Task(std::string task_name, int period, int deadline, int priority, int callback_type,
            int fet, int offset, bool is_read, bool is_write, int ecu_id,
            std::string prodcuer, std::string consumer)
{
    /**
     * Member variable initializaion
     */
    m_task_name = task_name;
    m_task_id = vectors::task_vector.size();
    //m_task_id = std::stoi(m_task_name.substr(4));
    m_period = period;
    m_deadline = deadline;
    m_offset = offset;
    m_is_read = is_read;
    m_is_write = is_write;
    m_producer_info = prodcuer;
    m_consumer_info = consumer;
    m_ecu_id = ecu_id;
}

Task::Task(std::string task_name, int period, int deadline, int priority, int callback_type,
            int fet, int offset, bool is_read, bool is_write, int ecu_id,
            std::shared_ptr<Task> prodcuer, std::shared_ptr<Task> consumer)
{
    /**
     * Member variable initializaion
     */
    m_task_name = task_name;
    m_task_id = vectors::task_vector.size();
    m_period = period;
    m_deadline = deadline;
    m_offset = offset;
    m_is_read = is_read;
    m_is_write = is_write;
    m_producer = prodcuer;
    m_consumer = consumer;
    m_ecu_id = ecu_id;
}

/**
 * @fn Task::~Task()
 * @brief the function of basic destructor of Task
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Task::~Task()
{

}

/**
 * @fn std::string get_task_name()
 * @brief Getter for task name
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return Task name
 * @bug none
 * @warning none
 * @todo none
 */
std::string Task::get_task_name()
{
    return m_task_name;
}

/**
 * @fn int get_task_id()
 * @brief Getter for task ID
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return Task ID
 * @bug none
 * @warning none
 * @todo none
 */
int Task::get_task_id()
{
    return m_task_id;
}
int Task::get_vector_idx()
{
    return m_vector_idx;
}

/**
 * @fn int get_period()
 * @brief Getter for task period
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return Task Period
 * @bug none
 * @warning none
 * @todo none
 */
int Task::get_period()
{
    return m_period;
}

/**
 * @fn int get_deadline()
 * @brief Getter for task deadline
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return Task Deadline
 * @bug none
 * @warning none
 * @todo none
 */
int Task::get_deadline()
{
    return m_deadline;
}

/**
 * @fn int get_offset()
 * @brief Getter for task's offset
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return Task offset
 * @bug none
 * @warning none
 * @todo none
 */
int Task::get_offset()
{
    return m_offset;
}

int Task::get_priority()
{
    return m_priority;    
}

int Task::get_callback_type()
{
    return m_callback_type;    
}

int Task::get_fet()
{
    return m_fet;    
}
/**
 * @fn int get_is_read()
 * @brief Getter for task's read constraint
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return Task read constraint
 * @bug none
 * @warning none
 * @todo none
 */
bool Task::get_is_read()
{
    return m_is_read;
}

/**
 * @fn int get_is_write()
 * @brief Getter for task's write constraint
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  - None
 * @param none
 * @return Task write constraint
 * @bug none
 * @warning none
 * @todo none
 */
bool Task::get_is_write()
{
    return m_is_write;
}

int Task::get_ECU_id()
{
    return m_ecu_id;
}

std::shared_ptr<Task> Task::get_producer()
{
    return m_producer;
}

std::shared_ptr<Task> Task::get_consumer()
{
    return m_consumer;
}
std::string Task::get_producer_info()
{
    return m_producer_info;
}
std::string Task::get_consumer_info()
{
    return m_consumer_info;
}

/**
 * Setter member functions
 */

void Task::set_task_name(std::string task_name)
{
    m_task_name = task_name;
}

void Task::set_task_id(int task_id)
{
    m_task_id = task_id;
}
void Task::set_vector_idx(int vector_idx)
{
    m_vector_idx = vector_idx;
}
void Task::set_period(int period)
{
    m_period = period;
}

void Task::set_deadline(int deadline)
{
    m_deadline = deadline;
}

void Task::set_offset(int offset)
{
    m_offset = offset;
}
void Task::set_is_read(bool is_read)
{
    m_is_read = is_read;
}
void Task::set_is_write(bool is_write)
{
    m_is_write = is_write;
}
void Task::set_priority(int priority)
{
    m_priority = priority;
}
void Task::set_callback_type(int type)
{
    m_callback_type = type;
}
void Task::set_fet(int fet)
{
    m_fet = fet;
}

void Task::set_producer(std::shared_ptr<Task> producer)
{
    m_producer = producer;
}
void Task::set_consumer(std::shared_ptr<Task> consumer)
{
    m_consumer = consumer;
}
void Task::set_producer_info(std::string producer_info)
{
    m_producer_info = producer_info;
}
void Task::set_consumer_info(std::string consumer_info)
{
    m_consumer_info = consumer_info;
}
void Task::set_ECU_id(int ecu_id)
{
    m_ecu_id = ecu_id;
}

void Task::synchronize_producer_consumer_relation()
{
    for(auto task : vectors::task_vector)
        if(task->get_task_name() == m_producer_info)
        {
            m_producer = task;
        } 
        

    for(auto task : vectors::task_vector)
        if(task->get_task_name() == m_consumer_info)
        {       
            m_consumer = task;
            break; 
        }
}

void Task::add_task_to_consumer(std::shared_ptr<Task> task)
{
    bool is_overlapped = false;

    if(m_consumer == task)
    {
        is_overlapped = true;
    }
    
    if(is_overlapped == false)
        m_consumer = task;
}
void Task::add_task_to_producer(std::shared_ptr<Task> task)
{
    bool is_overlapped = false;
    
    if(m_producer == task)
    {
        is_overlapped = true;
    }
    
    if(is_overlapped == false)
        m_producer = task;
}