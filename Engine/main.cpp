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
#include <unistd.h>
#include <dlfcn.h>
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
        int epochs = 1000;
        int edf_simulatable_count = 0;
        int ros2_simulatable_count = 0;
        int edf_non_simulatable_count = 0;
        int ros2_non_simulatable_count = 0;

        int iter = 0;
        while(iter < epochs)
        {
            /** [Initialization with Specification]
             */
            //std::cout << "EPOCH : " << i << std::endl;
            Initializer initializer_module;
            ScheduleGenerator schedule_generator;
            OfflineGuider offline_guider;
            Executor executor;
            
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
            
            offline_guider.construct_job_precedence_graph();

            executor.set_simulator_scheduler_mode(0); // EDF Scheduling Mode
            executor.assign_deadline_for_simulated_jobs();
            while((utils::current_time <= utils::hyper_period) && is_simulatable) // we are going to run simulation with two hyper period times. 
            {
                // EDF
                executor.check_job_precedence_graph();
                is_simulatable = executor.run_simulation();                      // run a job on the simulator
                schedule_generator.generate_schedule_online();  // when a job finished, then generate a new job. attach that job to the vector
                offline_guider.update_job_precedence_graph();   // update job_precedence_graph
                utils::current_time = utils::current_time + 0.1;
            }
            if(is_simulatable == true)
            {
                edf_simulatable_count ++;
            }
            else
            {
                edf_non_simulatable_count++;
            }
            
            is_simulatable = true;
            utils::current_time = 0;
            vectors::job_precedence_graph.clear();
            for(auto ecu : vectors::ecu_vector)
            {
                ecu->clear_every_jobset();
            }
            schedule_generator.generate_schedule_offline();
            executor.set_simulator_scheduler_mode(1); // ROS2 Scheduling Mode
            for(auto ecu : vectors::ecu_vector)
            {
                for(auto job : ecu->get_finished_jobset())
                {
                    vectors::released_set.push_back(job);
                }
            }
            executor.assign_deadline_for_simulated_jobs_ros2();
            while((utils::current_time <= utils::hyper_period) && is_simulatable) // we are going to run simulation with two hyper period times. 
            {
                // ROS2 EXECUTOR
                executor.check_ros2_ready_set();
                is_simulatable = executor.run_simulation();          
                executor.check_ros2_ready_set();            // run a job on the simulator
                schedule_generator.generate_schedule_online();  // when a job finished, then generate a new job. attach that job to the vector
                offline_guider.update_job_precedence_graph();   // update job_precedence_graph
                utils::current_time = utils::current_time + 0.1;
            }
            if(is_simulatable == true)
            {
                ros2_simulatable_count ++;
            }
            else
            {
                ros2_non_simulatable_count++;
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
            std::cout << std::endl;
            std::cout << "--------------------" << std::endl;
            std::cout << edf_simulatable_count << " edf simulations were simulatable." << std::endl;
            std::cout << edf_non_simulatable_count << " edf simulations were non-simulatable." << std::endl;
            std::cout << ros2_simulatable_count << " simulations were simulatable." << std::endl;
            std::cout << ros2_non_simulatable_count << " simulations were non-simulatable." << std::endl;
            
            std::cout << "Simulatability ratio is " << edf_simulatable_count / (double)(edf_simulatable_count + edf_non_simulatable_count) << std::endl;
            std::cout << "Simulatability ratio is " << ros2_simulatable_count / (double)(ros2_simulatable_count + ros2_non_simulatable_count) << std::endl;
            std::cout << "--------------------" << std::endl;
        utils::write_factor = utils::write_factor + 0.1;
    }
    return 0;
}

 
        
    //      int simulation_termination_time = utils::hyper_period * 10000000000;
    //      bool is_simulatable = true;

    //      while(utils::current_time < simulation_termination_time && is_simulatable)
    //      {
    //         schedule_generator.simulate_scheduling_on_real(utils::current_time);
    //         offline_guider.construct_job_precedence_graph();
    //         is_simulatable = executor.run_simulation(utils::current_time);
    //         vectors::job_vector_of_simulator.clear();
    //         for(auto someVector : vectors::job_vectors_for_each_ECU)
    //             someVector.clear();
    //         vectors::job_vectors_for_each_ECU.clear();
    //      }
    //      is_simulatable ? ++simulatable_count : ++nonsimulatable_count;
        
    //     if(utils::real_workload == false)
    //     {
    //         // Reset Globals
    //         global_object::logger = nullptr;
    //         global_object::gld_vector.clear();
            
    //         vectors::job_vector_of_simulator.clear();
    //         vectors::ecu_vector.clear();
    //         vectors::task_vector.clear();

    //         for(auto someVector : vectors::job_vectors_for_each_ECU)
    //             someVector.clear();
    //         vectors::job_vectors_for_each_ECU.clear();
    //         utils::current_time = 0;
    //     }
    // }

    // if(utils::real_workload == false)
    // {
    //     std::cout << std::endl;
    //     std::cout << "--------------------" << std::endl;
    //     std::cout << simulatable_count << " simulations were simulatable." << std::endl;
    //     std::cout << nonsimulatable_count << " simulations were non-simulatable." << std::endl;
    //     std::cout << "Simulatability ratio is " << simulatable_count / (double)(simulatable_count + nonsimulatable_count) << std::endl;
    //     std::cout << "--------------------" << std::endl;
    // }        