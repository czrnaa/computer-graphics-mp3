//  final-clock-tower.cpp
//  OpenGL
//  Created by CJ Abendanio on 10/2/25.
//  Edited by Johann Taguiam 10/3/25.

#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

const float PI = 3.14159265f;
bool isDay = true;

// ---- Clock Simulation ----
float simMinutes = 0.0f;    // simulated minutes (0–720 for 12 hours)
float speedFactor = 60.0f;  // how fast time flows (60 = 1 min per real sec)

// Draw filled circle
void drawFilledCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * i / num_segments;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Draw circle outline
void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * i / num_segments;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Draw instruction text
void drawText(const char* text, float x, float y) {
    glColor3f(isDay ? 0.0f : 1.0f, isDay ? 0.0f : 1.0f, isDay ? 0.0f : 1.0f);
    glRasterPos2f(x, y);
    while(*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

// ---- Dynamic Hands (hour, minute only) ----
void drawClockHands(float cx, float cy) {
    // Compute hours & minutes from simulated time
    float totalMinutes = simMinutes;
    float hours   = floor(totalMinutes / 60.0f);
    float minutes = fmod(totalMinutes, 60.0f);

    // Convert to angles
    float hourAngle   = 2.0f * PI * ((hours + minutes/60.0f) / 12.0f) - PI/2;
    float minuteAngle = 2.0f * PI * (minutes / 60.0f) - PI/2;

    // Hour hand
    float hx = 0.08f * cosf(hourAngle);
    float hy = 0.08f * sinf(hourAngle);
    glLineWidth(3.0f);
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
        glVertex2f(cx, cy);
        glVertex2f(cx + hx, cy + hy);
    glEnd();

    // Minute hand
    float mx = 0.12f * cosf(minuteAngle);
    float my = 0.12f * sinf(minuteAngle);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(cx, cy);
        glVertex2f(cx + mx, cy + my);
    glEnd();

    glLineWidth(1.0f); // reset
}

// ---- Tower + Clock ----
void drawTower() {
    // Tower body
    glColor3f(0.5f, 0.35f, 0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.2f, -0.9f);
        glVertex2f(0.2f, -0.9f);
        glVertex2f(0.2f, 0.0f);
        glVertex2f(-0.2f, 0.0f);
    glEnd();

    // Main roof triangle
    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.2f, 0.0f);
        glVertex2f(0.2f, 0.0f);
        glVertex2f(0.0f, 0.3f);
    glEnd();

    // Roof shadow
    glColor3f(0.35f, 0.15f, 0.1f);
    glBegin(GL_POLYGON);
        glVertex2f(0.2f, 0.0f);
        glVertex2f(0.37f, -0.05f);
        glVertex2f(0.24f, 0.17f);
        glVertex2f(0.0f, 0.3f);
    glEnd();

    // Right tower extension
    glColor3f(0.45f, 0.25f, 0.15f);
    glBegin(GL_POLYGON);
        glVertex2f(0.2f, 0.0f);
        glVertex2f(0.37f, -0.05f);
        glVertex2f(0.37f, -0.8f);
        glVertex2f(0.2f, -0.9f);
    glEnd();

    // Clock face
    float clockCx = 0.0f;
    float clockCy = -0.35f;
    float clockR  = 0.15f;
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(clockCx, clockCy, clockR, 100);

    // Hour ticks
    glBegin(GL_LINES);
    for(int i = 0; i < 12; i++){
        float angle = 2.0f * PI * i / 12.0f;
        float x1 = 0.12f * cosf(angle);
        float y1 = 0.12f * sinf(angle);
        float x2 = 0.15f * cosf(angle);
        float y2 = 0.15f * sinf(angle);
        glVertex2f(clockCx + x1, clockCy + y1);
        glVertex2f(clockCx + x2, clockCy + y2);
    }
    glEnd();

    // Dynamic hands
    drawClockHands(clockCx, clockCy);
}

// ---- Timer for smooth fast-forward ----
void timer(int value) {
    simMinutes += speedFactor / 60.0f;  // advance 1 simulated second each tick

    if (simMinutes >= 720.0f) simMinutes -= 720.0f; // loop every 12 hrs

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 60 FPS
}

// ---- Sun, Moon, Clouds, Stars ----
void drawSun() { glColor3f(1,1,0); drawFilledCircle(0.7f,0.7f,0.1f,30); }
void drawMoon() { 
    glColor3f(1,1,0.8f); drawFilledCircle(-0.7f,0.7f,0.08f,30);
    glColor3f(0.05f,0.05f,0.2f); drawFilledCircle(-0.72f,0.72f,0.08f,30);
}
void drawCloud(float cx, float cy) {
    glColor3f(1,1,1);
    drawFilledCircle(cx, cy, 0.08f, 20);
    drawFilledCircle(cx+0.06f, cy+0.03f, 0.06f, 20);
    drawFilledCircle(cx-0.06f, cy+0.02f, 0.07f, 20);
}
void drawStars() {
    glColor3f(1,1,1);
    for(int i=0;i<50;i++){
        float x = (rand()%2000)/1000.0f - 1.0f;
        float y = (rand()%1000)/500.0f - 1.0f;
        glBegin(GL_POINTS); glVertex2f(x,y); glEnd();
    }
}

// ---- Display ----
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if(isDay){
        glColor3f(0.5f,0.8f,1.0f);
        glBegin(GL_QUADS);
            glVertex2f(-1,-1); glVertex2f(1,-1);
            glVertex2f(1,1); glVertex2f(-1,1);
        glEnd();
        drawSun();
        drawCloud(-0.5f,0.8f);
        drawCloud(0.3f,0.75f);
    } else {
        glColor3f(0.05f,0.05f,0.2f);
        glBegin(GL_QUADS);
            glVertex2f(-1,-1); glVertex2f(1,-1);
            glVertex2f(1,1); glVertex2f(-1,1);
        glEnd();
        drawStars();
        drawMoon();
        drawCloud(-0.3f,0.7f);
        drawCloud(0.5f,0.75f);
    }

    drawTower();
    drawText("Press Spacebar for Day or Night", -0.95f, -0.95f);

    glutSwapBuffers();
}

// ---- Keyboard toggle ----
void keyboard(unsigned char key,int,int){
    if(key==' ') { isDay=!isDay; glutPostRedisplay(); }
}

// ---- Reshape ----
void reshape(int w,int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ---- Main ----
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(800,600);
    glutCreateWindow("Clock Tower Day/Night Fast-Forward");

    glClearColor(0.5f,0.8f,1.0f,1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(0, timer, 0); // start fast-forward

    glutMainLoop();
    return 0;
}
