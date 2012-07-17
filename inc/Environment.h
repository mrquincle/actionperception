/**
 * @brief 
 * @file Environment.h
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 *
 * @author 	Anne C. van Rossum
 * @date	Apr 16, 2012
 * @project	Replicator FP7
 * @company	Almende B.V.
 * @case	Self-organised criticality
 */


#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <Structs.h>

//! The action vector over an entire multi-agent system
typedef std::vector<Action*> AP_MAS_ACTION;

//! A similar observation vector
typedef std::vector<Observation*> AP_MAS_OBSERVATION;

//! And a much needed state vector (over which the agent does not have control)
typedef std::vector<State*> AP_MAS_STATE;

/**
 * The environment is more sophisticated than a system in the sense that it does not just
 * receive "one action" and "one observation", but a vector of them. This means that it can
 * introduce some dependencies between agents in the world. So, e.g. in the multi-agent
 * setting of the multi-tiger problem or the robot recycling problem the reward can be
 * defined across the joint action of two (or more) agents.
 */
class Environment {
public:
	Environment();

	virtual ~Environment();

	//! This is the function that needs to be implemented by the environment
	//! In other words, that what defines your benchmark
	virtual void Tick(const AP_MAS_ACTION &actions, AP_MAS_OBSERVATION &observations,
			AP_MAS_STATE &states) = 0;

	//! We have to start with a certain state
	virtual void Init(AP_MAS_OBSERVATION& observations, AP_MAS_STATE& states) = 0;

	//! Restart the environment
	virtual void Restart() = 0;

	//! Get total sum of reward
	inline int GetAccumulatedReward() { return accumulated_reward; }

	//! Get discounted reward
	inline AP_TYPE GetDiscountedReward() { return discounted_reward; }

	//! Get last reward
	inline AP_TYPE GetInstantaneousReward() { return instantaneous_reward; }

	void Clear();

	inline void SetVerbosity(const char verbosity) { this->verbosity = verbosity; }

protected:
	//! Reward in total
	int accumulated_reward;

	//! Discounted reward (not over infinite times)
	AP_TYPE discounted_reward;

	//! Last given reward
	AP_TYPE instantaneous_reward;

	//! Verbosity level
	char verbosity;
};



#endif /* ENVIRONMENT_H_ */
