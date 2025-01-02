#include <freeglut.h>
#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>


void renderBitmapString(float x, float y, void* font, const std::string& str) {
    glColor3f(1.0f, 1.0f, 1.0f); // White color for text
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(font, c);
    }
}

// Simple color structure
struct Color {
    float r, g, b;
    Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f)
        : r(red), g(green), b(blue) {}
};

// Constants and settings
const double M_PI = 3.14159265358979323846;
int windowWidth = 1024;
int windowHeight = 768;
float globalZoom = 1.0f;
float minZoom = 0.1f;
float maxZoom = 10.0f;
float rotationAngle = 0.0f;
float panX = 0.0f;
float panY = 0.0f;
float panStep = 0.1f;
float cameraZ = 5.0f;
float cameraSpeed = 0.01f;
float timeScale = 1.0f;

// Pattern-specific parameters
float centralSphereRadius = 0.2f;
float centralSphereShrinkRate = 0.0005f;
float minSphereRadius = 0.007f;
const int numArms = 4;
const int particlesPerArm = 500;
const float armSeparation = 2.0f * M_PI / numArms;
const float armSpread = 0.1f;
const float armLength = 5.0f;
const float armTwist = 10.0f;

// Added rotation speed controls
float armRotationSpeed = 0.5f;
float pointRotationSpeed = 0.002f;
const float speedIncrement = 0.1f;
const float minSpeed = 0.0f;
const float maxSpeed = 2.0f;
//float armRotationSpeed = 0.5f;
//float armRotation = 0.0f;

float fractalRotation = 0.0f;
float fractalScale = 1.0f;
float fractalEvolution = 0.0f;

// Pattern selection
enum GalaxyPattern {
    FRACTAL_SPIRAL,
    ENHANCED_REALISTIC
};
GalaxyPattern currentPattern = FRACTAL_SPIRAL;

// Forward declarations of our galaxy classes
class FractalGalaxy;
class EnhancedGalaxy;
class StarField;

// Global instances
FractalGalaxy* fractalGalaxy = nullptr;
EnhancedGalaxy* enhancedGalaxy = nullptr;
StarField* starField = nullptr;

// Star class implementation
class Star {
public:
    float mass;
    float temperature;
    float luminosity;
    float age;
    Color color;
    float size;

    Star(std::mt19937& gen) {
        std::uniform_real_distribution<float> massDist(0.08f, 50.0f);
        mass = massDist(gen);
        temperature = 4600.0f * std::pow(mass, 0.5f);
        luminosity = std::pow(mass, 3.5f);
        age = 0.0f;
        calculateColor();
        size = std::sqrt(luminosity) * 0.5f;
    }

private:
    void calculateColor() {
        float temp = temperature / 1000.0f;
        float r = 1.0f, g = 0.0f, b = 0.0f;

        if (temp < 3.0f) {
            r = 1.0f;
            g = temp / 3.0f;
            b = 0.0f;
        }
        else if (temp < 6.0f) {
            r = 1.0f;
            g = std::min(1.0f, 0.4f * temp);
            b = std::min(1.0f, 0.4f * (temp - 3.0f));
        }
        else {
            r = std::max(0.0f, 2.0f - temp / 6.0f);
            g = std::max(0.0f, 1.8f - temp / 7.0f);
            b = 1.0f;
        }

        color = Color(r, g, b);
    }
};

// StarField implementation
class StarField {
private:
    struct StarPoint {
        float x, y, z;
        float basebrightness;  // Persistent base brightness
        float twinkleSpeed;
        float size;
        float currentBrightness;  // Dynamic brightness that changes
        float twinklePhase;  // Unique phase for each star
    };

    std::vector<StarPoint> stars;
    std::mt19937 gen;
    float totalElapsedTime;  // Track total elapsed time independently

public:
    StarField(int numStars) : gen(std::random_device{}()), totalElapsedTime(0.0f) {
        std::uniform_real_distribution<> posDis(-50.0, 50.0);
        std::uniform_real_distribution<> speedDis(1.0, 3.0);
        std::uniform_real_distribution<> sizeDis(0.5, 2.0);

        stars.resize(numStars);
        for (auto& star : stars) {
            star.x = posDis(gen);
            star.y = posDis(gen);
            star.z = posDis(gen) - 25.0;
            star.basebrightness = 0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f;
            star.twinkleSpeed = speedDis(gen);
            star.size = sizeDis(gen);
            star.twinklePhase = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;  // Random initial phase
            star.currentBrightness = 1;
        }
    }

