// Funkcijos, kurios apdorojamos programoje:
//   1. readFromUser - perskaito reikiamą informaciją iš vartotojo,
//   2. generateClient - grąžina true arba false pagal tai, ar kliento atėjimo tikimybė yra didensnė arba lygi atsitiktine tvarka sugeneruoto skaičiaus reikšmei tarp 0 ir 1.
//   3. freeCashiers - grąžiną laisvų kasininkų skaičių.
//   4. getPQueuePriority - grąžina prioritetą, kuris šiuo metu yra didžiausias prioritetinėje eilėje.
//   5. resetArrays - nunulina masyvus.
//   6. reset2DArrays - nunulina dvimačius masyvus.
//   7. startWorkingWithClients - sumažina kasininko likusį laiką dirbti su klientu vienu taktu ir praneša apie klientus, kurios baigėm aptarnauti.
//   8. leftOverTimeMax - grąžina to kasininko likusį laiką dirbti su klientu, kuriam dirbti liko ilgiausiai.
//   9. checkDownTime - skaičiuoja prastovos laiką.
//   10. incrementWaitingTime - padidina kliento laukimo eilėje laiką vienetu.
//   11. calculateClientAverageWaitingTime - apskaičiuoja paprasto kliento vidutinį laukimo laiką eilėje.
//   12. calculateVIPClientMaxWaitingTime - apskaičiuoja VIP klieto maksimalų laukimo eilėje laiką.
//   13. printToFile1 - išspausdina reikiamus duomenis į protokolą.
//   14. howMuchClientsWaiting - iššspausdina informaciją apie laukenčių klientų kiekį eilėje.
//   15. howMuchClientsServiced - išspausdina informaciją apie aptarnaujamų klientų kiekį.

// Apribojimai:
//   MAX_CASHIERS - Maksimalus paprastų klientų ir VIP klientų kasininkų skaičius
//   MAX_TIME - Masksimalus paprastų klientų ir VIP klientų aptarnavimo laikas
//   MAX_WORK_TIME - Maksimalus darbo dienos ilgis
//   SIZE - Prioritetinės eilės dydis

#ifndef __BANK_H_
#define __BANK_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "priority_queue.h"

#define MAX_CASHIERS 30
#define MAX_TIME 20
#define MAX_WORK_TIME 300
#define SIZE 600

void readFromUser(double *probability, double *probabilityVIP, int *cashier, int *cashierVIP, int *time, int *timeVIP, int *workTime);
bool generateClient(double probability);
int freeCashiers(int cashier, int workingCashier[]);
int getPQueuePriority(PriorityQueue *p_queue);
void resetArrays(int count, int array[]);
void reset2DArrays(int count, int array[][2]);
void startWorkingWithClients(FILE *protocol, int cashier, int workingCashier[], int storeServicedClients[][2], int priotirty, int *protocolCounter);
int leftOverTimeMax(int cashier, int cashierVIP, int workingCashier[], int workingCashierVIP[]);
void checkDownTime(int cashier, int cashierArray[], int downTimeArray[]);
void incrementWaitingTime(PriorityQueue *p_queue);
double calculateClientAverageWaitingTime(int waitingTimeCounter, int waitingTime[]);
int calculateVIPClientMaxWaitingTime(int waitingTimeVIPCounter, int waitingTimeVIP[]);
void printToFile1(FILE *protocol, int dataArray[], double dataArray2[]);
void howMuchClientsWaiting(FILE *protocol, PriorityQueue *p_queue, int *protocolCounter);
void howMuchClientsServiced(FILE *protocol, int cashier, int workingCashier[], int cashierVIP, int workingCashierVIP[], int *protocolCounter, int storeServicedClients[][2], int storeServicedClientsVIP[][2]);
void printValues(FILE *protocol, PriorityQueue *p_queue, int priority, int flag);

#endif