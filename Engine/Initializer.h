#ifndef INITIALIZER_H__

#include <stdio.h>
#include <thread>

#include "Utils.h"
#include "Logger.h"


/** This file is engine code of CPSim-Re engine
 * @file Initializer.h
 * @author Seonghyeon Park
 * @date 2020-08-18
 * @class Initializer
 * @brief Codes for Engine-Initializer.\n
 * The Initializer is responsible for initializing entire objects of 
 * simulator engine.
*/

class Initializer
{
private:
public:
    /**
     * Constructor & Destructor
    */
    Initializer();
    ~Initializer();

    int execution_time_mapping_function();
        
    void random_task_generator(int); // this function is for experiments.
    void random_ecu_generator(int); // this function is for experiments.
    void random_producer_consumer_generator();
    void random_constraint_selector(double, double);

    void set_simulatorPC_performance();
    void initialize(std::string location);
};

#endif
