#include "Utils.h"

/**
 *  This file is the cpp file for the RUBIS_Util class.
 *  @file RUBIS_Util.cpp
 *  @brief cpp file for Engine-RUBIS_Util
 *  @author Seonghyeon Park
 *  @date 2020-03-31
 */

/**
 * @fn int greatest_common_divider
 * @brief get the greatest common divider of two numbers
 * @author Seonghyeon Park
 * @date 2020-05-12
 * @details 
 *  - None
 * @param int a
 * @param int b
 * @return a
 * @bug none
 * @warning none
 * @todo none
 */
int utils::greatest_common_divider(int a, int b)
{
	int c;
	
	while (b != 0)
	{	
		c = a % b;
		a = b;
		b = c;
	}

	return a;
}

/**
 * @fn int least_common_multiple
 * @brief get the greatest common divider of two numbers
 * @author Seonghyeon Park
 * @date 2020-05-12
 * @details 
 *  - None
 * @param int a
 * @param int b
 * @return a
 * @bug none
 * @warning none
 * @todo none
 */
int utils::least_common_multiple(int a, int b)
{
	return (a * b) / utils::greatest_common_divider(a, b);
}

/**
 * @fn int least_common_multiple_array(std::vector<std::shared_ptr<Task> >& task_set)
 * @brief get the greatest common divider of number of arrays
 * @author Alex
 * @date 2020-05-19
 * @details 
 *  - None
 * @param std::vector<std::shared_ptr<Task> >& task_set
 * @return least common multiple
 * @bug none
 * @warning none
 * @todo none
 */
int utils::least_common_multiple_array(std::vector<std::shared_ptr<Task> >& task_set)
{
	// IF CODE IS USING SMART POINTERS
	// NEVER USE RAW POINTERS
	/**
	To be deleted code 
	Just for test
	std::cout << task_set->size() << std::endl;
	task_set->pop_back();
	std::cout << task_set->size() << std::endl;
	
	void* ptr = &task_vector;

	task_vector->INNER ARRAY

	(CAST)ptr->pop_back()
	*/

	int lcm_of_array = 0;

	for (auto iter = task_set.begin(); iter != task_set.end(); iter++)
	{
		
		/**
		To be deleted code
		std::cout << iter->get()->get_task_name() << std::endl;
		*/

		
		if (lcm_of_array == 0)
		{
			/**
			 * To be delete code for testing
			 * std::cout << iter->get()->get_period() << std::endl; 
			 */
			
			lcm_of_array = iter->get()->get_period();
		}
		else
		{
			/**
			 * To be delete code for testing
			 * std::cout << iter->get()->get_period() << std::endl;
			 */ 
			lcm_of_array = utils::least_common_multiple(lcm_of_array, iter->get()->get_period());
		}
	}

	/**
	 * To be delete code for testing
	 * std::cout << "the last output is : " << lcm_of_array << std::endl;
	 */

	return lcm_of_array;
}

/**
 * @fn void calculate_hyper_period()
 * @brief This function stops the simulation when ctrl + C command is triggered.
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  Using signal handler API.
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
int utils::calculate_hyper_period(std::vector<std::shared_ptr<Task>>& task_set)
{
	int hyper_period = 0;
	hyper_period = utils::least_common_multiple_array(task_set);
	//std::cout << "Hyper Period is : " << hyper_period << std::endl;
	return hyper_period;
}

/**
 * @fn void exit_simulation()
 * @brief This function stops the simulation when ctrl + C command is triggered.
 * @author Seonghyeon Park
 * @date 2020-08-20
 * @details 
 *  Using signal handler API.
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
void utils::exit_simulation(int signo)
{
	std::cout << "Simulation End\n" << std::endl;
	exit(signo);
}

bool utils::compare(std::shared_ptr<Job> pred, std::shared_ptr<Job> succ)
{
    //if two jobs priority are same, then choose job id first.
    if(pred->get_priority() == succ->get_priority())
		return pred->get_job_id() > succ->get_job_id();
	return pred->get_priority() > succ->get_priority();
}

bool utils::first_release(std::shared_ptr<Job> pred, std::shared_ptr<Job> succ)
{
	if(pred->get_real_release_time() == succ->get_real_release_time())
	{
		return pred->get_priority() < succ->get_priority();
	}
	else 
	{
		return pred->get_real_release_time() < succ->get_real_release_time();
	}	
}