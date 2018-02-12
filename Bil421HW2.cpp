#include <stdlib.h>			// standard definitions
#include <stdio.h>			// C I/O (for sprintf) 
#include <math.h>			// standard definitions
#include <time.h>
#include <string.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
//Burak Han Demirbilek - 151201022

typedef struct Thing {
	GLfloat x;
	GLfloat y;
	GLfloat veloX;
	GLfloat veloY;
	int level;
	int id;
	struct Thing * next;
	struct Thing * previous;
} thing_t;

typedef struct bomb {
	GLfloat x;
	GLfloat y;
	GLfloat level;
	struct bomb * next;
	struct bomb * previous;
} bomb_t;
void createThing(int level);
double randomize(int maxVal);
void printThings();
int calcGlutToGLCord(int y);
void drawThing(GLfloat x, GLfloat y, int level);
void reCalcCoords();
void pauseGame();
void singleStep();
void printDebug();
void createBomb(GLfloat x, GLfloat y);
void printBombs();
void drawBomb(GLfloat x, GLfloat y, int level);
void reCalcBombs();
void levelUpBomb(bomb_t * tmpBomb);
void checkBombIntersection(bomb_t *tmpBomb);
void deleteExplodedBombs();
void checkGameOver();
// global variable declarations

GLint windowHeight = 800, windowWidth = 800; //global windowHeight and windowWidth variables, default is 400x400
int numberOfThings = 20;
GLfloat thingWidth = 0.1f;
GLfloat thingHeigth = 0.1f;
GLfloat thingVelocityMax = 0.25f;
GLfloat bombWidth = 0.04f;
GLfloat bombHeigth = 0.04f;
int pause = 0;
int gameOver = 0;
int start = 0;
int score = 0;
int timeElapsed = 0;
int timeLimit = 60;
int FPS = 60;
int scoreLevel[5] = { -4000,-2000,-1000,1000,2000 };


thing_t * thingHead = NULL;
bomb_t * bombHead = NULL;



