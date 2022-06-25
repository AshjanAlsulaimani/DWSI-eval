/*
 * EMERGENCEEXPVERSION.h
 *
 *  Created on: 20 Dec 2019
 *      Author: alsulaia
 */

//#ifndef EMERGENCE_EXP_VERSION_EMERGENCEEXPVERSION_H_
//#define EMERGENCE_EXP_VERSION_EMERGENCEEXPVERSION_H_
//
//class EMERGENCE_EXP_VERSION {
//public:
//	EMERGENCE_EXP_VERSION();
//	virtual ~EMERGENCE_EXP_VERSION();
//};
//
//#endif /* EMERGENCE_EXP_VERSION_EMERGENCEEXPVERSION_H_ */
/*
 * This code for finding the gold emergence only
 */

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



#include "../misc.h"

using namespace std;


extern const unsigned long block_size;
extern const unsigned long block_size_print;
//extern const unsigned long allocated_size;
extern const unsigned long num_of_directories;
extern char const * dirpathprint;
extern char const *	drpath_total;
extern char const *drpathsenseGyear;
extern char const *dirname_valid;
extern char const *dirname_filtered;
extern char const *dirname_filteredProbabilities;
extern char const *dirname_infoemergence;
extern char const *dirname_Parameters_values;
extern char const *dirname_SenseSINFO;
extern char const *alldirectories[];

extern float step_increase_threshold_wrtMax;
extern float min_prop_years_with_increase_wrtWindowSize;
extern float min_prop_previouslowyears_wrtoutsidetheWindow;
extern float max_threshold_previouslowyearswrtMax;

extern float min_totalincrease_surges_wrtWindowSize;

extern int const numoflabels;
extern char const *emergenceprintinglabels[];

extern char const *colonames_SENSESINFO;// for evaluation

extern char const *colonames_allemer;
extern int bflag;

/*
 * I can store each file descriptor in a map! not nessecarly
 */

typedef struct _SENSE_EMERGENCE{

	// the range of the data per a sense
	int s_min_year_counts; // the first year in which this sense has some data (when it start to exist in the data)
	int s_min_year_counts_value; // the value that the first year.
	int s_max_year_counts; // the last year in which this sense has some data
	int s_max_year_counts_value; // the value when its the last year when the sense has some data.


	// the maximum year where we have data for this sense
	long double s_max_sense_given_year_value;
	unsigned int s_max_sense_given_year_year;
	//pair<int,int>min_maxyear_freq;

	int catchBaselineEmergence;// do it by looking at the difference from the matrix (takes either 0=start or 1=not start)
	int sensStartYearOrder;// zero means emerged at the start of the data for this wsd (not true emergence).


	int Senseemerged_BLprameter;

	int order_diff_table;

}SENSEEMERGENCE;



typedef struct _EMERGENCEWSD{

	char *wsdname;
	unsigned int maxyear;
	unsigned int minyear;
	unsigned int total_num_instances;// start from 1
	map<string,int> *sensescounts;// start from 1
	//map<string, map<string,int>, int>*sensescounts;  //<sense, <Major/minor,counts> totalcounts>
	unsigned int num_senses;// start from 1

	set<string> *senseorder;// start from 0
	//unsigned char **sensesorders;
	// unsigned int **yearsorders;
	int **senses_years_counts;//matrix which contains the frequency for each year and sense
	int *coutsallsenses_by_YEAR;

	long double **sense_given_year;// conditional probability for the original data, without filtering or removing the missing years

	int *is_valid_year;// missing years: yes 1, no 0

	int *is_filtered_year;// by threshold: yes 1, no 0

	unsigned int size_of_threshold_data;
	//int **senses_given_years_filtered;

	map<string, SENSEEMERGENCE*> *senses_min_max;

	long int **diff_senses_firstyears;

	int *emergenceyearsbysenses;//from martin algorithm

	int wsdEmerged_BLparamter;

	char ***sensesOrderByStartYear;
	char ** senses_filteredTrueEmeregence;

	unsigned int size_sortedyear;
	unsigned int *sortedyears;// no duplicates
}WSIEMERGENCE;


