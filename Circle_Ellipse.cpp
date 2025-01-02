#include <glut.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <string>
#include <sstream>
#include<thread>
#include<chrono>


using namespace std;

float current_a = 0.0f, current_b = 0.0f; // Size of the latest ellipse or circle
bool isLeftShape = true; // True if the latest shape is on the left side


struct Point {
    float x, y;
};


struct Shape {
    Point p1, p2;
    float color[3]; // RGB values
};

float currentColor_elip[3] = { 0.0f, 0.0f, 0.0f }; 
float currentColor_circ[3] = { 0.0f, 0.0f, 0.0f };

int radius;

vector<Point> left_points, right_points;
vector<pair<Point, Point>> left_circle, right_ellipses;

float left_a, left_b, left_xc, left_yc;
float right_a, right_b, right_xc, right_yc;


// for resizing shapes
float scaleFactor_cir = 1.0f;
float scaleFactor_elip = 1.0f;
bool isResizingLeftShape = false;

bool elip = false;
bool cir = false;

void specialKeyboard(int key, int x, int y) {

    if (elip) {
    switch (key) {
    case GLUT_KEY_UP:
        scaleFactor_elip *= 1.1f; // Increase size
        break;
    case GLUT_KEY_DOWN:
        scaleFactor_elip /= 1.1f; // Decrease size
        break;
    }

    }
    else {
        switch (key) {
        case GLUT_KEY_UP:
            scaleFactor_cir *= 1.1f; // Increase size
            break;
        case GLUT_KEY_DOWN:
            scaleFactor_cir /= 1.1f; // Decrease size
            break;
        }
    }

    glutPostRedisplay();
}


void drawCircle(Point p_center, Point p_max) {

    float xc = p_center.x;
    float yc = p_center.y;
  /*  float a = abs(p_max.x - p_center.x) * scaleFactor;
    float b = abs(p_max.y - p_center.y) * scaleFactor;*/

    float rad =sqrt((p_max.x - p_center.x) * (p_max.x - p_center.x) + 
        (p_max.y - p_center.y) * (p_max.y - p_center.y)) * scaleFactor_cir;

    //if (isLeft) {
        left_xc = xc; left_yc = yc; left_a = rad; left_b = rad;
    //}
    /*else {
        right_xc = xc; right_yc = yc; right_a = a; right_b = b;
    }*/

     //   float radius = sqrt((p_max.x - p_center.x) * (p_max.x - p_center.x) +
  //          (p_max.y - p_center.y) * (p_max.y - p_center.y));

        // Initialize Midpoint Circle Algorithm variables
        int r = static_cast<int>(rad); // Use the calculated radius
        int x = 0;
        int y = r;
        int p = 1 - r; // Decision parameter

    glColor3f(currentColor_circ[0], currentColor_circ[1], currentColor_circ[2]);

    glPointSize(5.0);
    glBegin(GL_POINTS);

    while (y>=x) {
        glVertex2f(xc + x, yc + y);
        glVertex2f(xc - x, yc + y);
        glVertex2f(xc + x, yc - y);
        glVertex2f(xc - x, yc - y);

        glVertex2f(xc + y, yc + x);
        glVertex2f(xc - y, yc + x);
        glVertex2f(xc + y, yc - x);
        glVertex2f(xc - y, yc - x);

        x++;

        if (p <= 0) {            
            p += 2 * x + 1;;
        }

        else {
            
            y--;
            p += 2*x - 2*y + 1;
        }
    }

   /* float p2 = ((b * b) * ((x + 0.5) * (x + 0.5))) + ((a * a) * ((y - 1) * (y - 1))) - (a * a * b * b);

    while (y >= 0) {
        glVertex2f(xc + x, yc + y);
        glVertex2f(xc - x, yc + y);
        glVertex2f(xc + x, yc - y);
        glVertex2f(xc - x, yc - y);

        if (p2 >= 0) {
            y--;
            p2 += (a * a) - (2 * a * a * y);
        }
        else {
            x++;
            y--;
            p2 += (2 * b * b * x) - (2 * a * a * y) + (a * a);
        }
    }*/

    glEnd();
    glFlush();
}




