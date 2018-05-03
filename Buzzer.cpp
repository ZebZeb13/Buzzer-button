//
// Created by chamayou on 04/04/2018.
//

#include "Buzzer.h"


Buzzers::Buzzers(int nb_buzzer_max, bool info) {
    this->nb_buzzer_max = nb_buzzer_max;
    this->DEV_INFO = info;
}

Buzzers::Buzzers(int nb_buzzer_max) {
    this->nb_buzzer_max = nb_buzzer_max;
    this->DEV_INFO = false;
}

//----------------------------------------------------------------------------------------------------------------------
void Buzzers::printSeparator() {
    if (DEV_INFO) {
        Serial.println("+------------------------------------------------------------------+");
    }
}

void Buzzers::printFunctionName(String name) {
    if (DEV_INFO) {
        printSeparator();
        Serial.print("\t\t\t");
        Serial.println(name);
        printSeparator();
    }
}


void Buzzers::displayInfoBuzzer(Buzzer *buzzer) {
    if (DEV_INFO) {
        printSeparator();
        Serial.print("Buzzer numero : ");
        Serial.println(buzzer->number);

        Serial.print("Buzzer pin button : ");
        Serial.println(buzzer->pin_button);

        Serial.print("Buzzer pin led : ");
        Serial.println(buzzer->pin_led);

        Serial.print("Buzzer equipe : ");
        Serial.println(buzzer->team);

        Serial.print("Buzzer score : ");
        Serial.println(buzzer->score);


        Serial.print("Buzzer used : ");
        Serial.println(buzzer->used);

        printSeparator();
    }

}

void Buzzers::displayInfoBuzzer(Buzzer *buzzer, Buzzer_Info info) {
    if (DEV_INFO) {
        switch (info) {
            case NUMBER:
                Serial.print("Buzzer numero : ");
                Serial.println(buzzer->number);
                break;
            case BUTTON:
                Serial.print("Buzzer pin button : ");
                Serial.println(buzzer->pin_button);
                break;
            case LED:
                Serial.print("Buzzer pin led : ");
                Serial.println(buzzer->pin_led);
                break;
            case TEAM:
                Serial.print("Buzzer team : ");
                Serial.println(buzzer->team);
                break;
            case SCORE:
                Serial.print("Buzzer score : ");
                Serial.println(buzzer->score);
                break;
            case USED:
                Serial.print("Buzzer used : ");
                Serial.println(buzzer->used);
                break;
        }
    }

}


