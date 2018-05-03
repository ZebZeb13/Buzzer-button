/*  Arduino TFT Tutorial
 *  Program made by Dejan Nedelkovski,
 *  www.HowToMechatronics.com 
 */

/*  This program uses the UTFT and URTouch libraries
 *  made by Henning Karlsen. 
 *  You can find and download them at:
 *  www.RinkyDinkElectronics.com
 */


#include "Buzzer.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#define USE_ADAFRUIT_SHIELD_PINOUT


#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif


#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin


#define TS_MINX 120
#define TS_MAXX 950
#define TS_MINY 90
#define TS_MAXY 900

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define    BLACK   0x0000
#define    BLUE    0x001F
#define    RED     0xF800
#define    GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define BACK_GROUND WHITE

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


#define PENRADIUS 3
int oldcolor, currentcolor;


enum {
    HOME_screen,
    TEAM_screen,
    RESULT_screen,
    CHECK_screen,
    INDIVIDUAL_screen
};

enum {
    Individual_mode,
    Team_mode
};

int currentPage = HOME_screen;

int mode = Individual_mode;

int place = 0;

/*
typedef struct Buzzer {
    int pin_button;
    int pin_led;
    int number;
    int team;
    int score;
    bool used;
} Buzzer;
 */

typedef struct Button {
    int x;
    int y;
    int x_size;
    int y_size;
    String string;
    int size_string;
    uint16_t color_string;
    uint16_t color_background;
} Button;


Buzzers buzzers(10, false);

Buzzer buzzer[] = {{51, 45, -1, -1, 0, false},
                   {27, 31, -1, -1, 0, false},
                   {29, 25, -1, -1, 0, false},
                   {26, 24, -1, -1, 0, false},
                   {35, 40, -1, -1, 0, false},
                   {37, 32, -1, -1, 0, false},
                   {41, 46, -1, -1, 0, false},
                   {28, 44, -1, -1, 0, false},
                   {30, 42, -1, -1, 0, false},
                   {39, 33, -1, -1, 0, false},
};


Buzzer *lastBuzzer;
Buzzer *lastBuzzer_push;

#define WIDTH_SCREEN 480
#define HEIGHT_SCREEN 320

#define BOXSIZE_X 250
#define BOXSIZE_Y 75

#define CENTER_BOX_X (WIDTH_SCREEN / 2 - BOXSIZE_X / 2)

Button InitDone = {CENTER_BOX_X, HEIGHT_SCREEN / 1.5, BOXSIZE_X, BOXSIZE_Y, "Terminer", 3, BLACK, BLUE};

Button Individual = {CENTER_BOX_X, HEIGHT_SCREEN / 4, BOXSIZE_X, BOXSIZE_Y, "Individuel", 3, BLACK, BLUE};

Button Team = {CENTER_BOX_X, HEIGHT_SCREEN / 1.5, BOXSIZE_X, BOXSIZE_Y, "Equipe", 3, BLACK, BLUE};

Button OK = {WIDTH_SCREEN / 8, HEIGHT_SCREEN / 1.5, 150, 75, "OK", 3, BLACK, GREEN};

Button NO_OK = {WIDTH_SCREEN / 8 * 5, HEIGHT_SCREEN / 1.5, 150, 75, "NO OK", 3, BLACK, RED};

Button B_1 = {WIDTH_SCREEN / 8 * 6.5, HEIGHT_SCREEN / 3, 100, 75, "-1", 3, BLACK, RED};

Button RETURN = {10, 10, 60, 40, "<-", 4, BLACK, BLUE};

Button Nb_buzzers = {CENTER_BOX_X, (HEIGHT_SCREEN - BOXSIZE_Y) / 2, BOXSIZE_X, BOXSIZE_Y, "0", 4, BLACK, BACK_GROUND};

Button Nb_team = {CENTER_BOX_X, (HEIGHT_SCREEN - BOXSIZE_Y) / 2, BOXSIZE_X, BOXSIZE_Y, "1", 4, BLACK, BACK_GROUND};

Button Finish_game = {CENTER_BOX_X, HEIGHT_SCREEN / 1.25, BOXSIZE_X, BOXSIZE_Y, "Fin de la partie", 2, BLACK, RED};

