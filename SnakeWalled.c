#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

#define VERTICAL_PXLS 30
#define HORIZONTAL_PXLS 50

int i, j, Field[VERTICAL_PXLS][ HORIZONTAL_PXLS], x, y, Gy, Head, Tail, Game, Apples, AppleX, AppleY, keyStroke, direction, applesEaten;
double Score;
int speed, level;

void snakeFieldInitialization();
void printFrame();
void resetScreenPosittion();
void randomApple();
void eatApple();
void movement();
int getCharacter();
void tailRemove();
void GameOver();
void verticalBorderInit(int posX, int posY, int pxls);
void chooseLevel();

int main()
{
    chooseLevel();
    snakeFieldInitialization(); /* Initialising values and states */
    while (Game) /* Game Loop */
    {
        printFrame(); /* Printing the last frame state */
        resetScreenPosittion(); /* Reseting terminal cursor */
        randomApple(); /* Generating random apple if 0 Apples exist */
        movement(); 
        tailRemove();
        Sleep(speed);
    }

    return 0;
}

void chooseLevel()
{
    printf("PLEASE CHOOSE THE GAME DEFFICULTY: \n");
    printf("1. Easy, 2. Average, 3. Hard \n");
    scanf("%d", &level);

    switch (level)
    {
    case 1:
        speed = 200;
        break;
    case 2:
        speed = 20;
        break;
    case 3:
        speed = 1;
        break;
    
    default:
        speed = 200;
        break;
    }
}

void snakeFieldInitialization() /* initializing zeroes in the field */
{
    for (i = 0; i < VERTICAL_PXLS; i++) /* Initializing 0 rows numbers in field  looping through vertical pixels */
    {
        for (j = 0; j < HORIZONTAL_PXLS; j++) /* Initializing 0 Columns numbers in field looping through horizontal pixels */
        {
            Field[i][j] = 0;
        }
    }

    x = VERTICAL_PXLS / 2;
    y = HORIZONTAL_PXLS / 2; /* Starting point of the snake */
    Gy = y;    /* Graphic Y */
    Head = 3;  /* field value for location of head */
    Tail = 1;  /* field value for location of tail */
    Game = 1;  /* While 1 Game loop is looping */
    Apples = 0;
    applesEaten = 0;

    for (i = 0; i < Head; i++)
    {
        Gy++;                                                  /* 21,22,23,24,25 */
        Field[x][Gy - Head] = i + 1; /* @y 16=1,17=2...21=5 */ /* body and head field initialization */
    }

    verticalBorderInit(10,5,6);
    verticalBorderInit(5,40,4);
    verticalBorderInit(15,40,4);
    horizontalBorderInit(20, 7, 8);
}

void verticalBorderInit(int posX, int posY, int pxls)
{
    for (i = 0; i < pxls; i++)
    {
        Field[posX + i][posY] = -4;
    }
    
}

void horizontalBorderInit(int posX, int posY, int pxls)
{
    for (i = 0; i < pxls; i++)
    {
        Field[posX][posY + i] = -5;
    }
    
}

void resetScreenPosittion() /*  */
{
    HANDLE handleConsole;                              /* Provides handle to the console */
    COORD Position;                           /* keeps coordination position */
    handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);   /* using STD_OUTPUT_HANDLE */
    Position.X = 0;                           /* Lef */
    Position.Y = 0;                           /* Top */
    SetConsoleCursorPosition(handleConsole, Position); /* Sets Cursor to 0,0 position */
    
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(handleConsole, &info);
}

void printFrame()
{
    /*************** START: FRAME BUILDING*********************/
    /* TOP FRAME */
    for (i = 0; i < HORIZONTAL_PXLS + 2; i++) /* From 0 to HORIZONTAL_PXLS+1 = HORIZONTAL_PXLS+2 characters with 2 ch to boarders */
    {
        if (i == 0) /* To print the left top corner ╔ */
        {
            printf("%c", 201); /* ASCII for ╔ */
        }
        else if (i == HORIZONTAL_PXLS + 1) /* To print the Right top corner ╔ */
        {
            printf("%c", 187); /* ASCII for ╗ */
        }
        else /* To print the rest of top frame ═════ */
        {
            printf("%c", 205); /* ASCII for ═ */
        }
    }
    printf("\n"); /* Printing new line */

    /* Sides & Renders */
    for (i = 0; i < VERTICAL_PXLS; i++) /* Repeat process by vertical Characters */
    {
        printf("%c", 186); /* For printing left side ║ */

        for (j = 0; j < HORIZONTAL_PXLS + 1; j++) /* the rest of HORIZONTAL_PXLS spaces and 1 side character */
        {
            if (j == HORIZONTAL_PXLS)
                printf("%c\n", 186); /* To print right side with ║ */
            else if (Field[i][j] == 0)
                printf(" "); /* Spaces for gameplay */
            else if (Field[i][j] > 0 && Field[i][j] != Head)
                printf("%c", 176); /* Body of the snake ░░░░ */
            else if (Field[i][j] == Head)
                printf("%c", 178); /* Head of the snake ▓ */
            else if (Field[i][j] == -4)
                printf("%c", 186); /* vertical border ║ */
            else if (Field[i][j] == -5)
                printf("%c", 205); /* Horizontl border ═ */
            else if (Field[i][j] == -1)
                printf("%c", 42); /* The Apple */
            else
                printf("error"); /* Spaces for gameplay */

            // if (Field[i][j] = 0) printf(" "); /* Spaces for gameplay */
            // if (j==M) printf("%c\n",186); /* To print right side with ║ */
        }
    }

    /* BOTTOM FRAME */
    for (i = 0; i < HORIZONTAL_PXLS + 2; i++) /* From 0 to HORIZONTAL_PXLS+1 = HORIZONTAL_PXLS+2 characters with 2 ch to boarders */
    {
        if (i == 0) /* To print the left bottom corner ╔ */
        {
            printf("%c", 200); /* ASCII for ╚ */
        }
        else if (i == HORIZONTAL_PXLS + 1) /* To print the Right bottom corner ╔ */
        {
            printf("%c", 188); /* ASCII for ╝ */
        }
        else /* To print the rest of bottom frame ═════ */
        {
            printf("%c", 205); /* ASCII for ═ */
        }
    }

    Score  = ((applesEaten*50)+((Head-3)/10.0));
    /* For printing Score */
    printf("\nApples: %d, Steps: %d, Score: %.2f", applesEaten, Head-3, Score);

    /*************** End: FRAME BUILDING*********************/
    /***************** Miswada ************************* 
    printf("Ascii of 201 gives: %c\n", 201); 
    Ascii of 201 gives: ╔
    Ascii of 187 gives: ╗
    Ascii of 205 gives: ═
    Ascii of 186 gives: ║
    Ascii of 200 gives: ╚
    Ascii of 188 gives: ╝

    **************************************************/
}

