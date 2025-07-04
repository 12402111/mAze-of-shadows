#include <math.h>
#include <stdlib.h>
#include "iGraphics.h"
#include "iSound.h"
Image fire, fire1, log1, tree, tree1_png;
#define NUM_OBSTACLES 30 // Adjust based on worldWidth
int playerScore = 0;

int obstacleX[NUM_OBSTACLES];
int obstacleY[NUM_OBSTACLES];
int collisionSoundChannel = -1;
Image endImage;
bool endSoundPlayed = false;
// check

int playerHealth = 3;
bool hitCooldown = false;
int hitCooldownTimer = 0;
const int HIT_COOLDOWN_LIMIT = 30; // frames to wait before next hit allowed

int screenWidth = 1000, screenHeight = 600;
Image bg1;
#define MAX_LEAVES 50

typedef struct
{
    int x, y;
    int speed;
    bool active;
} Leaf;

Leaf leaves[MAX_LEAVES];
Image leafImg;

int navY = 490;
int buttonWidth = 150;
int buttonHeight = 50;
int currentScreen = 0;

int playerX = 500, playerY = 50, playerWidth = 55, playerHeight = 100;
int worldScreenCount = 3;
int worldWidth = worldScreenCount * screenWidth, worldX = 0;

Image bg;
Image runRight[18];
Sprite playerSprite;
bool moving = false;
bool movingRight = false;
bool movingLeft = false;
int runFrameCount = 8;
int moveTimer = 0;
const int MOVE_INTERVAL = 10;

#define MAX_STARS 100
typedef struct
{
    int x, y;
    bool active;
} Star;

Star stars[MAX_STARS];
Image throwingStar;
int starSpeed = 15;
Image starthrowing[6];
int starthrowframecount = 5;
Sprite starthrow;
bool isthrowingstar = false;
bool starthrowdirection = false;
int throwingFrame = 0;

typedef struct
{ // Golem done by Imtiaz.
    int x, y;
    int speed;
    bool active = true;
    int health;
} Golem;
Sprite golemSprite;
Image golemImage[12];
bool golemRight = true;
bool golemLeft = false;
#define GOLEM_SPAWN_INTERVAL 850
#define MAX_GOLEMS 20
Golem golems[MAX_GOLEMS];
int numGolems = 0;



Image settingsmenu, soundon, soundoff, songon, songoff;
Image knob;
bool soundknobclicked = false, songknobclicked = false;
int soundknobx = 643;
int soundknoby = 333;
int songknobx = 643;
int songknoby = 245;
int soundvolume = 50, songvolume = 50;
int bgsongchannel = -1;
int lastSongVolume = -1;
bool wasSongOn = false;

bool sound = true, song = true;

bool isJumping = false;
int jumpSpeed = 20, gravity = -2, verticalSpeed = 0;

char *navItems[] = {"Start", "About", "Settings", "Help"};

bool isPlayButtonClicked = false;
void drawRoad()
{
    int roadHeight = 50;
    iSetColor(60, 60, 60); // dark gray road color
    iFilledRectangle(0, 0, screenWidth, roadHeight);
}

