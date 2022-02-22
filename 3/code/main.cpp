#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#define DISPLACEMENT 0.1
#define ROTATION 15


float ox = 0, oy = 0, oz = 0 , angle = 0;
int mode = GL_LINE, rota = 0, blink = 0;


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


void drawCylinder( float radius, float height, int slices) {

// put code to draw cylinder in here

    //TOP

    float arc = M_PI * 2 / slices;
    float x,y,z;


    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(0,height,0);

    for(int i=0; i<=slices; i++){
        x = (float) (radius * sin(1.0*i*arc));
        y = height;
        z = (float) (radius * cos(1.0*i*arc));
        glColor3f(float(sin(i)),float(cos(i)),float(tan(i)));
        glVertex3f(x,y,z);


    }
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i<=slices; i++){
        x = (float) (radius * sin(1.0*i*arc));
        z = (float) (radius * cos(1.0*i*arc));
        glVertex3f(x,height,z);
        x = (float) (radius * sin(1.0*i*arc +arc/2));
        z = (float) (radius * cos(1.0*i*arc + arc/2));
        glColor3f(float(sin(i)),float(cos(i)),float(tan(i)));
        glVertex3f(x,0,z);
    }
    glEnd();

    //BASE
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(0,0,0);
    for(int i=slices; i>=0; i--){
        x = (float) (radius * sin(1.0*i*arc));
        y = 0;
        z = (float) (radius * cos(1.0*i*arc));
        glColor3f(float(sin(i-slices)),float(cos(i-slices)),float(tan(i-slices)));
        glVertex3f(x,y,z);
    }
    glEnd();

}




void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    glTranslatef(ox,oy,oz);
    glRotatef(angle,0,1,0);
    if(rota==1){
        glRotatef(float(glutGet(GLUT_ELAPSED_TIME) / 20 % 360), 0.0, 1.0, 0.0);
    }
    else if(rota==-1){
        glRotatef(float(- glutGet(GLUT_ELAPSED_TIME) / 20 % 360), 0.0, 1.0, 0.0);
    }

    if(blink ==1){
        int m = int(glutGet(GLUT_ELAPSED_TIME))%15;
        if(m <5)
            mode = GL_LINE;
        else if(m>=5 && m<10 )
            mode = GL_POINT;
        else
            mode = GL_FILL;
    }


    glPolygonMode(GL_FRONT, mode);
	drawCylinder(1,2,10);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
    switch (c) {
        case 'a':
            ox -= DISPLACEMENT;
            break;
        case 'd':
            ox += DISPLACEMENT;
            break;
        case 'w':
            oz -= DISPLACEMENT;
            break;
        case 's':
            oz += DISPLACEMENT;
            break;
        case 'S':
            oy -= DISPLACEMENT;
            break;
        case 'W':
            oy += DISPLACEMENT;
            break;
        case 'e':
            angle += ROTATION;
            break;
        case 'q':
            angle -= ROTATION;
            break;
        case 'f':
            mode = GL_FILL;
            blink = 0;
            break;
        case 'l':
            mode = GL_LINE;
            blink = 0;
            break;
        case 'p':
            mode = GL_POINT;
            blink = 0;
            break;
        case ' ':
            rota = 0;
            break;
        case 'b':
            blink= 1;
            break;
        case 'r':
            angle = 0;
            ox = 0;
            oy = 0;
            oz = 0;
            rota = 0;
            blink = 0;
            mode = GL_LINE;
        default:
            break;

    }
    //glutPostRedisplay();


}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here
    switch (key) {
        case GLUT_KEY_RIGHT:
            rota = 1;
            break;
        case GLUT_KEY_LEFT:
            rota = -1;
            break;
        default:
            break;

    }

}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Cylinder");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
