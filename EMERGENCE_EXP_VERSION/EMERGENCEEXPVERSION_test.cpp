/*
 * EMERGENCEEXPVERSION_test.cpp
 *
 *  Created on: 20 Dec 2019
 *      Author: alsulaia
 */

#include "EMERGENCEEXPVERSION.h"

//#include "EMERGENCE.h"

#include <ctype.h>
#include <unistd.h>



void usage_EXP(FILE *stream){

	fprintf(stderr, "Usage: ./EMERGENCE_test [options] <Path to the WSI files> <PATH TO RESULTS DIRECTORY> <Threshold filter Value> <Window Size Value>\n");

	fprintf(stderr,  "<path to the WSI files> contains the files of frequencies that are collected from MEDLINE for each WSI (sense by year)!\n");
	fprintf(stderr,  "<path to results directory> the results of this code are: \n");
	fprintf(stderr,  "<threshold filter value> gives the threshold value, if the threshold value is zero, it means no filtering\n");
	fprintf(stderr,  "<window size value> gives the window size value, which corresponds to the number of years you take into account\n\n");

	fprintf(stderr, "Options\n\n");

	fprintf(stderr, "-h <help message> ");
	fprintf(stderr, "-b <absolute or proportional wrt Max> use the low years threshold as absolute value. By default the low years threshold is multiplied by the maximum\n");

	//fprintf(stderr, " -l use 0.1 for the max threshold i.e. use the threshold as absolute value. By default the threshold 0.1 is multiplied by the maximum probability)\n");
	//fprintf(stderr,"  -s <step increase value threshold> gives (the condition) 0.02% for having 20% total increase between the steps within any window size with respect to Max probability or any total increase you are looking to achieve, default value: i\n", step_increase_threshold_wrtMax);

	fprintf(stderr,"-l <low value threshold> gives the value (the condition) zero for no occurrences before the year of emergence, default value: %lf\n", 0.1);

	fprintf(stderr,"-s <step increase value threshold> gives the minimum increase by year (as proportion of the maximum) for a surge. By default, this value is calculated based on the window size (so that the total window increase is at least 20%% of the max\n");

	//fprintf(stderr, " -")

}



int main (int argc, char **argv){

	//fprintf(stderr, "ME: step_increase_threshold_wrtMax is %lf\n", step_increase_threshold_wrtMax);
	int index;
	int c;
	opterr = 0;
	while ((c = getopt (argc, argv, "bl:s:")) != -1)
		switch (c)
		{
		case 'b':
			bflag = 1;
			break;

		case 'l':
			max_threshold_previouslowyearswrtMax = atof(optarg);
			break;

		case 's':

			step_increase_threshold_wrtMax = atof(optarg);


			fprintf(stderr, "OW: step_increase_threshold_wrtMax is %lf\n", step_increase_threshold_wrtMax);

			break;


		case 'h':
			usage_EXP(stdout);
			exit(0);

		case '?':
			if (optopt == 'c')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr,
						"Unknown option character `\\x%x'.\n",
						optopt);
			usage_EXP(stderr);

			return 1;
		default:
			abort ();
		}

	// mandatory arguments
	for (index = optind; index < argc; index++){
		printf ("Non-option argument %s\n", argv[index]);
	}


