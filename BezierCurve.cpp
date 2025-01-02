#include <glut.h>
#include <cmath>
#include <vector>
#include <string>
using namespace std;
#define M_PI 3.14

// Add new structure for energy particles
struct EnergyParticle {
    float x, y;          // Current position
    float speed;         // Movement speed
    float alpha;         // Transparency
    float length;        // Line length
    float angle;         // Line angle
    int targetChakra;    // Current target chakra

    // Control points for Bézier curve
    float cp1x, cp1y;    // First control point
    float cp2x, cp2y;    // Second control point
    float endx, endy;    // End point
    float t;             // Parameter for Bézier curve (0 to 1)
};

std::vector<EnergyParticle> energyParticles;
float energyFlowSpeed = 0.01f;
float particleLifetime = 0.0f;
bool showControlPoints = false;

struct ChakraProperties {
    float r, g, b;      // RGB color
    float scale;        // Base scale
    float rotSpeed;     // Rotation speed
    const char* name;   // Chakra name
};
// Helper struct to define pattern boundaries
struct PatternBoundary {
    float innerRadius;  // Inner radius for the symbol
    float outerRadius;  // Outer radius for the lotus petals
    float symbolScale; // Scale factor for the Sanskrit symbol
};

float currentRotation = 0.0f;
float baseScale = 0.3f;
float breatheScale = 0.0f;
float breatheSpeed = 0.02f;

ChakraProperties chakras[] = {
    {0.8f, 0.0f, 0.8f, 0.5f, 0.4f, "Crown"},        // Crown - smaller
    {0.0f, 0.0f, 0.8f, 0.5f, 0.5f, "Third Eye"},    // Third Eye - smaller
    {0.0f, 0.8f, 0.8f, 0.55f, 0.6f, "Throat"},      // Throat - slightly larger
    {0.0f, 0.8f, 0.0f, 0.6f, 0.7f, "Heart"},        // Heart - larger
    {1.0f, 1.0f, 0.0f, 0.58f, 0.8f, "Solar Plexus"},// Solar Plexus - medium
    {1.0f, 0.5f, 0.0f, 0.55f, 0.9f, "Sacral"},      // Sacral - slightly smaller
    {0.8f, 0.0f, 0.0f, 0.5f, 1.0f, "Root"},         // Root - smaller
};

void drawText(const std::string& text, float x, float y, float z) {
    glRasterPos3f(x, y, z - 1.0f + 0.1f);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); // Changed font to Helvetica
    }
}


const float chakraPositions[] = {
    0.85f,   // Crown - adjusted higher
    0.65f,   // Third Eye
    0.45f,   // Throat
    0.25f,   // Heart
    0.05f,   // Solar Plexus
    -0.15f,  // Sacral
    -0.35f   // Root - adjusted lower
};

const char* chakraInfo[] = {
    "Crown (Sahasrara): Spiritual connection, enlightenment, divine wisdom",
    "Third Eye (Ajna): Intuition, perception, psychic abilities",
    "Throat (Vishuddha): Communication, self-expression, truth",
    "Heart (Anahata): Love, compassion, emotional balance",
    "Solar Plexus (Manipura): Personal power, confidence, will",
    "Sacral (Svadhishthana): Creativity, emotions, relationships",
    "Root (Muladhara): Stability, grounding, physical security"
};

