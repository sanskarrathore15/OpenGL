#include <glut.h>
#include <GL/gl.h>
#include <array>
#include <vector>
#include <cmath>
#include <string>
#define PI 3.14159265


enum Shape { CUBE, PYRAMID, SPHERE, CYLINDER };
Shape currentShape = CYLINDER;

std::vector<std::array<float, 4>>  cubeVertices = {
            {-0.5f, -0.5f, -0.5f, 1.0f},
            {0.5f, -0.5f, -0.5f, 1.0f},
            {0.5f, 0.5f, -0.5f, 1.0f},
            {-0.5f, 0.5f, -0.5f, 1.0f},
            {-0.5f, -0.5f, 0.5f, 1.0f},
            {0.5f, -0.5f, 0.5f, 1.0f},
            {0.5f, 0.5f, 0.5f, 1.0f},
            {-0.5f, 0.5f, 0.5f, 1.0f}
};

std::vector<std::array<float, 4>> pyramidVertices = {
    {0.5f, 0.5f, 0.5f, 1.0f},  // back-left
    {0.5f, -0.5f, -0.5f, 1.0f},  // back-right
    {0.5f, 0.5f, 0.5f, 1.0f},    // front-right
    {-0.5f, -0.5f, 0.5f, 1.0f},  // front-left
    {0.0f, 0.5f, 0.0f, 1.0f}     // apex
};


std::vector<std::array<float, 4>> sphereVertices = {
    {0.0f, 0.0f, 1.0f, 1.0f},  // North pole
    {0.5f, 0.0f, 0.5f, 1.0f},
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.0f, -0.5f, 1.0f},
    {0.0f, 0.0f, -1.0f, 1.0f},  // South pole
    {-0.5f, 0.0f, -0.5f, 1.0f},
    {-1.0f, 0.0f, 0.0f, 1.0f},
    {-0.5f, 0.0f, 0.5f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},   // Equator points
    {0.0f, -1.0f, 0.0f, 1.0f}
    // More vertices can be added for greater detail
};


std::vector<std::array<float, 4>> cylinderVertices = {
    {0.5f, 0.0f, 1.0f, 1.0f},    // Top circle (vertices on the top)
    {0.0f, 0.0f, 1.0f, 1.0f},    // Top circle center
    {-0.5f, 0.0f, 1.0f, 1.0f},
    {-0.5f, 0.0f, 1.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
    {0.5f, 0.0f, 1.0f, 1.0f},

    {0.5f, 0.0f, -1.0f, 1.0f},   // Bottom circle (vertices on the bottom)
    {0.0f, 0.0f, -1.0f, 1.0f},   // Bottom circle center
    {-0.5f, 0.0f, -1.0f, 1.0f},
    {-0.5f, 0.0f, -1.0f, 1.0f},
    {0.0f, 0.0f, -1.0f, 1.0f},
    {0.5f, 0.0f, -1.0f, 1.0f}
    // More vertices and divisions can be added to generate smoother circles
};



GLfloat lightPosition[] = { 0.0f, 5.0f, 5.0f, 1.0f };
GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
bool lightingEnabled = false;
int currentLight = 0; // 0: Position, 1: Ambient, 2: Diffuse, 3: Specular

// Add these material properties
GLfloat materialAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat materialDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess[] = { 50.0f };


const float step = 0.1f;


// Add these global variables for camera control
float cameraPos[] = { 0.0f, 1.0f, 5.0f };
float cameraFront[] = { .0f, -0.2f, -1.0f };
float cameraUp[] = { 0.0f, 1.0f, 0.0f };
float cameraSpeed = 0.1f;
float yaw = -90.0f;
float pitch = 0.0f;
int lastX = 0, lastY = 0;
bool firstMouse = true;
bool mouseEnabled = false;


void processSpecialKeys(int key, int x, int y);
void mouseMovement(int x, int y);
void mouseButton(int button, int state, int x, int y);



int currentOperation = 0;

float translationX = 0.0f;
float translationY = 0.0f;
float translationZ = 0.0f;

float rotationAngleX = 0.0f;
float rotationAngleY = 0.0f;
float rotationAngleZ = 0.0f;

float scalingFactorX = 1.0f;
float scalingFactorY = 1.0f;
float scalingFactorZ = 1.0f;

float shearX1 = 0.0f;
float shearX2 = 0.0f;
float shearY1 = 0.0f;
float shearY2 = 0.0f;
float shearZ1 = 0.0f;
float shearZ2 = 0.0f;

char reflectionAxis = 'n';
bool isReflecting = false;
float reflectionProgress = 0.0f;
float reflectionMat[4][4],
reflectionTargetMat[4][4];
BOOLEAN gridYes = true;

void drawGrid() {
    // Draw the main grid
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw finer grid lines
    glLineWidth(1.0f);
    float gridSize = 20.0f;
    float spacing = .2f;
    if (gridYes) {

        glBegin(GL_LINES);
        for (float i = -gridSize; i <= gridSize; i += spacing) {
            if (i == 0) continue;

            // Calculate distance from center for alpha
            float alpha = 1.0f - (fabs(i) / gridSize) * 0.5f;

            // Draw grid lines parallel to X axis
            glColor4f(0.2f, 0.2f, 0.2f, alpha * 0.3f);
            glVertex3f(i, 0.0f, -gridSize);
            glVertex3f(i, 0.0f, gridSize);

            // Draw grid lines parallel to Y axis
            glVertex3f(-gridSize, 0.0f, i);
            glVertex3f(gridSize, 0.0f, i);
        }
        glEnd();
    }

    // Draw the main axes
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // X axis - Red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-gridSize, 0.0f, 0.0f);
    glVertex3f(gridSize, 0.0f, 0.0f);

    // Y axis - Green
    glColor3f(0.0f, 0.8f, 0.0f);
    glVertex3f(0.0f, -gridSize, 0.0f);
    glVertex3f(0.0f, gridSize, 0.0f);

    // Z axis - Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -gridSize);
    glVertex3f(0.0f, 0.0f, gridSize);
    glEnd();

    // Draw axis arrows
    glLineWidth(2.0f);
    float arrowSize = 0.3f;

    // X axis arrow
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(gridSize, 0.0f, 0.0f);
    glVertex3f(gridSize - arrowSize, arrowSize, 0.0f);
    glVertex3f(gridSize, 0.0f, 0.0f);
    glVertex3f(gridSize - arrowSize, -arrowSize, 0.0f);

    // Y axis arrow
    glColor3f(0.0f, 0.8f, 0.0f);
    glVertex3f(0.0f, gridSize, 0.0f);
    glVertex3f(arrowSize, gridSize - arrowSize, 0.0f);
    glVertex3f(0.0f, gridSize, 0.0f);
    glVertex3f(-arrowSize, gridSize - arrowSize, 0.0f);

    // Z axis arrow
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, gridSize);
    glVertex3f(arrowSize, 0.0f, gridSize - arrowSize);
    glVertex3f(0.0f, 0.0f, gridSize);
    glVertex3f(-arrowSize, 0.0f, gridSize - arrowSize);
    glEnd();

    glDisable(GL_BLEND);
}

