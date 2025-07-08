#include <math.h>
#include "iGraphics.h"
#include "iSound.h"
Image fire, fire1, log1, tree, tree1_png;
<<<<<<< HEAD
Image nameEntryImage;
Image navBg;
Image highScoreBg;

#define NUM_OBSTACLES 30 
int playerScore = 0;
typedef struct
{
=======
Image nameEntryImage;Image navBg; 
Image highScoreBg;
// global at top

#define NUM_OBSTACLES 30 // Adjust based on worldWidth
int playerScore = 0;
typedef struct {
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
    char name[100];
    int score;
} HighScore;

#define MAX_HIGH_SCORES 10
HighScore highScores[MAX_HIGH_SCORES];
int numHighScores = 0;

<<<<<<< HEAD
=======



>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
char playerName[100] = "";
bool enteringName = false;
int nameCharIndex = 0;

<<<<<<< HEAD
=======

>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
int obstacleX[NUM_OBSTACLES];
int obstacleY[NUM_OBSTACLES];
int collisionSoundChannel = -1;
Image endImage;
bool endSoundPlayed = false;
<<<<<<< HEAD
=======
//check
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa

int playerHealth = 3;
bool hitCooldown = false;
int hitCooldownTimer = 0;
<<<<<<< HEAD
const int HIT_COOLDOWN_LIMIT = 30; 
=======
const int HIT_COOLDOWN_LIMIT = 30; // frames to wait before next hit allowed
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa

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
<<<<<<< HEAD
{ // Golem
=======
{ // Golem 
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
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

<<<<<<< HEAD
=======

>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
Image settingsmenu, soundon, soundoff, songon;
Image songoff;
Image knob;
bool soundknobclicked = false, songknobclicked = false;
int soundknobx = 643;
int soundknoby = 333;
int songknobx = 643;
int songknoby = 245;
<<<<<<< HEAD
int soundvolume = 50, songvolume = 50;
int bgsongchannel = -1;
int lastSongVolume = -1;
bool wasSongOn = false;
=======
int soundvolume = 50,songvolume = 50;
int bgsongchannel = -1;
int lastSongVolume = -1; 
bool wasSongOn = false;  


>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa

bool sound = true, song = true;

int worldScreenCount = 3;
int worldWidth = worldScreenCount * screenWidth, worldX = 0;

bool isJumping = false;
int jumpSpeed = 20, gravity = -2, verticalSpeed = 0;

<<<<<<< HEAD
=======


>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
bool isPlayButtonClicked = false;
void drawRoad()
{
    int roadHeight = 50;
<<<<<<< HEAD
    iSetColor(60, 60, 60);
    iFilledRectangle(0, 0, screenWidth, roadHeight);
}

void load_Images()
{

    iLoadImage(&nameEntryImage, "saves/assets/images/namescreen.jpg"); 
    iResizeImage(&nameEntryImage, 900, 600);                           
    iLoadImage(&leafImg, "saves/assets/images/leaf.png");
    iResizeImage(&leafImg, 20, 20);

    iLoadImage(&highScoreBg, "saves/assets/images/highscore.jpg");
    iResizeImage(&highScoreBg, 1000, 600);

    iLoadImage(&fire1, "saves/assets/images/obstacles/fire1.png");
    iResizeImage(&fire1, 80, 80);

    iLoadImage(&log1, "saves/assets/images/obstacles/log1.png");
    iResizeImage(&log1, 80, 80);

    iLoadImage(&tree1_png, "saves/assets/images/obstacles/tree1.png");
    iResizeImage(&tree1_png, 80, 80);

    iLoadImage(&navBg, "saves/assets/images/ninja.png");
    iResizeImage(&navBg, 1000, 600);

    iLoadImage(&bg1, "saves/assets/images/start.png");
    iResizeImage(&bg1, 1000, 600);

    iLoadImage(&throwingStar, "saves/assets/images/starthrowing.png");
    iResizeImage(&throwingStar, 20, 20);

    iLoadImage(&settingsmenu, "saves/assets/images/settingBg.png");
    iResizeImage(&settingsmenu, 1000, 600);

    iLoadImage(&soundon, "saves/assets/images/SoundOn.png");
    iResizeImage(&soundon, 80, 80);

    iLoadImage(&soundoff, "saves/assets/images/SoundOff.png");
    iResizeImage(&soundoff, 80, 80);

    iLoadImage(&songon, "saves/assets/images/starthrowing.png");
    iResizeImage(&songon, 65, 65);

    iLoadImage(&songoff, "saves/assets/images/SongOff.png");
    iResizeImage(&songoff, 65, 65);

    iLoadImage(&knob, "saves/assets/images/starthrowing.png");
    iResizeImage(&knob, 14, 14);
    iLoadImage(&endImage, "saves/assets/images/end.png");
    iResizeImage(&endImage, 1000, 600);

=======
    iSetColor(60, 60, 60); // dark gray road color
    iFilledRectangle(0, 0, screenWidth, roadHeight);
}


void load_Images()
{

    // Load and resize obstacles
   iLoadImage(&nameEntryImage, "saves/assets/images/namescreen.jpg"); //name
iResizeImage(&nameEntryImage, 900, 600); // 
   iLoadImage(&leafImg, "saves/assets/images/leaf.png");
iResizeImage(&leafImg, 20, 20);

iLoadImage(&highScoreBg, "saves/assets/images/highscore.jpg"); 
iResizeImage(&highScoreBg, 1000, 600);

    iLoadImage(&fire1, "saves/assets/images/obstacles/fire1.png");
    iResizeImage(&fire1, 80, 80);

    iLoadImage(&log1, "saves/assets/images/obstacles/log1.png");
    iResizeImage(&log1, 80, 80);

    iLoadImage(&tree1_png, "saves/assets/images/obstacles/tree1.png");
    iResizeImage(&tree1_png, 80, 80);

iLoadImage(&navBg, "saves/assets/images/ninja.png");
iResizeImage(&navBg, 1000, 600); 


    iLoadImage(&bg1, "saves/assets/images/start.png");
    iResizeImage(&bg1, 1000, 600);

    iLoadImage(&throwingStar, "saves/assets/images/starthrowing.png");
    iResizeImage(&throwingStar, 20, 20);

   iLoadImage(&settingsmenu, "saves/assets/images/settingBg.png");
   iResizeImage(&settingsmenu, 1000, 600);

    iLoadImage(&soundon, "saves/assets/images/SoundOn.png");
    iResizeImage(&soundon, 80, 80);

    iLoadImage(&soundoff, "saves/assets/images/SoundOff.png");
    iResizeImage(&soundoff, 80, 80);

   iLoadImage(&songon, "saves/assets/images/starthrowing.png");
  iResizeImage(&songon, 65, 65);

    iLoadImage(&songoff, "saves/assets/images/SongOff.png");
    iResizeImage(&songoff, 65, 65);

    iLoadImage(&knob, "saves/assets/images/starthrowing.png");
    iResizeImage(&knob, 14, 14);
iLoadImage(&endImage, "saves/assets/images/end.png");
iResizeImage(&endImage, 1000, 600); // Adjust size if needed

>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
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

<<<<<<< HEAD
=======

>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
    iLoadFramesFromFolder(golemImage, "saves/assets/images/sprites/Golem_2/RunThrowing");
    iInitSprite(&golemSprite, -1);
    iChangeSpriteFrames(&golemSprite, golemImage, 12);
    iResizeSprite(&golemSprite, playerWidth, playerHeight);
    // iSetSpritePosition(&golemSprite, golem, 30);
}

void generateObstacles()
{
    int spacing = 350; // space between each obstacle

    for (int i = 0; i < NUM_OBSTACLES; ++i)
    {
<<<<<<< HEAD
        obstacleX[i] = 300 + i * spacing; 
        obstacleY[i] = 30;                
=======
        obstacleX[i] = 300 + i * spacing; // distributed over the world
        obstacleY[i] = 30; // ground height
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
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
<<<<<<< HEAD
    char scoreText[20];
=======
     char scoreText[20];
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
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

<<<<<<< HEAD
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
=======




void loadHighScores() {
    FILE *file = fopen("saves/data/scores.txt", "r");
    if (file == NULL) return; // No scores yet

    numHighScores = 0;
    while (fscanf(file, "%[^,],%d\n", highScores[numHighScores].name, &highScores[numHighScores].score) == 2) {
        numHighScores++;
        if (numHighScores >= MAX_HIGH_SCORES) break;
    }
    fclose(file);
}
void saveHighScores() {
    FILE *file = fopen("saves/data/scores.txt", "w");
    if (file == NULL) return;

    for (int i = 0; i < numHighScores; i++) {
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
        fprintf(file, "%s,%d\n", highScores[i].name, highScores[i].score);
    }
    fclose(file);
}
<<<<<<< HEAD
void insertScore(char *name, int score)
{
    // Insert into sorted position
    for (int i = 0; i <= numHighScores; i++)
    {
        if (i == numHighScores || score > highScores[i].score)
        {
            for (int j = MAX_HIGH_SCORES - 1; j > i; j--)
            {
=======
void insertScore(char *name, int score) {
    // Insert into sorted position
    for (int i = 0; i <= numHighScores; i++) {
        if (i == numHighScores || score > highScores[i].score) {
            for (int j = MAX_HIGH_SCORES - 1; j > i; j--) {
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
                highScores[j] = highScores[j - 1];
            }
            strncpy(highScores[i].name, name, sizeof(highScores[i].name));
            highScores[i].score = score;
<<<<<<< HEAD
            if (numHighScores < MAX_HIGH_SCORES)
                numHighScores++;
=======
            if (numHighScores < MAX_HIGH_SCORES) numHighScores++;
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
            break;
        }
    }
    saveHighScores();
}

<<<<<<< HEAD
=======

>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
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
    char *navItems[] = {"Start", "About", "Settings", "Help", "High Scores"};
    int navItemCount = 5;

    iShowLoadedImage(0, 0, &navBg);
<<<<<<< HEAD
    for (int i = 0; i < navItemCount; i++)
    {
        int x = screenWidth - buttonWidth - 50;
        int y = screenHeight - 110 - (i + 1) * buttonHeight - i * 60;

        iSetColor(180, 180, 180);
        iFilledRectangle(x, y, buttonWidth, buttonHeight);

=======
    for (int i = 0; i < navItemCount; i++) {
        int x = screenWidth - buttonWidth - 50;
        int y = screenHeight - 110 - (i + 1) * buttonHeight - i * 60;

        // Set plain ash color for button background
        iSetColor(180, 180, 180); // ash color
        iFilledRectangle(x, y, buttonWidth, buttonHeight);

        // Set dark gray for button text for contrast
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
        iSetColor(60, 60, 60);
        iText(x + buttonWidth / 2 - 20, y + buttonHeight / 2 - 5, navItems[i], GLUT_BITMAP_HELVETICA_18);
    }
}
bool mouseNavigationBar(int mx, int my)
{
    int navItemCount = 5;
    int x = screenWidth - buttonWidth - 50;
<<<<<<< HEAD
    for (int i = 0; i < navItemCount; i++)
    {
        int y = screenHeight - 110 - (i + 1) * buttonHeight - i * 60;
        if (mx >= x && mx <= x + buttonWidth &&
            my >= y && my <= y + buttonHeight)
        {
            currentScreen = i + 1; 
            if (i == 4)
                currentScreen = 6;
=======
    for (int i = 0; i < navItemCount; i++) {
        int y = screenHeight - 110 - (i + 1) * buttonHeight - i * 60;
        if (mx >= x && mx <= x + buttonWidth &&
            my >= y && my <= y + buttonHeight) {
            currentScreen = i + 1; // 1=Start, 2=About, 3=Settings, 4=Help, 5=High Scores
            // If you want High Scores to be 6, add a check:
            if (i == 4) currentScreen = 6;
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
            return true;
        }
    }
    return false;
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

/*void animatePlayer()
{
    if ((movingRight || movingLeft) && !isthrowingstar && moving)
    {
        iAnimateSprite(&playerSprite);
    }
<<<<<<< HEAD
}*/
void animatePlayer()
=======
}*/void animatePlayer()
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
{
    if ((movingRight || movingLeft) && !isthrowingstar)
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

<<<<<<< HEAD
=======


>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
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
<<<<<<< HEAD
    // drawRoad();
    drawSprite();
    drawObstacles(); 
    drawstar();
    drawgolem(); 
    drawHealthBar();
    drawLeaves();
=======
   // drawRoad(); // ✅ draw road first
    drawSprite();
    drawObstacles(); // ✅ make sure this is added
    drawstar();
    drawgolem(); // 
    drawHealthBar();
    drawLeaves();

>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
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
    // Check for collision only when not jumping
<<<<<<< HEAD
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
=======
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
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa

    if (playerY <= 50)
    {
        playerY = 50;
        isJumping = false;
        verticalSpeed = 0;
    }
}
void soundVolume()
{
<<<<<<< HEAD

    iSetColor(255, 255, 255);
    iFilledRectangle(368, soundknoby, soundknobx - 368 + 7, 14);
    iShowLoadedImage(soundknobx, soundknoby, &knob);
}
void songVolume()
{
    iSetColor(255, 255, 255);
    iFilledRectangle(368, songknoby, songknobx - 368 + 7, 14);
    iShowLoadedImage(songknobx, songknoby, &knob);
=======
    
        iSetColor(255, 255, 255);
        iFilledRectangle(368, soundknoby, soundknobx - 368 + 7, 14);
        iShowLoadedImage(soundknobx, soundknoby, &knob);
}
void songVolume()
{
        iSetColor(255, 255, 255);
        iFilledRectangle(368, songknoby, songknobx - 368 + 7, 14);
        iShowLoadedImage(songknobx, songknoby, &knob);
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
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
<<<<<<< HEAD
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
    if (enteringName)
    {
        iClear(); 
        iShowLoadedImage(0, 0, &nameEntryImage);
        iSetColor(255, 255, 255);
        iText(300, 350, "Enter Your Name:", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(500, 350, playerName, GLUT_BITMAP_HELVETICA_18);
        return; 
    }

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
            if (!endSoundPlayed)
            {
                insertScore(playerName, playerScore);
                iPlaySound("saves/assets/sounds/gameover.wav", 0, soundvolume);
                endSoundPlayed = true;
                loadHighScores();
            }

            iShowLoadedImage(0, 0, &endImage);

            char scoreText[100];
            sprintf(scoreText, "Your Score: %d", playerScore);
            iSetColor(200, 0, 0);
            iText(50, 550, scoreText, GLUT_BITMAP_HELVETICA_18);

            char nameText[100];
            sprintf(nameText, "Player Name: %s", playerName);
            iSetColor(200, 0, 0);
            iText(50, 520, nameText, GLUT_BITMAP_HELVETICA_18);

            iSetColor(180, 0, 0);
            iText(50, 480, "Press 'R' to Restart", GLUT_BITMAP_HELVETICA_18);
        }

        else if (currentScreen == 6)
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
    generateGolems();
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
void SoundSongSettings(int mx, int my){
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
void iMouseDrag(int mx, int my)
{
    if (currentScreen == 3)
    {
       SoundSongSettings(mx,my); 
    }
}

=======
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
    if (enteringName)
    {
        iClear(); // ✅ First clear screen
        iShowLoadedImage(0, 0, &nameEntryImage);
        iSetColor(255, 255, 255);
        // Make "Enter Your Name:" text larger
        iText(300, 350, "Enter Your Name:", GLUT_BITMAP_TIMES_ROMAN_24);
iText(500, 350, playerName, GLUT_BITMAP_HELVETICA_18);
        return; // ✅ Prevent rest of iDraw() from running
    }

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
    if (!endSoundPlayed)
    {
        insertScore(playerName, playerScore);
        iPlaySound("saves/assets/sounds/gameover.wav", 0, soundvolume);
        endSoundPlayed = true;
        loadHighScores();
    }

    iShowLoadedImage(0, 0, &endImage);

    char scoreText[100];
    sprintf(scoreText, "Your Score: %d", playerScore);
    iSetColor(200, 0, 0); // Deep red
    iText(50, 550, scoreText, GLUT_BITMAP_HELVETICA_18);

    char nameText[100];
    sprintf(nameText, "Player Name: %s", playerName);
    iSetColor(200, 0, 0); // Deep red
    iText(50, 520, nameText, GLUT_BITMAP_HELVETICA_18);

    iSetColor(180, 0, 0); // Deep red
    iText(50, 480, "Press 'R' to Restart", GLUT_BITMAP_HELVETICA_18);
}

else if (currentScreen == 6) {
    iShowLoadedImage(0, 0, &highScoreBg); // draw background first

    iSetColor(255, 255, 255);
    iText(400, 550, "Top 10 High Scores", GLUT_BITMAP_HELVETICA_18);

    for (int i = 0; i < numHighScores; i++) {
        char entry[150];
        sprintf(entry, "%d. %s - %d", i + 1, highScores[i].name, highScores[i].score);
        iText(350, 500 - i * 30, entry, GLUT_BITMAP_HELVETICA_18);
    }
}


    }
 
    // Optional: if no movement keys pressed, reset moveTimer to avoid animation stuck
    if (!movingRight && !movingLeft)
    {
        moveTimer = 0;
    }
   

}void resetGame() {
    // Stop the end sound if it's playing
    if (endSoundPlayed) {
        iStopSound(collisionSoundChannel);
        iStopSound(bgsongchannel);
    }

    // Reset game state
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

    // Reset golems
    generateGolems();
}
/*void resetGame() {
    playerHealth = 3;          // Reset health
    playerScore = 0;           // Reset score
    playerX = 500;             // Reset player position
    playerY = 50;
    worldX = 0;                // Reset camera/scroll
    isJumping = false;         // Reset jumping state
    verticalSpeed = 0;
    hitCooldown = false;       // Reset hit cooldown
    hitCooldownTimer = 0;
    endSoundPlayed = false;    // Allow game-over sound to replay
    currentScreen = 1;         // Return to game screen (or 0 for main menu)
}*/

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

>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
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
<<<<<<< HEAD
           
=======
            // Check if inside the play button circle
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
            int dx = mx - centerX;
            int dy = my - centerY;
            if (dx * dx + dy * dy <= radius * radius)
            {
                isPlayButtonClicked = true;
<<<<<<< HEAD
=======
                enteringName = true; // Show name entry screen
                nameCharIndex = 0;
                playerName[0] = '\0';
                if (sound)
                {
                    iStopSound(-1);
                    iPlaySound("saves/assets/sounds/chime.wav", 0, soundvolume);
                }
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
            }
        }
        else if (isPlayButtonClicked && currentScreen == 0)
        {
            if (mouseNavigationBar(mx, my)) // Only play sound if a button was clicked
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

/*void iSpecialKeyboardUp(unsigned char key)
{
    // Stop movement flags when arrow keys are released
    if (key == GLUT_KEY_LEFT)
    {
        movingLeft = false;
        moveTimer = 0;
        moving = false;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        movingRight = false;
        //moveTimer = 0;
        moving = false;
    }
}*/
<<<<<<< HEAD
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
void hitCoolDown(){
=======



void iTimer()
{
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
    if (hitCooldown)
    {
        hitCooldownTimer++;
        if (hitCooldownTimer >= HIT_COOLDOWN_LIMIT)
        {
            hitCooldown = false;
            hitCooldownTimer = 0;
        }
    }
<<<<<<< HEAD

}
void playerScores(){
    if (currentScreen == 1 && playerHealth > 0)
    {
        playerScore++;
    }
}

void iTimer()
{
    updateLeaves();
    updatePlayer();
    updatestar();
    updateGolems();
    checkStarGolemCollision();
    checkGolemCollision();
    playBGSong();
    SpecialKeyboardUP(); 
=======
    if (currentScreen == 1 && playerHealth > 0) // only increase in game screen
{
    playerScore++;
}

updateLeaves();

    updatePlayer();
    updatestar();
     updateGolems();
    checkStarGolemCollision();
    checkGolemCollision();
    playBGSong();
    SpecialKeyboardUP(); // No need, we fixed key release handling
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // place your own initialization codes here.
    load_Images();
    generateObstacles();
    generateGolems();
    loadHighScores();
<<<<<<< HEAD
    initLeaves();
    iSetTimer(30,playerScores);
    iSetTimer(100, updateGolems);
=======

 

//  Initialize leaves here
    for (int i = 0; i < MAX_LEAVES; i++)
    {
        leaves[i].x = rand() % screenWidth;
        leaves[i].y = rand() % screenHeight + screenHeight;
        leaves[i].speed = rand() % 3 + 1;
        leaves[i].active = true;
    }



      iSetTimer(100, updateGolems);
>>>>>>> 94c2b57aaf06204618a581ea8bdbe7e84a3e0dfa
    iSetTimer(120, animategolem);
    iSetTimer(120, animatePlayer);
    iSetTimer(60, animatestarthrow);
    iSetTimer(20, iTimer);
    iInitializeSound();
    playBGSong();
    iInitialize(screenWidth, screenHeight, "Maze of Shadows");
    return 0;
}