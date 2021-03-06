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
float scale = 1.0, translateX = 0.0, translateY = 0.0, translateZ = 0.0;
float cubeCord[1000][3];
double angleH = 0.0f, angleV = 0.0f, rotationAngle = 0.0f;
int max = 200, noCubes = 0;
bool rotation, translation, scaling, r = false;

void drawCube()
{
	glColor3f(0.1f, 1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glRotatef(rotationAngle, 0.0, 1.0, 0.0);
	glutSolidCube(scale);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireCube(scale + 0.005);
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

void renderScene(void)
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

	if (translation == false && rotation == false && scaling == false)
	{
		for (int i = 0; i < noCubes; i++)
		{
			glPushMatrix();
			glTranslatef(cubeCord[i][0] + translateX, cubeCord[i][1], cubeCord[i][2]);
			drawCube();
			glPopMatrix();
		}
	}
	if (translation == true)
	{
		if (r == true)
			translateX = translateX - 0.001;
		if (r == false)
			translateX = translateX + 0.001;

		if (translateX <= 0.0)
			r = false;
		if (translateX >= 0.5)
			r = true;
		for (int i = 0; i < noCubes; i++)
		{
			glPushMatrix();
			glTranslatef(cubeCord[i][0] + translateX, cubeCord[i][1], cubeCord[i][2]);
			drawCube();
			glPopMatrix();
		}
	}
	if (scaling == true)
	{
		if (r == true)
			scale = scale - 0.002f;
		if (r == false)
			scale = scale + 0.002f;

		if (scale <= 0.2f)
			r = false;
		if (scale>= 4.0f)
			r = true;
		for (int i = 0; i < noCubes; i++)
		{
			glPushMatrix();
			glTranslatef(cubeCord[i][0] + translateX, cubeCord[i][1], cubeCord[i][2]);
			drawCube();
			glPopMatrix();
		}
	}
	if (rotation == true)
	{
		rotationAngle = rotationAngle + 0.05f;
		for (int i = 0; i < noCubes; i++)
		{
			glPushMatrix();
			glTranslatef(cubeCord[i][0] + translateX, cubeCord[i][1], cubeCord[i][2]);
			drawCube();
			glPopMatrix();
		}
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
		cubeCord[noCubes][0] = fmod(rand(), ((temp.buffer[0] * 2) - temp.buffer[0])) - abs(fmod(rand(), (temp.buffer[0] * 2)));
		cubeCord[noCubes][1] = fmod(rand(), ((temp.buffer[1] * 2) - temp.buffer[1]));
		cubeCord[noCubes][2] = z - rand()%10 - 3.0f;
		noCubes++;
		break;
	case '2':
		rotation = true;
		scaling = false;
		translation = false;
		break;
	case '3':
		scaling = true;
		rotation = false;
		translation = false;
		break;
	case '4':
		translation = true;
		rotation = false;
		scaling = false;
		break;
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
	glutCreateWindow("ACG HW2 Ex2");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 1;
}
