#include <glut.h>
#include <iostream>

bool isClickedSky = false;
bool isClickedSun = false;
// Initialize the GLUT window
void initGL()  {
    glMatrixMode(GL_PROJECTION);          // Projection Matrix
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);     // view

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


//void kite() {
//    glBegin(GL_POLYGON);
//
//    glColor3f(1, 0.5, 0);
//    glVertex2f(0.4f, .4f);
//    glVertex2f(.5f, .4f);
//    glVertex2f(.55f, .25f);
//    glVertex2f(.4f, .3f);
//        glEnd();
//    glBegin(GL_POLYGON);
//
//    glVertex2f(.65f, .25f);
//    glVertex2f(.55f, .195f);
//    glVertex2f(.55f, .25f);
//     glEnd();
//}

void hut_roof1() {
    glBegin(GL_POLYGON);

    //back_roof
    glColor3f(0.694,	0.593,	0.414);
    glVertex2f(0.0f, -.2f);
    glVertex2f(-.151f, -.2f);
    glVertex2f(-.31f, .15f);
    glVertex2f(-.25f, .21f);

    
    glEnd();
}
void hut_roof2() {
    glBegin(GL_POLYGON);

    //back_roof
    glColor3f(0.594, 0.453, 0.314);
    glVertex2f(-0.6f, -.18f);
    glVertex2f(-.95f, -.18f);
    glVertex2f(-.6f, .2f);
    glVertex2f(-.25f, .21f);


    glEnd();
}

void hut_wall1() {
    glBegin(GL_POLYGON);

    glColor3f(0.718,	0.498,	0.298);
    glVertex2f(-0.3f, .16f);
    glVertex2f(-.55f, -.12f);
    glVertex2f(-.55f, -.3f);
    glVertex2f(-.15f, -.27f);
    glVertex2f(-.15f, -.1f);


    glEnd();
}

//void hut_wall1_shadow() {
    
    //display


    /*glBegin(GL_POLYGON);

    glColor4f(0.1, 0.1, 0.10,.3f);
    glVertex2f(-0.3f, .15f);
    glVertex2f(-.55f, -.13f);
    glVertex2f(-.55f, -.19f);
    glVertex2f(-0.32f, .05f);
    glVertex2f(-.15f, -.19f);
    glVertex2f(-.15f, -.1f);


    glEnd();*/
//}

//void hut_wall2_shadow() {

    //indisplay

    /*glBegin(GL_POLYGON);

    glColor4f(0.1, 0.1, 0.10, .3f);
    glVertex2f(-.55f, -.21f);
    glVertex2f(-.55f, -.13f);
    glVertex2f(-.85f, -.13f);
    glVertex2f(-0.85f, -.21f);


    glEnd();*/
//}

void door() {
    glBegin(GL_POLYGON);
    glColor3f(0.412f, 0.278f, 0.180f);
    glVertex2f(-.35f, -.12f);
    glVertex2f(-.45f, -.13f);
    glVertex2f(-.45f, -.29f);
    glVertex2f(-.35f, -.28f);


    glEnd();
}

//void open_door() {
    /*glBegin(GL_POLYGON);

    glColor3f(0.412f, 0.278f, 0.180f);
    glVertex2f(-.43f, -.15f);
    glVertex2f(-.43f, -.26f);
    glVertex2f(-.35f, -.28f);
    glVertex2f(-.35f, -.12f);


    glEnd();*/
//}

void window() {
    glBegin(GL_POLYGON);

    glColor4f(0.412f, 0.278f, 0.180f,1.0f);
    glVertex2f(-.3f, -.13f);
    glVertex2f(-.3f, -.2f);
    glVertex2f(-.2f, -.19f);
    glVertex2f(-.2f, -.12f);


    glEnd();
}

void window_shadow() {
    glBegin(GL_POLYGON);

    glColor4f(0.412f, 0.278f, 0.180f, 1.0f);
    glVertex2f(-.3f, -.14f);
    glVertex2f(-.3f, -.2f);
    glVertex2f(-.215f, -.19f);
    glVertex2f(-.215f, -.133f);


    glEnd();
}

void hut_wall2() {
    glBegin(GL_POLYGON);

    //back_roof
    glColor3f(0.6518, 0.498, 0.298);
    glVertex2f(-.55f, -.13f);
    glVertex2f(-.55f, -.3f);
    glVertex2f(-.85f, -.25f);
    glVertex2f(-.85f, -.15f);


    glEnd();
}


