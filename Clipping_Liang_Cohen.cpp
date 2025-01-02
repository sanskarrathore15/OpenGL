#include <glut.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

// Structure to represent a line
struct Line {
    float x1, y1, x2, y2;
    bool clipped;
};
 
// input variables for the line endpoints and the clipping window
vector<Line> lines_lb, lines_cs;
float xwmin_lb, ywmin_lb, xwmax_lb, ywmax_lb; // clipping window for Liang-Barsky
float xwmin_cs, ywmin_cs, xwmax_cs, ywmax_cs; // clipping window for Cohen-Sutherland

// state variables
bool windowDrawn_lb = false;
bool windowDrawn_cs = false;
bool lineClipped_lb = false;
bool lineClipped_cs = false;
int clickCount_lb = 0;
int clickCount_cs = 0;
bool windowresize_lb = true;
bool windowresize_cs = true;
bool linedrawn_lb = false;
bool linedrawn_cs = false;


bool spacePressed = true;

//void drawWindow();
void drawLine(float x0, float y0, float x1, float y1, bool clipped);

bool liangBarsky(float x1, float y1, float x2, float y2, float& xx1, float& yy1, float& xx2, float& yy2);
bool cohenSutherland(float& x1, float& y1, float& x2, float& y2);




// Main window and subwindow IDs
int mainWindow, subWindow1, subWindow2;

// Current active subwindow
int activeWindow = 0;


//void graph_paper(bool isLeft) {
//
//    int j = 1;
//    glColor3f(0.0f, 0.8f, 0.0f);
//
//    for (int i = -1; i <= 0; i += 1) {
//        glLineWidth(1.0f);
//        glBegin(GL_LINES);
//        if (i == 0) {
//            glColor3f(0.0f, 0.0f, 0.0f);
//            glVertex2f(0, -1);
//            glVertex2f(0, 1);
//            glVertex2f(1, 0);
//            glVertex2f(-1, 0);
//        }
//
//        //glColor3f(0.0f, 0.8f, 0.0f);
//        glVertex2f(i, -j);
//        glVertex2f(i, j);
//        glVertex2f(-i, -j);
//        glVertex2f(-i, j);
//        glVertex2f(-j, i);
//        glVertex2f(j, i);
//        glVertex2f(-j, -i);
//        glVertex2f(j, -i);
//        glEnd();
//        glLineWidth(5.0f);
//        glBegin(GL_LINES);
//        if (i == -1) {
//
//            glColor3f(0.0f, 0.0f, 0.0f);
//            /* glVertex2f(100 - 10, 100);
//             glVertex2f(100 - 10, -100);
//             glVertex2f(100 + 10, 100);
//             glVertex2f(100 + 10, -100);*/
//            glVertex2f(1 - .2, 1);
//            glVertex2f(1 - .2, -1);
//            glVertex2f(1 + .2, 1);
//            glVertex2f(1 + .2, -1);
//            glColor3f(0.0f, 0.8f, 0.0f);
//        }
//        glEnd();
//
//    }
//
//}

//void points_coord(float x, float y) {  //for points render
//    glColor3f(0.0f, 0.0f, 0.0f);
//    glPointSize(3);
//    glBegin(GL_POINTS);
//    glVertex2f(x, y);
//    glEnd();
//}

//// Function to draw the clipping window
//void drawWindow() {
//    if (windowDrawn) {
//        glColor3f(0.0, 0.0, 0.0); // Black color for the clipping window boundary
//        glBegin(GL_LINE_LOOP);
//        glVertex2f(xwmin, ywmin);
//        glVertex2f(xwmin, ywmax);
//        glVertex2f(xwmax, ywmax);
//        glVertex2f(xwmax, ywmin);
//        glEnd();
//    }
//}