// Add this function after drawText:
void drawChakraInfo() {
    // Draw main heading
    glColor3f(0.0f, 0.0f, 0.0f);  // Black color for text
    glPushMatrix();
    glTranslatef(-0.5f,- 0.95f, 0.0f);
    drawText("Traditional Chakra System Visualization", 0, 0, 0);
    glPopMatrix();

    // Draw chakra information
    for (int i = 0; i < 7; i++) {
        float y = chakraPositions[i];
        glColor3f(chakras[i].r, chakras[i].g, chakras[i].b);

        // Draw line connecting text to chakra
        glBegin(GL_LINES);
        glVertex2f(0.0f, y);
        glVertex2f(0.6f, y);
        glEnd();

        // Draw chakra information text
        glPushMatrix();
        glTranslatef(0.65f, y - 0.02f, 0.0f);
        drawText(chakraInfo[i], 0, 0, 0);
        glPopMatrix();
    }

    // Draw author name in bottom-left corner
    glColor3f(0.0f, 0.0f, 0.0f);  // Black color for signature
    glPushMatrix();
    glTranslatef(0.990f, -0.95f, 0.0f);
    drawText("Sanskar Rathore", 0, 0, 0);
    //drawText("500108014", 0, 0, 0);
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 0.0f);  // Black color for signature
    glPushMatrix();
    glTranslatef(0.990f, -0.99f, 0.0f);
    //drawText("Sanskar Rathore", 0, 0, 0);
    drawText("500108014", 0, 0, 0);
    glPopMatrix();
}

// Get the safe drawing area for each chakra pattern
PatternBoundary getPatternBoundary(int chakraIndex) {
    PatternBoundary boundary;

    // Base measurements
    boundary.outerRadius = 0.4f; // Current lotus petal radius
    boundary.innerRadius = boundary.outerRadius * 0.6f; // Safe area for symbol
    boundary.symbolScale = 0.2f; // Default symbol scale

    // Adjust boundaries based on chakra type
    switch (chakraIndex) {
    case 0: // Crown - Most complex with double lotus
        boundary.outerRadius = 0.5f;     // Outer lotus
        boundary.innerRadius = 0.25f;    // Smaller inner area due to complexity
        boundary.symbolScale = 0.25f;    // Smaller ॐ symbol
        break;

    case 1: // Third Eye - Two petals
        boundary.outerRadius = 0.4f;
        boundary.innerRadius = 0.3f;     // Larger inner area for ऐं
        boundary.symbolScale = 0.35f;
        break;

    case 2: // Throat - 16 petals
        boundary.outerRadius = 0.4f;
        boundary.innerRadius = 0.3f;    // Balanced area for हं
        boundary.symbolScale = 0.35f;
        break;

    case 3: // Heart - 12 petals
        boundary.outerRadius = 0.4f;
        boundary.innerRadius = 0.3f;     // Good space for यं
        boundary.symbolScale = 0.35f;
        break;

    case 4: // Solar Plexus - 10 petals
        boundary.outerRadius = 0.4f;
        boundary.innerRadius = 0.3f;     // Balanced area for रं
        boundary.symbolScale = 0.35f;
        break;

    case 5: // Sacral - 6 petals
        boundary.outerRadius = 0.4f;
        boundary.innerRadius = 0.32f;    // Larger area for वं
        boundary.symbolScale = 0.38f;
        break;

    case 6: // Root - 4 petals
        boundary.outerRadius = 0.4f;
        boundary.innerRadius = 0.35f;    // Largest inner area for लं
        boundary.symbolScale = 0.4f;
        break;
    }

    return boundary;
}

void drawControlPoint(float x, float y) {
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();

    // Draw small circle around point
    glBegin(GL_LINE_LOOP);
    for (float angle = 0; angle < 2 * M_PI; angle += 0.2f) {
        float px = x + 0.01f * cos(angle);
        float py = y + 0.01f * sin(angle);
        glVertex2f(px, py);
    }
    glEnd();
}

// Add new function to draw control lines
void drawControlLines(float x0, float y0, float x1, float y1,
    float x2, float y2, float x3, float y3) {
    glBegin(GL_LINES);
    // Line from start point to first control point
    glVertex2f(x0, y0);
    glVertex2f(x1, y1);

    // Line between control points
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);

    // Line from second control point to end point
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}



