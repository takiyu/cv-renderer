#include "object.h"

using namespace cv;
using namespace std;

Object::Object(){
}
Object::Object(Point3f position, Scalar color) : position(position), color(color){
	this->Kd = 0.6f;
	this->Ks = 0.3f;
	this->glossiness = 8.0f;
}
cv::Scalar Object::sample(const Point3f& light_dir, const Point3f& look_dir, const Point3f& normal){
	float L = 0.0f;
	float Ld = light_dir.dot(normal);
	bool visible = (-look_dir.dot(normal) > 0.0);

	if(!visible){// for neg normal
		Ld *= -1;
		visible = true;
	}
	if(Ld > 0.0f && visible){
		L += Ld*Kd;

		Point3f r = reflect(-light_dir, normal);
		float Ls = Ks * pow(-look_dir.dot(r), glossiness);//ls
		if(Ls > 0){
			L += Ls;
		}
	}
	return (Scalar)(color * L);
}

Sphere::Sphere(){
}
Sphere::Sphere(Point3f position, float radius, Scalar color) : Object(position, color){
	this->radius = radius;
}
bool Sphere::intersect(const Point3f& ray_org, const Point3f& ray_dir, Intersection& dst){
	Point3f rel_pos = this->position - ray_org;

// 	float a = ray_dir.dot(ray_dir); // a = 1
	float b2_neg = ray_dir.dot(rel_pos);
	float c = rel_pos.dot(rel_pos) - radius*radius;

// 	float D_4 = b2_neg*b2_neg - a*c;
	float D_4 = b2_neg*b2_neg - c;

	if(D_4 < 0.0f) return false;

// 	float t1 = (b2_neg - sqrt(D_4))/ a;
// 	float t2 = (b2_neg + sqrt(D_4))/ a;
	float sqrt_D_4 = sqrt(D_4);
	float t1 = (b2_neg - sqrt_D_4);
	float t2 = (b2_neg + sqrt_D_4);

	if(t1 < NEAR_ZERO && t2 < NEAR_ZERO) return false;
	if(t1 > NEAR_ZERO) dst.distance = t1;
	else          dst.distance = t2;

	dst.position = dst.distance*ray_dir + ray_org;

	dst.normal = normalize(dst.position - this->position);

	return true;

// 	Point3f dir_dist = ray_dir.dot(rel_pos) * ray_dir;
//
// 	dst.vertical_vec = dir_dist - rel_pos;
// 	if(norm(dst.vertical_vec) > radius){//miss
// 		dst.position = ray_org + dir_dist;
// 		dst.normal = dir_dist - rel_pos;
// 		dst.distance = norm(dir_dist);
// 		return false;
// 	}
}

Rectangle::Rectangle(){
}
Rectangle::Rectangle(cv::Point3f position, cv::Point3f x_vec, cv::Point3f y_vec, cv::Scalar color) : Object(position, color){
	this->x_basis = normalize(x_vec);
	this->y_basis = normalize(y_vec);
	this->x_norm = norm(x_vec);
	this->y_norm = norm(y_vec);
	this->normal = normalize(this->x_basis.cross(this->y_basis));
}
bool Rectangle::intersect(const Point3f& ray_org, const Point3f& ray_dir, Intersection& dst){
	Point3f rel_pos = this->position - ray_org;

	dst.normal = this->normal;
	dst.distance = (rel_pos.dot(dst.normal) / ray_dir.dot(dst.normal));
	if(dst.distance <= NEAR_ZERO) return false;

	dst.position = dst.distance*ray_dir + ray_org;

	// Check range
	Point3f local_pos = dst.position - this->position;
	float x_len = local_pos.dot(this->x_basis) / this->x_norm;
	if(x_len < -0.5f || 0.5f < x_len) return false;
	float y_len = local_pos.dot(this->y_basis) / this->y_norm;
	if(y_len < -0.5f || 0.5f < y_len) return false;

	return true;
}


Light::Light(){
}
Light::Light(Point3f position, Scalar emission) : position(position), emission(emission * EMISSION_SCALE){
}