void drawEllipse(Point p_center, Point p_max) {
    float xc = p_center.x;
    float yc = p_center.y;
    float a = abs(p_max.x - p_center.x) * scaleFactor_elip;
    float b = abs(p_max.y - p_center.y) * scaleFactor_elip;

    /*if (isLeft) {
        left_xc = xc; left_yc = yc; left_a = a; left_b = b;
    }*/
 //   else {
        right_xc = xc; right_yc = yc; right_a = a; right_b = b;
   // }

    float x = 0;
    float y = b;
    float p1 = (b * b) - (a * a * b) + (0.25 * a * a);

    glColor3f(currentColor_elip[0], currentColor_elip[1], currentColor_elip[2]);
    glPointSize(5.0);
    glBegin(GL_POINTS);

    while ((b * b * x) < (a * a * y)) {
        glVertex2f(xc + x, yc + y);
        glVertex2f(xc - x, yc + y);
        glVertex2f(xc + x, yc - y);
        glVertex2f(xc - x, yc - y);

        if (p1 <= 0) {
            x++;
            p1 += (2 * b * b * x) + (b * b);
        }
        else {
            x++;
            y--;
            p1 += (2 * b * b * x) - (2 * a * a * y) + (b * b);
        }
    }

    float p2 = ((b * b) * ((x + 0.5) * (x + 0.5))) + ((a * a) * ((y - 1) * (y - 1))) - (a * a * b * b);

    while (y >= 0) {
        glVertex2f(xc + x, yc + y);
        glVertex2f(xc - x, yc + y);
        glVertex2f(xc + x, yc - y);
        glVertex2f(xc - x, yc - y);

        if (p2 >= 0) {
            y--;
            p2 += (a * a) - (2 * a * a * y);
        }
        else {
            x++;
            y--;
            p2 += (2 * b * b * x) - (2 * a * a * y) + (a * a);
        }
    }

    glEnd();
    glFlush();
}

bool fillCircle = false;
bool fillEllipse = false;

void drawAndFillCircle(Point p_center, Point p_max) {
    float xc = p_center.x;
    float yc = p_center.y;
    float rad = sqrt((p_max.x - p_center.x) * (p_max.x - p_center.x) +
        (p_max.y - p_center.y) * (p_max.y - p_center.y)) * scaleFactor_cir;

    // Draw the outline
    glColor3f(currentColor_circ[0], currentColor_circ[1], currentColor_circ[2]);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    // ... (existing circle drawing code)
    glEnd();

    // Fill the circle if fillCircle is true
    if (fillCircle) {
        glColor3f(0.5f, 0.5f, 0.5f);  // Fill color (gray)
        glBegin(GL_LINES);
        glVertex2f(xc, yc);  // Center point
        for (float angle = 0; angle <= 360; angle += 5) {
            float x = xc + rad * cos(angle * 3.14159 / 180);
            float y = yc + rad * sin(angle * 3.14159 / 180);
            glVertex2f(x, y);


          //  std::this_thread::sleep_for(std::chrono::milliseconds(10));
            //Sleep(50);   //50 milliseconds

        }
            glEnd();
            glFlush();   //drawing is updated
       
    }
}

void drawAndFillEllipse(Point p_center, Point p_max) {
    float xc = p_center.x;
    float yc = p_center.y;
    float a = abs(p_max.x - p_center.x) * scaleFactor_elip;
    float b = abs(p_max.y - p_center.y) * scaleFactor_elip;

    // Draw the outline
    glColor3f(currentColor_elip[0], currentColor_elip[1], currentColor_elip[2]);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    // ... (existing ellipse drawing code)
    glEnd();

    // Fill the ellipse if fillEllipse is true
    if (fillEllipse) {
        glColor3f(0.5f, 0.5f, 0.5f);  // Fill color (gray)
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(xc, yc);  // Center point
        for (float angle = 0; angle <= 360; angle += 5) {
            float x = xc + a * cos(angle * 3.14159 / 180);
            float y = yc + b * sin(angle * 3.14159 / 180);
            glVertex2f(x, y);

           // Sleep(50);   //50 milliseconds
           // glFlush();   //drawing is updated
        }
        glEnd();
    }
}


