#ifndef MATH_H_150224
#define MATH_H_150224

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>

/* Math */
inline cv::Point3f normalize(const cv::Point3f& src){
	return src * (1.0f/norm(src));
}
inline cv::Point3f reflect(const cv::Point3f& src, const cv::Point3f& n){
	return 2*(src.dot(n)) * n - src;
}
#endif
