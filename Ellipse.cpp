#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

using namespace std;

using namespace std;

struct Point {
    float x, y;
};

//float slope_dis;
float pt_x, pt_y;
vector<Point> points;
vector<std::pair<Point, Point>> ellipses;
float a;
float b;
float xc;
float yc;

float scaleFactor_elip = 1.0f;

void specialKeyboard(int key, int x, int y) {
    cout << "Special key pressed: " << key << endl;  // Add this line

    switch (key) {
    case GLUT_KEY_UP:
        scaleFactor_elip *= 1.1f; // Increase size
        break;
    case GLUT_KEY_DOWN:
        scaleFactor_elip /= 1.1f; // Decrease size
        break;
    }

    glutPostRedisplay();
}


//void bresenhams_line(Point p1, Point p2) {
//    float dx = abs(p2.x - p1.x);
//    float dy = abs(p2.y - p1.y);
//
//    float x = p1.x;
//    float y = p1.y;
//
//    float sx = (p1.x < p2.x) ? 0.01 : -0.01;  
//    float sy = (p1.y < p2.y) ? 0.01 : -0.01;  
//
//    bool steep = dy > dx;  // determine if the line is steep
//    if (steep) {
//        std::swap(dx, dy);  // swapping
//    }
//
//    int p = 2 * dy - dx;  // decision parameter
//
//    glPointSize(2.0f);
//    glBegin(GL_POINTS);
//
//    for (float i = 0; i <= dx; i += 0.01) {
//        glVertex2f(x, y);
//
//        while (p >= 0) {
//            if (steep) {
//                x += sx;  // increment x if steep
//            }
//            else {
//                y += sy;  // increment y if not steep
//            }
//            p -= 2 * dx;
//        }
//
//        if (steep) {
//            y += sy;  // increment y if steep
//        }
//        else {
//            x += sx;  // increment x if not steep
//        }
//        p += 2 * dy;
//    }
//
//    glEnd();
//    glFlush();
//}

//void DDA_Line(Point p1, Point p2) {
//    float dx = p2.x - p1.x;
//    float dy = p2.y - p1.y;
//
//    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
//    steps = steps * 100;
//
//    float Xinc = dx / (float)steps;
//    float Yinc = dy / (float)steps;
//
//    float x = p1.x;
//    float y = p1.y;
//
//    glPointSize(2.0f);
//    glBegin(GL_POINTS);
//    for (int i = 0; i <= steps; i++) {
//        glVertex2f(x, y);
//        x += Xinc;
//        y += Yinc;
//    }
//    glEnd();
//    glFlush();
//}
void drawEllipse(Point p_center, Point p_max) {
    xc = p_center.x;  // Center X     
    yc = p_center.y;  // Center Y      

    a = abs(p_max.x - p_center.x) * scaleFactor_elip;  // Semi-major axis     
    b = abs(p_max.y - p_center.y) * scaleFactor_elip;  // Semi-minor axis      

    cout << "Drawing ellipse with center (" << xc << ", " << yc << "), a = " << a << ", b = " << b << endl;

    float x = 0;
    float y = b;

    // Decision parameter for region 1     
    float p1 = (b * b) - (a * a * b) + (0.25 * a * a);

    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(5.0);
    glBegin(GL_POINTS);

    int pointCount = 0;

    // Region 1
    while ((b * b * x) < (a * a * y)) {
        glVertex2f(xc + x, yc + y);
        glVertex2f(xc - x, yc + y);
        glVertex2f(xc + x, yc - y);
        glVertex2f(xc - x, yc - y);
        pointCount += 4;

        if (p1 <= 0) {
            x++;
            p1 += (2 * b * b * x) + (b * b);
        }
        else {
            x++;
            y--;
            p1 += (2 * b * b * x) - (2 * a * a * y) + (b * b);
        }

        glEnd();
        glFlush();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ms delay
        glBegin(GL_POINTS);
    }

    // decision parameter   
    float p2 = ((b * b) * ((x + 0.5) * (x + 0.5))) + ((a * a) * ((y - 1) * (y - 1))) - (a * a * b * b);

    // Region 2
    while (y >= 0) { 
        glVertex2f(xc + x, yc + y);
        glVertex2f(xc - x, yc + y);
        glVertex2f(xc + x, yc - y);
        glVertex2f(xc - x, yc - y);
        pointCount += 4;

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

    cout << "Drew " << pointCount << " points for the ellipse." << endl;
}


void graph_paper() {
    glBegin(GL_LINES);
    int j = 100;
    glColor3f(0.0f, 0.8f, 0.0f);

    for (int i = -100; i <= 0; i += 10) {
        if (i == 0) {
            glColor3f(0.0f, 0.0f, 0.0f);

            glVertex2f(0, -100);
            glVertex2f(0, 100);

            glVertex2f(100, 0);
            glVertex2f(-100, 0);
        }

        glVertex2f(i, -j);
        glVertex2f(i, j);

        glVertex2f(-i, -j);
        glVertex2f(-i, j);

        glVertex2f(-j, i);
        glVertex2f(j, i);

        glVertex2f(-j, -i);
        glVertex2f(j, -i);
    }
    glEnd();
}

void points_coord(float x, float y) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    graph_paper();

    // draw points
    for (const auto& point : points) {
        points_coord(point.x, point.y);
    }

    for (const auto& ellipse : ellipses) {
        drawEllipse(ellipse.first, ellipse.second);
    }

    // draw ellipse if two points
    if (points.size() == 2) {
        drawEllipse(points[0], points[1]);
        ellipses.push_back({ points[0], points[1] });
    }


    glColor3f(0.0f, 0.0f, 0.0f);
    std::string name = "Sanskar Rathore, 500108014";
    glRasterPos2f(-90.0f, 90.0f);
    for (char c : name) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    std::ostringstream oss1;
    oss1 << a;
    std::string a_ ="a: " +  oss1.str();
    glRasterPos2f(-90.0f, 70.0f);
    for (char c : a_) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    std::ostringstream oss2;
    oss2 << b;
    std::string b_ = "b: " + oss2.str();
    glRasterPos2f(-90.0f, 65.0f);
    for (char c : b_) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    std::ostringstream oss3;
    std::ostringstream oss4;
    oss4 << yc;
    oss3 << xc;
    std::string xc_ = "Center: (" + oss3.str() + "," + oss4.str() +")";
    glRasterPos2f(-90.0f, 80.0f);
    for (char c : xc_) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

   
   /* std::string yc_ = "CenterY: " + oss4.str();
    glRasterPos2f(-90.0f, 60.0f);
    for (char c : yc_) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }*/


    //glutSwapBuffers();
    glFlush();
}