//    TRANSLATION
void translationMatrix(float tx, float ty, float tz, float matrix[4][4]) {
    matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = tx;
    matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = ty;
    matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = tz;
    matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;
}

//    ROTATION 
void rotationMatrixX(float angle, float matrix[4][4]) {
    float radians = angle * PI / 180.0f;
    float cosAngle = cos(radians);
    float sinAngle = sin(radians);

    matrix[0][0] = 1.0f; matrix[0][1] = 0.0f;     matrix[0][2] = 0.0f;    matrix[0][3] = 0.0f;
    matrix[1][0] = 0.0f; matrix[1][1] = cosAngle; matrix[1][2] = -sinAngle; matrix[1][3] = 0.0f;
    matrix[2][0] = 0.0f; matrix[2][1] = sinAngle; matrix[2][2] = cosAngle;  matrix[2][3] = 0.0f;
    matrix[3][0] = 0.0f; matrix[3][1] = 0.0f;     matrix[3][2] = 0.0f;     matrix[3][3] = 1.0f;
}

void rotationMatrixY(float angle, float matrix[4][4]) {
    float radians = angle * PI / 180.0f;
    float cosAngle = cos(radians);
    float sinAngle = sin(radians);

    matrix[0][0] = cosAngle;  matrix[0][1] = 0.0f; matrix[0][2] = sinAngle; matrix[0][3] = 0.0f;
    matrix[1][0] = 0.0f;      matrix[1][1] = 1.0f; matrix[1][2] = 0.0f;     matrix[1][3] = 0.0f;
    matrix[2][0] = -sinAngle; matrix[2][1] = 0.0f; matrix[2][2] = cosAngle;  matrix[2][3] = 0.0f;
    matrix[3][0] = 0.0f;      matrix[3][1] = 0.0f; matrix[3][2] = 0.0f;     matrix[3][3] = 1.0f;
}

void rotationMatrixZ(float angle, float matrix[4][4]) {
    float radians = angle * PI / 180.0f;
    float cosAngle = cos(radians);
    float sinAngle = sin(radians);

    matrix[0][0] = cosAngle; matrix[0][1] = -sinAngle; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
    matrix[1][0] = sinAngle; matrix[1][1] = cosAngle;  matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
    matrix[2][0] = 0.0f;     matrix[2][1] = 0.0f;      matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
    matrix[3][0] = 0.0f;     matrix[3][1] = 0.0f;      matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;
}

// SCALING 
void scalingMatrix(float sx, float sy, float sz, float matrix[4][4]) {
    matrix[0][0] = sx;   matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
    matrix[1][0] = 0.0f; matrix[1][1] = sy;   matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
    matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = sz;   matrix[2][3] = 0.0f;
    matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;
}