// Function to draw lotus petals (outer circle)
void drawBezierCurve(float x0, float y0, 
    float x1, float y1, float x2, float y2, 
    float x3, float y3) {
    glBegin(GL_LINE_STRIP);

    for (float t = 0; t <= 1.0f; t += 0.05f) {
        // Cubic Bézier formula
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        float x = uuu * x0 +
            3 * uu * t * x1 +
            3 * u * tt * x2 +
            ttt * x3;

        float y = uuu * y0 +
            3 * uu * t * y1 +
            3 * u * tt * y2 +
            ttt * y3;

        glVertex2f(x, y);
    }
    glEnd();

    if (showControlPoints) {
        // Save current color
        float currentColor[4];
        glGetFloatv(GL_CURRENT_COLOR, currentColor);

        // Draw control points in red
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

        // Draw control points
        drawControlPoint(x0, y0);  // Start point
        drawControlPoint(x1, y1);  // Control point 1
        drawControlPoint(x2, y2);  // Control point 2
        drawControlPoint(x3, y3);  // End point

        // Draw control lines
        glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
        drawControlLines(x0, y0, x1, y1, x2, y2, x3, y3);

        // Restore original color
        glColor4f(currentColor[0], currentColor[1], currentColor[2], currentColor[3]);
    }
}

void drawLotusPetals(int numPetals, float radius) {
    for (int i = 0; i < numPetals; i++) {
        float angle = (2 * M_PI * i) / numPetals;
        float nextAngle = (2 * M_PI * (i + 1)) / numPetals;

        // Calculate control points for petal curve
        float midAngle = (angle + nextAngle) / 2.0f;

        // Start point
        float x0 = radius * cos(angle);
        float y0 = radius * sin(angle);

        // End point
        float x3 = radius * cos(nextAngle);
        float y3 = radius * sin(nextAngle);

        // Control points (create bulge outward)
        float bulgeRadius = radius * 1.3f;  // Adjust this for different petal shapes
        float ctrl1Angle = angle + (midAngle - angle) * 0.5f;
        float ctrl2Angle = nextAngle - (nextAngle - midAngle) * 0.5f;

        float x1 = bulgeRadius * cos(ctrl1Angle);
        float y1 = bulgeRadius * sin(ctrl1Angle);
        float x2 = bulgeRadius * cos(ctrl2Angle);
        float y2 = bulgeRadius * sin(ctrl2Angle);

        drawBezierCurve(x0, y0, x1, y1, x2, y2, x3, y3);
    }
}

void drawOmSymbol() {
    // upper loop of ॐ
    drawBezierCurve(-0.4f, 0.1f, -0.4f, 0.4f, 0.4f, 0.4f, -.125f, 0.0f);
    // below
    drawBezierCurve(-.125f, 0.0f, 0.5f, -0.1f, 0.3f, -0.4f, -0.4f, -0.3f);
    // tail
    drawBezierCurve(-.125f, .0f, 0.6f, 0.6f, 0.6f, -0.6f, 0.6f, -0.3f);

    // 
    drawBezierCurve(0.1f, 0.4f, 0.2f, 0.2f, 0.2f, 0.2f, 0.6f, 0.4f);

    drawBezierCurve(0.25f, 0.4f, 0.26f, 0.4f, 0.26f, 0.4f, 0.27f, 0.4f);
}

void drawAimSymbol() {
    // Crescent part of "ऐं"
    drawBezierCurve(-0.25f, 0.0f, -0.4f, 0.3f, 0.4f, .3f, -.125f, -0.1f);
    drawBezierCurve(-.105f, -0.050f, 0.2f, -0.1f, 0.0f, -0.5f, -0.25f, -0.1f);

    drawBezierCurve(-0.15f, 0.3f, 0.0f, 0.2f, 0.0f, .2f, 0.2f, .3f);

    drawBezierCurve(-0.0f, 0.35f, 0.0f, 0.3f, 0.0f, .3f, 0.02f, .35f);

  
    // The dot above
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.4f);
    glEnd();
}

void drawHamSymbol() {
    // Vertical stroke
    drawBezierCurve(-0.1f, -0.2f, -0.1f, 0.0f, -0.1f, 0.2f, 0.0f, 0.3f);
    // Loop on the right
    drawBezierCurve(0.0f, 0.3f, 0.2f, 0.2f, 0.2f, 0.0f, 0.0f, -0.2f);
}