    void update(float deltaTime) {
        totalElapsedTime += deltaTime;

        for (auto& star : stars) {
            // More sophisticated twinkling using phase-shifted sine wave
            star.currentBrightness = 1 * (
                0.5f + 0.5f * std::sin(totalElapsedTime * star.twinkleSpeed + star.twinklePhase)
                );
        }
    }

    void render() {
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        glBegin(GL_POINTS);
        for (const auto& star : stars) {
            float distanceFactor = 1.0f / (std::abs(star.z - cameraZ) * 0.1f + 1.0f);
            float finalBrightness = star.currentBrightness * distanceFactor;

            glColor4f(0.9f, 0.95f, 1.0f, finalBrightness);
            glPointSize(star.size * distanceFactor * 2.0f);
            glVertex3f(star.x, star.y, star.z);
        }
        glEnd();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
};
class FractalGalaxy {
private:
    std::mt19937 gen;



public:
    float spiralScale;
    std::vector<float> vertices;
    std::vector<float> colors;

    void rotatePoints(float angle) {
        for (size_t i = 0; i < vertices.size(); i += 3) {
            float x = vertices[i];
            float y = vertices[i + 1];
            vertices[i] = x * std::cos(angle) - y * std::sin(angle);
            vertices[i + 1] = x * std::sin(angle) + y * std::cos(angle);
        }
    }

    void renderGalaxyArms() {
        glBegin(GL_POINTS); // Use points to render particles

        int blueArms = 4; // Number of blue arms
        int totalArms = numArms + blueArms; // Total arms including blue arms

        for (int arm = 0; arm < totalArms; ++arm) {
            float armOffset = arm * armSeparation; // Starting angle for each arm

            for (int i = 0; i < particlesPerArm; ++i) {
                // Particle distance from the center
                float distance = ((float)i / particlesPerArm) * armLength;

                // Calculate the angle for the particle with arm twist
                float angle = armOffset + distance * armTwist;

                // Add randomness for spread
                float x = distance * cos(angle) + (rand() % 100 / 1000.0f - 0.05f) * armSpread;
                float y = distance * sin(angle) + (rand() % 100 / 1000.0f - 0.05f) * armSpread;

                // Set particle color
                if (arm >= numArms) {
                    // Blue arms
                    float blueIntensity = 1.0f - (distance / armLength);
                    glColor4f(0.0f, 0.0f, blueIntensity, 1.0f);
                }
                else {
                    // Original arms (white/gradient)
                    float intensity = 1.0f - (distance / armLength);
                    glColor4f(intensity, intensity, intensity, 1.0f);
                }

                // Render particle
                glVertex3f(x, y, -1.0f);
            }
        }

        glEnd();
    }

    FractalGalaxy() : gen(std::random_device{}()), spiralScale(10.0f) {
        generateSpiralFractalPoints(vertices, colors, 0.0f, 0.0f, 0.0f, spiralScale, 4);
    }

