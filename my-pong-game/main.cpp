//
//  main.cpp
//  my-pong-game
//
//  Created by asamahy on 5/21/23.
//

#include <GLUT/GLUT.h> // Include the GLUT library
#include <string>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
using namespace std;
//#include <GL/glut.h> // Include the GLUT library

int windowWidth = 800; // Width of the game window
int windowHeight = 600; // Height of the game window

int paddleWidth = 10; // Width of the paddles
int paddleHeight = 80; // Height of the paddles

int paddle1Y = windowHeight / 2 - paddleHeight / 2; // Y-coordinate of the first paddle
int paddle2Y = windowHeight / 2 - paddleHeight / 2; // Y-coordinate of the second paddle

int ballSize = 10; // Diameter of the ball
float ballX = windowWidth / 2 - ballSize / 2; // X-coordinate of the ball
float ballY = windowHeight / 2 - ballSize / 2; // Y-coordinate of the ball
float ballSpeedX = -2.0f; // X-axis speed of the ball
float ballSpeedY = 1.5f; // Y-axis speed of the ball

int score1 = 0; // Score of player 1
int score2 = 0; // Score of player 2

bool isPaddle1UpPressed = false;
bool isPaddle1DownPressed = false;
bool isPaddle2UpPressed = false;
bool isPaddle2DownPressed = false;

enum GameState { MENU, PLAYING }; // Game states
GameState gameState = MENU;

void drawPaddle(float x, float y) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + paddleWidth, y);
    glVertex2f(x + paddleWidth, y + paddleHeight);
    glVertex2f(x, y + paddleHeight);
    glEnd();
}

void drawBall(float x, float y) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + ballSize, y);
    glVertex2f(x + ballSize, y + ballSize);
    glVertex2f(x, y + ballSize);
    glEnd();
}

//void displayMenu() {
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    glLoadIdentity();
//
//    // Draw the menu text
//    glRasterPos2f(windowWidth / 2 - 60, windowHeight / 2);
//    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"Press SPACE to start");
//
//    glutSwapBuffers();
//}
void displayMenu() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    // Set the color for the menu text
    glColor3f(1.0f, 1.0f, 1.0f);

    // Set the position of the menu text
    float textPosX = windowWidth / 2 - 100;
    float textPosY = windowHeight / 2;

    // Draw each character of the menu text
    string menuText = "Press SPACE to start";
    for (int i = 0; i < menuText.length(); i++) {
        glRasterPos2f(textPosX, textPosY);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, menuText[i]);
        textPosX += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, menuText[i]);
    }

    glutSwapBuffers();
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    if (gameState == MENU) {
        displayMenu();
    } else if (gameState == PLAYING) {
        // Draw the paddles
        drawPaddle(10, paddle1Y);
        drawPaddle(windowWidth - paddleWidth - 10, paddle2Y);

        // Draw the ball
        drawBall(ballX, ballY);

        // Draw the scores
        glRasterPos2f(windowWidth / 2 - 20, 20);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score1 + '0');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '-');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score2 + '0');
    }

    glutSwapBuffers();
}

void update(int value) {
    if (gameState == PLAYING) {
        // Update the ball position
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        // Ball collision with the paddles
        if (ballX <= paddleWidth && ballY >= paddle1Y && ballY <= paddle1Y + paddleHeight)
            ballSpeedX = -ballSpeedX;

        if (ballX >= windowWidth - paddleWidth - ballSize && ballY >= paddle2Y && ballY <= paddle2Y + paddleHeight)
            ballSpeedX = -ballSpeedX;

        // Ball collision with the top and bottom walls
        if (ballY >= windowHeight - ballSize || ballY <= 0)
            ballSpeedY = -ballSpeedY;

        // Paddle movement
        if (isPaddle1UpPressed && paddle1Y > 0)
            paddle1Y -= 5;
        if (isPaddle1DownPressed && paddle1Y < windowHeight - paddleHeight)
            paddle1Y += 5;
        if (isPaddle2UpPressed && paddle2Y > 0)
            paddle2Y -= 5;
        if (isPaddle2DownPressed && paddle2Y < windowHeight - paddleHeight)
            paddle2Y += 5;

        // Ball out of bounds
        if (ballX < 0) {
            score2++;
            ballX = windowWidth / 2 - ballSize / 2;
            ballY = windowHeight / 2 - ballSize / 2;
            ballSpeedX = -ballSpeedX;
        }
        if (ballX > windowWidth - ballSize) {
            score1++;
            ballX = windowWidth / 2 - ballSize / 2;
            ballY = windowHeight / 2 - ballSize / 2;
            ballSpeedX = -ballSpeedX;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

void keyPressed(unsigned char key, int x, int y) {
    if (gameState == MENU && key == ' ') {
        gameState = PLAYING;
    }

    switch (key) {
        case 'w':
            isPaddle1UpPressed = true;
            break;
        case 's':
            isPaddle1DownPressed = true;
            break;
        case 'i':
            isPaddle2UpPressed = true;
            break;
        case 'k':
            isPaddle2DownPressed = true;
            break;
    }
}

void keyReleased(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            isPaddle1UpPressed = false;
            break;
        case 's':
            isPaddle1DownPressed = false;
            break;
        case 'i':
            isPaddle2UpPressed = false;
            break;
        case 'k':
            isPaddle2DownPressed = false;
            break;
    }
}

void reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Pong Game");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(10, update, 0);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyReleased);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutMainLoop();
    return 0;
}
#pragma GCC diagnostic pop
