#ifndef ECU_H__
#define ECU_H__
#include <string>
#include "Utils.h"

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
    std::shared_ptr<Job> who_is_running;
    std::shared_ptr<Job> who_is_next;
    std::vector<std::shared_ptr<Job>> ecu_ready_set;
    std::vector<std::shared_ptr<Task>> ecu_task_set;

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
    std::string get_scheduling_policy();
    std::vector<Job> get_ecu_ready_set();

    /**
     * Setter member functions
     */
    void set_ECU_id(int);
    void set_performance(int);
    void set_num_of_task(int);
    void set_scheduling_policy(std::string);
    void set_ecu_ready_set();
};
#endif