void load_Images()
{
    iLoadImage(&leafImg, "saves/assets/images/leaf.png");
    iResizeImage(&leafImg, 20, 20);

    iLoadImage(&fire1, "saves/assets/images/obstacles/fire1.png");
    iResizeImage(&fire1, 80, 80);

    iLoadImage(&log1, "saves/assets/images/obstacles/log1.png");
    iResizeImage(&log1, 80, 80);

    iLoadImage(&tree1_png, "saves/assets/images/obstacles/tree1.png");
    iResizeImage(&tree1_png, 80, 80);

    iLoadImage(&bg1, "saves/assets/images/start.png");
    iResizeImage(&bg1, 1000, 600);

    iLoadImage(&throwingStar, "saves/assets/images/starthrowing.png");
    iResizeImage(&throwingStar, 20, 20);

    iLoadImage(&settingsmenu, "saves/assets/images/SoundOn.png");
    iResizeImage(&settingsmenu, 1000, 600);

    iLoadImage(&soundon, "saves/assets/images/SoundOn.png");
    iResizeImage(&soundon, 80, 80);

    iLoadImage(&soundoff, "saves/assets/images/SoundOff.png");
    iResizeImage(&soundoff, 80, 80);

    iLoadImage(&songon, "saves/assets/images/SongOff.png");
    iResizeImage(&songon, 65, 65);

    iLoadImage(&songoff, "saves/assets/images/SongOff.png");
    iResizeImage(&songoff, 65, 65);

    iLoadImage(&knob, "saves/assets/images/starthrowing.png");
    iResizeImage(&knob, 14, 14);
    iLoadImage(&endImage, "saves/assets/images/end.png");
    iResizeImage(&endImage, 1000, 600);

    iLoadImage(&bg, "saves/assets/images/backimg2.png");
    iResizeImage(&bg, 1000, 600);

    iLoadFramesFromFolder(runRight, "saves/assets/images/sprites/ninjas");
    iInitSprite(&playerSprite, -1);
    iChangeSpriteFrames(&playerSprite, runRight, runFrameCount);
    iResizeSprite(&playerSprite, playerWidth, playerHeight);
    iSetSpritePosition(&playerSprite, playerX, playerY);

    iLoadFramesFromFolder(starthrowing, "saves/assets/images/sprites/starthrowing");
    iInitSprite(&starthrow, -1);
    iChangeSpriteFrames(&starthrow, starthrowing, starthrowframecount);
    iResizeSprite(&starthrow, playerWidth, playerHeight);
    iSetSpritePosition(&starthrow, playerX, playerY);

    iLoadFramesFromFolder(golemImage, "saves/assets/images/sprites/Golem_2/RunThrowing");
    iInitSprite(&golemSprite, -1);
    iChangeSpriteFrames(&golemSprite, golemImage, 12);
    iResizeSprite(&golemSprite, playerWidth, playerHeight);
    // iSetSpritePosition(&golemSprite, golem, 30);
}

void generateObstacles()
{
    int spacing = 400; // space between each obstacle

    for (int i = 0; i < NUM_OBSTACLES; ++i)
    {
        obstacleX[i] = 300 + i * spacing; // distributed over the world
        obstacleY[i] = 30;                // ground height
    }
}

void drawObstacles()
{
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        int drawX = obstacleX[i] - worldX;

        if (drawX + 80 < 0 || drawX > screenWidth)
            continue;

        int type = i % 3;
        if (type == 0)
            iShowLoadedImage(drawX, obstacleY[i], &log1);
        else if (type == 1)
            iShowLoadedImage(drawX, obstacleY[i], &tree1_png);
        else
            iShowLoadedImage(drawX, obstacleY[i], &fire1);
    }
}
void drawHealthBar()
{
    int barWidth = 150, barHeight = 20;
    int barX = 20, barY = screenHeight - 40;

    iSetColor(255, 0, 0);
    iFilledRectangle(barX, barY, barWidth, barHeight);

    iSetColor(0, 255, 0);
    iFilledRectangle(barX, barY, barWidth * playerHealth / 3, barHeight);

    iSetColor(255, 255, 255);
    iRectangle(barX, barY, barWidth, barHeight);
    char scoreText[20];
    sprintf(scoreText, "Score: %d", playerScore);
    iSetColor(255, 255, 0); // Yellow
    iText(barX, barY - 25, scoreText, GLUT_BITMAP_HELVETICA_18);
}
void updateLeaves()
{
    for (int i = 0; i < MAX_LEAVES; i++)
    {
        if (leaves[i].active)
        {
            leaves[i].y -= leaves[i].speed;

            if (leaves[i].y < 0)
            {
                leaves[i].x = rand() % screenWidth;
                leaves[i].y = screenHeight + (rand() % 200);
                leaves[i].speed = rand() % 3 + 1;
            }
        }
    }
}
void drawLeaves()
{
    for (int i = 0; i < MAX_LEAVES; i++)
    {
        if (leaves[i].active)
        {
            iShowLoadedImage(leaves[i].x, leaves[i].y, &leafImg);
        }
    }
}

void generateGolems()
{
    numGolems = 0;
    for (int i = GOLEM_SPAWN_INTERVAL * 2; i < worldWidth - 200; i += GOLEM_SPAWN_INTERVAL)
    {
        if (numGolems >= MAX_GOLEMS)
            break;

        golems[numGolems].x = i;
        golems[numGolems].y = 45;
        golems[numGolems].speed = 2;
        golems[numGolems].health = 1;
        golems[numGolems].active = true;

        numGolems++;
    }
}

void drawgolem()
{
    for (int i = 0; i < numGolems; i++)
    {
        if (!golems[i].active)
            continue;

        int gx = golems[i].x - worldX;

        if (gx >= -100 && gx <= screenWidth + 100)
        {
            golems[i].y = 50;
            iSetSpritePosition(&golemSprite, gx, golems[i].y);
            iShowSprite(&golemSprite);
        }
    }
}

