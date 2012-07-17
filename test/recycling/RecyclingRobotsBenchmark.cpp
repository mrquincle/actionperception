/***************************************************************************************************
 * @brief
 * @file RecyclingRobotsBenchmark.cpp
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

#include <RecyclingRobotsBenchmark.h>
#include <RecyclingRobotsStructs.h>

#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <sys/syslog.h>

#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random/mersenne_twister.hpp>

using namespace std;

/**
 * Sets parameter values.
 */
RecyclingRobotsBenchmark::RecyclingRobotsBenchmark() {
	alphaBig = 0.5;
	alphaSmall = 0.7; 	// chance to stay at "high battery level" searching for small objects < 0.8
	betaBig = 0.3;
	betaSmall = 0.2;    // chance at depletion, so stay at "low battery level" is 0.8: 4 upon 5
						// expected risk: -10, expected gain: 2, 5 upon 6 makes it equal, so don't risk it!

	// a discount factor of 0.9, then we can calculate the maximum reward possible (value)
	// which is \sum_i d^i * r_max (with d equal to 0.9 and r_max equal to 5)
	// and with \sum_{i=0}{N-1} d^i = (1-d^N)(1-d) it is easy to calculate total possible
	// reward over time, namely 1/(1-d)*r_max (with d=0.9, r_max=5 it is 50).
	discount_factor = 0.9;

	depletion_count = 0;
	int seed = 38;
	srand48(seed);
}

/**
 * Default destructor does nothing
 */
RecyclingRobotsBenchmark::~RecyclingRobotsBenchmark() {}


void RecyclingRobotsBenchmark::SetSeed(int seed) {
	this->seed = seed;
	srand48(seed);
//	generator.seed(seed);
}

void RecyclingRobotsBenchmark::Restart() {
	Clear();
	depletion_count = 0;
	SetSeed(seed);
}

void RecyclingRobotsBenchmark::Init(AP_MAS_OBSERVATION& observations, AP_MAS_STATE& states) {
	for (int i = 0; i < NOF_SYSTEMS; ++i) {
		State &state = *states[i];
		state.clear();
		state.push_back(RRS_BATTERY_HIGH);
	}

	// make from states observations
	for (int i = 0; i < NOF_SYSTEMS; ++i) {
		State *state = states[i];
		if (state->empty()) {
			cout << "State is empty, cannot be used as observation input!" << endl;
			continue;
		}
		observations[i]->clear();
		observations[i]->push_back(state->front());
		observations[i]->push_back(instantaneous_reward);
	}
}

/**
 * Only implementation for 2 actors for now.
 */
void RecyclingRobotsBenchmark::Tick(const AP_MAS_ACTION& actions,
		AP_MAS_OBSERVATION& observations, AP_MAS_STATE& states) {
	assert (actions.size() == NOF_SYSTEMS);
	assert (observations.size() == NOF_SYSTEMS);
	assert (states.size() == NOF_SYSTEMS);
	int nr_bots = actions.size();

	bool depleted[nr_bots];
	for (int i = 0; i < nr_bots; ++i) {
		depleted[i] = false;
	}

	// please, observe that the state of the robots are considered independent, although the MAS
	// setting rewards cooperation between robots, it does not reward it in the sense that in the
	// case that both robots try to lift a can, it does cost less energy
	for (int i = 0; i < nr_bots; ++i) {
		Action &action = *actions[i];
		State &state = *states[i];

		if (action.empty()) {
			cerr << "No action coming from robot " << i << endl;
			continue;
		}
		if (state.empty()) {
			cerr << "No state, did you call Init() for the first state initialisation?" << endl;
			continue;
		}

		switch ((RR_ACTION)action[RRAT_SEARCHING]) {
		case RRA_SEARCH_BIG: {
			if ((RR_STATE)state[RRST_BATTERY_LEVEL] == RRS_BATTERY_HIGH) {
				if (drand48() < alphaBig) {
					state[RRST_BATTERY_LEVEL] = RRS_BATTERY_HIGH; // same state
				} else {
					if (verbosity >= LOG_DEBUG)	cout << "to robot " << i << ": You're unlucky, battery is discharged for large part" << endl;
					state[RRST_BATTERY_LEVEL] = RRS_BATTERY_LOW;
				}
			} else { // battery low
				if (drand48() < betaBig) { // by default 3 upon 10 times we get really depleted
					depleted[i] = true;
					if (verbosity >= LOG_DEBUG) cout << "to robot " << i << ": Depleted, reset to high battery level" << endl;
					state[RRST_BATTERY_LEVEL] = RRS_BATTERY_HIGH; // depleted, so reset
				} else {
					state[RRST_BATTERY_LEVEL] = RRS_BATTERY_LOW;
				}
			}
			break;
		}
		case RRA_SEARCH_SMALL: {
			if ((RR_STATE)state[RRST_BATTERY_LEVEL] == RRS_BATTERY_HIGH) {
				if (drand48() <= alphaSmall) {
					state[RRST_BATTERY_LEVEL] = RRS_BATTERY_HIGH; // same state
				} else {
					if (verbosity >= LOG_DEBUG) cout << "to robot " << i << ": You're unlucky, battery is discharged for large part (even though you searched for small item)" << endl;
					state[RRST_BATTERY_LEVEL] = RRS_BATTERY_LOW;
				}
			} else { // battery low
				if (drand48() <= betaSmall) { // by default 1 upon 5 times we get really depleted
					depleted[i] = true;
					if (verbosity >= LOG_DEBUG) cout << "to robot " << i << ": Depleted, reset to high battery level (even though you searched for small item)" << endl;
					state[RRST_BATTERY_LEVEL] = RRS_BATTERY_HIGH; // depleted, so reset
				} else {
					state[RRST_BATTERY_LEVEL] = RRS_BATTERY_LOW;
				}
			}
			break;
		}
		case RRA_RECHARGE: {
			state[RRST_BATTERY_LEVEL] = RRS_BATTERY_HIGH;
			break;
		}
		default:
			cerr << "Unknown state, error!" << endl;
			break;
		}
	}

	// calculate reward
	int count_big = 0;
	for (int i = 0; i < nr_bots; ++i) {
		Action &action = *actions[i];
		if (action.empty()) continue;
		if ((RR_ACTION)action[RRAT_SEARCHING] == RRA_SEARCH_BIG) {
			if (!depleted[i])
				++count_big;
		}
	}

	int count_small = 0;
	for (int i = 0; i < nr_bots; ++i) {
		Action &action = *actions[i];
		if (action.empty()) continue;
		if ((RR_ACTION)action[RRAT_SEARCHING] == RRA_SEARCH_SMALL) {
			if (!depleted[i])
				++count_small;
		}
	}


	instantaneous_reward = 0;
	if (count_big == nr_bots) instantaneous_reward+=5;
	instantaneous_reward += count_small*2;

	for (int i = 0; i < nr_bots; ++i) {
		if (depleted[i])
			instantaneous_reward -= 10;
	}

	accumulated_reward += instantaneous_reward;

	discounted_reward = instantaneous_reward + discounted_reward * discount_factor;

	for (int i = 0; i < nr_bots; ++i)
		depletion_count += depleted[i];

	// make from states observations
	for (int i = 0; i < nr_bots; ++i) {
		State *state = states[i];
		if (state->empty()) {
			cout << "State is empty, cannot be used as observation input!" << endl;
			continue;
		}
		observations[i]->push_back(state->front());
		observations[i]->push_back(instantaneous_reward);
	}
}