// SHEARING 
void shearingMatrix(float y1, float z1, float x1, float z2, float x2, float y2, float matrix[4][4]) {
    matrix[0][0] = 1.0f; matrix[0][1] = y1; matrix[0][2] = z1; matrix[0][3] = 0.0f;
    matrix[1][0] = x1; matrix[1][1] = 1.0f; matrix[1][2] = z2; matrix[1][3] = 0.0f;
    matrix[2][0] = x2; matrix[2][1] = y2; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
    matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;
}


// REFLECTION for 3D
void reflectionMatrix(char axis, float matrix[4][4], float position = 0.0f) {
    // Initialize matrix to identity
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    // Apply reflection based on the specified axis
    switch (axis) {
    case 'x':  // Reflect about yz-plane (x = position)
        matrix[0][0] = -1.0f;
        matrix[0][3] = 2.0f * position;  // Apply translation to reflect around x = position
        break;

    case 'y':  // Reflect about xz-plane (y = position)
        matrix[1][1] = -1.0f;
        matrix[1][3] = 2.0f * position;  // Apply translation to reflect around y = position
        break;

    case 'z':  // Reflect about xy-plane (z = position)
        matrix[2][2] = -1.0f;
        matrix[2][3] = 2.0f * position;  // Apply translation to reflect around z = position
        break;

    case 'o':  // Reflect through origin
        matrix[0][0] = -1.0f;
        matrix[1][1] = -1.0f;
        matrix[2][2] = -1.0f;
        break;
    }
}

void intermediate_points(float matrix[4][4], float progress) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] = (1.0f - progress) * reflectionMat[i][j] + progress * reflectionTargetMat[i][j];
        }
    }
}

// ANIMATION
void reflectionAnimation(int value) {
    if (isReflecting) {
        reflectionProgress += 0.0001f;
        if (reflectionProgress >= 1.0f) {
            reflectionProgress = 1.0f;
            isReflecting = false;
        }
        intermediate_points(reflectionMat, reflectionProgress);
        glutPostRedisplay();
        glutTimerFunc(16, reflectionAnimation, 0);
    }
}

//    MULTIPLICATION OF MATRICES
void multiplyMatrices(float a[4][4], float b[4][4], float result[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}


//CENTROID
// Centroid calculation for Cube, Sphere, and Cylinder
std::array<float, 4> calculateCentroid() {
    std::array<float, 4> centroid = { 0.0f, 0.0f, 0.0f, 1.0f };

    int numVertices = cubeVertices.size();
    switch (currentShape) {
    case CUBE:
        // For Cube: Calculate the centroid based on cube vertices
        for (const auto& vertex : cubeVertices) {
            centroid[0] += vertex[0];
            centroid[1] += vertex[1];
            centroid[2] += vertex[2];
        }
        if (numVertices > 0) {
            centroid[0] /= numVertices;
            centroid[1] /= numVertices;
            centroid[2] /= numVertices;
        }
        break;

    case SPHERE:
        // For Sphere: The centroid is the center of the sphere
        // Assuming the sphere's center is at the origin (0, 0, 0)
        centroid[0] = 0.0f;
        centroid[1] = 0.0f;
        centroid[2] = 0.0f;
        break;

    case CYLINDER:
        // For Cylinder: The centroid is at the center of the base and half of its height
        // Assuming the cylinder is oriented along the z-axis and the base is at z=0
        centroid[0] = 0.0f; // Center of the base along the x-axis
        centroid[1] = 0.0f; // Center of the base along the y-axis
        centroid[2] = 4 / 2.0f; // Midpoint of the height along the z-axis
        break;

    default:
        //std::cout << "Invalid shape for centroid calculation!" << std::endl;
        break;
    }

    return centroid;
}

//TEXT
void drawText(const std::string& text, float x, float y, float z) {
    glRasterPos3f(x, y, z - 1.0f + 0.1f);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); // Changed font to Helvetica
    }
}


void print(const std::vector<std::array<float, 4>>& vertices) {
    // Initial text with the header
    std::string text = "Cube Vertices:";

    // Temporarily switch to orthographic projection     
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-5, 5, -5, 5);  // Adjust as needed     

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing so text appears on top     
    glDisable(GL_DEPTH_TEST);

    // Set initial position for the text, moved down a bit    
    float xPos = 3.1f;
    float yPos = 4.5f;  // Adjusted lower to move text display down     

    // Draw the header text     
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(text, xPos + 1.0f, -yPos, 0.0f);
    yPos -= 0.4f; // Add space below the header     

    // Draw each vertex line     
    for (const auto& vertex : vertices) {
        std::string line =
            std::to_string(vertex[0]).substr(0, 4) + " " +
            std::to_string(vertex[1]).substr(0, 4) + " " +
            std::to_string(vertex[2]).substr(0, 4);
        drawText(line, xPos + 1.0f, -yPos, 0.0f);
        yPos -= 0.2f; // Move down for each new line     
    }

    // Restore the original projection and modelview matrices     
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}



void updateLighting() {
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    }
    else {
        glDisable(GL_LIGHTING);
    }
}




