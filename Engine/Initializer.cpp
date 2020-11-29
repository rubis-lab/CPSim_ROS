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
    m_10 = 0;
    m_25 = 0;
    m_50 = 0;
    m_100 = 0;
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
void Initializer::initialize()
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
        //random_ecu_generator((rand() % 8) + 3);
        random_ecu_generator(2);
        /**
         * Task Vector Initialization
         */
        
        int ecu_num = vectors::ecu_vector.size();
        //int random_task_num = ecu_num * ((rand() % 5) + 1);
        int random_task_num = 7;
        int random_transaction_num = std::ceil(utils::transaction_factor * random_task_num);
        //int random_transaction_num = 2;
        if(random_transaction_num == 0)
            random_transaction_num = 1;
        // std::cout << "ECU :" << ecu_num  << std::endl;
        // std::cout << "TASK : " << random_task_num << std::endl;
        // std::cout << "TRANSACTION : " << random_transaction_num << std::endl;
        if(!random_transaction_generator(random_transaction_num, random_task_num))
            std::cout << "TASK NUMBER IS TOO MUCH" << std::endl;
        /**
         * 
         */
        random_constraint_selector(utils::read_factor, utils::write_factor);
        transaction_producer_consumer_generator();
    }
                          
    /**
     * Global Hyper Period Initialization
     */
    utils::hyper_period = utils::calculate_hyper_period(vectors::timer_vector);

    /**
     * Global Logger Initialization
     */    
    global_object::logger = std::make_shared<Logger>();
    global_object::logger->log_transaction_status();
}

void Initializer::random_ecu_generator(int ecu_num)
{
    for(int i = 0; i < ecu_num; i++)
    {
        std::shared_ptr<ECU> ecu =  std::make_shared<ECU>(100,"RM");
        vectors::ecu_vector.push_back(std::move(ecu));
    }
}

// For transactions, we already set transaction relation with transaction vector.
// So, using this. 
// Just, make each task producer and consumer here.
void Initializer::transaction_producer_consumer_generator()
{
    for(auto transaction : vectors::transaction_vector)
    {
        for(int task_idx = 0; task_idx < transaction.size(); task_idx++)
        {
            if(task_idx == transaction.size() - 1)
            {
                // THIS IS LAST TASK OF THIS TRANSACTION
                break;
            }
            else
            {
                transaction.at(task_idx)->add_task_to_consumer(transaction.at(task_idx + 1)); 
                transaction.at(task_idx + 1)->add_task_to_producer(transaction.at(task_idx));
            }
        }
    }    
}

// Write Constraint can be assigned to one transaction.
// Read Constraint can be assigned to timer callback only(same as transaction also).
// Then, we can think each constraint can be assigned to a transaction. 
void Initializer::random_constraint_selector(double read_ratio, double write_ratio)
{
    int transaction_num = vectors::transaction_vector.size();
    int number_of_read = std::ceil(read_ratio * transaction_num); //read ratio is 30%
    int number_of_write = std::ceil(write_ratio * transaction_num); //write ratio is 30%

    int selector = 0;
    while(number_of_read != 0)
    {
        selector = rand() % vectors::transaction_vector.size();                                                   // Sync jobs can only read from Init jobs (gpu wait time).
        if(vectors::transaction_vector.at(selector).at(0)->get_is_read() == true)
        {
            continue;
        }
        else
        {
            vectors::transaction_vector.at(selector).at(0)->set_is_read(true);
            number_of_read--;
        }
        
    }

    while(number_of_write != 0)
    {
        selector = rand() % vectors::transaction_vector.size();                                                             // Init jobs only write to the gpu.
        if(vectors::transaction_vector.at(selector).at(vectors::transaction_vector.at(selector).size()-1)->get_is_write() == true)
        {
            continue;
        }
        else
        {
            vectors::transaction_vector.at(selector).at(vectors::transaction_vector.at(selector).size()-1)->set_is_write(true);
            number_of_write--;
        }
    }
}