void updateGolems()
{
    for (int i = 0; i < numGolems; i++)
    {
        if (!golems[i].active)
            continue;

        int gx = golems[i].x;
        int px = playerX + worldX; // playerX is in only screenwidth.(0<=playerx<=screenwidth).So we added worldX.

        if (gx > px && gx - px <= 250)
        {
            if (golemRight)
            {
                golemLeft = true;
                golemRight = false;
                iMirrorSprite(&golemSprite, HORIZONTAL);
            }
            golems[i].x -= golems[i].speed;
        }
        else if (gx < px && px - gx <= 250)
        {
            if (golemLeft)
            {
                golemRight = true;
                golemLeft = false;
                iMirrorSprite(&golemSprite, HORIZONTAL);
            }
            golems[i].x += golems[i].speed;
        }
    }
}

void checkStarGolemCollision()
{
    for (int i = 0; i < MAX_STARS; i++)
    {
        if (!stars[i].active)
            continue;

        for (int j = 0; j < 5; j++)
        {
            if (!golems[j].active)
                continue;

            int gx = golems[j].x - worldX;
            int gy = golems[j].y;
            int gWidth = playerWidth;
            int gHeight = playerHeight;

            if (stars[i].x < gx + gWidth && stars[i].x + 20 > gx && stars[i].y < gy + gHeight && stars[i].y + 20 > gy)
            {

                golems[j].health--;
                stars[i].active = false;

                if (golems[j].health <= 0)
                {
                    golems[j].active = false;
                    playerScore += 100; // Golem marle bonus....
                }
            }
        }
    }
}
void checkGolemCollision()
{
    if (hitCooldown)
        return;

    for (int i = 0; i < numGolems; i++)
    {
        if (!golems[i].active)
            continue;

        int gx = golems[i].x - worldX;
        int gy = golems[i].y;
        int gWidth = playerWidth;
        int gHeight = playerHeight;

        if (playerX - 35 <= gx + gWidth && playerX + playerWidth - 35 >= gx && playerY <= gy + gHeight && playerY + playerHeight >= gy)
        {
            if (sound)
            {
                iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
            }

            playerHealth--;
            hitCooldown = true;

            if (playerHealth <= 0)
            {
                playerHealth = 0;
                currentScreen = 5;
            }

            break;
        }
    }
}

void animategolem()
{
    iAnimateSprite(&golemSprite);
}

void iPlayButton()
{

    iSetTransparentColor(61, 174, 191, 0.1);
    iFilledCircle((screenWidth / 4) * 3, (screenHeight / 4), 44);

    iSetTransparentColor(255, 128, 31, 0.8);
    iFilledCircle((screenWidth / 4) * 3, (screenHeight / 4), 40);

    iSetColor(60, 60, 60);
    iText((screenWidth / 4) * 3 - 20, (screenHeight / 4) - 5, "▶Play", GLUT_BITMAP_HELVETICA_18);
}

void drawNavigationBar()
{
    iClear();
    iShowImage(0, 0, "saves/assets/images/ninja.png");

    for (int i = 0; i < 4; i++)
    {
        int x = screenWidth - buttonWidth - 50;
        int y = screenHeight - 110 - (i + 1) * buttonHeight - i * 60;

        iSetTransparentColor(255, 255, 255, 0.3);
        iFilledRectangle(x, y, buttonWidth, buttonHeight);

        iSetColor(252, 106, 43);
        iText(x + buttonWidth / 2 - 20, y + buttonHeight / 2 - 5, navItems[i], GLUT_BITMAP_HELVETICA_18);
    }
}
void mouseNavigationBar(int mx, int my)
{
    int x = screenWidth - buttonWidth - 50;
    if (mx >= x && mx <= x + buttonWidth)
    {
        if (my >= 440 && my <= 440 + buttonHeight)
        {
            currentScreen = 1;
        }
        else if (my >= 330 && my <= 330 + buttonHeight)
        {
            currentScreen = 2;
        }
        else if (my >= 220 && my <= 220 + buttonHeight)
        {
            currentScreen = 3;
        }
        else if (my >= 110 && my <= 110 + buttonHeight)
        {
            currentScreen = 4;
        }
    }
}

void drawBackground()
{
    iShowLoadedImage(0, 0, &bg);
}