void specialKeyboard(int key, int x, int y) {
    float resizeFactor = .05f;

    if (activeWindow == subWindow1) {
        
        switch (key) {
        case GLUT_KEY_UP:
            ywmax_lb += resizeFactor;
            ywmin_lb += resizeFactor;
            cout << "leftWindow";
            break;
        case GLUT_KEY_DOWN:
            ywmax_lb -= resizeFactor;
            ywmin_lb -= resizeFactor;
            break;
        case GLUT_KEY_RIGHT:
            xwmax_lb += resizeFactor;
            xwmin_lb += resizeFactor;
            break;
        case GLUT_KEY_LEFT:
            xwmax_lb -= resizeFactor;
            xwmin_lb -= resizeFactor;
            break;
        }
    }
    else if (activeWindow == subWindow2) {
        switch (key) {
        case GLUT_KEY_UP:
            ywmax_cs += resizeFactor;
            ywmin_cs += resizeFactor;
            break;
        case GLUT_KEY_DOWN:
            ywmax_cs -= resizeFactor;
            ywmin_cs -= resizeFactor;
            break;
        case GLUT_KEY_RIGHT:
            xwmax_cs += resizeFactor;
            xwmin_cs += resizeFactor;
            break;
        case GLUT_KEY_LEFT:
            xwmax_cs -= resizeFactor;
            xwmin_cs -= resizeFactor;
            break;
        }
    }

    glutPostRedisplay();
}
// Function to draw a line
void drawLine(float x0, float y0, float x1, float y1, bool clipped) {
    
    if (spacePressed) {
        //createLine = true;
       // if (createLine)
       // if (activeWindow == subWindow1) {
            glColor3f(clipped ? 1.0 : 0.0, clipped ? 0.0 : 1.0, 0.0); // Red for clipped part, Green for visible part
       // }
      //  else {
          //  glColor3f(clipped ? 1.0 : 0.0, clipped ? 0.0 : 1.0, 0.0); // Red for clipped part, Green for visible part
      //  }
    }

    else {

        if (activeWindow == subWindow1) {
            if ((y1 < ywmin_lb && y0 < ywmin_lb) || (y1 > ywmax_lb && y0 > ywmax_lb) || (x1 < xwmin_lb && x0 < xwmin_lb) || (x1 > xwmax_lb && x0 > xwmax_lb)) //(y1 > ywmin && y0 < ywmax) || (y1 < ywmax && y0 > ywmax) || (x1>xwmax && x0<xwmax) || (x1>xwmin && x0<xwmin)
            {
                // glColor3f(0.0, 0.0, 1.0);
                glColor3f(0.0, 0.0, 0.0);
            }
            else
                // glColor3f(1.0, 1.0, 1.0);
                glColor3f(0.0, 0.0, 1.0);// Blue for default line

        }
        else {
            if ((y1 < ywmin_cs && y0 < ywmin_cs) || (y1 > ywmax_cs && y0 > ywmax_cs) || (x1 < xwmin_cs && x0 < xwmin_cs) || (x1 > xwmax_cs && x0 > xwmax_cs)) //(y1 > ywmin && y0 < ywmax) || (y1 < ywmax && y0 > ywmax) || (x1>xwmax && x0<xwmax) || (x1>xwmin && x0<xwmin)
            {
                // glColor3f(0.0, 0.0, 1.0);
                glColor3f(0.0, 0.0, 0.0);
            }
            else
                // glColor3f(1.0, 1.0, 1.0);
                glColor3f(0.0, 0.0, 1.0);
        }
    }

    glBegin(GL_LINES);
    glVertex2f(x0, y0);
    glVertex2f(x1, y1);
    glEnd();
}


// Define region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Function to compute 4 bit region code for a point (x, y)
int computeCode(float x, float y) {
    int code = INSIDE;
    if (x < xwmin_cs)
        code |= LEFT;
    else if (x > xwmax_cs)
        code |= RIGHT;
    if (y < ywmin_cs)
        code |= BOTTOM;
    else if (y > ywmax_cs)
        code |= TOP;
    return code;
}

