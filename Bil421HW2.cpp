#include <stdlib.h>			// standard definitions
#include <stdio.h>			// C I/O (for sprintf) 
#include <math.h>			// standard definitions
#include <time.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
//Burak Han Demirbilek - 151201022

double randomize(int maxVal);
void printThings();
int calcGlutToGLCord(int y);
void drawThing(GLfloat x, GLfloat y, int level);
void reCalcCoords();
void pauseGame();
void singleStep();
void printDebug();
void createBomb(int x, int y);
// global variable declarations

GLint windowHeight = 800, windowWidth = 800; //global windowHeight and windowWidth variables, default is 400x400
int numberOfThings = 20;
GLfloat thingWidth = 0.1;
GLfloat thingHeigth = 0.1;
GLfloat thingVelocityMax = 0.25;
GLfloat bombWidth = 0.4;
GLfloat bombHeigth = 0.4;
int pause = 0;
int gameOver = 0;
int score = 0;
int FPS = 60;
typedef struct Thing {
	GLfloat x;
	GLfloat y;
	GLfloat veloX;
	GLfloat veloY;
	int level;
};

typedef struct bomb {
	GLfloat x;
	GLfloat y;
	GLfloat level;
	struct bomb * next;
} bomb_t;

struct Thing things[20];
bomb_t * head = NULL;


void createThings() {
	int i;
	for (i = 0; i < numberOfThings; i++) {
		struct Thing *tmpThing;
		tmpThing = (Thing*)malloc(sizeof(struct Thing));
		tmpThing->x = randomize(1.0/thingWidth)*thingWidth ;
		tmpThing->y = (randomize(1.0 / thingWidth) +1)*thingHeigth ;
		tmpThing->veloX = randomize(thingVelocityMax * 100 + 1)/100 ;
		randomize(2) == 0 ? tmpThing->veloX *= 1 : tmpThing->veloX *= -1;
		tmpThing->veloY = randomize(thingVelocityMax * 100 + 1)/100 ;
		randomize(2) == 0 ? tmpThing->veloY *= 1 : tmpThing->veloY *= -1;
		tmpThing->level = i / 4;
		things[i] = *tmpThing;
	}
	
}
void createBomb(int x, int y) {
	
	if (head == NULL) {
		head = (bomb_t*)malloc(sizeof(bomb_t));
		if (head == NULL) {
			fprintf(stderr, "Error on creating head.\n");
			exit(1);
		}
		head->x = 1;
		head->next = NULL;
		return;
	}
	else {
		bomb_t * tmpBomb = head;
		while (tmpBomb->next!=NULL) {
			tmpBomb = tmpBomb->next;
		}
		bomb_t * tmpBombCreated = (bomb_t*)malloc(sizeof(bomb_t));
		tmpBomb->next = tmpBombCreated;
		tmpBombCreated->x = x;
		tmpBombCreated->y = y;
		tmpBombCreated->level = 0;
		tmpBombCreated->next = NULL;
	}
}

void printThings() {
	int i;
	for (i = 0; i < numberOfThings; i++) {
		//printf("Thing no:%d, x:%f y:%f veloX:%f veloY:%f level:%d\n", i, things[i].x, things[i].y, things[i].veloX, things[i].veloY, things[i].level);
		drawThing(things[i].x, things[i].y, things[i].level);
	}
}
void printDebug() {
	int i;
	for (i = 0; i < numberOfThings; i++) {
		printf("Thing no:%d, x:%f y:%f veloX:%f veloY:%f level:%d\n", i, things[i].x, things[i].y, things[i].veloX, things[i].veloY, things[i].level);
	}
}
void reCalcCoords() {
	int i;
	for (i = 0; i < numberOfThings; i++) {
		things[i].x += things[i].veloX / FPS;
		things[i].y += things[i].veloY / FPS;
		//printf("%f, %f\n", things[i].x, things[i].y);
		if (things[i].x < 0 || things[i].x > 1 - thingWidth) {
			things[i].veloX *= -1;
		}
		if (things[i].y < thingHeigth || things[i].y > 1) {
			things[i].veloY *= -1;
		}
	}
}
double randomize(int maxVal) {
	double r = rand()%(maxVal);      // returns a pseudo-random integer between 0 and RAND_MAX
	return r;
}
int calcGlutToGLCord(int y) {
	return windowHeight - y;
}
void drawThing(GLfloat x, GLfloat y, int level) {
	GLfloat colorx=0.0f, colory=0.0f, colorz=0.0f;
	if (level == 0) {
		colorx = 255.0f;
		colory = 255.0f;
		colorz = 0.0f;
	}
	else if (level == 1) {
		colorx = 204.0f;
		colory = 204.0f;
		colorz = 0.0f;
	}
	else if (level == 2) {
		colorx = 153.0f;
		colory = 153.0f;
		colorz = 0.0f;
	}
	else if (level == 3) {
		colorx = 204.0f;
		colory = 0.0f;
		colorz = 204.0f;
	}
	else if (level == 4) {
		colorx = 127.0f;
		colory = 0.0f;
		colorz = 255.0f;
	}		
	glColor3f(colorx/255, colory/255, colorz/255);
	glBegin(GL_TRIANGLE_STRIP);		//using triangle strip instead of polygon which is deprecated after 3.0
	glVertex2f((x),(y));		//making the diamond width=3 and height=4, when the center is in (0,0) then the vertexes should be in (0, 2), (-1.5, 0), (0, -2), (1.5, 0)
	//printf("%f, %f\n",x,y);
	glVertex2f((x+ thingWidth), (y));		//and also adding the center positions of diamond to move it in run time.
	glVertex2f((x+ thingWidth), (y-thingHeigth));
	glVertex2f((x), (y- thingHeigth));
	glVertex2f((x), (y));
	glEnd();
}

