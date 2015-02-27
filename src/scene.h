#ifndef SCENE_H_150224
#define SCENE_H_150224

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "math_util.h"
#include "ray.h"
#include "object.h"

class Scene {
public:
	Scene();
	~Scene();
	void addObject(Object* obj_ptr);
	void addLight(Light* light_ptr);
	void traceRay(Ray& ray);

	std::vector<Object*> objects;
	std::vector<Light*> lights;
private:
};

#endif