    void generateSpiralFractalPoints(std::vector<float>& vertices,
        std::vector<float>& colors,
        float centerX, float centerY, float centerZ,
        float radius, int depth, float angleOffset = 0.0f) {

        float timeInfluence = std::sin(fractalEvolution) * 0.3f;
        float dynamicRadius = radius * (1.0f + timeInfluence);
        float dynamicSpiral = 0.3f + std::cos(fractalEvolution * 0.5f) * 0.1f;


        if (depth <= 0) return;

        std::uniform_real_distribution<> dis(-1.0, 1.0);
        int pointsPerLevel = 1000 * (depth + 1); // Increased number of points

        float spiralTightness = 0.3f; // Adjusted for more spiral arms
        //float armCount = 4.0f; // Number of spiral arms

        float armCount = 4.0f + std::sin(fractalEvolution) * 2.0f;
        spiralTightness = dynamicSpiral;


        for (int i = 0; i < pointsPerLevel; ++i) {
            float t = i / static_cast<float>(pointsPerLevel);
            float spiralAngle = t * 2 * M_PI * 10 + angleOffset; // More spiral revolutions
            float spiralRadius = radius * (0.1f + t * 0.9f); // Modified radius distribution


            // Create multiple spiral arms
            for (int arm = 0; arm < armCount; ++arm) {
                float armAngle = spiralAngle + (2 * M_PI * arm) / armCount;
                float r = spiralRadius * std::pow(std::abs(dis(gen)), 1.5f / depth);

                // Enhanced spiral equations for more complex patterns
                float x = centerX + r * std::cos(armAngle) *
                    (1 + timeInfluence * std::sin(armAngle * 3));
                float y = centerY + r * std::sin(armAngle) *
                    (1 + timeInfluence * std::cos(armAngle * 3));
                float z = centerZ + r * 0.2f * std::sin(armAngle * 2 + fractalEvolution);

                // Dynamic noise based on fractalEvolution
                float noiseScale = 0.05f * radius * (1 - t) *
                    (1 + std::sin(fractalEvolution) * 0.2f);
                float noiseX = dis(gen) * noiseScale;
                float noiseY = dis(gen) * noiseScale;
                float noiseZ = dis(gen) * noiseScale;

                vertices.push_back(x + noiseX);
                vertices.push_back(y + noiseY);
                vertices.push_back(z + noiseZ);

                // Enhanced color calculation for more vibrancy
                float hue = armAngle / (2 * M_PI) + t * 0.5f + fractalEvolution * 0.1f;
                float sat = 0.8f + 0.2f * std::sin(armAngle * 5 + fractalEvolution);
                float val = 0.7f + 0.3f * std::cos(armAngle * 3 + fractalEvolution);

                // Convert HSV to RGB with enhanced vibrancy
                float h = std::fmod(hue * 6.0f, 6.0f);
                float f = h - std::floor(h);
                float p = val * (1.0f - sat);
                float q = val * (1.0f - sat * f);
                float t = val * (1.0f - sat * (1.0f - f));

                float  g, b;
                switch (static_cast<int>(h)) {
                case 0: r = val; g = t; b = p; break;
                case 1: r = q; g = val; b = p; break;
                case 2: r = p; g = val; b = t; break;
                case 3: r = p; g = q; b = val; break;
                case 4: r = t; g = p; b = val; break;
                default: r = val; g = p; b = q; break;
                }

                colors.push_back(r);
                colors.push_back(g);
                colors.push_back(b);
                colors.push_back(0.7f + 0.3f * (1.0f - t)); // Dynamic alpha
            }
        }

        // recursive patterns
        if (depth > -1) {
            float subRadius = radius * 0.4f;
            float angleStep = 2 * M_PI / 3;
            for (int i = 0; i < 4; ++i) {
                float angle = i * angleStep;
                generateSpiralFractalPoints(vertices, colors,
                    centerX + subRadius * std::cos(angle),
                    centerY + subRadius * std::sin(angle),
                    centerZ + subRadius * 0.1f * std::sin(fractalEvolution),
                    radius * 0.3f,
                    depth - 1,
                    angle);
            }
        }
    }

};

FractalGalaxy galaxy;

struct GalaxyParams {
    float bulgeSize;        // Central bulge size
    float diskThickness;    // Thickness of the galactic disk
    float dustLaneIntensity;// Intensity of dust lanes
    float starDensity;      // Density of stars
    float spiralTightness;  // How tightly wound the spiral arms are
    float barLength;        // Length of the galactic bar
    bool hasBar;           // Whether this is a barred spiral galaxy
    int numArms;           // Number of spiral arms
    float armWidth;        // Width of spiral arms

    GalaxyParams() :
        bulgeSize(0.3f),
        diskThickness(0.1f),
        dustLaneIntensity(0.4f),
        starDensity(5.0f),
        spiralTightness(0.5f),
        barLength(0.6f),
        hasBar(true),
        numArms(4),
        armWidth(0.15f) {}
};

GalaxyParams galaxyParams;

class EnhancedGalaxy {
private:
    std::mt19937 gen;
    std::vector<float> vertices;
    std::vector<float> colors;
    float spiralScale;

