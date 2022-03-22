#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

# include<stdlib.h>
#include<time.h>
#include <vector>

struct Tuple{
    int angle;
    int radius;
    Tuple(int angle, int raidus){
        this->angle = angle;
        this->radius = raidus;
    }

};

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;
int reforest = 1;
std::vector<Tuple> treesPositions;


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawTree(float trunk_base, float trunk_height, float top_base, float top_height, int slices, int stacks){
    glPushMatrix();
    glRotatef(-90,1,0,0);
    glColor3f(0.57, 0.42, 0.07);
    glutSolidCone(trunk_base,trunk_height,slices,stacks);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,trunk_height*0.7,0);
    glRotatef(-90,1,0,0);
    glColor3f(0.1, 0.28, 0.11);
    glutSolidCone(top_base,top_height,slices,stacks);
    glPopMatrix();
    glColor3f(1,1,1);


}

void drawTrees(int N){

    if(reforest){
        treesPositions.clear();
        for(int i=0; i<N;i++){
            int angle= rand();
            int radius = 50 + rand()%50;

            treesPositions.push_back(Tuple(angle, radius));
        }

        reforest = 0;
    }
    for(int i=0;i<N; i++){
        glPushMatrix();
        int angle = treesPositions[i].angle;
        int radius = treesPositions[i].radius;
        glRotatef(angle,0,1,0);
        glTranslatef(radius,0,0);
        //drawTree(0.5,2,1,3,20,20);
        drawTree(1,6,3,9,20,20);
        glPopMatrix();
    }

}

void drawTorus(){
    glPushMatrix();
    glTranslatef(0,0.5,0);
    glColor3f(0.73, 0.04, 0.61);
    glutSolidTorus(2,5,50,50);
    glPopMatrix();
    glColor3f(1,1,1);
}

void drawBlueTeaPots(int N){
    float arch = 360/N;
    glColor3f(0.07, 0.11, 0.84);
    for(int i=0; i<N; i++){
        glPushMatrix();
        glRotatef(arch*i,0,1,0);
        glTranslatef(15,1.25,0);
        glRotatef(-glutGet(GLUT_ELAPSED_TIME) / 20 % 360, 0.0, 1.0, 0.0);
        glutSolidTeapot(2);
        glPopMatrix();
    }

    glColor3f(1,1,1);

}

void drawRedTeaPots(int N){
    float arch = 360/N;
    glColor3f(0.84, 0.07, 0.07);
    for(int i=0; i<N; i++){
        glPushMatrix();
        glRotatef(arch*i,0,1,0);
        glTranslatef(35,1.25,0);
        glRotatef(90,0,1,0);
        glRotatef(glutGet(GLUT_ELAPSED_TIME) / 20 % 360, 0.0, 1.0, 0.0);
        glutSolidTeapot(2);
        glPopMatrix();
    }

    glColor3f(1,1,1);

}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();

    glColor3f(1,1,1);
    drawTrees(400);
    drawTorus();
    int rotation = glutGet(GLUT_ELAPSED_TIME) / 20 % 360;
    glRotatef(-rotation, 0.0, 1.0, 0.0);
    drawBlueTeaPots(8);
    glRotatef(2*rotation, 0.0, 1.0, 0.0);
    drawRedTeaPots(18);


	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
    switch (c) {

        case ' ':
            reforest = 1;
            break;
        default:
            break;
    }
    glutPostRedisplay();

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
        default:
            break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {
    srand(1);
// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
