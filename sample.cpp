#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include <iostream>



//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			PREET TIBREWALA

// title of these windows:

const char* WINDOWTITLE = { "PREET TIBREWALA FINAL PROJECT- SOLAR SYSTEM" };
const char* GLUITITLE = { "User Interface Window" };

// what the glui package defines as true and false:

const int GLUITRUE = { true };
const int GLUIFALSE = { false };

// the escape key:

const int ESCAPE = { 0x1b };

// initial window size:

const int INIT_WINDOW_SIZE = { 1080 };
const int window_width = 1920;
const int window_height = 1080;


// size of the 3d box:

const float BOXSIZE = { 10.f };

// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };

// minimum allowable scale factor:

const float MINSCALE = { 0.05f };

// scroll wheel button values:

const int SCROLL_WHEEL_UP = { 3 };
const int SCROLL_WHEEL_DOWN = { 4 };

// equivalent mouse movement when we click a the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = { 5. };

// active mouse buttons (or them together):

const int LEFT = { 4 };
const int MIDDLE = { 2 };
const int RIGHT = { 1 };

unsigned char* TextureArray0, * TextureArray1, * TextureArray2, * TextureArray3, * TextureArray4, * TextureArray5, * TextureArray6, * TextureArray7, * TextureArray8, * TextureArray9; // global variables


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};

// which button:

enum ButtonVals
{
	RESET,
	QUIT
};

// window background color (rgba):

const GLfloat BACKCOLOR[] = { 0., 0., 0., 1. };

// line width for the axes:

const GLfloat AXES_WIDTH = { 3. };

// the color numbers:
// this order must match the radio button order

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};

char* ColorNames[] =
{
	(char*)"Red",
	(char*)"Yellow",
	(char*)"Green",
	(char*)"Cyan",
	(char*)"Blue",
	(char*)"Magenta",
	(char*)"White",
	(char*)"Black"
};

// the color definitions:
// this order must match the menu order

const GLfloat Colors[][3] =
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};

// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE = { GL_LINEAR };
const GLfloat FOGDENSITY = { 0.30f };
const GLfloat FOGSTART = { 1.5 };
const GLfloat FOGEND = { 4. };


// what options should we compile-in?
// in general, you don't need to worry about these
// i compile these in to show class examples of things going wrong
//#define DEMO_Z_FIGHTING
//#define DEMO_DEPTH_BUFFER

// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
GLuint	sun,mercury,venus,earth,mars,jupiter,saturn,uranus,neptune,starslist,asteroidbelt,orbits;				// object display list
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to force the creation of z-fighting
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
float	Time;					// timer in the range [0.,1.)
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
unsigned char* texture;			// for texture
int		Longitudes, Latitudes;
boolean	pause = false;
struct point* Pts;
int width = 1024, height = 512;
GLuint Tex0, Tex1, Tex2, Tex3, Tex4, Tex5, Tex6, Tex7, Tex8, Tex9;					// texture building
int level = 0, ncomps = 3, border = 0;
bool Distort, textureON;					// Enable or Disable Distort
float theta;					// angle
float ms;
float m = 0.0f, prev = -1.0f;
char* option = "Texture On";


// function prototypes:

void	Animate();
void	Display();
void	DoAxesMenu(int);
void	DoColorMenu(int);
void	DoDepthBufferMenu(int);
void	DoDepthFightingMenu(int);
void	DoDepthMenu(int);
void	DoDebugMenu(int);
void	DoMainMenu(int);
void	DoProjectMenu(int);
void	DoShadowMenu();
void	DoRasterString(float, float, float, char*);
void	DoStrokeString(float, float, float, float, char*);
float	ElapsedSeconds();
void	InitGraphics();
void	InitLists();
void	InitMenus();
void	Keyboard(unsigned char, int, int);
void	MouseButton(int, int, int, int);
void	MouseMotion(int, int);
void	Reset();
void	Resize(int, int);
void	Visibility(int);
void	SetPointLight(int, float, float, float, float, float, float);
float* Array3(float, float, float);
void			Axes(float);
unsigned char* BmpToTexture(char*, int*, int*);
void			HsvRgb(float[3], float[3]);
int				ReadInt(FILE*);
short			ReadShort(FILE*);

void			Cross(float[3], float[3], float[3]);
float			Dot(float[3], float[3]);
float			Unit(float[3], float[3]);
void	OsuSphere(float, int, int);
//////////////////////////////////////change for animation
float angleMoon = 0.0, angleEarth = 0.0, angleAstroid = 0.0,
angleMars = 0.0,
angleMercury = 0.0,
angleVenus = 0.0,
angleJupiter = 0.0,
angleSaturn = 0.0,
angleUranus = 30.0,
angleNeptune = 60.0;
GLfloat sx = 0.2, sy = 0.2, sz = 0.2;
/////////////////////////////////////////
int random(int m)
{
	return rand() % m;
}

void saturnrings(float cx, float cy, float r, int num_segments)
{
	float theta = 3.1415926 * 2 / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor 

	float radial_factor = cosf(theta);//calculate the radial factor 

	float x = r;//we start at angle = 0 

	float y = 0;
	//glLineWidth(2);
	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3f(.2, .2, .2);
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex3f(x + cx, 0.0, y + cy);//output vertex 

		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 

		float tx = -y;
		float ty = x;

		//add the tangential vector 

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		//correct using the radial factor 

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}

void drawCircle(float cx, float cy, float r, int num_segments)
{
	float theta = 3.1415926 * 2 / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor 

	float radial_factor = cosf(theta);//calculate the radial factor 

	float x = r;//we start at angle = 0 

	float y = 0;
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 1.0);
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex3f(x + cx, 0.0, y + cy);//output vertex 

		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 

		float tx = -y;
		float ty = x;

		//add the tangential vector 

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		//correct using the radial factor 

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}
void asteroid(float cx, float cy, float r, int num_segments)
{
	float theta = 3.1415926 * 2 / float(num_segments);
	float tangetial_factor = tanf(theta); 

	float radial_factor = cosf(theta);

	float x = r;

	float y = 0;
	//glLineWidth(2);
	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3f(.2, .2, .2);
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex3f(x + cx, 0.0, y + cy);//output vertex 

		float tx = -y;
		float ty = x;
		x += tx * tangetial_factor;
		y += ty * tangetial_factor;
		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();

}

