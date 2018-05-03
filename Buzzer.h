//
// Created by chamayou on 04/04/2018.
//

#ifndef Buzzer_h
#define Buzzer_h

#include <Arduino.h>

#define BUTTON_test 51

typedef struct Buzzer {
    int pin_button;
    int pin_led;
    int number;
    int team;
    int score;
    bool used;
} Buzzer;

typedef enum Buzzer_Info {
    BUTTON, LED, NUMBER, TEAM, SCORE, USED, ALL
};


class Buzzers {
public:

    int wait_time = 5;
    int default_time = 200;
    int default_numb = 3;

    int nb_team = 0;
    int nb_buzzer = 0;
    int nb_buzzer_max = 0;

    Buzzers(int nb_buzzer_max);

    Buzzers(int nb_buzzer_max, bool info);

    void printSeparator();

    void printFunctionName(String name);

    void displayInfoBuzzer(Buzzer *buzzer);

    void displayInfoBuzzer(Buzzer *buzzer, Buzzer_Info info);

    void displayAllInfosBuzzer(Buzzer buzzer[]);

    void setBuzzer(Buzzer *buzzer);

    void setAllBuzzer(Buzzer buzzer[]);


//----------------------------------------------------------------------------------------------------------------------
    void ledOnBuzzer(Buzzer *buzzer);

    void ledOnBuzzer(Buzzer *buzzer, int time);

    void ledOnAllBuzzer(Buzzer buzzer[]);

    void ledOnAllBuzzer(Buzzer buzzer[], int time);

    void ledOnAllBuzzer(Buzzer buzzer[], bool used);

    void ledOnAllBuzzer(Buzzer buzzer[], Buzzer_Info info);

    void ledOnAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num);

    void ledOnAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num, bool used);


    //----------------------------------------------------------------------------------------------------------------------
    void ledOffBuzzer(Buzzer *buzzer);

    void ledOffBuzzer(Buzzer *buzzer, int time);

    void ledOffAllBuzzer(Buzzer buzzer[]);

    void ledOffAllBuzzer(Buzzer buzzer[], bool used);

    void ledOffAllBuzzer(Buzzer buzzer[], Buzzer_Info info);

    void ledOffAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num);

    void ledOffAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num, bool used);


    //----------------------------------------------------------------------------------------------------------------------
    void blinkBuzzer(Buzzer *buzzer);

    void blinkBuzzer(Buzzer *buzzer, int time, int numb);


    void blinkAllBuzzer(Buzzer buzzer[]);

    void blinkAllBuzzer(Buzzer buzzer[], Buzzer_Info info);

    void blinkAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num);

    void blinkAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int time, int numb);


    void blinkAllBuzzer(Buzzer buzzer[], int time, int numb);

    void blinkAllBuzzer(Buzzer buzzer[], int time, int numb, int used);

    void blinkAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num, int time, int numb);


//----------------------------------------------------------------------------------------------------------------------
    bool getValueBuzzer(Buzzer *buzzer);

    Buzzer *getBuzzerPush(Buzzer buzzer[]);

    Buzzer *getBuzzerPush(Buzzer buzzer[], bool used);

    Buzzer *getBuzzerPush(Buzzer buzzer[], Buzzer_Info info);

    Buzzer *getBuzzerPush(Buzzer buzzer[], Buzzer_Info info, int num);

    Buzzer *getBuzzerPush(Buzzer buzzer[], Buzzer_Info info, int num, bool used);

//----------------------------------------------------------------------------------------------------------------------
    int numberSelectBuzzer(Buzzer buzzer[]);

    int comptBuzzerPush(Buzzer buzzer[]);

    int comptBuzzerPush(Buzzer *buzzer, int num);

    bool waitBuzzerPush(Buzzer *buzzer);

    bool waitBuzzerPush(Buzzer *buzzer, int time);

//----------------------------------------------------------------------------------------------------------------------



    bool allBuzzerUsed(Buzzer buzzer[]);

    bool allBuzzerTeam(Buzzer buzzer[]);

    void resetBuzzerAllScore(Buzzer buzzer[]);

    void resetBuzzerAllTeam(Buzzer buzzer[]);

    bool getValueButtonTest();

    void testInitBuzzer(Buzzer buzzer[]);

    void testInitTeam(Buzzer buzzer[]);


    void print_unsigned_long_long(unsigned long long value);

    unsigned long long superMillis();

    unsigned long long superMicros();


    int scoreTeam(Buzzer buzzer[], int team);

/*
int orderScoreTeam(Buzzer buzzer[], int place) {
    int score = -1;
    int courant_place = 0;
    int tab[nb_team][2];

    for (int i = 0; i < nb_team; ++i) {
        tab[i][0] = i;
        tab[i][1] = scoreTeam(buzzer, i);
    }
    Serial.print("team 1: ");
    Serial.println(tab[0][0]);

    descendingOrder(tab,nb_team);

    Serial.print("team 2: ");
    Serial.println(tab[0][0]);

    print_array(tab);
}

/*
void displayTab(int tab[], int tab_size) {
    int j = 0;
    for (j = 0; j < tab_size; j++) {
        Serial.print("index : ");
        Serial.print(i);
        Serial.print("value : ");
        Serial.println(tab[j]);
    }
}

void descendingOrder(int tab[], int tab_size) {
    int i = 0;
    int tmp0 = 0;
    int j = 0;

    for (i = 0; i < tab_size; i++)          //On veut remplir la case i du tableau
    {
        for (j = i + 1; j < tab_size; j++) {                                //Dans les cases suivantes
            if (tab[j] < tab[i]) {
                tmp0 = tab[i];
                tab[i] = tab[j];
                tab[j] = tmp0;
            }
        }
    }
}
 */

/*
void print_array(int A[][2])
{
    int i = nb_team;
    int j = 2;
    printf("\n");
    int m,n;
    for (m=0;m<i;m++)
    {
        for (n=0;n<j;n++)
        {
            Serial.print("index : ");
            Serial.print(A[m][n]);
            Serial.print(" value : ");
            Serial.println(A[m][n]);
        }
    }
}

void descendingOrder(int tab[][2], int tab_size) {
    int i = 0;
    int tmp0 = 0;
    int tmp1 = 0;
    int j = 0;

    for (i = 0; i < tab_size; i++)          //On veut remplir la case i du tableau
    {
        for (j = i + 1; j < tab_size; j++) {                                //Dans les cases suivantes
            if (tab[j][0] < tab[i][0]) {
                tmp0 = tab[i][0];
                tmp1 = tab[i][1];//Si c'est le cas on intervertit les cases
                tab[i][0] = tab[j][0];
                tab[i][1] = tab[j][1];
                tab[j][0] = tmp0;
                tab[j][1] = tmp1;
            }
        }
    }
    tab[0][0] = 5;
}
*/
    Buzzer *orderScoreBuzzer(Buzzer buzzer[], int place);

    void descendingOrder(Buzzer buzzer[], int tab_size);

    void orderBuzzerUsed(Buzzer buzzer[]);


private:
    bool DEV_INFO = false;

};

#endif //Buzzer_h










































































































































































































































































































































































































































