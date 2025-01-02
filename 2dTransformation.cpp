#include <glut.h>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#define M_PI 3.145

// Structure to store point coordinates
struct Point {
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

// Global variables
std::vector<Point> vertices;
std::vector<Point> orig_vertices;
std::vector<Point> transformed;
std::vector<Point> currentPoints;  // Points during animation
float inputValue1 = 0.0f, inputValue2 = 0.0f;
int currentOperation = 0;
bool isTransforming = false;
Point centroid;
char inputBuffer[50] = "";
bool isInputActive = false;
int activeInput = 1; // 1 for first value, 2 for second value


// Animation variables
bool isAnimating = false;
float animationProgress = 0.0f;
const float ANIMATION_SPEED = 0.02f;  // Adjust for faster/slower animation


// Input buffer variables
std::string input1Buffer = "0";
std::string input2Buffer = "0";

// Helper function to convert string to float safely
float stringToFloat(const std::string& str) {
    try {
        return std::stof(str);
    }
    catch (...) {
        return 0.0f;
    }
}


//// Function to validate numeric input
//bool isValidNumericInput(const std::string& current, char newChar) {
//    if (newChar >= '0' && newChar <= '9') return true;
//    if (newChar == '-' && current.empty()) return true;
//    if (newChar == '.' && current.find('.') == std::string::npos) return true;
//    return false;
//}
// Function to validate numeric input
bool isValidNumericInput(const std::string& current, char newChar) {
    // Allow digits anywhere
    if (newChar >= '0' && newChar <= '9') return true;

    // Allow minus sign at start or to replace first character
    if (newChar == '-') {
        if (current.empty() || current == "0") return true;
        // Allow replacing first character with minus
        if (current[0] != '-') return true;
    }

    // Allow decimal point if one doesn't exist already
    if (newChar == '.') {
        return current.find('.') == std::string::npos;
    }

    return false;
}


// Function to interpolate between two points
Point interpolatePoints(const Point& start, const Point& end, float t) {
    Point result;
    result.x = start.x + (end.x - start.x) * t;
    result.y = start.y + (end.y - start.y) * t;
    return result;
}

// Timer function for animation
void animationTimer(int value) {
    if (isAnimating) {
        animationProgress += ANIMATION_SPEED;

        if (animationProgress >= 1.0f) {
            animationProgress = 1.0f;
            isAnimating = false;
            currentPoints = transformed;
        }
        else {
            // Interpolate between original and transformed positions
            currentPoints.clear();
            for (size_t i = 0; i < vertices.size(); i++) {
                currentPoints.push_back(interpolatePoints(vertices[i], transformed[i], animationProgress));
            }
            glutTimerFunc(16, animationTimer, 0);  // 60 FPS approximately
        }
        glutPostRedisplay();
    }
}

// Function to start animation
void startAnimation() {
    if (!isAnimating) {
        isAnimating = true;
        animationProgress = 0.0f;
        currentPoints = vertices;
        glutTimerFunc(0, animationTimer, 0);
    }
}



// Function to calculate centroid
void calculateCentroid() {
    if (vertices.empty()) return;

    float sumX = 0, sumY = 0;
    for (const Point& p : vertices) {
        sumX += p.x;
        sumY += p.y;
    }
    centroid.x = sumX / vertices.size();
    centroid.y = sumY / vertices.size();
}



// Draw grid lines
void drawGrid() {
    glColor3f(0.0f, 0.8f, 0.0f);  // Light gray color for grid
    glBegin(GL_LINES);

    // Vertical lines
    for (float x = -10.0f; x <= 10.0f; x += .5f) {
        if (x == 0) continue;  // Skip center lines
        glVertex2f(x, -10.0f);
        glVertex2f(x, 10.0f);
    }

    // Horizontal lines
    for (float y = -10.0f; y <= 10.0f; y += 1.0f) {
        if (y == 0) continue;  // Skip center lines
        glVertex2f(-10.0f, y);
        glVertex2f(10.0f, y);
    }
    glEnd();

    // Draw axes in darker gray
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    // X-axis
    glVertex2f(-10.0f, 0.0f);
    glVertex2f(10.0f, 0.0f);
    // Y-axis
    glVertex2f(0.0f, -10.0f);
    glVertex2f(0.0f, 10.0f);
    glEnd();
}

void drawText(float x, float y, const std::string& text) {
    glPointSize(2.0f);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}


// Function to multiply a point by a 3x3 matrix
void matrixMultiply(float matrix[3][3], Point& p) {
    float x = p.x;
    float y = p.y;
    float w = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2];
    if (w == 0) w = 1;

