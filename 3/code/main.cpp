#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include<iostream>
#define DISPLACEMENT 0.1
#define ROTATION 15


float ox = 0, oy = 0, oz = 0 , angle = 0, alpha = 0, beta = 0, eyeX = 5.0, eyeY = 5.0, eyeZ = 5.0, radius;


int mode = GL_LINE, rota = 0,  rx = 0, ry =1, rz =0, blink = 0, speed = 5, max_speed = 15,
width = 800, height = 800, mouseX = 0, mouseY = 0;

void setCamera(){
    eyeX = radius*sin(alpha)*cos(beta);
    eyeY = radius*sin(beta);
    eyeZ = radius*cos(alpha)*cos(beta);
}


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

void drawAxis(){
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(radius,0,0);
    glVertex3f(-radius,0,0);

    glColor3f(0,1,0);
    glVertex3f(0, radius,0);
    glVertex3f(0,-radius,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,radius);
    glVertex3f(0,0,-radius);
    glEnd();
    glColor3f(1,1,1);
}


void drawCylinder( float radius, float height, int slices) {

// put code to draw cylinder in here


    float arc = M_PI * 2 / slices;
    float x, z, next_x, next_z;
    int c;

    //TOP
    glBegin(GL_TRIANGLES);
    for(int i=0; i<=slices; i++){
        c = (i * slices)%90;
        //glColor3f(float(sin(c)),float(cos(c)),float(tan(c)));



        x = (float) (radius * sin(1.0*i*arc));
        z = (float) (radius * cos(1.0*i*arc));
        next_x = (float) (radius * sin(1.0*(i+1)*arc));
        next_z = (float) (radius * cos(1.0*(i+1)*arc));
        glVertex3d(0,height,0);
        glVertex3f(x,height,z);
        glVertex3f(next_x,height,next_z);




    }
    glEnd();


    //SIDE
    glBegin(GL_TRIANGLES);

    for (int i = 0; i<=slices; i++){
        c = (i * slices)%90;
        //glColor3f(float(sin(c)),float(cos(c)),float(tan(c)));

        x = (float) (radius * sin(1.0*i*arc));
        z = (float) (radius * cos(1.0*i*arc));
        next_x = (float) (radius * sin(1.0*(i+1)*arc));
        next_z = (float) (radius * cos(1.0*(i+1)*arc));

        glVertex3f(x,height,z);
        glVertex3f(x,0,z);
        glVertex3f(next_x,height,next_z);

        c = ((i-slices) * slices)%90;
        //glColor3f(float(sin(c)),float(cos(c)),float(tan(c)));

        glVertex3f(x,0,z);
        glVertex3f(next_x,0,next_z);
        glVertex3f(next_x,height,next_z);
    }
    glEnd();



    //BASE
    glBegin(GL_TRIANGLES);
    for(int i=slices; i>=0; i--){
        c = ((i-slices) * slices)%90;
        //glColor3f(float(sin(c)),float(cos(c)),float(tan(c)));


        x = (float) (radius * sin(1.0*i*arc));
        z = (float) (radius * cos(1.0*i*arc));
        next_x = (float) (radius * sin(1.0*(i+1)*arc));
        next_z = (float) (radius * cos(1.0*(i+1)*arc));

        glVertex3d(0,0,0);
        glVertex3f(next_x,0,next_z);
        glVertex3f(x,0,z);
    }
    glEnd();

}










void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(eyeX,eyeY,eyeZ,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    glTranslatef(ox,oy,oz);
    glRotatef(angle,rx,ry,rz);
    if(rota==1){
        glRotatef(float(glutGet(GLUT_ELAPSED_TIME) / 20 % 360), rx, ry, rz);
    }
    else if(rota==-1){
        glRotatef(float(- glutGet(GLUT_ELAPSED_TIME) / 20 % 360), rx, ry, rz);
    }

    if(blink ==1){
        if(speed <max_speed/3)
            mode = GL_LINE;
        else if(speed>=max_speed/3 && speed<(max_speed/3)*2 )
            mode = GL_POINT;
        else
            mode = GL_FILL;
        speed++;
        speed %= max_speed;
    }
    drawAxis();

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
        case 'E':
            rota = 1;
            break;
        case 'q':
            angle -= ROTATION;
            break;
        case 'Q':
            rota = -1;
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
            blink += 1;
            blink %= 2;
            break;
        case 'x':
            rx++;
            rx %=2;
            break;
        case 'y':
            ry++;
            ry %= 2;
            break;
        case 'z':
            rz++;
            rz %=2;
            break;
        case 'r':
            angle = 0;
            ox = 0;
            oy = 0;
            oz = 0;
            rota = 0;
            rx = 0;
            ry = 1;
            rz = 0;
            blink = 0;
            mode = GL_LINE;
            break;
        default:
            break;

    }
    //glutPostRedisplay();


}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here
    switch (key) {
        case GLUT_KEY_RIGHT:
            alpha -= 0.1;
            break;
        case GLUT_KEY_LEFT:
            alpha +=0.1;
            break;
        case GLUT_KEY_UP:
            beta +=0.1;
            if(beta > M_PI_2)
                beta = M_PI_2;
            break;
        case GLUT_KEY_DOWN:
            beta -=0.1;
            if(beta < -M_PI_2)
                beta = -M_PI_2;
            break;
        case GLUT_KEY_PAGE_UP:
            radius += 0.1;
            break;
        case GLUT_KEY_PAGE_DOWN:
            radius -= 0.1;
            if(radius < 0.1)
                radius = 0.1;
            break;

        default:
            break;

    }
    setCamera();

}

void passiveMouseFunc(int x, int y){
    if(mouseX ==0 && mouseY==0){
        mouseX = x;
        mouseY = y;
    }
    if(x>mouseX){
        alpha -= 0.05;
        mouseX = x;

    }

    else if(x<mouseX){
        alpha += 0.05;
        mouseX = x;
    }

    if(y>mouseY){
        beta -= 0.05;
        if(beta < -M_PI_2)
            beta = -M_PI_2;
        mouseY = y;

    }

    else if(y<mouseY){
        beta += 0.05;
        if(beta > M_PI_2)
            beta = M_PI_2;
        mouseY = y;
    }

    mouseY = y;
    mouseX = x;
    setCamera();

}


int main(int argc, char **argv) {


    radius = sqrt(eyeX*eyeX + eyeY*eyeY + eyeZ*eyeZ);
    beta = asin(eyeY/radius);
    alpha = asin(eyeX/(radius*cos(beta)));

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
    glutPassiveMotionFunc(passiveMouseFunc);
	
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
