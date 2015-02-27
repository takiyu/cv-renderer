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
// 	cvtColor(frame, frame, CV_GRAY2BGRA);

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

int main(int argc, char * argv[]){
	/* Add objects */
	const float BOX_SIZE = 8.0f;
	scene.addObject(new Rectangle(Point3f(0, BOX_SIZE/2,0), Point3f(BOX_SIZE,0,0), Point3f(0,0,BOX_SIZE), Scalar(255,255,255)));
	scene.addObject(new Rectangle(Point3f(0,-BOX_SIZE/2,0), Point3f(BOX_SIZE,0,0), Point3f(0,0,BOX_SIZE), Scalar(255,255,255)));

// 	scene.addObject(new Rectangle(Point3f(0,0,BOX_SIZE/2), Point3f(0,BOX_SIZE,0), Point3f(BOX_SIZE,0,0), Scalar(255,255,255)));
	scene.addObject(new Rectangle(Point3f(0,0,-BOX_SIZE/2), Point3f(0,BOX_SIZE,0), Point3f(BOX_SIZE,0,0), Scalar(255,255,255)));

	scene.addObject(new Rectangle(Point3f(BOX_SIZE/2,0,0), Point3f(0,BOX_SIZE,0), Point3f(0,0,BOX_SIZE), Scalar(0,0,255)));
	scene.addObject(new Rectangle(Point3f(-BOX_SIZE/2,0,0), Point3f(0,BOX_SIZE,0), Point3f(0,0,BOX_SIZE), Scalar(0,255,0)));

	scene.addObject(new Sphere(Point3f(-1,-BOX_SIZE/2+1,0), 1, Scalar(0,0,255)));
	scene.addObject(new Sphere(Point3f(1,-BOX_SIZE/2+1,1), 1, Scalar(0,255,0)));
	scene.addObject(new Sphere(Point3f(1,-BOX_SIZE/2+1,-1), 1, Scalar(255,255,255)));

	scene.addLight(new Light(Point3f(0.0,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
	scene.addLight(new Light(Point3f(0.1,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
	scene.addLight(new Light(Point3f(0.2,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
// 	scene.addLight(new Light(Point3f(0.3,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
// 	scene.addLight(new Light(Point3f(0.4,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
// 	scene.addLight(new Light(Point3f(0.5,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
// 	scene.addLight(new Light(Point3f(0.6,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
// 	scene.addLight(new Light(Point3f(0.7,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
// 	scene.addLight(new Light(Point3f(0.8,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
// 	scene.addLight(new Light(Point3f(0.9,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));
// 	scene.addLight(new Light(Point3f(1.0,BOX_SIZE/2*0.99,0), Scalar(255,255,255)));

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
