/***************************************************************************************************
 * @brief Implementations of the calculation of mutual information
 * @file MutualInformation.h
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
 * @date	May 8, 2012
 * @project	Replicator FP7
 * @company	Almende B.V. & Distributed Organisms B.V.
 * @case	Self-organised criticality
 **************************************************************************************************/

#ifndef MUTUALINFORMATION_H_
#define MUTUALINFORMATION_H_

#include <Structs.h>

enum MIApproximation {
	// "Estimating Mutual Information", by Kraskov, Stögbauer, Grassberger (2003)
	MI_K_NEAREST_NEIGHBOUR,			//< k-nearest neighbour for entropy estimation
	// to-be-done
	// "MI approximation via maximum likelihood estimation of density ratio" by Suzuki et al. (2009)
	MI_DENSITY_RATIO,
	MI_ADAPTIVE_BINNING,  			//< or adaptive histograms
	MI_KERNEL_DENSITY_ESTIMATION,	//< most often using a Gaussian kernel
	MI_EDGEWORTH_EXPANSION,			//< normal distribution + higher-order correction terms
	MI_COUNT
};

enum DistanceMetric { DM_EUCLIDEAN, DM_DOTPRODUCT, DM_TYPES };

/**
 ***************************************************************************************************
 * Mutual information is a useful measure for independence. It can be used:
 * <ul>
 * <li>for information flow from the environment to the robot
 * <li>for information flow from the environment to a group of robots
 * <li>for information from one robot to another
 * <li>for selecting a subset of sensors on a smartphone
 * </ul>
 * There are several approximation methods which can be found in the MIApproximation enumeration.
 * We implement first methods that are useful for sensorimotor settings, and which does not use for
 * example Gaussian's as assumed underlying probability density functions. And we focus first on
 * methods that can be used easily for random variables which are vectors rather than just scalars.
 *
 * The MI_K_NEAREST_NEIGHBOUR method is implemented
 ***************************************************************************************************
 */
class MutualInformation {
public:
	MutualInformation();

	~MutualInformation();

	//! You maybe would expect a function with "RandomVariable X, RandomVariable Y" as parameters
	//! However, considering the fact we want to use it for a sensorimotor trajectory mainly, that
	//! is used as a parameter
	PROB_TYPE calculate(SensorimotorPath &path);

	MIApproximation getMIApproximation() const;

	void setMIApproximation(MIApproximation miApproximation);

	//! Only for kNN approximation
	void setK(int k) { this->k_in_kNN = k; }
protected:
	friend class TestMutualInformation;

	//! Get the kNN approximation
	PROB_TYPE calckNNApproximation(SensorimotorPath &path, int k);

	//! Calculate the distance between two points
	AP_TYPE distance(const Point & p0, const Point & p1, DistanceMetric metric);

	//! Returns maximum distance (either between x0-x1 or between y0-y1)
	AP_TYPE maximumNorm(const Point& x0, const Point& x1, const Point &y0, const Point &y1,
			DistanceMetric metric);

	//! Returns maximum distance
	AP_TYPE maximumNorm(const SensationActionPair* x, const SensationActionPair *y,
			DistanceMetric metric);

	//! Get distance to nearest point in V from point p, this returns e.g. epsilon_x if V == X.
	AP_TYPE getNearestDistance(RandomVariable & V, const Point & p);

	AP_TYPE getNearestDistance(SensorimotorContainer & set, const SensationActionPair& p, int k);

	//! Get the number of neighbours
	void getNeighbourCount(SensorimotorPath & path, const SensationActionPair& p,
			const AP_TYPE dist, int &n_x, int &n_y);

	//! The digamma function, approximated by only a few terms
	AP_TYPE digamma(AP_TYPE x);
private:
	//! The approximation for mutual information that should be used
	MIApproximation mi_approximation;

	//! The only parameter for kNN
	int k_in_kNN;
};


#endif /* MUTUALINFORMATION_H_ */
