#include <math.h>
#include "iGraphics.h"
#include "iSound.h"
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
int count = 0;
int screenWidth = 1000, screenHeight = 600;
int worldScreenCount = 30;
int worldWidth = worldScreenCount * screenWidth, worldX = 0;
bool isJumping = false;
int jumpSpeed = 25, gravity = -2, verticalSpeed = 0;
bool isPlayButtonClicked = false;
bool allDragonsDefeated = false;
bool victoryTriggered = false;
int victoryTimer = 0;
const int VICTORY_DISPLAY_TIME = 120; // 3 seconds at 60fps
Image helpim, aboutim;
struct Heart
{
    int x, y;
    bool active;
};

Heart heart;
// int heartImage;
int heartFallSpeed = 3; // adjust for desired falling speed
Image heartImg;
bool heartonfloor = false;
int heartSpawnTimer = 0;
int heartSpawnInterval = 20 * 60; // 20 seconds (60 frames per second)

// Add these constants for better boundary management
#define WORLD_LEFT_BOUNDARY 0
#define WORLD_RIGHT_BOUNDARY (worldWidth - screenWidth)
int difficulty = 1; // FIXED: Set default to medium (1), not 0

int navY = 490;
int buttonWidth = 150;
int buttonHeight = 52;
int currentScreen = 0;

int levelTimerSeconds = 180; // 3 minutes
bool levelTimerRunning = false;
bool bossTriggered = false;

int playerX = 500, playerY = 50, playerWidth = 55, playerHeight = 100;

Image fire, fire1, log1, tree, tree1_png;
Image log1_switched, tree1_switched, fire1_switched;

Image highObstacle;
Image nameEntryImage;
Image navBg;
Image highScoreBg;

#define NUM_OBSTACLES 100
bool obstaclePassed[NUM_OBSTACLES];
int playerScore = 0;
typedef struct
{
    char name[100];
    int score;
} HighScore;

#define MAX_HIGH_SCORES 10
HighScore highScores[MAX_HIGH_SCORES];
int numHighScores = 0;

int golemSpawnTimer = 0;
const int GOLEM_SPAWN_DELAY = 200;
int nextGolemX = 1000;

int barbarianSpawnTimer = 0;
const int BARBARIAN_SPAWN_DELAY = 100;
int nextBarbarianX = 1500; // FIXED: Initialize properly

char playerName[100] = "";
bool enteringName = false;
int nameCharIndex = 0;

int obstacleX[NUM_OBSTACLES];
int obstacleY[NUM_OBSTACLES];
int collisionSoundChannel = -1;
Image endImage;
bool endSoundPlayed = false;

int playerHealth = 10; // FIXED: Initialize to 10 instead of 50
bool hitCooldown = false;
int hitCooldownTimer = 0;
const int HIT_COOLDOWN_LIMIT = 30; // frames to wait before next hit allowed

Image bg1;
bool worldSwitched = false;
int scrollSpeed = 7; // default speed
Image bg2;           // second world background

#define MAX_LEAVES 50

typedef struct
{
    int x, y;
    int speed;
    bool active;
} Leaf;

Leaf leaves[MAX_LEAVES];
Image leafImg;

Image bg;
FrameSet runRight;
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
    bool active = false;
    bool directionRight = true;
    bool directionLeft = false;
} Star;

Star stars[MAX_STARS];
Sprite throwingStarsprite[MAX_STARS];
FrameSet throwingStar;
int starSpeed = 15;
FrameSet starthrowing;
int starthrowframecount = 5;
Sprite starthrow;
bool isthrowingstar = false;
bool starthrowdirection = true; // FIXED: Initialize to true (facing right)
int throwingFrame = 0;

typedef struct
{ // Golem
    int x, y;
    int speed;
    bool active;
    int health;
} Golem;
Sprite golemSprite;
FrameSet golemImage;
bool golemRight = true;
bool golemLeft = false;
#define GOLEM_SPAWN_INTERVAL 850
#define MAX_GOLEMS 30
Golem golems[MAX_GOLEMS];
int numGolems = 0;

typedef struct
{ // Barbarian
    int x, y;
    int speed;
    bool active;
    int health;
} Barbarian;

Sprite barbarianSprite;
FrameSet barbarianImage;

bool barbarianRight = true;
bool barbarianLeft = false;

#define BARBARIAN_SPAWN_INTERVAL 400
#define MAX_BARBARIANS 50
#define BARBARIAN_SPAWN_DELAY 60

Barbarian barbarians[MAX_BARBARIANS];
int numBarbarians = 0;

#define DRAGON_FLY_FRAMES 6
#define DRAGON_FIRE_FRAMES 6
#define DRAGON_MAX_HEALTH 3
#define MAX_DRAGONS 20

FrameSet dragonFly;
FrameSet dragonFire;
Sprite dragonSprite;

typedef struct
{
    int x, y;
    int health;
    bool active;
    bool isSpeaking;
    bool isFiring;
    int fireDuration;
    int fireCooldown;
    int speed;
} Dragon;

Dragon dragons[MAX_DRAGONS];
int totalDragons = 0;
int currentDragonIndex = 0;

bool dragonIsFiring = false;
bool DragonFacingRight = false;
bool DragonFacingLeft = true;
int DragonSoundChannel;

// Platform..................................
#define MAX_PLATFORMS 50

typedef struct
{
    int x, y;
    int width, height;
    bool active;
} Platform;

Platform platforms[MAX_PLATFORMS];
int numPlatforms = 40;
bool onPlatform = false;
bool onLand = true;
Image PlatfromImage[6];

//.................................Platform

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

void drawRoad()
{
    int roadHeight = 50;
    iSetColor(60, 60, 60);
    iFilledRectangle(0, 0, screenWidth, roadHeight);
}

