/**
 * @file proj2.c
 * @author Petr Sopf (xsopfp00)
 * @brief Source for main file
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#include "data.h"
#include "program.h"
#include "proj2.h"

/** \copydoc main */
int main(int argc, char *argv[]) {
    //Get and validate arguments
    params = parseParameters(argc, argv);

    srand(time(0));
    signal(SIGINT, processFail);
    signal(SIGTERM, processFail);

    initProgram();

    //Create process for Adult
    pid_t adultP;
    pid_t adultPMaster = fork();

    if (adultPMaster == 0) {
        for (int i = 1; i <= params->A; i++) {

            if (params->AGT != 0) {
                usleep((rand() % params->AGT) * 1000);
            } else {
                usleep(0);
            }

            adultP = fork();

            if (adultP == 0) {
                doAdult(i);
                closeSemaphores();
                exit(0);
            } else if (adultP < 0) {
                perror("ERROR: Fork error!");
                endProgram();
                exit(2);
            }
        }

        if (adultP > 0) {
            while (wait(NULL) > 0);
        }

        closeSemaphores();
        exit(0);
    } else if (adultPMaster < 0) {
        perror("ERROR: Fork error!");
        endProgram();
        exit(2);
    }

    //Create process for Child
    pid_t childP;
    pid_t childPMaster = fork();
    if (childPMaster == 0) {
        for (int j = 1; j <= params->C; j++) {
            if (params->CGT != 0) {
                usleep((rand() % params->CGT) * 1000);
            } else {
                usleep(0);
            }

            childP = fork();
            if (childP == 0) {
                doChild(j);
                closeSemaphores();
                exit(0);
            } else if (childP < 0) {
                perror("ERROR: Fork error!");
                endProgram();
                exit(2);
            }
        }

        if (childP > 0) {
            while (wait(NULL) > 0);
        }

        closeSemaphores();
        exit(0);
    } else if (childPMaster < 0) {
        perror("ERROR: Fork error!");
        endProgram();
        exit(2);
    }


    if (adultPMaster > 0 || childPMaster > 0) {
        while (wait(NULL) > 0);
    }

    endProgram();
    exit(0);
}

/** \copydoc parseParameters */
Parameters *parseParameters(int argc, char *argv[]) {
    //Number of arguments must be 7
    if (argc != 7) {
        perror("ERROR: Bad number of arguments given!");
        exit(1);
    }

    char *ptr;

    //Number of adults must be integer bigger then zero
    int A;
    A = strtol(argv[1], &ptr, 10);
    if (A <= 0 || *ptr != '\0') {
        perror("ERROR: A argument must be integer bigger then zero!");
        exit(1);
    }

    //Number of child must be integer bigger then zero
    int C;
    C = strtol(argv[2], &ptr, 10);
    if (C <= 0 || *ptr != '\0') {
        perror("ERROR: C argument must be integer bigger then zero!");
        exit(1);
    }

    // Maximum time in ms, that new process adult is generated for must be integer between 0 and 5000!
    int AGT;
    AGT = strtol(argv[3], &ptr, 10);
    if (AGT < 0 || AGT > 5000 || *ptr != '\0') {
        perror("ERROR: AGT argument must be integer between 0 and 5000!");
        exit(1);
    }

    // Maximum time in ms, that new process child is generated for must be integer between 0 and 5000!
    int CGT;
    CGT = strtol(argv[4], &ptr, 10);
    if (CGT < 0 || CGT > 5000 || *ptr != '\0') {
        perror("ERROR: CGT argument must be integer between 0 and 5000!");
        exit(1);
    }

    // Maximum time in ms, that process adult simulates activity in center for must integer between 0 and 5000!
    int AWT;
    AWT = strtol(argv[5], &ptr, 10);
    if (AWT < 0 || AWT > 5000 || *ptr != '\0') {
        perror("ERROR: AWT argument must be integer between 0 and 5000!");
        exit(1);
    }

    // Maximum time in ms, that process child simulates activity in center for must integer between 0 and 5000!
    int CWT;
    CWT = strtol(argv[6], &ptr, 10);
    if (CWT < 0 || CWT > 5000 || *ptr != '\0') {
        perror("ERROR: CWT argument must be integer between 0 and 5000!");
        exit(1);
    }

    //All arguments validated, create struct
    Parameters *paramsParsed = NULL;
    paramsParsed = malloc(sizeof(Parameters));

    //Check for malloc fail
    if (paramsParsed == NULL) {
        perror("ERROR: Failed malloc for Parameters struct!");
        exit(1);
    }

    paramsParsed->A = A;
    paramsParsed->C = C;
    paramsParsed->AGT = AGT;
    paramsParsed->CGT = CGT;
    paramsParsed->AWT = AWT;
    paramsParsed->CWT = CWT;

    return paramsParsed;
}