void Buzzers::displayAllInfosBuzzer(Buzzer buzzer[]) {
    printFunctionName("displayAllInfosBuzzer");
    for (int i = 0; i < nb_buzzer_max; ++i) {
        displayInfoBuzzer(&buzzer[i]);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Buzzers::setBuzzer(Buzzer *buzzer) {

    printFunctionName("setBuzzer");
    displayInfoBuzzer(buzzer);
    pinMode(buzzer->pin_button, INPUT_PULLUP);
    pinMode(buzzer->pin_led, OUTPUT);
    digitalWrite(buzzer->pin_led, HIGH);
}

void Buzzers::setAllBuzzer(Buzzer buzzer[]) {
    printFunctionName("setAllBuzzer");
    for (int i = 0; i < nb_buzzer_max; ++i) {
        setBuzzer(&buzzer[i]);
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Buzzers::ledOnBuzzer(Buzzer *buzzer) {
    digitalWrite(buzzer->pin_led, HIGH);
}

void Buzzers::ledOnBuzzer(Buzzer *buzzer, int time) {
    blinkBuzzer(buzzer, time, 1);
}

void Buzzers::ledOnAllBuzzer(Buzzer buzzer[]) {
    ledOnAllBuzzer(buzzer, true);
}

void Buzzers::ledOnAllBuzzer(Buzzer buzzer[], int time) {
    blinkAllBuzzer(buzzer, time, 1);
}

void Buzzers::ledOnAllBuzzer(Buzzer buzzer[], bool used) {
    ledOnAllBuzzer(buzzer, ALL, 0, used);
}

void Buzzers::ledOnAllBuzzer(Buzzer buzzer[], Buzzer_Info info) {
    switch (info) {
        case NUMBER:
            ledOnAllBuzzer(buzzer, ALL, 0, true);
            break;
        case TEAM:
            ledOnAllBuzzer(buzzer, TEAM, -2, true);
            break;
    }
}

void Buzzers::ledOnAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num) {
    ledOnAllBuzzer(buzzer, info, num, true);
}

void Buzzers::ledOnAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num, bool used) {
    int nb_buzzer_count = 0;
    if (used) {
        nb_buzzer_count = nb_buzzer;
    } else {
        nb_buzzer_count = nb_buzzer_max;
    }
    for (int i = 0; i < nb_buzzer_count; ++i) {
        switch (info) {
            case TEAM:
                if (num == -2) {
                    if ((&buzzer[i])->team != -1) {
                        ledOnBuzzer(&buzzer[i]);
                    }
                } else if ((&buzzer[i])->team == num) {
                    ledOnBuzzer(&buzzer[i]);
                }
                break;

            case ALL:
                if ((&buzzer[i])->used == used) {
                    ledOnBuzzer(&buzzer[i]);
                }
                break;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Buzzers::ledOffBuzzer(Buzzer *buzzer) {
    digitalWrite(buzzer->pin_led, LOW);
}

void Buzzers::ledOffAllBuzzer(Buzzer buzzer[]) {
    ledOffAllBuzzer(buzzer, true);
}

void Buzzers::ledOffAllBuzzer(Buzzer buzzer[], bool used) {
    ledOffAllBuzzer(buzzer, ALL, 0, used);
}

void Buzzers::ledOffAllBuzzer(Buzzer buzzer[], Buzzer_Info info) {
    switch (info) {
        case NUMBER:
            return ledOffAllBuzzer(buzzer, ALL, 0, true);
        case TEAM:
            return ledOffAllBuzzer(buzzer, TEAM, -2, true);
    }
}

void Buzzers::ledOffAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num) {
    ledOffAllBuzzer(buzzer, info, num, true);
}

void Buzzers::ledOffAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num, bool used) {
    int nb_buzzer_count = 0;
    if (used) {
        nb_buzzer_count = nb_buzzer;
    } else {
        nb_buzzer_count = nb_buzzer_max;
    }
    for (int i = 0; i < nb_buzzer_count; ++i) {
        switch (info) {
            case TEAM:
                if (num == -2) {
                    if ((&buzzer[i])->team != -1) {
                        ledOffBuzzer(&buzzer[i]);
                    }
                } else if ((&buzzer[i])->team == num) {
                    ledOffBuzzer(&buzzer[i]);
                }
                break;

            case ALL:
                if ((&buzzer[i])->used == used) {
                    ledOffBuzzer(&buzzer[i]);
                }
                break;
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------
void Buzzers::blinkBuzzer(Buzzer *buzzer) {
    blinkBuzzer(buzzer, default_time, default_numb);
}

void Buzzers::blinkBuzzer(Buzzer *buzzer, int time, int numb) {
    for (int i = 0; i < numb; ++i) {
        ledOnBuzzer(buzzer);
        delay(time);
        ledOffBuzzer(buzzer);
        if (i < numb - 1) {
            delay(time);
        }
    }
}

void Buzzers::blinkAllBuzzer(Buzzer buzzer[]) {
    blinkAllBuzzer(buzzer, default_time, default_numb);
}

void Buzzers::blinkAllBuzzer(Buzzer buzzer[], int time, int numb) {
    blinkAllBuzzer(buzzer, ALL, 0, time, numb);
}

void Buzzers::blinkAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num) {
    blinkAllBuzzer(buzzer, info, num, default_time, default_numb);
}

void Buzzers::blinkAllBuzzer(Buzzer buzzer[], Buzzer_Info info) {
    blinkAllBuzzer(buzzer, info, -2, default_time, default_numb);
}

void Buzzers::blinkAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int time, int numb) {
    blinkAllBuzzer(buzzer, info, 0, time, numb);
}

void Buzzers::blinkAllBuzzer(Buzzer buzzer[], Buzzer_Info info, int num, int time, int numb) {
    for (int i = 0; i < numb; ++i) {
        switch (info) {
            case TEAM:
                ledOnAllBuzzer(buzzer, TEAM, num);
                delay(time);
                ledOffAllBuzzer(buzzer, TEAM, num);
                break;

            case ALL:
                ledOnAllBuzzer(buzzer);
                delay(time);
                ledOffAllBuzzer(buzzer);
                break;
        }
        if (i < numb - 1) {
            delay(time);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
bool Buzzers::getValueBuzzer(Buzzer *buzzer) {

    if (digitalRead(buzzer->pin_button) == LOW) {

        if (DEV_INFO) {
            printSeparator();
            Serial.println("Buzzer push");
            displayInfoBuzzer(buzzer);
            printSeparator();
        } else {
            Serial.println("Delay a changer et voir millis()");
            delay(300);
        }
        return true;
    } else {
        return false;
    }
}

Buzzer *Buzzers::getBuzzerPush(Buzzer buzzer[]) {
    return getBuzzerPush(buzzer, true);
}

Buzzer *Buzzers::getBuzzerPush(Buzzer buzzer[], bool used) {
    return getBuzzerPush(buzzer, ALL, 0, used);
}

Buzzer *Buzzers::getBuzzerPush(Buzzer buzzer[], Buzzer_Info info) {
    switch (info) {
        case NUMBER:
            return getBuzzerPush(buzzer, ALL, 0, true);
        case TEAM:
            return getBuzzerPush(buzzer, TEAM, -2, true);
    }
}

Buzzer *Buzzers::getBuzzerPush(Buzzer buzzer[], Buzzer_Info info, int num) {
    return getBuzzerPush(buzzer, info, num, true);
}

Buzzer *Buzzers::getBuzzerPush(Buzzer buzzer[], Buzzer_Info info, int num, bool used) {
    int nb_buzzer_count = 0;
    if (used) {
        nb_buzzer_count = nb_buzzer;
    } else {
        nb_buzzer_count = nb_buzzer_max;
    }
    for (int i = 0; i < nb_buzzer_count; ++i) {
        switch (info) {
            case NUMBER:
                if ((&buzzer[i])->number == num) {
                    if (getValueBuzzer(&buzzer[i])) {
                        return &buzzer[i];
                    }
                }
                break;

            case TEAM:
                if (num == -2) {
                    if ((&buzzer[i])->team != -1) {
                        if (getValueBuzzer(&buzzer[i])) {
                            return &buzzer[i];
                        }
                    }
                } else if ((&buzzer[i])->team == num) {
                    if (getValueBuzzer(&buzzer[i])) {
                        return &buzzer[i];
                    }
                }

                break;

            case ALL:
                if ((&buzzer[i])->used == used) {
                    if (getValueBuzzer(&buzzer[i])) {
                        return &buzzer[i];
                    }
                }
                break;
        }

    }
    return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
bool Buzzers::allBuzzerUsed(Buzzer buzzer[]) {
    for (int i = 0; i < nb_buzzer_max; ++i) {
        if (!((&buzzer[i])->used)) {
            return false;
        }
    }
    return true;
}

bool Buzzers::allBuzzerTeam(Buzzer buzzer[]) {
    for (int i = 0; i < nb_buzzer_max; ++i) {
        if ((((&buzzer[i])->team) == -1) && ((&buzzer[i])->used)) {
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
void Buzzers::resetBuzzerAllScore(Buzzer buzzer[]) {
    for (int i = 0; i < nb_buzzer_max; ++i) {
        (&buzzer[i])->score = 0;
    }
}

void Buzzers::resetBuzzerAllTeam(Buzzer buzzer[]) {
    nb_team = 0;
    for (int i = 0; i < nb_buzzer_max; ++i) {
        (&buzzer[i])->team = -1;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Buzzers::testInitBuzzer(Buzzer buzzer[]) {
    printFunctionName("testInitBuzzer");
    while (!allBuzzerUsed(buzzer)) {
        Buzzer *courantBuzzer = getBuzzerPush(buzzer, false);
        if (courantBuzzer) {
            nb_buzzer = nb_buzzer + 1;
            courantBuzzer->number = nb_buzzer;
            courantBuzzer->used = true;

            ledOffBuzzer(courantBuzzer);
        }
    }
    orderBuzzerUsed(buzzer);
    if (DEV_INFO) {
        Serial.println("initBuzzer donne");
    }
}

void Buzzers::testInitTeam(Buzzer buzzer[]) {
    printFunctionName("testInitTeam");
    ledOnAllBuzzer(buzzer);
    while (!allBuzzerTeam(buzzer)) {
        Buzzer *courantBuzzer = getBuzzerPush(buzzer, TEAM);
        if (courantBuzzer) {
            courantBuzzer->team = nb_team;
            ledOffBuzzer(courantBuzzer);
            displayInfoBuzzer(courantBuzzer);
        }
        if (getValueButtonTest()) {
            nb_team = nb_team + 1;
            Serial.print("nb_team : ");
            Serial.println(nb_team);
            delay(500);
        }
    }
    if (DEV_INFO) {
        Serial.println("initTeam donne");
    }
}

//----------------------------------------------------------------------------------------------------------------------
int Buzzers::scoreTeam(Buzzer buzzer[], int team) {
    int score = 0;
    for (int i = 0; i < nb_buzzer; ++i) {
        if ((((&buzzer[i])->team) == team)) {
            score = score + (&buzzer[i])->score;
        }
    }
    return score;
}

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
Buzzer *Buzzers::orderScoreBuzzer(Buzzer buzzer[], int place) {
    descendingOrder(buzzer, nb_buzzer);
    return (&buzzer[nb_buzzer - 1 - place]);
}

void Buzzers::descendingOrder(Buzzer buzzer[], int tab_size) {
    Buzzer tmp;
    for (int i = 0; i < tab_size; i++)          //On veut remplir la case i du tableau
    {
        for (int j = i + 1; j < tab_size; j++) {                                //Dans les cases suivantes
            if ((buzzer[j].score < buzzer[i].score)) {
                tmp = buzzer[i];              //Si c'est le cas on intervertit les cases
                buzzer[i] = buzzer[j];
                buzzer[j] = tmp;
            }
        }
    }
}

void Buzzers::orderBuzzerUsed(Buzzer buzzer[]) {
    printFunctionName("orderBuzzerUsed");
    Buzzer tmp;
    for (int i = 0; i < nb_buzzer_max; i++)          //On veut remplir la case i du tableau
    {
        for (int j = i + 1; j < nb_buzzer_max; j++) {                                //Dans les cases suivantes
            if (!(buzzer[i].used)) {
                tmp = buzzer[i];              //Si c'est le cas on intervertit les cases
                buzzer[i] = buzzer[j];
                buzzer[j] = tmp;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

int Buzzers::numberSelectBuzzer(Buzzer buzzer[]) {

    Buzzer *courantBuzzer = nullptr;
    while (!courantBuzzer) {
        courantBuzzer = getBuzzerPush(buzzer);
        if (courantBuzzer) {
            ledOnBuzzer(courantBuzzer);
            displayInfoBuzzer(courantBuzzer, NUMBER);
            int millis = superMillis();
            delay(300);
            //waitBuzzerPush(courantBuzzer);
            Serial.println(comptBuzzerPush(courantBuzzer, 1));

        }
    }
    displayInfoBuzzer(courantBuzzer, NUMBER);
    blinkBuzzer(courantBuzzer);
    Serial.println("good");

}

int Buzzers::comptBuzzerPush(Buzzer *buzzer, int num) {
    if (waitBuzzerPush(buzzer)) {
        return comptBuzzerPush(buzzer, num) + 1;
    } else {
        return num;
    }
}


bool Buzzers::waitBuzzerPush(Buzzer *buzzer) {
    waitBuzzerPush(buzzer, wait_time);
}

bool Buzzers::waitBuzzerPush(Buzzer *buzzer, int time) {
    unsigned long long millis = superMillis();
    while (!getBuzzerPush(buzzer, NUMBER, buzzer->number) && ((superMillis() - millis) < time * 1000));
    if (((superMillis() - millis) >= time * 1000)) {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

void Buzzers::print_unsigned_long_long(unsigned long long value) {
    unsigned long msb = value >> 32;
    unsigned long lsb = value & 0xFFFFFFFF;
    if (msb) {
        char buf[21];
        sprintf(buf, "%lu%010lu", msb, lsb);
        Serial.print(buf);
    } else {
        Serial.print(lsb);
    }
}

unsigned long long Buzzers::superMillis() {
    static unsigned long nbRollover = 0;
    static unsigned long previousMillis = 0;
    unsigned long currentMillis = millis();

    if (currentMillis < previousMillis) {
        nbRollover++;
    }
    previousMillis = currentMillis;

    unsigned long long finalMillis = nbRollover;
    finalMillis <<= 32;
    finalMillis += currentMillis;
    return finalMillis;
}

unsigned long long Buzzers::superMicros() {
    static unsigned long nbRollover = 0;
    static unsigned long previousMicros = 0;
    unsigned long currentMicros = micros();

    if (currentMicros < previousMicros) {
        nbRollover++;
    }
    previousMicros = currentMicros;

    unsigned long long finalMicros = nbRollover;
    finalMicros <<= 32;
    finalMicros += currentMicros;
    return finalMicros;
}

//----------------------------------------------------------------------------------------------------------------------
bool Buzzers::getValueButtonTest() {
    if (digitalRead(BUTTON_test) == LOW) {
        Serial.println("Test button push");
        delay(300);
        return true;
    } else {
        return false;
    }
}































































