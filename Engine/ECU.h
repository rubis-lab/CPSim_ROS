#ifndef ECU_H__
#define ECU_H__
#include <string>
#include "Job.h"

/**
 *  @file ECU.h
 *  @class ECU
 *  @author Seonghyeon Park
 *  @date 2020-08-18
 *  @brief class for Engine-ECU
 *  the properties of ECU
 *  - ECU id
 *  - Performance (Unit : MHz)
 *  - SchedulingPolicy (e.g., RM, EDF, etc.)
 */
class ECU
{
private:
    std::string m_scheduling_policy;
    int m_performance;
    int m_ecu_id;
    int m_num_of_task;
    int m_l_busy_period_start_time;
    int m_l_busy_period_finish_time;
    bool m_is_busy;
    std::shared_ptr<Job> who_is_running;
    std::shared_ptr<Job> who_is_next;
    std::vector<std::shared_ptr<Job>> m_ready_set;
    std::vector<std::shared_ptr<Job>> m_released_jobset;
    std::vector<std::shared_ptr<Job>> m_pending_jobset;
    std::vector<std::shared_ptr<Job>> m_finished_jobset;
    std::vector<std::shared_ptr<Task>> m_task_set;


public:
    /**
     * Constructors and Destructors
     */
    ECU();
    ECU(int, std::string);
    ECU(int, std::string,int ecu_id);
    ~ECU();

    /**
     * Getter member functions
     */
    int get_ECU_id();
    int get_performance();
    int get_num_of_task();
    int get_l_busy_period_start_time();
    int get_l_busy_period_finish_time();
    bool get_is_busy();
    std::string get_scheduling_policy();
    std::shared_ptr<Job> get_who_is_running();
    std::shared_ptr<Job> get_who_is_next();
    std::vector<std::shared_ptr<Job>> get_ecu_ready_set();
    std::vector<std::shared_ptr<Job>> get_released_jobset();
    std::vector<std::shared_ptr<Job>> get_pending_jobset();
    std::vector<std::shared_ptr<Job>> get_finished_jobset();
    std::vector<std::shared_ptr<Task>> get_task_set();

    /**
     * Setter member functions
     */
    void set_ECU_id(int);
    void set_performance(int);
    void set_num_of_task(int);
    void set_l_busy_period_start_time(int);
    void set_l_busy_period_finish_time(int);
    void set_is_busy(bool);
    void set_scheduling_policy(std::string);
    void set_who_is_running(std::shared_ptr<Job>);
    void set_who_is_next(std::shared_ptr<Job>);
    void set_ecu_ready_set(std::vector<std::shared_ptr<Job>>);
    void set_released_jobset(std::vector<std::shared_ptr<Job>>);
    void set_pendig_jobset(std::vector<std::shared_ptr<Job>>);
    void set_finished_jobset(std::vector<std::shared_ptr<Job>>);
    void set_task_set(std::vector<std::shared_ptr<Task>>);


    void add_task_to_ecu(std::shared_ptr<Task>);
    void add_job_to_ready_set(std::shared_ptr<Job>);
    void add_job_to_released_jobset(std::shared_ptr<Job>);
    void add_job_to_pending_jobset(std::shared_ptr<Job>);
    void add_job_to_finished_jobset(std::shared_ptr<Job>);

    void delete_job_from_released_jobset(std::shared_ptr<Job>);
    void delete_job_from_ready_set(std::shared_ptr<Job>);
    void delete_job_from_finished_jobset(std::shared_ptr<Job>);
    void copy_pending_jobset_to_ready_set();
    void flush_pending_jobset(); 
};
#endif