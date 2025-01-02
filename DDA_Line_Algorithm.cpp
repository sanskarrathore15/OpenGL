#include <glut.h>
#include <iostream>
#include <vector>
#include <iomanip>  // for setting precision of slope
#include <cmath>    // for mathematical functions
#include <string>
#include <sstream>

using namespace std;

struct Point {
    float x, y;
};

float slope_dis;
float pt_x, pt_y;
vector<Point> points;
vector<pair<Point, Point>> lines;

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

void DDA_Line(Point p1, Point p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    steps = steps * 100;

    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    float x = p1.x;
    float y = p1.y;

    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2f(x, y);
        x += Xinc;
        y += Yinc;
    }
    glEnd();
    glFlush();
}

void display_s_p() {
    glColor3f(0.0f, 0.0f, 0.0f);
    std::string sl = "Slope: ";
    glRasterPos2f(-90.0f, 80.0f);
    for (char c : sl) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    std::ostringstream oss;
    oss << slope_dis;
    std::string scoreStr = oss.str();
    glRasterPos2f(-75.0f, 80.0f); // Set the position for the text
    for (char c : scoreStr) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Render each character
    }
}

void graph_paper() {
    glBegin(GL_LINES);
    int j = 100;
    glColor3f(0.0f, 0.8f, 0.0f);

    for (int i = -100; i <= 0; i+=10) {
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
    for (const auto& line : lines) {
        /*bresenhams_line(line.first, line.second);*/
        DDA_Line(line.first, line.second);
    }

    points_coord(pt_x, pt_y);

    glColor3f(0.0f, 0.0f, 0.0f);
    std::string name = "Sanskar Rathore, 500108014";
    glRasterPos2f(-90.0f, 90.0f);
    for (char c : name) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    display_s_p();

    glutSwapBuffers();
}

void calc_slope(const Point& p1, const Point& p2) {
    if (p1.x != p2.x) {
        float slope = (p2.y - p1.y) / (p2.x - p1.x);
        float normalized_slope = slope / sqrt(1 + slope * slope);
        slope_dis = normalized_slope;

        float angle = atan(slope) * 180 / 3.14159265;
        float intercept = p1.y - slope * p1.x;

        cout << fixed << setprecision(2);
        cout << "Slope of the latest line: " << slope << endl;
        cout << "Normalized Slope: " << normalized_slope << endl;
        cout << "Angle (in degrees): " << angle << " deg" << endl;
        cout << "Y-Intercept: " << intercept << endl;
    }
    else {
        cout << "Slope is undefined (vertical line)." << endl;
        cout << "Normalized Slope and y intercept: Not applicable (vertical line)." << endl;
        cout << "Angle (in degrees): 90.00" << endl;
    }
}

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
        points_coord(p.x, p.y);
        points.push_back(p);

        if (points.size() == 2) {
            lines.push_back({ points[0], points[1] });
            calc_slope(points[0], points[1]);
            points.clear(); // reset points after storing the line
            glutPostRedisplay(); // trigger display to draw the lines
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lines.clear(); // clear all lines
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Assignment-3(DDA Line Graph)");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