void drawYamSymbol() {
    // Central loop
    drawBezierCurve(-0.15f, 0.2f, -0.1f, 0.2f, 0.2f, 0.2f, 0.1f, 0.2f);
    // Vertical stroke
    drawBezierCurve(0.05f, 0.2f, 0.05f, 0.1f, 0.05f, 0.3f, 0.05f, -0.3f);

    drawBezierCurve(-0.19f, 0.11f, 0.029f, 0.35f, -0.35f, -0.24f, 0.1f, -0.02f);
}

void drawRamSymbol() {
    // Loop of "रं"
    drawBezierCurve(-0.15f, 0.2f, -0.1f, 0.2f, 0.2f, 0.2f, 0.1f, 0.2f);
    // Vertical stroke
    //drawBezierCurve(0.05f, 0.2f, 0.05f, 0.1f, 0.05f, 0.3f, 0.05f, -0.3f);

    drawBezierCurve(-0.18f, 0.15f, 0.0f, 0.27f, -0.25f, -0.12f, -0.18f, 0.15f);

    drawBezierCurve(-0.18f, 0.15f, 0.0f, 0.27f, -0.25f, -0.12f, 0.06f, -0.19f);
}

void drawVamSymbol() {
    // Main loop
    drawBezierCurve(-0.1f, 0.2f, -0.3f, 0.2f, 0.3f, 0.2f, 0.1f, 0.2f);
    // The tail
    drawBezierCurve(0.05f, 0.2f, 0.05f, 0.0f, 0.05f, 0.0f, 0.05f, -0.2f);

    drawBezierCurve(-0.18f, 0.0f, -0.13f, 0.05f, -0.12f, 0.17f, 0.05f, -0.0f);

    drawBezierCurve(-0.18f, 0.0f, -0.13f, -0.05f, -0.1f, -0.17f, 0.05f, 0.0f);
}

void drawLamSymbol() {
    // Central loop
    drawBezierCurve(-0.2f, 0.0f, -0.3f, 0.3f, 0.3f, 0.3f, 0.2f, 0.0f);
    // Bottom tail
    drawBezierCurve(0.2f, 0.0f, 0.3f, -0.2f, 0.1f, -0.3f, 0.0f, -0.2f);


}

void drawHumanFigure() {
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f); // Transparent human figure

    // Drawing torso (Bezier curve)
    drawBezierCurve(0.0f, 0.0f, 0.3f, 1.0f, -0.3f, 1.0f, 0.0f, 2.0f); 

    // Draw left arm
    drawBezierCurve(-0.5f, 1.0f, -0.8f, 0.7f, -0.8f, 0.3f, -0.5f, 0.0f);

    // Draw right arm
    drawBezierCurve(0.5f, 1.0f, 0.8f, 0.7f, 0.8f, 0.3f, 0.5f, 0.0f);

    // Draw left leg
    drawBezierCurve(-0.5f, -1.0f, -0.7f, -1.5f, -0.6f, -2.0f, -0.3f, -2.0f);

    // Draw right leg
    drawBezierCurve(0.5f, -1.0f, 0.7f, -1.5f, 0.6f, -2.0f, 0.3f, -2.0f);
}

