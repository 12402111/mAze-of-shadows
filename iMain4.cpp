#include "iGraphics.h"
#include "iSound.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Constants
#define NUM_OBSTACLES 30
#define MAX_LEAVES 50
#define MAX_STARS 100
#define MAX_GOLEMS 20
#define MAX_BARBARIANS 10
#define MAX_HIGH_SCORES 10
#define WORLD_SCREENS 5
#define HIT_COOLDOWN_LIMIT 30
#define MOVE_INTERVAL 10

// Sprite frame counts
const int runFrameCount = 8;
const int starthrowframecount = 6;
const int golemFrameCount = 12;
const int barbarianFrameCount = 12;

// Image declarations
Image fire1, log1, tree1_png;
Image nameEntryImage, navBg, highScoreBg;
Image bg1, bg, leafImg, throwingStar;
Image runRight[18], starthrowing[6], golemImage[12], barbarianImage[12];
Image settingsmenu, soundon, soundoff, songon, songoff, knob;
Image endImage;

// Game state structures
typedef struct {
    char name[100];
    int score;
} HighScore;

typedef struct {
    int x, y;
    int speed;
    bool active;
} Leaf;

typedef struct {
    int x, y;
    bool active;
} Star;

typedef struct {
    int x, y;
    int speed;
    bool active;
    int health;
} Golem;

typedef struct {
    int x, y;
    int speed;
    bool active;
    int health;
} Barbarian;

// Global game state
int screenWidth = 1000, screenHeight = 600;
int worldWidth = WORLD_SCREENS * screenWidth, worldX = 0;

int playerX = 500, playerY = 50, playerWidth = 55, playerHeight = 100;
int playerHealth = 3, playerScore = 0;
bool isJumping = false;
int jumpSpeed = 20, gravity = -2, verticalSpeed = 0;

bool moving = false, movingRight = false, movingLeft = false;
int moveTimer = 0;

Sprite playerSprite, starthrow, golemSprite, barbarianSprite;
bool isthrowingstar = false, starthrowdirection = false;
int throwingFrame = 0, starSpeed = 15;

int obstacleX[NUM_OBSTACLES], obstacleY[NUM_OBSTACLES];
Leaf leaves[MAX_LEAVES];
Star stars[MAX_STARS];
Golem golems[MAX_GOLEMS];
Barbarian barbarians[MAX_BARBARIANS];
int numGolems = 0, numBarbarians = 0;

HighScore highScores[MAX_HIGH_SCORES];
int numHighScores = 0;

char playerName[100] = "";
bool enteringName = false;
int nameCharIndex = 0;

int currentScreen = 0;
int navY = 490, buttonWidth = 150, buttonHeight = 50;
bool isPlayButtonClicked = false;

// Sound settings
bool sound = true, song = true;
int soundvolume = 50, songvolume = 50;
int soundknobx = 643, soundknoby = 333;
int songknobx = 643, songknoby = 245;
bool soundknobclicked = false, songknobclicked = false;
int bgsongchannel = -1;
int lastSongVolume = -1;
bool wasSongOn = false;

// Game mechanics
bool hitCooldown = false;
int hitCooldownTimer = 0;
bool endSoundPlayed = false;

// Enemy spawn timers
int golemSpawnTimer = 0, barbarianSpawnTimer = 0;
const int GOLEM_SPAWN_DELAY = 200;
const int BARBARIAN_SPAWN_DELAY = 100;
int nextGolemX = 1000;
int nextBarbarianX = nextGolemX + 500;

// Direction flags
bool golemRight = true, golemLeft = false;
bool barbarianRight = true, barbarianLeft = false;

// Function prototypes
void load_Images();
void generateObstacles();
void drawRoad();
void drawObstacles();
void drawHealthBar();
void updateLeaves();
void drawLeaves();
void drawgolem();
void updateGolems();
void checkStarGolemCollision();
void checkGolemCollision();
void animategolem();
void spawnGolemIfNeeded();
void drawBarbarian();
void updateBarbarians();
void checkStarBarbarianCollision();
void checkBarbarianCollision();
void animateBarbarian();
void spawnBarbarianIfNeeded();
void loadHighScores();
void saveHighScores();
void insertScore(const char *name, int score);
void iPlayButton();
void drawNavigationBar();
bool mouseNavigationBar(int mx, int my);
void drawBackground();
void drawSprite();
void animatePlayer();
void animatestarthrow();
void playBGSong();
void drawstar();
void updatestar();
void start();
void updatePlayer();
void soundVolume();
void songVolume();
void settings();
void mouseSettings(int mx, int my);
void about();
void help();
void resetGame();
void cleanupGame();
void iDraw();
void iMouseMove(int mx, int my);
void iMouseDrag(int mx, int my);
void iMouse(int button, int state, int mx, int my);
void iMouseWheel(int dir, int mx, int my);
void iKeyboard(unsigned char key);
void iSpecialKeyboard(unsigned char key);
void SpecialKeyboardUP();
void iTimer();

