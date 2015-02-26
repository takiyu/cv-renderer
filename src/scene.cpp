#include "scene.h"

using namespace cv;
using namespace std;

Scene::Scene(){
}
Scene::~Scene(){
	/* delete objects */
	for(int i = 0; i < objects.size(); i++){
		delete objects[i];
	}
}
void Scene::addObject(Object* obj_ptr){
	this->objects.push_back(obj_ptr);
}
void Scene::addLight(Light* light_ptr){
	this->lights.push_back(light_ptr);
}
void Scene::traceRay(const cv::Point3f& org, const cv::Point3f& dir, RayInfo& ray_info){
	ray_info.hit = false;
	float min_distance = INFINITY;

	/* liner search */
	for(int i = 0; i < objects.size(); i++){
		/* get closest object */
		IntersectInfo inter_info;
		bool hit = objects[i]->intersect(org, dir, inter_info);
		if(hit && inter_info.distance < min_distance){
			ray_info.hit = true;
			ray_info.object_id = i;
			ray_info.hit_position = inter_info.position;
			ray_info.normal = inter_info.normal;
		}
	}
}
