#ifndef SCENE_H_150224
#define SCENE_H_150224

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "object.h"

struct RayInfo {
	bool hit;
	int object_id;
	cv::Point3f hit_position;
	cv::Point3f normal;
};

class Scene {
public:
	Scene();
	~Scene();
	void addObject(Object* obj_ptr);
	void addLight(Light* light_ptr);
	void traceRay(const cv::Point3f& org, const cv::Point3f& dir, RayInfo& ray_info);

	std::vector<Object*> objects;
	std::vector<Light*> lights;
private:
};

#endif
