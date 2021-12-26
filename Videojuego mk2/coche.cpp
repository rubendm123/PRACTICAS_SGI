#include <iostream> // Biblioteca de entrada salida
#include <cmath> // Biblioteca matemática de C
#include <freeglut.h> // Biblioteca grafica
#include <Utilidades.h>
#include <sstream>
#include <ctime> 

#define PROYECTO "Tatooine"
#define PI 3.1415926
#define tasaFPS 60

static float ticks = 0;
static const float tickRate = 1;

static enum { TEXTURAS, MAYA } Dibujado;
static enum { ON, OFF } LUCES;
static enum { DESPEJADO, NIEBLA,TARENA } Clima;
static enum { SPEED_OFF, SPEED_ON } UI;
static float displacement = 0.0;
static float speed = 0, despX = 0.296, despZ = 0.95;
static float PosX = 0, PosY = 1, PosZ = 0;
static float LastZ = 0;
static float CieloLZ = 0;
static bool DESIERTO1 = FALSE;

		

static float angulo = 7.5;

static GLuint motospeeder, TextCarretera, sky_texture, terreno, Horizonte_texture,ad1,ad2,ad3,ad4,desierto;

time_t h, m, s;

float static A = 10;//A=1,5
float static T = 200;//T=20,400 funcionan mas o menos bien
double static below = -0.001; // acercar un below mas ha 0 causa colisiones extrañas entre la carretera y el suelo
static float coef[16];

float trazado(float x, float amplitud, float periodo)
{
	return amplitud * sin(x * ((2 * PI) / periodo));
}

void lighting() {
	//Luz de la luna
	GLfloat Al0[] = { 0.1,0.1,0.1 };
	GLfloat Dl0[] = { 0.1,0.1,0.1 };
	GLfloat Sl0[] = { 0.0,0.0,0.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Al0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0);


	//Luz de la moto

	GLfloat Al1[] = { 1,1,1,1.0 };
	GLfloat Dl1[] = { 1.0,1.0,1.0,1.0 };
	GLfloat Sl1[] = { 0.3,0.3,0.3,1.0 };


	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, Al1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Dl1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Sl1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);
}


void loadtextures() {

	glGenTextures(1, &motospeeder);
	glBindTexture(GL_TEXTURE_2D, motospeeder);
	loadImageFile((char*)"motospeeder3.png");
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &TextCarretera);
	glBindTexture(GL_TEXTURE_2D, TextCarretera);
	loadImageFile((char*)"carretera.jpg");

	glGenTextures(1, &sky_texture);
	glBindTexture(GL_TEXTURE_2D, sky_texture);
	loadImageFile((char*)"sky.jpg");

	glGenTextures(1, &Horizonte_texture);
	glBindTexture(GL_TEXTURE_2D,Horizonte_texture);
	loadImageFile((char*)"Horizonte.png");

	glGenTextures(1, &desierto);
	glBindTexture(GL_TEXTURE_2D, desierto);
	loadImageFile((char*)"Desierto.jpg");

	glGenTextures(1, &terreno);
	glBindTexture(GL_TEXTURE_2D, terreno);
	loadImageFile((char*)"Arena2.jpg");

	glGenTextures(1, &ad1);
	glBindTexture(GL_TEXTURE_2D, ad1);
	loadImageFile((char*)"ad1.jpg");

	glGenTextures(1, &ad2);
	glBindTexture(GL_TEXTURE_2D, ad2);
	loadImageFile((char*)"ad2.jpg");

	glGenTextures(1, &ad3);
	glBindTexture(GL_TEXTURE_2D, ad3);
	loadImageFile((char*)"ad3.jpg");

	glGenTextures(1, &ad4);
	glBindTexture(GL_TEXTURE_2D, ad4);
	loadImageFile((char*)"ad4.jpg");

}

void init()
// Funcion de inicializacion propia
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);

	loadtextures();
	lighting();

	cout << "1. Flecha izquierda/derecha: giro del vehículo\n";
	cout << "2. Flecha arriba / abajo: aumento / disminución de la velocidad\n";
	cout << "3. s : Activa / desactiva Modelo Inalambrico \n";
	cout << "4. l : Cambia entre modo dia / noche\n";
	cout << "5. n : Cambia el estado de la niebla (on / off)\n";
	cout << "6. n : Cambia el estado de la tormenta de arena (on / off)\n";
	cout << "7. c : Cambia la visibilidad de elementos solidarios a la cámara(on / off):\n";
	cout << "8. d : Cambia entre 2 horizontes distintos:\n";

}

