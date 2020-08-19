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
    std::vector<std::shared_ptr<Job>> m_execution_order_b;
    std::vector<std::shared_ptr<Job>> m_execution_order_w;
    bool m_is_offline;

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

    /**
     * Setter member functions
     */
    void set_hyper_period();

    /**
     * Schedule Generator for real cyber system
     */
    void generate_real_cyber_system_schedule();
    void simulate_scheduling_on_real(double);
    void update_job_vector();
    void busy_period_analysis(std::vector<std::shared_ptr<Job>>& job_queue, int start, int& end, int ecu_id, bool setWorstCase);
};

#endif