void print_toFile_allemergence_Sense_EXP(WSIEMERGENCE *wsi, char const *colonamesSENSESINFO,map<int, set<int> *> *wsi_surges,char *dirnamesense);


void print_parameters_values_EXP(char const *respath,unsigned int thershold_exp, unsigned int windowsize_exp, float step_increase_threshold_Max,
		float min_total_surges_window_increase, float max_threshold_previouslowyears_Max,
		float min_prop_previouslowyears_wrtoutsidetheWindow, char const *dirname_ParametersValues);



//void print_emergencealgorithm(WSIEMERGENCE *wsi, int n, int np,int sensenum, int countSteps);
//void set_parameters(unsigned int thershold_exp, unsigned int windowsize_exp,float step_increase_threshold_wrtMax, float max_threshold_previouslowyears);
void usage_EXP(FILE *stream);
void set_parameters_EXP(char const *resultspath, unsigned int thershold_exp, unsigned int windowsize_exp, float step_increase_threshold_Max ,float max_threshold_previouslowyears_Max);

void print_OnlyDetected_BLemergence_EXP(WSIEMERGENCE *wsi, FILE *BLTrueonly);
void print_detectedORnot_BLemergence_EXP(WSIEMERGENCE *wsi, FILE *BLfile);
void detect_baseline_Emergence_EXP(WSIEMERGENCE *wsd, int timeInterval_from_start);

void sort_selection_years_EXP(WSIEMERGENCE *wsd);
void remove_duplicate_EXP(WSIEMERGENCE *wsd);
void rank_baseline_Sensestartyears_EXP(WSIEMERGENCE *wsd);
void label_baseline_Emergence_inOrder_EXP(WSIEMERGENCE *wsd);
void print_toFile_RankedBLYearsDIFF_EXP(WSIEMERGENCE *wsi,  FILE *wf);

void create_rankYearArray_EXP(WSIEMERGENCE *wsd);

//void print_toFile_BLYearsDIFF(WSIEMERGENCE *wsi, FILE *wf);
void create_RankedBLYearsDIFF_table_EXP(WSIEMERGENCE *wsi, char ***sensesorder);

//void create_BLYearsDIFF_table(WSIEMERGENCE *wsi, int max_nunmber_Datasenses);
//void print_toFile_BLYear_EMEYear_DIFF_BL_EME(WSIEMERGENCE *wsi, FILE *wf);


void print_toFile_StartYearOCCURENCEONLY_EXP(WSIEMERGENCE *wsi, FILE *writefile_startyear);
void print_toFile_allemergence_year_EXP(WSIEMERGENCE *WSICONTENT, FILE *writefile);
//FILE *write_to_FILE(char const *dirname_allemergence);
FILE *write_to_FILE_EXP(char const *dirname_allemergence, char const *colonames, char const *filename );

long double ***layout_print_EXP(WSIEMERGENCE *wsi);
void print_toFile_senseinfo_emergence_year_EXP(WSIEMERGENCE *wsi, char const *dir_emergence);



unsigned int Step_EXP(WSIEMERGENCE *wsi,SENSEEMERGENCE *senseinfo,int *is_FilteredorValid_year,int sensenum,int np);
unsigned int SurgeStrat_EXP(int n, int r, WSIEMERGENCE *wsi,int *is_FilteredorValid_year , int sensenum );
//unsigned int Step(WSIEMERGENCE *wsi,int *is_FilteredorValid_year,int sensenum,int np);
map<int, set<int>*> *detect_WSI_SENSE_GIVEN_YEAR_EMERGENCE_EXP(WSIEMERGENCE *wsi, int *is_FilteredorValid_year, unsigned int value_to_filter,
		unsigned int windowsize);

char *addlog_to_Directories_Paths_EXP_NOTMICS(char const *logResultPath, unsigned int thershold_exp, char const *dirname);


