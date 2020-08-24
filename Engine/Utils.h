#ifndef UTILS_H__
#define UTILS_H__

#include <numeric>
#include <vector>
#include <cstdarg>
#include <iostream>
#include <memory>
#include <signal.h>
#include <thread>
#include <algorithm>
#include <queue>

#include "Task.h"
#include "Job.h"
#include "ECU.h"
#include "Logger.h"

/** This file is engine code of CPSim-Re engine
 * @file RUBIS_Util.h
 * @class 
 * @author Seonghyeon Park
 * @date 2020-08-18
 * @brief Codes for Engine-RUBISUtil
 * This is for utilities of our laboratoy.
 * This functions are based on inline styles on C++ which is extern style of C . 
*/

namespace utils
{
    inline int hyper_period = 0;
    inline double current_time = 0; //simulation time(us)
    inline int number_of_ECUs = 0;
    inline int number_of_tasks = 0;
     
    inline double simple_mapping_function = 0.3;

    inline bool real_workload = false; 

    int greatest_common_divider(int a, int b);
    int least_common_multiple(int a, int b);
    int least_common_multiple_array(std::vector<std::shared_ptr<Task>>& task_set);
    int calculate_hyper_period(std::vector<std::shared_ptr<Task>>& task_set);
    bool compare(std::shared_ptr<Job> pred, std::shared_ptr<Job> succ);
    bool first_release(std::shared_ptr<Job> pred, std::shared_ptr<Job> succ);
    void exit_simulation(int signo);
}

namespace vectors
{
    inline std::vector<std::shared_ptr<ECU>> ecu_vector;
    inline std::vector<std::shared_ptr<Task>> task_vector;
    inline std::vector<std::shared_ptr<Task>> timer_vector;
    inline std::vector<std::shared_ptr<Task>> subscriber_vector;
    inline std::vector<std::vector<std::shared_ptr<Task>>> transaction_vector;
    inline std::vector<std::shared_ptr<Job>> released_set;
    inline std::vector<std::shared_ptr<Job>> job_precedence_graph;
}

namespace global_object
{
    inline std::shared_ptr<Job> running_job;
    inline std::priority_queue<std::shared_ptr<Job>> finished_job;
    inline std::shared_ptr<Logger> logger;

    typedef struct LogData
    {
        int est;
        int lst;
        int eft;
        int lft;
        int act_rel;
        int act_start;
        int wcbp_start;
        double sim_deadline;
        double sim_release;
        double sim_start;
        double sim_finish;
    } LogData;
    inline LogData gld;
    inline std::vector<LogData> gld_vector;
    // std::to_string(highest_job->get_est()) + ", ECU" + std::to_string(highest_job->get_ECU()->get_ECU_id()) + ": " + highest_job->get_task_name() + ", 1\n";
    // std::to_string(highest_job->get_eft()) + ", ECU" + std::to_string(highest_job->get_ECU()->get_ECU_id()) + ": " + highest_job->get_task_name() + ", 0\n";

}

#endif