void stars(float cx, float cy, float r, int num_segments)
{
	float theta = 3.1415926 * 2 / float(num_segments);
	float tangetial_factor = sinf(theta);

	float radial_factor = cosf(theta);

	float x = r;

	float y = 0;
	//glLineWidth(2);
	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3f(1., 1., 1.);
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex3f(x + cx, 0.0, y + cy);//output vertex 

		float tx = -y;
		float ty = x;
		x += tx * tangetial_factor;
		y += ty * tangetial_factor;
		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();

}


// main program:

int
main(int argc, char* argv[])
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// setup all the graphics stuff:
	glPushMatrix();
	glPointSize(1.5);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < 500; i++)
	{
		glVertex3f(random(700), random(700), random(700));
	}

	glEnd;
	glPopMatrix();

	InitGraphics();

	// init all the global variables used by Display( ):

	Reset();

	// create the display structures that will not change:

	InitLists();

	// setup all the user interface stuff:

	InitMenus();

	// draw the scene once and wait for some interaction:
	// (this will never return)
	glutIdleFunc(Animate);
	glutMainLoop();

	// glutMainLoop( ) never returns
	// this line is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate()
{
#define MS_IN_THE_ANIMATION_CYCLE	200000
	int ms = glutGet(GLUT_ELAPSED_TIME);	// milliseconds
	ms %= MS_IN_THE_ANIMATION_CYCLE;
	Time = (float)ms / (float)MS_IN_THE_ANIMATION_CYCLE;



	
	angleMercury += 2.4;
	if (angleMercury > 360) {
		angleMercury -= 360;
	}
	angleVenus += 1.75;
	if (angleVenus > 360) {
		angleVenus -= 360;
	}
	angleEarth += 1.49;
	if (angleEarth > 360) {
		angleEarth -= 360;
	}
	angleMars += 1.2;
	if (angleMars > 360) {
		angleMars -= 360;
	}
	angleJupiter += 0.65;
	if (angleJupiter > 360) {
		angleJupiter -= 360;
	}
	angleSaturn += 0.48;
	if (angleSaturn > 360) {
		angleSaturn -= 360;
	}


	angleUranus += 0.34;
	if (angleUranus > 360) {
		angleUranus -= 360;
	}


	angleNeptune += 0.27;
	if (angleNeptune > 360) {
		angleNeptune -= 360;
	}

	
	glutPostRedisplay();

}

void
SetPointLight(int ilight, float x, float y, float z, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));
	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);
}


// draw the complete scene:

void
Display()
{
	if (DebugOn != 0)
	{
		fprintf(stderr, "Display\n");
	}

	// set which window we want to do the graphics into:

	glutSetWindow(MainWindow);

	// erase the background:

	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
#ifdef DEMO_DEPTH_BUFFER
	if (DepthBufferOn == 0)
		glDisable(GL_DEPTH_TEST);
#endif

	// specify shading to be flat:

	glShadeModel(GL_FLAT);

	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = (vx - v) / 2;
	GLint yb = (vy - v) / 2;
	glViewport(xl, yb, v, v);

	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (WhichProjection == ORTHO)
		glOrtho(-3., 3., -3., 3., 0.1, 1000.);
	else
		gluPerspective(90., 1., 0.1, 1000.);

	// place the objects into the scene:

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set the eye position, look-at position, and up-vector:

	gluLookAt(5., 5., 10., 4., 0., -30., 0., 1., 0.);

	// rotate the scene:

	glRotatef((GLfloat)Yrot, 0., 1., 0.);
	glRotatef((GLfloat)Xrot, 1., 0., 0.);

	// uniformly scale the scene:

	if (Scale < MINSCALE)
		Scale = MINSCALE;
	glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);

	// set the fog parameters:
	// (this is really here to do intensity depth cueing)

	if (DepthCueOn != 0)
	{
		glFogi(GL_FOG_MODE, FOGMODE);
		glFogfv(GL_FOG_COLOR, FOGCOLOR);
		glFogf(GL_FOG_DENSITY, FOGDENSITY);
		glFogf(GL_FOG_START, FOGSTART);
		glFogf(GL_FOG_END, FOGEND);
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}

	// possibly draw the axes:

	if (AxesOn != 0)
	{
		glColor3fv(&Colors[WhichColor][0]);
		glCallList(AxesList);
	}

	

	glCallList(orbits);
	glCallList(starslist);
	glCallList(asteroidbelt);
	
	
	glCallList(sun);

	
	glPushMatrix();
	glRotatef(angleMercury, 0.0, 1.0, 0.0);
	glCallList(mercury);
	glPopMatrix();

	
	glPushMatrix();
	glRotatef(angleVenus, 0.0, 1.0, 0.0);
	glCallList(venus);
	glPopMatrix();
	
	
	
	glPushMatrix();
	glRotatef(angleEarth, 0.0, 1.0, 0.0);
	glCallList(earth);
	glPopMatrix();
	
	

	glPushMatrix();
	glRotatef(angleMars, 0.0, 1.0, 0.0);
	glCallList(mars);
	glPopMatrix();
	
	
	glPushMatrix();
	glRotatef(angleJupiter, 0.0, 1.0, 0.0);
	glCallList(jupiter);
	glPopMatrix();
	
	
	glPushMatrix();
	glRotatef(angleSaturn, 0.0, 1.0, 0.0);
	glCallList(saturn);
	glPopMatrix();
	

	glPushMatrix();
	glRotatef(angleUranus, 0.0, 1.0, 0.0);
	glCallList(uranus);
	glPopMatrix();
	
	
	glPushMatrix();
	glRotatef(angleNeptune, 0.0, 1.0, 0.0);
	glCallList(neptune);
	glPopMatrix();

	////////////////////////////////////////////////////light points
	//SetPointLight(GL_LIGHT1, 10., 10., 10., 1., 1., 1.0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHTING);
	//OsuSphere(0.1, 20, 20);
	
	/*
	for (j = 0; j < 15; j++)
	{
		
		for (i = 0; i < 4; i++)
		{

			glPushMatrix();
			glRotatef(x, 1, 0, 0);
			glColor3f(1.0, 1.0, 1.0);
			glTranslatef(25, -10, -10);
			stars(0, 75, 0, r);
			glPopMatrix();
			r = r + 5;

		}
		x = x + 15;

	}
	y = 0;
	for (j = 0; j < 15; j++)
	{

		for (i = 0; i < 4; i++)
		{

			glPushMatrix();
			glRotatef(y, 1, 0, 0);
			glColor3f(1.0, 1.0, 1.0);
			glTranslatef(0, 0, -5);
			stars(0, 20, 0, r);
			glPopMatrix();
			r = r + 5;

		}
		x = x + 25;

	}
	

	*/

	SetPointLight(GL_LIGHT2, 0, 0, 0, 1., 1., 1.);
	glEnable(GL_LIGHT2);



	






