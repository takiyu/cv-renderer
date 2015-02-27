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
	/* delete lights */
	for(int i = 0; i < lights.size(); i++){
		delete lights[i];
	}
}
void Scene::addObject(Object* obj_ptr){
	this->objects.push_back(obj_ptr);
}
void Scene::addLight(Light* light_ptr){
	this->lights.push_back(light_ptr);
}
void Scene::traceRay(Ray& ray){
	ray.distance = INFINITY;

	/* liner search */
	for(int i = 0; i < objects.size(); i++){
		/* get closest object */
		Intersection intersection;
		bool hit = objects[i]->intersect(ray.org, ray.dir, intersection);
		if(hit && intersection.distance < ray.distance){
			ray.distance = intersection.distance;

			ray.hit = true;
			ray.object_id = i;
			ray.hit_position = intersection.position;
			ray.normal = intersection.normal;
		}
	}
}
