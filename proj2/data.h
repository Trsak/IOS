/**
 * @file data.h
 * @author Petr Sopf (xsopfp00)
 * @brief Header file for defining constants and global variables
 */

#ifndef IOS_PROJ2_DATA_H
#define IOS_PROJ2_DATA_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

/**
 * @brief Structure for program parameters
 */
typedef struct {
    int A; /**< Number of adults */
    int C; /**< Number of child */
    int AGT; /**< Maximum time in ms, that new process adult is generated for */
    int CGT; /**< Maximum time in ms, that new process child is generated for */
    int AWT; /**< Maximum time in ms, that process adult simulates activity in center for */
    int CWT; /**< Maximum time in ms, that process child simulates activity in center for */
} Parameters;

//Struct pointer
Parameters *params;

//Constants
#define SHM_SIZE (sizeof(int)) /** Size of integer for shared memory */
#define LOG_FILE_NAME "proj2.out" /** Name of log file */

//Constants - Semaphore names
#define SEM_MUTEX "/xsopfp00-ios-proj2-semMutex"
#define SEM_FINISH "/xsopfp00-ios-proj2-semFinish"
#define SEM_CHILDQUEUE "/xsopfp00-ios-proj2-semChildQueue"
#define SEM_ADULTSQUEUE "/xsopfp00-ios-proj2-semAdultsQueue"

//Constants - Shared memory names
#define SHM_ACTIONN "/xsopfp00-ios-proj2-actionN"
#define SHM_DONE "/xsopfp00-ios-proj2-done"
#define SHM_ADULTS "/xsopfp00-ios-proj2-adults"
#define SHM_ADULTSIN "/xsopfp00-ios-proj2-adultsIn"
#define SHM_ADULTSWAITINGTOLEAVE "/xsopfp00-ios-proj2-adultWaitingToLeave"
#define SHM_CHILD "/xsopfp00-ios-proj2-child"
#define SHM_CHILDIN "/xsopfp00-ios-proj2-childIn"
#define SHM_CHILDWAITINGTOENTER "/xsopfp00-ios-proj2-childWaitingToEnter"

//Log file
extern FILE *logFile; /** Output log file */

//Semaphores
sem_t *semMutex;
sem_t *semFinish;
sem_t *semChildQueue;
sem_t *semAdultsQueue;

//Variables for shared memory usage
int *actionN; /** Number of actions done */
int *done; /** Number of processes ready to finish */
int *adults; /** Number of adult processes */
int *adultsIn; /** Number of adults in center */
int *adultWaitingToLeave; /** Number of adults waiting to leave */
int *child; /** Number of child processes */
int *childIn; /** Number of child in center */
int *childWaitingToEnter; /** Number of child waiting to enter */

//ID of variables in shared memory
int shmActionN;
int shmDone;
int shmAdults;
int shmAdultsIn;
int shmAdultsWaitingToLeave;
int shmChild;
int shmChildIn;
int shmChildWaitingToEnter;

#endif
