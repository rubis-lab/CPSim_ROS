#ifndef SCHEDULESIMULATOR_H__
#define SCHEDULESIMULATOR_H__

#include "Utils.h"
#include <ctime>
#include <chrono>

/** This file is engine code of CPSim-Re engine
 * @file ScheduleSimulator.h
 * @class ScheduleSimulator
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @brief Codes for Engine-ScheduleSimulator 
*/
class ScheduleGenerator {
private:
    int m_hyper_period;
    int m_g_busy_period_start_time;
    int m_g_busy_period_finish_time;
    std::vector<std::shared_ptr<Job>> m_execution_order_b;
    std::vector<std::shared_ptr<Job>> m_execution_order_w;
    bool m_is_offline;
    bool m_is_globally_busy;

    std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();

public:
    /**
     * Constructors and Destructors
     */
    ScheduleGenerator();
    ~ScheduleGenerator();

    /**
     * Getter member functions
     */
    bool get_is_globally_busy();

    /**
     * Setter member functions
     */
    void set_hyper_period();
    void set_is_globally_busy(bool);

    /**
     * Schedule Generator for real cyber system
     * Generate Real Cyber System Schedule Globally with a Hyper Period unit.
     */
    void generate_schedule_offline();
    void generate_schedule_online();
    void generate_job_instances_for_hyper_period();
    void generate_a_job();
    void busy_period_analysis(std::vector<std::shared_ptr<Job>>& job_queue, int start, int& end, int ecu_id, bool setWorstCase);
};

#endif