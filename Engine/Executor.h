#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include <memory>
#include "Job.h"

/** This file is engine code of CPSim-Re engine
 * @file Executor.h
 * @class Executor
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @brief Codes for Engine-Executor
 *  
*/

class Executor
{
private:
    int m_current_hyper_period_index;
    int m_current_hyper_period_start;
    int m_current_hyper_period_end;
    int m_simulator_scheduler_mode; // 0 : OURS, 1 : ALL-SYNC , 2 : TRUE TIME
    bool is_busy;
    std::shared_ptr<Job> who_is_running;
    std::vector<std::shared_ptr<Job>> m_simulation_ready_queue;
    std::vector<std::shared_ptr<Job>> m_ready_set;
    std::vector<std::shared_ptr<Job>> m_pending_set;
    std::vector<std::shared_ptr<Job>> m_job_order;

public:
    /**
     * Constructor & Destructor
     */
    Executor();
    ~Executor();
    
    /**
     * Getter & Setter
     */
    int get_current_hyper_period_index();
    int get_current_hyper_period_start();
    int get_current_hyper_period_end();
    int get_simulator_scheduler_mode();
    std::vector<std::shared_ptr<Job>> get_job_order();
    bool get_is_busy();

    void set_current_hyper_period_index(int); 
    void set_current_hyper_period_start(int);
    void set_current_hyper_period_end(int);
    void set_simulator_scheduler_mode(int);

    /**
     * Simulation Member Functions
     */
    void random_execution_time_generator();
    void update_jobset(std::shared_ptr<Job>);
    bool run_simulation();
    
    void check_job_precedence_graph();
    void check_ros2_ready_set();
    void update_simulation_pending_set();
    void delete_job_from_simulation_pending_set(std::shared_ptr<Job>);
    void delete_job_from_job_precedence_graph(std::shared_ptr<Job>);
    void delete_job_from_released_set(std::shared_ptr<Job>);
    void delete_job_from_simulation_ready_queue(std::shared_ptr<Job>);
    void delete_job_from_simulation_ready_set(std::shared_ptr<Job>);
    void assign_deadline_for_simulated_jobs();
    void assign_deadline_for_simulated_jobs_ros2();
    void assign_predecessors_successors();
    bool simulatability_analysis();
    void clear_simulation_ready_queue();
};

#endif
