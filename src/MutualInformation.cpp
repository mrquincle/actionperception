/***************************************************************************************************
 * @brief Implementations of mutual information
 * @file MutualInformation.cpp
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

#include <MutualInformation.h>

#include <functional>
#include <numeric>
#include <limits>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <math.h>

#include <boost/bind.hpp>
#include <boost/math/special_functions/digamma.hpp>

using namespace std;

/**
 * The norm for a N-dimensional vector, element-wise squaring the difference.
 */
template<class T>
T euclidean(T x, T y) {
	return (x-y)*(x-y);
}

MutualInformation::MutualInformation() {
	k_in_kNN = 6;
}

MutualInformation::~MutualInformation() {

}

MIApproximation MutualInformation::getMIApproximation() const {
	return mi_approximation;
}

void MutualInformation::setMIApproximation(MIApproximation miApproximation) {
	mi_approximation = miApproximation;
}

/**
 * Strictly positive return value (mutual information), except when there is an error (at which the
 * return values is -1.0).
 */
PROB_TYPE MutualInformation::calculate(SensorimotorPath &path) {
	switch (mi_approximation) {
	case MI_K_NEAREST_NEIGHBOUR:
		return calckNNApproximation(path, k_in_kNN);
		break;
	default:
		cerr << "Not implemented (yet), sorry!" << endl;
		break;
	}
	return PROB_TYPE(-1.0);
}

/**
 * The estimate is:
 * I(X,Y) = ψ(k)−1/N*sum_i{ψ(nx+1)+ψ(ny+1)}+ψ(N) with ψ the digamma function.
 * To get some feeling about what Kraskov et al. have been aiming at... Suppose two variables are highly
 * correlated, then the points of the Z=(X,Y) distribution will be on a diagonal line. The k-nearest
 * neighbour metric as defined will only find neighbours on that line, because there are only a few points
 * that are off-line. Hence, on average we might expect to find 2*k neighbours in the y-direction and also
 * (the same) 2*k neighbours in the x-direction.
 */
PROB_TYPE MutualInformation::calckNNApproximation(SensorimotorPath &path, int k) {
	PROB_TYPE digamma_nx_ny = 0;
	assert (path.size() > k-1);
	SensorimotorPath::iterator it;
	SensorimotorContainer set;
	for (it = path.begin(); it != path.end(); ++it) {
		set.push_back(*it);
	}
	for (it = path.begin(); it != path.end(); ++it) {
		PROB_TYPE dist = getNearestDistance(set, **it, k);
		int nx = 0, ny = 0;
		getNeighbourCount(path, **it, dist, nx, ny);
//		cout << "Nearest (k=" << k << ")-distance for " << (*it)->t << " = " << dist << endl;
//		cout << "The number of neighbours at this distance is " << nx << "+" << ny << endl;
		// we do not need to add 1 here, because the point itself is calculated towards its neighbours :-)
		digamma_nx_ny += digamma(nx+1)+digamma(ny+1);
//		digamma_nx_ny += digamma(nx)+digamma(ny);
	}
	digamma_nx_ny /= (PROB_TYPE)path.size();
	PROB_TYPE digamma_k = digamma((PROB_TYPE)k);
	PROB_TYPE digamma_N = digamma((PROB_TYPE)path.size());
	cout << "Calculate ψ(k)−1/N*sum_i{ψ(nx+1)+ψ(ny+1)}+ψ(N)=" << digamma_k << "-" << digamma_nx_ny << "+" << digamma_N << endl;
	return digamma_k - digamma_nx_ny + digamma_N;
}

/**
 * Calculate the distance between two points. So, this returns the distance between e.g. two sensor values.
 */
AP_TYPE MutualInformation::distance(const Point& p0, const Point& p1,
		DistanceMetric metric) {
	if (p0.size() != p1.size()) {
		cerr << "Points do not have the same dimension" << endl;
		assert (p0.size() == p1.size());
	}
	switch (metric) {
	case DM_DOTPRODUCT:
		return inner_product(p0.begin(), p0.end(), p1.begin(), AP_TYPE(0));
	case DM_EUCLIDEAN:
		return sqrt(inner_product(p0.begin(), p0.end(), p1.begin(), AP_TYPE(0), plus<AP_TYPE>(), euclidean<AP_TYPE>));
	default:
		cerr << "Unknown distance metric" << endl;
		return -1;
	}
}

/**
 * The maximum norm is used on the space Z=(X,Y).
 */
AP_TYPE MutualInformation::maximumNorm(const Point& x0, const Point& x1, const Point &y0, const Point &y1,
		DistanceMetric metric) {
	return max<AP_TYPE>(distance(x0,x1,metric),distance(y0, y1,metric));
}

/**
 * The same maximum norm function, but now with SensationActionPair's as input, so it can be used for sorting.
 * Take notice that pointers are used, and the entities are not passed by references. This screws up the boost::bind
 * functionality for member functions where this->&blah is not funny. :-)
 */
