/**
 * @file program.h
 * @author Petr Sopf (xsopfp00)
 * @brief Header file for functions
 */


#ifndef IOS_PROJ2_PROGRAM_H
#define IOS_PROJ2_PROGRAM_H

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
 * @brief Init program
 * @return void
 */
void initProgram();

/**
 * @brief Destroys and cleans everything
 * @return void
 */
void endProgram();

/**
 * @brief Closes all semaphores
 * @return void
 */
void closeSemaphores();

/**
 * @brief Adult process
 * @param id ID of adult
 * @ret
 */
void doAdult(int id);

/**
 * @brief Child process
 * @param id ID of child
 * @return void
 */
void doChild(int id);

/**
 * @brief Return MIN of two integers
 * @param a integer
 * @param b integer
 * @return MIN(a, b)
 */
int getMin(int a, int b);


/**
 * @brief On process fail signal
 * @return void
 */
void processFail();

#endif