void randomApple()
{
    srand(time(0));                  /* build a dynamic random generator according to milliseconds to avoid predictability */
    AppleX = (rand() % (VERTICAL_PXLS - 2)) + 1; /* Random X position for Apple */
    AppleY = (rand() % (HORIZONTAL_PXLS - 2)) + 1; /* Random Y position for Apple */

    if (Apples == 0 && Field[AppleX][AppleY] == 0) /* Checking whether there is a Apple or something on the field by the random position  */
    {
        Field[AppleX][AppleY] = -1; /* -1 is a special value to find out the Apple */
        Apples = 1;                 /* Setting the Apple to 1 */
    }
}

void eatApple()
{
    Apples = 0;
    applesEaten += 1;
    Tail -= 2; /* increase 2 pixels before trimming the tail */
}

int getCharacter()
{
    if (_kbhit())
        return _getch();
    else
        return -1;
}

void movement()
{
    keyStroke = getCharacter();
    keyStroke = tolower(keyStroke);

    if (((keyStroke == 'd' || keyStroke == 'a') || (keyStroke == 'w' || keyStroke == 's')) && abs(direction - keyStroke) > 5)
        /* the anded abs is to prevent change whenever opposite keys are pressed */
        direction = keyStroke; /* to have a continuos motion that only changes when a key is pressed */

    if (direction == 'd') /* Moving right */
    {
        y++; /* increase the location of y */
        if (Field[x][y] > 3 || Field[x][y] < -3)
            GameOver();
        if (y == HORIZONTAL_PXLS - 1)
            GameOver(); /* hit the wall */
        if (Field[x][y] == -1)
            eatApple();
        Head++;             /* increase the value for head */
        Field[x][y] = Head; /*  adding the new value of to the new location */
    }
    if (direction == 'a') /* Moving left */
    {
        y--;
        if (Field[x][y] > 3 || Field[x][y] < -3)
            GameOver();
        if (y == 0)
            GameOver(); /* to return to the end of the row when hit the left wall */
        if (Field[x][y] == -1)
            eatApple();
        Head++;
        Field[x][y] = Head;
    }
    if (direction == 'w') /* Moving Top */
    {
        x--;
        if (Field[x][y] > 3 || Field[x][y] < -3)
            GameOver();
        if (x == -1)
            GameOver();
        if (Field[x][y] == -1)
            eatApple();
        Head++;
        Field[x][y] = Head;
    }
    if (direction == 's') /* Moving right */
    {
        x++;
        if (Field[x][y] > 3 || Field[x][y] < -3)
            GameOver();
        if (x == VERTICAL_PXLS)
            GameOver();
        if (Field[x][y] == -1)
            eatApple();
        Head++;
        Field[x][y] = Head;
    }
}

void tailRemove()
{
    if (direction) /* Checking if the start has began */
    {
        for (i = 0; i < VERTICAL_PXLS; i++) /* Looping around the field 2d arays */
        {
            for (j = 0; j < HORIZONTAL_PXLS; j++)
            {
                if (Field[i][j] == Tail) /* Checking if there is a Tail value to remove */
                {
                    Field[i][j] = 0; 
                }
            }
        }
        Tail++; /* Incrementing the tail value in the field with the rest of the values */
    }
}

void GameOver()
{
    Score  = ((applesEaten*50)+((Head-3)/10.0));
    printf("\a");
    Sleep(800);
    system("cls");

    printf("\n\n           GAME OVER!!!!!\n");
    printf("           SCORE: %.2f\n", Score);
    printf("If you want new game press enter, to exit press Esc .. \n");
    while (1)
    {
        keyStroke = getCharacter();

        if (keyStroke == 13)
        {
            Game = 1;
            chooseLevel();
            snakeFieldInitialization();
            break;
        } else if (keyStroke == 27) {
            Game = 0;
            break;
        }
        
    }
    
}