//void min_max_senses_by_Year_forWSI(unsigned int *min, unsigned int *max, map<string, WSIEMERGENCE *> *WSI_EMERGENCE_INFO, char const *dirname_print);
//?multiplechar *addlog_to_Directories_Paths_EXP(char const *logResultPath, unsigned int thershold_exp, char const *dirname);
char const *create_LogRESULTS_Directory_EXP_NOTMISC(char const *results_path);

char *get_sensestring_from_index_EXP(WSIEMERGENCE *wsi, int sensenum);
char **get_sensesstrings_in_order_EXP(WSIEMERGENCE *wsi);
void store_min_max_senses_years_from_COUNTS_EXP(WSIEMERGENCE *wsi);// not debugged
void store_min_max_senses_years_from_CONDITIONAL_EXP(WSIEMERGENCE *wsi);

void print_sense_given_year_FILTERED_threshold_matrix_EXP(WSIEMERGENCE *wsi, int *is_FilteredorValid_year, unsigned int thershold_exp, char const *dirname_thresholdConProbabilities);

void print_toFile_label_years_with_certain_value_EXP(WSIEMERGENCE *wsi, int *is_included, int value_to_filter, char const *dirname_filtered);
void lable_years_with_certain_value_EXP(WSIEMERGENCE *wsi, int **is_included, int value_to_filter);


void print_sense_given_year_matrix_EXP(WSIEMERGENCE *wsi, char const *dirname_sense_given_year);
void generate_sense_given_year_matrix_EXP(WSIEMERGENCE *wsi);
long double calculate_sense_given_year_EXP(WSIEMERGENCE *wsi, int yearIndex, int senseIndex);


void print_WSI_EMERGENCE_INFO_EXP(map<string, WSIEMERGENCE *> *WSI_EMERGENCE_INFO, char const * dirname_print_map);


void read_COUNTS_By_YEAR_and_SENSE_EXP(map<string, WSIEMERGENCE *> *WSI_EMERGENCE_INFO, char const *dirname_print);
unsigned int read_COUNTS_EXP(char const  *dirname, char ****targetContent, char **targetNames);//works



unsigned int get_start_consecutive_years(WSIEMERGENCE *wsi, unsigned int threshold_exprmnt, unsigned int lastYear_tobe_used);

void filter_senses_years_counts_by_threshold_EXP(WSIEMERGENCE *wsi, int year);//NOT USED
unsigned int filter_minFreq_by_YEAR_EXP(WSIEMERGENCE *wsi, unsigned int threshold_exprmnt);//NOT USED
void generate_total_byYear_EXP(WSIEMERGENCE *wsi);


void store_WSIEMEREGENCE_map_EXP(WSIEMERGENCE *WSICONTENT,map<string, WSIEMERGENCE *> *WSI_EMERGENCE_INFO);
void print_generate_counts_bySenseandYear_EXP(WSIEMERGENCE *wsim , char const *dirname_print);
int getSenseIndex_set_EXP(char *sense, set<string> *senseorder);
void generate_counts_bySenseandYear_EXP(char ***targetContent, unsigned int size ,WSIEMERGENCE *wsi, int col_yearIndex, int col_wsdIndex );

WSIEMERGENCE *store_WSDContent_EXP(char ***targetContent, unsigned int size,unsigned int thershold_exp);
unsigned int store_WSIDATA_EXP(char const *dircontent, char ****targetContent);
//map<string, WSIEMERGENCE *> *read_WSI(char const *dirname, char const *dirname_print,unsigned int thershold_exp);

map<string, WSIEMERGENCE *> *read_WSI_EXP(char const *dirname, char const *dirname_counts,
		unsigned int thershold_exp, unsigned int windowsize_exp, char const *dirname_sense_given_year, char const *dirname_valid,
		char const *dirname_filtered, char const *dirname_thresholdConProbabilities, char const *dirname_emergence, char const *dirname_allemergence, char const *dirname_senseinfo);

const char **read_dir_Targetscontent_EXP(const char *dirname);

char ****read_WSI_Targets_EXP(char const  *dirname);// not tested!
