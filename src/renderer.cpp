#include "renderer.h"

using namespace cv;
using namespace std;

Scalar Renderer::BG_COLOR = Scalar(0,0,0); // background color

Renderer::Renderer(){
}
Mat Renderer::renderFrame(const int WIDTH, const int HEIGHT, Camera& camera, Scene& scene){
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

			(*frame_ptr) = render(look_org, look_dir, scene);
		}
	}

	return frame;
}

Scalar Renderer::render(const Point3f& look_org, const Point3f& look_dir, Scene& scene){
	RayInfo ray_info;
	scene.traceRay(look_org, look_dir, ray_info);
	if(!ray_info.hit) return BG_COLOR;

	Scalar color(0,0,0);
	/* direct light */
	for(int i = 0; i < scene.lights.size(); i++){
		RayInfo shadow_info;
		Point3f light_dir = normalize(scene.lights[i]->getPosition() - ray_info.hit_position);
		scene.traceRay(ray_info.hit_position + light_dir*0.00001, light_dir, shadow_info);
		if(shadow_info.hit) continue;

		Scalar L = scene.objects[ray_info.object_id]->sample(light_dir, look_dir, ray_info.normal);
		color += L;
	}

	return color;
}