// Main game functions implementation
void load_Images() {
    // Load all game images with error checking
    if (!iLoadImage(&nameEntryImage, "saves/assets/images/namescreen.jpg")) {
        printf("Error loading name entry image!\n");
        exit(1);
    }
    iResizeImage(&nameEntryImage, 900, 600);
    
    if (!iLoadImage(&leafImg, "saves/assets/images/leaf.png")) {
        printf("Error loading leaf image!\n");
        exit(1);
    }
    iResizeImage(&leafImg, 20, 20);

    if (!iLoadImage(&highScoreBg, "saves/assets/images/highscore.jpg")) {
        printf("Error loading highscore background!\n");
        exit(1);
    }
    iResizeImage(&highScoreBg, 1000, 600);

    if (!iLoadImage(&fire1, "saves/assets/images/obstacles/fire1.png")) {
        printf("Error loading fire obstacle!\n");
        exit(1);
    }
    iResizeImage(&fire1, 80, 80);

    if (!iLoadImage(&log1, "saves/assets/images/obstacles/log1.png")) {
        printf("Error loading log obstacle!\n");
        exit(1);
    }
    iResizeImage(&log1, 80, 80);

    if (!iLoadImage(&tree1_png, "saves/assets/images/obstacles/tree1.png")) {
        printf("Error loading tree obstacle!\n");
        exit(1);
    }
    iResizeImage(&tree1_png, 80, 80);

    if (!iLoadImage(&navBg, "saves/assets/images/ninja.png")) {
        printf("Error loading navigation background!\n");
        exit(1);
    }
    iResizeImage(&navBg, 1000, 600);

    if (!iLoadImage(&bg1, "saves/assets/images/start.png")) {
        printf("Error loading start screen!\n");
        exit(1);
    }
    iResizeImage(&bg1, 1000, 600);

    if (!iLoadImage(&throwingStar, "saves/assets/images/starthrowing.png")) {
        printf("Error loading throwing star!\n");
        exit(1);
    }
    iResizeImage(&throwingStar, 20, 20);

    if (!iLoadImage(&settingsmenu, "saves/assets/images/settingBg.png")) {
        printf("Error loading settings menu!\n");
        exit(1);
    }
    iResizeImage(&settingsmenu, 1000, 600);

    if (!iLoadImage(&soundon, "saves/assets/images/SoundOn.png")) {
        printf("Error loading sound on image!\n");
        exit(1);
    }
    iResizeImage(&soundon, 80, 80);

    if (!iLoadImage(&soundoff, "saves/assets/images/SoundOff.png")) {
        printf("Error loading sound off image!\n");
        exit(1);
    }
    iResizeImage(&soundoff, 80, 80);

    if (!iLoadImage(&songon, "saves/assets/images/starthrowing.png")) {
        printf("Error loading song on image!\n");
        exit(1);
    }
    iResizeImage(&songon, 65, 65);

    if (!iLoadImage(&songoff, "saves/assets/images/SongOff.png")) {
        printf("Error loading song off image!\n");
        exit(1);
    }
    iResizeImage(&songoff, 65, 65);

    if (!iLoadImage(&knob, "saves/assets/images/starthrowing.png")) {
        printf("Error loading knob image!\n");
        exit(1);
    }
    iResizeImage(&knob, 14, 14);

    if (!iLoadImage(&endImage, "saves/assets/images/end.png")) {
        printf("Error loading end screen!\n");
        exit(1);
    }
    iResizeImage(&endImage, 1000, 600);

    if (!iLoadImage(&bg, "saves/assets/images/backimg2.png")) {
        printf("Error loading game background!\n");
        exit(1);
    }
    iResizeImage(&bg, 1000, 600);

    // Load sprites with error checking
    if (!iLoadFramesFromFolder(runRight, "saves/assets/images/sprites/ninjas")) {
        printf("Error loading player run frames!\n");
        exit(1);
    }
    iInitSprite(&playerSprite, -1);
    iChangeSpriteFrames(&playerSprite, runRight, runFrameCount);
    iResizeSprite(&playerSprite, playerWidth, playerHeight);
    iSetSpritePosition(&playerSprite, playerX, playerY);

    if (!iLoadFramesFromFolder(starthrowing, "saves/assets/images/sprites/starthrowing")) {
        printf("Error loading throwing star frames!\n");
        exit(1);
    }
    iInitSprite(&starthrow, -1);
    iChangeSpriteFrames(&starthrow, starthrowing, starthrowframecount);
    iResizeSprite(&starthrow, playerWidth, playerHeight);
    iSetSpritePosition(&starthrow, playerX, playerY);

    if (!iLoadFramesFromFolder(golemImage, "saves/assets/images/sprites/Golem_2/RunThrowing")) {
        printf("Error loading golem frames!\n");
        exit(1);
    }
    iInitSprite(&golemSprite, -1);
    iChangeSpriteFrames(&golemSprite, golemImage, golemFrameCount);
    iResizeSprite(&golemSprite, playerWidth, playerHeight);

    if (!iLoadFramesFromFolder(barbarianImage, "saves/assets/images/sprites/barbarians/RunThrowing2")) {
        printf("Error loading barbarian frames!\n");
        exit(1);
    }
    iInitSprite(&barbarianSprite, -1);
    iChangeSpriteFrames(&barbarianSprite, barbarianImage, barbarianFrameCount);
    iResizeSprite(&barbarianSprite, playerWidth, playerHeight);
}