void hut_floor1() {
    glBegin(GL_POLYGON);

    //back_roof
    glColor3f(0.690,	0.616,	0.373);
    glVertex2f(-.55f, -.3f);
    glVertex2f(-.85f, -.23f);
    glVertex2f(-1.0f, -.28f);
    glVertex2f(-.45f, -.35f);
    glVertex2f(-.05f, -.25f);
    glVertex2f(-.15f, -.24f);
     

    glEnd();
}

void road() {

    glBegin(GL_POLYGON);
    glColor3f(0.19, 0.19, 0.195);


   // glVertex2f(.25f, -.4f);
    //glVertex2f(.15f, -.1f);
    glVertex2f(1.0f, -.5f);
    glVertex2f(.6f, -.3f);
    glVertex2f(-.0f, .1f);
    glVertex2f(-.3f, .3f);
    glVertex2f(-.68f, .5f);
    glVertex2f(-.58f, .5f);
    glVertex2f(-.15f, .4f);
    glVertex2f(.15f, .35f);
    glVertex2f(.4f, .3f);
    glVertex2f(.6f, .25f);
    glVertex2f(.8f, .2f);
    glVertex2f(1.0f, .15f);
    glEnd();
}

//void road_strip() {
//    glBegin(GL_POLYGON);
//
//    glVertex2f(-.35f, .4f);
//    glVertex2f(.3f, .0f);
//    glVertex2f(.3f, -.010f);
//    glVertex2f(-.35f, .39f);
//
//    glEnd();
//}

void hut_floor2() {
    glBegin(GL_POLYGON);

    //back_roof
    glColor3f(0.278,	0.314,	0.275);
    glVertex2f(-1.0f, -.28f);
    glVertex2f(-.45f, -.35f);
    glVertex2f(-.05f, -.25f);
    glVertex2f(-.05f, -.3f);
    glVertex2f(-.45f, -.45f);
    glVertex2f(-1.0f, -.35f);


    glEnd();
}

void roof_strips() {
    glBegin(GL_QUADS);

    //back_roof
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex2f(-.36f, .1f);
    glVertex2f(-.38f, .08f);
    glVertex2f(-.72f, .08f);
    glVertex2f(-.7f, .1f);


    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex2f(-.41f, .05f);
    glVertex2f(-.43f, .03f);
    glVertex2f(-.75f, .03f);
    glVertex2f(-.73f, .05f);


    glEnd();
}

void drawCircle() {
    glBegin(GL_POLYGON);

    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex2f(.25f, -.15f);
    glVertex2f(.30f, -.15f);
    glVertex2f(.35f, -.1f);
    glVertex2f(.30f, -.5f);
    glVertex2f(.25f, -.5f);
    glVertex2f(.22f, -.1f);

    glEnd();
}

void bg_color() {
    glBegin(GL_QUADS);


    //sky
    /*glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex2f(1.0f, .5f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(-1.0f, .5f);
    glEnd();*/

    
    //Grass
    glColor3f(0.5f, 1.0f, 0.5f);
    glVertex2f(1.0f, .5f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.0f, .5f);
    

    glEnd();
}

void sun_lines() {
   //indisplay
}

