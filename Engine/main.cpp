/** This file is engine code of CPSim-ROS2
 * @file main.cpp
 * @brief Codes for Engine-Main
 * @page Simulator
 * @author Seonghyeon Park
 * @date 2020-08-18
 * @section Simulator Logic
 *  Abstraction for Main Code Logic is as below algorithm,
 *  1. Execute Initializer
 *  2. Check all the informations for running simulator
 *  3. Execute Schedule Simulator
 *  4. Execute Offline Guider
 *  5. Execute Online Progressive Scheduling
 *  6. Execute Executor
 *  7. Execute OPS Updater
 *  8. Repeat Execution
 *
*/

/**
    Header file lists.. in UNIX standard library
*/
#include <memory>
#include <stdio.h>
#include <random>
#include <stdlib.h>

/**
    Header file lists.. in our simulator
*/
#include "ScheduleGenerator.h"
#include "Initializer.h"
#include "OfflineGuider.h"
#include "Executor.h"
#include "Utils.h"
#define CANMODE
#define EPSILON 0.000001
#define ABS(X) ((X) < 0 ? -(X) : (X))

/**
    @fn main(void)
    @brief this is main code of engine.
    @return none
    @param none

*/

int main(int argc, char *argv[])
{
    /**
     * SRAND FUNCTION IS FOR RANDOM TASK GENERATION : SYNTHETIC WORKLOAD MODE 
     */
    srand((unsigned int)time(NULL)); // only ever seed once, not in func that is executed many times
    /**
     * SIGNAL FUNCTION IS FOR SIG_KILL : SIGNAL HANDLER
     */
    signal(SIGINT, utils::exit_simulation);
    utils::transaction_factor = 0.5;
    utils::read_factor = 0.3;
    utils::write_factor = 0.0;
    while(utils::write_factor <= 1.0)
    {
        /**
         * SYNTHETIC WORKLOAD SIMULATION OPTIONS
         */
        int epochs = 100;
        int ours_simulatable_count = 0;
        int all_sync_simulatable_count = 0;
        int true_time_simulatable_count = 0;
        int ours_non_simulatable_count = 0;
        int all_sync_non_simulatable_count = 0;
        int true_time_non_simulatable_count = 0;

        int iter = 0;
        while(iter < epochs)
        {
            /** [Initialization with Specification]
             */
            //std::cout << "EPOCH : " << i << std::endl;
            Initializer initializer_module;
            ScheduleGenerator schedule_generator;
            OfflineGuider offline_guider;
            Executor executor_ours;
            Executor executor_all_sync;
            Executor executor_true_time;

            initializer_module.initialize();
            bool is_simulatable = true;
            bool is_schedulable = schedule_generator.generate_schedule_offline();
            if(is_schedulable == true)
            {
                //std::cout << iter << std::endl;
                iter++;
            }
            else
            {
                for(auto ecu : vectors::ecu_vector)
                {
                    ecu->clear_every_jobset();
                }
                vectors::ecu_vector.clear();
                vectors::job_precedence_graph.clear();
                vectors::released_set.clear();
                vectors::subscriber_vector.clear();
                vectors::timer_vector.clear();
                vectors::task_vector.clear();
                for(auto transaction : vectors::transaction_vector)
                {
                    transaction.clear();
                }
                vectors::transaction_vector.clear();
                utils::current_time = 0;
                continue;
            }
            global_object::logger->print_job_execution_on_ECU();
            offline_guider.construct_job_precedence_graph();

            executor_ours.set_simulator_scheduler_mode(0); // EDF Scheduling Mode
            executor_ours.assign_deadline_for_simulated_jobs();
            while((utils::current_time <= utils::hyper_period) && is_simulatable) // we are going to run simulation with two hyper period times. 
            {
                // OURS
                executor_ours.check_job_precedence_graph();
                is_simulatable = executor_ours.run_simulation();                      // run a job on the simulator
                schedule_generator.generate_schedule_online();  // when a job finished, then generate a new job. attach that job to the vector
                offline_guider.update_job_precedence_graph();   // update job_precedence_graph
                utils::current_time = utils::current_time + 0.1;
            }
            if(is_simulatable == true)
            {
                ours_simulatable_count ++;
            }
            else
            {
                ours_non_simulatable_count++;
            }
            
            is_simulatable = true;
            utils::current_time = 0;
            vectors::job_precedence_graph.clear();
            for(auto ecu : vectors::ecu_vector)
            {
                ecu->clear_every_jobset();
            }
            schedule_generator.generate_schedule_offline();
            offline_guider.construct_job_precedence_graph();
            executor_all_sync.set_simulator_scheduler_mode(1); // ALL-SYNC
            executor_all_sync.assign_deadline_for_simulated_jobs();

            //std::cout << std::endl << "ALL SYNC"<< std::endl;
            //utils::cnt = 0;
            while((utils::current_time <= utils::hyper_period) && is_simulatable) // we are going to run simulation with two hyper period times. 
            {
                // ALL-SYNC
                executor_all_sync.check_job_precedence_graph();
                is_simulatable = executor_all_sync.run_simulation();                      // run a job on the simulator
                schedule_generator.generate_schedule_online();  // when a job finished, then generate a new job. attach that job to the vector
                offline_guider.update_job_precedence_graph();   // update job_precedence_graph
                utils::current_time = utils::current_time + 0.1;
            }
            //std::cout << utils::cnt << std::endl;           
            if(is_simulatable == true)
            {
                all_sync_simulatable_count ++;
            }
            else
            {
                all_sync_non_simulatable_count++;
            }
 
            is_simulatable = true;
            utils::current_time = 0;
            vectors::job_precedence_graph.clear();
            for(auto ecu : vectors::ecu_vector)
            {
                ecu->clear_every_jobset();
            }
            schedule_generator.generate_schedule_offline();
            offline_guider.construct_job_precedence_graph();
            executor_true_time.set_simulator_scheduler_mode(2); // True time Scheduling Mode
            executor_true_time.assign_deadline_for_simulated_jobs();
            //std::cout << std::endl << "TRUE TIME "<< std::endl;
            while((utils::current_time <= utils::hyper_period) && is_simulatable) // we are going to run simulation with two hyper period times. 
            {
                // True Time
                executor_true_time.check_job_precedence_graph();
                is_simulatable = executor_true_time.run_simulation();                      // run a job on the simulator
                schedule_generator.generate_schedule_online();  // when a job finished, then generate a new job. attach that job to the vector
                offline_guider.update_job_precedence_graph();   // update job_precedence_graph
                utils::current_time = utils::current_time + 0.1;
            }
   
            
            //if(utils::all_sync_cnt != utils::true_time_cnt)
            //    std::cout << utils::all_sync_cnt << ", " << utils::true_time_cnt << std::endl;
            utils::true_time_cnt = 0;
            utils::all_sync_cnt = 0;
            if(is_simulatable == true)
            {
                true_time_simulatable_count ++;
            }
            else
            {
                true_time_non_simulatable_count++;
            }
            for(auto ecu : vectors::ecu_vector)
            {
                ecu->clear_every_jobset();
            }
            vectors::ecu_vector.clear();
            vectors::job_precedence_graph.clear();
            vectors::released_set.clear();
            vectors::subscriber_vector.clear();
            vectors::timer_vector.clear();
            vectors::task_vector.clear();
            for(auto transaction : vectors::transaction_vector)
            {
                transaction.clear();
            }
            vectors::transaction_vector.clear();
            utils::current_time = 0;
        }

        // PRINT THE INFORMATIONS
        std::cout << std::endl;
        // std::cout << "--------------------" << std::endl;
        // std::cout << ours_simulatable_count << " OURS simulations were simulatable." << std::endl;
        // std::cout << ours_non_simulatable_count << " OURS simulations were non-simulatable." << std::endl;
        // std::cout << all_sync_simulatable_count << " ALL-SYNC simulations were simulatable." << std::endl;
        // std::cout << all_sync_non_simulatable_count << " ALL-SYNC simulations were non-simulatable." << std::endl;
        // std::cout << true_time_simulatable_count << " TRUE TIME simulations were simulatable." << std::endl;
        // std::cout << true_time_non_simulatable_count << " TRUE TIME simulations were non-simulatable." << std::endl;
        
        std::cout << utils::write_factor << std::endl;
        std::cout << "OURS " << ours_simulatable_count / (double)(ours_simulatable_count + ours_non_simulatable_count) << std::endl;
        std::cout << "ALL-SYNC " << all_sync_simulatable_count / (double)(all_sync_simulatable_count + all_sync_non_simulatable_count) << std::endl;
        std::cout << "TRUE-TIME " << true_time_simulatable_count / (double)(true_time_simulatable_count + true_time_non_simulatable_count) << std::endl;
        std::cout << "--------------------" << std::endl;
        utils::write_factor = utils::write_factor + 0.1;
    }
    return 0;
}