//******************************************************************
// Project:		Car Driving Simulator in OpenGL
//******************************************************************

//******************************************************************
// Controls:
// a  : forwards	F1: Look forward
// z  : backwards	F2: Look right
// q  : turn left	F3: Look left
// w  : turn right	F4: Go back to default car view
// F5: Move camera back (+x) and right (-z) of vehicle
// F6: Move camera front (-x) and right (-z) of vehicle
// F7: Move camera front (-x) and left (+z) of vehicle
// F8: Move camera back (+x) and left (+z) of vehicle
// F9: Like F5 but farther away from vehicle
// F10: Like F6 but farther away from vehicle
// F11: Like F7 but farther away from vehicle
// F12: Like F8 but farther away from vehicle
//******************************************************************

#define PROGRAM_TITLE "Interactive OpenGL Game"
#define SPACEBAR 32

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <math.h>
#include <iostream>
#include <sstream>
#include <GL/glut.h>

using namespace std;

/*****************Global Variables Start*****************/
GLUquadricObj *t;
GLfloat r = 0, g = 0, b = 1;
float rotateCylinderBuildingTop = 1.0f;
float rotateSphereBuildingTop = 20.0f;
float antRot = 0;
float mapBoundary = 140;
int counter = 0;

int fixedTurnInterval = 25;
int numBuildingsPerBlock = 3;
int currentAxis = 1;
int rotateCarAngle = 0;

float angle;
float xt, yt, zt = 0.0;
float Window_Width = 1920;
float Window_Height = 1080;
float carSpeed = 0.5;

bool paused = false;

bool f1 = false, f2 = false, f3 = false, f4 = false, f5 = false,
f6 = false, f7 = false, f8 = false, f9 = false,
f10 = false, f11 = false, f12 = false;
/*****************Global Variables End*******************/

GLvoid Transform(GLfloat Width, GLfloat Height)
{
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, Width / Height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

GLvoid InitGL(GLfloat Width, GLfloat Height)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glLineWidth(2.0);              /* Add line width,   ditto */
	Transform(Width, Height); /* Perform the transformation */
	t = gluNewQuadric();
	gluQuadricDrawStyle(t, GLU_FILL);
}

void printString(string str) {
	for (int i = 0; i < str.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
	}
}

