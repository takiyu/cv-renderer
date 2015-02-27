#ifndef MATH_H_150224
#define MATH_H_150224

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>

/* Math */
inline cv::Point3f normalize(const cv::Point3f& src){
	return src * (1.0f/norm(src));
}

/* Calc reflected vector.
 *  src is incident light and reflected light will be returned.
 *  srcが入射光として反射点方向、反射光は逆向き */
inline cv::Point3f reflect(const cv::Point3f& src, const cv::Point3f& n){
	return src - 2*(src.dot(n)) * n;// nの符号はどちらでも良い
}
#endif