void createThings() {
	int i;
	for (i = 0; i < numberOfThings; i++) {
		createThing(i);
	}
	
}
void createThing(int id) {
	if (thingHead == NULL) {
		thingHead = (thing_t*)malloc(sizeof(thing_t));
		if (thingHead == NULL) {
			fprintf(stderr, "Error on creating head.\n");
			exit(1);
		}
		thingHead->x = randomize(1.0 / thingWidth)*thingWidth + thingWidth / 2;
		thingHead->y = (randomize(1.0 / thingHeigth) + 1)*thingHeigth - thingHeigth / 2;
		thingHead->veloX = randomize(thingVelocityMax * 100 + 1) / 100;
		randomize(2) == 0 ? thingHead->veloX *= 1 : thingHead->veloX *= -1;
		thingHead->veloY = randomize(thingVelocityMax * 100 + 1) / 100;
		randomize(2) == 0 ? thingHead->veloY *= 1 : thingHead->veloY *= -1;
		thingHead->level = id / (numberOfThings / 5);
		thingHead->id = id ;
		thingHead->next = NULL;
		thingHead->previous = NULL;
	}
	else {
		thing_t * tmpThing = thingHead;
		while (tmpThing->next != NULL) {
			tmpThing = tmpThing->next;
		}
		thing_t * tmpThingCreated = (thing_t*)malloc(sizeof(thing_t));
		tmpThing->next = tmpThingCreated;
		tmpThingCreated->x = randomize(1.0 / thingWidth)*thingWidth + thingWidth / 2;
		tmpThingCreated->y = (randomize(1.0 / thingHeigth) + 1)*thingHeigth - thingHeigth / 2;
		tmpThingCreated->veloX = randomize(thingVelocityMax * 100 + 1) / 100;
		randomize(2) == 0 ? tmpThingCreated->veloX *= 1 : tmpThingCreated->veloX *= -1;
		tmpThingCreated->veloY = randomize(thingVelocityMax * 100 + 1) / 100;
		randomize(2) == 0 ? tmpThingCreated->veloY *= 1 : tmpThingCreated->veloY *= -1;
		tmpThingCreated->level = id / (numberOfThings / 5);
		tmpThingCreated->id = id;
		tmpThingCreated->next = NULL;
		tmpThingCreated->previous = tmpThing;
	}
}
void createBomb(GLfloat x, GLfloat y) {
	
	if (bombHead == NULL) {
		bombHead = (bomb_t*)malloc(sizeof(bomb_t));
		if (bombHead == NULL) {
			fprintf(stderr, "Error on creating head.\n");
			exit(1);
		}
		bombHead->x = x;
		bombHead->y = y;
		bombHead->level = 0;
		bombHead->next = NULL;
		bombHead->previous = NULL;
	}
	else {
		bomb_t * tmpBomb = bombHead;
		while (tmpBomb->next!=NULL) {
			tmpBomb = tmpBomb->next;
		}
		bomb_t * tmpBombCreated = (bomb_t*)malloc(sizeof(bomb_t));
		tmpBomb->next = tmpBombCreated;
		tmpBombCreated->x = x;
		tmpBombCreated->y = y;
		tmpBombCreated->level = 0;
		tmpBombCreated->next = NULL;
		tmpBombCreated->previous = tmpBomb;
	}
}
void printBombs() {
	if (bombHead != NULL) {
		bomb_t * tmpBomb = bombHead;
		while (tmpBomb->next != NULL) {
			drawBomb(tmpBomb->x, tmpBomb->y, tmpBomb->level);
			tmpBomb = tmpBomb->next;
		}
		drawBomb(tmpBomb->x, tmpBomb->y, tmpBomb->level);
	}
}
void drawBomb(GLfloat x, GLfloat y, int level) {
	GLfloat colorx = 0.0f, colory = 0.0f, colorz = 0.0f;
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
	else return;
	GLfloat tmpWidth = bombWidth / (2 + level);
	GLfloat tmpHeigth = bombHeigth / (2 + level);
	glColor3f(colorx / 255.0f, colory / 255.0f, colorz / 255.0f);
	glBegin(GL_TRIANGLE_STRIP);		//using triangle strip instead of polygon which is deprecated after 3.0
	glVertex2f((x - tmpWidth), (y + tmpHeigth));		//making the diamond width=3 and height=4, when the center is in (0,0) then the vertexes should be in (0, 2), (-1.5, 0), (0, -2), (1.5, 0)
	glVertex2f((x + tmpWidth), (y + tmpHeigth));		//and also adding the center positions of diamond to move it in run time.
	glVertex2f((x - tmpWidth), (y - tmpHeigth));
	glVertex2f((x + tmpWidth), (y - tmpHeigth));
	glVertex2f((x - tmpWidth), (y + tmpHeigth));
	glEnd();
}

