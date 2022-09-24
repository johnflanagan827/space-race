// John Flanagan
// sp22-cse-20311.01
// project main

#include "gfx2.h"

void loadDebris(Debris*);
void checkDebris(Debris*);
void createDebris(Debris*);
void crashDetection(float*, Debris*);
void displayStart(int*);
void displayGame(char the_score[2], float *timer, float *xc, float *yc, Debris *debris);
void displayEnd(int*, int, char[2], char[2]);

int main()
{
    int wid = 480, ht = 720;
    int pausetime = 30000;
    float xc = 240, yc = 640;
    float timer = 0; 
    char c;
    int score = 0;
    char the_score[2] = "0", highscore[2] = "0";
    int red = 1, green = 1, start = 1;
    Debris debris;

    gfx_open(wid, ht, "Space Race");
    gfx_color(0, 255, 0);
    loadDebris(&debris);

    while(1) {
        if (start) {
            displayStart(&green);
            if (gfx_event_waiting()) {
                c = gfx_wait();
                if (c == 32) {
                    start = 0;
                    gfx_color(255, 255, 255);
                }
            }
        }

        else if (230 - timer >= 0) {
            displayGame(the_score, &timer, &xc, &yc, &debris);
            if (gfx_event_waiting()) {
                c = gfx_wait();
                if (c == 'w') yc -= 5;   
                if (c == 's') {
                    if (yc < 640) yc += 5;
                }
            }
            if (yc <= -10) {
                yc = 640;
                score += 1;
                snprintf(the_score, 2,"%d", score);
            }
        }
        else {
            displayEnd(&red, score, the_score, highscore);
            if (gfx_event_waiting()) {
                c = gfx_wait();
                if (c == 'r') {
                    gfx_color(255, 255, 255);
                    timer = 0, score = 0, red = 1;
                    yc = 640;
                    strcpy(the_score, "0");
                }
            }
        }
        gfx_flush();
        if (c == 'q') break;

        usleep(pausetime);
        gfx_clear();
    }
    return 0;
}

// adds randomized debris data to Debris struct
void loadDebris(Debris *debris)
{
    int i, random;
    for (i = 0; i < SIZE; i++)
    {
        while (1) {
            random = rand() % 1431 - 475;
            if ((random >= -475 && random <= -5) || (random >= 485 && random <= 955)) {
                debris->debrisxc[i] = random;
                break;
            }
        }
        debris->debrisyc[i] = rand() % 601;
        if (debris->debrisxc[i] < 0) debris->debrisvelocity[i] = 5;
        else debris->debrisvelocity[i] = -5;
    }
}

// checks if any debris is outside of the screen window. if so, replaces the debris with a new debris
void checkDebris(Debris *debris)
{
    int i, random;
    for (i = 0; i < SIZE; i++)
    {
        if ((debris->debrisxc[i] >= 485 && debris->debrisvelocity[i]>0) || (debris->debrisxc[i] <= -5 && debris->debrisvelocity[i]<0)) {
            while (1) {
                random = rand() % 1431 - 475;
                if ((random >= -475 && random <= -5) || (random >= 485 && random <= 955)) {
                    debris->debrisxc[i] = random;
                    break;
                }
            }
            debris->debrisyc[i] = rand() % 601;
            if (debris->debrisxc[i] < 0) debris->debrisvelocity[i] = rand() % 7 + 3;
            else debris->debrisvelocity[i] = rand() % 7 - 8;
        }
    }
}

// makes debris visible when location is on screen
void createDebris(Debris *debris)
{
    checkDebris(debris);
    
    int i, radius;
    for (i = 0; i < SIZE; i++) {
        if (debris->debrisxc[i] > 485 || debris->debrisxc[i] < -5) radius = 0;
        else radius = 5;
        
        gfx_fill_circle(debris->debrisxc[i], debris->debrisyc[i], radius); 
        debris->debrisxc[i] += debris->debrisvelocity[i];
    }
}

// checks if spaceship crashes with debris
void crashDetection(float *yc, Debris *debris)
{
    int i;
    for (i = 0; i < SIZE; i++)
    {
        if (abs(*yc - debris->debrisyc[i]) <= 15 && abs(240 - debris->debrisxc[i]) <= 5) *yc = 640;
    }
}

// displays the start menu
void displayStart(int *green)
{
    if (*green == 1) gfx_color(0, 255, 0);
    gfx_changefont("12x24");
    gfx_text(170, 275, "Space Race");
    gfx_line(170, 280, 290, 280);
    gfx_text(95, 335, "Press Spacebar to Start");
    gfx_text(95, 380, "Press q to Quit");
    gfx_rectangle(80, 245, 300, 160);
    gfx_changefont("8x16");
    *green = 0;
}

// displays the spaceship, debris, time remaining, and score
void displayGame(char the_score[2], float *timer, float *xc, float *yc, Debris *debris)
{
    gfx_text(10, 700, "Time Remaining:");
    gfx_text(400, 700, "Score:");
    gfx_text(450, 700, the_score);
    gfx_fill_rectangle(140, 690, 230 - *timer, 10);
    gfx_line(0, 660, 480, 660);
    gfx_fill_circle(*xc, *yc, 10);
    createDebris(debris);
    crashDetection(yc, debris);
    *timer += 0.1;
}

// displays end menu
void displayEnd(int *red, int score, char the_score[2], char highscore[2])
{
    if (*red) gfx_color(255, 0, 0);
    if (score > atoi(highscore)) snprintf(highscore, 2,"%d", score);
    gfx_changefont("12x24");
    gfx_text(170, 275, "Game over");
    gfx_changefont("8x16");
    gfx_text(160, 310, "Your Score:");
    gfx_text(275, 310, the_score);
    gfx_text(160, 335, "High Score:");
    gfx_text(275, 335, highscore);
    gfx_changefont("7x14");
    gfx_text(160, 370, "Press r to Restart");
    gfx_text(160, 390, "Press q to Quit");
    gfx_rectangle(80, 245, 300, 160);
    gfx_changefont("8x16");
    *red = 0;
}