AP_TYPE MutualInformation::maximumNorm(const SensationActionPair* x, const SensationActionPair *y,
		DistanceMetric metric) {
	if (x->t == y->t) {
		return numeric_limits<AP_TYPE>::max();
	}
	AP_TYPE d0 = distance(x->observation,y->observation, metric);
	AP_TYPE d1 = distance(x->action, y->action, metric);
//	cout << "Maximum norm between " << x->t << " and " << y->t << " is maximum of: " << d0 << " and " << d1 << endl;
	return max<AP_TYPE>(d0,d1);
}



/**
 *
 */
AP_TYPE MutualInformation::getNearestDistance(RandomVariable & V, const Point& p) {
	assert (V.size() > 0);
	RandomVariable::iterator it;
	AP_TYPE winner_value = numeric_limits<AP_TYPE>::max();
	for (it = V.begin(); it != V.end(); ++it) {
		AP_TYPE dist = distance(**it, p, DM_EUCLIDEAN);
		if (dist < winner_value) {
			winner_value = dist;
		}
	}
	return winner_value;
}

/**
 * Get nearest point in the "2D" space where the distance is "minimal". The maximum value for the action
 * or perception dimension is taken as distance measure. So, for a point at [0,0], and a neighbour of
 * [0.5, 0.8] the distance is 0.8.
 */
AP_TYPE MutualInformation::getNearestDistance(SensorimotorContainer & set, const SensationActionPair& p, int k) {
	assert (set.size() > k-1);
	SensorimotorContainer::iterator it;
	if (k == 1) {
		AP_TYPE winner_value = numeric_limits<AP_TYPE>::max();
		for (it = set.begin(); it != set.end(); ++it) {
			AP_TYPE dist = maximumNorm((*it)->observation, p.observation, (*it)->action, p.action, DM_EUCLIDEAN);
			if (dist < winner_value) {
				winner_value = dist;
			}
		}
		return winner_value;
	} else {
		// use boost::bind for sorting with a member function with additional argument
	    sort(set.begin(), set.end(),
	    		boost::bind<AP_TYPE>(&MutualInformation::maximumNorm, this, _1, &p, DM_EUCLIDEAN) <
	    		boost::bind<AP_TYPE>(&MutualInformation::maximumNorm, this, _2, &p, DM_EUCLIDEAN)
	    );
//		cout << "Sorted: " << endl;
//		it = set.begin();
//		int i = 0;
//		for (it = set.begin(); it != set.end(); ++it, ++i) {
//			AP_TYPE maxnorm  = maximumNorm((*it)->observation, p.observation, (*it)->action, p.action, DM_EUCLIDEAN);
//			AP_TYPE d0 = distance((*it)->observation, p.observation, DM_EUCLIDEAN);
//			AP_TYPE d1 = distance((*it)->action, p.action, DM_EUCLIDEAN);
//			cout << "Neighbour [" << i << "]: ";
//			cout << (*it)->action.front() << " (" << d1 << "), ";
//			cout << (*it)->observation.front() << " (" << d0 << "), ";
//			cout << "maxnorm = [" << maxnorm << "]  " << endl;
//		}
		it = set.begin();
		std::advance(it, k-1);
		AP_TYPE result = maximumNorm((*it)->observation, p.observation, (*it)->action, p.action, DM_EUCLIDEAN);
		return result;
	}
}

/**
 * Get the number of neighbouring points which are not further away than "distance" from point p. For a point
 * at [0,0] and a closest neighbour at [0.5,0.8] the distance is 0.8. All points that are between [-0.8,*]
 * and [0.8,*] and between [*,-0.8] and [*,0.8] will be counted as "neighbours".
 */
void MutualInformation::getNeighbourCount(SensorimotorPath & path, const SensationActionPair& p,
		const AP_TYPE dist, int &n_x, int &n_y) {
	SensorimotorPath::iterator it;
	AP_TYPE winner_value = numeric_limits<AP_TYPE>::max();
	for (it = path.begin(); it != path.end(); ++it) {
		// strictly less than the distance and also exclude the point itself (distance is 0)
		AP_TYPE calc_dist;
		calc_dist = distance((*it)->observation, p.observation, DM_EUCLIDEAN);
		if ((calc_dist < dist) && (calc_dist != 0)) {
//			cout << "Found neighbour at " << (*it)->observation.front() << endl;
			n_x++;
		}
		calc_dist = distance((*it)->action, p.action, DM_EUCLIDEAN);
		if ((calc_dist < dist) && (calc_dist != 0)) {
//			cout << "Found neighbour at " << (*it)->action.front() << endl;
			n_y++;
		}
	}
}

/**
 * According to J.M. Bernardo AS 103 algorithm[3] the digamma function for x, a real number, can be approximated by:
 * \psi(x) = \ln(x) - \frac{1}{2x} - \frac{1}{12x^2} + \frac{1}{120x^4} - \frac{1}{252x^6} + O\left(\frac{1}{x^8}\right)
 * From: http://en.wikipedia.org/wiki/Digamma_function
 */
AP_TYPE MutualInformation::digamma(AP_TYPE x) {
	return boost::math::digamma(x);
// possible approximation if boost libraries are not available
//	return log(x) - 1/(2*x) - 1/(12*x*x) + 1/(120*x*x*x*x) - 1/(252*x*x*x*x*x*x);
}