    p.x = (matrix[0][0] * x + matrix[0][1] * y + matrix[0][2]) / w;
    p.y = (matrix[1][0] * x + matrix[1][1] * y + matrix[1][2]) / w;
}

void applyTransformation() {
    if (vertices.empty()) return;
    calculateCentroid();
    transformed = vertices;

    // Translation matrices
    float toOrigin[3][3] = {
        {1, 0, -centroid.x},
        {0, 1, -centroid.y},
        {0, 0, 1}
    };

    float fromOrigin[3][3] = {
        {1, 0, centroid.x},
        {0, 1, centroid.y},
        {0, 0, 1}
    };

    switch (currentOperation) {
    case 1: // Translation
        for (Point& p : transformed) {
            p.x += inputValue1;
            p.y += inputValue2;
        }
        break;

    case 2: // Rotation
    {
        float angle = inputValue1 * M_PI / 180.0f;
        float rotationMatrix[3][3] = {
            {cos(angle), -sin(angle), 0},
            {sin(angle), cos(angle), 0},
            {0, 0, 1}
        };

        for (Point& p : transformed) {
            matrixMultiply(toOrigin, p);
            matrixMultiply(rotationMatrix, p);
            matrixMultiply(fromOrigin, p);
        }
        break;
    }

    case 3: // Scaling
        for (Point& p : transformed) {
            matrixMultiply(toOrigin, p);
            p.x *= inputValue1;
            p.y *= inputValue2;
            matrixMultiply(fromOrigin, p);
        }
        break;

    case 4: // Reflection about X-axis
        for (Point& p : transformed) {
            p.y = -p.y;
        }
        break;

    case 5: // Reflection about Y-axis
        for (Point& p : transformed) {
            p.x = -p.x;
        }
        break;

    case 6: // Shear X
    {
        float shearMatrix[3][3] = {
            {1, inputValue1, 0},
            {0, 1, 0},
            {0, 0, 1}
        };

        for (Point& p : transformed) {
            matrixMultiply(toOrigin, p);
            matrixMultiply(shearMatrix, p);
            matrixMultiply(fromOrigin, p);
        }
        break;
    }

    case 7: // Shear Y
    {
        float shearMatrix[3][3] = {
            {1, 0, 0},
            {inputValue1, 1, 0},
            {0, 0, 1}
        };

        for (Point& p : transformed) {
            matrixMultiply(toOrigin, p);
            matrixMultiply(shearMatrix, p);
            matrixMultiply(fromOrigin, p);
        }
        break;
    }
    }
}
void drawPolygon(const std::vector<Point>& points, bool isTransformed = false) {
    if (points.size() < 2) return;

    if (isTransformed) {
        glColor3f(1.0f, 0.0f, 0.0f);  // Red for transformed
    }
    else {
        glColor3f(0.0f, 0.0f, .4f);  // Blue for original
    }

    glBegin(GL_POLYGON);
    for (const Point& p : points) {
        glVertex2f(p.x, p.y);
    }
    if (points.size() > 2) {
        glVertex2f(points[0].x, points[0].y);
    }
    glEnd();

    // Draw vertices
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (const Point& p : points) {
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void drawInterface() {
    glColor3f(0.0f, 0.0f, 0.0f);

    std::string operationText;
    std::string value1Label, value2Label;
    switch (currentOperation) {
    case 1:
        operationText = "Translation";
        value1Label = "X: ";
        value2Label = "Y: ";
        break;
    case 2:
        operationText = "Rotation";
        value1Label = "Angle: ";
        value2Label = "";
        break;
    case 3:
        operationText = "Scaling";
        value1Label = "X: ";
        value2Label = "Y: ";
        break;
    case 4:
        operationText = "Reflection X";
        break;
    case 5:
        operationText = "Reflection Y";
        break;
    case 6:
        operationText = "Shear X";
        value1Label = "Shear Factor: ";
        value2Label = "";
        break;
    case 7:
        operationText = "Shear Y";
        value1Label = "Shear Factor: ";
        value2Label = "";
        break;
    default:
        operationText = "Select Operation";
    }

    // Draw operation name
    drawText(-9.5f, 9.0f, "Operation: " + operationText);

    //if (currentOperation <= 3) {
    //    std::stringstream ss1, ss2;
    //    ss1 << value1Label << inputValue1;
    //    drawText(-9.5f, 8.5f, ss1.str());

    //    if (!value2Label.empty()) {
    //        ss2 << value2Label << inputValue2;
    //        drawText(-9.5f, 8.0f, ss2.str());
    //    }

    //    // Show which input is active
    //    if (isInputActive) {
    //        drawText(-9.8f, activeInput == 1 ? 8.5f : 8.0f, ">");
    //    }
    //}
    if (currentOperation <= 3 || currentOperation == 6 || currentOperation == 7) {
        drawText(-9.5f, 8.5f, value1Label + input1Buffer);

        if (!value2Label.empty()) {
            drawText(-9.5f, 8.0f, value2Label + input2Buffer);
        }

        if (isInputActive) {
            drawText(-9.8f, activeInput == 1 ? 8.5f : 8.0f, ">");
        }
    }

    drawText(-9.5f, -9.0f, "Left Click: Add Points | Right Click: Menu | Enter: Apply | Tab: Switch Input");
}


void drawPoints(float x, float y, int index, const Point& p) {
    char buffer[50];
    sprintf_s(buffer, "Point %d: (%.1f, %.1f)", index + 1, p.x, p.y);
    glRasterPos2f(x, y);
    for (char c : buffer) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

int check = 1;
bool disp_original = false;
std::vector<Point> originalShape;
bool showOriginal = false;
bool hasStoredOriginal = false;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);




    /* if (disp_original) {
         drawPolygon(orig_vertices);
         disp_original = false;
     }*/
    drawText(7.0f, -9.0f, "Sanskar Rathore,500108014");
    drawGrid();
    drawInterface();
    drawPolygon(vertices);
    // In the display function, after drawGrid() and drawInterface()
    float startY = 7.5f;  // Starting Y position for the first point
    for (int i = 0; i < vertices.size(); i++) {
        drawPoints(-9.5f, startY - (i * 0.5f), i, vertices[i]);
    }
    // Draw original shape if toggle is on
    if (showOriginal && !originalShape.empty()) {
        glColor3f(.0f, .2f, 0.0f);  // Red color
        glBegin(GL_LINE_STRIP);
        for (const Point& p : originalShape) {
            glVertex2f(p.x, p.y);
        }
        if (originalShape.size() > 2) {
            glVertex2f(originalShape[0].x, originalShape[0].y);
        }
        glEnd();

        // Draw vertices of original shape
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        for (const Point& p : originalShape) {
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }

    // Draw transformed vertices if transforming
    if (isTransforming) {
        if (isAnimating) {
            drawPolygon(currentPoints, true);
        }
        else {
            drawPolygon(transformed, true);
        }
    }

    glutSwapBuffers();
}

//void keyboard(unsigned char key, int x, int y) {
//    if (currentOperation <= 3) {
//        if (key == 13) { // Enter key
//            isTransforming = true;
//            applyTransformation();
//            startAnimation();  // Start animation when transformation is applied
//        }
//        else if (key == 9) { // Tab key
//            if (currentOperation != 2) { // Don't switch for rotation
//                activeInput = (activeInput == 1) ? 2 : 1;
//            }
//        }
//        else if (key == 8) { // Backspace
//            if (activeInput == 1) {
//                inputValue1 = 0;
//            }
//            else {
//                inputValue2 = 0;
//            }
//        }
//        else if (key >= '0' && key <= '9'  key == '.'  key == '-') {
//            std::string* targetStr;
//            float* targetValue;
//
//            if (activeInput == 1) {
//                targetValue = &inputValue1;
//            }
//            else {
//                targetValue = &inputValue2;
//            }
//
//            std::stringstream ss;
//            ss << *targetValue;
//            if (key == 8) { // Backspace
//                std::string str = ss.str();
//                if (!str.empty()) {
//                    str.pop_back();
//                    ss.str("");
//                    ss << str;
//                    ss >> *targetValue;
//                }
//            }
//            else {
//                ss << key;
//                ss >> *targetValue;
//            }
//        }
//        glutPostRedisplay();
//    }
//}

//void keyboard(unsigned char key, int x, int y) {
//    if (currentOperation <= 3) {
//        std::string& currentBuffer = (activeInput == 1) ? input1Buffer : input2Buffer;
//
//        if (key == 13) { // Enter key
//            // Convert buffer strings to float values
//            inputValue1 = stringToFloat(input1Buffer);
//            inputValue2 = stringToFloat(input2Buffer);
//            isTransforming = true;
//            applyTransformation();
//            startAnimation();
//        }
//        else if (key == 9) { // Tab key
//            if (currentOperation != 2) { // Don't switch for rotation
//                activeInput = (activeInput == 1) ? 2 : 1;
//            }
//        }
//        else if (key == 8) { // Backspace
//            if (!currentBuffer.empty()) {
//                currentBuffer.pop_back();
//                if (currentBuffer.empty()) {
//                    currentBuffer = "0";
//                }
//            }
//        }
//        else if (isValidNumericInput(currentBuffer, key)) {
//            if (currentBuffer == "0") {
//                currentBuffer = key;
//            }
//            else {
//                currentBuffer += key;
//            }
//        }
//
//        glutPostRedisplay();
//    }
//}


void keyboard(unsigned char key, int x, int y) {

    if (key == 'o' ||  key == 'O') {
        showOriginal = !showOriginal;
        glutPostRedisplay();
        return;
    }

    if (key == 'f' ||  key == 'F') {
        // Only finalize if there's a transformation and animation is complete
             // Copy transformed points to vertices
        vertices = transformed;

        // Reset transformation state
        isTransforming = false;

        // Reset input values
        input1Buffer = "0";
        input2Buffer = "0";
        inputValue1 = 0.0f;
        inputValue2 = 0.0f;

        // Clear animation progress
        animationProgress = 0.0f;
        currentPoints.clear();

        glutPostRedisplay();

    }

    if (currentOperation <= 3 || currentOperation == 6 || currentOperation == 7) {
        std::string& currentBuffer = (activeInput == 1) ? input1Buffer : input2Buffer;

        if (key == 13) { // Enter key
            // Convert buffer strings to float values
            inputValue1 = stringToFloat(input1Buffer);
            inputValue2 = stringToFloat(input2Buffer);
            isTransforming = true;
            applyTransformation();
            startAnimation();
        }
        else if (key == 9) { //Tab key
            if (currentOperation != 2) { // Don't switch for rotation
                activeInput = (activeInput == 1) ? 2 : 1;
            }
        }
        else if (key == 8) { // Backspace
            if (!currentBuffer.empty()) {
                currentBuffer.pop_back();
                if (currentBuffer.empty()) {
                    currentBuffer = "0";
                }
            }
        }
        else if (isValidNumericInput(currentBuffer, key)) {
            if (key == '-') {
                if (currentBuffer[0] == '-') {
                    // Remove minus sign if it exists
                    currentBuffer = currentBuffer.substr(1);
                }
                else {
                    // Add minus sign
                    currentBuffer = "-" + currentBuffer;
                }
            }
            else {
                if (currentBuffer == "0") {
                    currentBuffer = key;
                }
                else {
                    currentBuffer += key;
                }
            }
        }

        glutPostRedisplay();
    }
}


void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

        // Convert screen coordinates to world coordinates
        float wx = (20.0f * x / windowWidth) - 10.0f;
        float wy = 10.0f - (20.0f * y / windowHeight);

        vertices.push_back(Point(wx, wy));
        originalShape = vertices;

        // Store original shape when first creating it
        if (!hasStoredOriginal) {
            originalShape = vertices;
            hasStoredOriginal = true;
        }

        if (!isTransforming) {
            transformed = vertices;
        }

        glutPostRedisplay();
    }
}

