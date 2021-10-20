/*
Transformaciones
Uso de listas y transformaciones en OpenGL

Dibujar una flor como composicion de varias partes

*/


#include <iostream>
#include <gl\freeglut.h>n


void init()
{


	petalo = glGenLists(1);
	glNewList(petalo, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);


	glColor3f(1, 1, 1);
	glPushMatrix();
	glScalef(0.15, 0.5, 0.1);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPopAttrib();
	glEndList();



	//corola

	corola = glGenLists(1);
	glNewList(corola, GL_COMPILE);

	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 1, 0);

	glPushMatrix();
	glScalef(0.2, 0.2, 0.1);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPopAttrib();
	glEndList();

	//flor

	flor = glGenLists(1);
	glNewList(flor, GL_COMPILE);


	//tallo

	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslatef(0, -0.125, 0);
	glScalef(0.1, 0.75, 0.1);

	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0, 0.25, 0);
	glScalef(0.5, 0.5, 1);
	glCallList(corola);

	glPopMatrix();

	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);




}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Inicializar a modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glCallList(petalo);

	glFlush();
}
void reshape(GLint w, GLint h)
{
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 400);
	glutCreateWindow("PROYECTO");
	std::cout << "Hola Mundo running" << std::endl;
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
}