void transformation(std::vector<std::array<float, 4>>& transformedVertices) {
    float rotationX[4][4], rotationY[4][4], rotationZ[4][4];
    float scaling[4][4], shearing[4][4];
    float real[4][4];
    float reflection[4][4];


    // Simply use identity matrices or translation to the desired position
    translationMatrix(0.0f, 0.0f, 0.0f, real);  // No translation applied

    float translationMatrix[4][4] = {
   {1, 0, 0, translationX},
   {0, 1, 0, translationY},
   {0, 0, 1, translationZ},
   {0, 0, 0, 1}
    };



    //           Rotation, scaling, and shearing matrices
    rotationMatrixX(rotationAngleX, rotationX);
    rotationMatrixY(rotationAngleY, rotationY);
    rotationMatrixZ(rotationAngleZ, rotationZ);
    scalingMatrix(scalingFactorX, scalingFactorY, scalingFactorZ, scaling);
    shearingMatrix(shearX1, shearX2, shearY1, shearY2, shearZ1, shearZ2, shearing);

    // Reflection matrix
    intermediate_points(reflectionMat, reflectionProgress);  // Interpolate reflection matrix

    float combined1[4][4], combined2[4][4], combined3[4][4], combined4[4][4], combined5[4][4], finalTransform[4][4];

    // Apply scaling, rotation, shearing transformations
    multiplyMatrices(real, scaling, combined1);                    // SCALING * REAL MATRIX
    multiplyMatrices(combined1, rotationZ, combined2);               // SCALING * REAL * Rz
    multiplyMatrices(combined2, rotationY, combined3);              // SCALING * REAL * Rz * Ry
    multiplyMatrices(combined3, rotationX, combined4);              // SCALING * REAL * Rz * Ry * Rx
    multiplyMatrices(combined4, real, combined1);                   // SCALING * REAL * Rz * Ry * Rx * REAL (identity here)
    multiplyMatrices(combined1, shearing, combined5);               // SCALING * REAL * Rz * Ry * Rx * REAL * SHEARING
    multiplyMatrices(combined5, reflectionMat, finalTransform);

    float finalTransformWithTranslation[4][4];
    multiplyMatrices(finalTransform, translationMatrix, finalTransformWithTranslation);


    for (const auto& vertex : cubeVertices) {
        std::array<float, 4> transformedVertex;
        for (int i = 0; i < 4; ++i) {
            transformedVertex[i] = 0.0f;
            for (int j = 0; j < 4; ++j) {
                transformedVertex[i] += finalTransformWithTranslation[i][j] * vertex[j];
            }
        }
        transformedVertices.push_back(transformedVertex);
    }



    print(transformedVertices);
}

void drawShape(const std::vector<std::array<float, 4>>& vertices) {
    if (currentShape == CUBE) {
        glBegin(GL_QUADS);

        // Back face
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3fv(vertices[0].data());
        glVertex3fv(vertices[1].data());
        glVertex3fv(vertices[2].data());
        glVertex3fv(vertices[3].data());

        // Front face
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3fv(vertices[4].data());
        glVertex3fv(vertices[5].data());
        glVertex3fv(vertices[6].data());
        glVertex3fv(vertices[7].data());

        // Left face
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3fv(vertices[0].data());
        glVertex3fv(vertices[3].data());
        glVertex3fv(vertices[7].data());
        glVertex3fv(vertices[4].data());

        // Right face
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3fv(vertices[1].data());
        glVertex3fv(vertices[5].data());
        glVertex3fv(vertices[6].data());
        glVertex3fv(vertices[2].data());

        // Top face
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3fv(vertices[3].data());
        glVertex3fv(vertices[2].data());
        glVertex3fv(vertices[6].data());
        glVertex3fv(vertices[7].data());

        // Bottom face
        glColor3f(1.0f, 0.5f, 0.6f);
        glVertex3fv(vertices[0].data());
        glVertex3fv(vertices[4].data());
        glVertex3fv(vertices[5].data());
        glVertex3fv(vertices[1].data());

        glEnd();
    }
    else if (currentShape == PYRAMID) {
        // Draw pyramid base
        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.5f); // Base color
        glVertex3fv(vertices[0].data()); // back-left
        glVertex3fv(vertices[1].data()); // back-right
        glVertex3fv(vertices[2].data()); // front-right
        glVertex3fv(vertices[3].data()); // front-left
        glEnd();

        // Draw pyramid faces
        glBegin(GL_TRIANGLES);

        // Front face
        glColor3f(1.0f, 0.0f, 0.0f); // Face color
        glVertex3fv(vertices[4].data()); // apex
        glVertex3fv(vertices[2].data()); // front-right
        glVertex3fv(vertices[3].data()); // front-left

        // Right face
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3fv(vertices[4].data()); // apex
        glVertex3fv(vertices[1].data()); // back-right
        glVertex3fv(vertices[2].data()); // front-right

        // Back face
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3fv(vertices[4].data()); // apex
        glVertex3fv(vertices[0].data()); // back-left
        glVertex3fv(vertices[1].data()); // back-right

        // Left face
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3fv(vertices[4].data()); // apex
        glVertex3fv(vertices[3].data()); // front-left
        glVertex3fv(vertices[0].data()); // back-left
        glEnd();
    }
    else if (currentShape == SPHERE) {
        // Draw sphere as a set of triangles
        glBegin(GL_TRIANGLES);
        // Iterate over all vertices of the sphere and draw them as triangles
        for (size_t i = 0; i < vertices.size(); i++) {
            // Set color (optional, you can set it based on the position or angle)
            glColor3f(0.5f, 0.5f, 1.0f);
            glVertex3fv(vertices[i].data());
        }
        glEnd();
    }
    else if (currentShape == CYLINDER) {
        // Draw the top and bottom circles as GL_TRIANGLE_FAN
        glBegin(GL_TRIANGLE_FAN);

        // Draw top circle (center first)
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex3fv(vertices[0].data()); // center
        for (size_t i = 1; i < vertices.size() / 2; i++) {
            glVertex3fv(vertices[i].data()); // top circle points
        }
        glEnd();

        // Draw bottom circle (center first)
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex3fv(vertices[vertices.size() / 2].data()); // center
        for (size_t i = vertices.size() / 2 + 1; i < vertices.size(); i++) {
            glVertex3fv(vertices[i].data()); // bottom circle points
        }
        glEnd();

        // Draw vertical sides as GL_QUAD_STRIP
        glBegin(GL_QUAD_STRIP);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        for (size_t i = 1; i < vertices.size() / 2; i++) {
            glVertex3fv(vertices[i].data());
            glVertex3fv(vertices[i + vertices.size() / 2].data());
        }
        glVertex3fv(vertices[1].data()); // Close the strip
        glVertex3fv(vertices[vertices.size() / 2 + 1].data());
        glEnd();
    }
}