void generateObstacles() {
    int spacing = 350;
    for (int i = 0; i < NUM_OBSTACLES && i < NUM_OBSTACLES; ++i) {
        obstacleX[i] = 300 + i * spacing;
        obstacleY[i] = 30;
    }
}

void drawRoad() {
    int roadHeight = 50;
    iSetColor(60, 60, 60);
    iFilledRectangle(0, 0, screenWidth, roadHeight);
}

void drawObstacles() {
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        int drawX = obstacleX[i] - worldX;
        if (drawX + 80 < 0 || drawX > screenWidth) continue;

        int type = i % 3;
        if (type == 0) iShowLoadedImage(drawX, obstacleY[i], &log1);
        else if (type == 1) iShowLoadedImage(drawX, obstacleY[i], &tree1_png);
        else iShowLoadedImage(drawX, obstacleY[i], &fire1);
    }
}

void drawHealthBar() {
    int barWidth = 150, barHeight = 20;
    int barX = 20, barY = screenHeight - 40;

    iSetColor(255, 0, 0);
    iFilledRectangle(barX, barY, barWidth, barHeight);

    iSetColor(0, 255, 0);
    iFilledRectangle(barX, barY, barWidth * playerHealth / 3, barHeight);

    iSetColor(255, 255, 255);
    iRectangle(barX, barY, barWidth, barHeight);
    
    char scoreText[20];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", playerScore);
    iSetColor(255, 255, 0);
    iText(barX, barY - 25, scoreText, GLUT_BITMAP_HELVETICA_18);
}

void updateLeaves() {
    for (int i = 0; i < MAX_LEAVES; i++) {
        if (leaves[i].active) {
            leaves[i].y -= leaves[i].speed;
            if (leaves[i].y < 0) {
                leaves[i].x = rand() % screenWidth;
                leaves[i].y = screenHeight + (rand() % 200);
                leaves[i].speed = rand() % 3 + 1;
            }
        }
    }
}

void drawLeaves() {
    for (int i = 0; i < MAX_LEAVES; i++) {
        if (leaves[i].active) {
            iShowLoadedImage(leaves[i].x, leaves[i].y, &leafImg);
        }
    }
}

void drawgolem() {
    for (int i = 0; i < numGolems && i < MAX_GOLEMS; i++) {
        if (!golems[i].active) continue;

        int gx = golems[i].x - worldX;
        if (gx >= -100 && gx <= screenWidth + 100) {
            golems[i].y = 50;
            iSetSpritePosition(&golemSprite, gx, golems[i].y);
            iShowSprite(&golemSprite);
        }
    }
}

void updateGolems() {
    for (int i = 0; i < numGolems && i < MAX_GOLEMS; i++) {
        if (!golems[i].active) continue;

        int gx = golems[i].x;
        int px = playerX + worldX;

        if (gx > px && gx - px <= 250) {
            if (!golemLeft) {
                iMirrorSprite(&golemSprite, HORIZONTAL);
                golemLeft = true;
                golemRight = false;
            }
            golems[i].x -= golems[i].speed;
        }
        else if (gx < px && px - gx <= 250) {
            if (!golemRight) {
                iMirrorSprite(&golemSprite, HORIZONTAL);
                golemRight = true;
                golemLeft = false;
            }
            golems[i].x += golems[i].speed;
        }
    }
}

void checkStarGolemCollision() {
    for (int i = 0; i < MAX_STARS; i++) {
        if (!stars[i].active) continue;

        for (int j = 0; j < numGolems && j < MAX_GOLEMS; j++) {
            if (!golems[j].active) continue;

            int gx = golems[j].x - worldX;
            int gy = golems[j].y;
            int gWidth = playerWidth;
            int gHeight = playerHeight;

            if (stars[i].x < gx + gWidth && stars[i].x + 20 > gx && 
                stars[i].y < gy + gHeight && stars[i].y + 20 > gy) {
                golems[j].health--;
                stars[i].active = false;

                if (golems[j].health <= 0) {
                    golems[j].active = false;
                    playerScore += 100;
                }
            }
        }
    }
}

