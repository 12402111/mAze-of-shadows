#include <math.h>
#include "iGraphics.h"
#include "iSound.h"

// Screen dimensions
const int screenWidth = 1000;
const int screenHeight = 600;

// Navigation
const int buttonWidth = 150;
const int buttonHeight = 50;
int currentScreen = 0;
char* navItems[] = {"Start", "About", "Settings", "Help"};
bool isPlayButtonClicked = false;

// Player
int playerX = 500, playerY = 50;
const int playerWidth = 55, playerHeight = 100;
const int jumpSpeed = 20, gravity = -2;
int verticalSpeed = 0;
bool movingRight = false, movingLeft = false, isJumping = false;

// Game world
const int worldScreenCount = 3;
const int worldWidth = worldScreenCount * screenWidth;
int worldX = 0;

// Obstacles
typedef struct {
    int x, y;
    int width, height;
    bool active;
    const char* soundFile;
    const char* imageFile;
    Image img;
} Obstacle;

Obstacle obstacles[] = {
    {800, 50, 70, 120, true, "saves/assets/sounds/chime.wav", "saves/assets/images/obstacles/tree1.png"},
    {1200, 50, 70, 120, true, "saves/assets/sounds/chime.wav", "saves/assets/images/obstacles/tree1.png"},
    {1600, 50, 90, 50, true, "saves/assets/sounds/chime.wav", "saves/assets/images/obstacles/fire1.png"},
    {2000, 50, 120, 50, true, "saves/assets/sounds/chime.wav", "saves/assets/images/obstacles/log1.png"}
};
const int obstacleCount = sizeof(obstacles) / sizeof(Obstacle);

// Graphics
Image bg;
Image runRight[18];
Sprite playerSprite;
const int runFrameCount = 8;

void iKeyboard(unsigned char key) {
    if (key == 'q') exit(0);
}

void iMouseMove(int mx, int my) {}
void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int x, int y) {}

void loadImages() {
    iLoadImage(&bg, "saves/assets/images/backimg2.png");
    iResizeImage(&bg, screenWidth, screenHeight);

    iLoadFramesFromFolder(runRight, "saves/assets/images/sprites/ninjas");
    iInitSprite(&playerSprite, -1);
    iChangeSpriteFrames(&playerSprite, runRight, runFrameCount);
    iResizeSprite(&playerSprite, playerWidth, playerHeight);
    iSetSpritePosition(&playerSprite, playerX, playerY);
}

void loadObstacles() {
    for (int i = 0; i < obstacleCount; i++) {
        if (iLoadImage(&obstacles[i].img, obstacles[i].imageFile)) {
            iResizeImage(&obstacles[i].img, obstacles[i].width, obstacles[i].height);
        }
    }
}

void drawPlayButton() {
    iSetTransparentColor(61, 174, 191, 0.1);
    iFilledCircle((screenWidth / 4) * 3, (screenHeight / 4), 44);
    iSetTransparentColor(255, 128, 31, 0.8);
    iFilledCircle((screenWidth / 4) * 3, (screenHeight / 4), 40);
    iSetColor(60, 60, 60);
    iText((screenWidth / 4) * 3 - 20, (screenHeight / 4) - 5, "▶Play", GLUT_BITMAP_HELVETICA_18);
}

void drawNavigationBar() {
    iShowImage(0, 0, "saves/assets/images/start.png");
    for (int i = 0; i < 4; i++) {
        int x = screenWidth - buttonWidth - 50;
        int y = screenHeight - 110 - (i + 1) * buttonHeight - i * 60;
        iSetTransparentColor(255, 255, 255, 0.3);
        iFilledRectangle(x, y, buttonWidth, buttonHeight);
        iSetColor(252, 106, 43);
        iText(x + buttonWidth / 2 - 20, y + buttonHeight / 2 - 5, navItems[i], GLUT_BITMAP_HELVETICA_18);
    }
}

void drawBackground() { iShowLoadedImage(0, 0, &bg); }
void drawPlayer() { iSetSpritePosition(&playerSprite, playerX, playerY); iShowSprite(&playerSprite); }

void drawObstacles() {
    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i].active) {
            int screenX = obstacles[i].x - worldX;
            if (screenX > -obstacles[i].width && screenX < screenWidth) {
                iShowImage2(screenX, obstacles[i].y, &obstacles[i].img, -1);
            }
        }
    }
}

void animatePlayer() {
    if (movingRight || movingLeft) iAnimateSprite(&playerSprite);
}

