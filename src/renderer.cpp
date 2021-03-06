#include "renderer.h"

using namespace cv;
using namespace std;


Renderer::Renderer() : BG_COLOR(0,0,0){
}
Mat Renderer::renderFrame(const int WIDTH, const int HEIGHT, Camera& camera, Scene& scene, bool accum){
	/* accumulator settings */
	this->accumulator.setScreenSize(WIDTH, HEIGHT);
	if(!accum) this->accumulator.clear();

	/* rendering frame */
	Mat frame(HEIGHT, WIDTH, CV_8UC4);

	/* camera Info */
	Point3f dir_base, x_vec, y_vec;
	Point3f look_org = camera.getOrg();
	camera.getScreenInf(WIDTH, HEIGHT, dir_base, x_vec, y_vec);

	/* render each pixel */
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int y = 0; y < HEIGHT; y++){
		Vec4b* frame_ptr = frame.ptr<Vec4b>(y);
		for(int x = 0; x < WIDTH; x++, frame_ptr++){
			Point3f look_dir = normalize(dir_base + (x_vec*x + y_vec*y));

			const int spp = 1;
			Scalar color(0,0,0);
			for(int i = 0; i < spp; i++){
				Ray ray(look_org, look_dir);
				color += render(ray, scene);
			}

			/* update and get accumulator */
			this->accumulator.update(x, y, color, spp);
			(*frame_ptr) = this->accumulator.get(x, y) * 255;
		}
	}
	return frame;
}


Scalar Renderer::render(Ray& ray, Scene& scene, int depth_count){
	scene.traceRay(ray);
	if(!ray.hit) return BG_COLOR;

	/* indirect light */
	Scalar ambient_color(0,0,0);
	if(depth_count > 0){
		Point3f reflected = reflect(ray.dir, ray.normal);//norm(reflected) == 1

		Point3f w, u, v;
		w = ray.normal;
		if(w.dot(reflected) < 0.0f) w *= -1;
		if (fabs(w.x) > 0.001) u = normalize(Point3f(0.0f, 1.0f, 0.0f).cross(w));
		else                   u = normalize(Point3f(1.0f, 0.0f, 0.0f).cross(w));
		v = w.cross(u);

		float r1 = float(rand()) / float(RAND_MAX) * 2 * M_PI;
		float r2 = float(rand()) / float(RAND_MAX);
		float sqrt_r2 = sqrtf(r2);
		Point3f dir = normalize((
					u * cos(r1) * sqrt_r2 +
					v * sin(r1) * sqrt_r2 +
					w * sqrt(1.0 - r2)));

		Ray n_ray(ray.hit_position, dir);

		Scalar L = scene.objects[ray.object_id]->sample(dir, ray.dir, ray.normal);
		ambient_color += L.mul(render(n_ray, scene, depth_count-1));//pdf : cos
	}

	/* direct light */
	Scalar direct_color(0,0,0);
	for(int i = 0; i < scene.lights.size(); i++){
		Point3f light_rel_pos = scene.lights[i]->getPosition() - ray.hit_position;
		Point3f light_dir = normalize(light_rel_pos);

		Ray n_ray(ray.hit_position, light_dir);
		scene.traceRay(n_ray);
		if(n_ray.hit && n_ray.distance < norm(light_rel_pos)) continue;

		Scalar L = scene.objects[ray.object_id]->sample(light_dir, ray.dir, ray.normal);
		direct_color += L.mul(scene.lights[i]->getEmissionRate());
	}
	direct_color *= 1.0f/scene.lights.size();

	return ambient_color + direct_color;
}