void load_Images()
{
    // Load and resize obstacles
    iLoadImage(&nameEntryImage, "saves/assets/images/namescreen.png");
    iResizeImage(&nameEntryImage, 1000, 600);
    iLoadImage(&leafImg, "saves/assets/images/leaf.png");
    iResizeImage(&leafImg, 20, 20);
    iLoadImage(&heartImg, "saves/assets/images/heart.png");
    iResizeImage(&heartImg, 30, 30);

    iLoadImage(&highScoreBg, "saves/assets/images/highscore.jpg");
    iResizeImage(&highScoreBg, 1000, 600);

    iLoadImage(&fire1, "saves/assets/images/obstacles/fire1.png");
    iResizeImage(&fire1, 80, 80);

    iLoadImage(&log1, "saves/assets/images/obstacles/log1.png");
    iResizeImage(&log1, 80, 80);

    iLoadImage(&tree1_png, "saves/assets/images/obstacles/obs2.png");
    iResizeImage(&tree1_png, 80, 80);

    iLoadImage(&log1_switched, "saves/assets/images/obstacles/ship.png");
    iResizeImage(&log1_switched, 80, 100);

    iLoadImage(&tree1_switched, "saves/assets/images/obstacles/dead_tree.png");
    iResizeImage(&tree1_switched, 80, 100);

    iLoadImage(&fire1_switched, "saves/assets/images/obstacles/lava.png");
    iResizeImage(&fire1_switched, 80, 100);

    iLoadImage(&navBg, "saves/assets/images/ninja.png");
    iResizeImage(&navBg, 1000, 600);

    iLoadImage(&bg1, "saves/assets/images/MoSpos1.png");
    iResizeImage(&bg1, 1000, 600);
    iLoadImage(&helpim, "saves/assets/images/Help.png");
    iResizeImage(&helpim, 1000, 600);

    iLoadImage(&aboutim, "saves/assets/images/About.png");
    iResizeImage(&aboutim, 1000, 600);

    iLoadFramesFromFolder(&throwingStar, "saves/assets/images/sprites/star");
    for (int i = 0; i < MAX_STARS; i++)
    {
        stars[i].active = false;
        iInitSprite(&throwingStarsprite[i]);
        iChangeSpriteFrames(&throwingStarsprite[i], &throwingStar);
        iResizeSprite(&throwingStarsprite[i], 20, 20);
    }

    iLoadImage(&settingsmenu, "saves/assets/images/settingBg.png");
    iResizeImage(&settingsmenu, 1000, 600);

    iLoadImage(&soundon, "saves/assets/images/SoundOn.png");
    iResizeImage(&soundon, 80, 80);

    iLoadImage(&soundoff, "saves/assets/images/SoundOff.png");
    iResizeImage(&soundoff, 80, 80);

    iLoadImage(&songon, "saves/assets/images/SongOn.png");
    iResizeImage(&songon, 65, 65);

    iLoadImage(&songoff, "saves/assets/images/SongOff.png");
    iResizeImage(&songoff, 65, 65);

    iLoadImage(&knob, "saves/assets/images/knob.png");
    iResizeImage(&knob, 14, 14);

    iLoadImage(&endImage, "saves/assets/images/GameOver.png");
    iResizeImage(&endImage, 1000, 600);

    iLoadImage(&bg, "saves/assets/images/backimg2.png");
    iResizeImage(&bg, 1000, 600);
    iLoadImage(&bg2, "saves/assets/images/backimg4.png");
    iResizeImage(&bg2, 1020, 600);

    iLoadFramesFromFolder(&runRight, "saves/assets/images/sprites/ninjas");
    iInitSprite(&playerSprite);
    iChangeSpriteFrames(&playerSprite, &runRight);
    iResizeSprite(&playerSprite, playerWidth, playerHeight);
    iSetSpritePosition(&playerSprite, playerX, playerY);

    iLoadFramesFromFolder(&starthrowing, "saves/assets/images/sprites/starthrowing");
    iInitSprite(&starthrow);
    iChangeSpriteFrames(&starthrow, &starthrowing);
    iResizeSprite(&starthrow, playerWidth, playerHeight);
    iSetSpritePosition(&starthrow, playerX, playerY);

    iLoadFramesFromFolder(&golemImage, "saves/assets/images/sprites/Golem_2/RunThrowing");
    iInitSprite(&golemSprite);
    iChangeSpriteFrames(&golemSprite, &golemImage);
    iResizeSprite(&golemSprite, playerWidth, playerHeight);

    iLoadFramesFromFolder(&barbarianImage, "saves/assets/images/sprites/barbarians/RunThrowing2");
    iInitSprite(&barbarianSprite);
    iChangeSpriteFrames(&barbarianSprite, &barbarianImage);
    iResizeSprite(&barbarianSprite, playerWidth, playerHeight);

    iLoadFramesFromFolder(&dragonFly, "saves/assets/images/sprites/Dragonflying");
    iLoadFramesFromFolder(&dragonFire, "saves/assets/images/sprites/Dragonfiresplit");
    iInitSprite(&dragonSprite);
    iChangeSpriteFrames(&dragonSprite, &dragonFly);
    iResizeSprite(&dragonSprite, 250, 200);
}

// FIXED: Better obstacle generation
void generateObstacles()
{
    int spacing = 350;
    int startX = 300;

    for (int i = 0; i < NUM_OBSTACLES; ++i)
    {
        obstacleX[i] = startX + (i * spacing);
        obstacleY[i] = 30;
        obstaclePassed[i] = false;
    }
}

// FIXED: Better obstacle culling and rendering
void drawObstacles()
{
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        int drawX = obstacleX[i] - worldX;

        // Expanded culling bounds to prevent pop-in/pop-out
        if (drawX + 120 < -50 || drawX > screenWidth + 50)
            continue;

        int type = i % 3;

        if (!worldSwitched)
        {
            if (type == 0)
                iShowLoadedImage(drawX, obstacleY[i], &log1);
            else if (type == 1)
                iShowLoadedImage(drawX, obstacleY[i], &tree1_png);
            else
                iShowLoadedImage(drawX, obstacleY[i], &fire1);
        }
        else
        {
            if (type == 0)
                iShowLoadedImage(drawX, obstacleY[i], &log1_switched);
            else if (type == 1)
                iShowLoadedImage(drawX, obstacleY[i], &tree1_switched);
            else
                iShowLoadedImage(drawX, obstacleY[i], &fire1_switched);
        }
    }
}

// FIXED: Platform initialization
void initPlatforms()
{
    int startX = 800;
    int currentX = startX;

    for (int i = 0; i < numPlatforms; i++)
    {
        int gap = 400 + rand() % 200;
        int y = 120 + rand() % 50;
        int width = 140 + rand() % 60;

        platforms[i] = (Platform){currentX, y, width, 10, true};
        currentX += gap;
    }
    for (int j = 0; j < 6; j++)
    {
        // Load platform images based on world state

        if (j == 0)
        {
            iLoadImage(&PlatfromImage[j], "saves/assets/images/Platform1.jpg");
            iResizeImage(&PlatfromImage[j], platforms[j].width, 30);
        }
        else if (j == 1)
        {
            iLoadImage(&PlatfromImage[j], "saves/assets/images/Platform2.png");
            iResizeImage(&PlatfromImage[j], platforms[j].width, 30);
        }
        else if (j == 2)
        {
            iLoadImage(&PlatfromImage[j], "saves/assets/images/Platform3.png");
            iResizeImage(&PlatfromImage[j], platforms[j].width, 30);
        }

        else if (j == 3)
        {
            iLoadImage(&PlatfromImage[j], "saves/assets/images/platformx1.png");
            iResizeImage(&PlatfromImage[j], platforms[j].width, 30);
        }
        else if (j == 4)
        {
            iLoadImage(&PlatfromImage[j], "saves/assets/images/platformx2.png");
            iResizeImage(&PlatfromImage[j], platforms[j].width, 30);
        }
        else
        {
            iLoadImage(&PlatfromImage[j], "saves/assets/images/platformx3.png");
            iResizeImage(&PlatfromImage[j], platforms[j].width, 30);
        }
    }
}

void drawPlatforms()
{
    for (int i = 0; i < numPlatforms; i++)
    {

        if (!worldSwitched && platforms[i].active && platforms[i].x - worldX <= 1000)
        {
            iShowLoadedImage(platforms[i].x - worldX, platforms[i].y, &PlatfromImage[i % 3]);
        }
        else if (worldSwitched && platforms[(i + 3) % numPlatforms].active && platforms[(i + 3) % numPlatforms].x - worldX <= 1000)
        {
            iShowLoadedImage(platforms[(i + 3) % numPlatforms].x - worldX, platforms[(i + 3) % numPlatforms].y, &PlatfromImage[(i % 3) + 3]);
        }
    }
}