    float generateGaussian(float mean, float stddev) {
        // Ensure minimum standard deviation to prevent assertion failure
        float safeStddev = std::max(stddev, 1e-6f);  // Use small positive number as minimum
        std::normal_distribution<float> dist(mean, safeStddev);
        return dist(gen);
    }

    void generateStar(float x, float y, float z, float density) {
        Star star(gen);

        // Add position
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

        // Add color with density-based alpha
        colors.push_back(star.color.r);
        colors.push_back(star.color.g);
        colors.push_back(star.color.b);
        colors.push_back(density * galaxyParams.starDensity);
    }

    void generateBulge() {
        int bulgePoints = 50000;
        float bulgeRadius = galaxyParams.bulgeSize * spiralScale;

        for (int i = 0; i < bulgePoints; ++i) {
            // Generate point in spherical coordinates with exponential density profile
            float r = bulgeRadius * std::pow(std::generate_canonical<float, 10>(gen), 0.5f);
            float theta = 2.0f * M_PI * std::generate_canonical<float, 10>(gen);
            float phi = std::acos(2.0f * std::generate_canonical<float, 10>(gen) - 1.0f);

            // Convert to Cartesian coordinates
            float x = r * std::sin(phi) * std::cos(theta);
            float y = r * std::sin(phi) * std::sin(theta);
            float z = r * std::cos(phi) * galaxyParams.diskThickness;

            // Calculate density falloff from center
            float density = std::exp(-r / bulgeRadius);
            generateStar(x, y, z, density);
        }
    }

    void generateSpiralArms() {
        int pointsPerArm = 100000; // Total points per arm
        float armOffset = 2.0f * M_PI / galaxyParams.numArms; // Angular offset between arms
        float a = 0.1f; // Logarithmic spiral scaling factor
        float b = galaxyParams.spiralTightness; // Spiral tightness factor

        for (int arm = 0; arm < galaxyParams.numArms; ++arm) {
            for (int i = 0; i < pointsPerArm; ++i) {
                float t = static_cast<float>(i) / pointsPerArm; // Normalized position along the arm
                float theta = t * 6.0f * M_PI + arm * armOffset; // Extend over 3 rotations
                float r = a * std::exp(b * theta); // Logarithmic spiral equation

                // Add random perturbations for realistic distribution
                float randR = r + generateGaussian(0.0f, galaxyParams.armWidth * r);
                float randTheta = theta + generateGaussian(0.0f, 0.2f); // Small angular offset

                float x = randR * std::cos(randTheta); // Convert to Cartesian coordinates
                float y = randR * std::sin(randTheta);
                float z = generateGaussian(0.0f, galaxyParams.diskThickness * std::exp(-r / spiralScale));

                // Generate star with density scaling
                float density = std::exp(-r / spiralScale);
                generateStar(x, y, z, density);
            }
        }
    }



public:
    EnhancedGalaxy() : gen(std::random_device{}()), spiralScale(5.0f) {
        generateBulge();
        generateSpiralArms();
    }

    void render() {
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin(GL_POINTS);
        for (size_t i = 0; i < vertices.size(); i += 3) {
            float depth = std::abs(vertices[i + 2] - cameraZ);
            float pointSize = 3.0f * (1.0f / (depth * globalZoom + 0.1f));
            glPointSize(std::min(10.0f, pointSize));

            size_t colorIdx = (i / 3) * 4;
            glColor4f(
                colors[colorIdx],
                colors[colorIdx + 1],
                colors[colorIdx + 2],
                colors[colorIdx + 3] * (1.0f / (depth * globalZoom + 0.1f))
            );

            glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
        }
        glEnd();
    }

    void renderGalaxyCenter() {
        // Center glow parameters
        float centerX = 0.0f;  // X-coordinate of the galaxy center
        float centerY = 0.0f;  // Y-coordinate of the galaxy center
        int segments = 100;    // Number of segments for smooth circle
        float maxLuminance = 1.0f; // Brightness of the center

        glBegin(GL_TRIANGLE_FAN);

        // Central bright point
        glColor4f(maxLuminance, maxLuminance, maxLuminance, 1.0f);
        glVertex3f(centerX, centerY, -1.0f); // At infinity (z = -1.0)

        // Outer glow with gradient
        for (int i = 0; i <= segments; ++i) {
            float angle = 2.0f * M_PI * i / segments;
            float x = centralSphereRadius * cos(angle);
            float y = centralSphereRadius * sin(angle);

            // Gradient fade effect (alpha reduces with radius)
            float luminance = 0.1f; // Outer ring has minimal luminance
            glColor4f(maxLuminance * luminance, maxLuminance * luminance, maxLuminance * luminance, 0.0f);
            glVertex3f(centerX + x, centerY + y, -1.0f);
        }

        glEnd();
    }
    