// Add these helper functions
void crossProduct(float v1[], float v2[], float result[]) {
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void normalizeVector(float vector[]) {
    float length = sqrt(vector[0] * vector[0] +
        vector[1] * vector[1] +
        vector[2] * vector[2]);
    if (length != 0.0f) {
        vector[0] /= length;
        vector[1] /= length;
        vector[2] /= length;
    }
}



void keyboard(unsigned char key, int x, int y) {
    float cameraRight[] = { 0.0f, 0.0f, 0.0f };

    // Calculate right vector
    crossProduct(cameraFront, cameraUp, cameraRight);
    normalizeVector(cameraRight);

    switch (key) {

    case 'c':
        gridYes = !gridYes;
        break;

    case 'd':
        if (currentOperation == 1) {
            translationX += 0.1f;
        }
        break;
    case 'a':
        if (currentOperation == 1) {
            translationX -= 0.1f;
        }
        break;
    case 'w':
        if (currentOperation == 1) {
            translationY += 0.1f;
        }
        break;
    case 's':
        if (currentOperation == 1) {
            translationY -= 0.1f;
        }
        break;
    case 'q':
        if (currentOperation == 1) {
            translationZ += 0.1f;
        }
        break;
    case 'e':
        if (currentOperation == 1) {
            translationZ -= 0.1f;
        }
        break;



    case 'x':
        if (currentOperation == 2) {
            rotationAngleX += 5.0f;
        }
        else if (currentOperation == 3) {
            scalingFactorX += 0.1f;
        }

        else if (currentOperation == 4) {
            reflectionAxis = 'x';
            isReflecting = true;
            reflectionMatrix(reflectionAxis, reflectionTargetMat);
            reflectionProgress = 0.0f;
            reflectionAnimation(0);
        }
        else if (currentOperation == 6) {
            shearX1 += 0.1f;
            shearX2 += 0.1f;

        }
        break;

    case 'X':
        if (currentOperation == 2) {
            rotationAngleX -= 5.0f;

        }
        else if (currentOperation == 3) {
            scalingFactorX -= 0.1f;
        }

        else if (currentOperation == 6) {
            shearX1 -= 0.1f;
            shearX2 -= 0.1f;

        }
        break;


    case 'y':
        if (currentOperation == 2) {
            rotationAngleY += 5.0f;
        }
        else if (currentOperation == 4) {
            reflectionAxis = 'y';
            isReflecting = true;
            reflectionMatrix(reflectionAxis, reflectionTargetMat);
            reflectionProgress = 0.0f;
            reflectionAnimation(0);
        }
        else if (currentOperation == 3) {
            scalingFactorY += 0.1f;
        }
        else if (currentOperation == 6) {
            shearY1 += 0.1f;
            shearY2 += 0.1f;

        }
        break;

    case 'Y':
        if (currentOperation == 2) {
            rotationAngleY -= 5.0f;
        }

        else if (currentOperation == 3) {
            scalingFactorY -= 0.1f;
        }
        else if (currentOperation == 6) {
            shearY1 -= 0.1f;
            shearY2 -= 0.1f;
        }
        break;

    case 'z':
        if (currentOperation == 2) {
            rotationAngleZ += 5.0f;
        }
        else if (currentOperation == 3) {
            scalingFactorZ += 0.1f;
        }
        else if (currentOperation == 4) {
            reflectionAxis = 'z';
            isReflecting = true;
            reflectionMatrix(reflectionAxis, reflectionTargetMat);
            reflectionProgress = 0.0f;
            reflectionAnimation(0);
        }

        else if (currentOperation == 6) {
            shearZ1 += 0.1f;
            shearZ2 += 0.1f;

        }
        break;
    case 'Z':
        if (currentOperation == 2) {
            rotationAngleZ -= 5.0f;
        }

        else if (currentOperation == 3) {
            scalingFactorZ -= 0.1f;
        }

        else if (currentOperation == 6) {
            shearZ1 += 0.1f;
            shearZ2 += 0.1f;
        }
        break;



    case '+':
        if (currentOperation == 3) {
            scalingFactorX += 0.1f;
            scalingFactorY += 0.1f;
            scalingFactorZ += 0.1f;
        }
        break;
    case '-':
        if (currentOperation == 3) {
            scalingFactorX -= 0.1f;
            scalingFactorY -= 0.1f;
            scalingFactorZ -= 0.1f;
        }

    case 'o':
        if (currentOperation == 4) {
            reflectionAxis = 'o';
            isReflecting = true;
            reflectionMatrix('o', reflectionTargetMat);
            reflectionProgress = 0.0f;
            reflectionAnimation(0);
        }
        break;
    case 'n':
        if (currentOperation == 4) {
            reflectionAxis = 'n';
            isReflecting = true;
            reflectionMatrix('n', reflectionTargetMat);
            reflectionProgress = 0.0f;
            reflectionAnimation(0);
        }
        break;



    case 27:
        exit(0);

    case 'j':  // Strafe left
        cameraPos[0] -= cameraRight[0] * cameraSpeed;
        cameraPos[1] -= cameraRight[1] * cameraSpeed;
        cameraPos[2] -= cameraRight[2] * cameraSpeed;
        break;
    case 'l':  // Strafe right
        cameraPos[0] += cameraRight[0] * cameraSpeed;
        cameraPos[1] += cameraRight[1] * cameraSpeed;
        cameraPos[2] += cameraRight[2] * cameraSpeed;
        break;
    case 'i':  // Move forward
        cameraPos[0] += cameraFront[0] * cameraSpeed;
        cameraPos[1] += cameraFront[1] * cameraSpeed;
        cameraPos[2] += cameraFront[2] * cameraSpeed;
        break;
    case 'k':  // Move backward
        cameraPos[0] -= cameraFront[0] * cameraSpeed;
        cameraPos[1] -= cameraFront[1] * cameraSpeed;
        cameraPos[2] -= cameraFront[2] * cameraSpeed;
        break;
    case 'u':  // Move up
        cameraPos[1] += cameraSpeed;
        break;
    case 'h':  // Move down
        cameraPos[1] -= cameraSpeed;
        break;
    case 'm':  // Toggle mouse control
        mouseEnabled = !mouseEnabled;
        if (mouseEnabled) {
            glutSetCursor(GLUT_CURSOR_NONE);
            firstMouse = true;
        }
        else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        break;

    case 'r':  // Toggle lighting
        lightingEnabled = !lightingEnabled;
        break;

    case 'f':  // Switch between lighting properties
        currentLight = (currentLight + 1) % 4;
        break;

    case '8':  // Increase current property
        switch (currentLight) {
        case 0:  // Position
            lightPosition[1] += step;
            break;
        case 1:  // Ambient
            lightAmbient[0] = std::min(1.0f, lightAmbient[0] + step);
            lightAmbient[1] = std::min(1.0f, lightAmbient[1] + step);
            lightAmbient[2] = std::min(1.0f, lightAmbient[2] + step);
            break;
        case 2:  // Diffuse
            lightDiffuse[0] = std::min(1.0f, lightDiffuse[0] + step);
            lightDiffuse[1] = std::min(1.0f, lightDiffuse[1] + step);
            lightDiffuse[2] = std::min(1.0f, lightDiffuse[2] + step);
            break;
        case 3:  // Specular
            lightSpecular[0] = std::min(1.0f, lightSpecular[0] + step);
            lightSpecular[1] = std::min(1.0f, lightSpecular[1] + step);
            lightSpecular[2] = std::min(1.0f, lightSpecular[2] + step);
            break;
        }
        break;

    case '2':  // Decrease current property
        switch (currentLight) {
        case 0:  // Position
            lightPosition[1] -= step;
            break;
        case 1:  // Ambient
            lightAmbient[0] = std::max(0.0f, lightAmbient[0] - step);
            lightAmbient[1] = std::max(0.0f, lightAmbient[1] - step);
            lightAmbient[2] = std::max(0.0f, lightAmbient[2] - step);
            break;
        case 2:  // Diffuse
            lightDiffuse[0] = std::max(0.0f, lightDiffuse[0] - step);
            lightDiffuse[1] = std::max(0.0f, lightDiffuse[1] - step);
            lightDiffuse[2] = std::max(0.0f, lightDiffuse[2] - step);
            break;
        case 3:  // Specular
            lightSpecular[0] = std::max(0.0f, lightSpecular[0] - step);
            lightSpecular[1] = std::max(0.0f, lightSpecular[1] - step);
            lightSpecular[2] = std::max(0.0f, lightSpecular[2] - step);
            break;
        }
        break;

    case '4':  // Move light left
        lightPosition[0] -= step;
        break;

    case '6':  // Move light right
        lightPosition[0] += step;
        break;

    case '7':  // Move light forward
        lightPosition[2] -= step;
        break;

    case '9':  // Move light backward
        lightPosition[2] += step;
        break;
    }

    updateLighting();
    glutPostRedisplay();
}


void menu(int value) {
    switch (value) {
    case 1: // Translate
        currentOperation = 1;
        // Reset cube to visible position
        cubeVertices = {
            {-0.5f, -0.5f, -0.5f, 1.0f},
            {0.5f, -0.5f, -0.5f, 1.0f},
            {0.5f, 0.5f, -0.5f, 1.0f},
            {-0.5f, 0.5f, -0.5f, 1.0f},
            {-0.5f, -0.5f, 0.5f, 1.0f},
            {0.5f, -0.5f, 0.5f, 1.0f},
            {0.5f, 0.5f, 0.5f, 1.0f},
            {-0.5f, 0.5f, 0.5f, 1.0f}
        };
        break;
    case 2: // Scaling
        currentOperation = 3;
        break;
    case 3: // Rotating
        currentOperation = 2;
        break;
    case 4: // Shearing
        currentOperation = 6;
        break;
    case 5: // Reflection
        currentOperation = 4;
        break;

    case 6: // Reset
        translationX = translationY = translationZ = 0.0f;
        rotationAngleX = rotationAngleY = rotationAngleZ = 0.0f;
        scalingFactorX = scalingFactorY = scalingFactorZ = 1.0f;
        shearX1 = shearX2 = shearY1 = shearY2 = shearZ1 = shearZ2 = 0.0f;

        gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
            cameraPos[0] + cameraFront[0],
            cameraPos[1] + cameraFront[1],
            cameraPos[2] + cameraFront[2],
            cameraUp[0], cameraUp[1], cameraUp[2]);
        mouseEnabled = false;
        break;

    case 7: // Switch to Cube
        currentShape = CUBE;
        break;

    case 8: // Switch to Pyramid
        currentShape = PYRAMID;
        break;
    case 9: // Switch to Sphere
        currentShape = SPHERE;
        break;
    case 10: // Switch to Cylinder
        currentShape = CYLINDER;
        break;


        glutPostRedisplay();

    }
}



