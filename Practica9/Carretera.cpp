#define PROYECTO "Videojuego de conduccion"
#define _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <cmath> 
#include <freeglut.h>
#include <Utilidades.h>
#include <sstream>
#include <ctime>
#include <irrKlang.h>

#pragma comment(lib, "irrklang.lib")

using namespace irrklang;

//booleanos modos
bool M_Alamb = false, M_Luz = true, M_Niebla = false;
bool M_Dif = false, M_Inter = false, M_Giro = false;
bool M_Arriba = false, M_Larga = false;

ISoundEngine* engine;

double x = 0, z = 0;
static int y = 1;

GLuint texturas[12];

//booleanos mov
bool arriba = false, abajo = false, der = false, izq = false;
bool der2 = false, izq2 = false;

//variables giro y velocidad
float incVel = 0.15;
float incVel_ori = 0.15;
float incGiro = PI / 90;
float incGiro_ori = 0.25;
float giro = 1.10, velocidad = 0;

GLfloat v0[3] = { 0,0,0 }, v1[3] = { 0,0,0 }, v2[3] = { 0,0,0 }, v3[3] = { 0,0,0 };

//carretera
GLint ancho = 5;
GLint distFarola = 12;

GLfloat roadDif[] = { 0.85, 0.85, 0.85, 1.0 };
GLfloat roadEsp[] = { 0.25, 0.25, 0.25, 1.0 };

float alfa = 0;

//anuncios y farolas
int anuncio = 0;
float ant = 4, ant_ = 4;

//Farolas
GLfloat farolas[] = { GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5 };




float A = 8, T = 100;
float func_sin(float x) {
	return A * sin(x * ((2 * PI) / T));
}
float der_sin(float x) {
	return ((2 * PI * A) / T) * cos(x * (2 * PI / T));
}

void updateControl(int seg) {
	izq2 = izq;
	der2 = der;

	glutTimerFunc(1500, updateControl, 1500);
	glutPostRedisplay();
}

//movimiento del coche
void onTimer(int v) {

	static int antes = 0;
	int ahora;
	double tiempo;
	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempo = ((double)ahora - antes) / 1000.0;
	x += (double)velocidad * sin(giro) * tiempo;
	z += (double)velocidad * cos(giro) * tiempo;
	antes = ahora;

	alfa += (360.0 / 30) * tiempo;

	//igual que la practica 6
	if (arriba) {
		if (M_Giro) velocidad += incVel_ori;
		else velocidad += incVel;
	}
	if (abajo && velocidad > 0.0001) {
		if (M_Giro) velocidad -= incVel_ori;
		else velocidad -= incVel;
	}
	if (izq) {
		if (M_Giro) giro += incGiro_ori;
		else giro += incGiro;
	}
	if (der) {
		if (M_Giro) giro -= incGiro_ori;
		else giro -= incGiro;
	}
	if (velocidad > 15) {
		velocidad = 15;
	}
	if (velocidad < 0.05) {
		velocidad = 0;
	}

	arriba = abajo = izq = der = false;

	//titulo y velocidad
	stringstream titulo;
	titulo << "Videojuego de conduccion - Velocidad: m/s: " << velocidad;
	glutSetWindowTitle(titulo.str().c_str());

	glutTimerFunc(1000 / 60, onTimer, 1000 / 60);
	glutPostRedisplay();
}

//Función que controla las teclas pulsadas
void onTecla(unsigned char tecla, int xp, int yp) {
	switch (tecla) {
	case 'W':
	case 'w':
		if (M_Larga) M_Larga = false; else M_Larga = true;
		break;
	case 'A':
	case 'a':
		if (M_Arriba) M_Arriba = false; else M_Arriba = true;
		break;
	case 'S':
	case 's':
		if (M_Alamb) M_Alamb = false; else M_Alamb = true;
		break;
	case 'D':
	case 'd':
		if (!M_Dif) {
			A = 15, T = 70;
			M_Dif = true;
			//Música de fondo
			engine->stopAllSounds();
			engine->play2D("./dragon.mp3", true);
		}
		else {
			A = 10, T = 120;
			M_Dif = false;
			//Música de fondo
			engine->stopAllSounds();
			engine->play2D("./bosque.mp3", true);

		}
		break;
	case 'L':
	case 'l':
		if (M_Luz) M_Luz = false; else M_Luz = true;
		break;
	case 'N':
	case 'n':
		if (M_Niebla) M_Niebla = false; else M_Niebla = true;
		break;
	case 27:
		exit(0);

	case 'H':
	case 'h':
		if (M_Inter) M_Inter = false; else M_Inter = true;
		break;
	case 'G':
	case 'g':
		if (M_Giro) M_Giro = false; else M_Giro = true;
		break;

	case 'C':
	case 'c':
		saveScreenshot((char*)"captura.jpg", 600, 600);
		break;
	}
	glutPostRedisplay();

}