void PostedeLuz(float x, float y, float z,GLuint light) {
	glPushMatrix();
	GLfloat posicionL2[] = { x,y,z,1 };//farola1
	glLightfv(light, GL_POSITION, posicionL2);
	glPopMatrix();
}

void backgroundObjectParams(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	if (LUCES == ON) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		GLfloat Dm[] = { 0.75,0.75,0.75,1.0 };
		GLfloat Sm[] = { 0.53,0.53,0.53,1.0 };
		GLfloat s = 1.0;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Dm);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Sm);
		glMaterialf(GL_FRONT, GL_SHININESS, s);
	}
	else { 
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		GLfloat Dm[] = { 0.8,0.8,0.8,1.0 };
		GLfloat Sm[] = { 0.0,0.0,0.0,1.0 };
		GLfloat s = 0.0;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Dm);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Sm);
		glMaterialf(GL_FRONT, GL_SHININESS, s);
	}
}

void GeneracionCircuito(int tamaño) {
	//Carretera y Anuncios

	for (auto i = -(PosZ + 1); i < PosZ + tamaño; i++)
	{

		//GLfloat v0[3] = { trazado(i, A, T) - 2,0,i }, v3[3] = { trazado(i, A, T) + 2,0,i }, v2[3] = { trazado(i + 1, A, T) + 2,0, i + 1 }, v1[3] = { trazado(i + 1, A, T) - 2,0, i + 1 };
		GLfloat v1[3] = { trazado(i, A, T) - 2,0,i }, 
			v2[3] = { trazado(i, A, T) + 2,0,i }, 
			v3[3] = { trazado(i + 1, A, T) + 2,0, i + 1 }, 
			v0[3] = { trazado(i + 1, A, T) - 2,0, i + 1 };
		glPushMatrix();
		backgroundObjectParams(TextCarretera);
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1, 0.5, 0.5);
		quad(v3, v0, v1, v2, 30, 30);
		glPopMatrix();
	}

}

void ads() {
	int aux = (int)PosZ;
	int multiplo100 = aux / 100;
	int i1 = 100 * multiplo100 + 25;
	int i2 = 100 * multiplo100 + 50;
	int i3 = 100 * multiplo100 + 75;
	int i4 = 100 * multiplo100 + 100;

	//Pos ad1

		glPushMatrix();
		GLfloat v1[3] = { trazado(i1, A, T) - 3,2,i1 },
			v2[3] = { trazado(i1, A, T) + 3,2,i1 },
			v3[3] = { trazado(i1, A, T) + 3,5, i1 },
			v0[3] = { trazado(i1, A, T) - 3,5, i1 };

		backgroundObjectParams(ad1);
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1, 0.5, 0.5);
		quad(v3, v0, v1, v2, 30, 30);

		//cout << "HOLA1\n";

		//Pos ad2
		GLfloat v5[3] = { trazado(i2, A, T) - 3,2,i2 },
			v6[3] = { trazado(i2, A, T) + 3,2,i2 },
			v7[3] = { trazado(i2, A, T) + 3,5, i2 },
			v4[3] = { trazado(i2, A, T) - 3,5, i2 };

		backgroundObjectParams(ad2);
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1, 0.5, 0.5);
		quad(v7, v4, v5, v6, 30, 30);

		//cout << "HOLA2\n";


		//Pos ad3
		GLfloat v9[3] = { trazado(i3, A, T) - 3,2,i3 },
			v10[3] = { trazado(i3, A, T) + 3,2,i3 },
			v11[3] = { trazado(i3, A, T) + 3,5, i3 },
			v8[3] = { trazado(i3, A, T) - 3,5, i3 };

		backgroundObjectParams(ad3);
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1, 0.5, 0.5);
		quad(v11, v8, v9, v10, 30, 30);


		//cout << "HOLA3\n";
		//ad4
		GLfloat v13[3] = { trazado(i4, A, T) - 3,2,i4 },
			v14[3] = { trazado(i4, A, T) + 3,2,i4 },
			v15[3] = { trazado(i4, A, T) + 3,5, i4 },
			v12[3] = { trazado(i4, A, T) - 3,5, i4 };

		backgroundObjectParams(ad4);
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1, 0.5, 0.5);
		quad(v15, v12, v13, v14, 30, 30);
		glPopMatrix();
	
}