void drawSprite()
{
    if (isthrowingstar)
    {
        iSetSpritePosition(&starthrow, playerX, playerY);
        iShowSprite(&starthrow);
    }
    else if (!isthrowingstar)
    {
        iSetSpritePosition(&playerSprite, playerX, playerY);
        iShowSprite(&playerSprite);
    }
}

void animatePlayer()
{
    if ((movingRight || movingLeft) && !isthrowingstar && moving)
    {
        iAnimateSprite(&playerSprite);
    }
}
/*void animatePlayer()
{
    if ((movingRight || movingLeft) && !isthrowingstar)
    {
        iAnimateSprite(&playerSprite);
    }
}*/

void animatestarthrow()
{
    if (isthrowingstar)
    {
        iAnimateSprite(&starthrow);
        throwingFrame++;

        if (throwingFrame >= starthrowframecount)
        {
            isthrowingstar = false;
            throwingFrame = 0;
        }
    }
}

void playBGSong()
{
    if (song)
    {
        if (!wasSongOn)
        {
            bgsongchannel = iPlaySound("saves/assets/sounds/start.wav", 1, songvolume);
            lastSongVolume = songvolume;
            wasSongOn = true;
        }
        else if (songvolume != lastSongVolume)
        {
            iStopSound(bgsongchannel);
            bgsongchannel = iPlaySound("saves/assets/sounds/start.wav", 1, songvolume);
            lastSongVolume = songvolume;
        }
    }
    else
    {
        if (wasSongOn)
        {
            iStopSound(bgsongchannel);
            bgsongchannel = -1;
            wasSongOn = false;
        }
    }
}

void drawstar()
{
    if (isthrowingstar)
    {
        for (int i = 0; i < MAX_STARS; i++)
        {
            if (stars[i].active)
            {

                iShowLoadedImage(stars[i].x, stars[i].y, &throwingStar);
            }
        }
    }
}
void updatestar()
{
    for (int i = 0; i < MAX_STARS; i++)
    {
        if (stars[i].active)
        {
            if (movingRight)
            {
                if (starthrowdirection)
                {
                    iMirrorImage(&throwingStar, HORIZONTAL);
                }
                stars[i].x += starSpeed;
                if (stars[i].x >= screenWidth || stars[i].x <= 10)
                {
                    stars[i].active = false;
                }
            }
            else if (movingLeft)
            {
                if (!starthrowdirection)
                {
                    iMirrorImage(&throwingStar, HORIZONTAL);
                }
                stars[i].x -= starSpeed;
                if (stars[i].x > screenWidth || stars[i].x <= 10)
                {
                    stars[i].active = false;
                }
            }
        }
    }
}

void start()
{
    iClear();
    drawBackground();
    // drawRoad();
    drawSprite();
    drawObstacles();
    drawgolem();
    drawstar();
    drawHealthBar();
    drawLeaves();
}

void updatePlayer()
{
    //<This part for falling><and this part for jumping>
    if (playerY > 50 || verticalSpeed > 0)
    {
        if (movingRight)
        {
            if (worldX < worldWidth - 500 - 10)
            {
                worldX += 8;
            }
            {
                worldX += 8;
            }
            if (worldX >= 0 && worldX <= worldWidth - 1000)
            {
                iWrapImage(&bg, -8);
            }
            else if ((worldX <= 0 && worldX > -500) || (worldX >= worldWidth - 1000 && worldX < worldWidth))
            {
                playerX += 8;
            }
        }
        else if (movingLeft)
        {
            if (worldX > -490)
            {
                worldX -= 8;
            }
            if (worldX >= 0 && worldX <= worldWidth - 1000)
            {
                iWrapImage(&bg, 8);
            }
            else if (worldX <= 0 || worldX >= worldWidth - 1000)
            {
                playerX -= 8;
            }
        }
        playerY += verticalSpeed;
        verticalSpeed += gravity;
    }
    // Check for collision only when not jumping
    if (!isJumping && !hitCooldown)
    {
        for (int i = 0; i < NUM_OBSTACLES; i++)
        {
            int drawX = obstacleX[i] - worldX;
            int obsWidth = 60, obsHeight = 80;

            if (playerX < drawX + obsWidth &&
                playerX + playerWidth > drawX &&
                playerY < obstacleY[i] + obsHeight &&
                playerY + playerHeight > obstacleY[i])
            {
                if (sound)
                {
                    iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
                }

                playerHealth--;
                hitCooldown = true;

                if (playerHealth <= 0)
                {
                    // Trigger Game Over logic here if needed
                    playerHealth = 0;
                    currentScreen = 5; // maybe game over screen?
                }

                break;
            }
        }
    }

    if (playerY <= 50)
    {
        playerY = 50;
        isJumping = false;
        verticalSpeed = 0;
    }
}
void soundVolume()
{

    iSetColor(255, 255, 255);
    iFilledRectangle(368, soundknoby, soundknobx - 368 + 7, 14);
    iShowLoadedImage(soundknobx, soundknoby, &knob);
}
void songVolume()
{
    iSetColor(255, 255, 255);
    iFilledRectangle(368, songknoby, songknobx - 368 + 7, 14);
    iShowLoadedImage(songknobx, songknoby, &knob);
}
void settings()
{
    iClear();
    iShowLoadedImage(0, 0, &settingsmenu);

    if (sound)
    {
        iShowLoadedImage(269, 299, &soundon);
        soundVolume();
    }
    else
    {
        iShowLoadedImage(269, 299, &soundoff);
    }

    if (song)
    {
        iShowLoadedImage(277, 217, &songon);
        songVolume();
    }
    else
    {
        iShowLoadedImage(277, 217, &songoff);
    }
}

