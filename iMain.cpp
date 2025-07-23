#include <math.h>
#include "iGraphics.h"
#include "iSound.h"

int screenWidth = 1000, screenHeight = 600;
int worldScreenCount = 5;
int worldWidth = worldScreenCount * screenWidth, worldX = 0;
bool isJumping = false;
int jumpSpeed = 25, gravity = -2, verticalSpeed = 0;
bool isPlayButtonClicked = false;

Image heartImg;

int navY = 490;
int buttonWidth = 150;
int buttonHeight = 52;
int currentScreen = 0;

int levelTimerSeconds = 10; // 3 minutes
bool levelTimerRunning = false;
bool bossTriggered = false;

int playerX = 500, playerY = 50, playerWidth = 55, playerHeight = 100;

Image fire, fire1, log1, tree, tree1_png;
Image log1_switched, tree1_switched, fire1_switched;

Image highObstacle;
Image nameEntryImage;
Image navBg;
Image highScoreBg;

#define NUM_OBSTACLES 30
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
int nextBarbarianX = nextGolemX + 500;

char playerName[100] = "";
bool enteringName = false;
int nameCharIndex = 0;

int obstacleX[NUM_OBSTACLES];
int obstacleY[NUM_OBSTACLES];
int collisionSoundChannel = -1;
Image endImage;
bool endSoundPlayed = false;

int playerHealth = 5;
bool hitCooldown = false;
int hitCooldownTimer = 0;
const int HIT_COOLDOWN_LIMIT = 30; // frames to wait before next hit allowed

Image bg1;
bool worldSwitched = false;
int scrollSpeed = 10; // default speed
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
{ // Golem
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

typedef struct
{ // Barbarian
    int x, y;
    int speed;
    bool active = true;
    int health;
} Barbarian;

Sprite barbarianSprite;
Image barbarianImage[12];

bool barbarianRight = true;
bool barbarianLeft = false;

#define BARBARIAN_SPAWN_INTERVAL 650
#define MAX_BARBARIANS 10
#define BARBARIAN_SPAWN_DELAY 150

Barbarian barbarians[MAX_BARBARIANS];
int numBarbarians = 0;

#define DRAGON_FLY_FRAMES 6
#define DRAGON_FIRE_FRAMES 6

// Dragon....................................
#define DRAGON_FLY_FRAMES 6
#define DRAGON_FIRE_FRAMES 6
#define DRAGON_MAX_HEALTH 3

Image dragonFly[DRAGON_FLY_FRAMES];
Image dragonFire[DRAGON_FIRE_FRAMES];
Sprite dragonSprite;
bool dragonIsFiring = false;

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

Dragon dragon;
bool DragonFacingRight = true;
bool DragonFacingLeft = false;
//....................................Dragon

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
    iLoadImage(&nameEntryImage, "saves/assets/images/namescreen.jpg");
    iResizeImage(&nameEntryImage, 1000, 600);
    iLoadImage(&leafImg, "saves/assets/images/leaf.png");
    iResizeImage(&leafImg, 20, 20);
    iLoadImage(&heartImg, "saves/assets/images/heart.png");
    iResizeImage(&heartImg, 30, 30);

    iLoadImage(&highScoreBg, "saves/assets/images/CleanBG.png");
    iResizeImage(&highScoreBg, 1000, 600);
    //iLoadImage(&highObstacle, "saves/assets/images/obstacles/highobs.png");
    //iResizeImage(&highObstacle, 80, 40); // wider and less tall

    iLoadImage(&fire1, "saves/assets/images/obstacles/fire1.png");
    iResizeImage(&fire1, 100, 80);

    iLoadImage(&log1, "saves/assets/images/obstacles/log1.png");
    iResizeImage(&log1, 100, 80);

    iLoadImage(&tree1_png, "saves/assets/images/obstacles/obs2.png");

    iResizeImage(&tree1_png, 100, 80);

    iLoadImage(&log1_switched, "saves/assets/images/obstacles/ship.png");
    iResizeImage(&log1_switched, 120, 100);

    iLoadImage(&tree1_switched, "saves/assets/images/obstacles/dead_tree.png");
    iResizeImage(&tree1_switched, 120, 100);

    iLoadImage(&fire1_switched, "saves/assets/images/obstacles/lava.png");
    iResizeImage(&fire1_switched, 120, 100);

    iLoadImage(&navBg, "saves/assets/images/ninja.png");
    iResizeImage(&navBg, 1000, 600);

    iLoadImage(&bg1, "saves/assets/images/MoSpos1.png");
    iResizeImage(&bg1, 1000, 600);

    iLoadImage(&throwingStar, "saves/assets/images/starthrowing.png");
    iResizeImage(&throwingStar, 20, 20);

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
    iLoadImage(&bg2, "saves/assets/images/backimg4.png"); // Use another background image
    iResizeImage(&bg2, 1000, 600);

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

    iLoadFramesFromFolder(barbarianImage, "saves/assets/images/sprites/barbarians/RunThrowing2");
    iInitSprite(&barbarianSprite, -1);
    iChangeSpriteFrames(&barbarianSprite, barbarianImage, 12);
    iResizeSprite(&barbarianSprite, playerWidth, playerHeight);

    iLoadFramesFromFolder(dragonFly, "saves/assets/images/sprites/Dragonflying");
    iLoadFramesFromFolder(dragonFire, "saves/assets/images/sprites/Dragonfiresplit");
    iInitSprite(&dragonSprite, -1);
    iChangeSpriteFrames(&dragonSprite, dragonFly, DRAGON_FLY_FRAMES);
    iResizeSprite(&dragonSprite, 250, 200);
}

