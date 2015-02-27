#ifndef UNIT_PIXEL_H_150226
#define UNIT_PIXEL_H_150226

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

#include "math_util.h"

class UnitPixel {
public:
	const static int MAX_UNIT_NUM = 10;

	UnitPixel();
	void setScreenSize(int width, int height);
	void clear();
	void update(int x, int y, const cv::Scalar& c, const float weight);
	cv::Scalar get(int x, int y);

private:
	int width, height;
	std::vector<std::vector<cv::Scalar> > data;
	std::vector<std::vector<float> > weights;
};

#endif