    void renderGalaxyArms() {
        glBegin(GL_POINTS); // Use points to render particles

        int blueArms = 12; // Number of additional blue arms
        int totalArms = numArms + blueArms; // Total arms including blue arms

        for (int arm = 0; arm < totalArms; ++arm) {
            float armOffset = arm * armSeparation; // Starting angle for each arm

            for (int i = 0; i < particlesPerArm; ++i) {
                // Particle distance from the center
                float distance = ((float)i / particlesPerArm) * armLength;

                // Calculate the angle for the particle with arm twist
                float angle = armOffset + distance * armTwist;

                // Add randomness for spread
                float x = distance * cos(angle) + (rand() % 100 / 1000.0f - 0.05f) * armSpread;
                float y = distance * sin(angle) + (rand() % 100 / 1000.0f - 0.05f) * armSpread;

                // Set particle color
                if (arm >= numArms) {
                    // Blue arms
                    float blueIntensity = 1.0f - (distance / armLength);
                    glColor4f(0.0f, 0.0f, blueIntensity, 1.0f);
                }
                else {
                    // Original arms (white/gradient)
                    float intensity = 1.0f - (distance / armLength);
                    glColor4f(intensity, intensity, intensity, 1.0f);
                }

                // Render particle
                glVertex3f(x, y, -1.0f);
            }
        }

        glEnd();
    }


};

namespace {
    EnhancedGalaxy* galaxy2 = nullptr;
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void switchGalaxyPattern() {
    // Clear any existing rendering state
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentPattern == FRACTAL_SPIRAL) {
        currentPattern = ENHANCED_REALISTIC;
        // Clean up fractal galaxy resources
        if (fractalGalaxy) {
            delete fractalGalaxy;
            fractalGalaxy = nullptr;
        }
        // Create enhanced galaxy if needed
        if (!enhancedGalaxy) {
            enhancedGalaxy = new EnhancedGalaxy();
        }
    }
    else {
        currentPattern = FRACTAL_SPIRAL;
        // Clean up enhanced galaxy resources
        if (enhancedGalaxy) {
            delete enhancedGalaxy;
            enhancedGalaxy = nullptr;
        }
        // Create fractal galaxy if needed
        if (!fractalGalaxy) {
            fractalGalaxy = new FractalGalaxy();
        }
    }

    // Force immediate redisplay
    glutPostRedisplay();
}

// Add these near your other global constants
float rotationSpeed = 0.05f;  // Increased rotation speed (adjust this value to make it faster/slower)
float currentRotation = 0.0f; // Tracks current rotation angle
float armRotation = 0.0f;       // Rotation angle of arms
//float armRotationSpeed = 0.05f; // Speed of rotation


float globalRotationSpeed = 0.05f;  // Overall rotation speed
float globalRotationAngle = 0.0f;   // Current global rotation angle
bool isPaused = false;  // Global pause state

void animate() {
    if (!isPaused) {

        if (starField) {
            starField->update(0.016f);  // Roughly 60 FPS 
        }

        if (centralSphereRadius > minSphereRadius) {
            centralSphereRadius -= centralSphereShrinkRate;
        }
        // Global rotation
        globalRotationAngle += globalRotationSpeed;
        if (globalRotationAngle >= 360.0f) {
            globalRotationAngle -= 360.0f;
        }

        // Existing animations
        armRotation += armRotationSpeed;
        if (armRotation >= 360.0f) {
            armRotation -= 360.0f;
        }

        // Camera zoom-out animation
        rotationAngle += 0.001f;
        cameraZ -= cameraSpeed * timeScale;
        if (cameraZ < 0.1f) {
            cameraZ = 5.0f;
            timeScale *= 0.95f;
            centralSphereRadius = 0.2f;
            centralSphereShrinkRate = 0.0005f;
            minSphereRadius = 0.001f;
        }

        glutPostRedisplay();
    }
}

// Add keyboard controls for rotation speed
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'p':  // Pattern switch
        switchGalaxyPattern();
        break;
    case 'q':  // Increase global rotation speed
        globalRotationSpeed = std::min(globalRotationSpeed + 0.01f, 0.5f);
        break;
    case 'a':  // Decrease global rotation speed
        globalRotationSpeed = std::max(globalRotationSpeed - 0.01f, -0.5f);
        break;
    //case 'r':  // Reverse global rotation direction
    //    globalRotationSpeed *= -1.5f;
    //    break;
    case ' ':  // Toggle pause/resume
        isPaused = !isPaused;
        break;
        break;
   case 'r': globalRotationSpeed *= 1.5f; break;        // Increase rotation speed
    case 'f': globalRotationSpeed *= 0.75f; break;       // Decrease rotation speed
    case 'x': globalRotationSpeed *= -1.0f; break;       // Reverse rotation direction

