/*
 * EvaluateEmergence_test.cpp
 *
 *  Created on: 5 Feb 2020
 *      Author: alsulaia
 */

#include "EvaluateEmergence.h"



void usage_EXP(FILE *stream){

	fprintf(stderr, "Usage: ./EvaluateEmergence_test [options] <Path to Gold EMERGENCE Files> <Path to Predicted EMERGENCE Files>  <OUTPUT Directory> <WindowSizeEmergenceEval>\n");
	fprintf(stderr, "Usage: ./EvaluateEmergence_test [p] <Path to goldpredfile NEO> <Path to goldpredfile SCAN>  <OUTPUT PATH> <WindowSizeEmergenceEval>\n");




	fprintf(stderr,  " <Path to Gold EMERGENCE Files> contains the sense info evaluation files of gold emergence files, if option p choose the input will change\n");
	fprintf(stderr,  " <Path to Predicted EMERGENCE Files> the results of this code are: if option p choose the input will change\n");
	fprintf(stderr,  " <OUTPUT Directory> gives the threshold value, if the threshold value is zero, it means no filtering\n");
	fprintf(stderr,  " <WindowSizeEmergenceEval> gives the window size value, which corresponds to the number of years that you allow .... \n\n");



	fprintf(stderr, "-h <help message>\n");
	fprintf(stderr, "-s <system Name>\n");
	fprintf(stderr, "-p <compare two systems> NEO VS SCAN \n");
	fprintf(stderr, "-i <another input to compare NEO to SCAN> \n");
	fprintf(stderr, "\n");
}