void drawHealthBar()
{
    int heartX = 20;
    int heartY = screenHeight - 50;

    for (int i = 0; i < playerHealth; i++)
    {
        iShowLoadedImage(heartX + i * 35, heartY, &heartImg);
    }

    // Score text
    char scoreText[20];
    sprintf(scoreText, "Score: %d", playerScore);
    iSetColor(255, 255, 0);
    iText(20, heartY - 25, scoreText, GLUT_BITMAP_HELVETICA_18);

    char countdownText[20];
    int minutes = levelTimerSeconds / 60;
    int seconds = levelTimerSeconds % 60;
    sprintf(countdownText, "Time Left: %02d:%02d", minutes, seconds);
    iSetColor(255, 100, 100);
    iText(800, screenHeight - 30, countdownText, GLUT_BITMAP_HELVETICA_18);
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
//  1. Spawn heart at random X in world space
void spawnHeart()
{
    if (!heart.active && heartSpawnTimer >= heartSpawnInterval)
    {
        //  heart.x = rand() % (worldWidth - 32); // World coordinate, not screen
        // heart.x = worldX + 300;
        heart.x = worldX + 400 + rand() % 200; // 400–600 px ahead of camera
        heart.y = screenHeight + 50;           // Start above screen
        heart.active = true;
        heartSpawnTimer = 0; // Reset timer after spawning
    }
}

//  2. Update heart falling & landing
void updateHeart()
{
    if (heart.active && !heartonfloor)
    {
        if (heart.y > 45)
        {
            heart.y -= heartFallSpeed;
        }
        else
        {
            heart.y = 45; // Land on ground

            heartonfloor = true; // Stop falling
        }
    }
}

//  3. Draw heart on screen relative to scrolling
void drawHeart()
{
    if (heart.active)
    {
        int drawX = heart.x - worldX; // Convert world to screen position
        iShowLoadedImage(drawX, heart.y, &heartImg);
    }
}

//  4. Collision with player (adjust for scrolling!)
void checkHeartCollision()
{
    if (heart.active)
    {
        int heartDrawX = heart.x - worldX; // Convert to screen coordinates for collision

        bool collision =
            heartDrawX < playerX + playerWidth &&
            heartDrawX + 32 > playerX &&
            heart.y < playerY + playerHeight &&
            heart.y + 32 > playerY;

        if (collision)
        {
            heart.active = false;
            heartonfloor = false;
            playerHealth++;

            // Optional: play pickup sound or animation here
        }
    }
}
// FIXED: Golem generation
void generateGolems()
{
    numGolems = 0;
    for (int i = GOLEM_SPAWN_INTERVAL * 2; i < worldWidth - 200; i += GOLEM_SPAWN_INTERVAL)
    {
        if (numGolems >= MAX_GOLEMS)
            break;

        golems[numGolems].x = i;
        golems[numGolems].y = 45;
        golems[numGolems].speed = 4;
        golems[numGolems].health = 1;
        golems[numGolems].active = true;

        numGolems++;
    }
}

// FIXED: Barbarian generation
void generateBarbarians()
{
    numBarbarians = 0;
    for (int i = BARBARIAN_SPAWN_INTERVAL * 3; i < worldWidth - 200; i += BARBARIAN_SPAWN_INTERVAL)
    {
        if (numBarbarians >= MAX_BARBARIANS)
            break;

        barbarians[numBarbarians].x = i;
        barbarians[numBarbarians].y = 45;
        barbarians[numBarbarians].speed = 4;
        barbarians[numBarbarians].health = 2;
        barbarians[numBarbarians].active = true;

        numBarbarians++;
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
        int px = playerX + worldX;

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

        for (int j = 0; j < MAX_GOLEMS; j++)
        {
            if (!golems[j].active)
                continue;

            if (/*iCheckCollision(&golemSprite[j], &throwingStarsprite[i])*/ iCheckCollision(&golemSprite, &throwingStarsprite[i]))
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
                iStopSound(-1);
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

void drawBarbarian()
{
    for (int i = 0; i < numBarbarians; i++)
    {
        if (!barbarians[i].active)
            continue;

        int bx = barbarians[i].x - worldX;

        if (bx >= -100 && bx <= screenWidth + 100)
        {
            barbarians[i].y = 50;
            iSetSpritePosition(&barbarianSprite, bx, barbarians[i].y);
            iShowSprite(&barbarianSprite);
        }
    }
}

void updateBarbarians()
{
    for (int i = 0; i < numBarbarians; i++)
    {
        if (!barbarians[i].active)
            continue;

        int bx = barbarians[i].x;
        int px = playerX + worldX;

        if (bx > px && bx - px <= 250)
        {
            if (barbarianRight)
            {
                barbarianLeft = true;
                barbarianRight = false;
                iMirrorSprite(&barbarianSprite, HORIZONTAL);
            }
            barbarians[i].x -= barbarians[i].speed;
        }
        else if (bx < px && px - bx <= 250)
        {
            if (barbarianLeft)
            {
                barbarianRight = true;
                barbarianLeft = false;
                iMirrorSprite(&barbarianSprite, HORIZONTAL);
            }
            barbarians[i].x += barbarians[i].speed;
        }
    }
}

void checkStarBarbarianCollision()
{
    for (int i = 0; i < MAX_STARS; i++)
    {
        if (!stars[i].active)
            continue;

        for (int j = 0; j < MAX_BARBARIANS; j++)
        {
            if (!barbarians[j].active)
                continue;

            int bx = barbarians[j].x - worldX;
            int by = barbarians[j].y;
            int bWidth = playerWidth;
            int bHeight = playerHeight;

            if (stars[i].x < bx + bWidth && stars[i].x + 20 > bx && stars[i].y < by + bHeight && stars[i].y + 20 > by)
            {
                barbarians[j].health--;
                stars[i].active = false;

                if (barbarians[j].health <= 0)
                {
                    barbarians[j].active = false;
                    playerScore += 100;
                }
            }
        }
    }
}

void checkBarbarianCollision()
{
    if (hitCooldown)
        return;

    for (int i = 0; i < numBarbarians; i++)
    {
        if (!barbarians[i].active)
            continue;

        int bx = barbarians[i].x - worldX;
        int by = barbarians[i].y;
        int bWidth = playerWidth;
        int bHeight = playerHeight;

        if (playerX - 35 <= bx + bWidth && playerX + playerWidth - 35 >= bx && playerY <= by + bHeight && playerY + playerHeight >= by)
        {
            if (sound)
            {
                iStopSound(-1);
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

void animateBarbarian()
{
    iAnimateSprite(&barbarianSprite);
}

// FIXED: Dragon initialization
void initDragon()
{
    if (difficulty == 0)
        totalDragons = 3; // Easy
    else if (difficulty == 1)
        totalDragons = 5; // Medium
    else
        totalDragons = 7; // Hard

    int baseX = 1500;   // FIXED: Place dragons further away
    int spacing = 1000; // FIXED: Increase spacing between dragons

    for (int i = 0; i < totalDragons; i++)
    {
        dragons[i].x = baseX + i * spacing;
        dragons[i].y = 250;
        dragons[i].health = DRAGON_MAX_HEALTH;
        dragons[i].active = true;
        dragons[i].isSpeaking = true;
        dragons[i].isFiring = false;
        dragons[i].fireCooldown = 200;
        dragons[i].fireDuration = 50;
        dragons[i].speed = 2;
    }

    currentDragonIndex = 0;
    dragonIsFiring = false;
    DragonFacingRight = true;
    DragonFacingLeft = false;
    iChangeSpriteFrames(&dragonSprite, &dragonFly);
}

void drawDragon()
{
    if (currentDragonIndex >= totalDragons)
        return;

    Dragon *dragon = &dragons[currentDragonIndex];
    if (!dragon->active)
        return;

    int dx = dragon->x - worldX;

    // Only draw if dragon is visible on screen
    if (dx < -300 || dx > screenWidth + 100)
        return;

    iSetSpritePosition(&dragonSprite, dx, dragon->y);
    iShowSprite(&dragonSprite);

    if (dragon->isSpeaking && dx < 800)
    {
        iSetColor(255, 255, 255);
        iText(dx - 20, dragon->y + 185, "You dare approach me?!!!!", GLUT_BITMAP_TIMES_ROMAN_24);
        static int speechTimer = 200;
        speechTimer--;
        if (speechTimer <= 0)
        {
            dragon->isSpeaking = false;
        }
    }

    // Health bar
    int barX = dx + 25;
    int barY = dragon->y + 160;
    int barWidth = 100;
    int barHeight = 12;

    float healthRatio = (float)dragon->health / DRAGON_MAX_HEALTH;

    iSetColor(100, 100, 100);
    iFilledRectangle(barX, barY, barWidth, barHeight);
    iSetColor(255, 0, 0);
    iFilledRectangle(barX, barY, barWidth * healthRatio, barHeight);
}

void updateDragonPosition()
{
    if (currentDragonIndex >= totalDragons)
        return;

    Dragon *dragon = &dragons[currentDragonIndex];
    if (!dragon->active)
        return;

    static int hoverTimer = 0;
    hoverTimer++;
    dragon->y += (hoverTimer % 120 < 60) ? -1 : 1; // FIXED: Smaller hover movement

    int px = playerX + worldX;
    int dx = dragon->x;
    if (dx > px && dx - px <= 300) // FIXED: Larger detection range
    {
        if (DragonFacingRight)
        {
            iMirrorSprite(&dragonSprite, HORIZONTAL);
            DragonFacingRight = false;
            DragonFacingLeft = true;
        }
        dragon->x -= dragon->speed;
    }
    else if (px > dx && px - dx <= 300)
    {
        if (DragonFacingLeft)
        {
            iMirrorSprite(&dragonSprite, HORIZONTAL);
            DragonFacingLeft = false;
            DragonFacingRight = true;
        }
        dragon->x += dragon->speed;
    }
}

void updateDragonAttack()
{
    if (currentDragonIndex >= totalDragons)
        return;

    Dragon *dragon = &dragons[currentDragonIndex];
    if (!dragon->active)
        return;

    if (dragon->fireCooldown > 0)
    {
        dragon->fireCooldown--;
    }
    else
    {
        dragon->isFiring = true;
        dragon->fireDuration--;

        if (!dragonIsFiring)
        {
            iChangeSpriteFrames(&dragonSprite, &dragonFire);
            dragonIsFiring = true;
            iStopSound(-1);
            DragonSoundChannel = iPlaySound("saves/assets/sounds/dragon.wav", 0, soundvolume);
        }

        if (dragon->fireDuration <= 0)
        {
            dragon->isFiring = false;
            dragon->fireCooldown = 150 + rand() % 100;
            dragon->fireDuration = 60;

            if (dragonIsFiring)
            {
                iChangeSpriteFrames(&dragonSprite, &dragonFly);
                dragonIsFiring = false;
            }
        }
    }
}

void checkDragonFireCollision()
{
    if (currentDragonIndex >= totalDragons)
        return;

    Dragon *dragon = &dragons[currentDragonIndex];
    if (!dragon->active || !dragon->isFiring || hitCooldown)
        return;

    int fx = dragon->x - worldX + 40;
    int fw = 10 + (60 - dragon->fireDuration) * 2;

    if (playerX + playerWidth >= fx && playerX <= fx + fw && playerY <= dragon->y)
    {
        playerHealth--;
        hitCooldown = true;

        if (sound)
        {
            iStopSound(-1);
            DragonSoundChannel = iPlaySound("saves/assets/sounds/dragon.wav", 0, soundvolume);
        }

        if (playerHealth <= 0)
            currentScreen = 5;
    }
}

void checkPlayerDragonCollision()
{
    if (currentDragonIndex >= totalDragons)
        return;

    Dragon *dragon = &dragons[currentDragonIndex];
    if (!dragon->active || hitCooldown)
        return;

    int dx = dragon->x - worldX;
    int dy = dragon->y;
    int dragonWidth = 150;
    int dragonHeight = 100;

    if (playerX + playerWidth > dx && playerX < dx + dragonWidth &&
        playerY + playerHeight > dy && playerY < dy + dragonHeight)
    {
        playerHealth--;
        hitCooldown = true;

        if (sound)
        {
            iStopSound(-1);
            DragonSoundChannel = iPlaySound("saves/assets/sounds/dragon.wav", 0, soundvolume);
        }

        if (playerHealth <= 0)
            currentScreen = 5;
    }
}

void checkStarDragonCollision()
{
    if (currentDragonIndex >= totalDragons)
        return;

    Dragon *dragon = &dragons[currentDragonIndex];
    if (!dragon->active)
        return;

    for (int i = 0; i < MAX_STARS; i++)
    {
        if (!stars[i].active)
            continue;

        int dx = dragon->x - worldX;
        if (stars[i].x + 20 > dx && stars[i].x < dx + 150 &&
            stars[i].y + 20 > dragon->y && stars[i].y < dragon->y + 150)
        {
            dragon->health--;
            stars[i].active = false;

            if (dragon->health <= 0)
            {
                dragon->active = false;
                playerScore += 1000;
                playerHealth++;

                currentDragonIndex++;
                dragonIsFiring = false;

                if (currentDragonIndex < totalDragons)
                {
                    iChangeSpriteFrames(&dragonSprite, &dragonFly);
                }
                else
                {
                    // ALL DRAGONS DEFEATED! Victory condition
                    allDragonsDefeated = true;
                    victoryTriggered = true;
                    victoryTimer = VICTORY_DISPLAY_TIME;
                    levelTimerRunning = false;

                    if (sound)
                    {
                        iStopSound(-1);
                        iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
                    }
                }
            }
        }
    }
}

void animateDragon()
{
    iAnimateSprite(&dragonSprite);
}

// FIXED: High score functions
void loadHighScores()
{
    FILE *file = fopen("saves/data/scores.txt", "r");
    if (file == NULL)
    {
        numHighScores = 0;
        return;
    }

    numHighScores = 0;
    while (fscanf(file, "%99[^,],%d\n", highScores[numHighScores].name, &highScores[numHighScores].score) == 2)
    {
        numHighScores++;
        if (numHighScores >= MAX_HIGH_SCORES)
            break;
    }
    fclose(file);
}

void saveHighScores()
{
    FILE *file = fopen("saves/data/scores.txt", "w");
    if (file == NULL)
        return;

    for (int i = 0; i < numHighScores; i++)
    {
        fprintf(file, "%s,%d\n", highScores[i].name, highScores[i].score);
    }
    fclose(file);
}

void insertScore(char *name, int score)
{
    // Find insertion point
    int insertPos = numHighScores;
    for (int i = 0; i < numHighScores; i++)
    {
        if (score > highScores[i].score)
        {
            insertPos = i;
            break;
        }
    }

    // Shift scores down
    for (int i = min(numHighScores, MAX_HIGH_SCORES - 1); i > insertPos; i--)
    {
        highScores[i] = highScores[i - 1];
    }

    // Insert new score
    if (insertPos < MAX_HIGH_SCORES)
    {
        strncpy(highScores[insertPos].name, name, sizeof(highScores[insertPos].name) - 1);
        highScores[insertPos].name[sizeof(highScores[insertPos].name) - 1] = '\0';
        highScores[insertPos].score = score;

        if (numHighScores < MAX_HIGH_SCORES)
            numHighScores++;
    }

    saveHighScores();
}

void iPlayButton()
{
    iSetTransparentColor(61, 174, 191, 0.6);
    iFilledCircle((screenWidth / 4) * 3, (screenHeight / 4), 44);

    iSetTransparentColor(255, 128, 31, 0.8);
    iFilledCircle((screenWidth / 4) * 3, (screenHeight / 4), 40);

    iSetColor(60, 60, 60);
    iText((screenWidth / 4) * 3 - 20, (screenHeight / 4) - 5, "▶Play", GLUT_BITMAP_HELVETICA_18);
}

void drawNavigationBar()
{
    char *navItems[] = {"Start", "About", "Settings", "Help", "High Scores"};
    int navItemCount = 5;

    iShowLoadedImage(0, 0, &navBg);
    for (int i = 0; i < navItemCount; i++)
    {
        int x = screenWidth - buttonWidth - 50;
        int y = screenHeight - 66 - (i + 1) * buttonHeight - i * buttonHeight;

        iSetColor(180, 180, 180);
        iFilledRectangle(x, y, buttonWidth, buttonHeight);
        if (i == 4)
        {
            iSetColor(60, 60, 60);
            iText(x + buttonWidth / 2 - 35, y + buttonHeight / 2 - 5, navItems[i], GLUT_BITMAP_HELVETICA_18);
        }
        else
        {
            iSetColor(60, 60, 60);
            iText(x + buttonWidth / 2 - 20, y + buttonHeight / 2 - 5, navItems[i], GLUT_BITMAP_HELVETICA_18);
        }
    }
}

bool mouseNavigationBar(int mx, int my)
{
    int navItemCount = 5;
    int x = screenWidth - buttonWidth - 50;
    for (int i = 0; i < navItemCount; i++)
    {
        int y = screenHeight - 66 - (i + 1) * buttonHeight - i * buttonHeight;
        if (mx >= x && mx <= x + buttonWidth &&
            my >= y && my <= y + buttonHeight)
        {
            currentScreen = i + 1;
            if (i == 0)
            {
                enteringName = true;
                nameCharIndex = 0;
                playerName[0] = '\0';
            }

            if (i == 4)
                currentScreen = 6;
            return true;
        }
    }
    return false;
}

// FIXED: Proper background scrolling
void drawBackground()
{
    if (worldSwitched)
    {
        int bgOffset = worldX % screenWidth;
        iShowLoadedImage(-bgOffset, 0, &bg2);
        iShowLoadedImage(-bgOffset + screenWidth, 0, &bg2);
        if (bgOffset > 0)
            iShowLoadedImage(-bgOffset - screenWidth, 0, &bg2);
    }
    else
    {
        int bgOffset = worldX % screenWidth;
        iShowLoadedImage(-bgOffset, 0, &bg);
        iShowLoadedImage(-bgOffset + screenWidth, 0, &bg);
        if (bgOffset > 0)
            iShowLoadedImage(-bgOffset - screenWidth, 0, &bg);
    }
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

        if (throwingFrame >= starthrowframecount * 2) // FIXED: Better frame counting
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
                iSetSpritePosition(&throwingStarsprite[i], stars[i].x, stars[i].y);
                iShowSprite(&throwingStarsprite[i]);
                // iAnimateSprite(&throwingStarsprite[i]);
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
            if (stars[i].directionRight)
            {
                stars[i].x += starSpeed;
            }
            else if (stars[i].directionLeft)
            {
                stars[i].x -= starSpeed;
            }

            if (stars[i].x >= screenWidth - 10 || stars[i].x <= 10)
            {
                stars[i].active = false;
            }
        }
    }
}
void checkObstacleCollision()
{
    // Obstacle Collision
    // Check for collision with obstacles
    if (!isJumping && !hitCooldown)
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
                    iStopSound(-1);
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
    int playerWorldX = playerX + worldX;
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        int obsRightEdge = obstacleX[i] + 80;
        if (!obstaclePassed[i] && playerWorldX > obsRightEdge + 10)
        {
            playerScore += 50;
            obstaclePassed[i] = true;
        }
    }
}

// FIXED: Complete rewrite of updatePlayer with proper movement logic
void updatePlayer()
{
    if (isJumping)
    { // Handle horizontal movement
        if (movingRight)
        {
            if (worldX < WORLD_RIGHT_BOUNDARY)
            {
                worldX += scrollSpeed;
            }
            else if (playerX < screenWidth - playerWidth - 10)
            {
                playerX += scrollSpeed;
            }
        }
        else if (movingLeft)
        {
            if (worldX > WORLD_LEFT_BOUNDARY)
            {
                worldX -= scrollSpeed;
            }
            else if (playerX > 10)
            {
                playerX -= scrollSpeed;
            }
        }
        playerY += verticalSpeed;
        verticalSpeed += gravity;
    }
    // Handle vertical movement (jumping/falling)
    // if (isJumping || playerY > 50)
    /*{
        playerY += verticalSpeed;
        verticalSpeed += gravity;
    }*/

    // FIXED: Platform collision logic
    if (difficulty != 0)
    {
        bool foundPlatform = false;
        bool wasOnPlatform = onPlatform;
        onPlatform = false;

        for (int i = 0; i < numPlatforms; i++)
        {
            if (!platforms[i].active)
                continue;

            int platformX = platforms[i].x - worldX;
            int platformY = platforms[i].y;
            int platformWidth = platforms[i].width;
            int platformHeight = platforms[i].height;

            // Platform top collision (landing/running)
            bool horizontalOverlap = (playerX + playerWidth - 20 > platformX) &&
                                     (playerX + 20 < platformX + platformWidth);

            bool playerAbovePlatform = (playerY >= platformY + platformHeight - 5);
            bool playerNearPlatform = (playerY <= platformY + platformHeight + 15);
            bool fallingOrStanding = (verticalSpeed <= 0 || (wasOnPlatform && playerY <= platformY + platformHeight + 5));

            if (horizontalOverlap && playerAbovePlatform && playerNearPlatform && fallingOrStanding)
            {
                playerY = platformY + platformHeight;
                verticalSpeed = 0;
                isJumping = false;
                onPlatform = true;
                onLand = true;
                foundPlatform = true;
                break;
            }

            // Platform bottom/side collision
            bool hitFromBelow = (playerY + playerHeight > platformY) &&
                                (playerY < platformY + platformHeight) &&
                                (playerX + playerWidth - 20 > platformX + 5) &&
                                (playerX - 20 < platformX + platformWidth - 5) &&
                                (verticalSpeed > 0);

            bool hitFromSide = (playerY + playerHeight - 10 > platformY) &&
                               (playerY + 10 < platformY + platformHeight) &&
                               (((playerX + playerWidth - 20 > platformX) && (playerX + playerWidth < platformX + 20) && movingRight) ||
                                ((playerX + 20 < platformX + platformWidth) && (playerX + 20 > platformX + platformWidth - 20) && movingLeft));

            if (hitFromBelow)
            {
                playerY = platformY - playerHeight;
                verticalSpeed = -5;
            }

            if (hitFromSide && !foundPlatform)
            {
                if (movingRight)
                {
                    playerX = platformX - playerWidth;
                    if (worldX < WORLD_RIGHT_BOUNDARY)
                        worldX -= scrollSpeed;
                }
                else if (movingLeft)
                {
                    playerX = platformX + platformWidth;
                    if (worldX > WORLD_LEFT_BOUNDARY)
                        worldX += scrollSpeed;
                }
            }
        }

        // Edge jumping logic
        if (!foundPlatform && wasOnPlatform && playerY > 50 && playerY < 250)
        {
            for (int i = 0; i < numPlatforms; i++)
            {
                if (!platforms[i].active)
                    continue;

                int platformX = platforms[i].x - worldX;
                int platformY = platforms[i].y;
                int platformWidth = platforms[i].width;

                bool atLeftEdge = (playerX >= platformX - 20) && (playerX <= platformX + 5) &&
                                  (playerY >= platformY + 15) && (playerY <= platformY + 25);
                bool atRightEdge = (playerX + playerWidth >= platformX + platformWidth - 5) &&
                                   (playerX + playerWidth <= platformX + platformWidth + 20) &&
                                   (playerY >= platformY + 15) && (playerY <= platformY + 25);

                if (atLeftEdge || atRightEdge)
                {
                    onPlatform = true;
                    onLand = true;
                    break;
                }
            }

            if (!onPlatform)
            {
                onLand = false;
                if (!isJumping)
                {
                    isJumping = true;
                    verticalSpeed = -2;
                }
            }
        }
    }

    // Ground collision
    if (!onPlatform && playerY <= 50)
    {
        playerY = 50;
        verticalSpeed = 0;
        isJumping = false;
        onPlatform = false;
        onLand = true;
    }
}
void updatePlayerPosition()
{
    if (!moving || isJumping)
        return;

    if (movingRight)
    {
        if (worldX < WORLD_RIGHT_BOUNDARY)
        {
            worldX += scrollSpeed;
        }
        else if (playerX < screenWidth - playerWidth - 10)
        {
            playerX += scrollSpeed;
        }
    }
    else if (movingLeft)
    {
        if (worldX > WORLD_LEFT_BOUNDARY)
        {
            worldX -= scrollSpeed;
        }
        else if (playerX > 10)
        {
            playerX -= scrollSpeed;
        }
    }
}

void updateLevelCountdown()
{
    if (victoryTriggered || allDragonsDefeated)
        return;

    if (levelTimerRunning && levelTimerSeconds > 0)
    {
        levelTimerSeconds--;

        if (levelTimerSeconds == 0 && !bossTriggered)
        {
            if (!allDragonsDefeated)
            {
                bossTriggered = true;
                currentScreen = 5;
            }
        }
    }
}

// FIXED: Reset game function
void resetGameForNextLevel()
{
    currentDragonIndex = 0;
    allDragonsDefeated = false;
    bossTriggered = false;

    initDragon();

    levelTimerSeconds = 180;
    levelTimerRunning = true;

    // Reset enemies
    numGolems = 0;
    numBarbarians = 0;
    generateGolems();
    generateBarbarians();

    worldX += 2000; // Move forward more
    playerX = 100;

    if (playerHealth < 10)
        playerHealth = min(10, playerHealth + 3);
}

void updateVictoryState()
{
    if (victoryTriggered && victoryTimer > 0)
    {
        victoryTimer--;

        if (victoryTimer <= 0)
        {
            victoryTriggered = false;

            if (difficulty < 2)
            {
                difficulty++;
                resetGameForNextLevel();
            }
            else
            {
                currentScreen = 7; // Victory screen
            }
        }
    }
}

void drawVictoryScreen()
{
    iClear();
    iShowLoadedImage(0, 0, &highScoreBg);

    iSetColor(255, 215, 0);
    iText(screenWidth / 2 - 100, screenHeight / 2 + 50, "VICTORY!", GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(255, 255, 255);
    iText(screenWidth / 2 - 150, screenHeight / 2, "You have conquered all difficulties!", GLUT_BITMAP_HELVETICA_18);

    char finalScore[50];
    sprintf(finalScore, "Final Score: %d", playerScore);
    iText(screenWidth / 2 - 50, screenHeight / 2 - 30, finalScore, GLUT_BITMAP_HELVETICA_18);

    iText(screenWidth / 2 - 80, screenHeight / 2 - 60, "Press 'R' to play again", GLUT_BITMAP_HELVETICA_12);
    iText(screenWidth / 2 - 60, screenHeight / 2 - 80, "Press 'B' for menu", GLUT_BITMAP_HELVETICA_12);
}

void start()
{
    iClear();
    iStopSound(bgsongchannel);
    drawBackground();

    if (difficulty != 0)
        drawPlatforms();

    drawSprite();
    drawObstacles();
    drawstar();
    drawgolem();
    drawBarbarian();
    drawDragon();
    drawHealthBar();
    drawLeaves();
    drawHeart();
    spawnHeart();

    if (victoryTriggered)
    {
        iSetColor(255, 255, 0);
        char victoryMsg[100];
        sprintf(victoryMsg, "ALL DRAGONS DEFEATED! LEVEL COMPLETE!");
        iText(screenWidth / 2 - 150, screenHeight / 2, victoryMsg, GLUT_BITMAP_HELVETICA_18);

        iSetColor(255, 255, 255);
        iText(screenWidth / 2 - 100, screenHeight / 2 - 30, "Advancing to next level...", GLUT_BITMAP_HELVETICA_12);
    }
}

// FIXED: Sound and settings functions
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

    // Difficulty Buttons
    iSetColor(255, 255, 255);

    // Easy Button
    if (difficulty == 0)
        iSetColor(0, 255, 0);
    else
        iSetColor(200, 200, 200);
    iFilledRectangle(280, 140, 100, 40);
    iSetColor(0, 0, 0);
    iText(310, 155, "Easy", GLUT_BITMAP_HELVETICA_18);

    // Medium Button
    if (difficulty == 1)
        iSetColor(255, 255, 0);
    else
        iSetColor(200, 200, 200);
    iFilledRectangle(400, 140, 100, 40);
    iSetColor(0, 0, 0);
    iText(420, 155, "Medium", GLUT_BITMAP_HELVETICA_18);

    // Hard Button
    if (difficulty == 2)
        iSetColor(255, 0, 0);
    else
        iSetColor(200, 200, 200);
    iFilledRectangle(520, 140, 100, 40);
    iSetColor(0, 0, 0);
    iText(545, 155, "Hard", GLUT_BITMAP_HELVETICA_18);
}

void mouseSettings(int mx, int my)
{
    if (mx >= 269 && mx <= 349 && my >= 299 && my <= 379)
    {
        sound = !sound;
    }

    if (mx >= 277 && mx <= 342 && my >= 217 && my <= 282)
    {
        song = !song;
    }

    // Difficulty selection
    if (mx >= 280 && mx <= 380 && my >= 140 && my <= 180)
    {
        difficulty = 0; // Easy
    }
    else if (mx >= 400 && mx <= 500 && my >= 140 && my <= 180)
    {
        difficulty = 1; // Medium
    }
    else if (mx >= 520 && mx <= 620 && my >= 140 && my <= 180)
    {
        difficulty = 2; // Hard
    }
}

void about()
{
    iClear();
    iShowLoadedImage(0, 0, &aboutim);
    /* iSetColor(255, 255, 255);
     iText(screenWidth / 2 - 150, screenHeight / 2, "A small 2D game developed by Imtiaz and Niloy.", GLUT_BITMAP_HELVETICA_18);*/
}

void help()
{
    iClear();
    iShowLoadedImage(0, 0, &helpim);
    /* iSetColor(255, 255, 255);
     iText(screenWidth / 2 - 100, screenHeight / 2 + 50, "CONTROLS:", GLUT_BITMAP_HELVETICA_18);
     iText(screenWidth / 2 - 100, screenHeight / 2 + 20, "Arrow Keys: Move", GLUT_BITMAP_HELVETICA_12);
     iText(screenWidth / 2 - 100, screenHeight / 2 - 10, "Space: Throw Star", GLUT_BITMAP_HELVETICA_12);
     iText(screenWidth / 2 - 100, screenHeight / 2 - 40, "S: Switch World", GLUT_BITMAP_HELVETICA_12);
     iText(screenWidth / 2 - 100, screenHeight / 2 - 70, "R: Restart (Game Over)", GLUT_BITMAP_HELVETICA_12);
     iText(screenWidth / 2 - 100, screenHeight / 2 - 100, "B: Back to Menu", GLUT_BITMAP_HELVETICA_12);*/
}

void drawHighScoreBoard()
{
    /* iShowLoadedImage(0, 0, &highScoreBg);

     iSetColor(255, 255, 255);
     iText(400, 550, "Top 10 High Scores", GLUT_BITMAP_HELVETICA_18);

     for (int i = 0; i < numHighScores; i++)
     {
         char entry[150];
         sprintf(entry, "%d. %s - %d", i + 1, highScores[i].name, highScores[i].score);
         iText(350, 500 - i * 30, entry, GLUT_BITMAP_HELVETICA_18);
     }*/
    // Background
    iShowLoadedImage(0, 0, &highScoreBg);

    // Title
    iSetColor(0, 0, 0);
    iText(362, 552, "TOP 10 HIGH SCORES", GLUT_BITMAP_TIMES_ROMAN_24); // shadow
    iSetColor(255, 215, 0);
    iText(360, 550, "TOP 10 HIGH SCORES", GLUT_BITMAP_TIMES_ROMAN_24); // text

    // Entries
    for (int i = 0; i < numHighScores; i++)
    {
        char entry[150];
        sprintf(entry, "%d. %s - %d", i + 1, highScores[i].name, highScores[i].score);

        // Shadow
        iSetColor(50, 50, 50);
        iText(332, 502 - i * 35, entry, GLUT_BITMAP_TIMES_ROMAN_24);

        // Main text
        iSetColor(255, 255, 255);
        iText(330, 500 - i * 35, entry, GLUT_BITMAP_TIMES_ROMAN_24);
    }
}

void drawGameOver()
{

    iPauseSound(bgsongchannel);
    iPauseSound(DragonSoundChannel);
    if (!endSoundPlayed)
    {
        insertScore(playerName, playerScore);
        if (sound)
        {
            iStopSound(-1);
            iPlaySound("saves/assets/sounds/end.wav", 0, soundvolume);
        }
        endSoundPlayed = true;
        loadHighScores();
    }
    levelTimerRunning = false;
    iShowLoadedImage(0, 0, &endImage);

    char nameText[100];
    sprintf(nameText, "%s", playerName);
    iSetColor(252, 5, 13);
    iTextAdvanced(260, 311, nameText, 0.2, 2, GLUT_STROKE_ROMAN);

    char scoreText[100];
    sprintf(scoreText, "%d", playerScore);
    iSetColor(252, 5, 13);
    iTextAdvanced(260, 165, scoreText, 0.2, 2, GLUT_STROKE_ROMAN);

    iSetColor(255, 255, 255);
    iText(900, 50, "Press 'R' to Restart", GLUT_BITMAP_HELVETICA_10);
}

void drawNameEntry()
{
    iClear();
    iShowLoadedImage(0, 0, &nameEntryImage);
    iSetColor(255, 255, 255);
    iText(420, 254, playerName, GLUT_BITMAP_TIMES_ROMAN_24);
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
        else if (enteringName)
        {
            drawNameEntry();
        }
        else if (currentScreen == 1)
        {
            start();
            if (!levelTimerRunning)
            {
                levelTimerSeconds = 180;
                levelTimerRunning = true;
                bossTriggered = false;
            }
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
            drawGameOver();
            levelTimerRunning = false;
        }
        else if (currentScreen == 6)
        {
            drawHighScoreBoard();
        }
        else if (currentScreen == 7)
        {
            drawVictoryScreen();
        }
    }

    // Reset moveTimer if no movement
    if (!movingRight && !movingLeft)
    {
        moveTimer = 0;
    }
}

// FIXED: Complete reset game function
void resetGame()
{
    if (endSoundPlayed)
    {
        iStopSound(collisionSoundChannel);
        iStopSound(bgsongchannel);
    }

    // Reset all game state
    playerHealth = 10;
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

    // Reset dragon/victory state
    allDragonsDefeated = false;
    victoryTriggered = false;
    victoryTimer = 0;
    currentDragonIndex = 0;
    bossTriggered = false;
    DragonFacingLeft = true;
    DragonFacingRight = false;

    // Reset enemies
    numGolems = 0;
    numBarbarians = 0;
    golemSpawnTimer = 0;
    barbarianSpawnTimer = 0;
    nextGolemX = screenWidth + 300;
    nextBarbarianX = nextGolemX + 500;

    // Initialize all stars as inactive
    for (int i = 0; i < MAX_STARS; i++)
    {
        stars[i].active = false;
    }

    for (int i = 0; i < MAX_GOLEMS; i++)
    {
        golems[i].active = false;
    }
    for (int i = 0; i < MAX_BARBARIANS; i++)
    {
        barbarians[i].active = false;
    }
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        obstaclePassed[i] = false;
    }

    generateGolems();
    generateBarbarians();
    generateObstacles();
    initDragon();
    initPlatforms(); // FIXED: Reinitialize platforms
    drawHeart();
    spawnHeart();

    levelTimerSeconds = 180;
    levelTimerRunning = true;
    // iResumeSound(bgsongchannel);
    // iResumeSound(DragonSoundChannel);
}

void iMouseMove(int mx, int my)
{
    // place your codes here
}

void iMouseDrag(int mx, int my)
{
    if (currentScreen == 3)
    {
        if (mx >= soundknobx && mx <= soundknobx + 14 && my >= soundknoby && my <= soundknoby + 14)
        {
            soundknobclicked = true;
            songknobclicked = false;
            soundknobx = max(368, min(639, mx - 7));
            soundvolume = (soundknobx - 368 + 7) * 100 / (639 - 368); // FIXED: Volume calculation
        }
        else if (mx >= songknobx && mx <= songknobx + 14 && my >= songknoby && my <= songknoby + 14)
        {
            songknobclicked = true;
            soundknobclicked = false;
            songknobx = max(368, min(639, mx - 7));
            songvolume = (songknobx - 368 + 7) * 100 / (639 - 368); // FIXED: Volume calculation
        }
        else
        {
            soundknobclicked = false;
            songknobclicked = false;
        }
    }
}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (!isPlayButtonClicked)
        {
            int centerX = (screenWidth / 4) * 3;
            int centerY = (screenHeight / 4);
            int radius = 40;
            int dx = mx - centerX;
            int dy = my - centerY;
            if (dx * dx + dy * dy <= radius * radius)
            {
                isPlayButtonClicked = true;
                if (sound)
                {   
                    iStopSound(-1);
                    iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
                }
            }
        }
        else if (isPlayButtonClicked && currentScreen == 0)
        {
            if (mouseNavigationBar(mx, my))
            {
                if (sound)
                {
                    iStopSound(-1);
                    iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
                }
            }
        }
        else if (currentScreen == 3)
        {
            mouseSettings(mx, my);
        }
    }
}

void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}
void throwStar()
{
    isthrowingstar = true;
    for (int i = 0; i < MAX_STARS; i++)
    {
        if (!stars[i].active)
        {
            stars[i].active = true;
            stars[i].x = playerX + (starthrowdirection ? playerWidth : 0);
            stars[i].y = playerY + playerHeight / 2 - 10;
            if (movingRight)
            {
                if (stars[i].directionLeft)
                {
                    iMirrorSprite(&throwingStarsprite[i], HORIZONTAL);
                    stars[i].directionLeft = false;
                    stars[i].directionRight = true;
                }
                iSetSpritePosition(&throwingStarsprite[i], stars[i].x, stars[i].y);
            }
            else if (movingLeft)
            {
                if (stars[i].directionRight)
                {
                    iMirrorSprite(&throwingStarsprite[i], HORIZONTAL);
                    stars[i].directionRight = false;
                    stars[i].directionLeft = true;
                }
                iSetSpritePosition(&throwingStarsprite[i], stars[i].x, stars[i].y);
            }
            break;
        }
    }
}

