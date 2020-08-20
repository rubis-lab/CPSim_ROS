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
    int m_10;
    int m_25;
    int m_50;
    int m_100;
    int m_timer_num;
    int m_subscriber_num;
    
public:
    /**
     * Constructor & Destructor
    */
    Initializer();
    ~Initializer();

    bool random_transaction_generator(int, int); //this function is for experiments.
    void random_ecu_generator(int); // this function is for experiments.
    void random_producer_consumer_generator();
    void random_constraint_selector(double, double);
    int uniform_ecu_selector();
    int uniform_period_selector(int);
    void set_simulatorPC_performance();
    void initialize();
};

#endif
