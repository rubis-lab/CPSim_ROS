#include "Initializer.h"
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cmath>

/** 
 *  This file is the cpp file for the Initializer class.
 *  @file Initializer.cpp
 *  @brief cpp file for Engine-Initializer
 *  @author Seonghyeon Park
 *  @date 2020-08-19
 */

/**
 * @fn Initializer::Initializer()
 * @brief the function of basic constructor of Initializer
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Initializer::Initializer()
{

}

/**
 * @fn Initializer::~Initializer()
 * @brief the function of basic destroyer of Initializer
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Initializer::~Initializer()
{
    
}

/**
 * @fn void Initializer::initialize()
 * @brief the function which is responsible 
 * @author Seonghyeon Park
 * @date 2020-08-19
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
void Initializer::initialize(std::string location)
{    
    if(utils::real_workload == true)
    {

    }   
    else
    {
        /**
         * ECU Vector Initialization
         * number of ECU is [3-10]
         */
        random_ecu_generator((rand() % 8) + 3);
        
        /**
         * Task Vector Initialization
         */
        random_task_generator(10);

        /**
         * Each task can be [0-2] data producer of random selected job.
         */
        random_constraint_selector(0.3, 0.3);
        random_producer_consumer_generator();
    }
                          
    
    /**
     * Global Hyper Period Initialization
     */
    utils::hyper_period = utils::calculate_hyper_period(vectors::task_vector);
    
    /**
     * Global Logger Initialization
     */    
    global_object::logger = std::make_shared<Logger>();
    global_object::logger->log_task_vector_status();

}

void Initializer::random_task_generator(int task_num)
{
    for(int i = 0; i < task_num; i++)
    {
        //if(gpu_jobs > 0)
            //std::cout << "We have " << gpu_jobs << " GPU jobs left to add." << std::endl;
        std::string task_name = "TASK" + std::to_string(i);
        bool is_gpu_task = false;
        
        int period = rand() % 100; //[10-100] milli sec.
        if(period < 25)
            period = 10;
        else if(period < 50)
            period = 20;
        else if(period < 75)
            period = 40;
        else period = 80;
        // Make sure period is large enough for GPU jobs so that we can have discrete sync / init execution times...
        if (period < 40 && is_gpu_task) period = 80;
        
        int bcet = std::ceil(period * ((rand() % 6 + 5) * 0.01)); // [5-10] percent of period.
        int wcet = ((rand() % 2) + 1) * (double)bcet;// [1.0-2.0] random variation factor multiply bcet
        int offset = 0; //RTSS paper sets offset as 0.
        bool is_read = false;
        bool is_write = false;
        int ecu_id = rand() % vectors::ecu_vector.size();
        
        //each task can be data producer of [0-2] randomly selected task.
        std::vector<std::string> producers;
        std::vector<std::string> consumers;
        int num_of_consumer = rand() % 3;
        bool is_overlapped = false;

        while(num_of_consumer != 0)
        {
            int random_select_taskID = rand() % (task_num + 1);
            std::string consumer = "TASK" + std::to_string(random_select_taskID);
            for(int k = 0; k < consumers.size(); k++)
            {
                if(consumers.at(k) == consumer)
                {
                    is_overlapped = true;
                    break;
                }
                if(consumer == task_name)
                {
                    is_overlapped = true;
                    break;
                }
            }
            if(is_overlapped == false)
            {
                consumers.push_back(consumer);
                num_of_consumer--;
            }
            else
            {
                is_overlapped = false;
            }
        }

        //each ecu can have [1-5] task.
        int min_num = INT_MAX;
        int min_ecu_id = 0;
        for(int j = 0; j < vectors::ecu_vector.size(); j++)
        {
            if(min_num > vectors::ecu_vector.at(j)->get_num_of_task())
            {
                min_num = vectors::ecu_vector.at(j)->get_num_of_task();
                min_ecu_id = vectors::ecu_vector.at(j)->get_ECU_id();
            }
        }

        ecu_id = min_ecu_id;
        vectors::ecu_vector.at(ecu_id)->set_num_of_task(min_num + 1);

        //Task Name(id), period, deadline, wcet, bcet, offset, read, write, ecu, producer, consumers

        std::shared_ptr<Task> task = std::make_shared<Task>(task_name, period, period, wcet, bcet, offset, is_read, is_write, ecu_id, producers, consumers);
        vectors::task_vector.push_back(std::move(task));
        
    }
    // What is this?
    for(int ecu_num =0; ecu_num < vectors::ecu_vector.size(); ecu_num++)
    {
        for(int i = 0; i < task_num; i++)
        {
            std::vector<std::shared_ptr<Job>> v_job_of_ecu;
            vectors::job_vectors_for_each_ECU.at(ecu_num).push_back(v_job_of_ecu);
        }
    }
    
}

void Initializer::random_ecu_generator(int ecu_num)
{
    for(int i = 0; i < ecu_num; i++)
    {
        std::shared_ptr<ECU> ecu =  std::make_shared<ECU>(100,"RM");
        vectors::ecu_vector.push_back(std::move(ecu));
    }
    // What is this
    for(int i = 0; i < ecu_num; i++)
    {
        std::vector<std::vector<std::shared_ptr<Job>>> v_job_of_ecu;
        vectors::job_vectors_for_each_ECU.push_back(v_job_of_ecu);
    }
}

void Initializer::random_producer_consumer_generator()
{
    for(auto task : vectors::task_vector)
    {
        for(auto consumer : vectors::task_vector)
        {
            for(auto to_be_consumer : task->get_consumers_info())
            {
                if(consumer->get_task_name() == to_be_consumer)
                {
                    task->add_task_to_consumers(consumer);
                    consumer->add_task_to_producers(task);
                }
            }
        }    
    }    
}

void Initializer::random_constraint_selector(double read_ratio, double write_ratio)
{
    int task_num = vectors::task_vector.size();
    int number_of_read = std::ceil(read_ratio * task_num); //read ratio is 30%
    int number_of_write = std::ceil(write_ratio * task_num); //write ratio is 30%

    int selector = 0;
    while(number_of_read != 0)
    {
        selector = rand() % vectors::task_vector.size();                                                   // Sync jobs can only read from Init jobs (gpu wait time).
        if(vectors::task_vector.at(selector)->get_is_read() == true)
        {
            continue;
        }
        else
        {
            vectors::task_vector.at(selector)->set_is_read(true);
            number_of_read--;
        }
        
    }

    while(number_of_write != 0)
    {
        selector = rand() % vectors::task_vector.size();                                                                    // Init jobs only write to the gpu.
        if(vectors::task_vector.at(rand() % vectors::task_vector.size())->get_is_write() == true)
        {
            continue;
        }
        else
        {
            vectors::task_vector.at(selector)->set_is_write(true);
            number_of_write--;
        }
    }
}