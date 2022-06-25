/*
 * EVALUATEDISTANCE.h
 *
 *  Created on: 8 May 2020
 *      Author: alsulaia
 */
//
//#ifndef EVALUATE_DISTANCE_EVALUATEDISTANCE_H_
//#define EVALUATE_DISTANCE_EVALUATEDISTANCE_H_
//
//class EVALUATE_DISTANCE {
//public:
//	EVALUATE_DISTANCE();
//	virtual ~EVALUATE_DISTANCE();
//};
//
//#endif /* EVALUATE_DISTANCE_EVALUATEDISTANCE_H_ */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "../misc.h"
#include <experimental/filesystem>

using namespace std;

const char ** read_dir_content_FULL(const char *dirpath);
char ***read_probabilitiesvectors(char *probfile,unsigned*sizeD);
void calculate_euclideanDistance( char ***goldData,  char ***predData, unsigned int sizeData, char *resultsPath, char *target, FILE*FP);
double calculate_differenceSquared(double a, double b, double presult);
void store_res_to_file(double disvalue, char *predSense, char *goldSense,FILE	*fp, char *target, FILE*FP, unsigned int sizeData);
const char **read_dir_Targetscontent(const char *dirname);
const char *get_Tgoldpath(const char *dirGoldpath,const char *Targetdirname, char **target);