//
//	fprintf(stderr, "SEE0: step_increase_threshold_wrtMax is %lf\n", step_increase_threshold_wrtMax);
//	fprintf(stderr, "SEE0: optind is %d\n", optind);
//	fprintf(stderr, "SEE0: argc is %d\n", argc);
//	fprintf(stderr, "SEE0: argv[optind] is %s\n", argv[optind]);
//	fprintf(stderr, "SEE0: argv[optind+1] is %s\n", argv[optind+1]);
//	fprintf(stderr, "SEE0: argv[optind+2] is %s\n", argv[optind+2]);
//	fprintf(stderr, "SEE0: argv[optind+3] is %s\n", argv[optind+3]);
//	fprintf(stderr, "SEE0: argv[optind+4] is %s\n", argv[optind+4]);



	for (index = optind; index < argc; index++){
		printf ("Non-option argument %s\n", argv[index]);
		}



	char *dirpath =NULL;
	char *results_path=NULL;
	char const *logResultPath=NULL;
	unsigned int thershold_exp =0;
	unsigned int windowsize_exp =0;

	//if (optind+4==argc) {

		if (optind+4==argc) {

//		fprintf(stderr, "SEEIN: optind is %d\n", optind);
//		fprintf(stderr, "SEEIN: argc is %d\n", argc);
//		fprintf(stderr, "SEEIN: argv[optind] is %s\n", argv[optind]);
//		fprintf(stderr, "SEEIN: argv[optind+1] is %s\n", argv[optind+1]);
//		fprintf(stderr, "SEEIN: argv[optind+2] is %s\n", argv[optind+2]);
//		fprintf(stderr, "SEEIN: argv[optind+3] is %s\n", argv[optind+3]);
//		fprintf(stderr, "SEEIN: argv[optind+4] is %szn", argv[optind+4]);

		int lenp =strlen(argv[optind]);
		dirpath = (char *)malloc((1+lenp)*sizeof(char));
		strcpy(dirpath, argv[optind]);

		//  check if its a real directory
		createDirSafely(dirpath);

		int llnng=strlen(argv[optind+1]);
		results_path=(char *)malloc((1+llnng)*sizeof(char));
		strcpy(results_path, argv[optind+1]);

		logResultPath=create_LogRESULTS_Directory_EXP_NOTMISC(results_path);

		thershold_exp =atoi(argv[optind+2]);
		windowsize_exp =atoi(argv[optind+3]);

	}   else {
		fprintf(stderr,"Error: wrong number of arguments, expecting 4\n");
		usage_EXP(stderr);
		exit(5);
	}



	char **DIRECTORIESwithPATH= (char **)malloc(num_of_directories*sizeof(char *));
	for(int i=0; i<num_of_directories; i++){
		DIRECTORIESwithPATH[i]= addlog_to_Directories_Paths_EXP_NOTMICS(logResultPath,thershold_exp, alldirectories[i]);
		fprintf(stderr, "DIRECTORIESwithPATH name %d\t%s\n", i,DIRECTORIESwithPATH[i]);
	}

	//		fprintf(stderr,
	//			"DEBUGF: Im read_WSI and the parameters for the directories: dirname_counts\t%s\n,dirname_sense_given_year\t%s\n,dirname_valid\t%s\n,dirname_filtered\t%s\n,dirname_thresholdConProbabilities\t%s\n",
	//			DIRECTORIESwithPATH[0], DIRECTORIESwithPATH[2],DIRECTORIESwithPATH[3], DIRECTORIESwithPATH[4],
	//			DIRECTORIESwithPATH[5]);



	fprintf(stderr,"FIND: the parameters values BEFORE:\n");

	fprintf(stderr,"FIND: thershold_exp %d\n", thershold_exp);
    fprintf(stderr,"FIND: windowsize_exp %d\n", windowsize_exp);
    fprintf(stderr,"FIND: step_increase_threshold_wrtMax %lf\n", step_increase_threshold_wrtMax);
	fprintf(stderr,"FIND: max_threshold_previouslowyearswrtMax %lf\n", max_threshold_previouslowyearswrtMax);
    fprintf(stderr, "FIND: min_totalincrease_surges_wrtWindowSize %lf\n", min_totalincrease_surges_wrtWindowSize);

	set_parameters_EXP(logResultPath, thershold_exp,  windowsize_exp, step_increase_threshold_wrtMax, max_threshold_previouslowyearswrtMax);


	map<string, WSIEMERGENCE *> *WSI_EMERGENCE_INFO= read_WSI_EXP(dirpath, DIRECTORIESwithPATH[0],thershold_exp, windowsize_exp, DIRECTORIESwithPATH[2], DIRECTORIESwithPATH[3], DIRECTORIESwithPATH[4],DIRECTORIESwithPATH[5],DIRECTORIESwithPATH[6],DIRECTORIESwithPATH[7],DIRECTORIESwithPATH[8]);







	//print_toFile_label_years_with_certain_value(wsi, wsi->is_valid_year, 0, dirname_filtered);



	//          printf("\n");
	//
	//		puts("I AM CALLING read_COUNTS_By_YEAR_and_SENSE\n");
	//	read_COUNTS_By_YEAR_and_SENSE(WSI_EMERGENCE_INFO, dirpathprint);

	print_WSI_EMERGENCE_INFO_EXP(WSI_EMERGENCE_INFO, DIRECTORIESwithPATH[1]);


	puts("*******************************IM STARTING FINDING THE EMERGENCE*********************\n");


	//TODO: FREE

	return 0;

}