void checkCollisions() {
    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i].active) {
            int obsScreenX = obstacles[i].x - worldX;
            if (playerX + playerWidth > obsScreenX &&
                playerX < obsScreenX + obstacles[i].width &&
                playerY + playerHeight > obstacles[i].y &&
                playerY < obstacles[i].y + obstacles[i].height) {
                if (playerY + playerHeight < obstacles[i].y + obstacles[i].height * 0.7) {
                    if (obstacles[i].soundFile) {
                        iPlaySound(obstacles[i].soundFile);
                        obstacles[i].soundFile = NULL;
                    }
                    if (movingRight) {
                        playerX = obsScreenX - playerWidth - 5;
                        worldX -= 5;
                    } else if (movingLeft) {
                        playerX = obsScreenX + obstacles[i].width + 5;
                        worldX += 5;
                    }
                }
            }
        }
    }
}

void updatePlayer() {
    if (playerY > 50 || verticalSpeed > 0) {
        playerY += verticalSpeed;
        verticalSpeed += gravity;
    }
    if (playerY <= 50) {
        playerY = 50;
        isJumping = false;
        verticalSpeed = 0;
    }
    if (movingRight) {
        if (worldX < worldWidth - screenWidth) {
            worldX += 5;
            iWrapImage(&bg, -5);
        } else if (playerX < screenWidth - playerWidth - 10) {
            playerX += 5;
        }
    } else if (movingLeft) {
        if (worldX > 0) {
            worldX -= 5;
            iWrapImage(&bg, 5);
        } else if (playerX > 10) {
            playerX -= 5;
        }
    }
    checkCollisions();
}

void startScreen() { drawBackground(); drawObstacles(); drawPlayer(); }
void aboutScreen() { iSetColor(255, 255, 255); iText(screenWidth / 2 - 100, screenHeight / 2, "About Screen", GLUT_BITMAP_HELVETICA_18); }
void settingsScreen() { iSetColor(255, 255, 255); iText(screenWidth / 2 - 100, screenHeight / 2, "Settings Screen", GLUT_BITMAP_HELVETICA_18); }
void helpScreen() { iSetColor(255, 255, 255); iText(screenWidth / 2 - 100, screenHeight / 2, "Help Screen", GLUT_BITMAP_HELVETICA_18); }

void iDraw() {
    iClear();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (!isPlayButtonClicked) {
        iShowImage(0, 0, "saves/assets/images/start.png");
        drawPlayButton();
    } else {
        drawNavigationBar();
        switch (currentScreen) {
            case 1: startScreen(); break;
            case 2: aboutScreen(); break;
            case 3: settingsScreen(); break;
            case 4: helpScreen(); break;
        }
    }
}

void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        iPlaySound("saves/assets/sounds/start.wav");
        if (!isPlayButtonClicked) {
            int centerX = (screenWidth / 4) * 3;
            int centerY = screenHeight / 4;
            if (sqrt(pow(mx - centerX, 2) + pow(my - centerY, 2)) <= 40) {
                isPlayButtonClicked = true;
            }
        } else {
            int x = screenWidth - buttonWidth - 50;
            if (mx >= x && mx <= x + buttonWidth) {
                if (my >= 440 && my <= 440 + buttonHeight) currentScreen = 1;
                else if (my >= 330 && my <= 330 + buttonHeight) currentScreen = 2;
                else if (my >= 220 && my <= 220 + buttonHeight) currentScreen = 3;
                else if (my >= 110 && my <= 110 + buttonHeight) currentScreen = 4;
            }
        }
    }
}

void iSpecialKeyboard(unsigned char key) {
    if (key == GLUT_KEY_HOME) currentScreen = 0;
    if (key == GLUT_KEY_LEFT) {
        if (movingRight) iMirrorSprite(&playerSprite, HORIZONTAL);
        movingLeft = true; movingRight = false;
    }
    if (key == GLUT_KEY_RIGHT) {
        if (movingLeft) iMirrorSprite(&playerSprite, HORIZONTAL);
        movingRight = true; movingLeft = false;
    }
    if (key == GLUT_KEY_UP && !isJumping) {
        isJumping = true;
        verticalSpeed = jumpSpeed;
        iPlaySound("saves/assets/sounds/chime.wav");
    }
}

void gameTimer() {
    updatePlayer();
    iResumeTimer(0);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    loadImages();
    loadObstacles();
    iSetTimer(120, animatePlayer);
    iSetTimer(20, gameTimer);
    iInitializeSound();
    iInitialize(screenWidth, screenHeight, "Maze of Shadows");
    return 0;
}
