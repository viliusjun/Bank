#include "bank.h"
#include "priority_queue.h"

// Funkcija, kuri perskaito reikiamą informaciją iš vartotojo.
void readFromUser(double *probability, double *probabilityVIP, int *cashier, int *cashierVIP, int *time, int *timeVIP, int *workTime){
    printf("\nĮveskite paprasto ir VIP kliento atėjimo tikimybes.\n");
    printf("Paprasto kliento: ");
    while (scanf("%lf", probability) != 1 || (*probability) < 0 || (*probability) > 1){
        while (getchar() != '\n')
            ;
        printf("Kliento atėjimo tikimybė turi būti realus arba sveikas skaičius tarp 0 ir 1 imtinai. Bandykite dar kartą:\n");
    }
    printf("VIP kliento: ");
    while (scanf("%lf", probabilityVIP) != 1 || (*probabilityVIP) < 0 || (*probabilityVIP) > 1){
        while (getchar() != '\n')
            ;
        printf("Kliento atėjimo tikimybė turi būti realus arba sveikas skaičius tarp 0 ir 1 imtinai. Bandykite dar kartą:\n");
    }

    printf("\nĮveskite paprastų ir VIP klientų aptarnaujančių kasininkų skaičių.\n");
    printf("Paprasto kliento: ");
    while (scanf("%d", cashier) != 1 || (*cashier) < 1 || (*cashier) > MAX_CASHIERS){
        while (getchar() != '\n')
            ;
        printf("Kasininkų skaičius turi būti tarp 1 ir %d imtinai. Bandykite dar kartą:\n", MAX_CASHIERS);
    }
    printf("VIP kliento: ");
    while (scanf("%d", cashierVIP) != 1 || (*cashierVIP) < 1 || (*cashierVIP) > MAX_CASHIERS){
        while (getchar() != '\n')
            ;
        printf("Kasininkų skaičius turi būti tarp 1 ir %d imtinai. Bandykite dar kartą:\n", MAX_CASHIERS);
    }

    printf("\nĮveskite paprasto ir VIP kliento aptarnavimo laiką.\n");
    printf("Paprasto kliento: ");
    while (scanf("%d", time) != 1 || (*time) < 1 || (*time) > MAX_TIME){
        while (getchar() != '\n')
            ;
        printf("Aptarnavimo laikas turi būti tarp 1 ir %d imtinai. Bandykite dar kartą:\n", MAX_TIME);
    }
    printf("VIP kliento: ");
    while (scanf("%d", timeVIP) != 1 || (*timeVIP) < 1 || (*timeVIP) > MAX_TIME){
        while (getchar() != '\n')
            ;
        printf("Aptarnavimo laikas turi būti tarp 1 ir %d imtinai. Bandykite dar kartą:\n", MAX_TIME);
    }

    printf("\nĮveskite darbo dienos ilgį: ");
    while (scanf("%d", workTime) != 1 || (*workTime) < 1 || (*workTime) > MAX_WORK_TIME){
        while (getchar() != '\n')
            ;
        printf("Darbo dienos ilgis turi būti tarp 1 ir %d imtinai. Bandykite dar kartą:\n", MAX_WORK_TIME);
    }

    return;
}

// Funkcija, kuri grąžina true arba false pagal tai, ar kliento atėjimo tikimybė yra didensnė arba lygi atsitiktine tvarka sugeneruoto skaičiaus reikšmei tarp 0 ir 1.
bool generateClient(double probability){
    double random = (double)rand() / (double)((unsigned)RAND_MAX + 1);

    if (probability >= random){
        return true;
    }

    return false;
}

// Funkcija, kuri grąžiną laisvų kasininkų skaičių.
int freeCashiers(int cashier, int workingCashier[]){
    int free = 0;

    for (int i = 0; i < cashier; ++i){
        if (workingCashier[i] == 0){
            free++;
        }
    }

    return free;
}

