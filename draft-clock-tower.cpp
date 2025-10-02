//
//  draft-clock-tower.cpp
//  OpenGL
//
//  Created by CJ Abendanio on 10/2/25.
//

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <cmath>
//using namespace std;

void tower() {
    glColor3f(1.0f, 1.0f, 1.0f); // white color
    
    // roof
    glBegin(GL_TRIANGLES);
    // start
        glVertex2f(-0.2f, 0.0f);  // left corner
        glVertex2f(0.2f, 0.0f);   // right corner
        glVertex2f(0.0f, 0.3f);   // top point
    glEnd();
    
    glBegin(GL_LINES);
    // roof shadow
    // top line
        glVertex2f(0.0f, 0.3f);
        glVertex2f(0.2f, 0.2f);
    // diagonal line
        glVertex2f(0.2f, 0.2f);
        glVertex2f(0.37f, -0.05f);
    // bottom line
        glVertex2f(0.2f, 0.0f);
        glVertex2f(0.37f, -0.05f);

    // body
    // left line
        glVertex2f(-0.2f, 0.0f);
        glVertex2f(-0.2f, -0.9f);
    // right line
        glVertex2f(0.2f,0.0f);
        glVertex2f(0.2f,-0.9f);
    // botton-mid connector
        glVertex2f(-0.2f, -0.9f);
        glVertex2f(0.2f, -0.9f);
    // right-2 line
        glVertex2f(0.37f, -0.05f);
        glVertex2f(0.37f, -0.8f);
    // bottom-right connecter
        glVertex2f(0.2f, -0.9f);
        glVertex2f(0.37f, -0.8f);
    glEnd();
    
    
    glFlush();
}

void clock_face() {
    float cx = 0.0f;    // center x
    float cy = -0.35f;  // slightly higher in tower
    float r = 0.18f;    // bigger clock radius

    glColor3f(1.0f, 1.0f, 1.0f); // white clock

    // draw perfect circle (adjust x for window aspect ratio 2:1)
    glBegin(GL_LINE_LOOP);
    int segments = 200;
    float scaleX = 0.5f; // compensate horizontal stretching
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = r * cosf(theta) * scaleX; // scale x
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();

    // hour hand - slightly longer
    glBegin(GL_LINES);
    glVertex2f(cx, cy);
    glVertex2f(cx, cy + 0.08f);
    glEnd();

    // minute hand - slightly longer
    glBegin(GL_LINES);
    glVertex2f(cx, cy);
    glVertex2f(cx + 0.12f * scaleX, cy); // scale x for minute hand
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // vertical rectangle
    // rectangle(-0.2f, -0.8f, 0.4f, 0.8f);
    tower();
    clock_face();

    glutSwapBuffers();
}

int main(int argc,char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,400);
    glutCreateWindow("Clock Tower");

    glClearColor(0.1f,0.1f,0.15f,1.0f);

    glutDisplayFunc(display);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutMainLoop();
    return 0;
}