// cohen-sutherland line clipping algorithm
bool cohenSutherland(float& x1, float& y1, float& x2, float& y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true) {
        if (!(code1 | code2)) {
            // both endpoints inside the clip window
            accept = true;
            break;
        }
        else if (code1 & code2) {
            // both endpoints outside the clip window
            break;
        }
        else {
            // line needs clipping(partially visibler)
            int codeOut = code1 ? code1 : code2; //pt. outside clipping win is chosen
            float x, y;

            if (codeOut & TOP) {
                x = x1 + (x2 - x1) * (ywmax_cs - y1) / (y2 - y1);
                y = ywmax_cs;
            }
            else if (codeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (ywmin_cs - y1) / (y2 - y1);
                y = ywmin_cs;
            }
            else if (codeOut & RIGHT) {
                y = y1 + (y2 - y1) * (xwmax_cs - x1) / (x2 - x1);
                x = xwmax_cs;
            }
            else if (codeOut & LEFT) {
                y = y1 + (y2 - y1) * (xwmin_cs - x1) / (x2 - x1);
                x = xwmin_cs;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    return accept;
}


bool liangBarsky(float x1, float y1, float x2, float y2, float& xx1, float& yy1, float& xx2, float& yy2) {
    float t1 = 0.0, t2 = 1.0;
    float dx = x2 - x1;
    float dy = y2 - y1;

    float p[4] = { -dx, dx, -dy, dy };
    float q[4] = { x1 - xwmin_lb, xwmax_lb - x1, y1 - ywmin_lb, ywmax_lb - y1 };

    for (int i = 0; i < 4; i++) {
        if (abs(p[i]) < 1e-3) {
            if (q[i] < 0) return false;
        }
        else {
            float r = q[i] / p[i];
            if (p[i] < 0) {
                t1 = max(t1, r);
            }
            else {
                t2 = min(t2, r);
            }
        }
    }

    if (t1 < t2) {
        xx1 = x1 + t1 * dx;
        yy1 = y1 + t1 * dy;
        xx2 = x1 + t2 * dx;
        yy2 = y1 + t2 * dy;
        return true;
    }

    return false;
}

//bool createLine = false;

// Display function to render the scene
void display1() {
    glClear(GL_COLOR_BUFFER_BIT);



    for (float i = 1; i >= -1; i -= .1) {
        glColor3f(0.0, 0.9, 0.0);
        glLineWidth(1.0f);
        glBegin(GL_LINES);

        glVertex2f(i, 1);
        glVertex2f(i, -1);

        glVertex2f(-1, i);
        glVertex2f(1, i);

    }
    glEnd();

    glLineWidth(5.0f);

    glColor3f(1.0, 1.0, 1.0);
   // drawWindow();

    if (windowDrawn_lb) {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(xwmin_lb, ywmin_lb);
        glVertex2f(xwmin_lb, ywmax_lb);
        glVertex2f(xwmax_lb, ywmax_lb);
        glVertex2f(xwmax_lb, ywmin_lb);
        glEnd();
    }

    for (const auto& line : lines_lb) {
        if (windowDrawn_lb && lineClipped_lb) {
            float clippedX1, clippedY1, clippedX2, clippedY2;
            if (liangBarsky(line.x1, line.y1, line.x2, line.y2, clippedX1, clippedY1, clippedX2, clippedY2)) {
                
                drawLine(clippedX1, clippedY1, clippedX2, clippedY2, false); //false for green


                if (spacePressed) {
                    drawLine(line.x1, line.y1, clippedX1, clippedY1, true);
                    drawLine(clippedX2, clippedY2, line.x2, line.y2, true);

                }
                
               // drawLine(clippedX1, clippedY1, clippedX2, clippedY2, false);
            }
            else if (spacePressed) {
                drawLine(line.x1, line.y1, line.x2, line.y2, true);
            }
        }
        else {
            // Draw original line
            drawLine(line.x1, line.y1, line.x2, line.y2, false); //
        }
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    string title = "Liang Barsky";
    glRasterPos2f(-.100f, .9f);
    for (char c : title) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }


    glColor3f(0.5f, 0.5f, 0.5f);
    string inst3 = "Press 1 : To Activate left win";
    glRasterPos2f(-.900f, -.66f);
    for (char c : inst3) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glColor3f(0.5f, 0.5f, 0.5f);
    string inst4 = "Press 2 : To Activate right win";
    glRasterPos2f(-.900f, -.72f);
    for (char c : inst4) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }


    glColor3f(0.5f, 0.5f, 0.5f);
    string inst1 = "Press L : To Clear Lines";
    glRasterPos2f(-.900f, -.78f);
    for (char c : inst1) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glColor3f(0.5f, 0.5f, 0.5f);
    string inst2 = "Press C : To Clear Clipping win";
    glRasterPos2f(-.900f, -.84f);
    for (char c : inst2) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glColor3f(0.5f, 0.5f, 0.5f);
    string inst5 = "Press F : To Fix/Change Window size";
    glRasterPos2f(-.900f, -.9f);
    for (char c : inst5) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glColor3f(0.5f, 0.5f, 0.5f);
    string inst6 = "Press Space : View change";
    glRasterPos2f(-.900f, -.96f);
    for (char c : inst6) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }


    glutSwapBuffers();
}