void checkGolemCollision() {
    if (hitCooldown) return;

    for (int i = 0; i < numGolems && i < MAX_GOLEMS; i++) {
        if (!golems[i].active) continue;

        int gx = golems[i].x - worldX;
        int gy = golems[i].y;
        int gWidth = playerWidth;
        int gHeight = playerHeight;

        if (playerX - 35 <= gx + gWidth && playerX + playerWidth - 35 >= gx && 
            playerY <= gy + gHeight && playerY + playerHeight >= gy) {
            if (sound) {
                iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
            }

            playerHealth--;
            hitCooldown = true;

            if (playerHealth <= 0) {
                playerHealth = 0;
                currentScreen = 5;
            }
            break;
        }
    }
}

void animategolem() {
    iAnimateSprite(&golemSprite);
}

void spawnGolemIfNeeded() {
    if (numGolems >= MAX_GOLEMS) return;

    if (golemSpawnTimer >= GOLEM_SPAWN_DELAY) {
        golems[numGolems].x = nextGolemX;
        golems[numGolems].y = 45;
        golems[numGolems].speed = 2;
        golems[numGolems].health = 2;
        golems[numGolems].active = true;

        numGolems++;
        golemSpawnTimer = 0;
        nextGolemX += 700 + rand() % 300;
    } else {
        golemSpawnTimer++;
    }
}

void drawBarbarian() {
    for (int i = 0; i < numBarbarians && i < MAX_BARBARIANS; i++) {
        if (!barbarians[i].active) continue;

        int bx = barbarians[i].x - worldX;
        if (bx >= -100 && bx <= screenWidth + 100) {
            barbarians[i].y = 50;
            iSetSpritePosition(&barbarianSprite, bx, barbarians[i].y);
            iShowSprite(&barbarianSprite);
        }
    }
}

void updateBarbarians() {
    for (int i = 0; i < numBarbarians && i < MAX_BARBARIANS; i++) {
        if (!barbarians[i].active) continue;

        int bx = barbarians[i].x;
        int px = playerX + worldX;

        if (bx > px && bx - px <= 250) {
            if (!barbarianLeft) {
                iMirrorSprite(&barbarianSprite, HORIZONTAL);
                barbarianLeft = true;
                barbarianRight = false;
            }
            barbarians[i].x -= barbarians[i].speed;
        }
        else if (bx < px && px - bx <= 250) {
            if (!barbarianRight) {
                iMirrorSprite(&barbarianSprite, HORIZONTAL);
                barbarianRight = true;
                barbarianLeft = false;
            }
            barbarians[i].x += barbarians[i].speed;
        }
    }
}

void checkStarBarbarianCollision() {
    for (int i = 0; i < MAX_STARS; i++) {
        if (!stars[i].active) continue;

        for (int j = 0; j < numBarbarians && j < MAX_BARBARIANS; j++) {
            if (!barbarians[j].active) continue;

            int bx = barbarians[j].x - worldX;
            int by = barbarians[j].y;
            int bWidth = playerWidth;
            int bHeight = playerHeight;

            if (stars[i].x < bx + bWidth && stars[i].x + 20 > bx && 
                stars[i].y < by + bHeight && stars[i].y + 20 > by) {
                barbarians[j].health--;
                stars[i].active = false;

                if (barbarians[j].health <= 0) {
                    barbarians[j].active = false;
                    playerScore += 100;
                }
            }
        }
    }
}

void checkBarbarianCollision() {
    if (hitCooldown) return;

    for (int i = 0; i < numBarbarians && i < MAX_BARBARIANS; i++) {
        if (!barbarians[i].active) continue;

        int bx = barbarians[i].x - worldX;
        int by = barbarians[i].y;
        int bWidth = playerWidth;
        int bHeight = playerHeight;

        if (playerX - 35 <= bx + bWidth && playerX + playerWidth - 35 >= bx && 
            playerY <= by + bHeight && playerY + playerHeight >= by) {
            if (sound) {
                iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
            }

            playerHealth--;
            hitCooldown = true;

            if (playerHealth <= 0) {
                playerHealth = 0;
                currentScreen = 5;
            }
            break;
        }
    }
}

void animateBarbarian() {
    iAnimateSprite(&barbarianSprite);
}

void spawnBarbarianIfNeeded() {
    if (numBarbarians >= MAX_BARBARIANS) return;

    if (barbarianSpawnTimer >= BARBARIAN_SPAWN_DELAY) {
        barbarians[numBarbarians].x = nextBarbarianX;
        barbarians[numBarbarians].y = 45;
        barbarians[numBarbarians].speed = 2;
        barbarians[numBarbarians].health = 2;
        barbarians[numBarbarians].active = true;

        numBarbarians++;
        barbarianSpawnTimer = 0;
        nextBarbarianX += 700 + rand() % 300;
    } else {
        barbarianSpawnTimer++;
    }
}

