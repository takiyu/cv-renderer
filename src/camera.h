#ifndef CAMERA_H_150217
#define CAMERA_H_150217

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "math_util.h"

class Camera {
public:
	const static float SCREEN_DIST = 1.0f;

	Camera();
	Camera(const cv::Point3f& from);
	void move(float dx, float dy, float dz);
	void rotateOrbit(float dtheta, float dphi);
	cv::Point3f getOrg(){ return from; }
	void getScreenInf(const int width, const int height,
	                  cv::Point3f& dir_base, cv::Point3f& x_vec, cv::Point3f& y_vec);
private:
	cv::Point3f from, to, up;
	float fov;//field of view
};
#endif
