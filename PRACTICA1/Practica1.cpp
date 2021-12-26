/***************************************************
ISGI:: Pract5 Reloj
Autor: Domenech Mora Ruben
***************************************************/

constexpr auto TITULO = "Pract5 Reloj";
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <freeglut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <math.h>
#include <ctime>
#include <cmath>
#include <chrono>

static float aSeg, aMin, aHora, rotacion, mov, angulo, vel = 450.0 * 3.1415926 / 180;

static GLint estrella, cuadrado, circulo, linea, triangulo;


void estrella_() {
	estrella = glGenLists(1);
	glNewList(estrella, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		float a = (1.0f + (i * 4) % 12) * M_PI / 6;
		glVertex3f(1.0f * cosf(a), 1.0f * sinf(a), 0.0f);
		glVertex3f(0.7f * cosf(a), 0.7f * sinf(a), 0.0f);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 4; i++) {
		float a = (3.0f + float((i * 4) % 12)) * M_PI / 6;
		glVertex3f(1.0f * cosf(a), 1.0f * sinf(a), 0.0);
		glVertex3f(0.7f * cosf(a), 0.7f * sinf(a), 0.0);
	}
	glEnd();
	glEndList();
}

void linea_() {
	linea = glGenLists(1);
	glNewList(linea, GL_COMPILE);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glEndList();
}

void cuadrado_() {
	cuadrado = glGenLists(1);
	glNewList(cuadrado, GL_COMPILE);
	glBegin(GL_QUADS);
	glVertex3f(-1, 0, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 0, 0);
	glEnd();
	glEndList();
}
void triangulo_() {
	triangulo = glGenLists(1);
	glNewList(triangulo, GL_COMPILE);
	glBegin(GL_QUADS);
	glVertex3d(-1, 0, -1);
	glVertex3d(-1, 0, 1);
	glVertex3d(1, 0, 1);
	glVertex3d(1, 0, -1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3d(0, 1, 0);
	glVertex3d(-1, 0, -1);
	glVertex3d(1, 0, -1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3d(0, 1, 0);
	glVertex3d(1, 0, -1);
	glVertex3d(1, 0, 1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3d(0, 1, 0);
	glVertex3d(-1, 0, 1);
	glVertex3d(-1, 0, -1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3d(0, 1, 0);
	glVertex3d(1, 0, 1);
	glVertex3d(-1, 0, 1);
	glEnd();
	glEndList();
}

void circulo_(float r, int n) {
	circulo = glGenLists(1);
	glNewList(circulo, GL_COMPILE);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++) {
		float aux = 2.0f * 3.1415926f * ((float)i) / ((float)n);
		float x = r * cosf(aux);
		float z = r * sinf(aux);
		glColor3f(0.7f, 0.0f, 0.0f);
		glVertex3f(x, 0, z);

	}
	for (int i = 0; i < n; i++) {
		float aux = 2.0f * 3.1415926f * ((float)i) / ((float)n);
		float x = r * cosf(aux);
		float y = r * sinf(aux);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0);
	}
	glEnd();
	glEndList();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glLineWidth(3);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	estrella_();

	triangulo_();

	linea_();

	cuadrado_();

	circulo_(1, 100);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(1, 1.3, 5, 0, 0, 0, 0, 1, 0);

	glPushMatrix();
	float aux = abs(sinf(mov)) / 4;
	float a = (90.0f + (angulo)) * M_PI / 180;
	glTranslatef(sinf(a), 0.0f, cosf(a));
	for (int i = 1; i <= 6; i++) {
		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(0.6f, 0.0f, 0.9f);
		glPushMatrix();
		glScalef(aux, aux, aux);
		glRotatef(30.0f * i + rotacion, 1, 1, 1);
		glCallList(estrella);
		glPopMatrix();
		glPopAttrib();
	}
	glPopMatrix();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 0);
	glScalef(1.0f, 1.0f, 1.0f);
	glRotatef(aSeg, 0, 0, 1);
	glCallList(linea);
	glPopAttrib();
	glPopMatrix();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 0, 0);
	glScalef(0.8f, 0.8f, 0.8f);
	glRotatef(aMin, 0, 0, 1);
	glCallList(linea);
	glPopAttrib();
	glPopMatrix();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 0, 0);
	glScalef(0.4f, 0.4f, 0.4f);
	glRotatef(aHora, 0, 0, 1);
	glCallList(linea);

	glPopAttrib();
	glPopMatrix();


	for (int i = 0; i < 60; i++) {
		glPushMatrix();
		float a = i * 6.0f * M_PI / 180;
		glTranslatef(cosf(a), sinf(a), 0.0f);
		glRotatef(90.0f + i * 6, 0.0f, 0.0f, 1.0f);
		glPushAttrib(GL_CURRENT_BIT);
		if (i % 5 == 0)
		{
			glColor3f(0.5f, 0.0f, 0.5f);
			glRotatef(30.0f + rotacion / 5, 0, 1, 0);
			glScalef(0.06f, 0.2f, 0.06f);
			glCallList(triangulo);
		}
		else
		{
			glColor3f(0.0f, 0.0f, 0.9f);
			glScalef(0.03f, 0.06f, 0.03f);
			glCallList(cuadrado);
		}
		glPopMatrix();
		glPopAttrib();
	}

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 0, 0);
	glCallList(circulo);
	glPopAttrib();
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	float dimen = (float)w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double dist, fov;
	dist = sqrt(pow(1, 2) + pow(1.3, 2) + pow(5, 2));
	fov = (asin(1 / dist) * 2.0) * 180 / M_PI;
	gluPerspective(fov, dimen, 1, 10);
}

void onIdle(int valor)
{
	static int antes = 0;
	int ahora, t_trans;
	ahora = glutGet(GLUT_ELAPSED_TIME);
	t_trans = ahora - antes;
	antes = ahora;

	mov += 0.5f * t_trans / 1000.0f;
	rotacion += 3 % 360;

	const std::time_t now = std::time(nullptr);
	const std::tm calendar_time = *std::localtime(std::addressof(now));
	angulo += vel * t_trans / 1000.0f;
	aSeg = 360.0f - (calendar_time.tm_sec * 6);
	aMin = 360.0f - (calendar_time.tm_min * 6);
	aHora = 360.0f - (calendar_time.tm_hour * 30);

	glutTimerFunc(1000 / 60, onIdle, 60);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 400);
	glutCreateWindow(TITULO);
	init();
	std::cout << TITULO << "hecho por: Domenech Mora Rubén" << std::endl;
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(1000 / 60, onIdle, 60);
	glutMainLoop();
}