void loadHighScores() {
    FILE *file = fopen("saves/data/scores.txt", "r");
    if (file == NULL) return;

    numHighScores = 0;
    while (fscanf(file, "%99[^,],%d\n", highScores[numHighScores].name, 
                 &highScores[numHighScores].score) == 2) {
        numHighScores++;
        if (numHighScores >= MAX_HIGH_SCORES) break;
    }
    fclose(file);
}

void saveHighScores() {
    FILE *file = fopen("saves/data/scores.txt", "w");
    if (file == NULL) return;

    for (int i = 0; i < numHighScores; i++) {
        fprintf(file, "%s,%d\n", highScores[i].name, highScores[i].score);
    }
    fclose(file);
}

void insertScore(const char *name, int score) {
    for (int i = 0; i <= numHighScores; i++) {
        if (i == numHighScores || score > highScores[i].score) {
            for (int j = MAX_HIGH_SCORES - 1; j > i; j--) {
                highScores[j] = highScores[j - 1];
            }
            strncpy(highScores[i].name, name, sizeof(highScores[i].name) - 1);
            highScores[i].name[sizeof(highScores[i].name) - 1] = '\0';
            highScores[i].score = score;
            if (numHighScores < MAX_HIGH_SCORES) numHighScores++;
            break;
        }
    }
    saveHighScores();
}

void iPlayButton() {
    iSetTransparentColor(61, 174, 191, 0.1);
    iFilledCircle((screenWidth / 4) * 3, (screenHeight / 4), 44);

    iSetTransparentColor(255, 128, 31, 0.8);
    iFilledCircle((screenWidth / 4) * 3, (screenHeight / 4), 40);

    iSetColor(60, 60, 60);
    iText((screenWidth / 4) * 3 - 20, (screenHeight / 4) - 5, "▶Play", GLUT_BITMAP_HELVETICA_18);
}

void drawNavigationBar() {
    char *navItems[] = {"Start", "About", "Settings", "Help", "High Scores"};
    int navItemCount = 5;

    iShowLoadedImage(0, 0, &navBg);
    for (int i = 0; i < navItemCount; i++) {
        int x = screenWidth - buttonWidth - 50;
        int y = screenHeight - 110 - (i + 1) * buttonHeight - i * 60;

        iSetColor(180, 180, 180);
        iFilledRectangle(x, y, buttonWidth, buttonHeight);

        iSetColor(60, 60, 60);
        iText(x + buttonWidth / 2 - 20, y + buttonHeight / 2 - 5, navItems[i], GLUT_BITMAP_HELVETICA_18);
    }
}

bool mouseNavigationBar(int mx, int my) {
    int navItemCount = 5;
    int x = screenWidth - buttonWidth - 50;
    for (int i = 0; i < navItemCount; i++) {
        int y = screenHeight - 110 - (i + 1) * buttonHeight - i * 60;
        if (mx >= x && mx <= x + buttonWidth &&
            my >= y && my <= y + buttonHeight) {
            currentScreen = i + 1;
            if (i == 4) currentScreen = 6;
            return true;
        }
    }
    return false;
}

void drawBackground() {
    int bgOffset = worldX % bg.width;
    iShowLoadedImage(-bgOffset, 0, &bg);
    if (bgOffset > 0) {
        iShowLoadedImage(bg.width - bgOffset, 0, &bg);
    }
}

void drawSprite() {
    if (isthrowingstar) {
        iSetSpritePosition(&starthrow, playerX, playerY);
        iShowSprite(&starthrow);
    }
    else {
        iSetSpritePosition(&playerSprite, playerX, playerY);
        iShowSprite(&playerSprite);
    }
}

void animatePlayer() {
    if ((movingRight || movingLeft) && !isthrowingstar) {
        iAnimateSprite(&playerSprite);
    }
}

void animatestarthrow() {
    if (isthrowingstar) {
        iAnimateSprite(&starthrow);
        throwingFrame++;
        if (throwingFrame >= starthrowframecount) {
            isthrowingstar = false;
            throwingFrame = 0;
        }
    }
}

void playBGSong() {
    if (song) {
        if (!wasSongOn) {
            bgsongchannel = iPlaySound("saves/assets/sounds/start.wav", 1, songvolume);
            lastSongVolume = songvolume;
            wasSongOn = true;
        }
        else if (songvolume != lastSongVolume) {
            iStopSound(bgsongchannel);
            bgsongchannel = iPlaySound("saves/assets/sounds/start.wav", 1, songvolume);
            lastSongVolume = songvolume;
        }
    }
    else {
        if (wasSongOn) {
            iStopSound(bgsongchannel);
            bgsongchannel = -1;
            wasSongOn = false;
        }
    }
}

