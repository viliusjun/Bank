// Vilius Junevičius, programų sistemos VU 1 kursas, 4 grupė, 1 pogrupis
// vilius.junevcius@mif.stud.vu.lt
// Trečioji ADS užduotis, 5 variantas

/*
Sąlyga:
VIP bankas (ADT: eilė, prioritetinė eilė). Procesas 1: klientai ateina į banką ir kreipiasi į bet kurį laisvą kasininką,
jei visi kasininkai užsiėmę, klientai stoja į bendrą eilę, tačiau yra VIP klientai, kurie aptarnaujami be eilės, žinoma,
jei visi kasininkai užimti VIP klientas irgi turi laukti, netgi gali susidaryti VIP eilė. Procesas 2: bankas turi atskirus kasininkus,
kurie dirba su paprastais ir VIP klientais. Pradiniai duomenys: paprasto ir VIP kliento atėjimo tikimybės, paprastus ir VIP klientus
aptarnaujančių kasininkų skaičius, paprasto ir VIP kliento aptarnavimo laikas, darbo dienos ilgis. Įvertinti, kuris procesas tikslingesnis
bankui. Vertinama, pavyzdžiui, pagal tokį kriterijų: suminis kasininkų prastovų laikas, dvigubas suminis kasininkų viršvalandžių laikas
(visi klientai, kurie atėjo į banką iki darbo pabaigos, turi būti aptarnauti), trigubas vidutinis paprasto kliento laukimo laikas,
dešimtgubas VIP kliento maksimalaus laukimo laikas.
*/

// Programa, kuri išsprendžiau aukščiau pateiktą sąlygą.
// Programos paleidimas:
//   make

// Programa suskaidyta į 3 dalis:
// 1. main.c - Programos pagrindinis failas.
// 2. bank.h - headerio failas, kuriama surašyti funkcijų aprašai ir maskimalios reikšmės.
// 3. bank.c - funkcijų failas, kuriame surašyti visi funkcijų apibrėžimai ir informacija apie funkcijas.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "priority_queue.h"
#include "bank.h"

#define FILE_NAME "Protokolas.txt"