void Cielo(int Area2) {

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, sky_texture);
	backgroundObjectParams(sky_texture);
	glBegin(GL_QUADS);
	int altura = 10; //Altura del ciero
	float aux = (PosZ / Area2); //Saco la parte decimal de la cantidad de cielo recorrido de la parte generada
	//cout <<"aux = "<< aux << " PosZ = "<< PosZ << "Area2= "<< Area2 << "\n";
	int aux1 = (int)aux;
	aux = aux - aux1;
	
	//if (aux > 0.6 || PosZ == 0) {//generar nuevo segmento
		//cielo
	glTexCoord2f(0, 0);
	glVertex3f(-Area2, altura, (PosZ + Area2));//1
	glTexCoord2f(1, 0);
	glVertex3f(Area2, altura, (PosZ + Area2)); //2
	glTexCoord2f(1, 1);
	glVertex3f(Area2, altura, PosZ-20);  //3
	glTexCoord2f(0, 1);
	glVertex3f(-Area2, altura, PosZ-20); //4
	CieloLZ = PosZ+1;
	glEnd();
	glPopMatrix();
		//Horizonte, delante
	glPushMatrix();
	if (DESIERTO1) {
		glBindTexture(GL_TEXTURE_2D, Horizonte_texture);
		backgroundObjectParams(Horizonte_texture);
	}
	else {

		glBindTexture(GL_TEXTURE_2D, desierto);
		backgroundObjectParams(desierto);
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-Area2, 0, (PosZ +70));//1
	glTexCoord2f(1, 0);
	glVertex3f(Area2, 0, (PosZ +70)); //2
	glTexCoord2f(1, 1);
	glVertex3f(Area2, altura, (PosZ + 70));  //3
	glTexCoord2f(0, 1);
	glVertex3f(-Area2, altura, (PosZ + 70)); //4
	glEnd();
	glPopMatrix();
		//LADO DER
	glPushMatrix();
	if (DESIERTO1) {
		glBindTexture(GL_TEXTURE_2D, Horizonte_texture);
		backgroundObjectParams(Horizonte_texture);
	}
	else {

		glBindTexture(GL_TEXTURE_2D, desierto);
		backgroundObjectParams(desierto);
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-Area2/2, 0, PosZ-20);//1
	glTexCoord2f(1, 0);
	glVertex3f(-Area2/2, 0, (PosZ + 70)); //2
	glTexCoord2f(1, 1);
	glVertex3f(-Area2/2, altura, (PosZ + 70));  //3
	glTexCoord2f(0, 1);
	glVertex3f(-Area2/2, altura, PosZ-20); //4
	glEnd();
	glPopMatrix();
		//LADO IZQ
	glPushMatrix();

	if (DESIERTO1) {
		glBindTexture(GL_TEXTURE_2D, Horizonte_texture);
		backgroundObjectParams(Horizonte_texture);
	}
	else {

		glBindTexture(GL_TEXTURE_2D, desierto);
		backgroundObjectParams(desierto);
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(Area2 / 2, 0, PosZ + 70 );//1
	glTexCoord2f(1, 0);
	glVertex3f(Area2 / 2, 0, (PosZ - 20)); //2
	glTexCoord2f(1, 1);
	glVertex3f(Area2 / 2, altura, (PosZ - 20));  //3
	glTexCoord2f(0, 1);
	glVertex3f(Area2 / 2, altura, PosZ + 70); //4
	glEnd();
	glPopMatrix();
	//}
	/* Codigo para mantener un segmento de cielo/Horizonte statico
	  else {//statico,sin generar nuevo
		glTexCoord2f(0, 0);
		glVertex3f(-Area2, altura, (CieloLZ + Area2));//1
		glTexCoord2f(1, 0);
		glVertex3f(Area2, altura, (CieloLZ + Area2)); //2
		glTexCoord2f(1, 1);
		glVertex3f(Area2, altura, CieloLZ);  //3
		glTexCoord2f(0, 1);
		glVertex3f(-Area2, altura, CieloLZ); //4
		glEnd();
		glPopMatrix();
		//Horizonte, delante
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, sky_texture);
		backgroundObjectParams(sky_texture);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-Area2, 0, (CieloLZ + 70));//1
		glTexCoord2f(1, 0);
		glVertex3f(Area2, 0, (CieloLZ + 70)); //2
		glTexCoord2f(1, 1);
		glVertex3f(Area2, altura, (CieloLZ + 70));  //3
		glTexCoord2f(0, 1);
		glVertex3f(-Area2, altura, (CieloLZ + 70)); //4
		glEnd();
		glPopMatrix();
	}*/


}