Button Add_team = {WIDTH_SCREEN / 8 - 10, HEIGHT_SCREEN / 1.5, 210, 75, "Nouvelle equipe", 2, BLACK, GREEN};

Button Init_team_done = {WIDTH_SCREEN / 8 * 5, HEIGHT_SCREEN / 1.5, 150, 75, "Terminer", 2, BLACK, RED};

Button Result = {CENTER_BOX_X, HEIGHT_SCREEN / 2 - 30, BOXSIZE_X, BOXSIZE_Y + 30, "Joueur :", 2, BLACK, GREEN};


Button Result_next = {WIDTH_SCREEN / 9 * 7, HEIGHT_SCREEN / 2, 60, 40, "->", 2, BLACK, CYAN};

Button Result_previous = {WIDTH_SCREEN / 9, HEIGHT_SCREEN / 2, 60, 40, "<-", 2, BLACK, CYAN};


int nb_run = 0;


int tour = 0;
unsigned long milis1 = 0;
unsigned long milis2 = 0;


//----------------------------------------------------------------------------------------------------------------------

int getPixelSizeWidthString(String string, int size) {
    return string.length() * 6 * size;

}

int getPixelSizeHeightString(int size) {
    return 8 * size;

}


//----------------------------------------------------------------------------------------------------------------------
/*
 //dessine un bouton en utilisant la fonction center
void drawButton(Button *button) {
    tft.fillRect(button->x, button->y, button->x_size, button->y_size, button->color_background);

    centerText(button->string,button->x,button->x+button->x_size,button->y + (button->y_size - getPixelSizeHeightString(button->size_string)) / 2,button->size_string,button->color_string);

}

*/
void drawButton(Button *button) {
    tft.fillRect(button->x, button->y, button->x_size, button->y_size, button->color_background);

    tft.setCursor(button->x + (button->x_size - getPixelSizeWidthString(button->string, button->size_string)) / 2,
                  button->y + (button->y_size - getPixelSizeHeightString(button->size_string)) / 2);
    tft.setTextColor(button->color_string);
    tft.setTextSize(button->size_string);
    tft.println(button->string);
}


/*
 //dessine une forme au tour du bouton tant qu'il est void drawButtonFrame(Button *button) {
    if (button->color_background == WHITE) {
        tft.drawRect(button->x, button->y, button->x_size, button->y_size, BLACK);
    } else {
        tft.drawRect(button->x, button->y, button->x_size, button->y_size, WHITE);
    }
    while (touchButton(button));
}
 */

bool touchButton(Button *button) {
    TSPoint p = ts.getPoint();

    // if sharing pins, you'll need to fix the directions of the touchscreen pins
    pinMode(XP, OUTPUT);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(YM, OUTPUT);

    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

        int temp = p.x;
        p.x = p.y;
        p.y = temp;
/*
        Serial.print("X = "); Serial.print(p.x);
        Serial.print("\tY = "); Serial.print(p.y);
        Serial.print("\tPressure = "); Serial.println(p.z);

*/

        // scale from 0->1023 to tft.width
        p.x = map(p.x, TS_MINX, TS_MAXX, WIDTH_SCREEN, 0);
        //p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
        p.y = map(p.y, TS_MINY, TS_MAXY, HEIGHT_SCREEN, 0);
/*

        Serial.print("("); Serial.print(p.x);
        Serial.print(", "); Serial.print(p.y);
        Serial.println(")");
*/


        if (((p.x >= button->x) && (p.x <= button->x + button->x_size) && (p.y >= button->y) &&
             (p.y <= button->y + button->y_size))) {


            //drawButtonFrame(button);

            return true;
        }

    }
    return false;
}


/*
 //dessine une grille pour mieux placer les elements
void grid(int raw,int col){
    for (int i = 0; i <= raw ; ++i) {
        tft.drawLine((WIDTH_SCREEN/col)*i, 0, (WIDTH_SCREEN/col)*i,HEIGHT_SCREEN, BLACK);
    }
    for (int j = 0; j <= col; ++j) {
        tft.drawLine(0, (HEIGHT_SCREEN/raw)*j, WIDTH_SCREEN, (HEIGHT_SCREEN/raw)*j, BLACK);
    }
}
 */