// Function to draw specific chakra symbols
void drawChakraSymbol(int chakraIndex) {
    glBegin(GL_LINE_STRIP);
    switch (chakraIndex) {
    case 6: // Root Chakra - Square with 4 petals
       
        drawLotusPetals(4, 0.4f);
        drawLamSymbol(); // "लं"
        break;

    case 5: // Sacral Chakra - Crescent
     
        drawLotusPetals(6, 0.4f);
        drawVamSymbol(); // "वं"
        break;

    case 4: // Solar Plexus - Triangle
     
        drawLotusPetals(10, 0.4f);
        drawRamSymbol(); // "रं"
        break;

    case 3: // Heart Chakra - Star of David
        drawLotusPetals(12, 0.4f);
        drawYamSymbol(); // "यं"
        break;

    case 2: // Throat Chakra - Circle
       
        drawHamSymbol(); // "हं"
        drawLotusPetals(16, 0.4f);
        break;

    case 1: // Third Eye - Om symbol simplified
        
        drawAimSymbol(); // "ऐं"
        drawLotusPetals(18, 0.4f); // Two-petal lotus
        break;

    case 0: // Crown Chakra - Complex Om with multiple petals
        // Multiple layers of lotus petals
        drawOmSymbol(); // "ॐ"
        drawLotusPetals(16, 0.4f);
        drawLotusPetals(32, 0.5f);
        break;
    }
}

// Helper function to check if a point is within the safe drawing area
bool isWithinSafeArea(float x, float y, const PatternBoundary& boundary) {
    float distance = sqrt(x * x + y * y);
    return distance <= boundary.innerRadius;
}

// Modified symbol drawing functions with boundary checks
void drawSymbolInBoundary(float x, float y, float scale, 
                         void (*symbolFunc)(), const PatternBoundary& boundary) {
    if (isWithinSafeArea(x, y, boundary)) {
        glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(scale * boundary.symbolScale, scale * boundary.symbolScale, 1.0f);
        symbolFunc();
        glPopMatrix();
    }
}


void drawSingleChakra(float centerX, float centerY, float scale, float rotation, const ChakraProperties& props, int index) {
    PatternBoundary boundary = getPatternBoundary(index);

    glPushMatrix();
    glTranslatef(centerX, centerY, 0.0f);

    float currentScale = scale * props.scale * (1.0f + 0.1f * sin(breatheScale));
    glScalef(currentScale, currentScale, 1.0f);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw semi-transparent background color as a filled circle
    glColor4f(props.r, props.g, props.b, 0.3f);
    glBegin(GL_POLYGON);
    for (float angle = 0.0f; angle < 2 * M_PI; angle += 0.1f) {
        float x = boundary.innerRadius * cos(angle) * 1.0;
        float y = boundary.innerRadius * sin(angle) * 1.0;
        glVertex2f(x, y);
    }
    glEnd();

    // Draw rotating lotus petals
    glPushMatrix();
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);  // Apply rotation only for the petals
    glColor3f(props.r, props.g, props.b);

    switch (index) {
    case 0: // Crown
        drawLotusPetals(16, 0.4f);
        drawLotusPetals(32, 0.5f);
        break;
    case 1: // Third Eye
        drawLotusPetals(18, 0.4f);
        break;
    case 2: // Throat
        drawLotusPetals(16, 0.4f);
        break;
    case 3: // Heart
        drawLotusPetals(12, 0.4f);
        break;
    case 4: // Solar Plexus
        drawLotusPetals(10, 0.4f);
        break;
    case 5: // Sacral
        drawLotusPetals(6, 0.4f);
        break;
    case 6: // Root
        drawLotusPetals(4, 0.4f);
        break;
    }
    glPopMatrix();

    // Draw non-rotating Sanskrit symbol
    glColor3f(props.r, props.g, props.b);
    glPushMatrix();
    glScalef(boundary.symbolScale * 3.0, boundary.symbolScale * 3.0, 1.0f);

    // Draw symbols without rotation
    switch (index) {
    case 0: drawOmSymbol(); break;
    case 1: drawAimSymbol(); break;
    case 2: drawHamSymbol(); break;
    case 3: drawYamSymbol(); break;
    case 4: drawRamSymbol(); break;
    case 5: drawVamSymbol(); break;
    case 6: drawLamSymbol(); break;
    }
    glPopMatrix();

    glDisable(GL_BLEND);
    glPopMatrix();
}

