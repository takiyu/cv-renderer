#ifndef FPS_H_150217
#define FPS_H_150217

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class FpsCounter{
public:
	FpsCounter();
	void init();
	void updateFrame();
	int getFps(){ return fps; }
private:
	int frame_count;
	int pre_frame_count;
	int64 now_time;
	int64 time_diff;
	int fps;
	double frequency;
	int64 start_time;
};
#endif
