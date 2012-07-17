/***************************************************************************************************
 * @brief
 * @file RecyclingRobot.h
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common Hybrid 
 * Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from thread pools and 
 * TCP/IP components to control architectures and learning algorithms. This software is published 
 * under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless, we 
 * personally strongly object against this software used by the military, in the bio-industry, for 
 * animal experimentation, or anything that violates the Universal Declaration of Human Rights.
 *
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 ***************************************************************************************************
 * @author 	Anne C. van Rossum
 * @date	May 22, 2012
 * @project	Replicator FP7
 * @company	Almende B.V. & Distributed Organisms B.V.
 * @case	Self-organised criticality
 **************************************************************************************************/

#ifndef RECYCLINGROBOT_H_
#define RECYCLINGROBOT_H_

#include <System.h>
#include <string>

#include <RecyclingRobotsStructs.h>

/**
 * The recycling robot should adapt itself to the observations (including reward) that it receives
 * and come up with a proper action that optimizes reward over the long-term.
 */
class RecyclingRobot: public System {
public:
	//! Give the robot a nice name
	RecyclingRobot(std::string name);

	//! Kill it
	~RecyclingRobot();

	//! Main function to call, this will return an action given the observation (including reward)
	void Tick(const Observation &observation, Action &action);

	//! The robot can be restarted by the experimenter
	void Restart();

	//! Make it scream
	inline void SetVerbosity(char verbosity) { this->verbosity = verbosity; }
protected:
	//! Manual definition of action to check if their are no simple policies that can easily be
	//! discovered
	void ManualTweaking(const RR_OBSERVATION & obs, const RR_OBSERVATION reward, Action& action);
private:
	std::string name;

	RR_ACTION prev_act;

	AP_TYPE prev_rew;

	char verbosity;
};


#endif /* RECYCLINGROBOT_H_ */
