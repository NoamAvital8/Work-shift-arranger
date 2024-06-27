#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_EMPLOYEES 5
#define MAX_NAME_LEN 5

#define EMPLOYEE1 "  Ido"
#define EMPLOYEE2 " Noam"
#define EMPLOYEE3 "  Adi"    //NAMES
#define EMPLOYEE4 " Leon"
#define EMPLOYEE5 "Hadar"

#define MAX_IN_A_ROW1 3
#define MAX_IN_A_ROW2 2
#define MAX_IN_A_ROW3 2     //MAX SHIFTS
#define MAX_IN_A_ROW4 2
#define MAX_IN_A_ROW5 2
#define MAX_NUM_OF_SHIFTS 8
#define MIN_NUM_OF_SHIFTS 7

#define NIGHT_WORTH 1
#define DAY_WORTH 1.5
#define FRIDAY_NIGHT_WORTH 2.2
#define SATURDAY_NIGHT_WORTH 1.8

#define PRINT_SIDOR 1
#define NUMBER_OF_SAVED_SHIFTS 11
#define FAIRNESS 0
#define TRIES 3000
#define DAYS_IN_MONTH 30
#define FIRST_DAY 1
#define MONTH 6

int highest_day = 0;
int numfair = 0;
double try[TRIES - 2];
int saveshift[3][NUMBER_OF_SAVED_SHIFTS];

int noonecanworkhere(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH]);

void resettry();

bool istaken(int date, int time);

int takenby(int date, int time);

void placesavedshifts(int arrangement[2][DAYS_IN_MONTH]);

char *getday(int date);

void getsavedshifts();

char *getname(int employeenum);

int max_in_a_row(int employeenum);

double findmin(double fairiest[TRIES - 2]);

double points(int employeenum, int arrangement[2][DAYS_IN_MONTH]);

double distance(double a, double b);

int rounddown(int num);

double isfair(int arrangement[2][DAYS_IN_MONTH]);

int how_many_shifts(int arrangement[2][DAYS_IN_MONTH], int employeenum);

int num_of_fridays_shifts();

int num_of_saturday_shifts();

int nexttime(int date, int time);

int nextdate(int date, int time);

int lasttime(int date, int time);

int lastdate(int date, int time);

void printcons(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH]);

int numofshifts(int employeenum, int arrangement[2][DAYS_IN_MONTH]);

void cleararray(int array[2][DAYS_IN_MONTH]);

void printsidor(int arrangement[2][DAYS_IN_MONTH]);

bool ismorning(int date);

double max(double a, double b);

double min(double a, double b);

void constraint(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH], char names[NUMBER_OF_EMPLOYEES][MAX_NAME_LEN]);

bool available(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH],
               int employeenum, int date, int time, int arrangement[2][DAYS_IN_MONTH]);

bool backtracking(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH], char names[NUMBER_OF_EMPLOYEES][MAX_NAME_LEN],
                  int employeenum, int date, int time, int arrangement[2][DAYS_IN_MONTH]);

bool lastday(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH], int arrangement[2][DAYS_IN_MONTH]);

int main() {
    resettry();
    bool done = false;
    int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH] = {0};
    if (noonecanworkhere(cons)) {
        printf("\nno one can work in - %d\n", noonecanworkhere(cons));
    }
    char names[NUMBER_OF_EMPLOYEES + 1][MAX_NAME_LEN] = {0};
    int arrangement[2][DAYS_IN_MONTH] = {0};
    constraint(cons, names);
    cleararray(arrangement);
    getsavedshifts();
    placesavedshifts(arrangement);
    for (int i = 1; i <= NUMBER_OF_EMPLOYEES; ++i) {
        if (ismorning(0)) {
            if (backtracking(cons, names, i, 0, 1, arrangement)) { done = true; }
        }
        if (!ismorning(0)) {
            if (ismorning(1)) {
                if (backtracking(cons, names, i, 1, 0,
                                 arrangement)) { done = true; }
            } else {
                if (backtracking(cons, names, i, 1, 1,
                                 arrangement)) { done = true; }
                else {
                }
            }
        }
        if (done) { break; }
    }
    if (FAIRNESS != 0) {
        printf("\n");
        for (int i = 1; i <= NUMBER_OF_EMPLOYEES; ++i) {
            printf("%s points : %.2lf  ,  shifts : %d\n", getname(i), points(i, arrangement),
                   numofshifts(i, arrangement));
        }
        printf("\n");
        printsidor(arrangement);
        printf("\n fairness : %.2lf", isfair(arrangement));
    } else printf("\nhigh day: %d \nmin is : %.2lf", highest_day+16, findmin(try));

    return 0;
}