void printString2(string str) {
	for (int i = 0; i < str.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}

void setOrthographicProjection() {
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, Window_Width, Window_Height, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void endPrintingOfText() {
	//Set things up for normal rendering of display
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	// Restore the original projection matrix
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void setText() {
	setOrthographicProjection();

	// std::to_string is not supported by codeblocks c++ compiler, we
	// must use stringstream as an alternative due to this
	stringstream ss;
	string axis, xVal, yVal, zVal;

	// Change int values to their string equivalent
	ss << currentAxis;
	ss >> axis;
	ss.clear();
	ss << xt;
	ss >> xVal;
	ss.clear();
	ss << yt;
	ss >> yVal;
	ss.clear();
	ss << zt;
	ss >> zVal;
	ss.clear();
	// done parsing values

	glColor3f(0.0f, 0.8f, 0.0f);

	glLoadIdentity();
	glRasterPos2f(Window_Width * 0.02, Window_Height * 0.10);
	printString2("CAR COORDINATES");

	glLoadIdentity();
	glRasterPos2f(Window_Width * 0.02, Window_Height * 0.12);
	printString("X: " + xVal);

	glLoadIdentity();
	glRasterPos2f(Window_Width * 0.02, Window_Height * 0.135);
	printString("Y: " + yVal);

	glLoadIdentity();
	glRasterPos2f(Window_Width * 0.02, Window_Height * 0.150);
	printString("Z: " + zVal);

	endPrintingOfText();
	glutPostRedisplay();
}

void drawCar() {
	///////////////////////DRAW CAR
	glPushMatrix();

	glTranslatef(xt, yt, zt);
	glRotatef(rotateCarAngle, 0, 1, 0);
	glRotatef(90, 0, 1, 0);

	glBegin(GL_QUADS);

	/* top of cube*/
	//********MAIN BODY*******
	glColor3f(r, g, b);
	//top
	glVertex3f(1.0, 1.25, 1.5);
	glVertex3f(-1.0, 1.25, 1.5);
	glVertex3f(-1.0, 1.25, -1.5);
	glVertex3f(1.0, 1.25, -1.5);
	//bottom
	glVertex3f(1.0, 0.25, 1.5);
	glVertex3f(-1.0, 0.25, 1.5);
	glVertex3f(-1.0, 0.25, -1.5);
	glVertex3f(1.0, 0.25, -1.5);
	//front
	glVertex3f(-1.0, 0.25, 1.5);
	glVertex3f(-1.0, 1.25, 1.5);

	glVertex3f(-1.0, 1.25, -1.5);
	glVertex3f(-1.0, 0.25, -1.5);

	//back
	glVertex3f(1.0, 1.25, 1.5);
	glVertex3f(1.0, 0.25, 1.5);
	glVertex3f(1.0, 0.25, -1.5);
	glVertex3f(1.0, 1.25, -1.5);
	//left
	glVertex3f(1.0, 0.25, 1.5);
	glVertex3f(-1.0, 0.25, 1.5);
	glVertex3f(-1.0, 1.25, 1.5);
	glVertex3f(1.0, 1.25, 1.5);

	//right
	glVertex3f(1.0, 0.25, -1.5);
	glVertex3f(-1.0, 0.25, -1.5);
	glVertex3f(-1.0, 1.25, -1.5);
	glVertex3f(1.0, 1.25, -1.5);

	//*********TOP BODY*******
	//top
	glVertex3f(1.0, 2.0, -0.5);
	glVertex3f(-1.0, 2.0, -0.5);
	glVertex3f(-1.0, 2.0, -1.1);
	glVertex3f(1.0, 2.0, -1.1);
	//bottom
	glVertex3f(1.0, 1.25, -0.3);
	glVertex3f(-1.0, 1.25, -0.3);
	glVertex3f(-1.0, 1.25, -1.3);
	glVertex3f(1.0, 1.25, -1.3);
	//front
	glVertex3f(-1.0, 2.0, -0.5);
	glVertex3f(-1.0, 1.25, -0.3);
	glVertex3f(-1.0, 1.25, -1.3);
	glVertex3f(-1.0, 2.0, -1.1);
	//back
	glVertex3f(1.0, 2.0, -0.5);
	glVertex3f(1.0, 1.25, -0.3);
	glVertex3f(1.0, 1.25, -1.3);
	glVertex3f(1.0, 2.0, -1.1);
	//left
	glVertex3f(1.0, 2.0, -0.5);
	glVertex3f(1.0, 1.25, -0.3);
	glVertex3f(-1.0, 1.25, -0.3);
	glVertex3f(-1.0, 2.0, -0.5);
	//right
	glVertex3f(-1.0, 2.0, -1.1);
	glVertex3f(-1.0, 1.25, -1.3);
	glVertex3f(1.0, 1.25, -1.3);
	glVertex3f(1.0, 2.0, -1.1);

	//********WINDOWS AND PLATE
	glColor3f(0.3, 0.3, 0.3);

	//front window
	glVertex3f(0.75, 1.91, -1.15);//driver top left corner
	glVertex3f(-0.75, 1.91, -1.15);//driver top right corner
	glVertex3f(-0.75, 1.33, -1.28);//driver bottom right corner
	glVertex3f(0.75, 1.33, -1.28);//driver bottom left corner

	//back window
	glVertex3f(0.75, 1.91, -0.46);//driver top left corner
	glVertex3f(-0.75, 1.91, -0.46);//driver top right corner
	glVertex3f(-0.75, 1.33, -0.32);//driver bottom right corner
	glVertex3f(0.75, 1.33, -0.32);//driver bottom left corner

	//plate
	glColor3f(0.8, 0.8, 0.8);
	glVertex3f(0.2, 0.65, 1.505);//viewer top right corner
	glVertex3f(-0.2, 0.65, 1.505);//viewer top left corner
	glVertex3f(-0.2, 0.4, 1.505);//viewer bottom left corner
	glVertex3f(0.2, 0.4, 1.505);//viewer bottom right corner

	glEnd();

	glPushMatrix();
	//make the tires
	glRotatef(90.0, 0, 1, 0);
	glTranslatef(1.0, 0.39, -1.0);
	glColor3f(0, 0, 0);
	glutSolidTorus(0.14, 0.17, 10, 25);//front left tire
	glTranslatef(-2.0, 0, 0);
	glutSolidTorus(0.14, 0.17, 10, 25);//back left tire
	glTranslatef(0, 0, 2.0);
	glutSolidTorus(0.14, 0.17, 10, 25);//back right tire
	glTranslatef(2.0, 0, 0);
	glutSolidTorus(0.14, 0.17, 10, 25);//front right tire
	glPopMatrix();

	glPushMatrix();
	//right rocket launcher
	glTranslatef(1.08, 0.95, -1.0);
	glColor3f(0.8, 0.0, 0.0);
	glRotatef(180.0, 0, 1, 0);
	glutSolidCone(0.1, 0.2, 10, 10);
	glTranslatef(0.0, 0.0, -0.2);
	gluCylinder(t, 0.05, 0.05, 0.3, 10, 10);

	//left rocket launcher
	glTranslatef(2.16, 0.0, 0.0);
	gluCylinder(t, 0.05, 0.05, 0.3, 10, 10);
	glTranslatef(0.0, 0.0, 0.2);
	glutSolidCone(0.1, 0.2, 10, 10);
	glPopMatrix();

	glPushMatrix();
	//on to the headlights
	glTranslatef(-0.7, 0.95, -1.5);
	glColor3f(0.7, 0.7, 0.2);
	gluSphere(t, 0.125, 10, 10);//right headlight
	glTranslatef(1.4, 0.0, 0.0);
	gluSphere(t, 0.125, 10, 10);//left headlight
	glTranslatef(0.0, 0.0, 3.0);
	glColor3f(0.7, 0.2, 0.2);
	gluSphere(t, 0.125, 10, 10);//left taillight
	glTranslatef(-1.4, 0.0, 0.0);
	gluSphere(t, 0.125, 10, 10);//right taillight
	glPopMatrix();

	glPushMatrix();

	//antenna stem
	glPushMatrix();
	glTranslatef(0.0, 1.25, 1.0);
	glRotatef(-90.0, 1, 0, 0);
	glColor3f(0.3, 0.3, 0.3);
	gluCylinder(t, 0.1, 0.1, 1.0, 10, 10);
	glPopMatrix();

	//antenna tip
	glPushMatrix();
	glTranslatef(0.0, 2.25, 1.0);
	glRotatef(antRot, 0, 1, 0);
	glTranslatef(0.0, 0, 0.0);
	glColor3f(1, 0.0, 0.0);
	glutSolidCube(0.40);
	glPopMatrix();

	glPopMatrix();

	//*************************************************************
}

// draws rectangular building with windows
void drawRectangleBuilding(int i, int j) {
	glMatrixMode(GL_MODELVIEW);
	glCullFace(GL_BACK);

	glPushMatrix();
	//middle

	glTranslatef(i + 9, 7.0f, j + 7.0);
	glScalef(3, 3, 3);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glBegin(GL_QUADS);
	// Top face (y = 1.0f)
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 2.5f, -1.0f);
	glVertex3f(-1.0f, 2.5f, -1.0f);
	glVertex3f(-1.0f, 2.5f, 1.0f);
	glVertex3f(1.0f, 2.5f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.0f, -2.5f, 1.0f);
	glVertex3f(-1.0f, -2.5f, 1.0f);
	glVertex3f(-1.0f, -2.5f, -1.0f);
	glVertex3f(1.0f, -2.5f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 2.5f, 1.0f);
	glVertex3f(-1.0f, 2.5f, 1.0f);
	glVertex3f(-1.0f, -2.5f, 1.0f);
	glVertex3f(1.0f, -2.5f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -2.5f, -1.0f);
	glVertex3f(-1.0f, -2.5f, -1.0f);
	glVertex3f(-1.0f, 2.5f, -1.0f);
	glVertex3f(1.0f, 2.5f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 2.5f, 1.0f);
	glVertex3f(-1.0f, 2.5f, -1.0f);
	glVertex3f(-1.0f, -2.5f, -1.0f);
	glVertex3f(-1.0f, -2.5f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 2.5f, -1.0f);
	glVertex3f(1.0f, 2.5f, 1.0f);
	glVertex3f(1.0f, -2.5f, 1.0f);
	glVertex3f(1.0f, -2.5f, -1.0f);

	//Right Window
	glColor3f(0.3, 0.3, 0.3);
	glVertex3f(1.1, 0.3, -0.5);
	glVertex3f(1.1, 0.3, 0.5);
	glVertex3f(1.1, -0.3, 0.5);
	glVertex3f(1.1, -0.3, -0.5);

	//Left Window
	glColor3f(0.3, 0.3, 0.3);
	glVertex3f(-1.1, 0.3, 0.5);
	glVertex3f(-1.1, 0.3, -0.5);
	glVertex3f(-1.1, -0.3, -0.5);
	glVertex3f(-1.1, -0.3, 0.5);

	//Front Window
	glColor3f(0.3, 0.3, 0.3);
	glVertex3f(0.5f, 1.0f, 1.1f);
	glVertex3f(-0.5f, 1.0f, 1.1f);
	glVertex3f(-0.5f, -1.0f, 1.1f);
	glVertex3f(0.5f, -1.0f, 1.1f);

	//Back Window
	glColor3f(0.3, 0.3, 0.3);
	glVertex3f(0.5f, -1.0f, -1.1f);
	glVertex3f(-0.5f, -1.0f, -1.1f);
	glVertex3f(-0.5f, 1.0f, -1.1f);
	glVertex3f(0.5f, 1.0f, -1.1f);

	glEnd();  // End of drawing rectangle building

	glPopMatrix();
}

// draws cylinder building with cube on top
void drawCylinderBuilding(int i, int j) {
	glMatrixMode(GL_MODELVIEW);
	glCullFace(GL_BACK);

	//draw cube on top
	glPushMatrix();
	//right
	glTranslatef(i + 8, 9.0f, j - 8.0);
	glScalef(3, 3, 3);

	glRotatef(rotateCylinderBuildingTop, 0.0f, 1.0f, 0.0f);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glBegin(GL_QUADS);
	// Top face (y = 1.0f)
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, -1.0f);
	glVertex3f(-1.5f, 1.0f, -1.0f);
	glVertex3f(-1.5f, 1.0f, 1.0f);
	glVertex3f(1.5f, 1.0f, 1.0f);
	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.5f, -1.0f, 1.0f);
	glVertex3f(-1.5f, -1.0f, 1.0f);
	glVertex3f(-1.5f, -1.0f, -1.0f);
	glVertex3f(1.5f, -1.0f, -1.0f);
	// Front face (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, 1.0f);
	glVertex3f(-1.5f, -1.0f, 1.0f);
	glVertex3f(1.5f, -1.0f, 1.0f);
	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, -1.0f);
	glVertex3f(-1.5f, -1.0f, -1.0f);
	glVertex3f(-1.5f, 1.0f, -1.0f);
	glVertex3f(1.5f, 1.0f, -1.0f);
	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, -1.0f);
	glVertex3f(-1.5f, -1.0f, -1.0f);
	glVertex3f(-1.5f, -1.0f, 1.0f);
	// Right face (x = 1.0f)
	glColor3f(1.5f, 0.0f, 1.0f);
	glVertex3f(1.5f, 1.0f, -1.0f);
	glVertex3f(1.5f, 1.0f, 1.0f);
	glVertex3f(1.5f, -1.0f, 1.0f);
	glVertex3f(1.5f, -1.0f, -1.0f);
	glEnd(); // End of drawing color-cube

	glPopMatrix();
	// end of drawing cube

	// draw cylinder on bottom
	glPushMatrix();
	glTranslatef(i + 8, 7.0f, j - 8.0);
	glScalef(3, 3, 3);


	glColor3f(0.3, 0.3, 0.3);
	glRotatef(90.0f, 25, 0.0f, 0.5f);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glBegin(GL_POLYGON);
	GLUquadricObj *quadratic = gluNewQuadric();
	gluCylinder(quadratic, 1.0, 1.0, 3.0f, 32, 32);
	glEnd();

	glPopMatrix();
	// end of drawing cylinder
}

