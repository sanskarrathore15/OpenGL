#include <freeglut.h>
#include <iostream>
#include <queue>  // Changed from stack to queue
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

vector<Point> currentPolygon;
int screenWidth, screenHeight;

bool isboundaryFillSelected = false;
bool isfloodFillSelected = false;
bool isPolygonDrawing = true;

void renderBitmapString(float x, float y, void* font, const char* string) {
    glColor3f(0.0, 0.0, 0.0);  // Black text color
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Helper function to calculate text width
int getTextWidth(const char* text, void* font) {
    int width = 0;
    for (const char* c = text; *c != '\0'; c++) {
        width += glutBitmapWidth(font, *c);
    }
    return width;
}

void setPixel(int x, int y, float r, float g, float b) {
    glBegin(GL_POINTS);
    glColor3f(r, g, b);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void getPixelColor(int x, int y, float* color) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

void clearFill() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();
}

void realTimeBoundaryFill(int x, int y, float* fillColor, float* boundaryColor) {
    queue<Point> points;  // Changed from stack to queue
    points.push(Point(x, y));

    while (!points.empty()) {
        Point p = points.front();  // Changed from top() to front()
        points.pop();

        float color[3];
        getPixelColor(p.x, p.y, color);

        if ((color[0] != boundaryColor[0] || color[1] != boundaryColor[1] || color[2] != boundaryColor[2]) &&
            (color[0] != fillColor[0] || color[1] != fillColor[1] || color[2] != fillColor[2])) {
            setPixel(p.x, p.y, fillColor[0], fillColor[1], fillColor[2]);

            if (points.size() % 50 == 0) {
                glutSwapBuffers();
            }

            // Push neighbors in clockwise order for more organized filling
            points.push(Point(p.x + 1, p.y));  // right
            points.push(Point(p.x, p.y + 1));  // up
            points.push(Point(p.x - 1, p.y));  // left
            points.push(Point(p.x, p.y - 1));  // down
        }
    }
}

void realTimeFloodFill(int x, int y, float* fillColor, float* defaultColor) {
    queue<Point> points;  // Changed from stack to queue
    points.push(Point(x, y));

    float targetColor[3];
    getPixelColor(x, y, targetColor);

    while (!points.empty()) {
        Point p = points.front();  // Changed from top() to front()
        points.pop();

        float color[3];
        getPixelColor(p.x, p.y, color);

        if (color[0] == targetColor[0] &&
            color[1] == targetColor[1] &&
            color[2] == targetColor[2]) {

            setPixel(p.x, p.y, fillColor[0], fillColor[1], fillColor[2]);

            if (points.size() % 50 == 0) {
                glutSwapBuffers();
            }

            // Push neighbors in clockwise order for more organized filling
            points.push(Point(p.x + 1, p.y));  // right
            points.push(Point(p.x, p.y + 1));  // up
            points.push(Point(p.x - 1, p.y));  // left
            points.push(Point(p.x, p.y - 1));  // down
        }
    }
}

void drawPolygon() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (const auto& point : currentPolygon) {
        glVertex2i(point.x, point.y);
    }
    glEnd();
}

void reshape(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Calculate margins and spacing
    int margin = 20;
    int lineSpacing = 25;
    int topMargin = screenHeight - margin;

    // Draw instructions
    void* font = GLUT_BITMAP_HELVETICA_18;  // Larger font for better visibility

    // Left-aligned instructions
    renderBitmapString(margin, topMargin, font, "Controls:");
    renderBitmapString(margin, topMargin - lineSpacing, font, "P/p - Stop taking input");
    renderBitmapString(margin, topMargin - lineSpacing * 2, font, "B/b - Boundary Fill");
    renderBitmapString(margin, topMargin - lineSpacing * 3, font, "F/f - Flood Fill");
    renderBitmapString(margin, topMargin - lineSpacing * 4, font, "R/r - Reset Polygon");
    renderBitmapString(margin, topMargin - lineSpacing * 5, font, "C/c - Clear Fill");
    //renderBitmapString(margin, topMargin - lineSpacing * 6, font, "ESC - Exit Fullscreen");

    // Center-aligned title
    const char* title = "Filling Algorithms";
    int titleWidth = getTextWidth(title, font);
    renderBitmapString((screenWidth - titleWidth) / 2, topMargin, font, title);

    // Right-aligned credit
    const char* credit = "Sanskar Rathore 500108014";
    int creditWidth = getTextWidth(credit, font);
    renderBitmapString(screenWidth - creditWidth - margin, margin + lineSpacing, font, credit);

    // Draw the polygon if it exists
    if (!currentPolygon.empty()) {
        drawPolygon();
    }

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && isPolygonDrawing) {
        currentPolygon.push_back(Point(x, screenHeight - y));
        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        currentPolygon.clear();
        clearFill();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'p':
    case 'P':
        isPolygonDrawing = !isPolygonDrawing;
        break;
    case 'b':
    case 'B':
        if (!currentPolygon.empty() && currentPolygon.size() > 2) {
            int centerX = 0, centerY = 0;
            for (const auto& point : currentPolygon) {
                centerX += point.x;
                centerY += point.y;
            }
            centerX /= currentPolygon.size();
            centerY /= currentPolygon.size();

            float fillColor[3] = { 1.0, 1.0, 0.0 };
            float boundaryColor[3] = { 1.0, 0.0, 0.0 };
            realTimeBoundaryFill(centerX, centerY, fillColor, boundaryColor);
        }
        break;
    case 'f':
    case 'F':
        if (!currentPolygon.empty() && currentPolygon.size() > 2) {
            int centerX = 0, centerY = 0;
            for (const auto& point : currentPolygon) {
                centerX += point.x;
                centerY += point.y;
            }
            centerX /= currentPolygon.size();
            centerY /= currentPolygon.size();

            float fillColor[3] = { 1.0, 1.0, 0.0 };
            realTimeFloodFill(centerX, centerY, fillColor, nullptr);
        }
        break;
    case 'r':
    case 'R':
        currentPolygon.clear();
        glutPostRedisplay();
        break;
    case 'c':
    case 'C':
        clearFill();
        break;
    case 27: // ESC key
        glutLeaveFullScreen();
        break;
    }
}

void init() {
    glClearColor(.8, 1.0, .8, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Get screen resolution
    screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow("Filling Algorithms");
    glutFullScreen();

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}