void drawstar() {
    if (isthrowingstar) {
        for (int i = 0; i < MAX_STARS; i++) {
            if (stars[i].active) {
                iShowLoadedImage(stars[i].x, stars[i].y, &throwingStar);
            }
        }
    }
}

void updatestar() {
    for (int i = 0; i < MAX_STARS; i++) {
        if (stars[i].active) {
            if (movingRight) {
                if (starthrowdirection) {
                    iMirrorImage(&throwingStar, HORIZONTAL);
                }
                stars[i].x += starSpeed;
                if (stars[i].x >= screenWidth || stars[i].x <= 10) {
                    stars[i].active = false;
                }
            }
            else if (movingLeft) {
                if (!starthrowdirection) {
                    iMirrorImage(&throwingStar, HORIZONTAL);
                }
                stars[i].x -= starSpeed;
                if (stars[i].x > screenWidth || stars[i].x <= 10) {
                    stars[i].active = false;
                }
            }
        }
    }
}

void start() {
    iClear();
    drawBackground();
    drawRoad();
    drawSprite();
    drawObstacles();
    drawstar();
    drawgolem();
    drawBarbarian();
    drawHealthBar();
    drawLeaves();
}

void updatePlayer() {
    if (playerY > 50 || verticalSpeed > 0) {
        if (movingRight) {
            if (worldX < worldWidth - screenWidth) {
                worldX += 5;
                if (worldX > worldWidth - screenWidth) worldX = worldWidth - screenWidth;
            }
            else {
                playerX += 5;
                if (playerX > screenWidth - playerWidth - 10) {
                    playerX = screenWidth - playerWidth - 10;
                }
            }
        }
        else if (movingLeft) {
            if (worldX > 0) {
                worldX -= 5;
                if (worldX < 0) worldX = 0;
            }
            else {
                playerX -= 5;
                if (playerX < 10) {
                    playerX = 10;
                }
            }
        }
        playerY += verticalSpeed;
        verticalSpeed += gravity;
    }

    if (!isJumping && !hitCooldown) {
        for (int i = 0; i < NUM_OBSTACLES; i++) {
            int drawX = obstacleX[i] - worldX;
            int obsWidth = 80, obsHeight = 80;

            if (playerX < drawX + obsWidth &&
                playerX + playerWidth > drawX &&
                playerY < obstacleY[i] + obsHeight &&
                playerY + playerHeight > obstacleY[i]) {
                if (sound) {
                    iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
                }

                playerHealth--;
                hitCooldown = true;

                if (playerHealth <= 0) {
                    playerHealth = 0;
                    currentScreen = 5;
                }
                break;
            }
        }
    }

    if (playerY <= 50) {
        playerY = 50;
        isJumping = false;
        verticalSpeed = 0;
    }
}

void soundVolume() {
    iSetColor(255, 255, 255);
    iFilledRectangle(368, soundknoby, soundknobx - 368 + 7, 14);
    iShowLoadedImage(soundknobx, soundknoby, &knob);
}

void songVolume() {
    iSetColor(255, 255, 255);
    iFilledRectangle(368, songknoby, songknobx - 368 + 7, 14);
    iShowLoadedImage(songknobx, songknoby, &knob);
}

void settings() {
    iClear();
    iShowLoadedImage(0, 0, &settingsmenu);

    if (sound) {
        iShowLoadedImage(269, 299, &soundon);
        soundVolume();
    }
    else {
        iShowLoadedImage(269, 299, &soundoff);
    }

    if (song) {
        iShowLoadedImage(277, 217, &songon);
        songVolume();
    }
    else {
        iShowLoadedImage(277, 217, &songoff);
    }
}

void mouseSettings(int mx, int my) {
    if (mx >= 269 && mx <= 349 && my >= 299 && my <= 379) {
        sound = !sound;
    }

    if (mx >= 277 && mx <= 342 && my >= 217 && my <= 282) {
        song = !song;
    }
}

void about() {
    iClear();
    iSetColor(255, 255, 255);
    iText(screenWidth / 2 - 100, screenHeight / 2, "About Screen", GLUT_BITMAP_HELVETICA_18);
}

void help() {
    iClear();
    iSetColor(255, 255, 255);
    iText(screenWidth / 2 - 100, screenHeight / 2, "Help Screen", GLUT_BITMAP_HELVETICA_18);
}