void constraint(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH], char names[NUMBER_OF_EMPLOYEES][MAX_NAME_LEN]) {
    for (int i = 1; i <= NUMBER_OF_EMPLOYEES; ++i) {
        int date = 1;
        printf("enter cons for %s ('0' to end):\n", getname(i));
        while (date) {
            scanf("%d", &date);
            if (date >= 16) {
                cons[i * 2 - 1][date - 16] = 1;
                cons[i * 2 - 2][date - 16] = 1;
            }
            if (date < 16 && date > 0) {
                cons[i * 2 - 1][DAYS_IN_MONTH - 16 + date] = 1;
                cons[i * 2 - 2][DAYS_IN_MONTH - 16 + date] = 1;
            }
        }
        date = 1;
        printf("enter cons for %s - mornings ('0' to end):\n", getname(i));
        while (date) {
            scanf("%d", &date);
            if (date >= 16) { cons[i * 2 - 2][date - 16] = 1; }
            if (date < 16 && date > 0) { cons[i * 2 - 2][DAYS_IN_MONTH - 16 + date] = 1; }
        }
        date = 1;
        printf("enter cons for %s - nights ('0' to end):\n", getname(i));
        while (date) {
            scanf("%d", &date);
            if (date >= 16) { cons[i * 2 - 1][date - 16] = 1; }
            if (date < 16 && date > 0) { cons[i * 2 - 1][DAYS_IN_MONTH - 16 + date] = 1; }
        }
    }
}

bool available(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH],
               int employeenum, int date, int time, int arrangement[2][DAYS_IN_MONTH]) {
    if (istaken(date, time) && takenby(date, time) != employeenum) {
        return false;
    }
    if (how_many_shifts(arrangement, employeenum) >= MAX_NUM_OF_SHIFTS) { return false; }
    if (cons[employeenum * 2 - 2 + time][date] == 1) { return false; }
    else {
        if (time == 0 && arrangement[1][(int) max(date - 1, 0)] == employeenum) {
            return false;
        }
        if (time == 0 && arrangement[1][date] == employeenum) {
            return false;
        }
        if (time == 1 && arrangement[0][date] == employeenum) {
            return false;
        }
        if (time == 1 && arrangement[0][date + 1] == employeenum) {
            return false;
        }
    }
    bool toomanyshifts = true;
    if (date >= max_in_a_row(employeenum)) {
        for (int i = 1; i <= max_in_a_row(employeenum); ++i) {
            if ((arrangement[1][date - i] != employeenum) && (arrangement[0][date - i] != employeenum)) {
                return true;
            }
        }
    }
    if (date >= max_in_a_row(employeenum) && toomanyshifts) { return false; }
    return true;
}

bool backtracking(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH], char names[NUMBER_OF_EMPLOYEES][MAX_NAME_LEN],
                  int employeenum, int date, int time, int arrangement[2][DAYS_IN_MONTH]) {
    if (available(cons, employeenum, lastdate(date, time), lasttime(date, time), arrangement)) {
        arrangement[lasttime(date, time)][lastdate(date, time)] = employeenum;
        if (PRINT_SIDOR) { printsidor(arrangement); }
        if (date > highest_day) {
            highest_day = date;
        }
    } else { return false; }
    if (date > DAYS_IN_MONTH - 2) {
        if (lastday(cons, arrangement)) {
            for (int i = 1; i <= NUMBER_OF_EMPLOYEES; ++i) {
                if (numofshifts(i, arrangement) < MIN_NUM_OF_SHIFTS) {
                    return false;
                }
            }
            if (isfair(arrangement) > FAIRNESS && numfair < TRIES) {
                try[numfair++] = isfair(arrangement);
                return false;
            } else return true;
        } else {
            return false;
        }
    }
    for (int i = 1; i <= NUMBER_OF_EMPLOYEES; ++i) {
        if (backtracking(cons, names, i, nextdate(date, time), nexttime(date, time), arrangement)) {
            return true;
        } else {
            arrangement[time][date] = 0;
        }
    }
    return false;
}

bool ismorning(int date) {
    int sum = date + FIRST_DAY;
    if (((sum + 1) % 7) == 0 || (sum % 7) == 0) { return true; }
    return false;
}

double max(double a, double b) {
    if (a > b) { return a; }
    return b;
}

double min(double a, double b) {
    if (a < b) { return a; }
    return b;
}

void printsidor(int arrangement[2][DAYS_IN_MONTH]) {
    printf("\n\n\n\n    day                   morning                   night\n\n");
    for (int i = 0; i < DAYS_IN_MONTH; ++i) {
        printf("%s             %s                   %s\n\n",
               getday(i), getname(arrangement[0][i]), getname(arrangement[1][i]));
    }
}