// draws sphere building with small cylinder on top
void drawSphereBuilding(int i, int j)
{
	glMatrixMode(GL_MODELVIEW);
	glCullFace(GL_BACK);

	// draw small cylinder on top
	glPushMatrix();
	
	glTranslatef(i + 18.0f, 8.9f, j - 9.0f);

	glScalef(3, 3, 3);

	glColor3f(0.5, 0.0, 1.0);

	glRotatef(rotateSphereBuildingTop, 0.0, 1.0f, 0.0f);
	glRotatef(90, 1, 0, 0);

	glBegin(GL_POLYGON);
	GLUquadricObj *quadratic = gluNewQuadric();
	gluCylinder(quadratic, 0.2, 0.2, 0.3f, 32, 32);
	glEnd();

	glPopMatrix();
	// end draw small cylinder

	// draw sphere on bottom
	glPushMatrix();
	glTranslatef(i + 18.0f, 4.0f, (j - 9.0));
	glScalef(2, 2, 2);


	glColor3f(1.0, 0.64, 0.0);
	glutSolidSphere(2, 30, 30);

	glPopMatrix();
	// end draw sphere
}

// Creates a building specified by "selection"
void drawBuildingGeneral(int i, int j, int selection) {
	switch (selection) {
	case 0:
		drawRectangleBuilding(i, j);
		break;
	case 1:
		drawCylinderBuilding(i, j);
		break;
	case 2:
		drawSphereBuilding(i, j);
		break;
	}
}

