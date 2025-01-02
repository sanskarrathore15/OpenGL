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

void bresenhams_line(Point p1, Point p2) {
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);

    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;

    int p; // decision parameter
    int x = x1, y = y1; 

    int xInc = (x2 > x1) ? 1 : -1; //based on direction of the line 
    int yInc = (y2 > y1) ? 1 : -1; //based on direction of the line 


    //1 > m > 0,  left to right , xInc = 1, yInc = 1;      
    //1 > m > 0,  right to left , xInc = -1, yInc = -1;
    // 0 > m > -1,  left to right , xInc = 1, yInc = -1;
    //0 > m > -1,  right to left , xInc = -1, yInc = 1;
    
    //vertical line > m > 1,  right to left , xInc = -1, yInc = -1;     
    //vertical line > m > 1,  left to right , xInc = 1, yInc = 1;       
    //vertical line < m < -1, left to right , xInc = 1, yInc = -1;
    //vertical line < m < -1, right to left , xInc = -1, yInc = 1;


    glPointSize(2.0);
    glBegin(GL_POINTS);

    int m;
    if (dx != 0)
        m = dy / dx;
    else
        m = dy;

    // case 1  dx>dy , 1 > m  && m > -1 or  m is between 1 and -1
    if (m<1) {
        p = 2 * dy - dx; 

        for (int i = 0; i <= dx; i++) { 
            glVertex2i(x, y); 

            x += xInc; 
            if (p < 0) {
                p += 2 * dy; 
            }
            else {                      //if   0 < m < 1, y = y+1; if y2>y1
                y += yInc;              //if  -1 < m < 0, y = y-1; if y2<y1
                p += 2 * (dy - dx); 
            }
        }
    }

    // case 2 for dy>=dx and since dy/dx is only magnitude, m>=1 && m<=-1 or m is not in between 1 and -1
    else {
        p = 2 * dx - dy; 

        for (int i = 0; i <= dy; i++) { 
            glVertex2i(x, y); 

            y += yInc; 
            if (p < 0) {
                p += 2 * dx; 
            }
            else {
                x += xInc; 
                p += 2 * (dx - dy); 
            }
        }
    }

    glEnd();
    glFlush();
}

void display_s_p() {
    glColor3f(0.0f, 0.0f, 0.0f);
    std::string sl = "Slope: ";
    glRasterPos2f(-490.0f, 440.0f);
    for (char c : sl) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    std::ostringstream oss;
    oss << slope_dis;
    std::string scoreStr = oss.str();
    glRasterPos2f(-415.0f, 440.0f); 
    for (char c : scoreStr) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}

void graph_paper() {
    glBegin(GL_LINES);
    int j = 500;
    glColor3f(0.0f, 0.8f, 0.0f);

    for (int i = -500; i <= 0; i += 50) { 
        if (i == 0) {
            glColor3f(0.0f, 0.0f, 0.0f);

            glVertex2f(0, -500);
            glVertex2f(0, 500);

            glVertex2f(500, 0);
            glVertex2f(-500, 0);
        }

        glVertex2f(i, -j);
        glVertex2f(i, j);

        glVertex2f(-i, -j);
        glVertex2f(-i, j);

        glVertex2f(-j, i);
        glVertex2f(j, i);

        glVertex2f(-j, -i);
        glVertex2f(j, -i);

        glVertex2f(-500, 500);
        glVertex2f(500, -500);

        glVertex2f(500, 500);
        glVertex2f(-500, -500);
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
        bresenhams_line(line.first, line.second);
    }

    points_coord(pt_x, pt_y);

    glColor3f(0.0f, 0.0f, 0.0f);
    std::string name = "Sanskar Rathore, 500108014";
    glRasterPos2f(-490.0f, 470.0f);
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

        float x_pos = ((x / (float)windowWidth) * 1000) - 500; 
        float y_pos = (((windowHeight - y) / (float)windowHeight) * 1000) - 500; 
        pt_x = x_pos;
        pt_y = y_pos;
        Point p = { x_pos, y_pos };
        cout << "Point clicked: (" << p.x << ", " << p.y << ")" << endl;
        points_coord(p.x, p.y);
        points.push_back(p);

        if (points.size() == 2) {
            lines.push_back({ points[0], points[1] });
            calc_slope(points[0], points[1]);
            points.clear(); // reseted points after storing the line
            glutPostRedisplay(); // trigger display to draw the lines
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lines.clear(); // clearing all lines
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(0.9, 1.0, 0.9, 1.0);
    glColor3f(0.0, 0.0, 0.0);

    glPointSize(5.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-500.0, 500.0, -500.0, 500.0); //updated coordinate range set to -500 to 500
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Assignment-4 (Bresenhams Line Graph)");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
