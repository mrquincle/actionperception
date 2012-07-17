/**
 * @brief 
 * @file main.cpp
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

#include <Embodiment.h>
#include <Information.h>
#include <RecyclingRobotsBenchmark.h>
#include <RecyclingRobot.h>
#include <RecyclingRobotsStructs.h>

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <sys/syslog.h>

using namespace std;

/**
 * Only undefined references left, which is true, I need to implement the methods.
 */
int main() {
	AP_SYSTEMS systems;
	for (int i = 0; i < NOF_SYSTEMS; ++i) {
		std::stringstream ss;
		ss << "robot " << i;
		RecyclingRobot *robot = new RecyclingRobot(ss.str());
		systems.push_back(robot);
//		robot->SetVerbosity(LOG_DEBUG);
	}
	RecyclingRobotsBenchmark env;
//		env.SetVerbosity(LOG_DEBUG);

	Information information;
	Embodiment embodiment;
	Coupling coupling;

	// think of how to do this neatly with multiple systems, each need other coupling parameters
	// they might communicate with each other
	embodiment.Couple(env, systems, coupling);

	ofstream file;
	file.open("rewards.txt");

	int timespan = 100000;
//	timespan = 20;
	AP_TYPE avg = AP_TYPE(0);
//	int window = 10;

	int nof_trials = 16;
	int max_trials = 16;
	assert (nof_trials <= max_trials);
	int seeds[] = {
			23, 349, 8904, 45802190,
			3483, 101, 292734, 3,
			2309, 929, 34509, 234523,
			23452345, 2828, 792384, 2348391};

	std::vector < std::vector<AP_TYPE> * > rewards;

	for (int trial = 0; trial < nof_trials; ++trial) {
		std::vector<AP_TYPE> *reward_per_trial = new std::vector<AP_TYPE>();
		reward_per_trial->clear();
		rewards.push_back(reward_per_trial);

		int seed = seeds[trial];
//		seed = 1;
		env.SetSeed(seed);
		embodiment.Restart();

//		avg = 0;
		for (int t = 1; t < timespan+1; ++t) {
			embodiment.Tick();
			AP_TYPE reward = env.GetDiscountedReward();
//			file << (t-1) << " " << reward << endl;
			reward_per_trial->push_back(reward);

//			avg += env.GetDiscountedReward();
//			if (!(t % window)) {
//				file << (t-1)/window << " " << avg/window << endl;
//				avg = 0;
//			}
			//		information.Calculate();
		}
		int print_reward = env.GetAccumulatedReward();
		cout << "Accumulated reward " << print_reward << endl;

//		print_reward = env.GetDiscountedReward();
//		cout << "Discounted reward " << print_reward << endl;

		int depletions = env.GetDepletionCount();
		cout << "Total number of depletions " << depletions << endl;
	}

	// write to file
	for (int t = 0; t < timespan; ++t) {
		avg = 0;
		for (int trial = 0; trial < nof_trials; ++trial) {
			std::vector<AP_TYPE> & reward_per_trial = *rewards[trial];
			AP_TYPE reward = reward_per_trial[t];
			avg += reward;
		}
		file << t << " " << avg/nof_trials << endl;
	}

	// calculate average reward
	avg = AP_TYPE(0);
	int start_time = 0;
	for (int t = start_time; t < timespan; ++t) {
		for (int trial = 0; trial < nof_trials; ++trial) {
			std::vector<AP_TYPE> & reward_per_trial = *rewards[trial];
			AP_TYPE reward = reward_per_trial[t];
			avg += reward;
		}
	}
	avg /= ((timespan-start_time) * nof_trials);
	cout << "Averaged discounted reward over " << nof_trials << " trials from t=" << start_time << ":" << timespan << " is " << avg << endl;

	for (int trial = nof_trials-1; trial >= 0; --trial) {
		delete rewards[trial];
		rewards.pop_back();
	}

	file.close();
}