void mouseMovement(int x, int y) {
    if (!mouseEnabled) return;

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    cameraFront[0] = cos(yaw * PI / 180.0f) * cos(pitch * PI / 180.0f);
    cameraFront[1] = sin(pitch * PI / 180.0f);
    cameraFront[2] = sin(yaw * PI / 180.0f) * cos(pitch * PI / 180.0f);
    normalizeVector(cameraFront);

    // Wrap cursor to window edges
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    if (x <= 0) {
        glutWarpPointer(windowWidth - 2, y);
        lastX = windowWidth - 2;
    }
    else if (x >= windowWidth - 1) {
        glutWarpPointer(1, y);
        lastX = 1;
    }
    if (y <= 0) {
        glutWarpPointer(x, windowHeight - 2);
        lastY = windowHeight - 2;
    }
    else if (y >= windowHeight - 1) {
        glutWarpPointer(x, 1);
        lastY = 1;
    }

    glutPostRedisplay();
}


void mouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        glutCreateMenu(menu);
        glutAddMenuEntry("Translate", 1);
        glutAddMenuEntry("Scale", 2);
        glutAddMenuEntry("Rotate", 3);
        glutAddMenuEntry("Shear", 4);
        glutAddMenuEntry("Reflect", 5);
        glutAddMenuEntry("Switch to Cube", 7);
        glutAddMenuEntry("Switch to Pyramid", 8);

        glutAddMenuEntry("Reset ", 6);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
}