//void mouse(int button, int state, int x, int y) {
//    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
//        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
//
//        float x_pos = ((x / (float)windowWidth) * 200) - 100;
//        float y_pos = (((windowHeight - y) / (float)windowHeight) * 200) - 100;
//        pt_x = x_pos;
//        pt_y = y_pos;
//        Point p = { x_pos, y_pos };
//        cout << "Point clicked: (" << p.x << ", " << p.y << ")" << endl;
//        points_coord(p.x, p.y);
//        points.push_back(p);
//
//        if (points.size() == 2) {
//           // lines.push_back({ points[0], points[1] });
//         //   calc_slope(points[0], points[1]);
//        ellipse(points[0], points[1]);
//        points.clear(); // reset points after storing the line
//        glutPostRedisplay(); // trigger display to draw the lines
//        }
//    }
//    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
//        lines.clear(); // clear all lines
//        glutPostRedisplay();
//    }
//}
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);

        float x_pos = ((x / (float)windowWidth) * 200) - 100;
        float y_pos = (((windowHeight - y) / (float)windowHeight) * 200) - 100;
        pt_x = x_pos;
        pt_y = y_pos;
        Point p = { x_pos, y_pos };
        cout << "Point clicked: (" << p.x << ", " << p.y << ")" << endl;

        if (points.size() < 2) {
            points.push_back(p);
            
        }
        else {
            // If we already have 2 points, replace them
            ellipses.push_back({ points[0], points[1] });
            points.clear();

            points.push_back(p);
        }

        glutPostRedisplay(); // trigger display to redraw
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        points.clear(); // clear all points
        ellipses.clear();
        glutPostRedisplay();
    }
}
void init() {
    glClearColor(0.9, 1.0, 0.9, 1.0);
    glColor3f(0.0, 0.0, 0.0);

    glPointSize(5.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100.0, 100.0, -100.0, 100.0); // Updated coordinate range set to -100 to 100
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Assignment-5(Ellipse)");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);  // Register the special keyboard function
    glutMainLoop();
    return 0;
}