void drawMeditationFigure() {
    // Set color to transparent black
    glColor4f(0.0f, 0.0f, 0.0f, 0.2f);  // Last parameter is alpha for transparency

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Head
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 2 * M_PI; t += 0.1f) {
        float x = 0.15f * cos(t);
        float y = 0.15f * sin(t) + 0.85f;
        glVertex2f(x, y);
    }
    glEnd();

    // Torso (using Bezier curve)
    drawBezierCurve(0.0f, 0.7f,   // Start at neck
        0.0f, 0.3f,    // Control point 1
        0.0f, 0.1f,    // Control point 2
        0.0f, -0.1f);  // End at base

    // Left arm
    drawBezierCurve(-0.0f, 0.4f,      // Start at shoulder
        -0.3f, 0.3f,      // Control point 1
        -0.4f, 0.0f,      // Control point 2
        -0.3f, -0.15f);   // End at hand

    // Right arm
    drawBezierCurve(0.0f, 0.4f,       // Start at shoulder
        0.3f, 0.3f,       // Control point 1
        0.4f, 0.0f,       // Control point 2
        0.3f, -0.15f);    // End at hand

    // Left leg (crossed)
    drawBezierCurve(-0.0f, -0.1f,     // Start at hip
        -0.3f, -0.2f,     // Control point 1
        -0.4f, -0.25f,    // Control point 2
        -0.5f, -0.3f);    // End at foot

    // Right leg (crossed)
    drawBezierCurve(0.0f, -0.1f,      // Start at hip
        0.3f, -0.2f,      // Control point 1
        0.4f, -0.25f,     // Control point 2
        0.5f, -0.3f);     // End at foot

    // Add crossed legs details
    drawBezierCurve(-0.5f, -0.3f,     // Left foot to right
        -0.3f, -0.35f,
        0.0f, -0.4f,
        0.2f, -0.35f);

    drawBezierCurve(0.5f, -0.3f,      // Right foot to left
        0.3f, -0.25f,
        0.0f, -0.2f,
        -0.2f, -0.25f);

    glDisable(GL_BLEND);
}

void drawMeditationFigure2() {
    // Set color to transparent black
    glColor4f(0.0f, 0.0f, 0.0f, 0.15f);  // Reduced alpha for subtler appearance

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw filled silhouette
    glBegin(GL_TRIANGLE_FAN);

    // Head and neck
    for (float t = 0; t <= 2 * M_PI; t += 0.1f) {
        float x = 0.15f * cos(t);
        float y = 0.15f * sin(t) + 0.85f;
        glVertex2f(x, y);
    }
    glEnd();

    // Neck and shoulders
    glBegin(GL_POLYGON);
    glVertex2f(-0.2f, 0.65f);  // Left shoulder
    glVertex2f(0.2f, 0.65f);   // Right shoulder
    glVertex2f(0.1f, 0.75f);   // Right neck
    glVertex2f(-0.1f, 0.75f);  // Left neck
    glEnd();

    // Torso (more natural curve)
    glBegin(GL_POLYGON);
    glVertex2f(-0.2f, 0.65f);  // Left shoulder
    glVertex2f(0.2f, 0.65f);   // Right shoulder
    glVertex2f(0.25f, 0.4f);   // Right upper torso
    glVertex2f(0.2f, 0.1f);    // Right waist
    glVertex2f(-0.2f, 0.1f);   // Left waist
    glVertex2f(-0.25f, 0.4f);  // Left upper torso
    glEnd();

    // Left arm (more natural curves)
    glBegin(GL_POLYGON);
    for (float t = 0; t <= 1.0f; t += 0.1f) {
        // Upper arm curve
        float x1 = -0.2f + (-0.1f * t);
        float y1 = 0.65f + (-0.3f * t);
        glVertex2f(x1, y1);

        // Lower arm curve
        float x2 = -0.3f + (-0.1f * t);
        float y2 = 0.35f + (-0.4f * t);
        glVertex2f(x2, y2);
    }
    glEnd();

    // Right arm (more natural curves)
    glBegin(GL_POLYGON);
    for (float t = 0; t <= 1.0f; t += 0.1f) {
        // Upper arm curve
        float x1 = 0.2f + (0.1f * t);
        float y1 = 0.65f + (-0.3f * t);
        glVertex2f(x1, y1);

        // Lower arm curve
        float x2 = 0.3f + (0.1f * t);
        float y2 = 0.35f + (-0.4f * t);
        glVertex2f(x2, y2);
    }
    glEnd();

    // Crossed legs (more natural curves)
    glBegin(GL_POLYGON);
    // Left leg
    glVertex2f(-0.2f, 0.1f);   // Hip
    glVertex2f(-0.3f, -0.1f);  // Upper thigh
    glVertex2f(-0.4f, -0.2f);  // Knee
    glVertex2f(-0.5f, -0.3f);  // Lower leg
    // Right leg crossing over
    glVertex2f(0.2f, 0.1f);    // Hip
    glVertex2f(0.3f, -0.1f);   // Upper thigh
    glVertex2f(0.4f, -0.2f);   // Knee
    glVertex2f(0.5f, -0.3f);   // Lower leg
    glEnd();

    // Add crossed legs details with smooth curves
    glBegin(GL_POLYGON);
    glVertex2f(-0.5f, -0.3f);
    glVertex2f(-0.3f, -0.35f);
    glVertex2f(0.0f, -0.4f);
    glVertex2f(0.3f, -0.35f);
    glVertex2f(0.5f, -0.3f);
    glVertex2f(0.3f, -0.25f);
    glVertex2f(0.0f, -0.2f);
    glVertex2f(-0.3f, -0.25f);
    glEnd();

    glDisable(GL_BLEND);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'c':  // Toggle control points with 'c' key
    case 'C':
        showControlPoints = !showControlPoints;
        glutPostRedisplay();
        break;
    case 27:   // ESC key to exit
        exit(0);
        break;
    }
}

