#ifndef RENDERER_H_150224
#define RENDERER_H_150224

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <omp.h>

#include "scene.h"
#include "camera.h"

class Renderer {
public:
	static cv::Scalar BG_COLOR; // background color

	Renderer();
	cv::Mat renderFrame(const int WIDTH, const int HEIGHT, Camera& camera, Scene& scene);
private:
	cv::Scalar render(const cv::Point3f& look_org, const cv::Point3f& look_dir, Scene& scene);
};

#endif