// Funkcija, kuri grąžina prioritetą, kuris šiuo metu yra didžiausias prioritetinėje eilėje.
int getPQueuePriority(PriorityQueue *p_queue){
    IF_PRINT_ERROR(isEmptyPQueue(p_queue), ERROR2, 2);
    if (errorno_PQueue == 2){
        int garbage;
        return garbage;
    }

    return p_queue->p_next->priority;
}

// Funkcija, kuri nunulina masyvus.
void resetArrays(int count, int array[]){
    for (int i = 0; i < count; ++i){
        array[i] = 0;
    }

    return;
}

// Funkcija, kuri nunulina dvimačius masyvus.
void reset2DArrays(int count, int array[][2]){
    for (int i = 0; i < count; ++i){
        for (int j = 0; j < 2; ++j){
            array[i][j] = 0;
        }
    }

    return;
}

// Funkcija, kuri sumažina kasininko likusį laiką dirbti su klientu vienu taktu ir praneša apie klientus, kurios baigėm aptarnauti.
void startWorkingWithClients(FILE *protocol, int cashier, int workingCashier[], int storeServicedClients[][2], int priotirty, int *protocolCounter){
    for (int i = 0; i < cashier; ++i){
        if (workingCashier[i] != 0){
            workingCashier[i]--;
            if (workingCashier[i] == 0){
                if (storeServicedClients[i][1] == 1){
                    fprintf(protocol, "\t%d) Paprastas klientas nr. %d baigtas aptarnauti.\n", *protocolCounter, storeServicedClients[i][0]);
                    (*protocolCounter)++;
                    storeServicedClients[i][0] = 0;
                    storeServicedClients[i][1] = 0;
                }
                else if (storeServicedClients[i][1] == 2){
                    fprintf(protocol, "\t%d) VIP klientas nr. %d baigtas aptarnauti.\n", *protocolCounter, storeServicedClients[i][0]);
                    (*protocolCounter)++;
                    storeServicedClients[i][0] = 0;
                    storeServicedClients[i][1] = 0;
                }
            }
        }
    }


    return;
}

// Funkcija, kuri grąžina to kasininko likusį laiką dirbti su klientu, kuriam dirbti liko ilgiausiai.
int leftOverTimeMax(int cashier, int cashierVIP, int workingCashier[], int workingCashierVIP[]){
    int max = 0;

    for (int i = 0; i < cashier; ++i){
        if (workingCashier[i] > max){
            max = workingCashier[i];
        }
    }

    for (int i = 0; i < cashierVIP; ++i){
        if (workingCashierVIP[i] > max){
            max = workingCashierVIP[i];
        }
    }

    return max;
}

// Funkcija, kuri skaičiuoja prastovos laiką.
void checkDownTime(int cashier, int cashierArray[], int downTimeArray[]){
    for (int i = 0; i < cashier; ++i){
        if (cashierArray[i] == 0){
            downTimeArray[i]++;
        }
    }

    return;
}

// Funkcija, kuri padidina kliento laukimo eilėje laiką vienetu.
void incrementWaitingTime(PriorityQueue *p_queue){
    PriorityQueue *p_current = p_queue->p_next;

    while (p_current){
        CustomType *ptrToCustom = &(p_current->value);
        int *ptrToInt = &(ptrToCustom->value2);
        //Incrementing element's value in the queue.
        *ptrToInt += 1;
        
        p_current = p_current->p_next;
    }

    return;
}

// Funkcija, kuri apskaičiuoja paprasto kliento vidutinį laukimo laiką eilėje.
double calculateClientAverageWaitingTime(int waitingTimeCounter, int waitingTime[]){
    if (waitingTimeCounter == 0){
        return 0;
    }

    int sum = 0;
    for (int i = 0; i < waitingTimeCounter; ++i){
        sum += waitingTime[i];
    }

    double average = (double) sum / waitingTimeCounter;

    return average;
}