//----------------------------------------------------------------------------------------------------------------------
void centerText(String string, int y, int size, uint16_t color) {
    tft.setCursor((WIDTH_SCREEN - getPixelSizeWidthString(string, size)) / 2, y);
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.println(string);
}
/*
void centerText(String string, int y, int size, uint16_t color) {
    centerText(string,0,WIDTH_SCREEN,y,size,color);
}


// Font qui centre une string sur une en un point
void centerText(String string, int x1, int x2, int y, int size, uint16_t color) {
    tft.setCursor(x1 + ((x1 - x2) - getPixelSizeWidthString(string, size)) / 2, y);
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.println(string);
}

*/

//----------------------------------------------------------------------------------------------------------------------

void drawInitBuzzer() {
    tft.fillScreen(BACK_GROUND);

    centerText("Appuyer sur tous les buzzers", HEIGHT_SCREEN / 6, 2, BLACK);

    drawButton(&InitDone);

    drawButton(&Nb_buzzers);
}

void drawInitTeam() {
    tft.fillScreen(BACK_GROUND);
    centerText("Appuyer sur tous les buzzers", HEIGHT_SCREEN / 6, 2, BLACK);
    centerText(" de l'equipe", HEIGHT_SCREEN / 4, 2, BLACK);

    Nb_team.string = String(buzzers.nb_team);
    drawButton(&Nb_team);

    drawButton(&Init_team_done);


    drawButton(&Add_team);
}


void drawHomeScreen() {
    tft.fillScreen(BACK_GROUND);

    drawButton(&Individual);


    drawButton(&Team);

}



void drawCheckScreen(Buzzer *buzzer) {
    tft.fillScreen(BACK_GROUND);

    centerText("Validez la reponse", HEIGHT_SCREEN / 6, 2, BLACK);

    if (mode == Team_mode) {
        String string = "Equipe : " + String(buzzer->team);
        centerText(string, HEIGHT_SCREEN / 4, 2, BLACK);
        String string1 = "Equipe Score : " + String(buzzers.scoreTeam(buzzer, buzzer->team));
        centerText(string1, HEIGHT_SCREEN / 3, 2, BLACK);
    }


    String string2 = "Joueur : " + String(buzzer->number);
    centerText(string2, HEIGHT_SCREEN / 2.5 + 5, 2, BLACK);
    String string3 = "Joueur Score: " + String(buzzer->score);
    centerText(string3, HEIGHT_SCREEN / 2, 2, BLACK);
    drawButton(&OK);


    drawButton(&NO_OK);

    drawButton(&B_1);
}

void drawIndividualScreen(Buzzer buzzer[]) {
    tft.fillScreen(BACK_GROUND);
    place = 0;

    centerText("Individuel", 10, 3, BLACK);

    String string = "Nombre de joueurs : " + String(buzzers.nb_buzzer);
    centerText(string, HEIGHT_SCREEN / 6, 2, BLACK);

    String string1 = "Nombre questions : " + String(nb_run);
    centerText(string1, HEIGHT_SCREEN / 4, 2, BLACK);


    if (nb_run > 0) {
        Result.string = "Joueur : " + String(buzzers.orderScoreBuzzer(buzzer, 0)->number);
    } else {
        Result.string = "Aucune reponse OK";
    }
    buzzers.displayAllInfosBuzzer(buzzer);

    drawButton(&Result);

    if (nb_run > 0) {
        String string2 = "Place : 1";
        centerText(string2, HEIGHT_SCREEN / 2 - 20, 2, BLACK);
        String string3 = "Score : " + String(buzzers.orderScoreBuzzer(buzzer, 0)->score);
        centerText(string3, HEIGHT_SCREEN / 2 + 45, 2, BLACK);
    }

    drawButton(&Result_previous);

    drawButton(&Result_next);

    drawButton(&Finish_game);
}