void mouseSettings(int mx, int my)
{
    if (mx >= 269 && mx <= 349 && my >= 299 && my <= 379 && sound)
    {
        sound = false;
    }
    else if (mx >= 269 && mx <= 349 && my >= 299 && my <= 379 && !sound)
    {
        sound = true;
    }

    if (mx >= 277 && mx <= 342 && my >= 217 && my <= 282 && song)
    {
        song = false;
    }
    else if (mx >= 277 && mx <= 342 && my >= 217 && my <= 282 && !song)
    {
        song = true;
    }
}

void about()
{
    iClear();
    iSetColor(255, 255, 255);
    iText(screenWidth / 2 - 100, screenHeight / 2, "About Screen", GLUT_BITMAP_HELVETICA_18);
}

void help()
{
    iClear();
    iSetColor(255, 255, 255);
    iText(screenWidth / 2 - 100, screenHeight / 2, "Help Screen", GLUT_BITMAP_HELVETICA_18);
}

void iDraw()
{
    iClear();

    if (!isPlayButtonClicked)
    {
        iShowLoadedImage(0, 0, &bg1);
        iPlayButton();
    }
    else if (isPlayButtonClicked)
    {
        if (currentScreen == 0)
        {
            drawNavigationBar();
        }
        else if (currentScreen == 1)
        {
            start();
        }
        else if (currentScreen == 2)
        {
            about();
        }
        else if (currentScreen == 3)
        {
            settings();
        }
        else if (currentScreen == 4)
        {
            help();
        }
        else if (currentScreen == 5)
        {
            iShowLoadedImage(0, 0, &endImage);

            char scoreText[100];
            sprintf(scoreText, "Your Score: %d", playerScore);   // 'score' must be your score variable
            iSetColor(255, 255, 255);                            // White text
            iText(50, 550, scoreText, GLUT_BITMAP_HELVETICA_18); // adjust position/font as you like

            if (!endSoundPlayed)
            {
                iPlaySound("saves/assets/sounds/gameover.wav", 0, soundvolume); // play once
                endSoundPlayed = true;
            }
        }
    }

    if (!movingRight && !movingLeft)
    {
        moveTimer = 0;
    }
}

/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
}