void generateObstacles()
{
    int spacing = 350;

    for (int i = 0; i < NUM_OBSTACLES; ++i)
    {
        obstacleX[i] = 300 + i * spacing;
        obstacleY[i] = 30;
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

void drawHealthBar()
{
    int heartX = 20;
    int heartY = screenHeight - 50;

    for (int i = 0; i < playerHealth; i++)
    {
        iShowLoadedImage(heartX + i * 35, heartY, &heartImg); // spacing = 35
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

// GolemCode========================================================================================================================
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

void spawnGolemIfNeeded()
{
    if (numGolems >= MAX_GOLEMS)
        return;

    if (golemSpawnTimer >= GOLEM_SPAWN_DELAY)
    {
        golems[numGolems].x = nextGolemX;
        golems[numGolems].y = 45;
        golems[numGolems].speed = 2;
        golems[numGolems].health = 1;
        golems[numGolems].active = true;

        numGolems++;
        golemSpawnTimer = 0;

        nextGolemX += 700 + rand() % 300;
    }
    else
    {
        golemSpawnTimer++;
    }
}
//======================================================================================================================GolemCode

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
                    playerScore += 100; // Barbarian marle bonus
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

void spawnBarbarianIfNeeded()
{
    if (numBarbarians >= MAX_BARBARIANS)
        return;

    if (barbarianSpawnTimer >= BARBARIAN_SPAWN_DELAY)
    {
        barbarians[numBarbarians].x = nextBarbarianX;
        barbarians[numBarbarians].y = 45;
        barbarians[numBarbarians].speed = 2;
        barbarians[numBarbarians].health = 1;
        barbarians[numBarbarians].active = true;

        numBarbarians++;
        barbarianSpawnTimer = 0;

        nextBarbarianX += 700 + rand() % 300;
    }
    else
    {
        barbarianSpawnTimer++;
    }
}

// DragonCode======================================================================================================================
void initDragon()
{
    dragon.x = 3000;
    dragon.y = 250;
    dragon.health = DRAGON_MAX_HEALTH;
    dragon.active = true;
    dragon.isSpeaking = true;
    dragon.isFiring = false;
    dragon.fireCooldown = 200;
    dragon.fireDuration = 50;
    dragonIsFiring = false;
    dragon.speed = 2;
    iChangeSpriteFrames(&dragonSprite, dragonFly, DRAGON_FLY_FRAMES);
}

void drawDragon()
{
    if (!dragon.active)
        return;

    int dx = dragon.x - worldX;
    iSetSpritePosition(&dragonSprite, dx, dragon.y);
    iShowSprite(&dragonSprite);

    if (dragon.isSpeaking && dragon.x - worldX < 800)
    {
        iText(dx - 20, dragon.y + 185, "You dare approach me?!!!!", GLUT_BITMAP_TIMES_ROMAN_24);
        static int speechTimer = 200;
        speechTimer--;
        if (speechTimer <= 0)
        {
            dragon.isSpeaking = false;
        }
    }
    int barX = dragon.x - worldX + 25;
    int barY = dragon.y + 160;
    int barWidth = 100;
    int barHeight = 12;

    float healthRatio = (float)dragon.health / DRAGON_MAX_HEALTH;

    iSetColor(100, 100, 100);
    iFilledRectangle(barX, barY, barWidth, barHeight);

    iSetColor(255, 0, 0);
    iFilledRectangle(barX, barY, barWidth * healthRatio, barHeight);
}
void updateDragonPosition()
{
    if (!dragon.active)
        return;

    static int hoverTimer = 0;
    hoverTimer++;
    dragon.y += (hoverTimer % 120 < 60) ? -3 : 3;

    int px = playerX + worldX;
    int dx = dragon.x;
    if (dx > px && dx - px <= 250)
    {
        if (DragonFacingRight)
        {
            iMirrorSprite(&dragonSprite, HORIZONTAL);
            DragonFacingRight = false;
            DragonFacingLeft = true;
        }
        dragon.x -= dragon.speed;
    }
    else if (px > dx && px - dx <= 250)
    {
        if (DragonFacingLeft)
        {
            iMirrorSprite(&dragonSprite, HORIZONTAL);
            DragonFacingLeft = false;
            DragonFacingRight = true;
        }
        dragon.x += dragon.speed;
    }
    else if (px == dx)
    {
        dragon.x += 0;
    }
}

void updateDragonAttack()
{
    if (!dragon.active)
        return;

    if (dragon.fireCooldown > 0)
    {
        dragon.fireCooldown--;
    }
    else
    {
        dragon.isFiring = true;
        dragon.fireDuration--;

        if (!dragonIsFiring)
        {
            iChangeSpriteFrames(&dragonSprite, dragonFire, DRAGON_FIRE_FRAMES);
            dragonIsFiring = true;
        }

        if (dragon.fireDuration <= 0)
        {
            dragon.isFiring = false;
            dragon.fireCooldown = 150 + rand() % 100;
            dragon.fireDuration = 60;

            if (dragonIsFiring)
            {
                iChangeSpriteFrames(&dragonSprite, dragonFly, DRAGON_FLY_FRAMES);
                dragonIsFiring = false;
            }
        }
    }
}

void checkDragonFireCollision()
{
    if (!dragon.active || !dragon.isFiring || hitCooldown)
        return;

    int fx = dragon.x - worldX + 40;
    int fw = 10 + (60 - dragon.fireDuration) * 2;

    if (playerX + playerWidth >= fx && playerX <= fx + fw && playerY <= dragon.y)
    {
        playerHealth--;
        hitCooldown = true;

        if (sound)
        {
            iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
        }

        if (playerHealth <= 0)
        {
            currentScreen = 5;
        }
    }
}
void checkPlayerDragonCollision()
{
    if (!dragon.active || hitCooldown)
        return;

    int dx = dragon.x - worldX;
    int dy = dragon.y;

    int dragonWidth = 150;
    int dragonHeight = 100;

    if (playerX + playerWidth > dx && playerX < dx + dragonWidth && playerY + playerHeight > dy && playerY < dy + dragonHeight)
    {
        playerHealth--;
        hitCooldown = true;

        if (sound)
        {
            iPlaySound("saves/assets/sounds/background.wav", 0, soundvolume);
        }

        if (playerHealth <= 0)
        {
            currentScreen = 5;
        }
    }
}

void checkStarDragonCollision()
{
    if (!dragon.active)
        return;

    for (int i = 0; i < MAX_STARS; i++)
    {
        if (!stars[i].active)
            continue;

        int dx = dragon.x - worldX;
        if (stars[i].x + 20 > dx && stars[i].x < dx + 150 && stars[i].y + 20 > dragon.y && stars[i].y < dragon.y + 150)
        {
            dragon.health--;
            stars[i].active = false;

            if (dragon.health <= 0)
            {
                dragon.active = false;
                playerScore += 1000; // Bonus after killing a dragon................
            }
        }
    }
}
void animateDragon()
{
    iAnimateSprite(&dragonSprite);
}
//=========================================================================================================================DragonCode

void loadHighScores()
{
    FILE *file = fopen("saves/data/scores.txt", "r");
    if (file == NULL)
        return;

    numHighScores = 0;
    while (fscanf(file, "%[^,],%d\n", highScores[numHighScores].name, &highScores[numHighScores].score) == 2)
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

    for (int i = 0; i <= numHighScores; i++)
    {
        if (i == numHighScores || score > highScores[i].score)
        {
            for (int j = MAX_HIGH_SCORES - 1; j > i; j--)
            {
                highScores[j] = highScores[j - 1];
            }
            strncpy(highScores[i].name, name, sizeof(highScores[i].name));
            highScores[i].score = score;
            if (numHighScores < MAX_HIGH_SCORES)
                numHighScores++;
            break;
        }
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

void drawBackground()
{
    if (worldSwitched)
        iShowLoadedImage(0, 0, &bg2);
    else
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
    drawstar();
    drawgolem();
    drawBarbarian();
    drawDragon();
    drawHealthBar();
    drawLeaves();
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        obstaclePassed[i] = false;
    }



}

void updatePlayer()
{
    //<This part for falling><and this part for jumping>
    if (playerY > 50 || verticalSpeed > 0)
    {
        if (movingRight)
        {
            if (worldX < worldWidth - 10)
            {
                worldX += scrollSpeed;
            }
            if (worldX > 0 && worldX < worldWidth - 1000)
            {
                iWrapImage(&bg, -5);
            }
            else if ((worldX <= 0 && worldX >= -490) || (worldX >= worldWidth - 1000 && worldX <= worldWidth - 10))
            {
                playerX += scrollSpeed;
            }
            else
            {
                playerX += 0;
            }
        }
        else if (movingLeft)
        {
            if (worldX > -490)
            {
                worldX -= scrollSpeed;
            }
            if (worldX > 0 && worldX < worldWidth - 1000)
            {
                iWrapImage(&bg, 5);
            }
            else if ((worldX <= 0 && worldX >= -490) || (worldX >= worldWidth - 1000 && worldX <= worldWidth - 10))
            {
                playerX -= scrollSpeed;
            }
            else
            {
                playerX += 0;
            }
        }
        playerY += verticalSpeed;
        verticalSpeed += gravity;
    }
    /*int obsWidth = 80, obsHeight = 80;
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        // Check if obstacle has moved past the player
        if (!obstaclePassed[i] && playerX > obstacleX[i] + obsWidth) {
            playerScore += 50;
            obstaclePassed[i] = true;
        }
    }*/
    /*int playerWorldX = playerX + worldX;

    for (int i = 0; i < NUM_OBSTACLES; i++) {
        int obsRightEdge = obstacleX[i] + 80;

        if (!obstaclePassed[i] && playerWorldX > obsRightEdge + 10) {
            playerScore += 50;
            obstaclePassed[i] = true;
        }
    }*/

    // Check for collision only when not jumping
    if (!isJumping && !hitCooldown)
    {
        for (int i = 0; i < NUM_OBSTACLES; i++)
        {
            int drawX = obstacleX[i] - worldX;
            int obsWidth = 80, obsHeight = 80;
            // int playerWorldX = playerX + worldX;
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
    if (isJumping && !hitCooldown)
        playerScore += 5;

    if (playerY <= 50)
    {
        playerY = 50;
        isJumping = false;
        verticalSpeed = 0;
    }
}

void updateLevelCountdown()
{
    if (levelTimerRunning && levelTimerSeconds > 0)
    {
        levelTimerSeconds--;

        if (levelTimerSeconds == 0 && !bossTriggered)
        {
            // Time's up! Trigger the boss
            bossTriggered = true;
            currentScreen=5;
            initDragon(); // call your existing dragon spawn/init
        }
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
    iText(screenWidth / 2 - 100, screenHeight / 2, "A small 2d game developed by Imtiaz and Niloy.", GLUT_BITMAP_HELVETICA_18);
}

void help()
{
    iClear();
    iSetColor(255, 255, 255);
    iText(screenWidth / 2 - 100, screenHeight / 2, "Help Screen", GLUT_BITMAP_HELVETICA_18);
}
void drawHighScoreBoard()
{
    iShowLoadedImage(0, 0, &highScoreBg);

    iSetColor(255, 255, 255);
    iText(400, 550, "Top 10 High Scores", GLUT_BITMAP_HELVETICA_18);

    for (int i = 0; i < numHighScores; i++)
    {
        char entry[150];
        sprintf(entry, "%d. %s - %d", i + 1, highScores[i].name, highScores[i].score);
        iText(350, 500 - i * 30, entry, GLUT_BITMAP_HELVETICA_18);
    }
}
void drawGameOver()
{

    if (!endSoundPlayed)
    {
        insertScore(playerName, playerScore);
        iPlaySound("saves/assets/sounds/gameover.wav", 0, soundvolume);
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
    iText(900, 10, "Press 'R' to Restart", GLUT_BITMAP_HELVETICA_10);
}
void drawNameEntry()
{
    iClear();
    iShowLoadedImage(0, 0, &nameEntryImage);
    iSetColor(255, 255, 255);
    // iText(300, 350, "Enter Your Name:", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(420, 254, playerName, GLUT_BITMAP_TIMES_ROMAN_24);
    return;
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
    }

    // Optional: if no movement keys pressed, reset moveTimer to avoid animation stuck
    if (!movingRight && !movingLeft)
    {
        moveTimer = 0;
    }
}
void resetGame()
{
    if (endSoundPlayed)
    {
        iStopSound(collisionSoundChannel);
        iStopSound(bgsongchannel);
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

    for (int i = 0; i < MAX_GOLEMS; i++)
    {
        golems[i].active = false;
    }
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        obstaclePassed[i] = false;
    }

    generateGolems();
    initDragon();
    levelTimerSeconds = 180;
            levelTimerRunning = true;
            bossTriggered = false;
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

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if (enteringName)
    {
        if (key == '\r') // Enter key pressed
        {
            enteringName = false;
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
        else if (nameCharIndex < 99)
        {
            playerName[nameCharIndex++] = key;
            playerName[nameCharIndex] = '\0';
        }
        return;
    }
    if (key == 's' || key == 'S')
    {
        worldSwitched = !worldSwitched; // Toggle world

        if (worldSwitched)
        {
            scrollSpeed = 12; // Increase speed in new world
        }
        else
        {
            scrollSpeed = 10; // Default speed
        }
    }
    // Add this block for 'b' key to go back to navigation bar
    if (key == 'b' || key == 'B')
    {
        currentScreen = 0;
        endSoundPlayed = false; // Optional: reset game over sound
        return;
    }

    // Restart game if on game over screen and 'r' is pressed
    if ((currentScreen == 5) && (key == 'r' || key == 'R'))
    {
        resetGame();
        currentScreen = 1; // Go back to game screen
        return;
    }

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
                if (worldX > 0 && worldX < worldWidth - 1000)
                {
                    iWrapImage(&bg, 5);
                }
                else if ((worldX <= 0 && worldX >= -490) || (worldX >= worldWidth - 1000 && worldX <= worldWidth - 10))
                {
                    playerX -= 5;
                }
                else
                {
                    playerX += 0;
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
                if (worldX < worldWidth - 10)
                {
                    worldX += 5;
                }
                if (worldX > 0 && worldX < worldWidth - 1000)
                {
                    iWrapImage(&bg, -5);
                }
                else if ((worldX <= 0 && worldX >= -490) || (worldX >= worldWidth - 1000 && worldX <= worldWidth - 10))
                {
                    playerX += 5;
                }
                else
                {
                    playerX += 0;
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
        // playerScore++;
    }
}

void iTimer()
{
    updateLeaves();
    hitCoolDown();
    updatePlayer();
    updatestar();
    updateBarbarians();
    checkBarbarianCollision();
    checkStarBarbarianCollision();
    checkStarGolemCollision();
    checkGolemCollision();
    updateDragonAttack();
    updateDragonPosition();
    checkDragonFireCollision();
    checkStarDragonCollision();
    checkPlayerDragonCollision();
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
    loadHighScores();
    initLeaves();
    initDragon();
    iSetTimer(30, playerScores);
    iSetTimer(100, updateGolems);
    iSetTimer(80, animategolem);
    iSetTimer(120, animatePlayer);
    iSetTimer(60, animatestarthrow);
    iSetTimer(20, iTimer);
    iSetTimer(120, spawnBarbarianIfNeeded);
    iSetTimer(120, animateBarbarian);
    iSetTimer(120, animateDragon);
    iSetTimer(1000, updateLevelCountdown);

    iInitializeSound();
    playBGSong();
    iInitialize(screenWidth, screenHeight, "Maze of Shadows");
    return 0;
}