void cleararray(int array[2][DAYS_IN_MONTH]) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < DAYS_IN_MONTH; ++j) {
            array[i][j] = 0;
        }
    }
}

void printcons(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH]) {
    for (int i = 0; i < NUMBER_OF_EMPLOYEES * 2; ++i) {
        for (int j = 0; j < DAYS_IN_MONTH; ++j) {
            printf(" %d ", cons[i][j]);
        }
    }
}

int lasttime(int date, int time) {
    if (time == 0) {
        return 1;
    } else {
        if (ismorning(date)) {
            return 0;
        } else return 1;
    }
}

int lastdate(int date, int time) {
    if (time == 0) {
        return (date - 1);
    } else {
        if (ismorning(date)) {
            return date;
        } else return date - 1;
    }
}

int nexttime(int date, int time) {
    if (time == 0) {
        return 1;
    } else {
        if (ismorning(date + 1)) {
            return 0;
        } else return 1;
    }
}

int nextdate(int date, int time) {
    if (time == 0) {
        return date;
    } else {
        return (date + 1);
    }
}

bool lastday(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH], int arrangement[2][DAYS_IN_MONTH]) {
    for (int i = 1; i <= NUMBER_OF_EMPLOYEES; ++i) {
        if (available(cons, i, DAYS_IN_MONTH - 1, 1, arrangement)) {
            arrangement[1][DAYS_IN_MONTH - 1] = i;
            return true;
        }
    }
    return false;
}

int num_of_fridays_shifts() {
    int sum = 0;
    for (int i = 0; i < DAYS_IN_MONTH; ++i) {
        if ((i + FIRST_DAY) % 7 == 0) {
            sum++;
        }
    }
    return sum;
}

int num_of_saturday_shifts() {
    int sum = 0;
    for (int i = 0; i < DAYS_IN_MONTH; ++i) {
        if ((i + FIRST_DAY + 1) % 7 == 0) {
            sum++;
        }
    }
    return sum;
}

double isfair(int arrangement[2][DAYS_IN_MONTH]) {
    double avg = 0, fairness = 0;
    /*/ avg = avg + NIGHT_WORTH * (DAYS_IN_MONTH - num_of_fridays_shifts() - num_of_saturday_shifts());
     avg = avg + DAY_WORTH * (num_of_fridays_shifts() + num_of_saturday_shifts());
     avg = avg + num_of_saturday_shifts() * SATURDAY_NIGHT_WORTH;
     avg = avg + num_of_fridays_shifts() * FRIDAY_NIGHT_WORTH;
     avg = avg / NUMBER_OF_EMPLOYEES;/*/
    for (int i = 1; i <= NUMBER_OF_EMPLOYEES; ++i) {
        avg = avg + points(i, arrangement);
    }
    avg = avg / NUMBER_OF_EMPLOYEES;
    for (int i = 1; i <= NUMBER_OF_EMPLOYEES; ++i) {
        fairness = fairness + distance(avg, points(i, arrangement));
    }
    return fairness;
}

int rounddown(int num) {
    int final = 0;
    while (num >= 1) {
        final++;
        num--;
    }
    return final;
}

double distance(double a, double b) {
    return (max(a, b) - min(a, b));
}

double points(int employeenum, int arrangement[2][DAYS_IN_MONTH]) {
    double points = 0;
    for (int i = 0; i <= 1; ++i) {
        for (int j = 0; j < DAYS_IN_MONTH; ++j) {
            if (arrangement[i][j] == employeenum) {
                if (i == 0) {
                    points = points + DAY_WORTH;
                }
                if (i == 1 && ismorning(j) && ismorning(j + 1)) {
                    points = points + FRIDAY_NIGHT_WORTH;
                }
                if (i == 1 && ismorning(j) && ismorning(j - 1)) {
                    points = points + SATURDAY_NIGHT_WORTH;
                }
                if (!ismorning(j) && i == 1) {
                    points = points + NIGHT_WORTH;
                }
            }
        }
    }
    return points;
}

int how_many_shifts(int arrangement[2][DAYS_IN_MONTH], int employeenum) {
    int number = 0;
    for (int i = 0; i <= 1; ++i) {
        for (int j = 0; j < DAYS_IN_MONTH; ++j) {
            if (arrangement[i][j] == employeenum) { number++; }
        }
    }
    return number;
}

double findmin(double fairiest[TRIES - 2]) {
    double min = fairiest[0];
    for (int i = 1; i < TRIES - 2; ++i) {
        if (fairiest[i] < min) {
            min = fairiest[i];
        }
    }
    return min;
}

