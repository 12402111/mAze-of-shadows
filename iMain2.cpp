#include <math.h>
#include "iGraphics.h"
#include "iSound.h"

Image fire, fire1, log1, tree, tree1_png;
#define NUM_OBSTACLES 30 // Adjust based on worldWidth

int obstacleX[NUM_OBSTACLES];
int obstacleY[NUM_OBSTACLES];

int playerHealth = 3;
bool hitCooldown = false;
int hitCooldownTimer = 0;
const int HIT_COOLDOWN_LIMIT = 30; // frames to wait before next hit allowed

int screenWidth = 1000, screenHeight = 600;
Image bg1;

int navY = 490;
int buttonWidth = 150;
int buttonHeight = 50;
int currentScreen = 0;

int playerX = 500, playerY = 50, playerWidth = 55, playerHeight = 100;

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

Image settingsmenu, soundon, soundoff, songon;
Image songoff;
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

int worldScreenCount = 3;
int worldWidth = worldScreenCount * screenWidth, worldX = 0;

bool isJumping = false;
int jumpSpeed = 20, gravity = -2, verticalSpeed = 0;

char *navItems[] = {"Start", "About", "Settings", "Help"};

bool isPlayButtonClicked = false;

void load_Images()
{
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
}

void generateObstacles()
{
    int spacing = 350; // space between each obstacle

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
    else
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
    drawSprite();
    drawObstacles();
    drawstar();
    drawHealthBar();
}

void updatePlayer()
{
    // Handle horizontal movement and background scrolling while jumping or falling
    if (playerY > 50 || verticalSpeed > 0)
    {
        if (movingRight)
        {
            if (worldX < worldWidth - screenWidth)
            {
                worldX += 5;
            }
            if (worldX >= 0 && worldX <= worldWidth - screenWidth)
            {
                iWrapImage(&bg, -5);
            }
            else if (worldX <= 0 || worldX >= worldWidth - screenWidth)
            {
                playerX += 5;
            }
        }
        else if (movingLeft)
        {
            if (worldX > -490)
            {
                worldX -= 5;
            }
            if (worldX >= 0 && worldX <= worldWidth - screenWidth)
            {
                iWrapImage(&bg, 5);
            }
            else if (worldX <= 0 || worldX >= worldWidth - screenWidth)
            {
                playerX -= 5;
            }
        }
        playerY += verticalSpeed;
        verticalSpeed += gravity;
    }

    // Check for collision only when not jumping and no cooldown
    if (!isJumping && !hitCooldown && currentScreen == 1)
    {
        for (int i = 0; i < NUM_OBSTACLES; i++)
        {
            int drawX = obstacleX[i] - worldX;
            int obsWidth = 80, obsHeight = 80;

            if (playerX < drawX + obsWidth &&
                playerX + playerWidth > drawX &&
                playerY < obstacleY[i] + obsHeight &&
                playerY + playerHeight > obstacleY[i])
            {
                if (sound)
                {
                    iPlaySound("saves/assets/sounds/hit.wav", 0, soundvolume);
                }

                playerHealth--;
                if (playerHealth < 0)
                    playerHealth = 0;

                hitCooldown = true;

                if (playerHealth == 0)
                {
                    currentScreen = 5; // Game over screen
                }

                break;
            }
        }
    }

    // Player hits the ground
    if (playerY <= 50)
    {
        playerY = 50;
        isJumping = false;
        verticalSpeed = 0;
    }

    // Clamp player inside screen bounds horizontally
    if (playerX < 0)
        playerX = 0;
    if (playerX > screenWidth - playerWidth)
        playerX = screenWidth - playerWidth;
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

void gameOverScreen()
{
    iClear();
    iSetColor(255, 0, 0);
    iText(screenWidth / 2 - 60, screenHeight / 2 + 20, "GAME OVER", GLUT_BITMAP_HELVETICA_18);
    iSetColor(255, 255, 255);
    iText(screenWidth / 2 - 150, screenHeight / 2 - 20, "Press HOME key to Restart", GLUT_BITMAP_HELVETICA_18);
}

/*
function iDraw() is called again and again by the system.
*/
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
            gameOverScreen();
        }
    }
}

