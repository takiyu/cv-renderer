#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <GL/gl.h>   
#include <GL/glut.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include "math_util.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"
#include "fps.h"

using namespace cv;
using namespace std;

/* Main */
Renderer renderer;
Camera camera;
Scene scene;
FpsCounter fps;

bool accum = false;

/* GLUT */
int WIDTH = 640;
int HEIGHT = 480;
int WINDOW_ID;

int mouse_mode = 0;
int mouse_x, mouse_y;
void displayFunc(){

	Mat frame = renderer.renderFrame(WIDTH, HEIGHT, camera, scene, accum);
	accum = true;

	flip(frame, frame, 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(WIDTH, HEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, frame.data);
	glFlush();

	fps.updateFrame();
	cout << "fps:" << fps.getFps() << endl;
}
void reshapeFunc(int w, int h){
	if(WIDTH == w && HEIGHT == h) return;
	WIDTH = w; HEIGHT = h;
	glViewport(0, 0, WIDTH, HEIGHT);
}
void idleFunc(){
	glutPostRedisplay();
}
void keyboardFunc(unsigned char k, int, int){
	switch (k){
		case '\033': case 'q': case 'Q':
		   glutDestroyWindow(WINDOW_ID);
		   exit(0);
		   break;
	}
}
void clickFunc(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON) mouse_mode = 1;
	mouse_x = x; mouse_y = y;
}
void motionFunc(int x, int y){
	float dClickX = float(mouse_x - x), dClickY = float(mouse_y - y);
	mouse_x = x; mouse_y = y;

	switch(mouse_mode){
		case 1:
			camera.rotateOrbit(0.005f*dClickX,0.005f*dClickY);
			break;
	}
	accum = false;
}

void addBox(Point3f pos, Point3f vx, Point3f vy, Point3f vz, Scalar color, Scene& scene){
	scene.addObject(new Rectangle(pos+vx*0.5, vy, vz, color));
	scene.addObject(new Rectangle(pos-vx*0.5, vy, vz, color));
	scene.addObject(new Rectangle(pos+vy*0.5, vx, vz, color));
	scene.addObject(new Rectangle(pos-vy*0.5, vx, vz, color));
	scene.addObject(new Rectangle(pos+vz*0.5, vx, vy, color));
	scene.addObject(new Rectangle(pos-vz*0.5, vx, vy, color));
}

int main(int argc, char * argv[]){
	/* Add objects */
	const float B0 = 6.0f;
	scene.addObject(new Rectangle(Point3f(0, B0/2,0), Point3f(B0,0,0), Point3f(0,0,B0), Scalar(1,1,1)));
	scene.addObject(new Rectangle(Point3f(0,-B0/2,0), Point3f(B0,0,0), Point3f(0,0,B0), Scalar(1,1,1)));
// 	scene.addObject(new Rectangle(Point3f(0,0,B0/2), Point3f(0,B0,0), Point3f(B0,0,0), Scalar(1,1,1)));
	scene.addObject(new Rectangle(Point3f(0,0,-B0/2), Point3f(0,B0,0), Point3f(B0,0,0), Scalar(1,1,1)));
	scene.addObject(new Rectangle(Point3f(B0/2,0,0), Point3f(0,B0,0), Point3f(0,0,B0), Scalar(0.25,0.25,0.75)));
	scene.addObject(new Rectangle(Point3f(-B0/2,0,0), Point3f(0,B0,0), Point3f(0,0,B0), Scalar(0.25,0.75,0.25)));


	/* Box */
	const float B1 = 1.6f;
	addBox(Point3f(1.1,(-B0+B1*2)*0.5,-1.2), Point3f(B1,0,1.0), Point3f(0,B1*2,0), Point3f(-1.0,0,B1), Scalar(1,1,1), scene);
	/* Sphere */
	const float RADIUS = 0.8;
	scene.addObject(new Sphere(Point3f(-RADIUS, -B0/2+RADIUS,       0), RADIUS, Scalar(1,1,1)));
	/* Light */
	const int light_num = 2;
	for(int i = 0; i < light_num; i++){
		for(int j = 0; j < light_num; j++){
			scene.addLight(new Light(Point3f(i*0.05,B0/2*0.99,j*0.05), Scalar(1,1,1)));
		}
	}

	/* GLUT */
	glutInit(&argc, argv);
    glutInitWindowSize((GLsizei)WIDTH, (GLsizei)HEIGHT);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
	WINDOW_ID = glutCreateWindow("title");

    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(displayFunc);
    glutIdleFunc(idleFunc);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(clickFunc);
    glutMotionFunc(motionFunc);

    glutMainLoop();
	return 0;
}
