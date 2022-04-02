

#include<stdio.h>
#include<stdlib.h>
#include<iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

unsigned int t;
int tw, th;
unsigned char * imageData;

GLuint* vertices;
double vertexCount;
GLuint buffers[1];

std::vector<float> vertexB;


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void drawTerrain() {

    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    for (int i = 0; i < th - 1 ; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, (tw) * 2 * i, (tw) * 2);
    }

    
}




void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    glPolygonMode(GL_FRONT, GL_LINE);

	drawTerrain();


	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	//glutWireTeapot(2.0);

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}



void processMouseButtons(int button, int state, int xx, int yy) {

    if (state == GLUT_DOWN)  {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {

            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}


void processMouseMotion(int xx, int yy) {

    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {


        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = r;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}


double h(int i, int j) {
    double pixel = imageData[i * tw + j];
    return pixel / 255 * 30;
}

void init() {

// 	Load the height map "terreno.jpg"
    ilGenImages(1, &t);
    ilBindImage(t);

    ilLoadImage((ILstring)"terreno.jpg");
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    imageData = ilGetData();

// 	Build the vertex arrays
    vertices = (GLuint *)calloc(th-1, sizeof(GLuint));
    glGenBuffers(1, buffers);

    int halfH = th / 2;
    int halfW = tw / 2;

    vertexCount = 2 * tw;

    for(int i = 0; i < th - 1; i++) {
        std::vector<double> strip;
        for(int j = 0; j < tw; j++) {
            vertexB.push_back(i + 1 - halfH);
            vertexB.push_back(h(i+1,j));
            vertexB.push_back(j - halfW);

            vertexB.push_back(i - halfH);
            vertexB.push_back(h(i,j));
            vertexB.push_back(j -halfW);

        }

    }
    glGenBuffers(1, buffers);
    glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
    glBufferData(GL_ARRAY_BUFFER,vertexB.size()*sizeof(float ) , vertexB.data(), GL_STATIC_DRAW);


// 	OpenGL settings
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
}






int main(int argc, char **argv) {

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("CG@DI-UM");


// Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);

    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);

    ilInit();

    init();

// enter GLUT's main cycle
    glutMainLoop();

    return 0;
}


