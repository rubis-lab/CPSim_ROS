#ifndef TASK_H__
#define TASK_H__
#include <cstdio>
#include <string.h>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

#ifdef __linux__
#include <dlfcn.h>
#elif _WIN32
// Windows code for loading dll func
#else
#error "OS not recognised."
#endif

/** This file is engine code of CPSim-Re engine
 * @file Task.h
 * @class Task
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @brief 
 *  Task's Properties
 *  A Task has below properties.
 *  1. Name
 *  2. Task ID 
 *  3. Period (Timer Callback Only)
 *  4. Deadline (Timer Callback Only)
 *  5. Offset
 * 	6. Priority (Callback Type, Registration Order)
 *  7. Callback Type (Timer Callback, Subscriber Callback)
 *  8. Fixed Execution Time (Evaluation mode)
 */

class Task
{
public:

private:
	std::string m_task_name;
	int m_task_id;
	int m_vector_idx;
	int m_period;
	int m_deadline;
	int m_offset;
	int m_priority;
	int m_callback_type;
	int m_fet; 
	int m_ecu_id;

	bool m_is_read;
	bool m_is_write;

	std::string m_producer_info;
	std::string m_consumer_info;
	std::shared_ptr<Task> m_producer;
	std::shared_ptr<Task> m_consumer;

public:
    /**
     * Constructors and Destructors
     */
	Task();
	/*
	 * name, period, deadline, offset, priority, callback_type, fixed_execution_time, isRead, isWrite, ecuId
	 */
	Task(std::string, int, int, int, int, int, int, bool, bool, int);
	/*
	 * name, period, deadline, offset, priority, callback_type, fixed_execution_time, isRead, isWrite, ecuId, producers, consumers
	 */
	Task(std::string, int, int, int, int, int, int, bool, bool, int, std::string, std::string);
	/*
	 * name, period, deadline, offset, priority, callback_type, fixed_execution_time, isRead, isWrite, ecuId, producers, consumers
	 */
	Task(std::string, int, int, int, int, int, int, bool, bool, int, std::shared_ptr<Task>, std::shared_ptr<Task>);
	~Task();

    /**
     * Getter member functions
     */

	std::string get_task_name();
	int get_task_id();
	int get_vector_idx();
	int get_period();
	int get_deadline();
	int get_offset();
	int get_priority();
	int get_callback_type();
	int get_fet();
	int get_ECU_id();
	bool get_is_read();
	bool get_is_write();

	std::shared_ptr<Task> get_producer();
	std::shared_ptr<Task> get_consumer();
	std::string get_producer_info();
	std::string get_consumer_info();

    /**
     * Setter member functions
     */
	void set_task_name(std::string);
	void set_task_id(int);
	void set_vector_idx(int);
	void set_period(int);
	void set_deadline(int);
	void set_offset(int);
	void set_priority(int);
	void set_callback_type(int);
	void set_fet(int);
	void set_is_read(bool);
	void set_is_write(bool);

	void set_producer(std::shared_ptr<Task>);
	void set_consumer(std::shared_ptr<Task>);
	void set_producer_info(std::string);
	void set_consumer_info(std::string);
	void set_ECU_id(int);

	
	void add_task_to_consumer(std::shared_ptr<Task>);
	void add_task_to_producer(std::shared_ptr<Task>);
	void synchronize_producer_consumer_relation();
};

#endif