void calculateBezierPoint(float t, float p0x, float p0y, float p1x, float p1y,
    float p2x, float p2y, float p3x, float p3y,
    float& outX, float& outY) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    outX = uuu * p0x +
        3 * uu * t * p1x +
        3 * u * tt * p2x +
        ttt * p3x;

    outY = uuu * p0y +
        3 * uu * t * p1y +
        3 * u * tt * p2y +
        ttt * p3y;
}

void drawEnergyFlow() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(2.0f);

    for (auto& particle : energyParticles) {
        // Get color based on current position
        float relativePos = (particle.y + 0.33f) / 1.31f;
        int currentChakra = 6 - (int)(relativePos * 7);
        currentChakra = std::max(0, std::min(6, currentChakra));

        // Draw the energy stream using multiple connected line segments
        glBegin(GL_LINE_STRIP);

        // Create a trail effect using multiple points along the Bézier curve
        for (float t = std::max(0.0f, particle.t - 0.2f); t <= particle.t; t += 0.02f) {
            float x, y;
            calculateBezierPoint(t,
                particle.x, particle.y,
                particle.cp1x, particle.cp1y,
                particle.cp2x, particle.cp2y,
                particle.endx, particle.endy,
                x, y);

            float alpha = (t - (particle.t - 0.2f)) / 0.2f * particle.alpha;
            glColor4f(chakras[currentChakra].r,
                chakras[currentChakra].g,
                chakras[currentChakra].b,
                alpha);
            glVertex2f(x, y);
        }
        glEnd();
    }

    glDisable(GL_BLEND);
}
// Modify the drawChakraColumn function to include the meditation figure
void drawChakraColumn() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(2.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float scaleFactor = 0.8f;  // Adjust this value to change overall size
    glScalef(scaleFactor, scaleFactor, 1.0f);

    // Draw the meditation figure
    glPushMatrix();
    glScalef(1.2f, 1.2f, 1.0f);
    drawMeditationFigure();
    drawMeditationFigure2();
    glPopMatrix();

    // Draw energy flow
    drawEnergyFlow();

    // Draw chakras
    for (int i = 0; i < 7; i++) {
        float rotation = currentRotation * chakras[i].rotSpeed;
        drawSingleChakra(0.0f, chakraPositions[i], baseScale, rotation, chakras[i], i);
    }

    drawChakraInfo();

    glutSwapBuffers();
}

