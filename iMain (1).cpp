#include <math.h>
#include "iGraphics.h"
#include "iSound.h"

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
int soundvolume = 50,songvolume = 50;
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
    iLoadImage(&bg1, "assets/images/MoSpos1.png");
    iResizeImage(&bg1, 1000, 600);

    iLoadImage(&throwingStar, "assets/images/starthrowing.png");
    iResizeImage(&throwingStar, 20, 20);

    iLoadImage(&settingsmenu, "assets/images/Settings.png");
    iResizeImage(&settingsmenu, 1000, 600);

    iLoadImage(&soundon, "assets/images/SoundOn.png");
    iResizeImage(&soundon, 80, 80);

    iLoadImage(&soundoff, "assets/images/SoundOff.png");
    iResizeImage(&soundoff, 80, 80);

    iLoadImage(&songon, "assets/images/SongOn.png");
    iResizeImage(&songon, 65, 65);

    iLoadImage(&songoff, "assets/images/SongOff.png");
    iResizeImage(&songoff, 65, 65);

    iLoadImage(&knob, "assets/images/Knob.png");
    iResizeImage(&knob, 14, 14);

    iLoadImage(&bg, "assets/images/BG2.png");
    iResizeImage(&bg, 1000, 600);

    iLoadFramesFromFolder(runRight, "assets/images/sprites/MoSNinja");
    iInitSprite(&playerSprite, -1);
    iChangeSpriteFrames(&playerSprite, runRight, runFrameCount);
    iResizeSprite(&playerSprite, playerWidth, playerHeight);
    iSetSpritePosition(&playerSprite, playerX, playerY);

    iLoadFramesFromFolder(starthrowing, "assets/images/sprites/starthrowing");
    iInitSprite(&starthrow, -1);
    iChangeSpriteFrames(&starthrow, starthrowing, starthrowframecount);
    iResizeSprite(&starthrow, playerWidth, playerHeight);
    iSetSpritePosition(&starthrow, playerX, playerY);
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
    iShowImage(0, 0, "assets/images/Ninja.png");

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
            bgsongchannel = iPlaySound("assets/sounds/start.wav", 1, songvolume);
            lastSongVolume = songvolume;
            wasSongOn = true;
        }
        else if (songvolume != lastSongVolume)
        {
            iStopSound(bgsongchannel);
            bgsongchannel = iPlaySound("assets/sounds/start.wav", 1, songvolume);
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
    drawstar();
}
void updatePlayer()
{
    //<This part for falling><and this part for jumping>
    if (playerY > 50 || verticalSpeed > 0)
    {
        if (movingRight)
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
        else if (movingLeft)
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
        playerY += verticalSpeed;
        verticalSpeed += gravity;
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

/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    // place your drawing codes here
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
    }

    // playBGSong();
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
            iPlaySound("assets/sounds/clicking1.wav", 0, soundvolume);
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
void SpecialKeyboardUP(){
     moveTimer++;
    if (moveTimer >= MOVE_INTERVAL){
        moving = false; 
    }
}


void iTimer()
{
    updatePlayer();
    updatestar();
    playBGSong();
    SpecialKeyboardUP();
    iResumeTimer(0); // Resume
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // place your own initialization codes here.
    load_Images();
    iSetTimer(120, animatePlayer);
    iSetTimer(60, animatestarthrow);
    iSetTimer(20, iTimer);
    iInitializeSound();
    playBGSong();
    iInitialize(screenWidth, screenHeight, "Maze of Shadows");
    return 0;
}