void drawGround()
{
	//Ground
	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(0.0f, 0.0f, 0.0f);

	glBegin(GL_QUADS);
	glVertex3f(-mapBoundary * 2, 0.0f, -mapBoundary * 2);
	glVertex3f(-mapBoundary * 2, 0.0f, mapBoundary * 2);
	glVertex3f(mapBoundary * 2, 0.0f, mapBoundary * 2);
	glVertex3f(mapBoundary * 2, 0.0f, -mapBoundary * 2);
	glEnd();

	glPopMatrix();
}

void drawVerticalStreet(int j)
{
	glPushMatrix();

	//the -2.2 is the offset to make the road in line with the origin. We want an intersection at (0,...,0)
	glTranslatef(0, 0.01f, j - 2.2);
	glScalef(5, 0, 5);

	glBegin(GL_QUADS);

	// Sides of Road
	glColor3f(0, 0, 0);
	glVertex3f(-mapBoundary / 2.5, 0.1, -0.05);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.0);
	glVertex3f(mapBoundary / 2.5, 0.1, 0.0);
	glVertex3f(mapBoundary / 2.5, 0.1, -0.05);

	glVertex3f(-mapBoundary / 2.5, 0.1, 1.05);
	glVertex3f(-mapBoundary / 2.5, 0.1, 1.00);
	glVertex3f(mapBoundary / 2.5, 0.1, 1.00);
	glVertex3f(mapBoundary / 2.5, 0.1, 1.05);


	glColor3f(0.7, 0.7, 0.7);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.45);		//a long road
	glVertex3f(mapBoundary / 2.5, 0.1, 0.45);
	glVertex3f(mapBoundary / 2.5, 0.1, 0);

	glColor3f(1.0, 0.75, 0.0);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.45);		//a median
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.55);
	glVertex3f(mapBoundary / 2.5, 0.1, 0.55);
	glVertex3f(mapBoundary / 2.5, 0.1, 0.45);

	glColor3f(0.7, 0.7, 0.7);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.55);
	glVertex3f(-mapBoundary / 2.5, 0.1, 1);			//a long road
	glVertex3f(mapBoundary / 2.5, 0.1, 1);
	glVertex3f(mapBoundary / 2.5, 0.1, 0.55);

	glEnd();

	glPopMatrix();
}