// Funkcija, kuri apskaičiuoja VIP klieto maksimalų laukimo eilėje laiką.
int calculateVIPClientMaxWaitingTime(int waitingTimeVIPCounter, int waitingTimeVIP[]){
    int max = waitingTimeVIP[0];
    for (int i = 1; i < waitingTimeVIPCounter; ++i){
        if (waitingTimeVIP[i] > max){
            max = waitingTimeVIP[i];
        }
    }

    return max;
}

// Funkcija, kuri išspausdina reikiamus duomenis į protokolą.
void printToFile1(FILE *protocol, int dataArray[], double dataArray2[]){
    fprintf(protocol, "Vilius Junevičius, programų sistemos 1 kursas, 4 grupė, 1 pogrupis\nTrečioji užduotis, 5 variantas\n\n");
    fprintf(protocol, "Sąlyga:\nVIP bankas (ADT: eilė, prioritetinė eilė). Procesas 1: klientai ateina į banką ir kreipiasi į bet kurį laisvą kasininką, jei visi kasininkai užsiėmę, klientai stoja į bendrą eilę,\ntačiau yra VIP klientai, kurie aptarnaujami be eilės, žinoma, jei visi kasininkai užimti VIP klientas irgi turi laukti, netgi gali susidaryti VIP eilė.\nProcesas 2: bankas turi atskirus kasininkus, kurie dirba su paprastais ir VIP klientais. Pradiniai duomenys: paprasto ir VIP kliento atėjimo tikimybės,\npaprastus ir VIP klientus aptarnaujančių kasininkų skaičius, paprasto ir VIP kliento aptarnavimo laikas, darbo dienos ilgis. Įvertinti, kuris procesas tikslingesnis bankui.\nVertinama, pavyzdžiui, pagal tokį kriterijų: suminis kasininkų prastovų laikas, dvigubas suminis kasininkų viršvalandžių laikas (visi klientai, kurie atėjo į banką iki darbo pabaigos,\nturi būti aptarnauti), trigubas vidutinis paprasto kliento laukimo laikas, dešimtgubas VIP kliento maksimalaus laukimo laikas.\n\n");
    fprintf(protocol, "Apribojimai:\n\t1) Maksimalus paprastų klientų ir VIP klientų kasininkų skaičius: %d.\n\t2) Masksimalus paprastų klientų ir VIP klientų aptarnavimo laikas: %d min.\n\t3) Maksimalus darbo dienos ilgis: %d min.\n\n", MAX_CASHIERS, MAX_TIME, MAX_WORK_TIME);
    fprintf(protocol, "Uždavinio laikas yra skačiuojamas laiko momentais T (minutėmis).\n\n\n\n");
    fprintf(protocol, "I DALIS - ĮVESTI DUOMENYS\n\n\t1) Paprasto kliento atėjimo tikimybė: %.2f.\n\t2) VIP kliento atėjimo tikimybė: %.2f.\n\t3) Paprastus klientus aptarnaujančių kasininkų skaičius: %d.\n\t4) VIP klientus aptarnaujančių kasininkų skaičius: %d.\n\t5) Paprasto kliento aptarnavimo laikas: %d min.\n\t6) VIP kliento aptarnavimo laikas: %d min.\n\t7) Darbo dienos ilgis: %d min.\n\n", dataArray2[0], dataArray2[1], dataArray[0], dataArray[1], dataArray[2], dataArray[3], dataArray[4]);
    fprintf(protocol, "\n\nII DALIS - VYKDYMAS\n");

    return;
}

// Funkcija, kuri iššspausdina informaciją apie laukenčių klientų kiekį eilėje.
void howMuchClientsWaiting(FILE *protocol, PriorityQueue *p_queue, int *protocolCounter){
    if (isEmptyPQueue(p_queue)){
        return;
    }

    int count = 0, countVIP = 0;

    PriorityQueue *p_current = p_queue->p_next;

    while (p_current != NULL){
        if (p_current->priority == 1){
            count++;
        }
        if (p_current->priority == 2){
            countVIP++;
        }

        p_current = p_current->p_next;
    }
    
    if (countVIP != 0){
        fprintf(protocol, "\t%d) Eilėje laukenčių VIP klientų kiekis -  %d:", *protocolCounter, countVIP);
        (*protocolCounter)++;
        printValues(protocol, p_queue, 2, 1);
    }
    if (count != 0){
        fprintf(protocol, "\t%d) Eilėje laukenčių paprastų klientų kiekis -  %d:", *protocolCounter, count);
        (*protocolCounter)++;
        printValues(protocol, p_queue, 1, 1);
    }

    return;
}

