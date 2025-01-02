#include <glut.h>
#include <cmath>
#include <vector>
#include <random>

const float PI = 3.14159265358979323846f;

int windowWidth = 1024;
int windowHeight = 768;

void renderBitmapString(float x, float y, void* font, const std::string& str) {
    glColor3f(1.0f, 1.0f, 1.0f); // White color for text
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(font, c);
    }
}

class SpiralGalaxy {
public:
    struct Star {
        float x, y, z;
        float r, g, b;
        float size;
    };

    std::vector<Star> generateSpiral(int numStars) {
        std::vector<Star> stars;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);

        for (int i = 0; i < numStars; ++i) {
            // Spiral parameters
            float t = static_cast<float>(i) / numStars;

            // Logarithmic spiral equation
            float a = 0.1f;  // Spiral tightness
            float b = 0.3f;  // Spiral spread
            float angle = 4 * t * 2 * PI;  // Rotation factor

            // Radius calculation with logarithmic spread
            float radius = a * exp(b * angle);

            Star star;
            // Precise spiral positioning
            float spiralX = radius * cos(angle);
            float spiralY = radius * sin(angle);

            // Add some depth variation
            float depth = (dis(gen) - 0.5f) * 5.0f;

            // Original star positioning
            star.x = spiralX;
            star.y = spiralY;
            star.z = depth;

            // Color gradient based on position
            float dist = sqrt(star.x * star.x + star.y * star.y);
            float normalizedDist = std::min(1.0f, dist / 10.0f);

            // Core to outer arm color transition
            star.r = 1.0f - 0.7f * normalizedDist;  // Bright core
            star.g = 0.6f - 0.4f * normalizedDist;  // Gradient green
            star.b = 0.3f + 0.7f * normalizedDist;  // Blue outer arms

            // Size variation
            star.size = 3.0f * (1.0f - normalizedDist);

            // Add some noise to break perfect symmetry
            star.x += (dis(gen) - 0.5f) * 0.5f;
            star.y += (dis(gen) - 0.5f) * 0.5f;

            stars.push_back(star);
        }

        return stars;
    }
};

class CameraController {
public:
    float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 20.0f;  // Initial camera position
    float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;  // Look-at point
    float upX = 0.0f, upY = 1.0f, upZ = 0.0f;  // Up vector

    // Camera movement speeds
    float movementSpeed = 0.5f;
    float rotationSpeed = 0.05f;

    // Camera rotation angles
    float yaw = 0.0f;
    float pitch = 0.0f;

    void moveForward() {
        // Calculate forward direction
        float dx = centerX - eyeX;
        float dy = centerY - eyeY;
        float dz = centerZ - eyeZ;
        float length = std::sqrt(dx * dx + dy * dy + dz * dz);

        // Normalize and scale by movement speed
        eyeX += (dx / length) * movementSpeed;
        eyeY += (dy / length) * movementSpeed;
        eyeZ += (dz / length) * movementSpeed;

        // Move look-at point proportionally
        centerX += (dx / length) * movementSpeed;
        centerY += (dy / length) * movementSpeed;
        centerZ += (dz / length) * movementSpeed;
    }

    void moveBackward() {
        float dx = centerX - eyeX;
        float dy = centerY - eyeY;
        float dz = centerZ - eyeZ;
        float length = std::sqrt(dx * dx + dy * dy + dz * dz);

        eyeX -= (dx / length) * movementSpeed;
        eyeY -= (dy / length) * movementSpeed;
        eyeZ -= (dz / length) * movementSpeed;

        centerX -= (dx / length) * movementSpeed;
        centerY -= (dy / length) * movementSpeed;
        centerZ -= (dz / length) * movementSpeed;
    }

    void strafeLeft() {
        // Compute perpendicular vector to look direction and up vector
        float dx = centerX - eyeX;
        float dy = centerY - eyeY;
        float dz = centerZ - eyeZ;

        float perpX = upY * dz - upZ * dy;
        float perpY = upZ * dx - upX * dz;
        float perpZ = upX * dy - upY * dx;

        // Normalize
        float length = std::sqrt(perpX * perpX + perpY * perpY + perpZ * perpZ);

        eyeX -= (perpX / length) * movementSpeed;
        eyeY -= (perpY / length) * movementSpeed;
        eyeZ -= (perpZ / length) * movementSpeed;

        centerX -= (perpX / length) * movementSpeed;
        centerY -= (perpY / length) * movementSpeed;
        centerZ -= (perpZ / length) * movementSpeed;
    }

    void strafeRight() {
        // Similar to strafeLeft, but in opposite direction
        float dx = centerX - eyeX;
        float dy = centerY - eyeY;
        float dz = centerZ - eyeZ;

        float perpX = upY * dz - upZ * dy;
        float perpY = upZ * dx - upX * dz;
        float perpZ = upX * dy - upY * dx;

        // Normalize
        float length = std::sqrt(perpX * perpX + perpY * perpY + perpZ * perpZ);

        eyeX += (perpX / length) * movementSpeed;
        eyeY += (perpY / length) * movementSpeed;
        eyeZ += (perpZ / length) * movementSpeed;

        centerX += (perpX / length) * movementSpeed;
        centerY += (perpY / length) * movementSpeed;
        centerZ += (perpZ / length) * movementSpeed;
    }

