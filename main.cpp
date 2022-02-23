#include "stdafx.h"
#include <stdlib.h>
#include "./GL/glut.h"
#include <Windows.h>
#include <vector>
#include <iostream>
#include <algorithm>

#pragma comment(lib, "glut32.lib")
#define WINDOW_TITLE "Math Project"

using namespace std;

GLsizei width, height;

float Qpercent = 0.25;
float Rpercent = 0.75;

struct Point {
	float x, y;
	Point() : x(0), y(0){}
	Point(const float a, const float b)
		:x(a), y(b){}
	Point(const Point& p) : x(p.x), y(p.y) {}
};

std::vector<Point> Points;


void display()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(1, 0, 0); // color of lines
	glBegin(GL_LINE_STRIP);
	int i = 0;
	while (i < Points.size())
	{
		glVertex2f(Points.at(i).x, Points.at(i).y);
		glVertex2f(Points.at((i + 1) % Points.size()).x, Points.at((i + 1) % Points.size()).y);
		i++;
	}

	glEnd();
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, (float)w, (float)h, 0.0, -1.0, 1.0);

	width = w;  // width global
	height = h; // height global

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
			{
				Points.push_back(Point(x, y));
				std::cout << "Point added at: (" << x << ", " << y << ")" << std::endl;

			}
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void motion(int x, int y)
{
	glutPostRedisplay();
}

void setup()
{
	glClearColor(1.0, 1.0, 1.0, 1.0); // *should* display black background
}

bool initialize()
{
	glEnable(GL_DEPTH_TEST);
	return true;
}


void AlgoChaikin(float a, float b) {
	std::vector<Point> NewPoints;
	for (unsigned int i = 0; i <= Points.size(); ++i) {

		const Point& p0 = Points[(i) % Points.size()];
		const Point& p1 = Points[(i + 1) % Points.size()];
		Point Q;
		Point R;
		Q.x = (1 - a) * p0.x + a * p1.x;
		Q.y = (1 - a) * p0.y + a * p1.y;

		R.x = (1 - b) * p0.x + b * p1.x;
		R.y = (1 - b) * p0.y + b * p1.y;

		NewPoints.push_back(Q);
		NewPoints.push_back(R);
	}

	Points = NewPoints;
}

void keyboard(unsigned char key, int, int) {
	switch (key) {
	case 'c':
		AlgoChaikin(Qpercent, Rpercent);
		break;
	case '-':
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Menu(int value) {
	switch (value) {
	case 1:
		Points.clear();
		break;
	case 2: // Exit
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 499.0, 0.0, 499.0);

	std::cout << "Enter both Q and R values (both summed must equal to 1)" << std::endl;
	cin >> Qpercent;
	cin >> Rpercent;
	while (Qpercent + Rpercent != 1.0f) {
		std::cout << "Your values summed did not equal 1" << std::endl;
		std::cout << "Enter both Q and R values (both summed must equal to 1)" << std::endl;
		cin >> Qpercent;
		cin >> Rpercent;
	}
	std::cout << "Click to add points, and then press C to use the algorithm." << std::endl;

}

int main(int argc, char** argv)
{
	if (!initialize()) return 1;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow(WINDOW_TITLE);	
	glutCreateMenu(Menu);
	glutAddMenuEntry("Clear Points", 1);
	glutAddMenuEntry("Exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	init();
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	
	glutMainLoop();
	return 0;
}