void drawMountain() {
    glBegin(GL_TRIANGLES); // Start drawing triangles

    // Wall (two triangles forming a rectangle)
    glColor3f(0.5f, 0.2f, 0.0f); // Brown color for the walls
  /*  glVertex2f(.40f, .0f);
    glVertex2f(.40f, -.2f);
    glVertex2f(.2f, -.2f);

    glColor3f(0.5f, 0.1f, 0.0f);
    glVertex2f(.9f, .0f);
    glVertex2f(.9f, -.2f);
    glVertex2f(.983f, -.2f);*/


   /* glVertex2f(-0.05f, -0.05f); // Bottom-left
    glVertex2f(0.05f, -0.05f);  // Bottom-right
    glVertex2f(0.05f, 0.0f);   // Top-right

    glVertex2f(-0.05f, -0.05f); // Bottom-left
    glVertex2f(0.05f, 0.0f);   // Top-right
    glVertex2f(-0.05f, 0.0f);  // Top-left*/
    
   

    // Roof (a triangle)
    // light green
    /* glVertex2f(-0.055f, 0.0f); // Left corner
    glVertex2f(0.055f, 0.0f);  // Right corner
    glVertex2f(0.0f, 0.06f);   // Top center*/

    // Door (two triangles forming a rectangle)
   // glColor3f(0.3f, 0.1f, 0.0f); // Darker brown for the door
    /*  glVertex2f(-0.01f, -0.05f); // Bottom-left
    glVertex2f(0.01f, -0.05f);  // Bottom-right
    glVertex2f(0.01f, -0.02f);  // Top-right

    glVertex2f(-0.01f, -0.05f); // Bottom-left
    glVertex2f(0.01f, -0.02f);  // Top-right
    glVertex2f(-0.01f, -0.02f); // Top-left*/

    
    //Mountains
    glColor3f(0.35f, 0.15f, 0.0f);
    glVertex2f(.63f, .83f);
    glVertex2f(.4f, .5f);
    glVertex2f(.9f, .5f);

    glVertex2f(.95f, .83f);
    glVertex2f(.67f, .5f);
    glVertex2f(1.1f, .5f);

    glVertex2f(-.2f, .81f);
    glVertex2f(.08f, .5f);
    glVertex2f(-.5f, .5f);

    glVertex2f(-.63f, .75f);
    glVertex2f(-.4f, .5f);
    glVertex2f(-.75f, .5f);

    glVertex2f(-.94f, .82f);
    glVertex2f(-.80f, .5f);
    glVertex2f(-1.2f, .5f);

    //back mountain
    glColor3f(0.3f, 0.1f, 0.0f);

    glVertex2f(1.0f, .5f);
    glVertex2f(.80f, .89f);
    glVertex2f(.6f, .5f);   

    glVertex2f(.65f, .5f);
    glVertex2f(.40f, .9f);
    glVertex2f(.2f, .5f);

    glVertex2f(.25f, .5f);
    glVertex2f(.0f, .92f);
    glVertex2f(-.2f, .5f);

    glVertex2f(-.18f, .5f);
    glVertex2f(-.40f, .89f);
    glVertex2f(-.6f, .5f);

    glVertex2f(-.65f, .5f);
    glVertex2f(-.8f, .95f);
    glVertex2f(-1.0f, .5f);


    

    glEnd(); // End drawing triangles
}

void drawBase() {
    glBegin(GL_QUADS); 

    //buttons
    glColor3f(0.1f, 0.1f, 0.0f);
     glVertex2f(.45f, -.9f);
    glVertex2f(.45f, -.87f);
    glVertex2f(.48f, -.87f);
    glVertex2f(.48f, -.9f);


    //roof
   // glColor3f(0.5f, 0.1f, 0.0f);
   // glVertex2f(.40f, .0f);
   // glVertex2f(.9f, .0f);
   // glVertex2f(.9f, -.2f);
   // glVertex2f(.40f, -.2f);
   // 
   // //door_wall
   // glColor3f(0.8f, 0.6f, 0.3f);
   // glVertex2f(.2f, -.2f);
   // glVertex2f(.40f, -.2f);
   // glVertex2f(.40f, -.5f);
   // glVertex2f(.2f, -.5f);

   // //door
   // glColor3f(0.5f, 0.1f, 0.0f);
   // glVertex2f(.24f, -.5f);
   // glVertex2f(.24f, -.34f);
   // glVertex2f(.36f, -.34f);
   // glVertex2f(.36f, -.5f);

   ///*//doornob
   // glColor3f(0.8f, 0.6f, 0.3f);
   // glVertex2f(.26f, -.23f);
   // glVertex2f(.26f, -.22f);
   // glVertex2f(.27f, -.22f);
   // glVertex2f(.27f, -.23f);*/ 

   // //window_wall
   // glColor3f(0.9f, 0.6f, 0.3f);
   // glVertex2f(.40f, -.2f);
   // glVertex2f(.40f, -.5f);
   // glVertex2f(.983f, -.5f);
   // glVertex2f(.983f, -.2f);

   /* //window

    //bg_window
    glColor3f(0.5f, 0.1f, 0.0f);
    glVertex2f(.55f, -.29f);
    glVertex2f(.55f, -.41f);
    glVertex2f(.75f, -.41f);
    glVertex2f(.75f, -.29f);

    //real_color
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex2f(.56f, -.3f);
    glVertex2f(.56f, -.4f);
    glVertex2f(.74f, -.4f);
    glVertex2f(.74f, -.3f);

    //window_bars
    glColor3f(0.5f, 0.1f, 0.0f);
    glVertex2f(.56f, -.35f);
    glVertex2f(.56f, -.36f);
    glVertex2f(.74f, -.36f);
    glVertex2f(.74f, -.35f);

    glVertex2f(.65f, -.3f);
    glVertex2f(.65f, -.4f);
    glVertex2f(.66f, -.4f);
    glVertex2f(.66f, -.3f);*/

    //river
    //glColor3f(0.0f, 0.0f, 1.0f);
    //glVertex2f(.45f, .65f);
    //glVertex2f(.25f, .65f);
    //glVertex2f(-.65f, -1.0f);
    //glVertex2f(-.05f, -1.0f);

    //overbridge
   /*glColor3f(0.5f, 0.1f, 0.0f);
    glVertex2f(.15f, -.6f);
    glVertex2f(-.4f, -.6f);
    glVertex2f(-.4f, -.7f);
    glVertex2f(.15f, -.8f);

    glVertex2f(-.65f, -.8f);
    glVertex2f(-.10f, -.7f);
    glVertex2f(-.10f, -.6f);
    glVertex2f(-.65f, -.6f);
    */
    ////tree1
    //glColor3f(0.5f, 0.1f, 0.0f);
    //glVertex2f(-.64f, -.2f);
    //glVertex2f(-.69f, -.2f);
    //glVertex2f(-.69f, -.4f);
    //glVertex2f(-.64f, -.4f);

    ////tree2
    //glColor3f(0.5f, 0.1f, 0.0f);
    //glVertex2f(-.64f, .1f);
    //glVertex2f(-.69f, .1f);
    //glVertex2f(-.69f, .3f);
    //glVertex2f(-.64f, .3f);

    
    glEnd(); // End drawing triangles
}