void drawHorizontalStreet(int j) {
	glPushMatrix();

	//-2.0 is to offset the road so our car starts on the intersection at (0,..,0)
	glTranslatef(j - 2.5, 0.02f, 0.0);
	glScalef(5, 0, 5);

	glRotatef(90, 0, 1, 0);

	glBegin(GL_QUADS);

	// Sides of Road
	glColor3f(0, 0, 0);
	glVertex3f(-mapBoundary / 2.5, 0.1, -0.05);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.0);
	glVertex3f(mapBoundary / 2.5, 0.1, 0.0);
	glVertex3f(mapBoundary / 2.5, 0.1, -0.05);

	glVertex3f(-mapBoundary / 2.5, 0.1, 1.05);
	glVertex3f(-mapBoundary / 2.5, 0.1, 1.00);
	glVertex3f(mapBoundary / 2.5, 0.1, 1.00);
	glVertex3f(mapBoundary / 2.5, 0.1, 1.05);


	glColor3f(0.7, 0.7, 0.7);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.45);		//a long road
	glVertex3f(mapBoundary / 2.5, 0.1, 0.45);
	glVertex3f(mapBoundary / 2.5, 0.1, 0);

	glColor3f(1.0, 0.75, 0.0);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.45);		//a median
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.55);
	glVertex3f(mapBoundary / 2.5, 0.1, 0.55);
	glVertex3f(mapBoundary / 2.5, 0.1, 0.45);

	glColor3f(0.7, 0.7, 0.7);
	glVertex3f(-mapBoundary / 2.5, 0.1, 0.55);
	glVertex3f(-mapBoundary / 2.5, 0.1, 1);			//a long road
	glVertex3f(mapBoundary / 2.5, 0.1, 1);
	glVertex3f(mapBoundary / 2.5, 0.1, 0.55);

	glEnd();
	glPopMatrix();
}