void printThings() {
	if (thingHead != NULL) {
		thing_t * tmpThing = thingHead;
		while (tmpThing->next != NULL) {
			drawThing(tmpThing->x, tmpThing->y, tmpThing->level);
			tmpThing = tmpThing->next;
		}
		drawThing(tmpThing->x, tmpThing->y, tmpThing->level);
	}
}
void drawExplosion(GLfloat x, GLfloat y) {
	glColor3f(251.0 / 255, 23.0 / 255, 23.0 / 255);	//the color of the circle is blue
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);	//defining our circles center.

	int lines;
	lines = 20;

	for (int i = 0; i <= lines; i++) {			//looping according to the total lines.
		float theta = ((2 * 3.14159f)*i) / lines;		//the angle calculated 2pi*i/lines which breaks our 2pi(360 degrees) into equal angles according to i.
		glVertex2f(cosf(theta)*thingWidth + x, sinf(theta)*thingHeigth + y);	//defining our vertexes using sinus and cosinus to draw a circle, also adding circle center positions to move it in runtime.
	}
	glEnd();
	glColor3f(1.0,1.0, 0);	//the color of the circle is blue
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);	//defining our circles center.

	for (int i = 0; i <= lines; i++) {			//looping according to the total lines.
		float theta = ((2 * 3.14159f)*i) / lines;		//the angle calculated 2pi*i/lines which breaks our 2pi(360 degrees) into equal angles according to i.
		glVertex2f(cosf(theta)*thingWidth/1.3 + x, sinf(theta)*thingHeigth/1.3 + y);	//defining our vertexes using sinus and cosinus to draw a circle, also adding circle center positions to move it in runtime.
	}
	glEnd();
}
void drawThing(GLfloat x, GLfloat y, int level) {
	if (level == 5) {
		drawExplosion(x, y);
	}
	GLfloat colorx = 0.0f, colory = 0.0f, colorz = 0.0f;
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
	else if (level == 5) {
		colorx = 0.0f;
		colory = 0.0f;
		colorz = 0.0f;
	}
	else return;
	glColor3f(colorx / 255, colory / 255, colorz / 255);
	glBegin(GL_TRIANGLE_STRIP);		//using triangle strip instead of polygon which is deprecated after 3.0
	glVertex2f((x - thingWidth / 2), (y + thingHeigth / 2));		//making the diamond width=3 and height=4, when the center is in (0,0) then the vertexes should be in (0, 2), (-1.5, 0), (0, -2), (1.5, 0)
	glVertex2f((x + thingWidth / 2), (y + thingHeigth / 2));		//and also adding the center positions of diamond to move it in run time.
	glVertex2f((x - thingWidth / 2), (y - thingHeigth / 2));
	glVertex2f((x + thingWidth / 2), (y - thingHeigth / 2));
	glVertex2f((x - thingWidth / 2), (y + thingHeigth / 2));
	glEnd();
	
}