// FIXED: Keyboard input handling
void iKeyPress(unsigned char key)
{
    if (enteringName)
    {
        if (key == '\r') // Enter key pressed
        {
            enteringName = false;
            currentScreen = 1; // Go to game screen
            return;
        }
        else if (key == '\b') // Backspace
        {
            if (nameCharIndex > 0)
            {
                nameCharIndex--;
                playerName[nameCharIndex] = '\0';
            }
        }
        else if (nameCharIndex < 99 && key >= 32 && key <= 126) // FIXED: Only printable characters
        {
            playerName[nameCharIndex++] = key;
            playerName[nameCharIndex] = '\0';
        }
        return;
    }

    switch (key)
    {
    case 's':
    case 'S':
        worldSwitched = !worldSwitched;
        if (worldSwitched)
        {
            scrollSpeed = 10;
        }
        else
        {
            scrollSpeed = 7;
        }
        initPlatforms(); // FIXED: Reinitialize platforms when switching worlds
        break;

    case 'b':
    case 'B':
        currentScreen = 0;
        endSoundPlayed = false;
        break;

    case 'r':
    case 'R':
        if (currentScreen == 5 || currentScreen == 7) // Game over or victory screen
        {
            resetGame();
        }
        break;

    case ' ':
        if (currentScreen == 1) // Only in game screen
        {

            if (sound)
            {
                iStopSound(-1);
                iPlaySound("saves/assets/sounds/star.wav", 0, soundvolume);
            }
            throwStar();
        }
        break;

    default:
        break;
    }
}