void drawTeamScreen() {
    tft.fillScreen(BACK_GROUND);

    centerText("Equipe", 10, 3, BLACK);

    String string = "Nombre d'equipe : " + String(buzzers.nb_team);
    centerText(string, HEIGHT_SCREEN / 6, 2, BLACK);

    int nb_player = 0;
    for (int i = 0; i < buzzers.nb_buzzer; ++i) {
        if (buzzer[i].team != -1) {
            nb_player = nb_player + 1;
        }
    }

    place = 1;

    String string1 = "Nombre de joueurs : " + String(nb_player);
    centerText(string1, HEIGHT_SCREEN / 4, 2, BLACK);

    String string2 = "Nombre questions : " + String(nb_run);
    centerText(string2, HEIGHT_SCREEN / 3, 2, BLACK);


    if (nb_run > 0) {
        Result.string = "Equipe : 1";
    } else {
        Result.string = "Aucune reponse OK";
    }
    buzzers.displayAllInfosBuzzer(buzzer);

    drawButton(&Result);

    if (nb_run > 0) {
        String string3 = "Score : " + String(buzzers.scoreTeam(buzzer, 1));
        centerText(string3, HEIGHT_SCREEN / 2 + 45, 2, BLACK);
    }

    drawButton(&Result_previous);

    drawButton(&Result_next);


    drawButton(&Finish_game);
}

//----------------------------------------------------------------------------------------------------------------------
void setup() {

    Serial.begin(9600);

    Serial.println(F("Paint!"));

    tft.reset();


    pinMode(BUTTON_test, INPUT_PULLUP);

/*
        pinMode(34,OUTPUT);

        digitalWrite(33,HIGH);
        Serial.println(33);
        delay(500);
        digitalWrite(33,LOW);



    for (int i = 22; i < 55; ++i) {
        pinMode(i,INPUT_PULLUP);
    }

    while (1) {
        for (int i = 22; i < 55; ++i) {
            if (!digitalRead(i)) {
                Serial.print("apuyer : ");
                Serial.println(i);
                delay(1000);
                if (digitalRead(i)) {
                    Serial.print("good : ");
                    Serial.println(i);
                }
            }

        }
    }
*/


    buzzers.setAllBuzzer(buzzer);

    tft.begin(0x9341);
    tft.setRotation(3);

    Serial.print("TFT size is ");
    Serial.print(tft.width());
    Serial.print("x");
    Serial.println(tft.height());
    tft.fillScreen(BACK_GROUND);





    drawInitBuzzer();

    while (!buzzers.allBuzzerUsed(buzzer) && !touchButton(&InitDone)) {
        Buzzer *courantBuzzer = buzzers.getBuzzerPush(buzzer, false);
        if (courantBuzzer) {
            buzzers.nb_buzzer = buzzers.nb_buzzer + 1;
            courantBuzzer->number = buzzers.nb_buzzer;
            courantBuzzer->used = true;
            buzzers.displayInfoBuzzer(courantBuzzer);

            buzzers.ledOffBuzzer(courantBuzzer);

            Nb_buzzers.string = String(buzzers.nb_buzzer);
            drawButton(&Nb_buzzers);
        }

    }
    buzzers.orderBuzzerUsed(buzzer);

    buzzers.ledOffAllBuzzer(buzzer, false);

    buzzers.blinkAllBuzzer(buzzer);

    buzzers.displayAllInfosBuzzer(buzzer);

    drawHomeScreen();
    currentPage = HOME_screen;
    milis1 = millis();



}