    void rotateView(float deltaX, float deltaY) {
        // Update yaw and pitch based on mouse movement
        yaw += deltaX * rotationSpeed;
        pitch += deltaY * rotationSpeed;

        // Limit pitch to prevent camera flipping
        pitch = std::max(-89.0f, std::min(89.0f, pitch));

        // Compute new look direction using spherical coordinates
        float cosP = std::cos(pitch * PI / 180.0f);
        float sinP = std::sin(pitch * PI / 180.0f);
        float cosY = std::cos(yaw * PI / 180.0f);
        float sinY = std::sin(yaw * PI / 180.0f);

        float dx = cosP * sinY;
        float dy = sinP;
        float dz = cosP * cosY;

        // Update look-at point relative to eye position
        centerX = eyeX + dx;
        centerY = eyeY + dy;
        centerZ = eyeZ + dz;
    }
};


// Global variables
std::vector<SpiralGalaxy::Star> galaxyStars;
double galaxyRotationZ = 0.0f;
double rotationSpeed = 1.0f;
bool isPaused = false;

// Camera controller
CameraController camera;

// Mouse tracking
int lastMouseX = 0, lastMouseY = 0;
bool mouseTracking = false;


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Fixed camera setup
    gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ,   // Eye position
        camera.centerX, camera.centerY, camera.centerZ,    // Look at point
        camera.upX, camera.upY, camera.upZ);   // Up vector

    // Apply ONLY Z-axis rotation to the entire galaxy
    glRotatef(galaxyRotationZ, 0, 0, 1);

    // Render stars
    glBegin(GL_POINTS);
    for (const auto& star : galaxyStars) {
        glColor3f(star.r, star.g, star.b);
        glPointSize(star.size * 2);
        glVertex3f(star.x, star.y, star.z);
    }
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render text in bottom right corner
    std::string authorText = "Sanskar Rathore, 500108014";
    int textWidth = 0;
    for (char c : authorText) {
        textWidth += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, c);
    }

    renderBitmapString(
        windowWidth - textWidth - 10,  // X position (right-aligned with padding)
        10,                            // Y position (bottom with padding)
        GLUT_BITMAP_HELVETICA_12,      // Font
        authorText                     // Text to render
    );

    // Restore previous projection and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();


    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    if (!isPaused) {
        // Rotate galaxy around Z-axis
        galaxyRotationZ += rotationSpeed;

        // Reset rotation to prevent potential floating-point overflow
        if (galaxyRotationZ >= 360.0f) {
            galaxyRotationZ -= 360.0f;
        }
        else if (galaxyRotationZ < 0.0f) {
            galaxyRotationZ += 360.0f;
        }

        glutPostRedisplay();
    }
}

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': camera.moveForward(); break;    // Move forward
    case 's': camera.moveBackward(); break;   // Move backward
    case 'a': camera.strafeLeft(); break;     // Strafe left
    case 'd': camera.strafeRight(); break;    // Strafe right
    case 'e': // Increase rotation speed
        rotationSpeed += 0.5f;
        break;
    case 'f': // Decrease rotation speed
        rotationSpeed -= 0.5f;
        break;
    case 'x': // Reverse rotation
        rotationSpeed = -rotationSpeed;
        break;
    case ' ': // Pause/Resume
        isPaused = !isPaused;
        break;
    case 'r': // Reset rotation
        camera.eyeX = 0.0f;
        camera.eyeY = 0.0f;
        camera.eyeZ = 20.0f;
        camera.centerX = 0.0f;
        camera.centerY = 0.0f;
        camera.centerZ = 0.0f;
        galaxyRotationZ = 0.0f;
        rotationSpeed = 1.0f;
        isPaused = false;
        break;
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (mouseTracking) {
        // Calculate mouse movement deltas
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        // Rotate camera view based on mouse movement
        camera.rotateView(deltaX, -deltaY);

        // Update last mouse position
        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Start mouse tracking when right button is pressed
            mouseTracking = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP) {
            // Stop mouse tracking when right button is released
            mouseTracking = false;
        }
    }
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("3D Spiral Galaxy Explorer");

    // OpenGL initialization
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    // Enable point smoothing for better appearance
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    // Generate galaxy stars
    SpiralGalaxy galaxyGenerator;
    galaxyStars = galaxyGenerator.generateSpiral(50000);  // High star count

    // Set up GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseMotion);
    glutFullScreen();
    // Print controls
    printf("Controls:\n");
    printf("WASD : Move camera\n");
    printf("Right Mouse : Look around\n");
    printf("'e' : Increase galaxy rotation speed\n");
    printf("'q' : Decrease galaxy rotation speed\n");
    printf("'x' : Reverse galaxy rotation\n");
    printf("' ' : Pause/Resume galaxy rotation\n");
    printf("'r' : Reset camera and galaxy\n");

    // Enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}