void myInit()
{
	glClearColor(1, 1, 1, 1.0);		// Setting the background color to white
	glShadeModel(GL_SMOOTH);		// smooth shading
	srand(time(NULL));
	createThings();
	
}
void myReshape(int winWidth, int winHeight)
{
	//setting the new window height and width to our global variables.
	windowHeight = winHeight;
	windowWidth = winWidth;

	//updating the viewport and projection
	glViewport(0, 0, winWidth, winHeight);  // update the viewport
	glMatrixMode(GL_PROJECTION);              // update the projection
	glLoadIdentity();

	//mapping our drawing area coords
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//Redisplaying everything after reshaping.
	glutPostRedisplay();

}

//This function draws circle for our assignment
void myDisplay()
{
	//clearing the window.
	glClear(GL_COLOR_BUFFER_BIT);

	printThings();
	glFlush();
	glutSwapBuffers();
}


void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':				//if q key is pressed than quits.
		exit(0);
		break;
	case 'p':				//if p key is pressed than pauses the game.
		pauseGame();
		break;
	case 's':				//if s key is pressed than .
		singleStep();
	}
	//glutPostRedisplay();
}
void myArrow(int key, int x, int y) {
	int state = glutGetModifiers();
	switch (key)
	{
	case GLUT_KEY_UP:
		
		break;
	case GLUT_KEY_DOWN:
		
		break;
	case GLUT_KEY_LEFT:
		createBomb(x, calcGlutToGLCord(y));
		printf("Bomb placed in %d, %d\n",x, calcGlutToGLCord(y));
		break;
	case GLUT_KEY_RIGHT:
		
		break;
	}
	glutPostRedisplay();
}
//Changes the circles position according to the mouse position
void leftClickMotion(int x, int y) {
	
	
}
//Changes the diamonds position according to the mouse position
void rightClickMotion(int x, int y) {
																						//printf("Right: x:%f, y:%f\n", diamondPosX, diamondPosY);
	//glutPostRedisplay();
}
void myMouse(int b, int s, int x, int y) {
	switch (b) {
	case GLUT_LEFT_BUTTON:
		glutMotionFunc(leftClickMotion);			//if left click is pressed, than making the motion function to leftclickmotion function which changes the circles position
		if (s == GLUT_DOWN) {
			//printf("DOWN\n");
			
		}
		else if (s == GLUT_UP) {
			//printf("UP\n");
			
		}
		break;
	case GLUT_RIGHT_BUTTON:
		glutMotionFunc(rightClickMotion);			//if right click is pressed, than making the motion function to rightclickmotion function which changes the diamons position
		if (s == GLUT_DOWN) {
			//printf("DOWN\n");
		}
		else if (s == GLUT_UP) {
			//printf("UP\n");
		}
		break;
	}
	//glutPostRedisplay();
}
void myTimeOut(int id) {
	reCalcCoords();
	glutPostRedisplay();				//redisplaying
	if (pause==0) {
		glutTimerFunc(1.0 / FPS * 1000, myTimeOut, 0);	//setting the timer again for desired FPS value
	}
	
}
void pauseGame() {
	if (pause == 0) {
		pause = 1;
	}
	else {
		pause = 0;
		glutTimerFunc(1.0 / FPS * 1000, myTimeOut, 0);
	}
}
void singleStep() {
	if(pause==0)
		pauseGame();
	int i;
	for (i = 0; i < FPS; i++)
		reCalcCoords();
	glutPostRedisplay();
	printDebug();
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);   //Initialize glut and gl
	glutInitDisplayMode(
		GLUT_DOUBLE |		// double buffering
		GLUT_RGB);		    // RGB color mode

	glutInitWindowSize(windowWidth, windowHeight);   // 400x400 window
	glutInitWindowPosition(0, 0);    // upper left
	glutCreateWindow("Bil 421 - Assignment 2 - Burak Han Demirbilek"); 	// create the window

																		//register callbacks
	glutDisplayFunc(myDisplay);		//  call myDisplay to redraw window
	glutKeyboardFunc(myKeyboard);   // call myKeyboard when key is hit
	glutReshapeFunc(myReshape);     // call myReshape if window is resized
	glutMouseFunc(myMouse);         // call in mouse event 
	glutTimerFunc(1.0 / FPS * 1000, myTimeOut, 0);		//setting the timer for desired FPS value
	glutSpecialFunc(myArrow);					//setting the special func.


	myInit();				// our own initializations
	

	glutMainLoop();			// pass control to GLUT, start it running
	return 0;               // ANSI C expects this
}