void loop() {

    /*
    if(tour < 5000000){
        tour = tour + 1;
        milis2 = millis();
        Serial.print(milis2);
        Serial.print("-");
        Serial.print(milis1);
        Serial.print("=");
        Serial.println(milis2-milis1);
        milis1 = millis();
    }
     */

    switch (currentPage) {
        case HOME_screen:

            if (touchButton(&Individual)) {
                nb_run = 0;
                drawIndividualScreen(buzzer);
                buzzers.ledOnAllBuzzer(buzzer);
                mode = Individual_mode;
                currentPage = INDIVIDUAL_screen;
            }

            if (touchButton(&Team)) {
                nb_run = 0;

                buzzers.resetBuzzerAllTeam(buzzer);
                buzzers.nb_team = 1;


                drawInitTeam();
                buzzers.ledOnAllBuzzer(buzzer);


                while (!buzzers.allBuzzerTeam(buzzer) && !touchButton(&Init_team_done)) {
                    Buzzer *courantBuzzer = buzzers.getBuzzerPush(buzzer, TEAM, -1);
                    if (courantBuzzer) {
                        courantBuzzer->team = buzzers.nb_team;
                        buzzers.ledOffBuzzer(courantBuzzer);
                        buzzers.displayInfoBuzzer(courantBuzzer);
                    }
                    if (touchButton(&Add_team)) {
                        buzzers.nb_team = buzzers.nb_team + 1;
                        Nb_team.string = String(buzzers.nb_team);
                        drawButton(&Nb_team);
                        delay(500);
                    }
                }
                drawTeamScreen();

                buzzers.ledOffAllBuzzer(buzzer);

                for (int i = 1; i <= buzzers.nb_team; ++i) {
                    buzzers.blinkAllBuzzer(buzzer, TEAM, i);
                }


                buzzers.ledOnAllBuzzer(buzzer, TEAM);
                mode = Team_mode;
                currentPage = TEAM_screen;
            }
            break;
//----------------------------------------------------------------------------------------------------------------------
        case CHECK_screen:

            if (touchButton(&OK)) {
                buzzers.blinkBuzzer(lastBuzzer_push);
                lastBuzzer_push->score = lastBuzzer_push->score + 1;

                nb_run = nb_run + 1;

                if (mode == Individual_mode) {
                    drawIndividualScreen(buzzer);
                    currentPage = INDIVIDUAL_screen;

                    buzzers.blinkAllBuzzer(buzzer);
                    buzzers.ledOnAllBuzzer(buzzer);
                } else if (mode == Team_mode) {
                    drawTeamScreen();
                    currentPage = TEAM_screen;

                    buzzers.blinkAllBuzzer(buzzer, TEAM);
                    buzzers.ledOnAllBuzzer(buzzer, TEAM);
                }
            }
            if (touchButton(&NO_OK)) {


                if (mode == Individual_mode) {
                    drawIndividualScreen(buzzer);
                    currentPage = INDIVIDUAL_screen;

                    //buzzers.blinkAllBuzzer(buzzer);
                    buzzers.ledOnAllBuzzer(buzzer);
                } else if (mode == Team_mode) {
                    drawTeamScreen();
                    currentPage = TEAM_screen;

                    //buzzers.blinkAllBuzzer(buzzer, TEAM);
                    buzzers.ledOnAllBuzzer(buzzer, TEAM);
                }
            }

            if(touchButton(&B_1)){
                lastBuzzer_push->score = lastBuzzer_push->score - 1;
                if (mode == Individual_mode) {
                    drawIndividualScreen(buzzer);
                    currentPage = INDIVIDUAL_screen;

                    //buzzers.blinkAllBuzzer(buzzer);
                    buzzers.ledOnAllBuzzer(buzzer);
                } else if (mode == Team_mode) {
                    drawTeamScreen();
                    currentPage = TEAM_screen;

                    //buzzers.blinkAllBuzzer(buzzer, TEAM);
                    buzzers.ledOnAllBuzzer(buzzer, TEAM);
                }
            }
            break;
//----------------------------------------------------------------------------------------------------------------------
        case INDIVIDUAL_screen:

            if (touchButton(&Result_next)) {

                if (place + 1 >= buzzers.nb_buzzer) {
                    place = 0;
                } else {
                    place = place + 1;
                }

                if (nb_run > 0) {
                    Result.string = "Joueur : " + String(buzzers.orderScoreBuzzer(buzzer, place)->number);
                } else {
                    Result.string = "Aucune reponse OK";
                }

                drawButton(&Result);

                if (nb_run > 0) {
                    String string2 = "Place : " + String(place + 1);
                    centerText(string2, HEIGHT_SCREEN / 2 - 20, 2, BLACK);
                    String string3 = "Score : " + String(buzzers.orderScoreBuzzer(buzzer, place)->score);
                    centerText(string3, HEIGHT_SCREEN / 2 + 45, 2, BLACK);
                }
            }


            if (touchButton(&Result_previous)) {
                if (place - 1 < 0) {
                    place = buzzers.nb_buzzer - 1;
                } else {
                    place = place - 1;
                }

                if (nb_run > 0) {
                    Result.string = "Joueur : " + String(buzzers.orderScoreBuzzer(buzzer, place)->number);
                } else {
                    Result.string = "Aucune reponse OK";
                }

                drawButton(&Result);

                if (nb_run > 0) {
                    String string2 = "Place : " + String(place + 1);
                    centerText(string2, HEIGHT_SCREEN / 2 - 20, 2, BLACK);
                    String string3 = "Score : " + String(buzzers.orderScoreBuzzer(buzzer, place)->score);
                    centerText(string3, HEIGHT_SCREEN / 2 + 45, 2, BLACK);
                }
            }

            if (touchButton(&Result)) {
                if (nb_run > 0) {
                    buzzers.blinkBuzzer(buzzers.orderScoreBuzzer(buzzer, place));
                    buzzers.ledOnBuzzer(buzzers.orderScoreBuzzer(buzzer, place));
                }
            }


            if (touchButton(&Finish_game)) {
                drawHomeScreen();
                currentPage = HOME_screen;
                buzzers.ledOffAllBuzzer(buzzer);
                buzzers.resetBuzzerAllScore(buzzer);
            }

            lastBuzzer = buzzers.getBuzzerPush(buzzer);
            if (lastBuzzer) {
                lastBuzzer_push = lastBuzzer;
                buzzers.ledOffAllBuzzer(buzzer);
                buzzers.ledOnBuzzer(lastBuzzer_push);
                drawCheckScreen(lastBuzzer_push);
                currentPage = CHECK_screen;

            }
            break;
//----------------------------------------------------------------------------------------------------------------------
        case TEAM_screen:

            if (touchButton(&Result_next)) {

                if (place + 1 > buzzers.nb_team) {
                    place = 1;
                } else {
                    place = place + 1;
                }

                if (nb_run > 0) {
                    Result.string = "Equipe : " + String(place);
                } else {
                    Result.string = "Aucune reponse OK";
                }

                drawButton(&Result);

                if (nb_run > 0) {
                    String string3 = "Score : " + String(buzzers.scoreTeam(buzzer, place));
                    centerText(string3, HEIGHT_SCREEN / 2 + 45, 2, BLACK);
                }
            }

            buzzers.displayAllInfosBuzzer(buzzer);


            if (touchButton(&Result_previous)) {
                if (place - 1 < 1) {
                    place = buzzers.nb_team;
                } else {
                    place = place - 1;
                }

                if (nb_run > 0) {
                    Result.string = "Equipe : " + String(place);
                } else {
                    Result.string = "Aucune reponse OK";
                }

                drawButton(&Result);

                if (nb_run > 0) {
                    String string3 = "Score : " + String(buzzers.scoreTeam(buzzer, place));
                    centerText(string3, HEIGHT_SCREEN / 2 + 45, 2, BLACK);

                }
            }

            if (touchButton(&Result)) {
                if (nb_run > 0) {
                    buzzers.blinkAllBuzzer(buzzer, TEAM, place);
                    buzzers.ledOnAllBuzzer(buzzer, TEAM, place);
                }
            }


            if (touchButton(&Finish_game)) {
                drawHomeScreen();
                currentPage = HOME_screen;
                buzzers.ledOffAllBuzzer(buzzer);
                buzzers.resetBuzzerAllScore(buzzer);
            }

            lastBuzzer = buzzers.getBuzzerPush(buzzer, TEAM);
            if (lastBuzzer) {
                lastBuzzer_push = lastBuzzer;
                buzzers.ledOffAllBuzzer(buzzer, TEAM);
                buzzers.ledOnBuzzer(lastBuzzer_push);
                drawCheckScreen(lastBuzzer_push);
                currentPage = CHECK_screen;

            }
            break;
    }


}
//----------------------------------------------------------------------------------------------------------------------




















