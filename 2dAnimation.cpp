#include <glut.h>
//bullets
#include <vector>
#include <cmath> // For sin function
//asteroids
#include <cstdlib>
#include <ctime>
#include <algorithm>
//collision
#include <iostream>
//score
#include <string>
#include <sstream>


struct Bullet {
    float x, y;
    float angle;
    float speed;
    bool active;
};

struct Asteroid {
    float x, y;
    float speed;
    bool active;
};

struct Star {
    float x, y;
    float size;
    float twinkle; // Brightness
};

std::vector<Bullet> bullets;
std::vector<Asteroid> asteroids;
std::vector<Star> stars;

float spaceshipAngle = 0.0f;
float spaceshipX = 0.0f, spaceshipY = -8.0f; // Start at the bottom center
bool gameOver = false; // Flag to indicate game over
int score = 0;


void initializeStars(int numStars) {
    stars.resize(numStars);
    for (auto& star : stars) {
        star.x = (rand() / (float)RAND_MAX) * 20.0f - 10.0f; // Random X between -10 and 10
        star.y = (rand() / (float)RAND_MAX) * 20.0f - 10.0f; // Random Y between -10 and 10
        star.size = (rand() / (float)RAND_MAX) * 0.5f + 0.1f; // Random size between 0.1 and 0.6
        star.twinkle = 0.0f; // initial brightness
    }
}

void drawSpaceship() {
    glColor3f(0.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLES); // Draw the spaceship body as a triangle
    glVertex2f(0.0f, 1.0f);    // Top vertex
    glVertex2f(-0.5f, -0.5f);  // Bottom-left vertex
    glVertex2f(0.5f, -0.5f);   // Bottom-right vertex
    glEnd();

    glBegin(GL_QUADS); // Draw the spaceship wings as rectangles
    // Left wing
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.8f, -0.2f);
    glVertex2f(-0.5f, 0.0f);
    glVertex2f(-0.2f, -0.2f);

    // Right wing
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.8f, -0.2f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(0.2f, -0.2f);
    glEnd();
}

void drawBullet(const Bullet& bullet) {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(bullet.x, bullet.y, 0);
    glBegin(GL_QUADS); // bullet as a small rectangle
    glVertex2f(-0.1f, -0.1f);
    glVertex2f(0.1f, -0.1f);
    glVertex2f(0.1f, 0.1f);
    glVertex2f(-0.1f, 0.1f);
    glEnd();
    glPopMatrix();
}

void fireBullet() {

    if (!gameOver) {
        Bullet newBullet;
        newBullet.x = spaceshipX;
        newBullet.y = spaceshipY;
        newBullet.angle = spaceshipAngle;
        newBullet.speed = 0.2f; // speed as needed
        newBullet.active = true;
        bullets.push_back(newBullet);
    }
}

bool checkCollision(float x1, float y1, float x2, float y2, float size) {
    //for overlap between two axis-aligned shapes
    return (abs(x1 - x2) < size) && (abs(y1 - y2) < size);
}

void updateBullets() {
    for (auto& bullet : bullets) {
        if (bullet.active) {
            // bullet position update based on speed and angle
           /* bullet.x += bullet.speed * cos(bullet.angle * 3.14159265 / 180.0f);
            bullet.y += bullet.speed * sin(bullet.angle * 3.14159265 / 180.0f);*/
            bullet.y += bullet.speed;
            // Deactivate bullets that go off screen
            if (bullet.y < -10.0f) {
                bullet.active = false;
            }
        }
    }



    // for collisions between bullets and asteroids
    for (auto& bullet : bullets) {
        if (bullet.active) {
            for (auto& asteroid : asteroids) {
                if (asteroid.active) {
                    if (checkCollision(bullet.x, bullet.y, asteroid.x, asteroid.y, 0.2f)) {
                        ++score;
                        bullet.active = false; // deactivate bullet
                        asteroid.active = false; // deactivate asteroid
                        break; // no need to check other asteroids
                    }
                }
            }
        }

        // remove inactive bullets
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.active; }),
            bullets.end());
    }
}

void updateAsteroids() {
    for (auto& asteroid : asteroids) {
        if (asteroid.active) {
            // asteroid position updates based on speed (only in the y-direction)
            asteroid.y -= asteroid.speed;

            // deactivate asteroids that go off screen
            if (asteroid.y < -10.0f) { // the bottom edge of the screen is y = -10.0
                asteroid.active = false;
            }
        }
    }

    // for collisions between spaceship and asteroids
    for (const auto& asteroid : asteroids) {
        if (asteroid.active) {
            if (checkCollision(spaceshipX, spaceshipY, asteroid.x, asteroid.y, 0.5f)) {
                gameOver = true; // game over flag
                break; // no need to check other asteroids
            }
        }
    }

    // remove inactive asteroids
    asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(),
        [](const Asteroid& a) { return !a.active; }),
        asteroids.end());
}

void drawAsteroid(const Asteroid& asteroid) {
    glPushMatrix();
    glColor3f(.8f, .8f, .8f);
    glTranslatef(asteroid.x, asteroid.y, 0);
    glBegin(GL_QUADS); // Draw asteroid as a small rectangle
    glVertex2f(-0.2f, -0.2f);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.2f, 0.2f);
    glVertex2f(-0.2f, 0.2f);
    glEnd();
    glPopMatrix();
}