void resetGame() {
    if (bgsongchannel != -1) {
        iStopSound(bgsongchannel);
        bgsongchannel = -1;
    }

    playerHealth = 3;
    playerScore = 0;
    playerX = 500;
    playerY = 50;
    worldX = 0;
    isJumping = false;
    verticalSpeed = 0;
    hitCooldown = false;
    hitCooldownTimer = 0;
    endSoundPlayed = false;
    currentScreen = 1;
    numGolems = 0;
    golemSpawnTimer = 0;
    nextGolemX = screenWidth + 300;
    numBarbarians = 0;
    barbarianSpawnTimer = 0;
    nextBarbarianX = nextGolemX + 500;

    for (int i = 0; i < MAX_GOLEMS; i++) {
        golems[i].active = false;
    }
    for (int i = 0; i < MAX_BARBARIANS; i++) {
        barbarians[i].active = false;
    }
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].active = false;
    }
}

void cleanupGame() {
    if (bgsongchannel != -1) {
        iStopSound(bgsongchannel);
    }
    // Add any additional cleanup needed
}

void iDraw() {
    iClear();
    if (enteringName) {
        iShowLoadedImage(0, 0, &nameEntryImage);
        iSetColor(255, 255, 255);
        iText(300, 350, "Enter Your Name:", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(500, 350, playerName, GLUT_BITMAP_HELVETICA_18);
        return;
    }

    if (!isPlayButtonClicked) {
        iShowLoadedImage(0, 0, &bg1);
        iPlayButton();
    }
    else if (isPlayButtonClicked) {
        switch (currentScreen) {
            case 0: drawNavigationBar(); break;
            case 1: start(); break;
            case 2: about(); break;
            case 3: settings(); break;
            case 4: help(); break;
            case 5: 
                if (!endSoundPlayed) {
                    insertScore(playerName, playerScore);
                    iPlaySound("saves/assets/sounds/gameover.wav", 0, soundvolume);
                    endSoundPlayed = true;
                    loadHighScores();
                }
                iShowLoadedImage(0, 0, &endImage);
                char scoreText[100], nameText[100];
                snprintf(scoreText, sizeof(scoreText), "Your Score: %d", playerScore);
                snprintf(nameText, sizeof(nameText), "Player Name: %s", playerName);
                iSetColor(200, 0, 0);
                iText(50, 550, scoreText, GLUT_BITMAP_HELVETICA_18);
                iText(50, 520, nameText, GLUT_BITMAP_HELVETICA_18);
                iText(50, 480, "Press 'R' to Restart", GLUT_BITMAP_HELVETICA_18);
                break;
            case 6:
                iShowLoadedImage(0, 0, &highScoreBg);
                iSetColor(255, 255, 255);
                iText(400, 550, "Top 10 High Scores", GLUT_BITMAP_HELVETICA_18);
                for (int i = 0; i < numHighScores; i++) {
                    char entry[150];
                    snprintf(entry, sizeof(entry), "%d. %s - %d", i + 1, highScores[i].name, highScores[i].score);
                    iText(350, 500 - i * 30, entry, GLUT_BITMAP_HELVETICA_18);
                }
                break;
        }
    }
}

void iMouseMove(int mx, int my) {}

void iMouseDrag(int mx, int my) {
    if (currentScreen == 3) {
        if (mx >= soundknobx && mx <= soundknobx + 14 && my >= soundknoby && my <= soundknoby + 14) {
            soundknobclicked = true;
            songknobclicked = false;
            soundknobx = mx - 7;
            if (soundknobx < 368) soundknobx = 368;
            if (soundknobx > 639) soundknobx = 639;
            soundvolume = (soundknobx - 368 + 7) * 50 / (639 - 368);
        }
        else if (mx >= songknobx && mx <= songknobx + 14 && my >= songknoby && my <= songknoby + 14) {
            songknobclicked = true;
            soundknobclicked = false;
            songknobx = mx - 7;
            if (songknobx < 368) songknobx = 368;
            if (songknobx > 639) songknobx = 639;
            songvolume = (songknobx - 368 + 7) * 50 / (639 - 368);
        }
        else {
            soundknobclicked = false;
            songknobclicked = false;
        }
    }
}

void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!isPlayButtonClicked) {
            int centerX = (screenWidth / 4) * 3;
            int centerY = (screenHeight / 4);
            int radius = 40;
            int dx = mx - centerX;
            int dy = my - centerY;
            if (dx * dx + dy * dy <= radius * radius) {
                isPlayButtonClicked = true;
                enteringName = true;
                nameCharIndex = 0;
                playerName[0] = '\0';
                if (sound) {
                    iStopSound(-1);
                    iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
                }
            }
        }
        else if (isPlayButtonClicked && currentScreen == 0) {
            if (mouseNavigationBar(mx, my)) {
                if (sound) {
                    iStopSound(-1);
                    iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
                }
            }
        }
        else if (currentScreen == 3) {
            mouseSettings(mx, my);
        }
    }
}

void iMouseWheel(int dir, int mx, int my) {}

