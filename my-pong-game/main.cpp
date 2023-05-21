//
//  main.cpp
//  my-pong-game
//
//  Created by asamahy on 5/21/23.
//
#include <iostream>
#include <GLUT/GLUT.h>
#include <string>
using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;

// Paddle dimensions
int paddleWidth = 10;
int paddleHeight = 80;

// Paddle positions
int paddle1Y = windowHeight / 2 - paddleHeight / 2;
int paddle2Y = windowHeight / 2 - paddleHeight / 2;

// Ball dimensions
int ballSize = 10;

// Ball position and speed
float ballX = windowWidth / 2 - ballSize / 2;
float ballY = windowHeight / 2 - ballSize / 2;
float ballSpeedX = -2.0f;
float ballSpeedY = 1.5f;

// Scores
int score1 = 0;
int score2 = 0;

// Game states
enum GameState { MENU, PLAYING, EXIT };
GameState gameState = MENU;

// Menu options
int menuOption = 1;

// Keyboard input flags
bool isPaddle1UpPressed = false;
bool isPaddle1DownPressed = false;
bool isPaddle2UpPressed = false;
bool isPaddle2DownPressed = false;

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

void displayMenu() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    // Set the color for the menu text
    glColor3f(1.0f, 1.0f, 1.0f);

    // Set the position of the menu options
    float textPosX = windowWidth / 2 - 60;
    float textPosY = windowHeight / 2 + 20;

    // Draw the menu options
    string option1 = "Start Game";
    if (menuOption == 1) {
        glRasterPos2f(textPosX - 10, textPosY);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '>');
    }
    glRasterPos2f(textPosX, textPosY);
    for (int i = 0; i < option1.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, option1[i]);
    }

    string option2 = "Quit";
    if (menuOption == 2) {
        glRasterPos2f(textPosX - 10, textPosY + 30);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '>');
    }
    glRasterPos2f(textPosX, textPosY + 30);
    for (int i = 0; i < option2.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, option2[i]);
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

void specialKeyPressed(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (gameState == MENU) {
                if (menuOption > 1)
                    menuOption--;
            }
            break;
        case GLUT_KEY_DOWN:
            if (gameState == MENU) {
                if (menuOption < 2)
                    menuOption++;
            }
            break;
    }
}

void keyPressed(unsigned char key, int x, int y) {
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
        case ' ':
        case 13: // Enter key
            if (gameState == MENU) {
                if (menuOption == 1)
                    gameState = PLAYING;
                else if (menuOption == 2)
                    gameState = EXIT;
            }
            break;
        case 27: // ESC key
            gameState = EXIT;
            break;
        case 9: // Tab key
            if (gameState == MENU) {
                if (menuOption == 1)
                    menuOption = 2;
                else if (menuOption == 2)
                    menuOption = 1;
            }
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
    glViewport(0, 0, width, height);
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
    glutSpecialFunc(specialKeyPressed);
    glutKeyboardUpFunc(keyReleased);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutMainLoop();

    return 0;
}

#pragma GCC diagnostic pop
