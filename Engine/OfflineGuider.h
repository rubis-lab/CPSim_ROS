#ifndef OFFLINEGUIDER_H__
#define OFFLINEGUIDER_H__

#include "Utils.h"

/** This file is engine code of CPSim-Re engine
 * @file OfflineGuider.h
 * @class OfflineGuider
 * @author Alex Noble
 * @date 2020-08-19
 * @brief Codes for Engine-OfflineGuider 
*/

class OfflineGuider {
private:
public:
    /**
     * Constructor & Destructor
    */
    OfflineGuider();
    ~OfflineGuider();
    /**
     * Generate Offline Guider
     */
    void construct_job_precedence_graph();
    void update_job_precedence_graph();
    void recurrent_transaction_analysis(std::shared_ptr<Job> job);
};

#endif