bool ninjaFacingRight = true;

void iSpecialKeyPress(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        starthrowdirection = false;

        if (movingRight)
        {
            iMirrorSprite(&playerSprite, HORIZONTAL);
            iMirrorSprite(&starthrow, HORIZONTAL);
        }

        movingLeft = true;
        movingRight = false;
        moving = true;

        break;

    case GLUT_KEY_RIGHT:
        starthrowdirection = true;

        if (movingLeft)
        {
            iMirrorSprite(&playerSprite, HORIZONTAL);
            iMirrorSprite(&starthrow, HORIZONTAL);
        }

        movingRight = true;
        movingLeft = false;
        moving = true;

        break;

    case GLUT_KEY_UP:

        if (!isJumping && onLand)
        {
            isJumping = true;
            verticalSpeed = jumpSpeed;
            onLand = false;
        }
        break;

    case GLUT_KEY_HOME:
        currentScreen = 0;
        break;

    default:
        break;
    }
}

// FIXED: Key release detection
void iSpecialKeyRelease(unsigned char key)
{
    if (key == GLUT_KEY_LEFT)
    {
        moving = false;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        moving = false;
    }
}

void initLeaves()
{
    for (int i = 0; i < MAX_LEAVES; i++)
    {
        leaves[i].x = rand() % screenWidth;
        leaves[i].y = rand() % screenHeight + screenHeight;
        leaves[i].speed = rand() % 3 + 1;
        leaves[i].active = true;
    }
}

