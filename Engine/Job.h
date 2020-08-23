#ifndef JOB_H__
#define JOB_H__

#include "Task.h"
#include <memory>
#include <map>
#include <iostream>

/** 
 *  This file is the header file for the Job class.
 *  @file Job.h
 *  @author Seonghyeon Park
 *  @date 2020-08-19
 *  @class Job
 *  @brief Header file for Engine-Job
 *  A job is the instance of certain task.
 *  So it has dynamical properties with the task.
 *  Job can have below properties.
 *  1. Name, of the task which is instantiated.
 *  2. Release time,
 *  3. Deadline,
 *  4. Start time ranges[EST, LST]
 *  5. Finish time ranges[EFT, LFT]
 *  6. Priority
 */

class Job : public Task
{
private: 
    // In real-cyber system
    bool m_is_real_released;
    bool m_is_real_running;
    bool m_is_real_started;
    bool m_is_real_finished;
    // In simulated-cyber system
    bool m_is_simulated_released;
    bool m_is_simulated_running;
    bool m_is_simulated_started;
    bool m_is_simulated_finished;

    int m_job_id;
    int m_real_release_time;
    int m_real_deadline;
    int m_real_execution_time;
    int m_real_start_time;
    int m_real_finish_time;
    int m_real_busy_period_start_time;
    int m_real_busy_period_finish_time;

    double m_simulated_release_time;
    double m_simulated_deadline;
    double m_simulated_execution_time;
    double m_simulated_start_time;
    double m_simulated_finish_time;

    std::vector<std::shared_ptr<Job>> m_history_of_sim_deadline;
    std::vector<std::shared_ptr<Job>> m_det_predecessors;
    std::vector<std::shared_ptr<Job>> m_det_successors;
    std::shared_ptr<Job> m_producer_job;
    std::shared_ptr<Job> m_consumer_job;
    

public:
    /**
     * Constructor & Destructor
     */
    Job();
    Job(std::shared_ptr<Task>, int, int);
    Job(std::shared_ptr<Task>, std::shared_ptr<Job>);
    ~Job();
    /**
     * Getter & Setter
     */
    bool get_is_real_started();
    bool get_is_real_finished();
    bool get_is_real_released();
    bool get_is_real_running();

    bool get_is_simulated_released();
    bool get_is_simulated_running();
    bool get_is_simulated_started();
    bool get_is_simulated_finished();

    int get_job_id();
    int get_real_release_time();
    int get_real_deadline();
    int get_real_start_time();
    int get_real_finish_time();
    int get_real_execution_time();
    int get_real_busy_period_start_time();
    int get_real_busy_period_finish_time();

    double get_simulated_release_time();
    double get_simulated_deadline();
    double get_simulated_start_time();
    double get_simulated_finish_time();
    double get_simulated_execution_time();
    std::vector<std::shared_ptr<Job>> get_history();

    std::shared_ptr<Job> get_producer_job();
    std::shared_ptr<Job> get_consumer_job();
    std::vector<std::shared_ptr<Job>>& get_job_set_pro_con_det();
    std::vector<std::shared_ptr<Job>>& get_det_prdecessors();
    std::vector<std::shared_ptr<Job>>& get_det_successors();


    void set_is_real_started(bool);
    void set_is_real_finished(bool);
    void set_is_real_released(bool);
    void set_is_real_running(bool);
    
    void set_is_simulated_released(bool);
    void set_is_simulated_running(bool);
    void set_is_simulated_started(bool);
    void set_is_simulated_finished(bool);
    
    void set_job_id(int);
    void set_real_release_time(int);
    void set_real_deadline(int);
    void set_real_start_time(int);
    void set_real_finish_time(int);
    void set_real_execution_time(int);

    void set_simulated_release_time(double);
    void set_simulated_deadline(double);
    void set_simulated_start_time(double);
    void set_simulated_finish_time(double);
    void set_simulated_execution_time(double);
    
    void set_real_busy_period_start_time(int);
    void set_real_busy_period_finish_time(int);
    
    void set_producer_job(std::shared_ptr<Job>);
    void set_consumer_job(std::shared_ptr<Job>);
    void set_job_set_pro_con_det(std::vector<std::shared_ptr<Job>>&);
    void set_det_predecessors(std::vector<std::shared_ptr<Job>>&);
    void set_det_successors(std::vector<std::shared_ptr<Job>>&);

    /**
     * SPECIFIC FUNCTIONS OF JOB CLASS
     */
    int calculate_release_time(int, int, int);
    int calculate_absolute_deadline(int, int);
    void initialize_simulated_deadline();
    void update_simulated_deadline();
    double min_simulated_deadline_det_successor();
    void add_history(std::shared_ptr<Job>);
    void add_job_to_successors(std::shared_ptr<Job>);
    void add_job_to_predecessors(std::shared_ptr<Job>);
    void delete_job_from_successors(std::shared_ptr<Job>);
    void delete_job_from_predecessors(std::shared_ptr<Job>);
    void copy_successor_successors(std::shared_ptr<Job>);
};

#endif