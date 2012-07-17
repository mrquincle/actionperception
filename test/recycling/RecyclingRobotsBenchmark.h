/***************************************************************************************************
 * @brief
 * @file RecyclingRobotsBenchmark.h
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
 * @date	May 21, 2012
 * @project	Replicator FP7
 * @company	Almende B.V. & Distributed Organisms B.V.
 * @case	Self-organised criticality
 **************************************************************************************************/


#ifndef RECYCLINGROBOTSBENCHMARK_H_
#define RECYCLINGROBOTSBENCHMARK_H_

#include <Environment.h>
#include <Structs.h>

#include <boost/random/mersenne_twister.hpp>

#define NOF_ROBOTS 2

/**
 * There are 2 states per robot (battery depleted and battery full). There are 3 actions, search for
 * big item, search for small item, and recharge. And there are 2 observations (battery depleted and
 * battery full).
 *
 * We will NOT make use of a transition table, because this is absolutely a monster when considering
 * more than 2 robots. Especially considering the fact that the probabilities are independent (the
 * battery depletion does only depend on the state of the given robot), there seems to be no point
 * in creating this table.
 */
//struct TransitionTable {
//	AP_TYPE states[2*NOF_ROBOTS];
//	AP_TYPE actions[3];
//	AP_TYPE observations[2];
//};

/**
 * We follow the implementation as in:
 *   http://rbr.cs.umass.edu/~camato/decpomdp/down/ProblemData3Probs.java
 * In case you want to check the Java code, realize that the transition table "trans[s][a][s_]" is
 * formulated as P(s'|a,s) in that order. And especially that s=0 means a HIGH battery level. So,
 * transEach[0][2][0]=alphaBig.
 * See also: http://users.isr.ist.utl.pt/~mtjspaan/decpomdp/recycling.dpomdp
 */
class RecyclingRobotsBenchmark: public Environment {
public:
	//! Default constructor
	RecyclingRobotsBenchmark();

	//! Default destructor
	~RecyclingRobotsBenchmark();

	/**
	 * Tick is the most important function. It will take a vector of actions and return a vector
	 * of observations. The "recycling robot" problem expects three possible actions (where the
	 * "waiting" state has been removed): search_big, search_small, and recharge.
	 */
	void Tick(const AP_MAS_ACTION &actions, AP_MAS_OBSERVATION &observations, AP_MAS_STATE &states);

	//! Initial state
	void Init(AP_MAS_OBSERVATION& observations, AP_MAS_STATE& states);

	//! Get number of times the robot has been totally depleted
	inline int GetDepletionCount() { return depletion_count; }

	//! Set seed for random number generated
	void SetSeed(int seed);

	//! Clear everything and restart (eventually with new seed)
	void Restart();

private:

	//! The chance of having a high energy level after a search_big action
	AP_TYPE alphaBig;

	//! The chance of having a high energy level after a search_small action
	AP_TYPE alphaSmall;

	//! The chance of having a low energy level after a search_big action
	AP_TYPE betaBig;

	//! The chance of having a low energy level after a search_small action
	AP_TYPE betaSmall;

	//! Factor that defines in how much past reward contributes to current reward
	AP_TYPE discount_factor;

	//! Number of total depletion events
	int depletion_count;

	//! Seed for random number generator
	int seed;

//	boost::mt19937 generator();
};


#endif /* RECYCLINGROBOTSBENCHMARK_H_ */