void keyboard(unsigned char key, int x, int y) {

   

    //if (elip) {

    //   

    //    switch (key) {
    //    case 'r':  // red
    //        currentColor_elip[0] = 1.0f;
    //        currentColor_elip[1] = 0.0f;
    //        currentColor_elip[2] = 0.0f;
    //        break;
    //    case 'g':  // green
    //        currentColor_elip[0] = 0.0f;
    //        currentColor_elip[1] = 1.0f;
    //        currentColor_elip[2] = 0.0f;
    //        break;
    //    case 'b':  // blue
    //        currentColor_elip[0] = 0.0f;
    //        currentColor_elip[1] = 0.0f;
    //        currentColor_elip[2] = 1.0f;
    //        break;
    //    case 'i':  // black 
    //        currentColor_elip[0] = 0.0f;
    //        currentColor_elip[1] = 0.0f;
    //        currentColor_elip[2] = 0.0f;
    //        break;
    //    case 'f': 
    //     //   drawFilledEllipse(right_points[0], left_points[1]);
    //        break;

    //    }
    //}
    //else {
    //    switch (key) {
    //    case 'r':  // red
    //        currentColor_circ[0] = 1.0f;
    //        currentColor_circ[1] = 0.0f;
    //        currentColor_circ[2] = 0.0f;
    //        break;
    //    case 'g':  // green
    //        currentColor_circ[0] = 0.0f;
    //        currentColor_circ[1] = 1.0f;
    //        currentColor_circ[2] = 0.0f;
    //        break;
    //    case 'b':  // blue
    //        currentColor_circ[0] = 0.0f;
    //        currentColor_circ[1] = 0.0f;
    //        currentColor_circ[2] = 1.0f;
    //        break;
    //    case 'i':  // black 
    //        currentColor_circ[0] = 0.0f;
    //        currentColor_circ[1] = 0.0f;
    //        currentColor_circ[2] = 0.0f;
    //        break;
    //    case 'f':
    //   //     scanLineFill();
    // //       drawFilledCircle(left_points[0], left_points[1]);
    //        break;

    //    }
    //}
  
    switch (key) {
    case 'f':
    case 'F':
        if (elip) {
            fillEllipse = !fillEllipse;
        }
        else {
            fillCircle = !fillCircle;
        }
        break;
        // ... (rest of the keyboard function remains the same)
    }
    glutPostRedisplay(); 
}


void graph_paper(bool isLeft) {
    
    int j = 100;
    glColor3f(0.0f, 0.8f, 0.0f);

    for (int i = -100; i <= 0; i += 10) {
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        if (i == 0) {
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex2f(0, -100);
            glVertex2f(0, 100);
            glVertex2f(100, 0);
            glVertex2f(-100, 0);
        }
      
        //glColor3f(0.0f, 0.8f, 0.0f);
        glVertex2f(i, -j);
        glVertex2f(i, j);
        glVertex2f(-i, -j);
        glVertex2f(-i, j);
        glVertex2f(-j, i);
        glVertex2f(j, i);
        glVertex2f(-j, -i);
        glVertex2f(j, -i);
        glEnd();
        glLineWidth(5.0f);
        glBegin(GL_LINES);
            if (i == -100) {
               
            glColor3f(0.0f, 0.0f, 0.0f);
           /* glVertex2f(100 - 10, 100);
            glVertex2f(100 - 10, -100);
            glVertex2f(100 + 10, 100);
            glVertex2f(100 + 10, -100);*/
            glVertex2f(100 - .2, 100);
            glVertex2f(100 - .2, -100);
            glVertex2f(100 + .2, 100);
            glVertex2f(100 + .2, -100);
            glColor3f(0.0f, 0.8f, 0.0f);
        }
            glEnd();

    }
  
}