// Funkcija, kuri išspausdina informaciją apie aptarnaujamų klientų kiekį.
void howMuchClientsServiced(FILE *protocol, int cashier, int workingCashier[], int cashierVIP, int workingCashierVIP[], int *protocolCounter, int storeServicedClients[][2], int storeServicedClientsVIP[][2]){
    int count = 0, countVIP = 0;

    for (int i = 0; i < cashier; ++i){
        if (workingCashier[i] != 0){
            count++;
        }
    }
 
    for (int i = 0; i < cashierVIP; ++i){
        if (workingCashierVIP[i] != 0){
            countVIP++;
        }
    }

    int flag = 0, flag1 = 0;

    if (countVIP + count != 0){
        fprintf(protocol, "\t%d) Šiuo metu aptarnaujamų klientų kiekis %d:\n", *protocolCounter, countVIP + count);
        (*protocolCounter)++;
        flag = 0;
        flag1 = 0;

        for (int i = 0; i < cashier; ++i){
            if (workingCashier[i] != 0){
                if (storeServicedClients[i][1] == 2){
                    if (flag == 0){
                        fprintf(protocol, "\t\t(VIP klientai nr. ");
                        flag = 1;
                    }
                    fprintf(protocol, "%d, ", storeServicedClients[i][0]);
                }
            }
        }
        for (int i = 0; i < cashierVIP; ++i){
            if (workingCashierVIP[i] != 0){
                if (storeServicedClientsVIP[i][1] == 2){
                    if (flag == 0){
                        fprintf(protocol, "\t\t(VIP klientai nr. ");
                        flag = 1;
                    }
                    fprintf(protocol, "%d, ", storeServicedClientsVIP[i][0]);
                }
            }
        }
        if (flag == 1){
            fseek(protocol, -2, SEEK_CUR);
            fprintf(protocol, ").\n");
        }

        for (int i = 0; i < cashier; ++i){
            if (workingCashier[i] != 0){
                if (storeServicedClients[i][1] == 1){
                    if (flag1 == 0){
                        fprintf(protocol, "\t\t(Paprasti klientai nr. ");
                        flag1 = 1;
                    }
                    fprintf(protocol, "%d, ", storeServicedClients[i][0]);
                }
            }
        }
        for (int i = 0; i < cashierVIP; ++i){
            if (workingCashierVIP[i] != 0){
                if (storeServicedClientsVIP[i][1] == 1){
                    if (flag1 == 0){
                        fprintf(protocol, "\t\t(Paprasti klientai nr. ");
                        flag1 = 1;
                    }
                    fprintf(protocol, "%d, ", storeServicedClientsVIP[i][0]);
                }
            }
        }
        if (flag1 == 1){
            fseek(protocol, -2, SEEK_CUR);
            fprintf(protocol, ").\n");
        }

    }

    return;    
}

// Funkcija, kuri išspausdina visų eilėje esančių elementų prioritetus.
void printValues(FILE *protocol, PriorityQueue *p_queue, int priority, int flag){
    if (isEmptyPQueue(p_queue)){
        return ;
    }

    PriorityQueue *p_current = p_queue->p_next;
    if (flag == 1){
        fprintf(protocol, "\n\t\t(klientai nr. ");
    }

    while (p_current != NULL){
        Data value = p_current->value;

        if (p_current->priority == priority){
            fprintf(protocol, DATA ", ", value);
        }

        p_current = p_current->p_next;
    }
    fseek(protocol, -2, SEEK_CUR);
    fprintf(protocol, ").\n");

    return;
}