void floor(int Area2,int numero) {
	for (int i = 0; i <= numero; i++) {
		int incerteza = 10;
		glPushMatrix();
		backgroundObjectParams(terreno);
		glBegin(GL_QUADS);
		float aux = (PosZ / Area2); //Saco la parte decimal de la cantidad de carretera recorrida de la parte generada
		int aux1 = (int)aux;
		aux = aux - aux1;
		if ( aux > 0.99|| PosZ == 0) {
			glTexCoord2f(0, 0);
			glVertex3f(-Area2, below, (PosZ - Area2) + (Area2*i));//1
			glTexCoord2f(1, 0);
			glVertex3f(Area2, below, (PosZ - Area2) + (Area2*i)); //2
			glTexCoord2f(1, 1);
			glVertex3f(Area2, below, (PosZ)+((Area2)*i));  //3
			glTexCoord2f(0, 1);
			glVertex3f(-Area2, below, (PosZ)+((Area2)*i)); //4
			LastZ = PosZ;


		}else{
			glTexCoord2f(0, 0);
			glVertex3f(-Area2, below, (LastZ - Area2) + (Area2*i));//1
			glTexCoord2f(1, 0);
			glVertex3f(Area2, below, (LastZ - Area2) + (Area2*i)); //2
			glTexCoord2f(1, 1);
			glVertex3f(Area2, below, (LastZ)+((Area2)*i));  //3
			glTexCoord2f(0, 1);
			glVertex3f(-Area2, below, (LastZ)+((Area2)*i)); //4
		}


		glEnd();
		glPopMatrix();
	}
}




void moto() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	backgroundObjectParams(motospeeder);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);//1
	glVertex3f(-0.2, 0.5, -2);
	glTexCoord2f(1, 1);//2
	glVertex3f(0.2, 0.5, -2);
	glTexCoord2f(1, 0);//3
	glVertex3f(0.2, 0.1, -2);
	glTexCoord2f(0, 0);//4
	glVertex3f(-0.2, 0.1, -2);



	glEnd();
	glPopMatrix();

}

void speedBar() {

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_POLYGON);

	if (speed >= 3.0) {
		glColor3f(0.0, 0.9, 0.0);
	}
	if (speed >= 5.5) {
		glColor3f(0.9, 0.9, 0.0);
	}
	if (speed >= 7) {
		glColor3f(1.0, 0, 0.0);
	}

	glTranslatef(-0.5, -5, 0);
	glVertex3f(1.4, 0.8, -2.0);
	glVertex3f(1.4, 0.75, -2.0);
	
	if (speed >= 3.0) {
		glColor3f(1.0, 0.5, 0.0);
	}
	if (speed >= 5.5) {
		glColor3f(1.0, 0.0, 0.0);
	}
	if (speed >= 7) {
		glColor3f(0.6, 0, 0);
	}
	glVertex3f(1.4 - (1.4 - 0.5)*speed / (15 + 0.0001), 0.75, -2.0);
	if (speed >= 10) {
		glColor3f(0, 0, 0.9);
	}
	glVertex3f(1.4 - (1.4 - 0.5)*speed / (15 + 0.0001), 0.8, -2.0);
	glEnd();
	
	glColor3f(1, 1, 1);
	glPopMatrix();
}



void populate_world() {
	int area = 100; //area de un segmento de carretera
	int Nseg = 5;
	GeneracionCircuito(100);
	floor(area, Nseg);//tamaño de 1 segmento de carretera indivisual + numero de segmentos generados
	Cielo(100);
	lighting();
	ads();
}

void speed_bar_setting() {
	if (UI == SPEED_ON) {
		glClearColor(1, 1, 1, 1);
		speedBar();
		//mapUI();
	}
}

void time_of_day_setting() {
	if (LUCES == OFF) {

		glPushMatrix();
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);

		glPopMatrix();
	}
	else {
		glClearColor(1, 1, 1, 1);
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glPopMatrix();
	}

}

void Modo_Dibujo() {
	if (Dibujado == MAYA) {
		glPushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_TEXTURE_2D);
	}
}

