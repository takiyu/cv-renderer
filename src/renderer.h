#ifndef RENDERER_H_150224
#define RENDERER_H_150224

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <omp.h>

#include <cstdio>//for rand

#include "math_util.h"
#include "ray.h"
#include "scene.h"
#include "camera.h"
#include "accumulator.h"

class Renderer {
public:
	const cv::Scalar BG_COLOR; // background color

	Renderer();
	cv::Mat renderFrame(const int WIDTH, const int HEIGHT, Camera& camera, Scene& scene, bool accum = false);
private:
	Accumulator accumulator;

	cv::Scalar render(Ray& ray, Scene& scene, int depth_count = 1);
};


#endif