void display2() {
    glClear(GL_COLOR_BUFFER_BIT);


    for (float i = 1; i >= -1; i -= .1) {
        glColor3f(0.0, 0.9, 0.0);
        glLineWidth(1.0f);
        glBegin(GL_LINES);

        glVertex2f(i, 1);
        glVertex2f(i, -1);

        glVertex2f(-1, i);
        glVertex2f(1, i);

    }
    glEnd();
  
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(5.0f);
    glBegin(GL_LINES);

    glVertex2f(-1, -1);
    glVertex2f(-1, 1);
    glEnd();


    glColor3f(1.0, 1.0, 1.0);
   // drawWindow();
    if (windowDrawn_cs) {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(xwmin_cs, ywmin_cs);
        glVertex2f(xwmin_cs, ywmax_cs);
        glVertex2f(xwmax_cs, ywmax_cs);
        glVertex2f(xwmax_cs, ywmin_cs);
        glEnd();
    }

    for (const auto& line : lines_cs) {



            float clippedX1 = line.x1, clippedY1 = line.y1, clippedX2 = line.x2, clippedY2 = line.y2;
        if (windowDrawn_cs && lineClipped_cs) {
            if (cohenSutherland(clippedX1, clippedY1, clippedX2, clippedY2)) {
                drawLine(clippedX1, clippedY1, clippedX2, clippedY2, false);
                
                if (spacePressed) {
                    drawLine(line.x1, line.y1, clippedX1, clippedY1, true);
                    drawLine(clippedX2, clippedY2, line.x2, line.y2, true);
                }
               // drawLine(clippedX1, clippedY1, clippedX2, clippedY2, false);
            }
            else if (spacePressed) {
                drawLine(line.x1, line.y1, line.x2, line.y2, true);
            }
        }

       /* glBegin(GL_LINES);
        glVertex2f(clippedX1, clippedY1);
        glVertex2f(clippedX2, clippedY2);
        glEnd();*/
        else {
            drawLine(line.x1, line.y1, line.x2, line.y2, false);
        }
    }
    

    glColor3f(0.0f, 0.0f, 0.0f);
    string title = "Cohen Sutherland";
    glRasterPos2f(-.100f, .9f);
    for (char c : title) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    string name = "Sanskar Rathore, 500108014";
    glRasterPos2f(.3f, -.95f);
    for (char c : name) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glColor3f(0.5f, 0.5f, 0.5f);
    string inst3 = "Press up : To move up";
    glRasterPos2f(-.900f, -.66f);
    for (char c : inst3) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glColor3f(0.5f, 0.5f, 0.5f);
    string inst4 = "Press down : To move down";
    glRasterPos2f(-.900f, -.72f);
    for (char c : inst4) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }


    glColor3f(0.5f, 0.5f, 0.5f);
    string inst1 = "Press right : To right";
    glRasterPos2f(-.900f, -.78f);
    for (char c : inst1) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glColor3f(0.5f, 0.5f, 0.5f);
    string inst2 = "Press left : To Cleft";
    glRasterPos2f(-.900f, -.84f);
    for (char c : inst2) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glutSwapBuffers();
}
bool check;
// Main display function
void display() {




    if (check == true) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);  // Set background to black
        glColor3f(1.0, 1.0, 1.0);  // Set text color to white

        // Display text
        glRasterPos2f(-0.5, 0.0);
        string message = "Press 's' to start the program";
        for (char c : message) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    else
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);



        glutSetWindow(subWindow1);
        display1();
        glColor3f(1.0, 1.0, 1.0);
        glutSetWindow(subWindow2);
        display2();
        glutSetWindow(mainWindow);
    }

    glutSwapBuffers();
}


// Keyboard function to handle various commands
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 32: // Spacebar
        spacePressed = !spacePressed;
        break;
    case 'c':
    case 'C':
        if (activeWindow == subWindow1) {
            windowDrawn_lb = false;
            lineClipped_lb = false;
            clickCount_lb = 0;
        }
        else if (activeWindow == subWindow2) {
            windowDrawn_cs = false;
            lineClipped_cs = false;
            clickCount_cs = 0;
        }
        break;
    case 'l':
    case 'L':
        if (activeWindow == subWindow1) {
            lines_lb.clear();
            lineClipped_lb = false;
            clickCount_lb = 0;
        }
        else if (activeWindow == subWindow2) {
            lines_cs.clear();
            lineClipped_cs = false;
            clickCount_cs = 0;
        }
            break;
    case 'f':
    case 'F':
        if (activeWindow == subWindow1) {
            windowresize_lb = !windowresize_lb;
            linedrawn_lb = !linedrawn_lb;
            clickCount_lb = 0;
        }
        else if (activeWindow == subWindow2) {
            windowresize_cs = !windowresize_cs;
            linedrawn_cs = !linedrawn_cs;
            clickCount_cs = 0;
        }
        break;
    case '1':
        activeWindow = subWindow1;
        glutSetWindow(activeWindow);
        break;
    case '2':
        activeWindow = subWindow2;
        glutSetWindow(activeWindow);
        break;
    case 's':
    case 'S':
        check = false;
        break;
        
    }
        glutPostRedisplay();
}

