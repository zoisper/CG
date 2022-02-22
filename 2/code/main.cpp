#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#define DISPLACEMENT 0.1
#define SCALE 0.1
#define ROTATION 15


float ox = 0, oy = 0, oz = 0  , angle = 0 , sx = 1, sy = 1, sz = 1;

int width = 800, height = 800, mode = GL_FILL;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;
    width = w;
    height = h;


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


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

// put the geometric transformations here
    glScalef(sx,sy,sz);
    glTranslatef(ox,oy,oz);
    glRotatef(angle,0,1,0);

// put drawing instructions here

    glPolygonMode(GL_FRONT, mode);
    glBegin(GL_TRIANGLES);

    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-0.4, 0.0, 0.4);
    glVertex3f(0.4, 0.0, 0.4);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(0.4, 0.0, 0.4);
    glVertex3f(0.4, 0.0, -0.4);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(0.4, 0.0, -0.4);
    glVertex3f(-0.4, 0.0, -0.4);

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-0.4, 0.0, -0.4);
    glVertex3f(-0.4, 0.0, 0.4);



    glEnd();



	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events

void keyboardFunc (unsigned char key, int x, int y){

    switch(key){
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
        case'X':
            sx += SCALE;
            break;
        case 'x':
            sx -= SCALE;
            break;
        case'Y':
            sy += SCALE;
            break;
        case 'y':
            sy -= SCALE;
        case 'Z':
            sz += SCALE;
            break;
        case 'z':
            sz -= SCALE;
            break;
        case '+':
            sx += SCALE;
            sy += SCALE;
            sz += SCALE;
            break;
        case '-':
            sx -= SCALE;
            sy -= SCALE;
            sz -= SCALE;
            break;
        case 'l':
            mode = GL_LINE;
            break;
        case 'f':
            mode = GL_FILL;
            break;
        case 'p':
            mode = GL_POINT;
            break;
        case 'r':
            ox = 0;
            oy = 0;
            oz = 0;
            angle = 0;
            sx = 1;
            sy = 1;
            sz = 1;
            mode = GL_FILL;
            break;
        default:
            break;
    }
    glutPostRedisplay();

}

    void specialFunc(int key_code, int x, int y){
        switch (key_code) {
            case GLUT_KEY_UP:
                oy += DISPLACEMENT;
                break;
            case GLUT_KEY_DOWN:
                oy -= DISPLACEMENT;
                break;
            case GLUT_KEY_RIGHT:
                ox += DISPLACEMENT;
                break;
            case GLUT_KEY_LEFT:
                ox -= DISPLACEMENT;
                break;
            default:
                break;
        }

        glutPostRedisplay();

    }

void mouseFunc (int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        oz -= DISPLACEMENT;
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
       oz += DISPLACEMENT;



    glutPostRedisplay();
}

void passiveMouseFunc(int x, int y){
    if(x> width/2 && y > height/2)
        glClearColor(0.5f, 0.0f, 0.5f, 0.0f);
    else if(x>width/2 && y<height/2)
        glClearColor(0.0f, 0.5f, 0.5f, 0.0f);
    else if(width/2 && y<height/2)
        glClearColor(0.5f, 0.5f, 0.0f, 0.0f);
    else if(x<width/2 && y>height/2)
        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    else
        glClearColor(0.0f,0.0f,0.0f,0.0f);

    glutPostRedisplay();
}










int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Pyramid");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
    //glutIdleFunc(renderScene);

	
// put here the registration of the keyboard callbacks

    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutMouseFunc(mouseFunc);
    glutPassiveMotionFunc(passiveMouseFunc);



//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