//Each block is 20x20 units
void generateBlock(int i, int j)
{
	//Rectangle
	drawBuildingGeneral(i, j, 0);

	//Cylinder
	drawBuildingGeneral(i, j, 1);

	//Sphere
	drawBuildingGeneral(i, j, 2);
}

void generateCity()
{
	//Draw the streets
	for (int i = -10; i < 10; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			//Vertical and Horizontal streets are seperated by 25 units
			drawVerticalStreet(25 * i);
			drawHorizontalStreet(25 * j);

			//If the block to draw is within range
			if (25 * i > xt + 50 || 25 * j > zt + 50 || 25 * i < xt - 75 || 25 * j < zt - 75)
			{
				continue;

			}
			generateBlock(25 * i, 25 * j);
		}

	}
}

void processLookAtView()
{
	//Look forwards
	if (f1 == true) {
		glLoadIdentity();
		if (rotateCarAngle == 0)
		{
			gluLookAt(xt - 2.0, 2, zt, xt - 5.0, yt + 1.5, zt, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 90) {
			gluLookAt(xt, 2, zt + 2.0, xt, yt + 1.5, zt + 5.0, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 180) {
			gluLookAt(xt + 2.0, 2, zt, xt + 5.0, yt + 1.5, zt, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 270) {
			gluLookAt(xt, 2, zt - 2.0, xt, yt + 1.5, zt - 5.0, 0.0, 1.0, 0.0);
		}

		glPushMatrix();

	}

	//Look Left
	else if (f2 == true) {
		glLoadIdentity();
		if (rotateCarAngle == 0)
		{
			gluLookAt(xt, 2, zt, xt, yt + 1.5, zt + 4.0, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 90) {
			gluLookAt(xt, 2, zt, xt + 4.0, yt + 1.5, zt, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 180) {
			gluLookAt(xt, 2, zt, xt, yt + 1.5, zt - 4.0, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 270) {
			gluLookAt(xt, 2, zt, xt - 4.0, yt + 1.5, zt, 0.0, 1.0, 0.0);
		}

		glPushMatrix();

	}

	//TODO: Look Right
	else if (f3 == true) {
		glLoadIdentity();
		if (rotateCarAngle == 0)
		{
			gluLookAt(xt, 2, zt, xt, yt + 1.5, zt - 4.0, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 90) {
			gluLookAt(xt, 2, zt, xt - 4.0, yt + 1.5, zt, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 180) {
			gluLookAt(xt, 2, zt, xt, yt + 1.5, zt + 4.0, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 270) {
			gluLookAt(xt, 2, zt, xt + 4.0, yt + 1.5, zt, 0.0, 1.0, 0.0);
		}
		glPushMatrix();

	}

	// back (+x) and right (-z) of vehicle
	else if (f5 == true) {
		glLoadIdentity();
		gluLookAt(xt + 5.0, yt + 20, zt - 5.0, xt, yt, zt, 0, 1, 0);
		glPushMatrix();
	}
	// front (-x) and right (-z) of vehicle
	else if (f6 == true) {
		glLoadIdentity();
		gluLookAt(xt - 5.0, yt + 10.0, zt - 5.0, xt, yt, zt, 0, 1, 0);
		glPushMatrix();
	}
	// front (-x) and left (+z) of vehicle
	else if (f7 == true) {
		glLoadIdentity();
		gluLookAt(xt - 5.0, yt + 10.0, zt + 5.0, xt, yt, zt, 0, 1, 0);
		glPushMatrix();
	}
	// back (+x) and left (+z) of vehicle
	else if (f8 == true) {
		glLoadIdentity();
		gluLookAt(xt + 10.0, yt + 10.0, zt + 10.0, xt, yt, zt, 0, 1, 0);
		glPushMatrix();
	}
	// like F5, but farther away
	else if (f9 == true) {
		glLoadIdentity();
		gluLookAt(xt + 10.0, yt + 20.0, zt - 10.0, xt, yt, zt, 0, 1, 0);
		glPushMatrix();
	}
	// like F6, but farther away
	else if (f10 == true) {
		glLoadIdentity();
		gluLookAt(xt - 10.0, yt + 20.0, zt - 10.0, xt, yt, zt, 0, 1, 0);
		glPushMatrix();
	}
	// like F7, but farther away
	else if (f11 == true) {
		glLoadIdentity();
		gluLookAt(xt - 10.0, yt + 20.0, zt + 10.0, xt, yt, zt, 0, 1, 0);
		glPushMatrix();
	}
	// like F8, but farther away
	else if (f12 == true) {
		glLoadIdentity();
		gluLookAt(xt + 10.0, yt + 20.0, zt + 10.0, xt, yt, zt, 0, 1, 0);
		glPushMatrix();
	}

	//Default View
	else {
		glLoadIdentity();

		if (rotateCarAngle == 0)
		{
			gluLookAt(xt - 2.0, 2, zt, xt - 5.0, yt + 1.5, zt, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 90) {
			gluLookAt(xt, 2, zt + 2.0, xt, yt + 1.5, zt + 5.0, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 180) {
			gluLookAt(xt + 2.0, 2, zt, xt + 5.0, yt + 1.5, zt, 0.0, 1.0, 0.0);
		}
		else if (rotateCarAngle == 270) {
			gluLookAt(xt, 2, zt - 2.0, xt, yt + 1.5, zt - 5.0, 0.0, 1.0, 0.0);
		}

		glPushMatrix();
	}
}

void display() {
	if (!paused) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		InitGL(Window_Width, Window_Height);
		processLookAtView();
		generateCity();

		drawCar();
		drawGround();
		setText();

		glutSwapBuffers();
		rotateSphereBuildingTop += 1.0f;
		rotateCylinderBuildingTop += 1.5f;

		antRot += 3.0;
		if (antRot >= 360.0) {
			antRot -= 360.0;
		}
	}
}

void NormalKey(GLubyte key, GLint x, GLint y)
{
	switch (key)
	{
		//return car to origin
	case 'r':
		xt = zt = rotateCarAngle = 0;
		break;

	case 'q':
		if (paused) {
			break;
		}

		if (((int)fabs(xt) + 26) % fixedTurnInterval <= 2 && currentAxis == 1)
		{
			currentAxis = 2;
			rotateCarAngle += 90;

			if (rotateCarAngle >= 360) {
				rotateCarAngle -= 360;
			}
		}

		else if (((int)fabs(zt) + 26) % fixedTurnInterval <= 2 && currentAxis == 2)
		{
			currentAxis = 1;
			rotateCarAngle += 90;

			if (rotateCarAngle >= 360) {
				rotateCarAngle -= 360;
			}
		}

		break;

		//Turn right
	case 'w':
		if (paused) {
			break;

		}

		if (((int)fabs(xt) + 26) % fixedTurnInterval <= 2 && currentAxis == 1)
		{
			currentAxis = 2;
			rotateCarAngle -= 90;

			if (rotateCarAngle < 0) {
				rotateCarAngle += 360.0;
			}

			break;
		}

		if (((int)fabs(zt) + 26) % fixedTurnInterval <= 2 && currentAxis == 2)
		{
			currentAxis = 1;
			rotateCarAngle -= 90;

			if (rotateCarAngle < 0) {
				rotateCarAngle += 360.0;
			}
			break;
		}

		break;

		// move backwards along axis you are on
	case 'z':
		if (paused) {
			break;
		}

		angle += 5;

		if (rotateCarAngle == 0 && (xt + carSpeed < mapBoundary * 2)) {
			xt += carSpeed;
		}

		else if (rotateCarAngle == 90 && (zt - carSpeed > -mapBoundary * 2)) {
			zt -= carSpeed;
		}

		else if (rotateCarAngle == 180 && (xt - carSpeed > -mapBoundary * 2)) {
			xt -= carSpeed;
		}

		else if (rotateCarAngle == 270 && (zt + carSpeed < mapBoundary * 2)) {
			zt += carSpeed;
		}

		glutPostRedisplay();
		break;

		// move forwards along axis you are on
	case 'a':
		if (paused) {
			break;
		}

		angle += 5;
		if (rotateCarAngle == 0 && (xt - carSpeed > -mapBoundary * 2)) {
			xt -= carSpeed;
		}

		else if (rotateCarAngle == 90 && (zt + carSpeed < mapBoundary * 2)) {
			zt += carSpeed;
		}

		else if (rotateCarAngle == 180 && (xt + carSpeed < mapBoundary * 2)) {
			xt += carSpeed;
		}

		else if (rotateCarAngle == 270 && (zt - carSpeed > -mapBoundary * 2)) {
			zt -= carSpeed;
		}

		glutPostRedisplay();
		break;

	case 'p':
		paused = !paused;
		break;

	default:
		break;
	}
}

void mySpecialFunc(int key, int x, int y) {
	switch (key) {
		// implement f1, f2, f3 later
	case GLUT_KEY_F1:
		f1 = true;
		f2 = f3 = f4 = f5 = f6 = f7 = f8 = f9 = f10 = f11 = f12 = false;
		break;

	case GLUT_KEY_F2:
		f2 = true;
		f1 = f3 = f4 = f5 = f6 = f7 = f8 = f9 = f10 = f11 = f12 = false;
		break;

	case GLUT_KEY_F3:
		f3 = true;
		f1 = f2 = f4 = f5 = f6 = f7 = f8 = f9 = f10 = f11 = f12 = false;
		break;

	case GLUT_KEY_F4:
		f4 = true;
		f1 = f2 = f3 = f5 = f6 = f7 = f8 = f9 = f10 = f11 = f12 = false;
		break;

		// back (+x) and right (-z) of vehicle
	case GLUT_KEY_F5:
		f5 = true;
		f1 = f2 = f3 = f4 = f6 = f7 = f8 = f9 = f10 = f11 = f12 = false;
		break;

		// front (-x) and right (-z) of vehicle
	case GLUT_KEY_F6:
		f6 = true;
		f1 = f2 = f3 = f4 = f5 = f7 = f8 = f9 = f10 = f11 = f12 = false;
		break;

		// front (-x) and left (+z) of vehicle
	case GLUT_KEY_F7:
		f7 = true;
		f1 = f2 = f3 = f4 = f5 = f6 = f8 = f9 = f10 = f11 = f12 = false;
		break;

		// back (+x) and left (+z) of vehicle
	case GLUT_KEY_F8:
		f8 = true;
		f1 = f2 = f3 = f4 = f5 = f6 = f7 = f9 = f10 = f11 = f12 = false;
		break;

		// like F5, but farther away
	case GLUT_KEY_F9:
		f9 = true;
		f1 = f2 = f3 = f4 = f5 = f6 = f7 = f8 = f10 = f11 = f12 = false;
		break;

		// like F6, but farther away
	case GLUT_KEY_F10:
		f10 = true;
		f1 = f2 = f3 = f4 = f5 = f6 = f7 = f8 = f9 = f11 = f12 = false;
		break;

		// like F7, but farther away
	case GLUT_KEY_F11:
		f11 = true;
		f1 = f2 = f3 = f4 = f5 = f6 = f7 = f8 = f9 = f10 = f12 = false;
		break;

		// like F8, but farther away
	case GLUT_KEY_F12:
		f12 = true;
		f1 = f2 = f3 = f4 = f5 = f6 = f7 = f8 = f9 = f10 = f11 = false;
		break;

	default:
		break;
	}
}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);				    	// Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE);		    	// Enable double buffered mode
	glutInitWindowSize(Window_Width, Window_Height);    	// Set the window's initial width & height
	glutInitWindowPosition(50, 50);				// Position the window's initial top-left corner
	glutCreateWindow(PROGRAM_TITLE);			// Create window with the given title
	glutDisplayFunc(display);				// Register callback handler for window re-paint event
	glutIdleFunc(display);					// If nothing to do, refresh screen
	glutReshapeFunc(reshape);				// Register callback handler for window re-size event
	glutKeyboardFunc(NormalKey);				// Register callback for normal key events
	glutSpecialFunc(mySpecialFunc);				// Register callback for special key events

	glEnable(GL_DEPTH_TEST);				// Enable depth testing
	glutMainLoop();						// Enter the infinite event-processing loop
	return 0;
}