#include "camera.h"

using namespace cv;
using namespace std;

Camera::Camera() : from(0,0,4), to(0,0,0), up(0,1,0), fov(90) {
}
Camera::Camera(const Point3f& from) : from(from), to(0,0,0), up(0,1,0), fov(90) {
}
void Camera::move(float dx, float dy, float dz){
	from.x += dx;
	from.y += dy;
	from.z += dz;
}
void Camera::rotateOrbit(float dtheta, float dphi){
	Point3f dir = this->to - this->from;
	float dir_norm = norm(dir);
	dir *= 1.0/dir_norm;//normalize

	float theta = atan2f(dir.x, dir.z);
	float phi   = asinf(dir.y);
	theta += dtheta;
	phi   += dphi;

	dir.x = cosf(phi)*sinf(theta);
	dir.y = sinf(phi);
	dir.z = cosf(phi)*cosf(theta);

	dir *= dir_norm;//reverce normalize
	this->from = this->to - dir;
}
void Camera::getScreenInf(const int width, const int height, Point3f& dir_base, Point3f& x_vec, Point3f& y_vec){
	float screen_half_width = abs(SCREEN_DIST * tanf(0.5f*this->fov * M_PI/180));
	float scale = (screen_half_width * 2.0f) / width;

	Point3f dir = this->to - this->from;

	x_vec = normalize(dir.cross(up));
	x_vec *= scale;//normalize

	y_vec = normalize(dir.cross(x_vec));
	y_vec *= scale;//normalize

	dir_base = dir * (1.0/norm(dir));
	dir_base -= y_vec * (height*0.5f);
	dir_base -= x_vec * (width *0.5f);
}
