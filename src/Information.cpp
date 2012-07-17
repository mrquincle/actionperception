/**
 * @brief 
 * @file Information.cpp
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
 * @date	Apr 17, 2012
 * @project	Replicator FP7
 * @company	Almende B.V.
 * @case	Self-organised criticality
 */

#include <Information.h>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <iostream>

using namespace std;

InfoType Information::getInfoType() const
{
    return info_type;
}

Information::Information()
{
}

Information::~Information()
{
}

void Information::setInfoType(InfoType infoType)
{
    info_type = infoType;
}

int Information::Calculate()
{
	switch (info_type) {
	case IT_EMPOWERMENT:
		CalculateEmpowerment();
		break;
	default:
		cerr << "Unknown information type" << endl;
		break;
	}
	return 0;
}

int Information::CalculateEmpowerment() {

}


/**********************************************************************************************
 * Helper functions that can operate on standard containers.
 *********************************************************************************************/

/**
 * The norm for a N-dimensional vector, element-wise squaring the difference.
 */
AP_TYPE euclidean(AP_TYPE x, AP_TYPE y) {
	return (x-y)*(x-y);
}

/**
 * Create a template function which moves vector x from or towards y with a learning rate "mu".
 * A positive mu will move "x" away, while a negative mu will move "x" towards "y".
 */
//template<typename T>
//class op_adjust: std::binary_function<T,T,T> {
//	T mu_;
//public:
//	op_adjust(T mu): mu_(mu) {}
//	T operator()(T x, T y) const {
//		T result = x+(x-y)*mu_;
//		return result;
//	}
//};

template<class T>
T likeliness(T sum, T value) {
	return sum+value*log10(value);
}

/**********************************************************************************************
 * Helper functions that calculate standard information-metrics like entropy
 * They operate on the RandomVariable structure
 *********************************************************************************************/

int Information::Uncertainty(Point & var) {
	Point::iterator i;
	return -accumulate(var.begin(), var.end(), PROB_TYPE(0), likeliness<PROB_TYPE>);
}

//void Information::JointProbability(Point &x, Point &y, Point &xy) {
//
//}

/**
 * H(Y|X) = - SUM_x SUM_y p(x,y) log { p(x) / p(x,y) }
 */
//int Information::ConditionalEntropy(Point &x, Point &y {
//
//}



/**
 * This function tells something about the "distance" between vectors, in other words the similarity or
 * dissimilarity. There are currently several metrics implemented:
 *   DM_DOTPRODUCT:		return sum_i { x_i*w_i }
 *   DM_EUCLIDEAN:		return sum_i { (x_i-w_i)^2 }
 * It is assumed that the prototype size is equal to the aspect size.
 * @param aspect		in: incoming value
 * @param prototype		in: prototype to check against
 * @param metric		in: a certain distance metric
 * @return				out: the distance between aspect and prototype
 */
//AP_TYPE Information::Distance(const RandomVariable & aspect, const RandomVariable & prototype, DistanceMetric metric) {
//	if (aspect.size() != prototype.size()) {
//		cerr << "Aspect size " << aspect.size() << " while prototype size " << prototype.size() << endl;
//		assert (aspect.size() == prototype.size());
//	}
//	switch (metric) {
//	case DM_DOTPRODUCT:
//		return inner_product(aspect.begin(), aspect.end(), prototype.begin(), AP_TYPE(0));
//	case DM_EUCLIDEAN:
//		return inner_product(aspect.begin(), aspect.end(), prototype.begin(), AP_TYPE(0), plus<AP_TYPE>(), euclidean);
//	default:
//		cerr << "Unknown distance metric" << endl;
//		return -1;
//	}
//}