void iKeyboard(unsigned char key) {
    if (enteringName) {
        if (key == '\r') {
            enteringName = false;
            return;
        }
        else if (key == '\b') {
            if (nameCharIndex > 0) {
                nameCharIndex--;
                playerName[nameCharIndex] = '\0';
            }
        }
        else if (nameCharIndex < 99) {
            playerName[nameCharIndex++] = key;
            playerName[nameCharIndex] = '\0';
        }
        return;
    }

    if (key == 'b' || key == 'B') {
        currentScreen = 0;
        endSoundPlayed = false;
        return;
    }

    if ((currentScreen == 5) && (key == 'r' || key == 'R')) {
        resetGame();
        currentScreen = 1;
        return;
    }

    switch (key) {
        case ' ':
            isthrowingstar = true;
            for (int i = 0; i < MAX_STARS; i++) {
                if (!stars[i].active) {
                    stars[i].active = true;
                    stars[i].x = playerX + playerWidth;
                    stars[i].y = playerY + playerHeight / 2 - 10;
                    break;
                }
            }
            break;
        case 'q':
            cleanupGame();
            exit(0);
            break;
    }
}

void iSpecialKeyboard(unsigned char key) {
    switch (key) {
        case GLUT_KEY_HOME:
            currentScreen = 0;
            break;
        case GLUT_KEY_LEFT:
            starthrowdirection = false;
            if (playerX > 10) {
                if (movingRight) {
                    iMirrorSprite(&playerSprite, HORIZONTAL);
                    iMirrorSprite(&starthrow, HORIZONTAL);
                }
                movingLeft = true;
                movingRight = false;
                moveTimer = 0;
                moving = true;

                if (!isJumping) {
                    if (worldX > 0) {
                        worldX -= 5;
                        if (worldX < 0) worldX = 0;
                    }
                    else {
                        playerX -= 5;
                        if (playerX < 10) playerX = 10;
                    }
                }
            }
            break;
        case GLUT_KEY_RIGHT:
            starthrowdirection = true;
            if (playerX < screenWidth - playerWidth - 10) {
                if (movingLeft) {
                    iMirrorSprite(&playerSprite, HORIZONTAL);
                    iMirrorSprite(&starthrow, HORIZONTAL);
                }

                movingRight = true;
                movingLeft = false;
                moveTimer = 0;
                moving = true;

                if (!isJumping) {
                    if (worldX < worldWidth - screenWidth) {
                        worldX += 5;
                        if (worldX > worldWidth - screenWidth) worldX = worldWidth - screenWidth;
                    }
                    else {
                        playerX += 5;
                        if (playerX > screenWidth - playerWidth - 10) {
                            playerX = screenWidth - playerWidth - 10;
                        }
                    }
                }
            }
            break;
        case GLUT_KEY_UP:
            if (!isJumping) {
                isJumping = true;
                verticalSpeed = jumpSpeed;
            }
            break;
        case GLUT_KEY_DOWN:
            break;
    }
}

void SpecialKeyboardUP() {
    moveTimer++;
    if (moveTimer >= MOVE_INTERVAL) {
        moving = false;
    }
}

void iTimer() {
    if (hitCooldown) {
        hitCooldownTimer++;
        if (hitCooldownTimer >= HIT_COOLDOWN_LIMIT) {
            hitCooldown = false;
            hitCooldownTimer = 0;
        }
    }

    if (currentScreen == 1 && playerHealth > 0) {
        playerScore++;
    }

    updateLeaves();
    updatePlayer();
    updatestar();
    updateGolems();
    updateBarbarians();
    spawnGolemIfNeeded();
    spawnBarbarianIfNeeded();
    checkStarGolemCollision();
    checkStarBarbarianCollision();
    checkGolemCollision();
    checkBarbarianCollision();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    
    // Initialize game elements
    load_Images();
    generateObstacles();
    loadHighScores();

    // Initialize leaves
    for (int i = 0; i < MAX_LEAVES; i++) {
        leaves[i].x = rand() % screenWidth;
        leaves[i].y = rand() % screenHeight + screenHeight;
        leaves[i].speed = rand() % 3 + 1;
        leaves[i].active = true;
    }

    // Set up timers
    iSetTimer(20, iTimer);  // Main game timer first
    iSetTimer(100, updateGolems);
    iSetTimer(100, updateBarbarians);
    iSetTimer(120, animategolem);
    iSetTimer(120, spawnGolemIfNeeded);
    iSetTimer(120, animateBarbarian);
    iSetTimer(120, spawnBarbarianIfNeeded);
    iSetTimer(120, animatePlayer);
    iSetTimer(60, animatestarthrow);

    // Initialize sound and graphics
    iInitializeSound();
    playBGSong();
    iInitialize(screenWidth, screenHeight, "Maze of Shadows");
    
    // Set cleanup function
    atexit(cleanupGame);
    
    return 0;
}