/**
 * @file program.c
 * @author Petr Sopf (xsopfp00)
 * @brief Source file for functions
 */

#include "data.h"
#include "program.h"

/** \copydoc initProgram */
void initProgram() {
    //Create semaphores
    semMutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
    semFinish = sem_open(SEM_FINISH, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    semChildQueue = sem_open(SEM_CHILDQUEUE, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    semAdultsQueue = sem_open(SEM_ADULTSQUEUE, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);

    //Check for failure
    if (semMutex == SEM_FAILED || semFinish == SEM_FAILED || semChildQueue == SEM_FAILED ||
        semAdultsQueue == SEM_FAILED) {
        perror("ERROR: Can't create semaphores!");
        endProgram();
        exit(2);
    }

    //Create shm
    shmActionN = shm_open(SHM_ACTIONN, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    shmDone = shm_open(SHM_DONE, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    shmAdults = shm_open(SHM_ADULTS, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    shmAdultsIn = shm_open(SHM_ADULTSIN, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    shmAdultsWaitingToLeave = shm_open(SHM_ADULTSWAITINGTOLEAVE, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    shmChild = shm_open(SHM_CHILD, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    shmChildIn = shm_open(SHM_CHILDIN, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    shmChildWaitingToEnter = shm_open(SHM_CHILDWAITINGTOENTER, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    //Check for failure
    if (shmActionN == -1 || shmDone == -1 || shmAdults == -1 || shmAdultsIn == -1 || shmAdultsWaitingToLeave == -1 ||
        shmChild == -1 || shmChildIn == -1 || shmChildWaitingToEnter == -1) {
        perror("ERROR: Can't create shm!");
        endProgram();
        exit(2);
    }

    //Truncate
    ftruncate(shmActionN, SHM_SIZE);
    ftruncate(shmDone, SHM_SIZE);
    ftruncate(shmAdults, SHM_SIZE);
    ftruncate(shmAdultsIn, SHM_SIZE);
    ftruncate(shmAdultsWaitingToLeave, SHM_SIZE);
    ftruncate(shmChild, SHM_SIZE);
    ftruncate(shmChildIn, SHM_SIZE);
    ftruncate(shmChildWaitingToEnter, SHM_SIZE);

    //Map
    actionN = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmActionN, 0);
    done = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmDone, 0);
    adults = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmAdults, 0);
    adultsIn = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmAdultsIn, 0);
    adultWaitingToLeave = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmAdultsWaitingToLeave, 0);
    child = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmChild, 0);
    childIn = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmChildIn, 0);
    childWaitingToEnter = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmChildWaitingToEnter, 0);

    //Check for failure
    if (actionN == MAP_FAILED || done == MAP_FAILED || adults == MAP_FAILED || adultsIn == MAP_FAILED ||
        adultWaitingToLeave == MAP_FAILED || child == MAP_FAILED || childIn == MAP_FAILED ||
        childWaitingToEnter == MAP_FAILED) {
        perror("ERROR: Can't map!");
        endProgram();
        exit(2);
    }

    //Init logfile
    logFile = fopen(LOG_FILE_NAME, "w");
    if (logFile == NULL) {
        perror("ERROR: Can't open/create logfile!");
        exit(1);
    }

    setbuf(logFile, NULL);
}

/** \copydoc endProgram */
void endProgram() {
    //All semaphores must be closed
    closeSemaphores();

    //Unlink semaphores
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_FINISH);
    sem_unlink(SEM_CHILDQUEUE);
    sem_unlink(SEM_ADULTSQUEUE);

    //Close shm
    close(shmActionN);
    close(shmDone);
    close(shmAdults);
    close(shmAdultsIn);
    close(shmAdultsWaitingToLeave);
    close(shmChild);
    close(shmChildIn);
    close(shmChildWaitingToEnter);

    //Unmap
    munmap(actionN, SHM_SIZE);
    munmap(done, SHM_SIZE);
    munmap(adults, SHM_SIZE);
    munmap(adultsIn, SHM_SIZE);
    munmap(adultWaitingToLeave, SHM_SIZE);
    munmap(child, SHM_SIZE);
    munmap(childIn, SHM_SIZE);
    munmap(childWaitingToEnter, SHM_SIZE);

    //Unlink shm
    shm_unlink(SHM_ACTIONN);
    shm_unlink(SHM_DONE);
    shm_unlink(SHM_ADULTS);
    shm_unlink(SHM_ADULTSIN);
    shm_unlink(SHM_ADULTSWAITINGTOLEAVE);
    shm_unlink(SHM_CHILD);
    shm_unlink(SHM_CHILDIN);
    shm_unlink(SHM_CHILDWAITINGTOENTER);

    //Free memory for params
    free(params);

    //Close logfile
    fclose(logFile);
}

/** \copydoc closeSemaphores */
void closeSemaphores() {
    sem_close(semMutex);
    sem_close(semFinish);
    sem_close(semChildQueue);
    sem_close(semAdultsQueue);
}

/** \copydoc doAdult */
void doAdult(int id) {
    //Starting
    sem_wait(semMutex);
    (*actionN)++;
    fprintf(logFile, "%d: A %d: started\n", *actionN, id);
    sem_post(semMutex);

    //Entering
    sem_wait(semMutex);
    (*adults)++;
    (*adultsIn)++;

    sem_post(semMutex);
    sem_wait(semMutex);

    if (*childWaitingToEnter) {
        int n = getMin(3, *childWaitingToEnter);
        for (int k = n; k > 0; k--) {
            sem_post(semChildQueue);
        }

        *childWaitingToEnter -= n;
        *childIn += n;
    }

    (*actionN)++;
    fprintf(logFile, "%d: A %d: enter\n", *actionN, id);
    sem_post(semMutex);

    //Simulate
    if (params->AWT != 0) {
        usleep((rand() % params->AWT) * 1000);
    } else {
        usleep(0);
    }

    //Leave
    sem_wait(semMutex);
    (*actionN)++;
    fprintf(logFile, "%d: A %d: trying to leave\n", *actionN, id);

    if (*childIn <= ((*adultsIn) - 1) * 3) {
        (*adultsIn)--;
    } else {
        (*adultWaitingToLeave)++;
        (*actionN)++;
        fprintf(logFile, "%d: A %d: waiting: %d : %d\n", *actionN, id, *adultsIn, *childIn);

        sem_post(semMutex);
        sem_wait(semAdultsQueue);
        sem_wait(semMutex);
    }

    //If all adults are done, but there are children left
    if (params->A == *adults) {
        for (int k = (*childWaitingToEnter); k > 0; k--) {
            sem_post(semChildQueue);
            (*childWaitingToEnter)--;
        }
    }

    (*actionN)++;
    fprintf(logFile, "%d: A %d: leave\n", *actionN, id);

    (*done)++;
    if (*done == (params->A + params->C)) {
        for (int k = (params->A + params->C); k > 0; k--) {
            sem_post(semFinish);
        }
    }
    sem_post(semMutex);

    //End all together
    sem_wait(semFinish);

    sem_wait(semMutex);
    (*actionN)++;
    fprintf(logFile, "%d: A %d: finished\n", *actionN, id);
    sem_post(semMutex);
}

/** \copydoc doChild */
void doChild(int id) {
    //Starting
    sem_wait(semMutex);
    (*child)++;
    (*actionN)++;
    fprintf(logFile, "%d: C %d: started\n", *actionN, id);
    sem_post(semMutex);

    //Enter
    sem_wait(semMutex);
    if (*childIn < *adultsIn * 3) {
        (*childIn)++;
    }
        //If there are no adult left
    else if (*adults == params->A && *adultsIn == 0) {
        (*childIn)++;
    } else {
        (*actionN)++;
        fprintf(logFile, "%d: C %d: waiting: %d : %d\n", *actionN, id, *adultsIn, *childIn);

        (*childWaitingToEnter)++;
        sem_post(semMutex);
        sem_wait(semChildQueue);
        sem_wait(semMutex);
    }

    (*actionN)++;
    fprintf(logFile, "%d: C %d: enter\n", *actionN, id);
    sem_post(semMutex);

    //Simulate
    if (params->CWT != 0) {
        usleep((rand() % params->CWT) * 1000);
    } else {
        usleep(0);
    }

    //Leaving
    sem_wait(semMutex);
    (*actionN)++;
    fprintf(logFile, "%d: C %d: trying to leave\n", *actionN, id);

    (*childIn)--;
    if (*adultWaitingToLeave && *childIn <= ((*adultsIn) - 1) * 3) {
        (*adultWaitingToLeave)--;
        (*adultsIn)--;
        sem_post(semAdultsQueue);
    }

    (*actionN)++;
    fprintf(logFile, "%d: C %d: leave\n", *actionN, id);

    (*done)++;
    if (*done == (params->A + params->C)) {
        for (int k = (params->A + params->C); k > 0; k--) {
            sem_post(semFinish);
        }
    }
    sem_post(semMutex);

    //End all together
    sem_wait(semFinish);

    sem_wait(semMutex);
    (*actionN)++;
    fprintf(logFile, "%d: C %d: finished\n", *actionN, id);
    sem_post(semMutex);
}

/** \copydoc getMin */
int getMin(int a, int b) {
    if (a < b) {
        return a;
    }

    return b;
}

/** \copydoc processFail */
void processFail() {
    kill(getpid(), SIGTERM);
    endProgram();
    exit(2);
}