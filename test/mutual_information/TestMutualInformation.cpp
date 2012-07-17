/***************************************************************************************************
 * @brief
 * @file TestMutualInformation.cpp
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
 * @date	May 9, 2012
 * @project	Replicator FP7
 * @company	Almende B.V. & Distributed Organisms B.V.
 * @case	Self-organised criticality
 **************************************************************************************************/

#include <MutualInformation.h>
#include <TestMutualInformation.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/multivariate_normal_distribution.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include <cholesky.hpp>
#include <determinant.h>

using namespace std;
using namespace boost;
using namespace boost::numeric;

TestMutualInformation::TestMutualInformation() {
	mi = new MutualInformation();
}

TestMutualInformation::~TestMutualInformation() {
	delete mi;
}

void TestMutualInformation::Test() {
	//	Sinus();

	if (fabs(mi->digamma(1.0) + 0.5772156) > 0.01) {
		cout << "digamma(1.0) should be -0.5772156 and is: " << mi->digamma(1.0) << endl;
	}
	srand48(time(NULL));
	//	Independent();
	Normal();
}

/**
 * Two normal (Gaussian) distributions can be tested using "r", a correlation value. If this value is
 * low (r approaching 0) these distributions are uncorrelated and the mutual information figure is low.
 * If it is high (r approaching 1) the distributions are correlated and the mutual information figure
 * should be high.
 * Keep in mind that mutual information here is used as a qualitative measure for the correlation
 * between sensors and actuators. The result for example is in "nats" and not in "bits", and we do not
 * care about that. The approach also seems to give higher values if longer time series are taken into
 * account. This should be considered a fault, but also that does not hamper the goal we have set. We
 * just should not compare mutual information for two different time series of different length.
 */
void TestMutualInformation::Normal() {

	mi->setMIApproximation(MI_K_NEAREST_NEIGHBOUR);

	double seed = 10;
	mt19937 rng(seed);

	// the correlation value (should be less than 1.0)
	// we set the standard deviation to 1, so the correlation matrix is the covariance matrix
	double r = .9;

	ublas::matrix<double> A(2,2);
	A(0,0) = 1.; A(0,1) = r;
    A(1,0) = r; A(1,1) = 1.;

	// the Cholesky matrix multiplied by itself (transposed) gives the correlation matrix
	ublas::matrix<double> C(2,2);
    cholesky_decompose(A, C);
    ublas::matrix<double> R(2,2);
    R = ublas::prod(C, ublas::trans(C));
    cout << "A: " << R << endl;

    double det = determinant(A);
    cout << "Determinant is: " << det << endl; // with unit variance this is 1 - r*r
    cout << "The mutual information should be: " << -0.5*log(det) << endl;

    // the average values of the distribution
    ublas::vector<double> m(2);
    m(0) = 0; m(1) = 0;

    // create a bivariate distribution
	multivariate_normal_distribution<double> D(C, m);

	// and create a generator for samples from it
	variate_generator<mt19937&, multivariate_normal_distribution<> > var_norm(rng, D);

	ofstream fout;
	fout.open("data.txt");

	SensorimotorPath path;
	int action_vector = 1; int observation_vector = 1;
	int timespan = 1000;
	for (int t = 1; t < timespan+1; ++t) {
		SensationActionPair *sa = new SensationActionPair();
		sa->t = t;
		for (int a = 0; a < action_vector; ++a) {
			AP_TYPE action = var_norm();
			sa->action.push_back(action);
			fout << action << " ";
		}
		for (int s = 0; s < observation_vector; ++s) {
			AP_TYPE observation = var_norm();
			sa->observation.push_back(observation);
			fout << observation << endl;
		}
		path.push_back(sa);
	}
	fout.close();

	int k = 20;
	cout << "With k=" << k << " and N=" << path.size() << " k/N=" << k/(double)path.size() << endl;
	mi->setK(k);

	PROB_TYPE result = mi->calculate(path);
	cout << "Mutual information is " << result << endl;
}

void TestMutualInformation::Sinus() {

	mi->setMIApproximation(MI_K_NEAREST_NEIGHBOUR);

	SensorimotorPath path;
	int action_vector = 1; int observation_vector = 1;
	int timespan = 500;
	for (int t = 1; t < timespan+1; ++t) {
		SensationActionPair *sa = new SensationActionPair();
		cout << "t=" << t;
		sa->t = t;
		for (int a = 0; a < action_vector; ++a) {
			//AP_TYPE action = (a+1) / (AP_TYPE)(t+action_vector);
			AP_TYPE action = sin(t/100.0+a);
			sa->action.push_back(action);
			cout << " a=" << action;
		}
		for (int s = 0; s < observation_vector; ++s) {
			//AP_TYPE observation = (s+1) / (AP_TYPE)(t+observation_vector);
			AP_TYPE observation = sin(t/100.0+s);
			sa->observation.push_back(observation);
			cout << " s=" << observation;
		}
		cout << endl;
		path.push_back(sa);
	}

	PROB_TYPE m = mi->calculate(path);
	cout << "Mutual information is " << m << endl;
}

void TestMutualInformation::Independent() {
	mi->setMIApproximation(MI_K_NEAREST_NEIGHBOUR);

	SensorimotorPath path;
	int action_vector = 1; int observation_vector = 1;
	int timespan = 500;
	for (int t = 1; t < timespan+1; ++t) {
		SensationActionPair *sa = new SensationActionPair();
		cout << "t=" << t;
		sa->t = t;
		for (int a = 0; a < action_vector; ++a) {
			//AP_TYPE action = (a+1) / (AP_TYPE)(t+action_vector);
			AP_TYPE action = drand48();
			sa->action.push_back(action);
			cout << " a=" << action;
		}
		for (int s = 0; s < observation_vector; ++s) {
			//AP_TYPE observation = (s+1) / (AP_TYPE)(t+observation_vector);
			AP_TYPE observation = drand48();
			sa->observation.push_back(observation);
			cout << " s=" << observation;
		}
		cout << endl;
		path.push_back(sa);
	}

	PROB_TYPE m = mi->calculate(path);
	cout << "Mutual information is " << m << endl;
}


int main() {
	TestMutualInformation tmi;
	tmi.Test();
	return EXIT_SUCCESS;
}



