/***************************************************************************************************
 * @brief
 * @file RecyclingRobot.cpp
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

#include <RecyclingRobot.h>
#include <RecyclingRobotsStructs.h>

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <sys/syslog.h>

using namespace std;

RecyclingRobot::RecyclingRobot(std::string name) {
	this->name = name;
	verbosity = LOG_DEBUG;
	verbosity = LOG_INFO;
	prev_act = RRA_SEARCH_BIG;
	prev_rew = 0;
}

RecyclingRobot::~RecyclingRobot() {
}

/**
 * Sets the first action to SEARCH BIG.
 */
void RecyclingRobot::Restart() {
	prev_act = RRA_SEARCH_BIG;
	prev_rew = 0;
}

/**
 * Which action to execute given a certain local observation. That's the problem a robot encounters
 * in a DEC-POMDP problem.
 *
 * We want to be able to call this function:
 *  Algorithm::update(State * st, Action * action, double rt, State * st_, bool endOfEpisode,
 *    double * learningRate, double gamma);
 * What needs some "care" is the endOfEpisode boolean. In the intrinsic motivation case, this should
 * be part of the observation set, or derived by the robot itself. The double array with learning
 * rates can be used for multiple trials. There is the current state and the next state "st_". These
 * states can of course be "observations", but most naturally are "beliefs": entities derived from
 * (a history of) action-perception events.
 *
 * The construction of e.g. Sarsa() or Qlearning() requires a predefined World, with for example a
 * predefined number of states.
 *
 * We will also call StateActionAlgorithm::explore which should return a new action for us (this
 * will be returned in the given Action* parameter).
 */
void RecyclingRobot::Tick(const Observation& observation, Action& action) {
	assert (observation.size() <= 2);
	assert (action.empty());

	if (observation.size() < 1) {
		cerr << "No observation or no reward!" << endl;
		action.clear();
		action.push_back(RRA_RECHARGE);
		return;
	}
	RR_OBSERVATION obs = (RR_OBSERVATION)observation[RROT_BATTERY_LEVEL];

	RR_OBSERVATION reward = (RR_OBSERVATION)observation[RROT_INSTANTANEOUS_REWARD];

	if (verbosity >= LOG_DEBUG)
		cout << name << ": observation = " << RR_OBSERVATION_STR[obs] << endl;
	action.clear();

	// This function should actually be replaced by a real function using Bellman recursion etc.
	ManualTweaking(obs, reward, action);

	if (verbosity >= LOG_DEBUG) {
		cout << name << ": action = " << RR_ACTION_STR[prev_act] << endl;
	}
}

/**
 * My random thoughts over time trying to quickly implement something that does not use learning at all... :-)
 *
 * Given reward and own action history it should be possible to estimate the state of
 * the other robot, e.g. a reward equal to 0 means that the last action of both was RRA_RECHARGE and both their states HIGH
 * if it would be possible to derive the state of the other as HIGH, then it pays to RRA_SEARCH_BIG even when battery is low
 * after receiving reward == 5, and being HIGH, there is only 50% chance that the other is still at HIGH BATTERY level
 * that is apparently not good enough...
 * after receiving reward == 4, and being HIGH/LOW there is a 70% chance that the other is HIGH too
 * after receiving reward == 2, and being HIHG/LOW there is a ??% chance that the other is HIGH
 * I am afraid that it's not so easy to quickly write out additional rules... it prob. is a more complicated function of the
 * reward history...
 * a reward history of [ 4 5 .. .. ] with most recent first, means that a SEARCH_BIG action was followed by a SEARCH_SMALL and
 * both were successful... there is now a
 */
void RecyclingRobot::ManualTweaking(const RR_OBSERVATION & obs, const RR_OBSERVATION reward, Action& action) {
	switch(obs) {
	case RRO_BATTERY_HIGH:
		if (reward <= 0) { // after both recharged, it's safe to search for BIG
			action.push_back(RRA_SEARCH_BIG);
#ifdef TRY_TO_USE_HISTORY
		} else 	if ((reward == 4) && prev_rew == 5) {
			action.push_back(RRA_SEARCH_BIG);
#endif
		} else {
			action.push_back(RRA_SEARCH_SMALL);
		}
		break;
	case RRO_BATTERY_LOW:
#ifdef TRY_TO_USE_HISTORY
		if ((reward == 4) && prev_rew == 5)
			action.push_back(RRA_SEARCH_BIG);
		else
#endif
			action.push_back(RRA_RECHARGE);
		break;
	default:
		break;
	}

	// in case we want to use the action at t-1
	prev_rew = reward;
	prev_act = (RR_ACTION)action[RRAT_SEARCHING];
}

