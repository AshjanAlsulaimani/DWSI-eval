/*
 * EvaluateEmergence.h
 *
 *  Created on: 5 Feb 2020
 *      Author: alsulaia
 */
//
//#ifndef EVALUATE_EMERGENCE_EVALUATEEMERGENCE_H_
//#define EVALUATE_EMERGENCE_EVALUATEEMERGENCE_H_
//
//class EvaluateEmergence {
//public:
//	EvaluateEmergence();
//	virtual ~EvaluateEmergence();
//};
//
//#endif /* EVALUATE_EMERGENCE_EVALUATEEMERGENCE_H_ */


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
#include "../misc.h"

using namespace std;


extern const unsigned long block_size;
//extern const unsigned long block_size_print;
extern int numberstored;
extern char const *colonames_allemerEval[];





void goldemergencesetbyTarget(char ***EvalContent,int size,map<string, char ***>*Goldemergence,map<string, char ***>*GoldNOemergence, int colnumcondition);
void goldemergencesetbySense(char ***EvalContent,int size,multimap<string, char ***>*Goldemergence,multimap<string, char ***>*GoldNOemergence);


void compare_Emergence_includeYears(char ***EvalNEOContent, char ***EvalSCANContent, int input1size, char *resultsPath);
void compare_Emergence_tiddyTable(char ***EvalNEOContent, char ***EvalSCANContent, int input1size, char *resultsPath);
void compare_Emergence(char ***EvalNEOContent, char ***EvalSCANContent, int input1size, char *resultsPath);
void MAE_format(char ***EvalContent , char *resultsPath);



//void print_performance(double* performance,double meanabsoluteerror,char *resDir);
void print_performance(double* performance,double Globalmeanabsoluteerror,double GlobalnormalisedmeanabsoluteError,char *resDir);

double* calculate_performance(unsigned int **emergencebinarymatrix);

void print_binaryEmergenceMatrix(unsigned int **emergencetable,char *resDir);
//void create_goldpredtable(OUTPUTSEMERGENCE  *WSI_EMERGENCE_INFO, char *goldemergenceFile);
void print_goldpredtable(char ***goldpredemergencetable,char *resDir);
unsigned int** counts_performance(char ***goldpredemergencetable, int EvalStatusIndex);
double sum_meanabsoluteError(char ***goldpredemergencetable, int IndivErrorIndex);
double normalised_meanabsoluteError(char *** goldpredemergencetable, int NormalisedErrorIndex);

unsigned int store_WSIDATA_EVAL(char const *filename, char ****targetContent);
const char **read_dir_Targetscontent_EVAL(const char *dirname);
map<string, char ***>* read_emergence( char *emerPath);
char *** create_goldpredtable(map<string, char ***>*GoldEmergenceContent, map<string, char ***>*PredEmergenceContent, unsigned int windowsizeemergenceeval);


int find_RowGoldCUIIndex(char * PredCUI,char ***GoldEmerContent);
const char *get_eval_status(char *GoldEmerYear,char *PredEmerYear,unsigned int windowsizeemergenceeval);
unsigned int get_error(char *GoldEmerYear,char *PredEmerYear, unsigned int maxError);
