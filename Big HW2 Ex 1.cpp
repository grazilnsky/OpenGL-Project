#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <gl\GL.h>
#include "GLUT\include\GL\glut.h"
#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "glut32.lib" )

using namespace std;

class vec4f {
public:
	float buffer[4];
public:
	vec4f(float x, float y, float z, float w) {
		buffer[0] = x;
		buffer[1] = y;
		buffer[2] = z;
		buffer[3] = w;
	}
	vec4f(float* buffer) {
		for (int i = 0; i < 4; i++) {
			this->buffer[i] = buffer[i];
		}
	}
};

class matrix4fv {
private:
	float* buffer;
public:
	matrix4fv(float *buffer) : buffer(buffer) {}
	matrix4fv() {
		delete buffer;
	}
	vec4f operator*(vec4f& other) {
		float buffer[4];
		for (int i = 0; i < 4; i++) {
			buffer[i] = 0.0f;
			for (int j = 0; j < 4; j++) {
				buffer[i] += this->buffer[i * 4 + j] * other.buffer[j];
			}
		}
		return vec4f(buffer);
	}

};

float lx = 0.0f, lz = -1.0f, ly = 0.0f;
float x = 0.0f, z = 5.0f, y = 1.0f;
float randXcord, randYcord, randZcord;
double angleH = 0.0f, angleV = 0.0f;
int max = 200;
bool triangle, square, circle;

void drawTriangle()
{
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
}
void drawSquare()
{
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(2.0, 1.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(3.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();
}

void drawCircle(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i < num_segments; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);

		float x = r * cosf(theta);
		float y = r * sinf(theta);

		glVertex2f(x + cx, y + cy);

	}
	glEnd();
}

void changeSize(int w, int h)
{
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45.0f, ratio, 0.1f, 40.0f);

	glMatrixMode(GL_MODELVIEW);
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-4.0f, 0.0f, -4.0f);
	glVertex3f(-4.0f, 0.0f, 4.0f);
	glVertex3f(4.0f, 0.0f, 4.0f);
	glVertex3f(4.0f, 0.0f, -4.0f);
	glEnd();


	if (triangle)
	{
		glPushMatrix();
		glTranslatef(randXcord, randYcord, randZcord);
		drawTriangle();
		glPopMatrix();

	}
	if (square)
	{
		glPushMatrix();
		glTranslatef(randXcord, randYcord, randZcord);
		drawSquare();
		glPopMatrix();
	}
	if (circle)
	{
		glPushMatrix();
		glTranslatef(randXcord, randYcord, randZcord);
		drawCircle(randXcord, randYcord, 1, 360);
		glPopMatrix();
	}
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xs, int ys)
{
	float fraction = 0.3f, fractionY = 0.05f;
	vec4f left_corner(-1.0f, -1.0f, 0.0f, 1.0f);
	vec4f right_corner(1.0f, 1.0f, 0.0f, 1.0f);

	float projection_buffer[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projection_buffer);

	matrix4fv mat(projection_buffer);

	vec4f temp = mat *left_corner;
	for (int i = 0; i < 4; i++) {
		cout << temp.buffer[i] << " ";
	}
	cout << endl;
	switch (key)
	{
	case 'a':
		angleH -= 0.1f;
		lx = sin(angleH);
		lz = -cos(angleH);
		break;
	case 'd':
		angleH += 0.1f;
		lx = sin(angleH);
		lz = -cos(angleH);
		break;
	case 'w':
		x += lx * fraction;
		y += ly * fraction;
		z += lz * fraction;
		break;
	case 's':
		x -= lx * fraction;
		y -= ly * fraction;
		z -= lz * fraction;
		break;
	case 'c':
		angleV += 0.02f;
		ly = sin(angleV);
		break;
	case 'v':
		angleV -= 0.02f;
		ly = sin(angleV);
		break;
	case 'r':
		y += fractionY;
		break;
	case 'f':
		y -= fractionY;
		break;
	case '1':
		square = false;
		circle = false;
		triangle = true;
		randXcord = fmod(rand(), ((abs(temp.buffer[0]) * 2) - 2.41)) - abs(fmod(rand(), (temp.buffer[0] * 2)));
		randYcord = fmod(rand(), ((abs(temp.buffer[1] * 2) - 2.41)));
		randZcord = z - 10.0f;
		cout << randXcord << " " << randYcord << " " << randZcord << endl;
		break;
	case '2':
		circle = false;
		triangle = false;
		square = true;
		randXcord = fmod(rand(), ((temp.buffer[0] * 2) - temp.buffer[0])) - abs(fmod(rand(), (temp.buffer[0] * 2)));
		randYcord = fmod(rand(), ((temp.buffer[1] * 2) - temp.buffer[1]));
		randZcord = z - 10.0f;
		cout << randXcord << " " << randYcord << " " << randZcord << endl;
		break;
	case '3':
		triangle = false;
		square = false;
		circle = true;
		randXcord = fmod(rand(), ((abs(temp.buffer[0]) * 2) - 2.41)) - abs(fmod(rand(), (temp.buffer[0] * 2)));
		randYcord = fmod(rand(), ((abs(temp.buffer[1] * 2) - 2.41)));
		randZcord = z - 10.0f;
	}

	if (key == 27)
		exit(0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("ACG HW2 Ex1");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 1;
}