/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    if (currentScreen == 3)
    {
        if (mx >= soundknobx && mx <= soundknobx + 14 && my >= soundknoby && my <= soundknoby + 14)
        {
            soundknobclicked = true;
            songknobclicked = false;
            soundknobx = mx - 7;
            if (soundknobx < 368)
                soundknobx = 368;
            if (soundknobx > 639)
                soundknobx = 639;
            soundvolume = (soundknobx - 368 + 7) * 50 / (639 - 368);
        }
        else if (mx >= songknobx && mx <= songknobx + 14 && my >= songknoby && my <= songknoby + 14)
        {
            songknobclicked = true;
            soundknobclicked = false;
            songknobx = mx - 7;
            if (songknobx < 368)
                songknobx = 368;
            if (songknobx > 639)
                songknobx = 639;
            songvolume = (songknobx - 368 + 7) * 50 / (639 - 368);
        }
        else
        {
            soundknobclicked = false;
            songknobclicked = false;
        }
    }
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (sound)
        {
            iStopSound(-1);
            iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
        }
        if (!isPlayButtonClicked)
        {
            if (mx >= ((screenWidth / 4) * 3 - 40) && (mx <= ((screenWidth / 4) * 3 + 40)) && (my >= ((screenHeight / 4) - sqrt(1600 - (mx - ((screenWidth / 4) * 3)) * (mx - ((screenWidth / 4) * 3))))) && (my <= ((screenHeight / 4) + sqrt(1600 - (mx - ((screenWidth / 4) * 3)) * (mx - ((screenWidth / 4) * 3))))))
            {
                isPlayButtonClicked = true;
            }
        }
        else if (isPlayButtonClicked && currentScreen == 0)
        {
            mouseNavigationBar(mx, my);
        }
        else if (currentScreen == 3)
        {
            mouseSettings(mx, my);
        }
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        {
            // place your codes here
        }
    }
}
/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    switch (key)
    {
    case ' ':
        isthrowingstar = true;
        for (int i = 0; i < MAX_STARS; i++)
        {
            if (!stars[i].active)
            {
                stars[i].active = true;
                stars[i].x = playerX + playerWidth;
                stars[i].y = playerY + playerHeight / 2 - 10;
                break; // Add only one star per press
            }
        }
        break;
    case 'q':
        // do something with 'q'
        break;
    // place your codes for other keys here
    default:
        break;
    }
}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_HOME:
        currentScreen = 0; // do something
        break;
    // place your codes for other keys here
    default:
        break;
    }
    if (key == GLUT_KEY_LEFT)
    {
        starthrowdirection = false;
        if (playerX > 10)
        {
            if (movingRight)
            {
                iMirrorSprite(&playerSprite, HORIZONTAL);
                iMirrorSprite(&starthrow, HORIZONTAL);
            }
            movingLeft = true;
            movingRight = false;
            moveTimer = 0;
            moving = true;

            if (!isJumping)
            {
                if (worldX > -490)
                {
                    worldX -= 5;
                }
                if (worldX >= 0 && worldX <= worldWidth - 1000)
                {
                    iWrapImage(&bg, 5);
                }
                else if (worldX <= 0 || worldX >= worldWidth - 1000)
                {
                    playerX -= 5;
                }
            }
        }
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        starthrowdirection = true;
        if (playerX < screenWidth - playerWidth - 10)
        {
            if (movingLeft)
            {
                iMirrorSprite(&playerSprite, HORIZONTAL);
                iMirrorSprite(&starthrow, HORIZONTAL);
            }

            movingRight = true;
            movingLeft = false;
            moveTimer = 0;
            moving = true;

            if (!isJumping)
            {
                if (worldX < 2990)
                {
                    worldX += 5;
                }
                if (worldX >= 0 && worldX <= worldWidth - 1000)
                {
                    iWrapImage(&bg, -5);
                }
                else if (worldX <= 0 || worldX >= worldWidth - 1000)
                {
                    playerX += 5;
                }
            }
        }
    }
    if (key == GLUT_KEY_UP)
    {
        if (!isJumping)
        {
            isJumping = true;
            verticalSpeed = jumpSpeed;
        }
    }
    else if (key == GLUT_KEY_DOWN)
    {
        // do something for down key
    }
}
void SpecialKeyboardUP()
{
    moveTimer++;
    if (moveTimer >= MOVE_INTERVAL)
    {
        moving = false;
    }
}

void iTimer()
{
    if (hitCooldown)
    {
        hitCooldownTimer++;
        if (hitCooldownTimer >= HIT_COOLDOWN_LIMIT)
        {
            hitCooldown = false;
            hitCooldownTimer = 0;
        }
    }
    if (currentScreen == 1 && playerHealth > 0) // only increase in game screen
    {
        playerScore++;
    }

    updateLeaves();

    updatePlayer();
    updatestar();
    // updateGolems();
    checkStarGolemCollision();
    checkGolemCollision();
    playBGSong();
    SpecialKeyboardUP();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // place your own initialization codes here.
    load_Images();
    generateObstacles();
    generateGolems();

    for (int i = 0; i < MAX_LEAVES; i++)
    {
        leaves[i].x = rand() % screenWidth;
        leaves[i].y = rand() % screenHeight + screenHeight;
        leaves[i].speed = rand() % 3 + 1;
        leaves[i].active = true;
    }

    iSetTimer(100, updateGolems);
    iSetTimer(120, animategolem);
    iSetTimer(120, animatePlayer);
    iSetTimer(60, animatestarthrow);
    iSetTimer(20, iTimer);
    iInitializeSound();
    playBGSong();
    iInitialize(screenWidth, screenHeight, "Maze of Shadows");
    return 0;
}