void display() {

    glClearColor(.7f, 1.0f, .7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //drawGrid();

    glLoadIdentity();

    // Update camera view
    gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
        cameraPos[0] + cameraFront[0],
        cameraPos[1] + cameraFront[1],
        cameraPos[2] + cameraFront[2],
        cameraUp[0], cameraUp[1], cameraUp[2]);


    drawGrid();

    std::vector<std::array<float, 4>> transformedVertices;
    transformation(transformedVertices);
    drawShape(transformedVertices);  // Changed from drawCube to drawShape

    // Now switch to 2D orthographic projection for text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluOrtho2D(-5, 5, -5, 5);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing for 2D rendering
    glDisable(GL_DEPTH_TEST);


    glColor3f(0.0f, 0.0f, 0.0f);
    drawText("Camera Controls:", -4.8f, -3.8f, 0.9f);
    drawText("'i/k': Move Forward/Backward", -4.8f, -4.0f, 0.9f);
    drawText("'j/l': Strafe Left/Right", -4.8f, -4.2f, 0.9f);
    drawText("'u/h': Move Up/Down", -4.8f, -4.4f, 0.9f);
    drawText("'m': Toggle Mouse Look", -4.8f, -4.6f, 0.9f);


    glColor3f(0.0f, 0.0f, 0.0f);  // Black color for text
    drawText("Sanskar Rathore", 4.0f, 4.5f, 1.0f);
    drawText("500108014", 4.0f, 4.3f, 1.0f);
    drawText("3D Transformation", -.2f, 4.5f, 0.9f);

    drawText("Right Click for Transformations Menu", -4.8f, -3.5f, 0.9f);

    drawText("INSTRUCTIONS:", -4.8f, -3.1f, 0.9f);  // Instructions heading

    if (currentOperation == 0) {  // Lighting Controls
        std::string lightMode;
        switch (currentLight) {
        case 0: lightMode = "Current: Light Position"; break;
        case 1: lightMode = "Current: Ambient Light"; break;
        case 2: lightMode = "Current: Diffuse Light"; break;
        case 3: lightMode = "Current: Specular Light"; break;
        }
        /* drawText("Lighting Controls:", -4.8f, 4.5f, 0.9f);
         drawText("'l': Toggle Lighting", -4.8f, 4.3f, 0.9f);
         drawText("'m': Switch Light Property", -4.8f, 4.1f, 0.9f);
         drawText("'8/2': Increase/Decrease Current Property", -4.8f, 3.9f, 0.9f);
         drawText("'4/6': Move Light Left/Right", -4.8f, 3.7f, 0.9f);
         drawText("'7/9': Move Light Forward/Backward", -4.8f, 3.5f, 0.9f);
         drawText(lightMode, -4.5f, 3.1f, 0.9f);*/
    }

    if (currentOperation == 1) {  // Translation Controls
        drawText("Translation:", -4.8f, 4.5f, 0.9f);
        drawText("'W': Move UP", -4.8f, 4.3f, 0.9f);
        drawText("'S': Move DOWN", -4.8f, 4.1f, 0.9f);
        drawText("'A': Move LEFT", -4.8f, 3.9f, 0.9f);
        drawText("'D': Move RIGHT", -4.8f, 3.7f, 0.9f);
        drawText("'Q': Move FORWARD", -4.8f, 3.5f, 0.9f);
        drawText("'E': Move BACKWARD", -4.8f, 3.3f, 0.9f);
    }

    if (currentOperation == 2) {  // Rotation Controls
        drawText("Rotation:", -4.8f, 4.5f, 0.9f);
        drawText("'x': Rotate around X-Axis", -4.8f, 4.3f, 0.9f);
        drawText("'y': Rotate around Y-Axis", -4.8f, 4.1f, 0.9f);
        drawText("'z': Rotate around Z-Axis", -4.8f, 3.9f, 0.9f);
        drawText("'# capslock ON + x/y/z': Rotate in the Negative Direction of X/Y/Z", -4.8f, 3.7f, 0.9f);
    }

    if (currentOperation == 3) {  // Scaling Controls
        drawText("Scaling:", -4.8f, 4.5f, 0.9f);
        drawText("'x': Scale along X-Axis", -4.8f, 4.3f, 0.9f);
        drawText("'y': Scale along Y-Axis", -4.8f, 4.1f, 0.9f);
        drawText("'z': Scale along Z-Axis", -4.8f, 3.9f, 0.9f);
    }

    if (currentOperation == 4) {  // Reflection Controls
        drawText("Reflection:", -4.8f, 4.5f, 0.9f);
        drawText("'o': Reflect over XY-Plane", -4.8f, 4.3f, 0.9f);
        drawText("'x': Reflect over X-Axis", -4.8f, 4.1f, 0.9f);
        drawText("'y': Reflect over Y-Axis", -4.8f, 3.9f, 0.9f);
        drawText("'n': # Remove All Reflections", -4.8f, 3.7f, 0.9f);
    }

    if (currentOperation == 6) {  // Shearing Controls
        drawText("Shearing:", -4.8f, 4.5f, 0.9f);
        drawText("'x': Shear along X-Axis", -4.8f, 4.3f, 0.9f);
        drawText("'y': Shear along Y-Axis", -4.8f, 4.1f, 0.9f);
    }

    // Restore matrices and enable depth testing as before
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glutSwapBuffers();
}

void init() {
    reflectionMatrix('n', reflectionMat);


    // Initialize lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Set material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D TRANSFORMATIONS");
    glEnable(GL_DEPTH_TEST);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMovement);
    glutFullScreen();
    glutMainLoop();

    return 0;
}