int main (int argc, char **argv){



	int index;
	char *dirGoldpath =NULL;
	char *dirPredpath=NULL;
	char *results_path=NULL;
	char *resultsPath=NULL;
	char *systemName=NULL;
	int compare =0;
	int evalcount=0;
	char const *logResultPath=NULL;
	unsigned int windowsizeemergenceeval=0;
	char *secondpath=NULL;
	for (index = optind; index < argc; index++){
		printf ("Non-option argument %s\n", argv[index]);
	}

	int c;
	opterr = 0;
	while ((c = getopt (argc, argv, "hs:pi:e")) != -1)
		switch (c)
		{

		case 'h':
			usage_EXP(stdout);
			exit(0);

		case 's':
			systemName=optarg;
			break;

		case 'p':
			compare=1;
			break;

		case 'i':
			secondpath=optarg;
			break;

		case 'e':
			evalcount=1;
			break;

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


	if (optind+4==argc) {

		int lenp =strlen(argv[optind]);
		dirGoldpath= (char *)malloc((1+lenp)*sizeof(char));
		strcpy(dirGoldpath , argv[optind]);

		int llnng=strlen(argv[optind+1]);
		dirPredpath=(char *)malloc((1+llnng)*sizeof(char));
		strcpy(dirPredpath, argv[optind+1]);

		int llnng2=strlen(argv[optind+2]);
		results_path=(char *)malloc((1+llnng2)*sizeof(char));
		strcpy(results_path , argv[optind+2]);

		char const *timestamp =give_time_stamped();

		int len=strlen(results_path);
		int len2=strlen(timestamp);
		resultsPath=(char *)malloc(len+len2+1*sizeof(char));
		strcpy(resultsPath,results_path);
		strcat(resultsPath,timestamp);

		//	fprintf(stderr, "I am resultsPath %s\n", resultsPath);

		char const *windowsizeeval = strdup(argv[optind+3]);
		windowsizeemergenceeval=atoi(windowsizeeval);

	}   else {
		fprintf(stderr,"Error: wrong number of arguments, expecting 4\n");
		usage_EXP(stderr);
		exit(5);
	}



	//	unsigned int windowsize_exp=0;
	//	float step_increase_threshold_Max=-1;
	//	float max_threshold_previouslowyears_Max=-1;
	//	char const *dirname;
	//	char const *filename;
	//	OUTPUTSEMERGENCE * outputsEmergence =call_EMEREXP( windowsize_exp,  step_increase_threshold_wrtMax , max_threshold_previouslowyearswrtMax,dirname, filename);




	if(evalcount==1){


		char ***EvalNEOContent= (char ***)malloc(block_size*sizeof(char **));
		EvalNEOContent[block_size-1]=NULL;
		const char * goldpredNEO=dirGoldpath;
		unsigned int input1size =store_WSIDATA_EVAL(dirGoldpath, &EvalNEOContent);


		char ***EvalSCANContent= (char ***)malloc(block_size*sizeof(char **));
		EvalSCANContent[block_size-1]=NULL;
		const char * goldpredSCAN=dirPredpath;
		unsigned int input2size =store_WSIDATA_EVAL(goldpredSCAN, &EvalSCANContent);// the input size doesnt include the header



map<string, char ***>*Goldemergence=new map<string, char ***>();
map<string, char ***>*GoldNOemergence= new map<string, char ***>();


multimap<string, char ***>*GoldemergenceSense=new multimap<string, char ***>();
multimap<string, char ***>*GoldNOemergenceSense= new multimap<string, char ***>();
goldemergencesetbyTarget(EvalNEOContent,input1size,Goldemergence,GoldNOemergence,3);
goldemergencesetbySense(EvalNEOContent,input1size,GoldemergenceSense,GoldNOemergenceSense);


map<string, char ***>*Predemergence=new map<string, char ***>();
map<string, char ***>*PredNOemergence= new map<string, char ***>();

goldemergencesetbyTarget(EvalNEOContent,input1size,Predemergence,PredNOemergence,4);



//map<string, char ***>*SCANPredemergence=new map<string, char ***>();
//map<string, char ***>*SCANPredNOemergence= new map<string, char ***>();

//goldemergencesetbyTarget(EvalSCANContent,input1size,SCANPredemergence,SCANPredNOemergence,4);



fprintf(stderr,"size gold emergence by Target %lu\n",Goldemergence->size());
fprintf(stderr,"size gold Noemergence by Target %lu\n",GoldNOemergence->size());

fprintf(stderr,"%s size pred emergence by Target %lu\n","NEO",Predemergence->size());
fprintf(stderr,"%s size pred Noemergence by Target %lu\n","NEO",PredNOemergence->size());

//fprintf(stderr,"%s size pred emergence by Target %lu\n","SCAN",SCANPredemergence->size());
//fprintf(stderr,"%s size pred Noemergence by Target %lu\n","SCAN",SCANPredNOemergence->size());


exit(1);
		return 0;
	}




	if(compare ==1){

		// Reading the evaluation results after running this code to allow comparisons between goldpred files.
		char ***EvalNEOContent= (char ***)malloc(block_size*sizeof(char **));
		EvalNEOContent[block_size-1]=NULL;
		const char * goldpredNEO=dirGoldpath;
		unsigned int input1size =store_WSIDATA_EVAL(dirGoldpath, &EvalNEOContent);


		char ***EvalSCANContent= (char ***)malloc(block_size*sizeof(char **));
		EvalSCANContent[block_size-1]=NULL;
		const char * goldpredSCAN=dirPredpath;
		unsigned int input2size =store_WSIDATA_EVAL(goldpredSCAN, &EvalSCANContent);// the input size doesnt include the header


		//	compare_Emergence(EvalNEOContent, EvalSCANContent, input1size,results_path);


		compare_Emergence_tiddyTable(EvalNEOContent, EvalSCANContent, input1size, results_path);

		//compare_Emergence_includeYears(EvalNEOContent, EvalSCANContent, input1size, results_path);



	}else{

		createDirSafely(resultsPath);

		int IndivErrorIndex=-1;
		int NormalisedErrorIndex=-1;
		int EvalStatusIndex=-1;

		map<string, char ***>*GoldEmergenceContent=read_emergence(dirGoldpath);//GOLD
		map<string, char ***>*PredEmergenceContent=read_emergence(dirPredpath);//PREDICTED

		char ***goldpredemergencetable	=create_goldpredtable(GoldEmergenceContent, PredEmergenceContent,  windowsizeemergenceeval);



		print_goldpredtable(goldpredemergencetable,resultsPath);



		for(int i=0; i<20; i++){
			if(strcmp("IndivError",colonames_allemerEval[i])==0){
				IndivErrorIndex=i;
			}
			if(strcmp("NormalisedError",colonames_allemerEval[i])==0){
				NormalisedErrorIndex=i;
			}
			if(strcmp("EvalStatus",colonames_allemerEval[i])==0){
				EvalStatusIndex=i;
			}
		}



		unsigned int** emergencebinarymatrix =counts_performance(goldpredemergencetable, EvalStatusIndex);

		double GlobalmeanabsoluteError=sum_meanabsoluteError(goldpredemergencetable, IndivErrorIndex);




		double GlobalnormalisedmeanabsoluteError=normalised_meanabsoluteError(goldpredemergencetable, NormalisedErrorIndex);

		print_binaryEmergenceMatrix(emergencebinarymatrix,resultsPath);

		double*performance= calculate_performance(emergencebinarymatrix);

		print_performance( performance, GlobalmeanabsoluteError,GlobalnormalisedmeanabsoluteError,resultsPath);

		MAE_format(goldpredemergencetable, resultsPath);

	}


	return 0;

}