//flechas del teclado
void onFlechas(int tecla, int xp, int yp) {
	switch (tecla) {
	case GLUT_KEY_UP:
		arriba = true;
		izq2 = false;
		der2 = false;
		break;
	case GLUT_KEY_DOWN:
		abajo = true;
		izq2 = false;
		der2 = false;
		break;
	case GLUT_KEY_RIGHT:
		der = true;
		der2 = true;
		break;
	case GLUT_KEY_LEFT:
		izq = true;
		izq2 = true;
		break;
	}
	glutPostRedisplay();
}

//Dibujo del coche
void coche() {
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (der2) {
		glBindTexture(GL_TEXTURE_2D, texturas[9]);
	}
	else if (izq2) {
		glBindTexture(GL_TEXTURE_2D, texturas[10]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, texturas[11]);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5, 0.1, -2);
	glTexCoord2f(1, 0);
	glVertex3f(0.5, 0.1, -2);
	glTexCoord2f(1, 1);
	glVertex3f(0.5, 0.9, -2);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5, 0.9, -2);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
}

void init()
{
	engine = createIrrKlangDevice();

	//musica
	engine->play2D("./dragon.mp3", true);

	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	//luces practica 7
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_LIGHT6);
	glEnable(GL_LIGHT7);

	//luces ambiente
	GLfloat Amb[] = { 0.05,0.05,0.05,1.0 };
	GLfloat Amb2[] = { 0.0,0.0,0.0,1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Amb2);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	//faros
	GLfloat fAmb[] = { 0.2,0.2,0.2,1.0 };
	GLfloat fDif[] = { 1.0,1.0,1.0,1.0 };
	GLfloat fAmb2[] = { 0.3,0.3,0.3,1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, fAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, fDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, fAmb2);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);

	//farolas
	GLfloat farolaAmb[] = { 0.0,0.0,0.0,1.0 };
	GLfloat farolaDif[] = { 0.5,0.5,0.2,1.0 };
	GLfloat pos[] = { 0.0, -1.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, farolaAmb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, farolaDif);
	glLightfv(GL_LIGHT2, GL_SPECULAR, farolaAmb);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, pos);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT3, GL_AMBIENT, farolaAmb);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, farolaDif);
	glLightfv(GL_LIGHT3, GL_SPECULAR, farolaAmb);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, pos);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT4, GL_AMBIENT, farolaAmb);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, farolaDif);
	glLightfv(GL_LIGHT4, GL_SPECULAR, farolaAmb);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, pos);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT5, GL_AMBIENT, farolaAmb);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, farolaDif);
	glLightfv(GL_LIGHT5, GL_SPECULAR, farolaAmb);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, pos);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0);

	//anuncios
	GLfloat anuncioAmb[] = { 0.5,0.5,0.5,1.0 };
	GLfloat anuncioDif[] = { 1,1,1,1.0 };

	glLightfv(GL_LIGHT6, GL_AMBIENT, anuncioAmb);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, anuncioDif);
	glLightfv(GL_LIGHT6, GL_SPECULAR, anuncioAmb);
	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, pos);
	glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT7, GL_AMBIENT, anuncioAmb);
	glLightfv(GL_LIGHT7, GL_DIFFUSE, anuncioDif);
	glLightfv(GL_LIGHT7, GL_SPECULAR, anuncioAmb);
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, pos);
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 10.0);

	glEnable(GL_TEXTURE_2D);

	//carretera
	glGenTextures(1, &texturas[1]);
	glBindTexture(GL_TEXTURE_2D, texturas[1]);
	loadImageFile((char*)"carretera.png");

	//cesped
	glGenTextures(1, &texturas[2]);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);
	loadImageFile((char*)"cesped.jpeg");

	//cielo 
	glGenTextures(1, &texturas[3]);
	glBindTexture(GL_TEXTURE_2D, texturas[3]);
	loadImageFile((char*)"cielo.jpg");

	//cielo estrellado
	glGenTextures(1, &texturas[4]);
	glBindTexture(GL_TEXTURE_2D, texturas[4]);
	loadImageFile((char*)"noche.jpg");

	//anuncios
	glGenTextures(1, &texturas[5]);
	glBindTexture(GL_TEXTURE_2D, texturas[5]);
	loadImageFile((char*)"anuncio1.jpg");

	glGenTextures(1, &texturas[6]);
	glBindTexture(GL_TEXTURE_2D, texturas[6]);
	loadImageFile((char*)"anuncio2.png");

	glGenTextures(1, &texturas[7]);
	glBindTexture(GL_TEXTURE_2D, texturas[7]);
	loadImageFile((char*)"anuncio3.jpg");

	glGenTextures(1, &texturas[8]);
	glBindTexture(GL_TEXTURE_2D, texturas[8]);
	loadImageFile((char*)"anuncio4.png");

	//coche
	glGenTextures(1, &texturas[9]);
	glBindTexture(GL_TEXTURE_2D, texturas[9]);
	loadImageFile((char*)"der.png");

	glGenTextures(1, &texturas[10]);
	glBindTexture(GL_TEXTURE_2D, texturas[10]);
	loadImageFile((char*)"izq.png");

	glGenTextures(1, &texturas[11]);
	glBindTexture(GL_TEXTURE_2D, texturas[11]);
	loadImageFile((char*)"normal.png");

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (M_Alamb) {
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
	}
	else {

		if (M_Luz) {
			glDisable(GL_LIGHTING);
			glClearColor(1.0, 1.0, 1.0, 1.0);
		}
		else
		{
			glShadeModel(GL_SMOOTH);
			glEnable(GL_LIGHTING);
			glClearColor(0.0, 0.0, 0.0, 0.0);
		}
		glEnable(GL_TEXTURE_2D);
	}
	if (M_Niebla) {
		glEnable(GL_FOG);
		glFogfv(GL_FOG_COLOR, BLANCO);
		glFogf(GL_FOG_DENSITY, 0.08);
	}
	else {
		glDisable(GL_FOG);
	}


	glMatrixMode(GL_MODELVIEW);

	//dibujo de farolas
	GLfloat ini = ant;
	GLfloat bucle = 0;
	for (int i = 0; i < 4; i++) {
		ini += distFarola;
		if (i == 0) bucle = ini;
		float xfarola = ini;
		float zfarola = func_sin(xfarola);
		GLfloat posF[] = { xfarola,4,zfarola,1.0 };

		glLightfv(farolas[i], GL_POSITION, posF);
	}

	if (x > bucle) {
		ant = bucle;
	}
	//dibujo de los anuncios
	GLfloat bucle_ = 0;
	GLfloat ini_ = ant_;
	for (int i = 0; i < 4; i++) {
		ini_ += 75;
		if (i == 0) bucle_ = ini_;
		float xfarola = ini_;
		float zfarola = func_sin(xfarola);
		GLfloat posicion1[] = { xfarola,12,zfarola + 2,1.0 };
		GLfloat posicion2[] = { xfarola,12,zfarola - 2,1.0 };
		glPushMatrix();
		switch (i) {
		case 0: glLightfv(GL_LIGHT6, GL_POSITION, posicion1);
			glLightfv(GL_LIGHT7, GL_POSITION, posicion2);
			glBindTexture(GL_TEXTURE_2D, texturas[(0 + anuncio) % 4 + 5]);
			break;
		case 1: glBindTexture(GL_TEXTURE_2D, texturas[(1 + anuncio) % 4 + 5]); break;
		case 2: glBindTexture(GL_TEXTURE_2D, texturas[(2 + anuncio) % 4 + 5]); break;
		case 3: glBindTexture(GL_TEXTURE_2D, texturas[(3 + anuncio) % 4 + 5]); break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (M_Alamb) {
			glPolygonMode(GL_BACK, GL_LINE);
			glPolygonMode(GL_FRONT, GL_LINE);
		}
		else {
			glPolygonMode(GL_BACK, GL_FILL);
			glPolygonMode(GL_FRONT, GL_FILL);
		}
		glTranslatef(xfarola, 1, zfarola);
		glColor3f(1, 0, 1);
		GLfloat g0[3] = { 1 , 1,-4 }, g1[3] = { 1, 1, +4 }, g2[3] = { 1, 6, +4 }, g3[3] = { 1, 6, -4 };
		quadtex(g0, g1, g2, g3, 0, 1, 0, 1, 50, 50);
		glPopMatrix();
		glPushMatrix();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glTranslatef(xfarola + 1, 7, zfarola - 4);
		glColor3f(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		if (M_Alamb) glutWireCylinder(0.1, 10, 30, 30);
		else  glutSolidCylinder(0.1, 10, 30, 30);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glPopMatrix();
		glPushMatrix();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glTranslatef(xfarola + 1, 7, zfarola + 4);
		glColor3f(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		if (M_Alamb) glutWireCylinder(0.1, 10, 30, 30);
		else
		{
			glutSolidCylinder(0.1, 10, 30, 30);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_LIGHTING);
		}
		glPopMatrix();
	}
	if (x > bucle_) {
		anuncio = (anuncio + 1) % 4;
		ant_ = bucle_;
	}

	//dibujo carretera  
	float inicio = x - 10, vfseno = func_sin(inicio);
	float der = der_sin(inicio);
	GLfloat pre[3] = { inicio,0,vfseno };
	GLfloat tz[3] = { -der,0,1 };
	GLfloat normales[3] = { (1 / sqrt(1 + der * der)) * tz[0] , 0 ,(1 / sqrt(1 + der * der)) * tz[2] };
	for (int i = 0; i < 3; i++) {
		v0[i] = pre[i] - (normales[i] * ancho);
		v3[i] = pre[i] + (normales[i] * ancho);
	}
	for (int i = 1; i < 100; i++) {
		float aux = inicio + i;
		vfseno = func_sin(aux);
		float der = der_sin(aux);
		GLfloat pre2[3] = { aux,0,vfseno };
		GLfloat tz[3] = { -der,0,1 };
		GLfloat normales2[3] = { (1 / sqrt(1 + der * der)) * tz[0] , 0 ,(1 / sqrt(1 + der * der)) * tz[2] };
		for (int i = 0; i < 3; i++) {
			v1[i] = pre2[i] - (normales2[i] * ancho);
			v2[i] = pre2[i] + (normales2[i] * ancho);
		}
		glPushMatrix();

		if (M_Alamb) {
			glPolygonMode(GL_BACK, GL_LINE);
			glPolygonMode(GL_FRONT, GL_LINE);
		}
		else {
			glPolygonMode(GL_BACK, GL_FILL);
			glPolygonMode(GL_FRONT, GL_FILL);
		}
		//textura
		glBindTexture(GL_TEXTURE_2D, texturas[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glColor3f(0, 1, 0);

		//material
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, roadDif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, roadEsp);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 5);

		quadtex(v2, v1, v0, v3, 0, 1, 0, 1, 50, 50);
		glPopMatrix();
		for (int i = 0; i < 3; i++) {
			v0[i] = v1[i];
			v3[i] = v2[i];
		}
	
	//fin dibujo carretera

	
	}

	//cesped
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BLANCO);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, BLANCO);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 5);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glColor3f(1, 1, 0);

	GLfloat p0[3] = { 100 + x, -0.1, -100 - z }, p1[3] = { 100 + x, -0.1, 100 - z }, p2[3] = { -100 - x, -0.1, 100 + z }, p3[3] = { -100 - x, -0.1, -100 - z };
	quadtex(p0, p1, p2, p3, 0, 10, 0, 10, 10 * 10, 5 * 10);
	glPopMatrix();


	glLoadIdentity();

	if (!M_Arriba) {
		glEnable(GL_LIGHT1);
		glPushMatrix();
		glTranslatef(0, -1, 0);
		coche();
		glPopMatrix();
		

		if (!M_Larga) {
			GLfloat faroAmb[] = { 0.2,0.2,0.2,1.0 };
			GLfloat faroDif[] = { 1.0,1.0,1.0,1.0 };
			GLfloat faroAmb2[] = { 0.3,0.3,0.3,1.0 };

			glLightfv(GL_LIGHT1, GL_AMBIENT, faroAmb);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, faroDif);
			glLightfv(GL_LIGHT1, GL_SPECULAR, faroAmb2);
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
			glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);
			GLfloat pos[] = { 0.0, 0.7, 0.0, 1.0 };
			GLfloat dir[] = { 0.0, -0.5, -0.7 };
			glLightfv(GL_LIGHT1, GL_POSITION, pos);
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
		}
		else {
			GLfloat faroAmb[] = { 0.5,0.5,0.5,1.0 };
			GLfloat faroDif[] = { 1.0,1.0,1.0,1.0 };
			GLfloat faroAmb2[] = { 0.3,0.3,0.3,1.0 };

			glLightfv(GL_LIGHT1, GL_AMBIENT, faroAmb);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, faroDif);
			glLightfv(GL_LIGHT1, GL_SPECULAR, faroAmb2);
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
			glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 5.0);
			GLfloat pos[] = { 0.0, 0.7, 0.0, 1.0 };
			GLfloat dir[] = { 0.0, -0.4, -0.7 };
			glLightfv(GL_LIGHT1, GL_POSITION, pos);
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
		}
		if (M_Alamb) {
			glDisable(GL_LIGHTING); 
			glDisable(GL_TEXTURE_2D); 
		}
	}
	else {

		glDisable(GL_LIGHT1);
	}

	if (M_Inter) {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glPushMatrix();
		
		glTranslatef(0.55, -0.55, -2);
		glScalef(0.2, 0.2 * velocidad, 0);
		if (velocidad <= 5) glColor3f(0, 1, 0);
		else if (velocidad <= 10) glColor3f(1, 1, 0);
		else glColor3f(1, 0, 0);

		if (M_Alamb) {
			glutWireCube(1);
		}
		else {

			glutSolidCube(1);

		}
		glPopMatrix();

		glPushMatrix();

		glTranslatef(-0.75, -0.75, -2);
		glColor3f(1, 0, 1);
		glRotatef(((giro) * 180 / PI) - 90, 0, 0, 1);
		if (M_Alamb) {
			glPushMatrix();
			glTranslatef(0.0, 0.15, 0.0);
			glRotatef(-90, 1, 0, 0);
			glScalef(2, 2, 2);
			glutWireCone(0.03, 0.05, 100, 100);
			glPopMatrix();
			glPushMatrix();
			glRotatef(-90, 1, 0, 0);
			glutWireCylinder(0.01, 0.15, 100, 100);
			glPopMatrix();
		}
		else {
			glPushMatrix();
			glTranslatef(0.0, 0.15, 0.0);
			glRotatef(-90, 1, 0, 0);
			glScalef(2, 2, 2);
			glutSolidCone(0.03, 0.05, 100, 100);
			glPopMatrix();
			glPushMatrix();
			glRotatef(-90, 1, 0, 0);
			glutSolidCylinder(0.01, 0.15, 100, 100);
			glPopMatrix();
		}
		glPopMatrix();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}

	//vector practica 6
	if (!M_Arriba) {
		gluLookAt(x, y, z, 10 * sin(giro) + x, 1, 10 * cos(giro) + z, 0, 1, 0); 
	}
	else {
		gluLookAt(x, 100, z, 10 * sin(giro) + x, 1, 10 * cos(giro) + z, 0, 1, 0); 
	}

	if (M_Luz) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//luz nocturna
	GLfloat luz[] = { 0.0, 10.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, luz);

	//cielo, despejado o estrellado
	glPushMatrix();
	if (M_Luz) {
		glBindTexture(GL_TEXTURE_2D, texturas[3]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, texturas[4]);
	}
	float alpha = 2 * PI / 50;
	GLfloat cil0[3] = { 200 * cos(0) + x,100,200 * -sin(0) + z };
	GLfloat cil1[3] = { 200 * cos(0) + x,-55,200 * -sin(0) + z };
	GLfloat cil2[3];
	GLfloat cil3[3];
	for (int i = 1; i <= 50; i++) {
		cil2[0] = 200 * cos(i * alpha) + x;
		cil2[1] = 100;
		cil2[2] = 200 * -sin(i * alpha) + z;
		cil3[0] = 200 * cos(i * alpha) + x;
		cil3[1] = -55;
		cil3[2] = 200 * -sin(i * alpha) + z;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor3f(0, 0, 1);
		quadtex(cil3, cil1, cil0, cil2, (i) / 50.0 + 0.5, (i - 1.0) / 50.0 + 0.5, 0, 1);
		for (int j = 0; j < 3; j++) {
			cil0[j] = cil2[j];
			cil1[j] = cil3[j];
		}
	}
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);

	float ra = float(w) / float(h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, ra, 1, 250);
}

void main(int argc, char** argv)
{
	FreeImage_Initialise();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);

	glutCreateWindow(PROYECTO);

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(onTecla);

	glutSpecialFunc(onFlechas);
	glutTimerFunc(1000 / 60, onTimer, 1000 / 60);
	glutTimerFunc(1500, updateControl, 1500);

	system("CLS");

	cout <<
		"Flecha izq/der: giro del coche " << endl <<
		"Flecha arriba/abajo: cambio de la velocidad del coche" << endl <<
		"W/w: cambia las luces cortas a largas y viceversa " << endl <<
		"A/a: proporciona una vista desde Arriba" << endl <<
		"S/s: activa/desactiva un modelo simple en alambrico" << endl <<
		"D/d: cambia el algulo de las curvas " << endl <<
		"L/l: cambia entre modo dia y noche " << endl <<
		"N/n: cambia el estado de la niebla (on/off) " << endl <<
		"H/h: añade/quita elementos del -HUD- (on/off) " << endl <<
		"G/g: cambia la velocidad de giro (on/off) " << endl <<
     	"C/c: guarda una captura de pantalla del videojuego " << endl;

	glutMainLoop();

	FreeImage_DeInitialise();
}