#ifdef DEMO_Z_FIGHTING
	if (DepthFightingOn != 0)
	{
		glPushMatrix();
		glRotatef(90., 0., 1., 0.);
		glCallList(BoxList);
		glPopMatrix();
	}
#endif

	// draw some gratuitous text that just rotates on top of the scene:

	//glDisable( GL_DEPTH_TEST );
	//glColor3f( 0., 1., 1. );
	//DoRasterString( 0., 1., 0., (char *)"Text That Moves" );

	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., 100., 0., 100.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1., 1., 1.);
	DoRasterString(5., 5., 0., (char*)"SOLAR SYSTEM");

	// swap the double-buffered framebuffers:

	glutSwapBuffers();

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush();
}


void
DoAxesMenu(int id)
{
	AxesOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


void
DoColorMenu(int id)
{
	WhichColor = id - RED;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


void
DoDebugMenu(int id)
{
	DebugOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


void
DoDepthBufferMenu(int id)
{
	DepthBufferOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


void
DoDepthFightingMenu(int id)
{
	DepthFightingOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


void
DoDepthMenu(int id)
{
	DepthCueOn = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


// main menu callback:

void
DoMainMenu(int id)
{
	switch (id)
	{
	case RESET:
		Reset();
		break;

	case QUIT:
		// gracefully close out the graphics:
		// gracefully close the graphics window:
		// gracefully exit the program:
		glutSetWindow(MainWindow);
		glFinish();
		glutDestroyWindow(MainWindow);
		exit(0);
		break;

	default:
		fprintf(stderr, "Don't know what to do with Main Menu ID %d\n", id);
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


void
DoProjectMenu(int id)
{
	WhichProjection = id;
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


// use glut to display a string of characters using a raster font:

void
DoRasterString(float x, float y, float z, char* s)
{
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
	char c;			// one character to print
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString(float x, float y, float z, float ht, char* s)
{
	glPushMatrix();
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
	float sf = ht / (119.05f + 33.33f);
	glScalef((GLfloat)sf, (GLfloat)sf, (GLfloat)sf);
	char c;			// one character to print
	for (; (c = *s) != '\0'; s++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
	}
	glPopMatrix();
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds()
{
	// get # of milliseconds since the start of the program:
	int ms = glutGet(GLUT_ELAPSED_TIME);

	// convert it to seconds:
	return (float)ms / 1000.f;
}


void textureAndDistort(int t) {
	if (t == 0) {
		textureON = !textureON;
		option = textureON ? "Texture Off" : "Texture On";
		InitMenus();
	}
	else if (t == 1) {
		Distort = false;
		glutIdleFunc(NULL);
	}
	else {
		Distort = true;
		glutIdleFunc(Animate);
	}
}

// initialize the glui window:

void
InitMenus()
{
	glutSetWindow(MainWindow);

	int numColors = sizeof(Colors) / (3 * sizeof(int));
	int colormenu = glutCreateMenu(DoColorMenu);
	for (int i = 0; i < numColors; i++)
	{
		glutAddMenuEntry(ColorNames[i], i);
	}

	int axesmenu = glutCreateMenu(DoAxesMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int depthcuemenu = glutCreateMenu(DoDepthMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int textureAndD = glutCreateMenu(textureAndDistort);
	glutAddMenuEntry(option, 0);
	glutAddMenuEntry("Distort Off", 1);
	glutAddMenuEntry("Distort On", 2);

	int depthbuffermenu = glutCreateMenu(DoDepthBufferMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int depthfightingmenu = glutCreateMenu(DoDepthFightingMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int debugmenu = glutCreateMenu(DoDebugMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int projmenu = glutCreateMenu(DoProjectMenu);
	glutAddMenuEntry("Orthographic", ORTHO);
	glutAddMenuEntry("Perspective", PERSP);

	int mainmenu = glutCreateMenu(DoMainMenu);
	glutAddSubMenu("Axes", axesmenu);
	glutAddSubMenu("Colors", colormenu);

#ifdef DEMO_DEPTH_BUFFER
	glutAddSubMenu("Depth Buffer", depthbuffermenu);
#endif

#ifdef DEMO_Z_FIGHTING
	glutAddSubMenu("Depth Fighting", depthfightingmenu);
#endif

	glutAddSubMenu("Depth Cue", depthcuemenu);
	glutAddSubMenu("Projection", projmenu);
	glutAddSubMenu("Texture and Distort", textureAndD);
	glutAddMenuEntry("Reset", RESET);
	glutAddSubMenu("Debug", debugmenu);
	glutAddMenuEntry("Quit", QUIT);

	// attach the pop-up menu to the right mouse button:

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}





// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics()
{
	int width0, height0, width1, height1, width2, height2, width3, height3, width4, height4, width5, height5, width6, height6, width7, height7, width8, height8, width9, height9;

	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// set the initial window configuration:

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_height);

	// open the window and set its title:

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);

	// set the framebuffer clear values:
	fprintf(stderr, "\n2k_sun.bmp:\n");
	fprintf(stderr, "\n2k_mercury.bmp:\n");
	fprintf(stderr, "\n2k_venus_surface.bmp:\n");
	fprintf(stderr, "\n2k_earth_daymap.bmp:\n");
	fprintf(stderr, "\n2k_mars.bmp:\n");
	fprintf(stderr, "\n2k_jupiter.bmp:\n");
	fprintf(stderr, "\n2k_saturn.bmp:\n");
	fprintf(stderr, "\n2k_uranus.bmp:\n");
	fprintf(stderr, "\n2k_neptune.bmp:\n");
	fprintf(stderr, "\n2k_moon.bmp:\n");
	//unsigned char* t08 = BmpToTexture((char*)"24701-nature-natural-beauty.bmp", &width, &height);

	TextureArray0 = BmpToTexture((char*)"2k_sun.bmp", &width0, &height0);
	TextureArray1 = BmpToTexture((char*)"2k_mercury.bmp", &width1, &height1);
	TextureArray2 = BmpToTexture((char*)"2k_venus_surface.bmp", &width2, &height2);
	TextureArray3 = BmpToTexture((char*)"2k_earth_daymap.bmp", &width3, &height3);
	TextureArray4 = BmpToTexture((char*)"2k_mars.bmp", &width4, &height4);
	TextureArray5 = BmpToTexture((char*)"2k_jupiter.bmp", &width5, &height5);
	TextureArray6 = BmpToTexture((char*)"2k_saturn.bmp", &width6, &height6);
	TextureArray7 = BmpToTexture((char*)"2k_uranus.bmp", &width7, &height7);
	TextureArray8 = BmpToTexture((char*)"2k_neptune.bmp", &width8, &height8);
	TextureArray9 = BmpToTexture((char*)"2k_neptune.bmp", &width9, &height9);

	glGenTextures(1, &Tex0);
	glGenTextures(1, &Tex1);
	glGenTextures(1, &Tex2);
	glGenTextures(1, &Tex3);
	glGenTextures(1, &Tex4);
	glGenTextures(1, &Tex5);
	glGenTextures(1, &Tex6);
	glGenTextures(1, &Tex7);
	glGenTextures(1, &Tex8);
	glGenTextures(1, &Tex9);

	//SUN/////////////
	glBindTexture(GL_TEXTURE_2D, Tex0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width0, height0, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray0);

	//Mercury///////////////
	glBindTexture(GL_TEXTURE_2D, Tex1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width1, height1, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray1);

	//Venus///////////////////////

	glBindTexture(GL_TEXTURE_2D, Tex2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width2, height2, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray2);

	//earth/////////////////////////

	glBindTexture(GL_TEXTURE_2D, Tex3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width3, height3, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray3);

	//mars/////////////////////////
	glBindTexture(GL_TEXTURE_2D, Tex4);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width4, height4, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray4);
	//jupiter//////////////////////
	glBindTexture(GL_TEXTURE_2D, Tex5);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width5, height5, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray5);
	//saturn///////////////////////
	glBindTexture(GL_TEXTURE_2D, Tex6);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width6, height6, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray6);
	//uranus//////////////////////
	glBindTexture(GL_TEXTURE_2D, Tex7);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width7, height7, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray7);
	//neptune//////////////////////
	glBindTexture(GL_TEXTURE_2D, Tex8);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width8, height8, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray8);

	/////////////////////////////////////////

	/// <moon>
	glBindTexture(GL_TEXTURE_2D, Tex9);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width9, height9, border, GL_RGB, GL_UNSIGNED_BYTE, TextureArray9);
	/// </moon>
	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);

	


	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow(MainWindow);
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MouseMotion);
	//glutPassiveMotionFunc( NULL );
	glutVisibilityFunc(Visibility);
	glutEntryFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpaceballMotionFunc(NULL);
	glutSpaceballRotateFunc(NULL);
	glutSpaceballButtonFunc(NULL);
	glutButtonBoxFunc(NULL);
	glutDialsFunc(NULL);
	glutTabletMotionFunc(NULL);
	glutTabletButtonFunc(NULL);
	glutMenuStateFunc(NULL);
	glutTimerFunc(-1, NULL, 0);
	glutIdleFunc(NULL);

	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "glewInit Error\n");
	}
	else
		fprintf(stderr, "GLEW initialized OK\n");
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists()
{
	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow(MainWindow);

	// create the object:

	//orbits
	orbits = glGenLists(1);
	glNewList(orbits, GL_COMPILE);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	drawCircle(0.0, 0.0, 6, 100);//merc
	drawCircle(0.0, 0.0, 7.0, 100);//venus
	drawCircle(0.0, 0.0, 9.5, 100);//earth
	drawCircle(0.0, 0.0, 13.25, 100);//mars
	drawCircle(0.0, 0.0, 20.0, 100);//jupiter
	drawCircle(0.0, 0.0, 28.25, 100);//saturn
	drawCircle(0.0, 0.0, 38.25, 100);//uranus
	drawCircle(0.0, 0.0, 49.25, 100);//neptune
	glPopMatrix();
	glEndList();

	//for asteroid belt
	asteroidbelt = glGenLists(1);
	glNewList(asteroidbelt, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_NORMALIZE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	asteroid(0.0, 0.0, 15., 175);
	asteroid(0.0, 0.0, 15.2, 193);
	asteroid(0.0, 0.0, 15.4, 180);
	asteroid(0.0, 0.0, 15.5, 155);
	asteroid(0.0, 0.0, 15.6, 185);
	asteroid(0.0, 0.0, 15.75, 165);
	asteroid(0.0, 0.0, 15.9, 190);
	asteroid(0.0, 0.0, 16., 170);
	asteroid(0.0, 0.0, 16.2, 187);
	asteroid(0.0, 0.0, 16.4, 190);
	asteroid(0.0, 0.0, 16.5, 150);
	asteroid(0.0, 0.0, 16.6, 200);
	asteroid(0.0, 0.0, 16.75, 185);
	asteroid(0.0, 0.0, 16.9, 210);
	asteroid(0.0, 0.0, 16., 190);
	asteroid(0.0, 0.0, 17.2, 200);
	asteroid(0.0, 0.0, 17.4, 130);
	asteroid(0.0, 0.0, 17.5, 170);
	asteroid(0.0, 0.0, 17.6, 240);
	asteroid(0.0, 0.0, 17.75, 255);
	asteroid(0.0, 0.0, 17.9, 285);
	glPopMatrix();
	glEndList();
	//for Sun
	sun = glGenLists(1);
	glNewList(sun, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_NORMALIZE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex0);
	OsuSphere(5.5, 34, 45);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEndList();
	
	//for merc
	mercury = glGenLists(1);
	glNewList(mercury, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex1);	
	glTranslatef(6, 0.0, 0.0);
	OsuSphere(0.3, 34, 45);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	glEndList();

	// venus
	venus = glGenLists(1);
	glNewList(venus, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex2);
	glTranslatef(7.0, 0.0, 0.0);
	OsuSphere(0.5, 34, 45);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	glEndList();

	//earth
	earth = glGenLists(1);
	glNewList(earth, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex3);
	glTranslatef(9.5, 0.0, 0.0);
	OsuSphere(0.75, 34, 45);
	glPushMatrix();
	drawCircle(0., 0., 0.95, 100);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	glEndList();
	//mars
	mars = glGenLists(1);
	glNewList(mars, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex4);
	glTranslatef(13.25, 0.0, 0.0);
	OsuSphere(0.8, 34, 45);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	glEndList();
	//jupiter
	jupiter = glGenLists(1);
	glNewList(jupiter, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex5);
	glTranslatef(20.0, 0.0, 0.0);
	OsuSphere(1.45, 34, 45);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	glEndList();
	//saturn
	saturn = glGenLists(1);
	glNewList(saturn, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex6);
	glTranslatef(28.25, 0.0, 0.0);
	OsuSphere(0.95, 34, 45);
	glPushMatrix();
	glRotatef(-35, -1.0, 0.0, 0.0);
	saturnrings(0.0, 0.0, 1.25, 200);
	saturnrings(0.0, 0.0, 1.35, 170);
	saturnrings(0.0, 0.0, 1.45, 160);
	saturnrings(0.0, 0.0, 1.55, 180);
	saturnrings(0.0, 0.0, 1.65, 250);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	glEndList();
	//uranus
	uranus = glGenLists(1);
	glNewList(uranus, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex7);
	glTranslatef(38.25, 0.0, 0.0);
	OsuSphere(0.7, 34, 45);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	glEndList();
	//neptune
	neptune = glGenLists(1);
	glNewList(neptune, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Tex8);
	glTranslatef(49.25, 0.0, 0.0);
	OsuSphere(0.65, 34, 45);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	glEndList();

	starslist = glGenLists(1);
	glNewList(starslist, GL_COMPILE);
	glPushMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(5., 5., 5.);
	stars(0, 0, 25, 10);
	glPopMatrix();


	glPushMatrix();
	glRotatef(45, 0, 0, 0);
	glTranslatef(-5., 5., 15.);
	stars(0, 0, 45, 20);
	glPopMatrix();

	glPushMatrix();
	glRotatef(45, 1, 0, 0);
	glTranslatef(-5., 5., 15.);
	stars(0, 0, 45, 20);
	glPopMatrix();
	glPushMatrix();
	glRotatef(145, 1, 0, 0);
	glTranslatef(-5., 5., -3.);
	stars(0, 0, 30, 20);
	glPopMatrix();
	glPushMatrix();
	glRotatef(32, 1, 0, 0);
	glTranslatef(5., 5., 5.);
	stars(0, 0, 55, 30);
	glPopMatrix();

	float j, r = 10;
	for (j = 0; j < 8; j++)
	{
		glRotatef(45, 1, 0, 0);

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(25, -10, -10);
		stars(0, 0, 15, r);
		glPopMatrix();
		r = r + 2;

	}
	for (j = 0; j < 10; j++)
	{
		glRotatef(20, 1, 0, 0);

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-10, -10, -10);
		stars(0, 0, 15, 10);
		glPopMatrix();
		//r = r + 2;

	}
	for (j = 0; j < 10; j++)
	{
		glRotatef(30, 1, 0, 0);

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(10, -25, 5);
		stars(0, 0, 25, 20);
		glPopMatrix();
		//r = r + 2;

	}
	for (j = 0; j < 10; j++)
	{
		glRotatef(30, 1, 0, 0);

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-25, -25, 15);
		stars(0, 0, 25, 20);
		glPopMatrix();
		//r = r + 2;

	}

	

	glPopMatrix();
	glEndList();
	// create the axes:

	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(AXES_WIDTH);
	Axes(10.5);
	glLineWidth(1.);
	glEndList();
}


// the keyboard callback:

void
Keyboard(unsigned char c, int x, int y)
{
	if (DebugOn != 0)
		fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

	switch (c)
	{
	case 'o':
	case 'O':
		WhichProjection = ORTHO;
		break;

	case 'p':
	case 'P':
		WhichProjection = PERSP;
		break;
	case 'f':
		if (pause)
			glutIdleFunc(Animate);
		else
			glutIdleFunc(NULL);
		pause = !pause;
		break;
	case 't':
	case 'T':
		textureON = !textureON;
		break;
	case 'd':
	case 'D':
		Distort = !Distort;
		if (Distort && textureON)
			glutIdleFunc(Animate);
		else {
			glutIdleFunc(NULL);
		}
		break;
	case 'q':
	case 'Q':
	case ESCAPE:
		DoMainMenu(QUIT);	// will not return here
		break;				// happy compiler

	default:
		fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c);
	}

	// force a call to Display( ):

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


// called when the mouse button transitions down or up:

void
MouseButton(int button, int state, int x, int y)
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if (DebugOn != 0)
		fprintf(stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y);

	// get the proper button bit mask:

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		b = LEFT;		break;

	case GLUT_MIDDLE_BUTTON:
		b = MIDDLE;		break;

	case GLUT_RIGHT_BUTTON:
		b = RIGHT;		break;

	case SCROLL_WHEEL_UP:
		Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
		// keep object from turning inside-out or disappearing:
		if (Scale < MINSCALE)
			Scale = MINSCALE;
		break;

	case SCROLL_WHEEL_DOWN:
		Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
		// keep object from turning inside-out or disappearing:
		if (Scale < MINSCALE)
			Scale = MINSCALE;
		break;

	default:
		b = 0;
		fprintf(stderr, "Unknown mouse button: %d\n", button);
	}

	// button down sets the bit, up clears the bit:

	if (state == GLUT_DOWN)
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();

}


// called when the mouse moves while a button is down:

void
MouseMotion(int x, int y)
{
	if (DebugOn != 0)
		fprintf(stderr, "MouseMotion: %d, %d\n", x, y);

	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if ((ActiveButton & LEFT) != 0)
	{
		Xrot += (ANGFACT * dy);
		Yrot += (ANGFACT * dx);
	}

	if ((ActiveButton & MIDDLE) != 0)
	{
		Scale += SCLFACT * (float)(dx - dy);

		// keep object from turning inside-out or disappearing:

		if (Scale < MINSCALE)
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset()
{
	ActiveButton = 0;
	AxesOn = 0;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize(int width, int height)
{
	if (DebugOn != 0)
		fprintf(stderr, "ReSize: %d, %d\n", width, height);

	// don't really need to do anything since the window size is
	// checked each time in Display( ):

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


// handle a change to the window's visibility:

void
Visibility(int state)
{
	if (DebugOn != 0)
		fprintf(stderr, "Visibility: %d\n", state);

	if (state == GLUT_VISIBLE)
	{
		glutSetWindow(MainWindow);
		glutPostRedisplay();
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[] = {
		0.f, 1.f, 0.f, 1.f
};

static float xy[] = {
		-.5f, .5f, .5f, -.5f
};

static int xorder[] = {
		1, 2, -3, 4
};

static float yx[] = {
		0.f, 0.f, -.5f, .5f
};

static float yy[] = {
		0.f, .6f, 1.f, 1.f
};

static int yorder[] = {
		1, 2, 3, -2, 4
};

static float zx[] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
};

static float zy[] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
};

static int zorder[] = {
		1, 2, 3, 4, -5, 6
};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes(float length)
{
	glBegin(GL_LINE_STRIP);
	glVertex3f(length, 0., 0.);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., length, 0.);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., length);
	glEnd();

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; i++)
	{
		int j = xorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(base + fact * xx[j], fact * xy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 5; i++)
	{
		int j = yorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(fact * yx[j], base + fact * yy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 6; i++)
	{
		int j = zorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(0.0, fact * zy[j], base + fact * zx[j]);
	}
	glEnd();

}

// read a BMP file into a Texture:

#define VERBOSE		false
#define BMP_MAGIC_NUMBER	0x4d42
#ifndef BI_RGB
#define BI_RGB			0
#define BI_RLE8			1
#define BI_RLE4			2
#endif


// bmp file header:
struct bmfh
{
	short bfType;		// BMP_MAGIC_NUMBER = "BM"
	int bfSize;		// size of this file in bytes
	short bfReserved1;
	short bfReserved2;
	int bfOffBytes;		// # bytes to get to the start of the per-pixel data
} FileHeader;

// bmp info header:
struct bmih
{
	int biSize;		// info header size, should be 40
	int biWidth;		// image width
	int biHeight;		// image height
	short biPlanes;		// #color planes, should be 1
	short biBitCount;	// #bits/pixel, should be 1, 4, 8, 16, 24, 32
	int biCompression;	// BI_RGB, BI_RLE4, BI_RLE8
	int biSizeImage;
	int biXPixelsPerMeter;
	int biYPixelsPerMeter;
	int biClrUsed;		// # colors in the palette
	int biClrImportant;
} InfoHeader;

// read a BMP file into a Texture:

unsigned char*
BmpToTexture(char* filename, int* width, int* height)
{
	FILE* fp;
#ifdef _WIN32
	errno_t err = fopen_s(&fp, filename, "rb");
	if (err != 0)
	{
		fprintf(stderr, "Cannot open Bmp file '%s'\n", filename);
		return NULL;
	}
#else
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		fprintf(stderr, "Cannot open Bmp file '%s'\n", filename);
		return NULL;
	}
#endif

	FileHeader.bfType = ReadShort(fp);

	// if bfType is not BMP_MAGIC_NUMBER, the file is not a bmp:

	if (VERBOSE) fprintf(stderr, "FileHeader.bfType = 0x%0x = \"%c%c\"\n",
		FileHeader.bfType, FileHeader.bfType & 0xff, (FileHeader.bfType >> 8) & 0xff);
	if (FileHeader.bfType != BMP_MAGIC_NUMBER)
	{
		fprintf(stderr, "Wrong type of file: 0x%0x\n", FileHeader.bfType);
		fclose(fp);
		return NULL;
	}

	FileHeader.bfSize = ReadInt(fp);
	if (VERBOSE)	fprintf(stderr, "FileHeader.bfSize = %d\n", FileHeader.bfSize);

	FileHeader.bfReserved1 = ReadShort(fp);
	FileHeader.bfReserved2 = ReadShort(fp);

	FileHeader.bfOffBytes = ReadInt(fp);
	if (VERBOSE)	fprintf(stderr, "FileHeader.bfOffBytes = %d\n", FileHeader.bfOffBytes);

	InfoHeader.biSize = ReadInt(fp);
	if (VERBOSE)	fprintf(stderr, "InfoHeader.biSize = %d\n", InfoHeader.biSize);
	InfoHeader.biWidth = ReadInt(fp);
	if (VERBOSE)	fprintf(stderr, "InfoHeader.biWidth = %d\n", InfoHeader.biWidth);
	InfoHeader.biHeight = ReadInt(fp);
	if (VERBOSE)	fprintf(stderr, "InfoHeader.biHeight = %d\n", InfoHeader.biHeight);

	const int nums = InfoHeader.biWidth;
	const int numt = InfoHeader.biHeight;

	InfoHeader.biPlanes = ReadShort(fp);
	if (VERBOSE)	fprintf(stderr, "InfoHeader.biPlanes = %d\n", InfoHeader.biPlanes);

	InfoHeader.biBitCount = ReadShort(fp);
	if (VERBOSE)	fprintf(stderr, "InfoHeader.biBitCount = %d\n", InfoHeader.biBitCount);

	InfoHeader.biCompression = ReadInt(fp);
	if (VERBOSE)	fprintf(stderr, "InfoHeader.biCompression = %d\n", InfoHeader.biCompression);

	InfoHeader.biSizeImage = ReadInt(fp);
	if (VERBOSE)	fprintf(stderr, "InfoHeader.biSizeImage = %d\n", InfoHeader.biSizeImage);

	InfoHeader.biXPixelsPerMeter = ReadInt(fp);
	InfoHeader.biYPixelsPerMeter = ReadInt(fp);

	InfoHeader.biClrUsed = ReadInt(fp);
	if (VERBOSE)	fprintf(stderr, "InfoHeader.biClrUsed = %d\n", InfoHeader.biClrUsed);

	InfoHeader.biClrImportant = ReadInt(fp);

	// fprintf( stderr, "Image size found: %d x %d\n", ImageWidth, ImageHeight );

	// pixels will be stored bottom-to-top, left-to-right:
	unsigned char* texture = new unsigned char[3 * nums * numt];
	if (texture == NULL)
	{
		fprintf(stderr, "Cannot allocate the texture array!\n");
		return NULL;
	}

	// extra padding bytes:

	int requiredRowSizeInBytes = 4 * ((InfoHeader.biBitCount * InfoHeader.biWidth + 31) / 32);
	if (VERBOSE)	fprintf(stderr, "requiredRowSizeInBytes = %d\n", requiredRowSizeInBytes);

	int myRowSizeInBytes = (InfoHeader.biBitCount * InfoHeader.biWidth + 7) / 8;
	if (VERBOSE)	fprintf(stderr, "myRowSizeInBytes = %d\n", myRowSizeInBytes);

	int oldNumExtra = 4 * (((3 * InfoHeader.biWidth) + 3) / 4) - 3 * InfoHeader.biWidth;
	if (VERBOSE)	fprintf(stderr, "Old NumExtra padding = %d\n", oldNumExtra);

	int numExtra = requiredRowSizeInBytes - myRowSizeInBytes;
	if (VERBOSE)	fprintf(stderr, "New NumExtra padding = %d\n", numExtra);

	// this function does not support compression:

	if (InfoHeader.biCompression != 0)
	{
		fprintf(stderr, "Wrong type of image compression: %d\n", InfoHeader.biCompression);
		fclose(fp);
		return NULL;
	}

	// we can handle 24 bits of direct color:
	if (InfoHeader.biBitCount == 24)
	{
		rewind(fp);
		fseek(fp, FileHeader.bfOffBytes, SEEK_SET);
		int t;
		unsigned char* tp;
		for (t = 0, tp = texture; t < numt; t++)
		{
			for (int s = 0; s < nums; s++, tp += 3)
			{
				*(tp + 2) = fgetc(fp);		// b
				*(tp + 1) = fgetc(fp);		// g
				*(tp + 0) = fgetc(fp);		// r
			}

			for (int e = 0; e < numExtra; e++)
			{
				fgetc(fp);
			}
		}
	}

	// we can also handle 8 bits of indirect color:
	if (InfoHeader.biBitCount == 8 && InfoHeader.biClrUsed == 256)
	{
		struct rgba32
		{
			unsigned char r, g, b, a;
		};
		struct rgba32* colorTable = new struct rgba32[InfoHeader.biClrUsed];

		rewind(fp);
		fseek(fp, sizeof(struct bmfh) + InfoHeader.biSize - 2, SEEK_SET);
		for (int c = 0; c < InfoHeader.biClrUsed; c++)
		{
			colorTable[c].r = fgetc(fp);
			colorTable[c].g = fgetc(fp);
			colorTable[c].b = fgetc(fp);
			colorTable[c].a = fgetc(fp);
			if (VERBOSE)	fprintf(stderr, "%4d:\t0x%02x\t0x%02x\t0x%02x\t0x%02x\n",
				c, colorTable[c].r, colorTable[c].g, colorTable[c].b, colorTable[c].a);
		}

		rewind(fp);
		fseek(fp, FileHeader.bfOffBytes, SEEK_SET);
		int t;
		unsigned char* tp;
		for (t = 0, tp = texture; t < numt; t++)
		{
			for (int s = 0; s < nums; s++, tp += 3)
			{
				int index = fgetc(fp);
				*(tp + 0) = colorTable[index].r;	// r
				*(tp + 1) = colorTable[index].g;	// g
				*(tp + 2) = colorTable[index].b;	// b
			}

			for (int e = 0; e < numExtra; e++)
			{
				fgetc(fp);
			}
		}

		delete[] colorTable;
	}

	fclose(fp);

	*width = nums;
	*height = numt;
	return texture;
}

int
ReadInt(FILE* fp)
{
	const unsigned char b0 = fgetc(fp);
	const unsigned char b1 = fgetc(fp);
	const unsigned char b2 = fgetc(fp);
	const unsigned char b3 = fgetc(fp);
	return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}

short
ReadShort(FILE* fp)
{
	const unsigned char b0 = fgetc(fp);
	const unsigned char b1 = fgetc(fp);
	return (b1 << 8) | b0;
}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb(float hsv[3], float rgb[3])
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while (h >= 6.)	h -= 6.;
	while (h < 0.) 	h += 6.;

	float s = hsv[1];
	if (s < 0.)
		s = 0.;
	if (s > 1.)
		s = 1.;

	float v = hsv[2];
	if (v < 0.)
		v = 0.;
	if (v > 1.)
		v = 1.;

	// if sat==0, then is a gray:

	if (s == 0.0)
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:

	float i = (float)floor(h);
	float f = h - i;
	float p = v * (1.f - s);
	float q = v * (1.f - s * f);
	float t = v * (1.f - (s * (1.f - f)));

	float r = 0., g = 0., b = 0.;			// red, green, blue
	switch ((int)i)
	{
	case 0:
		r = v;	g = t;	b = p;
		break;

	case 1:
		r = q;	g = v;	b = p;
		break;

	case 2:
		r = p;	g = v;	b = t;
		break;

	case 3:
		r = p;	g = q;	b = v;
		break;

	case 4:
		r = t;	g = p;	b = v;
		break;

	case 5:
		r = v;	g = p;	b = q;
		break;
	}

	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void
Cross(float v1[3], float v2[3], float vout[3])
{
	float tmp[3];
	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];
	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}

float
Dot(float v1[3], float v2[3])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float
Unit(float vin[3], float vout[3])
{
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}
	return dist;
}

struct point
{
	float x, y, z;		// coordinates
	float nx, ny, nz;	// surface normal
	GLfloat s, t;		// texture coords
};

inline
struct point*
	PtsPointer(int lat, int lng)
{
	if (lat < 0)	lat += (Latitudes - 1);
	if (lng < 0)	lng += (Longitudes - 0);
	if (lat > Latitudes - 1)	lat -= (Latitudes - 1);
	if (lng > Longitudes - 1)	lng -= (Longitudes - 0);
	return &Pts[Longitudes * lat + lng];
}

inline
void
DrawPoint(struct point* p)
{
	glNormal3fv(&p->nx);
	//glTexCoord2fv(&p->s);
	glTexCoord2f((*p).s, (*p).t);
	glVertex3fv(&p->x);
}
//code given by prof
void
OsuSphere(float radius, int slices, int stacks)
{
	// set the globals:

	Longitudes = slices;
	Latitudes = stacks;
	if (Longitudes < 3)
		Longitudes = 3;
	if (Latitudes < 3)
		Latitudes = 3;

	// allocate the point data structure:

	Pts = new struct point[Longitudes * Latitudes];

	// fill the Pts structure:

	for (int ilat = 0; ilat < Latitudes; ilat++)
	{
		float lat = -M_PI / 2. + M_PI * (float)ilat / (float)(Latitudes - 1);	// ilat=0/lat=0. is the south pole
		// ilat=Latitudes-1, lat=+M_PI/2. is the north pole
		float xz = cosf(lat);
		float  y = sinf(lat);
		for (int ilng = 0; ilng < Longitudes; ilng++)				// ilng=0, lng=-M_PI and
			// ilng=Longitudes-1, lng=+M_PI are the same meridian
		{
			float lng = -M_PI + 2. * M_PI * (float)ilng / (float)(Longitudes - 1);
			float x = xz * cosf(lng);
			float z = -xz * sinf(lng);
			struct point* p = PtsPointer(ilat, ilng);
			p->x = radius * x;
			p->y = radius * y;
			p->z = radius * z;
			p->nx = x;
			p->ny = y;
			p->nz = z;
			if (Distort)
			{
				p->s = (lng + M_PI) / (2. * M_PI);
				p->t = (lat + cosf(4 * M_PI * (ms / 60.0 + (float)ilng / (float)Longitudes)) + M_PI / 2.) / M_PI;
				//p->s = ((lng + M_PI) / 0.5 + sinf(M_PI * 2 * ms / 45 + lat) / 2) / (2. * 2. * M_PI);
				//p->t = (lat + M_PI / 2.) / M_PI;
			}
			else
			{
				p->s = (lng + M_PI) / (2. * M_PI);
				p->t = (lat + M_PI / 2.) / M_PI;
			}
		}
	}

	struct point top, bot;		// top, bottom points

	top.x = 0.;		top.y = radius;	top.z = 0.;
	top.nx = 0.;		top.ny = 1.;		top.nz = 0.;
	top.s = 0.;		top.t = 1.;

	bot.x = 0.;		bot.y = -radius;	bot.z = 0.;
	bot.nx = 0.;		bot.ny = -1.;		bot.nz = 0.;
	bot.s = 0.;		bot.t = 0.;

	// connect the north pole to the latitude Latitudes-2:

	glBegin(GL_TRIANGLE_STRIP);
	for (int ilng = 0; ilng < Longitudes; ilng++)
	{
		float lng = -M_PI + 2. * M_PI * (float)ilng / (float)(Longitudes - 1);
		top.s = (lng + M_PI) / (2. * M_PI);
		DrawPoint(&top);
		struct point* p = PtsPointer(Latitudes - 2, ilng);	// ilat=Latitudes-1 is the north pole
		DrawPoint(p);
	}
	glEnd();

	// connect the south pole to the latitude 1:

	glBegin(GL_TRIANGLE_STRIP);
	for (int ilng = Longitudes - 1; ilng >= 0; ilng--)
	{
		float lng = -M_PI + 2. * M_PI * (float)ilng / (float)(Longitudes - 1);
		bot.s = (lng + M_PI) / (2. * M_PI);
		bot.s;
		DrawPoint(&bot);
		struct point* p = PtsPointer(1, ilng);					// ilat=0 is the south pole
		DrawPoint(p);
	}
	glEnd();

	// connect the horizontal strips:

	for (int ilat = 2; ilat < Latitudes - 1; ilat++)
	{
		struct point* p;
		glBegin(GL_TRIANGLE_STRIP);
		for (int ilng = 0; ilng < Longitudes; ilng++)
		{
			p = PtsPointer(ilat, ilng);
			DrawPoint(p);
			p = PtsPointer(ilat - 1, ilng);
			DrawPoint(p);
		}
		glEnd();
	}

	// clean-up:

	delete[] Pts;
	Pts = NULL;
}



float*
Array3(float a, float b, float c)
{
	static float array[4];

	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}