bool Initializer::random_transaction_generator(int transaction_num, int task_num)
{
    /**
     * We need N timer callbacks if there are N transactions.
     * So, transaction num <= task num ALWAYS.
     */
    if(transaction_num > task_num)
        return false;

    /**
     * Create Transaction Vector Spaces First
     */
    for(int i = 0; i < transaction_num; i++)
    {
        std::vector<std::shared_ptr<Task>> transaction;
        vectors::transaction_vector.push_back(transaction);
    }

    /**
     * Create Timer Callbacks with Number of Transactions.
     */
    m_timer_num = transaction_num;
    for(int i = 0; i < m_timer_num; i++)
    {
        std::string task_name = "T" + std::to_string(i) + ",0";
        int period = uniform_period_selector(transaction_num);
        int offset = 0;
        int priority = rand() % m_timer_num;
        int callback_type = 0;
        int fet = (double)period * 0.2;
        bool is_read = false;
        bool is_write = false;
        int ecu_id = uniform_ecu_selector();
        int transaction_id = i;
        std::shared_ptr<Task> task = std::make_shared<Task>(task_name, period, period, priority, callback_type, fet, offset, is_read, is_write, ecu_id, transaction_id);
        task->set_chain_pos(vectors::transaction_vector.at(transaction_id).size());
        
        vectors::ecu_vector.at(ecu_id)->add_task_to_ecu(task);
        vectors::task_vector.push_back(task);
        vectors::timer_vector.push_back(task);
        vectors::transaction_vector.at(transaction_id).push_back(task);
    }
    

    /**
     * Create Subscriber Callbacks with Task Num - Transaction Num
     */
    m_subscriber_num = task_num - m_timer_num;
    for(int i = 0; i < m_subscriber_num; i++)
    {
        int period = -1;
        int offset = -1;
        int priority = 1000 + (rand() % m_subscriber_num);
        int callback_type = 1;
        bool is_read = false;
        bool is_write = false;
        int ecu_id = uniform_ecu_selector();
        int transaction_id = random_transaction_selector(transaction_num);
        std::string task_name = "T" + std::to_string(transaction_id) + "," + std::to_string(vectors::transaction_vector.at(transaction_id).size());
        int fet = vectors::transaction_vector.at(transaction_id).at(0)->get_fet();
        std::shared_ptr<Task> task = std::make_shared<Task>(task_name, period, period, priority, callback_type, fet, offset, is_read, is_write, ecu_id, transaction_id);
        task->set_chain_pos(vectors::transaction_vector.at(transaction_id).size());
        
        vectors::ecu_vector.at(ecu_id)->add_task_to_ecu(task);
        vectors::task_vector.push_back(task);
        vectors::subscriber_vector.push_back(task);
        vectors::transaction_vector.at(transaction_id).push_back(task); //TBD
    }
    return true;
}

int Initializer::random_transaction_selector(int transaction_num)
{
    int selector = rand() % transaction_num;
    return selector; 
}
int Initializer::uniform_ecu_selector()
{   
    int ecu_id = rand() % vectors::ecu_vector.size();
    for(int i = 0; i < vectors::ecu_vector.size(); i++)
    {
        if(vectors::ecu_vector.at(ecu_id)->get_num_of_task() > vectors::ecu_vector.at(i)->get_num_of_task())
        {
            ecu_id = i;
        }
    }
    vectors::ecu_vector.at(ecu_id)->set_num_of_task(vectors::ecu_vector.at(ecu_id)->get_num_of_task() + 1);
    return ecu_id;
}

int Initializer::uniform_period_selector(int transaction_num)
{
    //[10-100] milli sec. uniform 10, 25, 50, 100 in transaction num

    int min_val = INT_MAX;
    int min_idx = 0;
    for(int i = 0; i < 4; i++)
    {
        if(i == 0)
            if(min_val > m_10)
            {
                min_val = m_10;
                min_idx = 0;
            }
        if(i == 1)
            if(min_val > m_25)
            {
                min_val = m_25;
                min_idx = 1;
            }
        if(i == 2)
            if(min_val > m_50)
            {
                min_val = m_50;
                min_idx = 2;
            }
        if(i == 3)
            if(min_val > m_100)
            {
                min_val = m_100;
                min_idx = 3;
            }
    }
    if(min_idx == 0)
    {
        m_10++;
        return 10;
    }
    else if(min_idx == 1)
    {
        m_25++;
        return 25;
    }
    else if(min_idx == 2)
    {
        m_50++;
        return 50;
    }    
    else if(min_idx == 3)
    {
        m_100++;
        return 100;
    }
}