/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
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
            if (soundknobx < 370)
                soundknobx = 370;
            if (soundknobx > 770)
                soundknobx = 770;

            soundvolume = (soundknobx - 370) * 100 / (770 - 370);
        }
        else if (mx >= songknobx && mx <= songknobx + 14 && my >= songknoby && my <= songknoby + 14)
        {
            songknobclicked = true;
            soundknobclicked = false;
            songknobx = mx - 7;
            if (songknobx < 370)
                songknobx = 370;
            if (songknobx > 770)
                songknobx = 770;

            songvolume = (songknobx - 370) * 100 / (770 - 370);
        }
    }
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (!isPlayButtonClicked)
    {
        int playX = (screenWidth / 4) * 3;
        int playY = (screenHeight / 4);
        int r = 40;

        // Check if play button clicked
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            int dx = mx - playX;
            int dy = my - playY;
            if (dx * dx + dy * dy <= r * r)
            {
                isPlayButtonClicked = true;
                currentScreen = 0; // navigation menu
            }
        }
    }
    else
    {
        if (currentScreen == 0)
        {
            mouseNavigationBar(mx, my);
        }
        else if (currentScreen == 3)
        {
            mouseSettings(mx, my);
        }
    }
}

/*
function iKeyboard() is called whenever the user presses a key.
*/
void iKeyboard(unsigned char key)
{
    if (key == ' ')
    {
        if (!isJumping)
        {
            verticalSpeed = jumpSpeed;
            isJumping = true;
        }
    }
    if (key == 27) // ESC to exit
    {
        exit(0);
    }
}

/*
function iSpecialKeyboard() is called whenver user hits special keys like
arrow keys, home, end, pg up, pg down, etc. 
*/
// Add key state tracking variables
bool leftKeyDown = false;
bool rightKeyDown = false;

void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_RIGHT:
        rightKeyDown = true;
        movingRight = true;
        movingLeft = false;
        moving = true;
        break;

    case GLUT_KEY_LEFT:
        leftKeyDown = true;
        movingLeft = true;
        movingRight = false;
        moving = true;
        break;

    case GLUT_KEY_HOME:
        // Restart game on Home key
        currentScreen = 0; // back to navigation menu
        playerHealth = 3;
        playerX = 500;
        playerY = 50;
        worldX = 0;
        hitCooldown = false;
        hitCooldownTimer = 0;
        moving = false;
        movingLeft = false;
        movingRight = false;
        isJumping = false;
        verticalSpeed = 0;
        leftKeyDown = false;
        rightKeyDown = false;
        break;
    }
}

/*
function SpecialKeyboardUP() is called when special keys are released.
*/
void iSpecialKeyboardUp(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_RIGHT:
        rightKeyDown = false;
        movingRight = false;
        if (!leftKeyDown)
            moving = false;
        break;
    case GLUT_KEY_LEFT:
        leftKeyDown = false;
        movingLeft = false;
        if (!rightKeyDown)
            moving = false;
        break;
    }
}

/*
function iTimer() is called every 20 ms
*/
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

    if (moving)
    {
        // Animate running sprite
        moveTimer++;
        if (moveTimer >= MOVE_INTERVAL)
        {
            iAnimateSprite(&playerSprite);
            moveTimer = 0;
        }
    }

    updatePlayer();
    updatestar();
    playBGSong();
}

int main()
{
    load_Images();
    generateObstacles();

    iSetTimer(20, iTimer);

    iInitialize(screenWidth, screenHeight, "Ninja Game with Collision");

    // Register special key up handler if your framework supports it
    // For GLUT: glutSpecialUpFunc(iSpecialKeyboardUp);

    return 0;
}
