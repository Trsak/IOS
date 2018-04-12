/**
 * @file proj2.h
 * @author Petr Sopf (xsopfp00)
 * @brief Header for main file
 */

#ifndef IOS_PROJ2_PROJ2_H
#define IOS_PROJ2_PROJ2_H

/**
 * @brief Main function
 * @param argc Number of arguments given
 * @param argv Arguments
 * @return exit code
 */
int main(int argc, char *argv[]);

/**
 * @brief Parses parameters and checks for their validity
 * @param argc Number of arguments given
 * @param argv Arguments
 * @return struct Parameters
 */
Parameters *parseParameters(int argc, char *argv[]);

#endif