//void drawTreePuff1() {
//    glBegin(GL_POLYGON);
//
//    
//    glColor3f(.0f, 1.0f, 0.0f);
//    // glVertex2f(.45f, .65f);
//    glVertex2f(-.62f, -.22f);
//    glVertex2f(-.71f, -.22f);
//    glVertex2f(-.78f, -.17f);
//    glVertex2f(-.78f, -.07f);
//    glVertex2f(-.71f, -.0f);
//    glVertex2f(-.62f, -.0f);
//    glVertex2f(-.55f, -.07f);
//    glVertex2f(-.55f, -.17f);
//
//    glEnd();
//}
//
//void drawTreePuff2() {
//    glBegin(GL_POLYGON);
//
//
//    glColor3f(.0f, 1.0f, 0.0f);
//    // glVertex2f(.45f, .65f);
//    glVertex2f(-.61f, .3f);
//    glVertex2f(-.72f, .3f);
//    glVertex2f(-.78f, .38f);
//    glVertex2f(-.78f, .48f);
//    glVertex2f(-.70f, .57f);
//    glVertex2f(-.61f, .57f);
//    glVertex2f(-.54f, .48f);
//    glVertex2f(-.54f, .38f);
//
//    glEnd();
//}

void drawSun() {

    //indisplay
    
}

//void drawBorder() {
//    glBegin(GL_LINE_LOOP); // Start drawing triangles
//
//    //window
//    glColor3f(0.8f, 0.8f, 0.8f);
//    glVertex2f(.56f, -.3f);
//    glVertex2f(.56f, -.4f);
//    glVertex2f(.74f, -.4f);
//    glVertex2f(.74f, -.3f);
//   
//    glEnd(); // End drawing triangles
//}

void renderBitmapString(float x, float y, void* font, const char* string) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x, y);  // Set the position for the text
    while (*string) {
        glutBitmapCharacter(font, *string);  // Render each character
        string++;
    }
}