void updateEnergyParticles() {
    particleLifetime += 0.016f;

    for (auto& particle : energyParticles) {
        // Update particle position along Bézier curve
        particle.t += particle.speed;

        if (particle.t >= 1.0f) {
            // Reset particle and set new target
            particle.t = 0.0f;
            particle.x = particle.endx;
            particle.y = particle.endy;

            particle.targetChakra--;
            if (particle.targetChakra < 0) {
                particle.targetChakra = 6;
                particle.y = -0.33f;
            }

            // Calculate new control points for next curve
            float angle = particle.angle * M_PI / 180.0f;
            float nextY = chakraPositions[particle.targetChakra];

            // Set new Bézier curve control points
            particle.cp1x = particle.x + 0.2f * cos(angle);
            particle.cp1y = particle.y + (nextY - particle.y) * 0.33f;
            particle.cp2x = particle.x - 0.2f * cos(angle);
            particle.cp2y = particle.y + (nextY - particle.y) * 0.66f;
            particle.endx = particle.x;
            particle.endy = nextY;
        }

        // Update current position
        calculateBezierPoint(particle.t,
            particle.x, particle.y,
            particle.cp1x, particle.cp1y,
            particle.cp2x, particle.cp2y,
            particle.endx, particle.endy,
            particle.x, particle.y);

        // Update angle for visual effect
        particle.angle += 2.0f;
        if (particle.angle > 360.0f) particle.angle -= 360.0f;

        // Pulsing alpha effect
        particle.alpha = 0.5f + 0.3f * sin(particleLifetime * 2.0f);
    }
}


void update(int value) {
    currentRotation += 1.0f;
    if (currentRotation > 360.0f) {
        currentRotation -= 360.0f;
    }

    breatheScale += breatheSpeed;

    // Update energy particles with new Bézier curve movement
    updateEnergyParticles();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}
void display() {
    drawChakraColumn();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / (float)h;
    if (w >= h) {
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    }
    else {
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }

    glMatrixMode(GL_MODELVIEW);
}



void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    float screenAspect = (float)screenWidth / (float)screenHeight;

    // Set orthographic projection that maintains aspect ratio
    if (screenWidth >= screenHeight) {
        gluOrtho2D(-1.0 * screenAspect, 1.0 * screenAspect, -1.0, 1.0);
    }
    else {
        gluOrtho2D(-1.0, 1.0, -1.0 / screenAspect, 1.0 / screenAspect);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(2.0f);
}

void initEnergyParticles() {
    const int numParticles = 12;
    float baseSpeed = 0.005f;  // Slower speed for smoother movement

    for (int i = 0; i < numParticles; i++) {
        EnergyParticle particle;
        float angle = (2 * M_PI * i) / numParticles;

        // Starting position (root chakra)
        particle.x = 0.1f * cos(angle);
        particle.y = -0.33f;

        // Initialize Bézier curve control points
        particle.cp1x = particle.x + 0.2f * cos(angle);
        particle.cp1y = particle.y + 0.3f;
        particle.cp2x = particle.x - 0.2f * cos(angle);
        particle.cp2y = particle.y + 0.6f;
        particle.endx = particle.x;
        particle.endy = chakraPositions[5]; // Target next chakra

        particle.speed = baseSpeed;
        particle.alpha = 0.8f;
        particle.length = 0.05f;
        particle.angle = angle * 180.0f / M_PI;
        particle.targetChakra = 6;
        particle.t = 0.0f;

        energyParticles.push_back(particle);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    // Create window at full screen size
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Traditional Chakra Symbols");

    glutFullScreen();

    init();
    initEnergyParticles();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();

    return 0;
}