void printDebug() {
	if (thingHead != NULL) {
		thing_t * tmpThing = thingHead;
		while (tmpThing->next != NULL) {
			printf("Thing: x:%f y:%f veloX:%f veloY:%f level:%d\n", tmpThing->x, tmpThing->y, tmpThing->veloX, tmpThing->veloY, tmpThing->level);
			tmpThing = tmpThing->next;
		}
		printf("Thing: x:%f y:%f veloX:%f veloY:%f level:%d\n", tmpThing->x, tmpThing->y, tmpThing->veloX, tmpThing->veloY, tmpThing->level);
	}
	if (bombHead != NULL) {
		bomb_t * tmpBomb=bombHead;
		while (tmpBomb->next != NULL) {
			printf("Bomb, x:%f, y:%f, level:%f\n", tmpBomb->x, tmpBomb->x, tmpBomb->level);
			tmpBomb = tmpBomb->next;
		}
		printf("Bomb, x:%f, y:%f, level:%f\n", tmpBomb->x, tmpBomb->x, tmpBomb->level);
	}
}
void reCalcCoords() {
	if (thingHead != NULL) {
		thing_t * tmpThing = thingHead;
		while (tmpThing->next != NULL) {
			tmpThing->x += tmpThing->veloX / FPS;
			tmpThing->y += tmpThing->veloY / FPS;
			if (tmpThing->x - thingWidth / 2 < 0 || tmpThing->x + thingWidth / 2 > 1) {
				tmpThing->veloX *= -1;
			}
			if (tmpThing->y - thingHeigth / 2< 0 || tmpThing->y + thingHeigth / 2 > 1) {
				tmpThing->veloY *= -1;
			}
			tmpThing = tmpThing->next;
		}
		tmpThing->x += tmpThing->veloX / FPS;
		tmpThing->y += tmpThing->veloY / FPS;
		if (tmpThing->x - thingWidth / 2 < 0 || tmpThing->x + thingWidth / 2 > 1) {
			tmpThing->veloX *= -1;
		}
		if (tmpThing->y - thingHeigth / 2< 0 || tmpThing->y + thingHeigth / 2 > 1) {
			tmpThing->veloY *= -1;
		}
	}
}
thing_t *searchThing(int id) {
	if (thingHead != NULL) {
		thing_t * tmpThing = thingHead;
		while (tmpThing->next != NULL) {
			if (tmpThing->id == id) return tmpThing;
			tmpThing = tmpThing->next;
		}
		if (tmpThing->id == id) return tmpThing;
	}
	return NULL;
}
void deleteExplodedThings(int id) {
	thing_t *tmpThing = searchThing(id);
	if (tmpThing->previous != NULL) {
		tmpThing->previous->next = tmpThing->next;
		if (tmpThing->next != NULL) {
			tmpThing->next->previous = tmpThing->previous;
		}
	}
	else {
		thingHead = tmpThing->next;
		if(tmpThing->next!=NULL)
			tmpThing->next->previous = NULL;
	}
	
}
void deleteExplodedBombs() {
	if (bombHead != NULL) {
		bomb_t * tmpBomb = bombHead;
		while (tmpBomb->next != NULL) {
			if (tmpBomb->level == 5) {
				if (tmpBomb->previous != NULL) {
					tmpBomb->previous->next = tmpBomb->next;
					tmpBomb->next->previous = tmpBomb->previous;
				}
				else {
					bombHead = tmpBomb->next;
					tmpBomb->next->previous = NULL;
				}
			}
			tmpBomb = tmpBomb->next;
		}
		if (tmpBomb->level == 5) {
			if (NULL!= tmpBomb->previous) {
				tmpBomb->previous->next = NULL;
			}
			else {
				bombHead = NULL;
			}
		}
		
	}
}
void reCalcBombs() {
	if (bombHead != NULL) {
		bomb_t * tmpBomb = bombHead;
		while (tmpBomb->next != NULL) {
			levelUpBomb(tmpBomb);
			tmpBomb = tmpBomb->next;
		}
		levelUpBomb(tmpBomb);
	}
	printf("Score: %d\n", score);
	printf("Time elapsed: %d seconds\n", timeElapsed);
}
void levelUpBomb(bomb_t * tmpBomb) {
	if (tmpBomb->level < 5) {
		tmpBomb->level++;
	}
}
void reCalcScore() {
	deleteExplodedBombs();
	if (bombHead != NULL) {
		bomb_t * tmpBomb = bombHead;
		while (tmpBomb->next != NULL) {
			checkBombIntersection(tmpBomb);
			tmpBomb = tmpBomb->next;
		}
		checkBombIntersection(tmpBomb);
	}
	

}
void checkBombIntersection(bomb_t *tmpBomb) {
	if (thingHead != NULL) {
		thing_t * tmpThing = thingHead;
		while (tmpThing->next != NULL) {
			if (fabs(tmpBomb->x - tmpThing->x) < (bombWidth/(2+tmpBomb->level) + thingWidth/2) &&
				fabs(tmpBomb->y - tmpThing->y) < (bombHeigth/(2+tmpBomb->level) + thingHeigth/2) &&
				tmpBomb->level == tmpThing->level) {
				//kill the thing, add score
				//return 1;
				printf("Thing x:%f,y:%f\nBomb x:%f,y:%f\n", tmpBomb->x, tmpBomb->y, tmpThing->x, tmpThing->y);
				if (tmpBomb->level == 0) {
					score += scoreLevel[0];
				}
				else if (tmpBomb->level == 1) {
					score += scoreLevel[1];
				}
				else if (tmpBomb->level == 2) {
					score += scoreLevel[2];
				}
				else if (tmpBomb->level == 3) {
					score += scoreLevel[3];
				}
				else if (tmpBomb->level == 4) {
					score += scoreLevel[4];
				}
				tmpThing->level = 5;	//killing the thing, TODO: after the linked list impl. of things, delete the thing after hanging out black for 1 sec.
				glutTimerFunc(1.0 * 1000, deleteExplodedThings, tmpThing->id);
			}

			tmpThing = tmpThing->next;
		}
		if (fabs(tmpBomb->x - tmpThing->x) * 2 < (bombWidth + thingWidth) &&
			fabs(tmpBomb->y - tmpThing->y) * 2 < (bombHeigth + thingHeigth) &&
			tmpBomb->level == tmpThing->level) {
			//kill the thing, add score
			//return 1;
			printf("Thing x:%f,y:%f\nBomb x:%f,y:%f\n", tmpBomb->x, tmpBomb->y, tmpThing->x, tmpThing->y);
			if (tmpBomb->level == 0) {
				score += scoreLevel[0];
			}
			else if (tmpBomb->level == 1) {
				score += scoreLevel[1];
			}
			else if (tmpBomb->level == 2) {
				score += scoreLevel[2];
			}
			else if (tmpBomb->level == 3) {
				score += scoreLevel[3];
			}
			else if (tmpBomb->level == 4) {
				score += scoreLevel[4];
			}
			tmpThing->level = 5;	//killing the thing, TODO: after the linked list impl. of things, delete the thing after hanging out black for 1 sec.
			glutTimerFunc(1.0 * 1000, deleteExplodedThings, tmpThing->id);
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
void print(GLfloat x, GLfloat y, char *string)
{
	glColor3d(0.0, 0.0, 0.0);
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);
	//get the length of the string to display
	int len = (int)strlen(string);

	//loop to display character by character
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
};
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
	printBombs();
	if (gameOver != 1) {
		char strTime[25];
		snprintf(strTime, 25, "Time Elapsed: %d\n", timeElapsed);
		print(0.0, 0.95, strTime);
		snprintf(strTime, 25, "Time Remaining: %d\n", timeLimit - timeElapsed);
		print(0.0, 0.90, strTime);
		char strScore[25];
		snprintf(strScore, 25, "Score: %d\n", score);
		print(0.80, 0.95, strScore);
	}
	else {
		char strMsg[25]="Game Over.";
		print(0.4, 0.6, strMsg);
		char strScore[25];
		snprintf(strScore, 25, "Score: %d\n", score);
		print(0.4, 0.55, strScore);
		snprintf(strScore, 25, "Press q to exit", score);
		print(0.4, 0.5, strScore);
	}
	
	glFlush();
	glutSwapBuffers();
}
void drawButton() {

	glColor3f(225 / 255.0, 247 / 255.0, 213/ 255.0);
	glBegin(GL_TRIANGLE_STRIP);		
	glVertex2f(0.3, 0.3);		
	glVertex2f(0.3, 0.5);			
	glVertex2f(0.7, 0.5);
	glVertex2f(0.7, 0.3);
	glVertex2f(0.3, 0.3);
	glEnd();
	char strMsg[150] = "Start!";
	print(0.45, 0.4, strMsg);
}
void myWelcomeDisplay()
{
	//clearing the window.

	glClearColor(201 / 255, 201 / 255.0, 255/ 255.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	char strMsg[150] = "Welcome to my 2D video game. ";
	print(0.3, 0.8, strMsg);
	strcpy_s(strMsg, "The objective of the game is to destroy all the evil boxes");
	print(0.12, 0.75, strMsg);
	strcpy_s(strMsg, "(Colored with Purple and Pink) while destroying as few good");
	print(0.1, 0.7, strMsg);
	strcpy_s(strMsg, "boxes as possible(Colored with yellow and green)");
	print(0.17, 0.65, strMsg);
	strcpy_s(strMsg, "Controls: P = Pause, Left Click = Place Bomb, S = SingleStep");
	print(0.1, 0.6, strMsg);
	strcpy_s(strMsg, "Press the button to start");
	print(0.35, 0.55, strMsg);
	drawButton();

	glFlush();
	glutSwapBuffers();
}
void checkGameOver() {
	int gameActive = 0;
	if (thingHead != NULL) {
		thing_t * tmpThing = thingHead;
		while (tmpThing->next != NULL) {
			if (tmpThing->level > 2) gameActive++;
			tmpThing = tmpThing->next;
		}
		if (tmpThing->level > 2) gameActive++;
		if (gameActive == 0) {
			gameOver = 1;
			pauseGame();
		}
	}
	else {
		gameOver = 1;
		pauseGame();
	}
	if (timeElapsed > timeLimit) gameOver = 1;
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
		
		break;
	case GLUT_KEY_RIGHT:
		
		break;
	}
	//glutPostRedisplay();
}
//Changes the circles position according to the mouse position
void leftClickMotion(int x, int y) {
	
	
}
//Changes the diamonds position according to the mouse position
void rightClickMotion(int x, int y) {
																						//printf("Right: x:%f, y:%f\n", diamondPosX, diamondPosY);
	//glutPostRedisplay();
}
void myTimeOut(int id) {

	reCalcCoords();
	reCalcScore();
	glutPostRedisplay();				//redisplaying

	if (pause == 0 && gameOver == 0) {
		glutTimerFunc(1.0 / FPS * 1000, myTimeOut, 0);	//setting the timer again for desired FPS value
	}

}
void myTimerForBombs(int id) {
	reCalcBombs();
	timeElapsed++;
	checkGameOver();
	if (pause == 0) {
		glutTimerFunc(1.0 * 1000, myTimerForBombs, 0);
	}

}
void myMouse(int b, int s, int x, int y) {
	switch (b) {
	case GLUT_LEFT_BUTTON:
		glutMotionFunc(leftClickMotion);			//if left click is pressed, than making the motion function to leftclickmotion function which changes the circles position
		if (s == GLUT_DOWN) {
			if (start == 0) {
				if (x*1.0 / windowWidth > 0.3 &&x*1.0 / windowWidth<0.7&&
					calcGlutToGLCord(y)*1.0 / windowHeight>0.3 &&calcGlutToGLCord(y)*1.0 / windowHeight < 0.5) {

					start = 1;
					myInit();
					glutDisplayFunc(myDisplay);
					glutTimerFunc(1.0 / FPS * 1000, myTimeOut, 0);		//setting the timer for desired FPS value
					glutTimerFunc(1.0 * 1000, myTimerForBombs, 0);		//setting the timer for desired FPS value
				}
				
			}
			else {
				if (pause != 1 && gameOver != 1) {
					createBomb((x) / (GLfloat)windowWidth, (calcGlutToGLCord(y)) / (GLfloat)windowHeight);
					reCalcScore();
					printf("Bomb placed in %d, %d\n", x, calcGlutToGLCord(y));
				}
			}
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
	case GLUT_MIDDLE_BUTTON:
		if (s == GLUT_DOWN) {
			pauseGame();
		}
		break;
	}
	//glutPostRedisplay();
}

void pauseGame() {
	if (gameOver != 1) {
		if (pause == 0) {
			pause = 1;
		}
		else {
		
			pause = 0;
			glutTimerFunc(1.0 / FPS * 1000, myTimeOut, 0);
			glutTimerFunc(1.0 * 1000, myTimerForBombs, 0);
		}
		
	}
}
void singleStep() {
	if (gameOver != 1) {
		if (pause == 0)
			pauseGame();
		else
			reCalcScore();
		int i;
		for (i = 0; i < FPS; i++)
			reCalcCoords();
		timeElapsed++;
		reCalcBombs();
		glutPostRedisplay();
		printDebug();
	}
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
	glutDisplayFunc(myWelcomeDisplay);		//  call myWelcomeDisplay to redraw window
	glutKeyboardFunc(myKeyboard);   // call myKeyboard when key is hit
	glutReshapeFunc(myReshape);     // call myReshape if window is resized
	glutMouseFunc(myMouse);         // call in mouse event 
	
	glutSpecialFunc(myArrow);					//setting the special func.
	
	

					// our own initializations
	

	glutMainLoop();			// pass control to GLUT, start it running
	return 0;               // ANSI C expects this
}