//void processMenu(int option) {
//    currentOperation = option;
//    isInputActive = (option <= 3);
//    activeInput = 1;
//    inputValue1 = 0.0f;
//    inputValue2 = 0.0f;
//
//    if (option == 6) { // Clear
//        vertices.clear();
//        transformed.clear();
//        currentPoints.clear();
//        isTransforming = false;
//        isAnimating = false;
//    }
//    else if (option >= 4) { // Reflections
//        isTransforming = true;
//        applyTransformation();
//        startAnimation();  // Start animation for reflections
//    }
//
//    glutPostRedisplay();
//}


void processMenu(int option) {
    currentOperation = option;
    // Allow input for translation, rotation, scaling, and skewing
    isInputActive = (option <= 3 || option == 6 || option == 7);
    activeInput = 1;

    // Reset input buffers
    input1Buffer = "0";
    input2Buffer = "0";
    inputValue1 = 0.0f;
    inputValue2 = 0.0f;

    if (option == 8) { // Clear
        vertices.clear();
        transformed.clear();
        currentPoints.clear();
        originalShape.clear();  // Clear original vertices too
        hasStoredOriginal = false;
        showOriginal = false;
        isTransforming = false;
        isAnimating = false;
    }
    else if (option == 4 || option == 5) { // Reflections only
        isTransforming = true;
        applyTransformation();
        startAnimation();
    }

    glutPostRedisplay();
}

void createMenu() {
    glutCreateMenu(processMenu);
    glutAddMenuEntry("Translate", 1);
    glutAddMenuEntry("Rotate", 2);
    glutAddMenuEntry("Scale", 3);
    glutAddMenuEntry("Reflect X", 4);
    glutAddMenuEntry("Reflect Y", 5);
    glutAddMenuEntry("Shear X", 6);
    glutAddMenuEntry("Shear Y", 7);
    glutAddMenuEntry("Clear", 8);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void init() {
    glClearColor(.85f, 1.0f, .85f, 1.0f);  // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);  // Coordinate system from -10 to 10
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Transformations");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    createMenu();
    glutFullScreen();
    glutMainLoop();
    return 0;
}