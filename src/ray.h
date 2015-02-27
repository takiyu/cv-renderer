#ifndef RAY_H_150226
#define RAY_H_150226

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "math_util.h"

struct Ray {
	Ray(const cv::Point3f& org, const cv::Point3f& dir) : org(org), dir(dir){
		hit = false;
	}
	const cv::Point3f org, dir;

	/* Result variables */
	bool hit;
	int object_id;
	cv::Point3f hit_position;
	cv::Point3f normal;
	float distance;
};

struct Intersection {
	cv::Point3f position; // hit position
	cv::Point3f normal;
	float distance;
// 	cv::Point3f vertical_vec;// vertical vector to the ray
};

#endif