void points_coord(float x, float y) {  //for points render
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Left half
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //scanLineFill();
    graph_paper(true);
    for (const auto& point : left_points) {
        points_coord(point.x, point.y);
    }
    for (const auto& ellipse : left_circle) {
        drawCircle(ellipse.first, ellipse.second);
    }
    if (left_points.size() == 2) {
        
        drawCircle(left_points[0], left_points[1]);
        drawAndFillCircle(left_points[0], left_points[1]);
    }

    // Right half
    glViewport(glutGet(GLUT_WINDOW_WIDTH) / 2, 0, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    graph_paper(false);
    for (const auto& point : right_points) {
        points_coord(point.x, point.y);
    }
    for (const auto& ellipse : right_ellipses) {
        drawEllipse(ellipse.first, ellipse.second);
    }
    if (right_points.size() == 2) {
       
        drawEllipse(right_points[0], right_points[1]);
        drawAndFillEllipse(right_points[0], right_points[1]);
    }

    // Display info
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.0f, 0.0f, 0.0f);
    string name = "Sanskar Rathore, 500108014";
    glRasterPos2f(-90.0f, 90.0f);
    for (char c : name) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    string circle = "Circle";
    glRasterPos2f(-52.0f, -90.0f);
    for (char c : circle) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    string ellipse = "Ellipse";
    glRasterPos2f(48.0f, -90.0f);
    for (char c : ellipse) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    ostringstream oss;
    oss << "Radius: " << left_a << ", Center=(" << left_xc << "," << left_yc << ")" << ", Scale: " << scaleFactor_cir;
    string left_info = oss.str();
    glRasterPos2f(-90.0f, 80.0f);
    for (char c : left_info) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    oss.str("");
    oss << "Right: a=" << right_a << ", b=" << right_b << ", Center=(" << right_xc << "," << right_yc << ")" <<" , Scale: " << scaleFactor_elip;
    string right_info = oss.str();
    glRasterPos2f(10.0f, 80.0f);
    for (char c : right_info) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);


       /* if (windowWidth >= -91.0f && windowWidth <= -85.0f && windowHeight >= -90.0f && windowHeight <= -92.0f) {
            currentColor[0] = 1.0f;
            currentColor[1] = 0.0f;
            currentColor[2] = 0.0f;
        }
        else if (windowWidth >= -91.0f && windowWidth <= -85.0f && windowHeight >= -86.0f && windowHeight <= -88.0f) {
            currentColor[0] = 1.0f;
            currentColor[1] = 0.0f;
            currentColor[2] = 0.0f;
        }
        else if (windowWidth >= -91.0f && windowWidth <= -85.0f && windowHeight >= -82.0f && windowHeight <= -84.0f) {
            currentColor[0] = 1.0f;
            currentColor[1] = 0.0f;
            currentColor[2] = 0.0f;
        }*/


        float x_pos, y_pos;
        if (x < windowWidth / 2) {  // Left half
            x_pos = ((x / (float)(windowWidth / 2)) * 200) - 100;
            y_pos = (((windowHeight - y) / (float)windowHeight) * 200) - 100;

            Point p = { x_pos, y_pos };
            cout << "Left point clicked: (" << p.x << ", " << p.y << ")" << endl;

            if (left_points.size() < 2) {
                elip = false;
              //  cir = true;
                left_points.push_back(p);
            }
            else {
              
                left_circle.push_back({ left_points[0], left_points[1] });
                //scaleFactor_cir = 1.0f;
                left_points.clear();
                left_points.push_back(p);
            }
        }
        else {  // Right half
            x_pos = (((x - windowWidth / 2) / (float)(windowWidth / 2)) * 200) - 100;
            y_pos = (((windowHeight - y) / (float)windowHeight) * 200) - 100;

            Point p = { x_pos, y_pos };
            cout << "Right point clicked: (" << p.x << ", " << p.y << ")" << endl;

            if (right_points.size() < 2) {
                elip = true;
               // cir = false;
                right_points.push_back(p);
            }
            else {

       

                right_ellipses.push_back({ right_points[0], right_points[1] });
              //  scaleFactor_elip = 1.0f;
                right_points.clear();
                right_points.push_back(p);
            }
        }

        glutPostRedisplay();
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        left_points.clear();
        right_points.clear();
        left_circle.clear();
        right_ellipses.clear();
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(0.9, 1.0, 0.9, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1600, 1600);
    glutCreateWindow("Split Screen Ellipse Renderer");
  glutFullScreen();
    init();
    glutDisplayFunc(display);

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);

    glutMainLoop();
    return 0;
}