int main(){
    srand(time(NULL));

    FILE *protocol;
    protocol = fopen(FILE_NAME, "w");

    if (protocol == NULL){
        printf("Error! Could not open the file.\n");
        return 0;
    }

    double probability, probabilityVIP;
    int cashier, cashierVIP;
    int time, timeVIP;
    int workTime;

    readFromUser(&probability, &probabilityVIP, &cashier, &cashierVIP, &time, &timeVIP, &workTime);

    int dataArray[5] = {cashier, cashierVIP, time, timeVIP, workTime};
    double dataArray2[2] = {probability, probabilityVIP};

    printToFile1(protocol, dataArray, dataArray2);

    // Išimtis: klientai neatvyksta į banką.
    if (probability == 0 && probabilityVIP == 0){
        fprintf(protocol, "\nVykdymas nevyksta, nes klientai neatvyksta į banką.\n\n");
        fprintf(protocol, "\nIII DALIS - SPRENDINIAI\n\n");
        fprintf(protocol, "\nSprendinių nėra, nes klientai neatvyksta į banką.\n\n");
        fclose(protocol);
        return 0;
    }

    // Procesas 1:
    fprintf(protocol, "\nPROCESAS NR. 1 - VIP ir paprasti klientai yra aptarnaujami bet kuriuose laisvose kasose. VIP klientai yra aptarnaujami pirmiau negu paprasti klientai.");
    PriorityQueue *array[3];
    int queueCount = 0, clientCount = 0, VIPClientCount = 0, waitingTimeCounter = 0, waitingTimeVIPCounter = 0, protocolCounter = 1;
    int workingCashier[cashier], workingCashierVIP[cashierVIP], downTimeCashier[cashier], downTimeCashierVIP[cashierVIP];
    int waitingTime[workTime], waitingTimeVIP[workTime], storeServicedClients[cashier][2], storeServicedClientsVIP[cashierVIP][2];
    resetArrays(cashier, workingCashier);
    resetArrays(cashierVIP, workingCashierVIP);
    resetArrays(cashier, downTimeCashier);
    resetArrays(cashierVIP, downTimeCashierVIP);
    resetArrays(workTime, waitingTime);
    resetArrays(workTime, waitingTimeVIP);
    reset2DArrays(cashier, storeServicedClients);
    reset2DArrays(cashierVIP, storeServicedClientsVIP);
    createEmptyPQueue(&(array[queueCount]), SIZE);

    for (int i = 0; i < workTime; ++i){
        protocolCounter = 1;
        fprintf(protocol, "\n\nT = %d min.\n", i);

        // Generuojam klientus
        if (generateClient(probability) == true){
            clientCount++;
            fprintf(protocol, "\t%d) Atėjo paprastas klietas nr. %d.\n", protocolCounter, clientCount);
            protocolCounter++;
            CustomType PriorityQueueDataType = {clientCount, 0};
            insertPQueue(&(array[queueCount]), PriorityQueueDataType, 1);
        }

        if (generateClient(probabilityVIP) == true){
            VIPClientCount++;
            fprintf(protocol, "\t%d) Atėjo VIP klientas nr. %d.\n", protocolCounter, VIPClientCount);
            protocolCounter++;
            CustomType PriorityQueueDataType = {VIPClientCount, 0};
            insertPQueue(&(array[queueCount]), PriorityQueueDataType, 2);
        }

        // Priiminėjam klientus
        while ((freeCashiers(cashier, workingCashier) + freeCashiers(cashierVIP, workingCashierVIP)) > 0 && !(isEmptyPQueue(array[queueCount]))){

            Data num = getPQueue(array[queueCount]);
            int priority = getPQueuePriority(array[queueCount]);
            int waitingTimeClient = removePQueue(&array[queueCount]);

            if (errorno_PQueue == 1){
                errorno_PQueue = 0;
                fprintf(protocol, ERROR1 "\n");
            }
            else {
                int flag = 0;

                for (int i = 0; i < cashier; ++i){
                    if (workingCashier[i] == 0){
                        if (priority == 2){
                            waitingTimeVIP[waitingTimeVIPCounter] = waitingTimeClient;
                            waitingTimeVIPCounter++;
                            fprintf(protocol, "\t%d) VIP klientas nr. " DATA " yra priimamas aptarnauti į %d-ą bendrą kasą.\n", protocolCounter, num.value, i + 1);
                            protocolCounter++;
                            workingCashier[i] = timeVIP;
                            storeServicedClients[i][0] = num.value;
                            storeServicedClients[i][1] = priority;
                            flag = 1;
                            break;
                        }
                        else if (priority == 1){
                            waitingTime[waitingTimeCounter] = waitingTimeClient;
                            waitingTimeCounter++;
                            fprintf(protocol, "\t%d) Paprastas klientas nr. " DATA " yra priimamas aptarnauti į %d-ą bendrą kasą.\n",protocolCounter, num.value, i + 1);
                            protocolCounter++;
                            workingCashier[i] = time;
                            storeServicedClients[i][0] = num.value;
                            storeServicedClients[i][1] = priority;
                            flag = 1;
                            break;
                        }
                    }
                }
                
                if (flag == 0){
                    for (int i = 0; i < cashierVIP; ++i){
                        if (workingCashierVIP[i] == 0){
                            if (priority == 2){
                                waitingTimeVIP[waitingTimeVIPCounter] = waitingTimeClient;
                                waitingTimeVIPCounter++;
                                fprintf(protocol, "\t%d) VIP klientas nr. " DATA " yra priimamas aptarnauti į %d-ą bendrą kasą.\n",protocolCounter, num.value, i + 1 + cashier);
                                protocolCounter++;
                                workingCashierVIP[i] = timeVIP;
                                storeServicedClientsVIP[i][0] = num.value;
                                storeServicedClientsVIP[i][1] = priority;
                                break;
                            }
                            else if (priority == 1){
                                waitingTime[waitingTimeCounter] = waitingTimeClient;
                                waitingTimeCounter++;
                                fprintf(protocol, "\t%d) Paprastas klientas nr. " DATA " yra priimamas aptarnauti į %d-ą bendrą kasą.\n",protocolCounter, num.value, i + 1 + cashier);
                                protocolCounter++;
                                workingCashierVIP[i] = time;
                                storeServicedClientsVIP[i][0] = num.value;
                                storeServicedClientsVIP[i][1] = priority;
                                break;
                            }
                        }
                    }
                }

                flag = 0;
            }
            
        }

        // Patikrinam, ar kasininkas dirbs (prastova), ir padidinam klientų laukimo laiką
        checkDownTime(cashier, workingCashier, downTimeCashier);
        checkDownTime(cashierVIP, workingCashierVIP, downTimeCashierVIP);
        incrementWaitingTime(array[queueCount]);

        // Pradeda dirbt su klientais
        startWorkingWithClients(protocol, cashier, workingCashier, storeServicedClients, 1, &protocolCounter);
        startWorkingWithClients(protocol, cashierVIP, workingCashierVIP, storeServicedClientsVIP, 2, &protocolCounter);

        // Spausdinam informaciją apie eilėje laukenčius ir aptarnaujamus klientus
        howMuchClientsServiced(protocol, cashier, workingCashier, cashierVIP, workingCashierVIP, &protocolCounter, storeServicedClients, storeServicedClientsVIP);
        howMuchClientsWaiting(protocol, array[queueCount], &protocolCounter);
    }


    // Proceso 1 virsvalandziai:
    fprintf(protocol, "\n\n\nPROCESO NR. 1 VIRSVALNDZIAI:");
    int overTime1 = 0;
    
    while (!(isEmptyPQueue(array[queueCount]))){
        protocolCounter = 1;
        overTime1++;
        fprintf(protocol, "\n\nT = %d min. (Viršvalandis nr. %d):\n", overTime1 + workTime, overTime1);

        // priiminėjam klientus
        while ((freeCashiers(cashier, workingCashier) + freeCashiers(cashierVIP, workingCashierVIP)) > 0 && !(isEmptyPQueue(array[queueCount]))){
            Data num = getPQueue(array[queueCount]);
            int priority = getPQueuePriority(array[queueCount]);
            int waitingTimeClient = removePQueue(&array[queueCount]);

            if (errorno_PQueue == 1){
                errorno_PQueue = 0;
                fprintf(protocol, ERROR1 "\n");
            }
            else {
                int flag = 0;

                for (int i = 0; i < cashier; ++i){
                    if (workingCashier[i] == 0){
                        if (priority == 2){
                            waitingTimeVIP[waitingTimeVIPCounter] = waitingTimeClient;
                            waitingTimeVIPCounter++;
                            fprintf(protocol, "\t%d) VIP klientas nr. " DATA " yra priimamas aptarnauti į %d-ą bendrą kasą.\n", protocolCounter, num.value, i + 1);
                            protocolCounter++;
                            workingCashier[i] = timeVIP;
                            storeServicedClients[i][0] = num.value;
                            storeServicedClients[i][1] = priority;
                            flag = 1;
                            break;
                        }
                        else if (priority == 1){
                            waitingTime[waitingTimeCounter] = waitingTimeClient;
                            waitingTimeCounter++;
                            fprintf(protocol, "\t%d) Paprastas klientas nr. " DATA " yra priimamas aptarnauti į %d-ą bendrą kasą.\n", protocolCounter, num.value, i + 1);
                            protocolCounter++;
                            workingCashier[i] = time;
                            storeServicedClients[i][0] = num.value;
                            storeServicedClients[i][1] = priority;
                            flag = 1;
                            break;
                        }
                    }
                }
                
                if (flag == 0){
                    for (int i = 0; i < cashierVIP; ++i){
                        if (workingCashierVIP[i] == 0){
                            if (priority == 2){
                                waitingTimeVIP[waitingTimeVIPCounter] = waitingTimeClient;
                                waitingTimeVIPCounter++;
                                fprintf(protocol, "\t%d) VIP klientas nr. " DATA " yra priimamas aptarnauti į %d-ą bendrą kasą.\n", protocolCounter, num.value, i + 1 + cashier);
                                protocolCounter++;
                                workingCashierVIP[i] = timeVIP;
                                storeServicedClientsVIP[i][0] = num.value;
                                storeServicedClientsVIP[i][1] = priority;
                                break;
                            }
                            else if (priority == 1){
                                waitingTime[waitingTimeCounter] = waitingTimeClient;
                                waitingTimeCounter++;
                                fprintf(protocol, "\t%d) Paprastas klientas nr. " DATA " yra priimamas aptarnauti į %d-ą bendrą kasą.\n", protocolCounter, num.value, i + 1 + cashier);
                                protocolCounter++;
                                workingCashierVIP[i] = time;
                                storeServicedClientsVIP[i][0] = num.value;
                                storeServicedClientsVIP[i][1] = priority;
                                break;
                            }
                        }
                    }
                }

                flag = 0;
            }
        }

        // Patikrinam, ar kasininkas dirbs (prastova)
        checkDownTime(cashier, workingCashier, downTimeCashier);
        checkDownTime(cashierVIP, workingCashierVIP, downTimeCashierVIP);
        incrementWaitingTime(array[queueCount]);

        // Pradeda dirbt su klientais
        startWorkingWithClients(protocol, cashier, workingCashier, storeServicedClients, 1, &protocolCounter);
        startWorkingWithClients(protocol, cashierVIP, workingCashierVIP, storeServicedClientsVIP, 2, &protocolCounter);

        // Spausdinam informaciją apie eilėje laukenčius ir aptarnaujamus klientus
        howMuchClientsServiced(protocol, cashier, workingCashier, cashierVIP, workingCashierVIP, &protocolCounter, storeServicedClients, storeServicedClientsVIP);
        howMuchClientsWaiting(protocol, array[queueCount], &protocolCounter);
    }


    // Aptarnaujam paskutinius klientus
    int max = leftOverTimeMax(cashier, cashierVIP, workingCashier, workingCashierVIP);

    for (int i = 0; i < max; ++i){
        protocolCounter = 1;
        overTime1++;
        fprintf(protocol, "\n\nT = %d min. (Viršvalandis nr. %d):\n", overTime1 + workTime, overTime1);

        // Patikrinam, ar kasininkas dirbs (prastova)
        checkDownTime(cashier, workingCashier, downTimeCashier);
        checkDownTime(cashierVIP, workingCashierVIP, downTimeCashierVIP);
        incrementWaitingTime(array[queueCount]);

        // Pradeda dirbt su klientais
        startWorkingWithClients(protocol, cashier, workingCashier, storeServicedClients, 1, &protocolCounter);
        startWorkingWithClients(protocol, cashierVIP, workingCashierVIP, storeServicedClientsVIP, 2, &protocolCounter);

        // Spausdinam informaciją apie eilėje laukenčius ir aptarnaujamus klientus
        howMuchClientsServiced(protocol, cashier, workingCashier, cashierVIP, workingCashierVIP, &protocolCounter, storeServicedClients, storeServicedClientsVIP);
        howMuchClientsWaiting(protocol, array[queueCount], &protocolCounter);
    }

    int downTime1 = 0;

    for (int i = 0; i < cashier; ++i){
        downTime1 += downTimeCashier[i];
    }

    for (int i = 0; i < cashierVIP; ++i){
        downTime1 += downTimeCashierVIP[i];
    }

    double averageClientWaitingTime1 = calculateClientAverageWaitingTime(waitingTimeCounter, waitingTime);
    int maxVIPClientWaitingTime1 = calculateVIPClientMaxWaitingTime(waitingTimeVIPCounter, waitingTimeVIP);


    // Procesas 2:
    // bankas turi atskirus kasininkus, kurie dirba su paprastais ir VIP klientais.
    fprintf(protocol, "\n\n\n\nPROCESAS NR. 2 - bankas turi atskirus kasininkus, kurie dirba su paprastais ir VIP klientais.");

    // Nunulinam visus duomenis sekančiam procesui
    queueCount++;
    clientCount = 0;
    VIPClientCount = 0;
    waitingTimeCounter = 0;
    waitingTimeVIPCounter = 0;
    resetArrays(cashier, workingCashier);
    resetArrays(cashierVIP, workingCashierVIP);
    resetArrays(cashier, downTimeCashier);
    resetArrays(cashierVIP, downTimeCashierVIP);
    resetArrays(workTime, waitingTime);
    resetArrays(workTime, waitingTimeVIP);
    reset2DArrays(cashier, storeServicedClients);
    reset2DArrays(cashierVIP, storeServicedClientsVIP);
    createEmptyPQueue(&(array[queueCount]), SIZE);
    createEmptyPQueue(&(array[queueCount + 1]), SIZE);

    for (int i = 0; i < workTime; ++i){
        protocolCounter = 1;
        fprintf(protocol, "\n\nT = %d min.\n", i);

        // Generuojam klientus
        if (generateClient(probability) == true){
            clientCount++;
            fprintf(protocol, "\t%d) Atėjo paprastas klietas nr. %d.\n", protocolCounter, clientCount);
            protocolCounter++;
            CustomType PriorityQueueDataType = {clientCount, 0};
            insertPQueue(&(array[queueCount]), PriorityQueueDataType, 1);
        }

        if (generateClient(probabilityVIP) == true){
            VIPClientCount++;
            fprintf(protocol, "\t%d) Atėjo VIP klientas nr. %d.\n", protocolCounter, VIPClientCount);
            protocolCounter++;
            CustomType PriorityQueueDataType = {VIPClientCount, 0};
            insertPQueue(&(array[queueCount + 1]), PriorityQueueDataType, 2);
        }

        // Priiminėjam VIP klientus
        while (freeCashiers(cashierVIP, workingCashierVIP) > 0 && !(isEmptyPQueue(array[queueCount + 1]))){
            Data num = getPQueue(array[queueCount + 1]);
            int priority = getPQueuePriority(array[queueCount + 1]);
            int waitingTimeClient = removePQueue(&array[queueCount + 1]);

            if (errorno_PQueue == 1){
                errorno_PQueue = 0;
                fprintf(protocol, ERROR1 "\n");
            }
            else {
                for (int i = 0; i < cashierVIP; ++i){
                    if (workingCashierVIP[i] == 0){
                        waitingTimeVIP[waitingTimeVIPCounter] = waitingTimeClient;
                        waitingTimeVIPCounter++;
                        fprintf(protocol, "\t%d) VIP klientas nr. " DATA " yra priimamas aptarnauti į VIP %d-ą kasą.\n", protocolCounter, num.value, i + 1);
                        protocolCounter++;
                        workingCashierVIP[i] = timeVIP;
                        storeServicedClientsVIP[i][0] = num.value;
                        storeServicedClientsVIP[i][1] = priority;
                        break;
                    }
                }
            }
        }

        // Priiminėjam paprastus klientus
        while (freeCashiers(cashier, workingCashier) > 0 && !(isEmptyPQueue(array[queueCount]))){
            Data num = getPQueue(array[queueCount]);
            int priority = getPQueuePriority(array[queueCount]);
            int waitingTimeClient = removePQueue(&array[queueCount]);

            if (errorno_PQueue == 1){
                errorno_PQueue = 0;
                fprintf(protocol, ERROR1 "\n");
            }
            else {
                for (int i = 0; i < cashier; ++i){
                    if (workingCashier[i] == 0){
                        waitingTime[waitingTimeCounter] = waitingTimeClient;
                        waitingTimeCounter++;
                        fprintf(protocol, "\t%d) Paprastas klientas nr. " DATA " yra priimamas aptarnauti į %d-ą paprastų klientų kasą.\n", protocolCounter, num.value, i + 1);
                        protocolCounter++;
                        workingCashier[i] = time;
                        storeServicedClients[i][0] = num.value;
                        storeServicedClients[i][1] = priority;
                        break;
                    }
                }
            }
        }

        // Patikrinam, ar kasininkas dirbs (prastova)
        checkDownTime(cashier, workingCashier, downTimeCashier);
        checkDownTime(cashierVIP, workingCashierVIP, downTimeCashierVIP);
        incrementWaitingTime(array[queueCount]);
        incrementWaitingTime(array[queueCount + 1]);

        // Pradeda dirbt su klientais
        startWorkingWithClients(protocol, cashier, workingCashier, storeServicedClients, 1, &protocolCounter);
        startWorkingWithClients(protocol, cashierVIP, workingCashierVIP, storeServicedClientsVIP, 2, &protocolCounter);

        // Spausdinam informaciją apie eilėje laukenčius ir aptarnaujamus klientus
        howMuchClientsServiced(protocol, cashier, workingCashier, cashierVIP, workingCashierVIP, &protocolCounter, storeServicedClients, storeServicedClientsVIP);
        howMuchClientsWaiting(protocol, array[queueCount], &protocolCounter);
        howMuchClientsWaiting(protocol, array[queueCount + 1], &protocolCounter);
    }

    
    // Proceso 2 viršvalandžiai:
    fprintf(protocol, "\n\n\nPROCESO NR. 2 VIRSVALNDZIAI:");
    int overTime2 = 0;
    
    while ((!(isEmptyPQueue(array[queueCount]))) || (!(isEmptyPQueue(array[queueCount + 1])))){
        protocolCounter = 1;
        overTime2++;
        fprintf(protocol, "\n\nT = %d min. (Viršvalandis nr. %d):\n", overTime2 + workTime, overTime2);

        // Priiminėjam VIP klientus
        while (freeCashiers(cashierVIP, workingCashierVIP) > 0 && !(isEmptyPQueue(array[queueCount + 1]))){
            Data num = getPQueue(array[queueCount + 1]);
            int priority = getPQueuePriority(array[queueCount + 1]);
            int waitingTimeClient = removePQueue(&array[queueCount + 1]);

            if (errorno_PQueue == 1){
                errorno_PQueue = 0;
                fprintf(protocol, ERROR1 "\n");
            }
            else {
                for (int i = 0; i < cashierVIP; ++i){
                    if (workingCashierVIP[i] == 0){
                        waitingTimeVIP[waitingTimeVIPCounter] = waitingTimeClient;
                        waitingTimeVIPCounter++;
                        fprintf(protocol, "\t%d) VIP klientas nr. " DATA " yra priimamas aptarnauti į VIP %d-ą kasą.\n", protocolCounter, num.value, i + 1);
                        protocolCounter++;
                        workingCashierVIP[i] = timeVIP;
                        storeServicedClientsVIP[i][0] = num.value;
                        storeServicedClientsVIP[i][1] = priority;
                        break;
                    }
                }
            }
        }

        // Priiminėjam paprastus klientus
        while (freeCashiers(cashier, workingCashier) > 0 && !(isEmptyPQueue(array[queueCount]))){
            Data num = getPQueue(array[queueCount]);
            int priority = getPQueuePriority(array[queueCount]);
            int waitingTimeClient = removePQueue(&array[queueCount]);

            if (errorno_PQueue == 1){
                errorno_PQueue = 0;
                fprintf(protocol, ERROR1 "\n");
            }
            else {
                for (int i = 0; i < cashier; ++i){
                    if (workingCashier[i] == 0){
                        waitingTime[waitingTimeCounter] = waitingTimeClient;
                        waitingTimeCounter++;
                        fprintf(protocol, "\t%d) Paprastas klientas nr. " DATA " yra priimamas aptarnauti į %d-ą paprastų klientų kasą.\n", protocolCounter, num.value, i + 1);
                        protocolCounter++;
                        workingCashier[i] = time;
                        storeServicedClients[i][0] = num.value;
                        storeServicedClients[i][1] = priority;
                        break;
                    }
                }
            }
        }

        // Patikrinam, ar kasininkas dirbs (prastova)
        checkDownTime(cashier, workingCashier, downTimeCashier);
        checkDownTime(cashierVIP, workingCashierVIP, downTimeCashierVIP);
        incrementWaitingTime(array[queueCount]);
        incrementWaitingTime(array[queueCount + 1]);

        // Pradeda dirbt su klientais
        startWorkingWithClients(protocol, cashier, workingCashier, storeServicedClients, 1, &protocolCounter);
        startWorkingWithClients(protocol, cashierVIP, workingCashierVIP, storeServicedClientsVIP, 2, &protocolCounter);

        // Spausdinam informaciją apie eilėje laukenčius ir aptarnaujamus klientus
        howMuchClientsServiced(protocol, cashier, workingCashier, cashierVIP, workingCashierVIP, &protocolCounter, storeServicedClients, storeServicedClientsVIP);
        howMuchClientsWaiting(protocol, array[queueCount], &protocolCounter);
        howMuchClientsWaiting(protocol, array[queueCount + 1], &protocolCounter);
    }


    // Aptarnaujam paskutinius klientus
    max = leftOverTimeMax(cashier, cashierVIP, workingCashier, workingCashierVIP);

    for (int i = 0; i < max; ++i){
        protocolCounter = 1;
        overTime2++;
        fprintf(protocol, "\n\nT = %d min. (Viršvalandis nr. %d):\n", overTime2 + workTime, overTime2);

        // Patikrinam, ar kasininkas dirbs (prastova)
        checkDownTime(cashier, workingCashier, downTimeCashier);
        checkDownTime(cashierVIP, workingCashierVIP, downTimeCashierVIP);
        incrementWaitingTime(array[queueCount]);
        incrementWaitingTime(array[queueCount + 1]);

        // Pradeda dirbt su klientais
        startWorkingWithClients(protocol, cashier, workingCashier, storeServicedClients, 1, &protocolCounter);
        startWorkingWithClients(protocol, cashierVIP, workingCashierVIP, storeServicedClientsVIP, 2, &protocolCounter);

        // Spausdinam informaciją apie eilėje laukenčius ir aptarnaujamus klientus
        howMuchClientsServiced(protocol, cashier, workingCashier, cashierVIP, workingCashierVIP, &protocolCounter, storeServicedClients, storeServicedClientsVIP);
        howMuchClientsWaiting(protocol, array[queueCount], &protocolCounter);
        howMuchClientsWaiting(protocol, array[queueCount + 1], &protocolCounter);
    }

    int downTime2 = 0;

    for (int i = 0; i < cashier; ++i){
        downTime2 += downTimeCashier[i];
    }

    for (int i = 0; i < cashierVIP; ++i){
        downTime2 += downTimeCashierVIP[i];
    }

    double averageClientWaitingTime2 = calculateClientAverageWaitingTime(waitingTimeCounter, waitingTime);
    int maxVIPClientWaitingTime2 = calculateVIPClientMaxWaitingTime(waitingTimeVIPCounter, waitingTimeVIP);


    // Sprendimai
    fprintf(protocol, "\n\n\n\nIII DALIS - SPRENDINIAI");

    printf("\n\nSuminis kasininkų prastovų laikas (procesas nr. 1): %d min.\n", downTime1);
    printf("Suminis kasininkų prastovų laikas (procesas nr. 2): %d min.\n\n", downTime2);
    printf("Dvigubas suminis kasininkų viršvalandžių laikas (procesas nr. 1): %d min.\n", overTime1 * 2);
    printf("Dvigubas suminis kasininkų viršvalandžių laikas (procesas nr. 2): %d min.\n\n", overTime2 * 2);
    printf("Trigubas vidutinis paprasto kliento laukimo laikas (procesas nr. 1): %.2f min.\n", averageClientWaitingTime1 * 3);
    printf("Trigubas vidutinis paprasto kliento laukimo laikas (procesas nr. 2): %.2f min.\n\n", averageClientWaitingTime2 * 3);
    printf("Dešimtgubas VIP kliento maksimalaus laukimo laikas (procesas nr. 1): %d min.\n", maxVIPClientWaitingTime1 * 10);
    printf("Dešimtgubas VIP kliento maksimalaus laukimo laikas (procesas nr. 2): %d min.\n\n", maxVIPClientWaitingTime2 * 10);
    fprintf(protocol, "\n\nProceso nr. 1:\n");
    fprintf(protocol, "\t1) Suminis kasininkų prastovų laikas: %d min.\n", downTime1);
    fprintf(protocol, "\t2) Dvigubas suminis kasininkų viršvalandžių laikas: %d min.\n", overTime1 * 2);
    fprintf(protocol, "\t3) Trigubas vidutinis paprasto kliento laukimo laikas: %.2f min.\n", averageClientWaitingTime1 * 3);
    fprintf(protocol, "\t4) Dešimtgubas VIP kliento maksimalaus laukimo laikas: %d min.\n", maxVIPClientWaitingTime1 * 10);
    fprintf(protocol, "\n\nProceso nr. 2:\n");
    fprintf(protocol, "\t1) Suminis kasininkų prastovų laikas: %d min.\n", downTime2);
    fprintf(protocol, "\t2) Dvigubas suminis kasininkų viršvalandžių laikas: %d min.\n", overTime2 * 2);
    fprintf(protocol, "\t3) Trigubas vidutinis paprasto kliento laukimo laikas: %.2f min.\n", averageClientWaitingTime2 * 3);
    fprintf(protocol, "\t4) Dešimtgubas VIP kliento maksimalaus laukimo laikas: %d min.\n", maxVIPClientWaitingTime2 * 10);

    int overall1 = downTime1 + overTime1 * 2 + floor(averageClientWaitingTime1 * 3) + maxVIPClientWaitingTime1 * 10;
    int overall2 = downTime2 + overTime2 * 2 + floor(averageClientWaitingTime2 * 3) + maxVIPClientWaitingTime2 * 10;

    printf("\n\nProceso nr. 1 kriterijų vertinimo suma:\n%d\n", overall1);
    printf("\nProceso nr. 2 kriterijų vertinimo suma:\n%d\n", overall2);
    fprintf(protocol, "\n\nProceso nr. 1 kriterijų vertinimo laiko suma: %d min.\n", overall1);
    fprintf(protocol, "\nProceso nr. 2 kriterijų vertinimo suma: %d min.\n\n", overall2);

    if (overall1 < overall2){
        printf("\nŠiuo atveju procesas nr. 1 yra geresnis.\n\n");
        fprintf(protocol, "\nŠiuo atveju procesas nr. 1 yra geresnis.\n");
    }
    else if (overall1 > overall2){
        printf("\nŠiuo atveju procesas nr. 2 yra geresnis.\n\n");
        fprintf(protocol, "\nŠiuo atveju procesas nr. 2 yra geresnis.\n");
    }
    else {
        printf("\nŠiuo atveju abu procesai panašūs.\n\n");
        fprintf(protocol, "\nŠiuo atveju abu procesai panašūs.\n");
    }

    printf("Protokolas %s sėkmingai sukurtas.\n", FILE_NAME);
    
    // Uzdarom failą ir ištrinam prioritetines eiles
    fclose(protocol);
    deletePQueue(&(array[queueCount - 1]));
    deletePQueue(&(array[queueCount + 0]));
    deletePQueue(&(array[queueCount + 1]));

    return 0;
}