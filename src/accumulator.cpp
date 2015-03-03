#include "accumulator.h"

using namespace cv;
using namespace std;

Accumulator::Accumulator(){
	width = height = 0;
}
void Accumulator::setScreenSize(int width, int height){
	if(width == this->width && height == this->height) return;
	this->width = width;
	this->height = height;

	data.clear();
	weights.clear();
	unsigned int w = width, h = height;
	for(int i = 0; i < MAX_UNIT_NUM; i++){
		int size = (w+1)*(h+1);
		vector<Scalar> tmp1(size);
		vector<float> tmp2(size);
		data.push_back(tmp1);
		weights.push_back(tmp2);

		w >>= 1; h >>= 1;
		if(w == 0 || h == 0) break;
	}
}
void Accumulator::clear(){
	for(int i = 0; i < data.size(); i++){
		for(int j = 0; j < data[i].size(); j++){
			data[i][j] = Scalar(0,0,0,0); 
			weights[i][j] = 0;
		}
	}
}
void Accumulator::update(int x, int y, const Scalar& c, const float weight){
	if(weight > 0) {
		//update pixel maps
		for(int i = 0; i < data.size(); i++){
			int idx = (y>>i)*(width>>i)+(x>>i);
			data[i][idx] += c; 
			weights[i][idx] += weight;
		}
	}
}
Scalar Accumulator::get(int x, int y){
	//search least scale effective pixel
	for(int i = 0; i < data.size(); i++){
		int idx = (y>>i)*(width>>i)+(x>>i);
		if(weights[i][idx] > 0) return (Scalar)(data[i][idx]/weights[i][idx]);
	}
// 	return Scalar(0,0,255);//for Debug
	return Scalar(0,0,0);
}
