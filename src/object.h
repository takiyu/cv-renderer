#ifndef OBJECT_H_150224
#define OBJECT_H_150224

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "math_util.h"
#include "ray.h"

// enum MaterialType {
// 	MATERIAL_TYPE_NORMAL, // 完全拡散面
// 	MATERIAL_TYPE_MIRROR, // 鏡面
// 	MATERIAL_TYPE_GLASS,  // ガラス的物質
// };

/* Base Object Class */
class Object {
public:
	const static float NEAR_ZERO = 0.0001;

	Object();
	Object(cv::Point3f position, cv::Scalar color);
	cv::Scalar getColor(){ return color; }
	virtual bool intersect(const cv::Point3f& ray_org, const cv::Point3f& ray_dir, Intersection& dst) = 0;
	cv::Scalar sample(const cv::Point3f& light_dir, const cv::Point3f& look_dir, const cv::Point3f& normal);
protected:
	cv::Point3f position;
	cv::Scalar color;
	float Kd;//拡散反射率
	float Ks;//鏡面反射率
	float glossiness;//光沢度
};

/* Sphere */
class Sphere : public Object {
public:
	Sphere();
	Sphere(cv::Point3f position, float radius, cv::Scalar color);
	virtual bool intersect(const cv::Point3f& ray_org, const cv::Point3f& ray_dir, Intersection& dst);
protected:
	float radius;
};

/* Rectangle */
class Rectangle : public Object {
public:
	Rectangle();
	Rectangle(cv::Point3f position, cv::Point3f x_vec, cv::Point3f y_vec, cv::Scalar color);
	virtual bool intersect(const cv::Point3f& ray_org, const cv::Point3f& ray_dir, Intersection& dst);
protected:
	cv::Point3f x_basis, y_basis;
	float x_norm, y_norm;
	cv::Point3f normal;
};


/* Light */
class Light {
public:
	Light();
	Light(cv::Point3f position, cv::Scalar emission);
	cv::Point3f getPosition(){ return position; }
	cv::Scalar getEmissionRate(){ return emission; }
protected:
	cv::Point3f position;
	cv::Scalar emission;
};


#endif