int max_in_a_row(int employeenum) {
    switch (employeenum) {
        case 1:
            return MAX_IN_A_ROW1;
        case 2:
            return MAX_IN_A_ROW2;
        case 3:
            return MAX_IN_A_ROW3;
        case 4:
            return MAX_IN_A_ROW4;
        case 5:
            return MAX_IN_A_ROW5;
        default:
            return 0;
    }
}

char *getday(int date) {
    int x = date + 16, y = MONTH;
    char *final;
    final = malloc(14);
    if (x > DAYS_IN_MONTH) {
        x = x - DAYS_IN_MONTH;
        y++;
    }
    while (date + FIRST_DAY > 7) {
        date = date - 7;
    }
    switch (date + FIRST_DAY) {
        case 1:
            strcpy(final, "sunday    ");
            break;
        case 2:
            strcpy(final, "monday    ");
            break;
        case 3:
            strcpy(final, "thusday   ");
            break;
        case 4:
            strcpy(final, "wednesday ");
            break;
        case 5:
            strcpy(final, "thursday  ");
            break;
        case 6:
            strcpy(final, "friday    ");
            break;
        case 7:
            strcpy(final, "saturday  ");
            break;
        default:
            strcpy(final, "problem  ");
    }
    sprintf(final, "%s%d/%d", final, x, y);
    return final;
}

char *getname(int employeenum) {
    switch (employeenum) {
        case 0:
            return "     ";
        case 1:
            return EMPLOYEE1;
        case 2:
            return EMPLOYEE2;
        case 3:
            return EMPLOYEE3;
        case 4:
            return EMPLOYEE4;
        case 5:
            return EMPLOYEE5;
        default:
            return "problem";
    }
}

int numofshifts(int employeenum, int arrangement[2][DAYS_IN_MONTH]) {
    int num = 0;
    for (int i = 0; i <= 1; ++i) {
        for (int j = 0; j < DAYS_IN_MONTH; ++j) {
            if (arrangement[i][j] == employeenum) {
                num++;
            }
        }
    }
    return num;
}


void getsavedshifts() {
    int x = 1, y, z;
    for (int i = 0; i < NUMBER_OF_SAVED_SHIFTS; ++i) {
        printf("enter date you want to save :\n");
        scanf("%d", &x);
        if (x >= 16) {
            x = x - 16;
        } else x = x + DAYS_IN_MONTH - 16;
        saveshift[0][i] = x;
        printf("enter time you want to save( 0-morning , 1-night ) :\n");
        scanf("%d", &y);
        saveshift[1][i] = y;
        printf("which employee ? :(1-%s,   2-%s,   3-%s,   4-%s,   5-%s, )\n", EMPLOYEE1, EMPLOYEE2, EMPLOYEE3,
               EMPLOYEE4, EMPLOYEE5);
        scanf("%d", &z);
        saveshift[2][i] = z;
    }
}

bool istaken(int date, int time) {
    for (int i = 0; i < NUMBER_OF_SAVED_SHIFTS; ++i) {
        if (saveshift[0][i] == date && saveshift[1][i] == time)
            return true;
    }
    return false;
}

void placesavedshifts(int arrangement[2][DAYS_IN_MONTH]) {
    for (int i = 0; i < NUMBER_OF_SAVED_SHIFTS; ++i) {
        arrangement[saveshift[1][i]][saveshift[0][i]] = saveshift[2][i];
    }
}

int takenby(int date, int time) {
    for (int i = 0; i < NUMBER_OF_SAVED_SHIFTS; ++i) {
        if (saveshift[0][i] == date && saveshift[1][i] == time) {
            return saveshift[2][i];
        }
    }
    return 0;
}

int noonecanworkhere(int cons[NUMBER_OF_EMPLOYEES * 2][DAYS_IN_MONTH]) {
    for (int i = 0; i < DAYS_IN_MONTH; ++i) {
        if (cons[0][i] == 1 && cons[2][i] == 1 && cons[4][i] == 1 && cons[6][i] == 1 && cons[8][i] == 1) {
            if (i < DAYS_IN_MONTH - 16) {
                return i + 16;
            } else return i + 16 - DAYS_IN_MONTH;
        }
        if (cons[1][i] == 1 && cons[3][i] == 1 && cons[5][i] == 1 && cons[7][i] == 1 && cons[9][i] == 1) {
            if (i < DAYS_IN_MONTH - 16) {
                return i + 16;
            } else return i + 16 - DAYS_IN_MONTH;
        }
    }
    return 0;
}

void resettry() {
    for (int i = 0; i < TRIES - 2; ++i) {
        try[i] = 100;
    }
}