// Mouse input function
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        float wx = (float)x / 400 - 1.0;
        float wy = 1.0 - (float)y / 400;


        if (activeWindow == subWindow1) {
        clickCount_lb++;

        if (windowresize_lb && clickCount_lb <= 2) {

            
            if (clickCount_lb == 1) {
                windowDrawn_lb = false;
                lineClipped_lb = false;
                xwmin_lb = wx; ywmin_lb = wy;
            }
            else if (clickCount_lb == 2) {
                xwmax_lb = wx; ywmax_lb = wy;
                if (xwmin_lb > xwmax_lb) swap(xwmin_lb, xwmax_lb);
                if (ywmin_lb > ywmax_lb) swap(ywmin_lb, ywmax_lb);
                windowDrawn_lb = true;
                lineClipped_lb = true;
                clickCount_lb = 0;
            }
        }

        else if (linedrawn_lb) {
            static Line currentLine;
            if (clickCount_lb % 2 == 1) {
                currentLine.x1 = wx;
                currentLine.y1 = wy;
            }
            else {
                currentLine.x2 = wx;
                currentLine.y2 = wy;
                currentLine.clipped = false;
                lines_lb.push_back(currentLine);
              
                    lines_lb.push_back(currentLine);
                
               /* else {
                    lines_cs.push_back(currentLine);
                }*/
                lineClipped_lb = windowDrawn_lb;
            }
            }

        }

        else if (activeWindow == subWindow2) {
            clickCount_cs++;

            if (windowresize_cs && clickCount_cs <= 2) {


                if (clickCount_cs == 1) {
                    windowDrawn_cs = false;
                    lineClipped_cs = false;
                    xwmin_cs = wx; ywmin_cs = wy;
                }
                else if (clickCount_cs == 2) {
                    xwmax_cs = wx; ywmax_cs = wy;
                    if (xwmin_cs > xwmax_cs) swap(xwmin_cs, xwmax_cs);
                    if (ywmin_cs > ywmax_cs) swap(ywmin_cs, ywmax_cs);
                    windowDrawn_cs = true;
                    lineClipped_cs = true;
                    clickCount_cs = 0;
                }
            }

            else if (linedrawn_cs) {
                static Line currentLine;
                if (clickCount_cs % 2 == 1) {
                    currentLine.x1 = wx;
                    currentLine.y1 = wy;
                }
                else {
                    currentLine.x2 = wx;
                    currentLine.y2 = wy;
                    currentLine.clipped = false;
                    lines_cs.push_back(currentLine);

                    lines_cs.push_back(currentLine);

                    /* else {
                         lines_cs.push_back(currentLine);
                     }*/
                    lineClipped_cs = windowDrawn_cs;
                }
            }

        }
    }
    glutPostRedisplay();


    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lines_lb.clear();
        lines_cs.clear();
        windowDrawn_lb = false;
        lineClipped_lb = false;
        clickCount_lb = 0;
        windowDrawn_cs = false;
        lineClipped_cs = false;
        clickCount_cs = 0;
        glutPostRedisplay();
    }
}


// Function to initialize OpenGL settings
void init() {
    glClearColor(.6, 1.0, .7, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1600, 800);
    mainWindow = glutCreateWindow("Line Clipping Algos");
    //graph_paper(true);
 
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);

    // Initialize check to true
    check = true;
    
    subWindow1 = glutCreateSubWindow(mainWindow, 0, 0, 800, 800);
    init();
    glutDisplayFunc(display1);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutSpecialFunc(specialKeyboard);

    subWindow2 = glutCreateSubWindow(mainWindow, 800, 0, 800, 800);
    init();
    glutDisplayFunc(display2);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutSpecialFunc(specialKeyboard);
    activeWindow = subWindow1;
    glutSetWindow(mainWindow);

    

    glutMainLoop();
    return 0;
}