void Climatologia() {
	if (Clima == NIEBLA) {
		glPushMatrix();
		glEnable(GL_FOG);
		GLfloat colour[] = { 0.2,0.2,0.2 };
		glFogfv(GL_FOG_COLOR, colour);
		glFogf(GL_FOG_DENSITY, 0.07);
		glPopMatrix();
	}
	if (Clima == TARENA) {
		glPushMatrix();
		glEnable(GL_FOG);
		GLfloat colour[] = { 0.5,0.2,0.1 };
		glFogfv(GL_FOG_COLOR, colour);
		glFogf(GL_FOG_DENSITY, 0.07);
		glPopMatrix();
	}
	if(Clima == DESPEJADO) {
		glDisable(GL_FOG);
	}
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glNormal3f(0, 1, 0);
	glColor3f(0, 0, 0);

	//HeadLight
	GLfloat p_light1[] = { 0.0, 0.7, 0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, p_light1);
	GLfloat d_light1[] = { 0.0, -0.5, -0.7 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, d_light1);

	speed_bar_setting();
	glPushMatrix();
	glTranslatef(0, -1, 0);
	moto();
	glPopMatrix();

	gluLookAt(PosX, 1, PosZ, PosX + despX, 1, PosZ + despZ, 0, 1, 0);

	PostedeLuz(0, 1, 3, GL_LIGHT2);
	//cout << "(" << PosX << "," << PosY << "," << PosZ << "," << PosX << "+" << despX << ",1," << PosZ << "+" << despZ << ")\n";
	//(0,1,0,0.296,1,0.95502)
	time_of_day_setting();
	Modo_Dibujo();
	Climatologia();


	populate_world();

	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float razon = (float)w / h;
	gluPerspective(45, razon, 1, 100);//TAG: VISION
}

void logic()
{
	static int init_time = glutGet(GLUT_ELAPSED_TIME);
	
	// Tiempo ahora (Final de la ventana)
	int now = glutGet(GLUT_ELAPSED_TIME);

	// Tiempo transcurrido en la ventana
	float time_diff = (now - init_time) / 1000.0f;

	// Numero de ticks transcurridos desde el inicio de la aplicacion
	ticks += tickRate * 360 * time_diff;

	// Calculo de movmiento
	displacement = time_diff * speed;

	// Movemos la ventana de tiempo
	init_time = now;

	PosX += displacement * despX;
	PosZ += displacement * despZ;

	glutPostRedisplay();
}

void onTimer(int tiempo)
{
	// Sirve para re-arrancar el reloj de cuenta atras
	glutTimerFunc(tiempo, onTimer, tiempo);
	logic();
}

void onArrow(int tecla, int x, int y)
{
	switch (tecla) {
	case GLUT_KEY_UP:
		//cout << PosX << " X,  " << PosZ << " Z \n";
		if (speed < 15) speed += 0.5;
		break;
	case GLUT_KEY_DOWN:
		//cout << PosX << " X,  " << PosZ << " Z \n";
		if (speed > 0.1) speed -= 1;
		else speed = 0.0;
		break;
	case GLUT_KEY_LEFT:
		//cout << angulo << " Angulo";
		if (angulo <= 73) {
			angulo += 0.75;
		}
		break;//74.25
	case GLUT_KEY_RIGHT:
		if (angulo >= -74){
			angulo -= 0.75;
		}
		//cout << angulo << " Angulo";
		break;
	}//-75.75
	
	// Partimos la distancia en sus componentes basado en el angulo
	despX = sin(angulo*PI / 180);
	despZ = cos(angulo*PI / 180);

	glutPostRedisplay();
}

void onKey(unsigned char key, int x, int y)
{
	switch (key) {
	case 's':
		if (Dibujado == MAYA) { Dibujado = TEXTURAS; }
		else Dibujado = MAYA;
		break;

	case 'l':
		if (LUCES == OFF) { LUCES = ON; }
		else LUCES = OFF;
		break;

	case 'n':
		if (Clima == NIEBLA) { Clima = DESPEJADO; }
		else Clima = NIEBLA;
		break;
	case 'm':
		if (Clima == TARENA) { Clima = DESPEJADO; }
		else Clima = TARENA;
		break;

	case 'c':
		if (UI == SPEED_ON) { UI = SPEED_OFF; }
		else UI = SPEED_ON;
		break;
	case 'd':
		if (DESIERTO1) { DESIERTO1=FALSE; }
		else DESIERTO1=TRUE;
		break;
	case 27:
		exit(0);
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
// Programa principal
{
	FreeImage_Initialise();
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(1280, 720); // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	init(); // Inicializacion propia. IMPORTANTE SITUAR AQUI
	std::cout << PROYECTO << " running" << std::endl; // Mensaje por consola

	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);

	glutSpecialFunc(onArrow);
	glutKeyboardFunc(onKey);

	glutMainLoop(); // Puesta en marcha del programa
	FreeImage_DeInitialise();
}