void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        float normX = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f;
        float normY = 1.0f - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2.0f;
         
        if (normX >= .45f && normX <= .48f && normY >= -.9f && normY <= -.87f) {
            isClickedSun = !isClickedSun;
           // glutPostRedisplay();
            /*isClickedSky = !isClickedSky;
            glutPostRedisplay();*/
        }
        if (normX >= .45f && normX <= .48f && normY >= -.9f && normY <= -.87f) {
           /* isClickedSun = !isClickedSun;
            glutPostRedisplay();*/
            isClickedSky = !isClickedSky;
            //glutPostRedisplay();
        }

        /*glVertex2f(.24f, -.5f);
        glVertex2f(.24f, -.34f);
        glVertex2f(.36f, -.34f);
        glVertex2f(.36f, -.5f);*/

      /*  if (normX >= 0.65f && normX <= 0.67f && normY >= 0.47f && normY <= 0.50f) {

           // isClickedCloud = !isClickedCloud;

        }

        if (normX >= 0.65f && normX <= 0.67f && normY >= 0.42f && normY <= 0.45f) {

           // isClickedSun = !isClickedSun;

        }

        if (normX >= 0.65f && normX <= 0.67f && normY >= 0.37f && normY <= 0.40f) {

            //isClickedGround = !isClickedGround;

        }*/  

    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    //drawSun();
    bg_color();
    if (isClickedSky) {
        glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex2f(1.0f, .5f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(-1.0f, .5f);
        glEnd();
    }
    if (!isClickedSky){
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(.95f, .92f);
        glVertex2f(.96f, .94f);
        glVertex2f(.96f, .92f);
        glVertex2f(.95f, .94f);
        glVertex2f(.955f, .92f);
        glVertex2f(.955f, .94f);


        glVertex2f(.90f, .90f);
        glVertex2f(.91f, .92f);
        glVertex2f(.91f, .90f);
        glVertex2f(.90f, .92f);
        glVertex2f(.905f, .90f);
        glVertex2f(.905f, .92f);

        glVertex2f(-.90f, .90f);
        glVertex2f(-.91f, .92f);
        glVertex2f(-.91f, .90f);
        glVertex2f(-.90f, .92f);
        glVertex2f(-.905f, .90f);
        glVertex2f(-.905f, .92f);

        glVertex2f(-.94f, .92f);
        glVertex2f(-.95f, .94f);
        glVertex2f(-.95f, .92f);
        glVertex2f(-.94f, .94f);
        glVertex2f(-.945f, .92f);
        glVertex2f(-.945f, .942f);

        glVertex2f(-.66f, .90f);
        glVertex2f(-.67f, .92f);
        glVertex2f(-.67f, .90f);
        glVertex2f(-.66f, .92f);
        glVertex2f(-.665f, .90f);
        glVertex2f(-.665f, .92f);

        glVertex2f(-.54f, .93f);
        glVertex2f(-.55f, .95f);
        glVertex2f(-.55f, .93f);
        glVertex2f(-.54f, .95f);
        glVertex2f(-.545f, .93f);
        glVertex2f(-.5465f, .95f);

        glVertex2f(-.24f, .93f);
        glVertex2f(-.25f, .95f);
        glVertex2f(-.25f, .93f);
        glVertex2f(-.24f, .95f);
        glVertex2f(-.245f, .93f);
        glVertex2f(-.2465f, .95f);

        glVertex2f(-.14f, .90f);
        glVertex2f(-.15f, .92f);
        glVertex2f(-.15f, .90f);
        glVertex2f(-.14f, .92f);
        glVertex2f(-.145f, .90f);
        glVertex2f(-.1465f, .92f);

        glVertex2f(.23f, .92f);
        glVertex2f(.24f, .94f);
        glVertex2f(.24f, .92f);
        glVertex2f(.23f, .94f);
        glVertex2f(.235f, .92f);
        glVertex2f(.235f, .94f);

        glVertex2f(.20f, .84f);
        glVertex2f(.21f, .86f);
        glVertex2f(.21f, .84f);
        glVertex2f(.20f, .86f);
        glVertex2f(.205f, .84f);
        glVertex2f(.205f, .86f);

        glVertex2f(.50f, .84f);
        glVertex2f(.51f, .86f);
        glVertex2f(.51f, .84f);
        glVertex2f(.50f, .86f);
        glVertex2f(.505f, .84f);
        glVertex2f(.505f, .86f);

        glVertex2f(.66f, .88f);
        glVertex2f(.67f, .9f);
        glVertex2f(.67f, .88f);
        glVertex2f(.66f, .9f);
        glVertex2f(.665f, .88f);
        glVertex2f(.665f, .9f);

        glEnd();


        glBegin(GL_QUADS);
        glColor3f(0.4f, .9f, 0.4f);
        glVertex2f(1.0f, .5f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(-1.0f, .5f);
        glEnd();

    }
    

                      // Draw the hut
    drawBase();
    sun_lines();
  //  drawTreePuff1();
  //  drawTreePuff2();
    hut_roof1();
    hut_floor2();
    hut_floor1();
    hut_wall1();
   
    //hut_wall1_shadow();
    window();
    window_shadow();
    hut_wall2();
    
    if (isClickedSun) {
        glBegin(GL_POLYGON);

        //sun
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(.2f, .5f);
        glVertex2f(.068f, .74f);
        glVertex2f(.13f, .79f);
        glVertex2f(.23f, .8f);
        glVertex2f(.32f, .77f);
        glVertex2f(.34f, .74f);
        glVertex2f(.2f, .5f);
        glEnd();

        //sun_lines
        glBegin(GL_LINES);

        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex2f(.1f, .79f);
        glVertex2f(.07f, .84f);
        glVertex2f(.105f, .79f);
        glVertex2f(.075f, .84f);

        glVertex2f(.14f, .8f);
        glVertex2f(.11f, .88f);
        glVertex2f(.145f, .8f);
        glVertex2f(.115f, .88f);

        glVertex2f(.18f, .8f);
        glVertex2f(.17f, .85f);
        glVertex2f(.185f, .8f);
        glVertex2f(.175f, .85f);


        glVertex2f(.22f, .8f);
        glVertex2f(.215f, .88f);
        glVertex2f(.225f, .8f);
        glVertex2f(.2155f, .88f);

        glVertex2f(.26f, .8f);
        glVertex2f(.269f, .85f);
        glVertex2f(.265f, .8f);
        glVertex2f(.2695f, .85f);

        glVertex2f(.3f, .79f);
        glVertex2f(.33f, .87f);
        glVertex2f(.3f, .79f);
        glVertex2f(.33f, .87f);

        glVertex2f(.34f, .78f);
        glVertex2f(.365f, .83f);
        glVertex2f(.345f, .78f);
        glVertex2f(.3655f, .83f);

        /*glVertex2f(.48f, .83f);
        glVertex2f(.54f, .92f);
        glVertex2f(.485f, .83f);
        glVertex2f(.545f, .92f);

        glVertex2f(.52f, .81f);
        glVertex2f(.57f, .84f);
        glVertex2f(.525f, .81f);
        glVertex2f(.575f, .84f);*/
        glEnd();

        //shadows
        glBegin(GL_POLYGON);

        glColor4f(0.1, 0.1, 0.10, .3f);
        glVertex2f(-0.3f, .155f);
        glVertex2f(-.55f, -.125f);
        glVertex2f(-.55f, -.19f);
        glVertex2f(-0.32f, .05f);
        glVertex2f(-.15f, -.19f);
        glVertex2f(-.15f, -.1f);


        glEnd();

        glBegin(GL_POLYGON);

        glColor4f(0.1, 0.1, 0.10, .3f);
        glVertex2f(-.55f, -.21f);
        glVertex2f(-.55f, -.13f);
        glVertex2f(-.85f, -.13f);
        glVertex2f(-0.85f, -.21f);


        glEnd();

        glBegin(GL_POLYGON);

        glColor4f(0.1, 0.1, 0.10, .3f);
        glVertex2f(-.55f, -.3f);
        glVertex2f(-.85f, -.25f);
        glVertex2f(-.87f, -.25f);
        glVertex2f(-.98f, -.283f);
        glVertex2f(-0.65f, -.33f);


        glEnd();


        glBegin(GL_POLYGON);

        glColor3f(.0f, 0.0f, 0.0f);
        glVertex2f(-.43f, -.15f);
        glVertex2f(-.43f, -.26f);
        glVertex2f(-.35f, -.28f);
        glVertex2f(-.35f, -.12f);


        glEnd();

    }
    
    drawMountain();
    hut_roof2();
    roof_strips();
    
    door();
   // open_door();
    if (isClickedSun) {
        glBegin(GL_POLYGON);

        //sun
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(.2f, .5f);
        glVertex2f(.068f, .74f);
        glVertex2f(.13f, .79f);
        glVertex2f(.23f, .8f);
        glVertex2f(.32f, .77f);
        glVertex2f(.34f, .74f);
        glVertex2f(.2f, .5f);
        glEnd();

        //sun_lines
        glBegin(GL_LINES);

        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex2f(.1f, .79f);
        glVertex2f(.07f, .84f);
        glVertex2f(.105f, .79f);
        glVertex2f(.075f, .84f);

        glVertex2f(.14f, .8f);
        glVertex2f(.11f, .88f);
        glVertex2f(.145f, .8f);
        glVertex2f(.115f, .88f);

        glVertex2f(.18f, .8f);
        glVertex2f(.17f, .85f);
        glVertex2f(.185f, .8f);
        glVertex2f(.175f, .85f);


        glVertex2f(.22f, .8f);
        glVertex2f(.215f, .88f);
        glVertex2f(.225f, .8f);
        glVertex2f(.2155f, .88f);

        glVertex2f(.26f, .8f);
        glVertex2f(.269f, .85f);
        glVertex2f(.265f, .8f);
        glVertex2f(.2695f, .85f);

        glVertex2f(.3f, .79f);
        glVertex2f(.33f, .87f);
        glVertex2f(.3f, .79f);
        glVertex2f(.33f, .87f);

        glVertex2f(.34f, .78f);
        glVertex2f(.365f, .83f);
        glVertex2f(.345f, .78f);
        glVertex2f(.3655f, .83f);

        /*glVertex2f(.48f, .83f);
        glVertex2f(.54f, .92f);
        glVertex2f(.485f, .83f);
        glVertex2f(.545f, .92f);

        glVertex2f(.52f, .81f);
        glVertex2f(.57f, .84f);
        glVertex2f(.525f, .81f);
        glVertex2f(.575f, .84f);*/
        glEnd();

        //shadows
        glBegin(GL_POLYGON);

        glColor4f(0.1, 0.1, 0.10, .3f);
        glVertex2f(-0.3f, .155f);
        glVertex2f(-.55f, -.125f);
        glVertex2f(-.55f, -.19f);
        glVertex2f(-0.32f, .05f);
        glVertex2f(-.15f, -.19f);
        glVertex2f(-.15f, -.1f);


        glEnd();

        glBegin(GL_POLYGON);

        glColor4f(0.1, 0.1, 0.10, .3f);
        glVertex2f(-.55f, -.21f);
        glVertex2f(-.55f, -.13f);
        glVertex2f(-.85f, -.13f);
        glVertex2f(-0.85f, -.21f);


        glEnd();

        glBegin(GL_POLYGON);

        glColor4f(0.1, 0.1, 0.10, .3f);
        glVertex2f(-.55f, -.3f);
        glVertex2f(-.85f, -.25f);
        glVertex2f(-.87f, -.25f);
        glVertex2f(-.98f, -.283f);
        glVertex2f(-0.65f, -.33f);


        glEnd();


        glBegin(GL_POLYGON);
        glColor3f(0.0,.0f,.0f);
        glVertex2f(-.35f, -.12f);
        glVertex2f(-.45f, -.13f);
        glVertex2f(-.45f, -.29f);
        glVertex2f(-.35f, -.28f);


        glEnd();

        glBegin(GL_POLYGON);

        glColor3f(0.412f, 0.278f, 0.180f);
        glVertex2f(-.43f, -.15f);
        glVertex2f(-.43f, -.26f);
        glVertex2f(-.35f, -.28f);
        glVertex2f(-.35f, -.12f);


        glEnd();

        glBegin(GL_POLYGON);

        glColor4f(0.0f,0.0f,0.0f ,1.0f);
        glVertex2f(-.3f, -.14f);
        glVertex2f(-.3f, -.2f);
        glVertex2f(-.215f, -.19f);
        glVertex2f(-.215f, -.133f);


        glEnd();
    }
    //hut_wall2_shadow();
//    kite();
    road();
    //road_strip();

    renderBitmapString(0.49f, -0.9f, GLUT_BITMAP_HELVETICA_18, "Day/Night");

    renderBitmapString(0.45f, -0.95f, GLUT_BITMAP_HELVETICA_18, "Sanskar Rathore, 500108014");

    glFlush();                    // Render now
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                            // Initialize GLUT
    glutInitWindowSize(1000, 1000);                     // Set the window size
    glutInitWindowPosition(400, 0);                 // Position the window
    glutCreateWindow("Hut Assignment 1");         // Create window
    glutDisplayFunc(display);                         // Register display callback function
    glutMouseFunc(mouse);
    initGL();                                         // Setup OpenGL
    glutMainLoop();                                   // Enter the event-processing loop
    //glutPostRedisplay();
    return 0;
}
