

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

struct Point{
    float x;
    float y;
    float z;
    Point(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

};

float alpha = 45, beta = 0, r = 50;
float camX = 0, camY = 30, camZ = 40, speed = 1.0f;
int startX, startY, tracking = 0;

float eyeX = 0, eyeY= 30, eyeZ = 40, centerX = 1, centerY =1.5, centerZ=1, upX=0.0, upY=1.0, upZ = 0.0,fov=45.0f,near=1.0f,far=1000.0f,
        dx=0, dy=0,dz=0,rx=0,ry=0,rz=0;

unsigned int t;
int tw, th;
unsigned char * imageData;

GLuint* vertices;
double vertexCount;
GLuint buffers[1];

std::vector<float> vertexB;

int reforest = 1;
std::vector<Point> treesPositions;

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
    float pixel = imageData[i * tw + j];
    //return pixel / 255 * 30;
    return pixel;
}

float hf(float x, float z){
    float x1 = floor(x);
    float x2 = x1 + 1;
    float z1 = floor(z);
    float z2 = z1 + 1;
    float fz = z - z1;
    float h_x1_z = h(x1,z1) * (1-fz) + h(x1,z2) * fz;
    float h_x2_z = h(x2,z1) * (1-fz) + h(x2,z2) * fz;
    float fx = x - x1;
    float height_xz = h_x1_z * (1 - fx) + h_x2_z * fx;

    return height_xz;
}



void spherical2Cartesian(){
    eyeY = 2 + hf(eyeX, eyeZ);
    centerX =  eyeX - cos(beta) * sin(alpha);
    centerY =  eyeY;// - sin(beta);
    centerZ =  eyeZ - cos(beta) * cos(alpha);
    dx = centerX - eyeX;
    dy = centerY - eyeY;
    dz = centerZ - eyeZ;
    float norm = sqrt(dx * dx + dy * dy + dz * dz);
    dx /= norm;
    dy /= norm;
    dz /= norm;
    rx = dy*upZ - dz*upY;
    ry = dz*upX - dx*upZ;
    rz = dx*upY - dy*upX;
}


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
        for(int i=0; i<N;i++){
            int angle = rand();
            int radius = 40 + rand()%70;
            float x = sin(angle)*radius;
            float z = cos(angle)*radius;
            float y = hf(x+ (th/2) ,z + (tw/2));

            treesPositions.push_back(Point(x, y, z));
        }

        reforest = 0;
    }
    //glPushMatrix();
    //glTranslatef(-th/2,0,-tw/2);
    for(Point p : treesPositions){
        glPushMatrix();
        glTranslatef(p.x,p.y,p.z);
        drawTree(1,6,3,9,20,20);
        glPopMatrix();
    }
    //glPopMatrix();

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

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ,
              centerX,centerY,centerZ,
              upX,upY,upZ);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	drawTerrain();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawTrees(400);
    drawTorus();
    int rotation = glutGet(GLUT_ELAPSED_TIME) / 20 % 360;
    glRotatef(-rotation, 0.0, 1.0, 0.0);
    drawBlueTeaPots(8);
    glRotatef(2*rotation, 0.0, 1.0, 0.0);
    drawRedTeaPots(18);


	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	//glutWireTeapot(2.0);

// End of frame
	glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy) {

    switch (key) {
        case GLUT_KEY_LEFT:
            alpha += 0.05;
            break;

        case GLUT_KEY_RIGHT:
            alpha -= 0.05;
            break;

        case GLUT_KEY_DOWN:
            beta += 0.05f;
            if (beta > 1.5f)
                beta = 1.5f;
            break;

        case GLUT_KEY_UP:
            beta -= 0.05f;
            if (beta < -1.5f)
                beta = -1.5f;
            break;

        default:
            break;
    }
    spherical2Cartesian();
    //glutPostRedisplay();

}

void processKeys(unsigned char c, int xx, int yy) {
    switch (c) {
        case 'w':
            centerX += speed*dx;
            eyeX += speed*dx;
            //centerY += speed*dy;
            //eyeY += speed*dy;
            centerZ += speed*dz;
            eyeZ += speed*dz;
            break;
        case 's':
            centerX -= speed*dx;
            eyeX -= speed*dx;
            //centerY -= speed*dy;
            //eyeY -= speed*dy;
            centerZ -= speed*dz;
            eyeZ -= speed*dz;
            break;
        case 'a':
            centerX -= speed*rx;
            eyeX -= speed*rx;
            //centerY -= speed*ry;
            //eyeY -= speed*ry;
            centerZ -= speed*rz;
            eyeZ -= speed*rz;
            break;
        case 'd':
            centerX += speed*rx;
            eyeX += speed*rx;
            centerY += speed*ry;
            eyeY += speed*ry;
            centerZ += speed*rz;
            eyeZ += speed*rz;
            break;
        case '+':
            speed *= 2;
            if (speed > 64.0f) speed = 64.0f;
            break;
        case '-':
            speed /= 2;
            if (speed < 0.1f) speed = 0.1f;
            break;

    }

    spherical2Cartesian();
    //glutPostRedisplay();


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
        std::vector<float> strip;
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

    //
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
    //glutSpecialFunc(processSpecialKeys);
    //glutMouseFunc(processMouseButtons);
    //glutMotionFunc(processMouseMotion);
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);


    //spherical2Cartesian();
    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);

    ilInit();

    init();

    spherical2Cartesian();

// enter GLUT's main cycle
    glutMainLoop();

    return 0;
}