void spawnAsteroids(int count) {
    for (int i = 0; i < count; ++i) {
        Asteroid newAsteroid;
        newAsteroid.x = (rand() / (float)RAND_MAX) * 20.0f - 10.0f; // Random X between -10 and 10
        newAsteroid.y = 10.0f; // Start above the top of the screen
        newAsteroid.speed = 0.1f; // Speed of the asteroid
        newAsteroid.active = true;
        asteroids.push_back(newAsteroid);
    }
}

void spawnAsteroidsCallback(int value) {
    if (!gameOver) {
        spawnAsteroids(1); // spawn 1 asteroids each time
        glutTimerFunc(100, spawnAsteroidsCallback, 0); // call this function every 100 milliseconds
    }
}

void updateStars(float time) {
    for (auto& star : stars) {
        star.twinkle = (sin(time * 5.0f) * 0.5f + 0.5f); // twinkle effect using sine wave
    }
}

void drawStars() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (const auto& star : stars) {
        glColor3f(star.twinkle, star.twinkle, star.twinkle); // color based on twinkle
        glVertex2f(star.x, star.y); // draw star at position
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Draw stars
    drawStars();

    // Draw spaceship at the current position
    glPushMatrix();
    glTranslatef(spaceshipX, spaceshipY, 0);
    glRotatef(spaceshipAngle, 0, 0, 1);
    drawSpaceship();
    glPopMatrix();

    // Draw bullets 
    for (const auto& bullet : bullets) {
        drawBullet(bullet);
    }

    // Draw asteroids 
    for (const auto& asteroid : asteroids) {
        drawAsteroid(asteroid);
    }

    // Display score
    //glColor3f(1.0f, 1.0f, 1.0f); // White color
    if (!gameOver) {
        std::ostringstream oss;
        oss << score;
        std::string scoreStr = oss.str();
        //renderBitmapString(9.0f, 9.0f, GLUT_BITMAP_HELVETICA_18, scoreStr);
        glRasterPos2f(9.0f, 9.0f); // Set the position for the text
        for (char c : scoreStr) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Render each character
        }
    }

    if (gameOver) {
        //glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 0.0f, 0.0f); // Red color
        glRasterPos2f(-1.5f, 0.0f);
        std::string message = "Game Over";
        for (char c : message) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        //showing score after gameover
      /*  glRasterPos2f(-1.5f, 1.0f);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score);*/
        // Display score after game over
        //glColor3f(1.0f, 1.0f, 1.0f); // White color
        std::ostringstream oss;
        oss << score;
        std::string scoreStr = oss.str();
        std::string score_ = "Score: ";
        glRasterPos2f(-1.1f, 1.0f);
        for (char c : score_) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
        glRasterPos2f(.45f, 1.0f); // Set the position for the text
        for (char c : scoreStr) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Render each character
        }



    }


    glColor3f(1.0f, 1.0f, 1.0f);
    std::string name = "Sanskar Rathore, 500108014";
    glRasterPos2f(-9.5f, -9.5f);
    for (char c : name) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    glutSwapBuffers();
}

void update() {

    if (!gameOver) {
        static float time = 0.0f; // static variable to keep track of time

        updateBullets();
        updateAsteroids();
        glutPostRedisplay();

        updateStars(time);
        time += 0.1f; // Increment time to create twinkle effect

    }

    //updateBullets();

    // Update spaceship position and angle

    // Update asteroid positions
   // updateAsteroids();

    // Check collisions

   // glutPostRedisplay();
}

// function to handle mouse motion
void mouseMotion(int x, int y) {

    if (!gameOver) {
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

        // convert mouse coordinates to world coordinates
        spaceshipX = (x / (float)windowWidth) * 20.0f - 10.0f;

        //the y values range from 0 at the top to windowHeight at the bottom.
        spaceshipY = ((windowHeight - y) / (float)windowHeight) * 20.0f - 10.0f;
        //spaceshipY = -8.0f;
    }
    glutPostRedisplay();
}

// function to handle mouse clicks
void mouseClick(int button, int state, int x, int y) {
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !gameOver) {
        fireBullet();
        spaceshipX = (x / (float)windowWidth) * 20.0f - 10.0f;
        spaceshipY = ((windowHeight - y) / (float)windowHeight) * 20.0f - 10.0f;
    }


}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10); // adjust based on your game world
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Asteroids Game(Assignment 2)");

    initializeStars(200); // Initialize 100 stars

    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouseMotion); // Register the mouse motion callback
    glutMouseFunc(mouseClick); // Register the mouse click callback

    //// Periodically spawn asteroids
    //glutTimerFunc(2000, [](int value) {
    //     spawnAsteroids(1000);
    //     glutTimerFunc(100, [](int value) { spawnAsteroids(); }, 0);
    //     }, 0);s
    // 
     // start the timer for asteroid spawning
    glutTimerFunc(2000, spawnAsteroidsCallback, 0);

    glutMainLoop();
    return 0;
}



//https://chatgpt.com/share/31d5d26d-6a3f-4c4d-86db-c3212ffd51f4