    case 27:  // ESC key
        delete fractalGalaxy;
        delete enhancedGalaxy;
        delete starField;
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y) {
    if (direction > 0) {
        globalZoom /= 1.1f;  // Zoom in
    }
    else {
        globalZoom *= 1.1f;  // Zoom out
    }

    globalZoom = std::max(minZoom, std::min(maxZoom, globalZoom));
    glutPostRedisplay();
}

// Modify your display function to include rotation
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // First render the static starfield
    glLoadIdentity();
    gluLookAt(panX, panY, cameraZ / globalZoom,
        panX, panY, -100.0,
        0.0, 1.0, 0.0);

    // Render background starfield if it exists
    if (!starField) {
        starField = new StarField(10000);
    }
    starField->render();

    // Now render the rotating galaxy
    glLoadIdentity();
    gluLookAt(panX, panY, cameraZ / globalZoom,
        panX, panY, -100.0,
        0.0, 1.0, 0.0);

    // Apply rotation only for the galaxy
    glRotatef(globalRotationAngle, 0.0f, 0.0f, 1.0f);

    // Enable rendering settings
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render only the current pattern
    if (currentPattern == FRACTAL_SPIRAL) {
        // Render fractal galaxy pattern
        fractalGalaxy->renderGalaxyArms();

        glBegin(GL_POINTS);
        for (size_t i = 0; i < galaxy.vertices.size(); i += 3) {
            float depth = std::abs(galaxy.vertices[i + 2] - cameraZ);
            float pointSize = 3.0f * (1.0f / (depth * globalZoom + 0.1f));
            glPointSize(std::min(10.0f, pointSize));

            float depthFade = std::min(1.0f, 1.0f / (depth * globalZoom + 0.1f));
            int colorIdx = (i / 3) * 4;
            glColor4f(
                galaxy.colors[colorIdx] * depthFade,
                galaxy.colors[colorIdx + 1] * depthFade,
                galaxy.colors[colorIdx + 2] * depthFade,
                galaxy.colors[colorIdx + 3] * depthFade
            );

            glVertex3f(galaxy.vertices[i],
                galaxy.vertices[i + 1],
                galaxy.vertices[i + 2]);
        }
        glEnd();
    }
    else if (currentPattern == ENHANCED_REALISTIC && enhancedGalaxy) {
        // Render enhanced realistic galaxy pattern
        enhancedGalaxy->render();
        enhancedGalaxy->renderGalaxyCenter();
        enhancedGalaxy->renderGalaxyArms();
    }

    // Switch to orthographic projection for text rendering
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

//void animate() {
//    rotationAngle += 0.001f;
//    cameraZ -= cameraSpeed * timeScale;rr
//    if (cameraZ < 0.1f) {
//        cameraZ = 5.0f;
//        timeScale *= 0.95f;
//    }
//    glutPostRedisplay();
//}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Enhanced Fractal Galaxy Simulation");

    glEnable(GL_DEPTH_TEST); 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Initialize galaxy
    enhancedGalaxy = new EnhancedGalaxy();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseWheelFunc(mouseWheel);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(animate);
    glutFullScreen();
    glutMainLoop();
    return 0;
}