void hitCoolDown()
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
}

void playerScores()
{
    if (currentScreen == 1 && playerHealth > 0)
    {
        // Additional scoring logic can go here
    }
}

void iTimer1()
{
    if (currentScreen == 1) // Only update game logic when in game screen
    {
        // checkKeyReleases();
        updateLeaves();
        updateHeart();
        checkHeartCollision();
        spawnHeart();
        hitCoolDown();
        updatestar();

        checkBarbarianCollision();
        checkStarBarbarianCollision();
        checkStarGolemCollision();
        checkGolemCollision();
        updateDragonAttack();
        updateDragonPosition();
        checkDragonFireCollision();
        checkStarDragonCollision();
        checkPlayerDragonCollision();
        updateVictoryState();
        checkObstacleCollision();
        // iSetTimer(25, updateHeart); // MUST EXIST
    }
    playBGSong(); // Always handle background music
}
void iTimer2()
{
    if (currentScreen == 1)
    {
        animategolem();
        animatePlayer();
        animatestarthrow();
        animateBarbarian();
        animateDragon();
        updatePlayerPosition();
    }
}
void iTimer3()
{
    if (currentScreen == 1)
    {
        updateGolems();
        updatePlayer();
        updatePlayerPosition();
        updateBarbarians();
    }
}
// FIXED: Main function
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    // Initialize all arrays to prevent garbage values
    for (int i = 0; i < MAX_STARS; i++)
    {
        stars[i].active = false;
        stars[i].x = 0;
        stars[i].y = 0;
    }

    for (int i = 0; i < MAX_GOLEMS; i++)
    {
        golems[i].active = false;
    }

    for (int i = 0; i < MAX_BARBARIANS; i++)
    {
        barbarians[i].active = false;
    }
    load_Images();
    generateObstacles();
    generateGolems();
    generateBarbarians();
    loadHighScores();
    initLeaves();
    spawnHeart();
    initDragon();
    initPlatforms();

    iSetTimer(30, playerScores);
    iSetTimer(16, iTimer1);
    iSetTimer(100, iTimer2);
    iSetTimer(30, iTimer3);
    iSetTimer(20000, spawnHeart);
    iSetTimer(1000, updateLevelCountdown);
    iSetTimer(16, updateVictoryState);
    // 20 seconds = 20000 ms
    // iSetTimer(25, updateHeart); // MUST EXIST
    iInitializeSound();
    playBGSong();
    iInitialize(screenWidth, screenHeight, "Maze of Shadows");
    return 0;
}