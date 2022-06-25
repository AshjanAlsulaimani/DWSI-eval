/*
 * EVALUATEDISTANCE_test.cpp
 *
 *  Created on: 8 May 2020
 *      Author: alsulaia
 */

#include "EVALUATEDISTANCE.h"

void usage_EXP(FILE *stream){

	fprintf(stderr, "Usage: ./EVALUATEDISTANCE_test [options] <Path to Gold EMERGENCE Files> <Path to Predicted Files>  \n");




	fprintf(stderr,  " <Path to Gold EMERGENCE Files> contains the sense info evaluation files of gold emergence files, if option p choose the input will change\n");


	fprintf(stderr, "-h <help message>\n");
	fprintf(stderr, "-s <system Name>\n");
	fprintf(stderr, "\n");
}





int main (int argc, char **argv){



	int index;
	char *dirGoldpath =NULL;
	char *dirPredpath=NULL;
	char *results_path=NULL;
	char *systemName=NULL;
	char *resultsPath=NULL;

	char const *logResultPath=NULL;

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


	if (optind+2==argc) {

		int lenp =strlen(argv[optind]);
		dirGoldpath= (char *)malloc((1+lenp)*sizeof(char));
		strcpy(dirGoldpath , argv[optind]);

		int llnng=strlen(argv[optind+1]);
		dirPredpath=(char *)malloc((1+llnng)*sizeof(char));
		strcpy(dirPredpath, argv[optind+1]);

		//		int llnng2=strlen(argv[optind+2]);
		//		results_path=(char *)malloc((1+llnng2)*sizeof(char));
		//		strcpy(results_path , argv[optind+2]);
		//
		//		char const *timestamp =give_time_stamped();
		//
		//		int len=strlen(results_path);
		//		int len2=strlen(timestamp);
		//		resultsPath=(char *)malloc(len+len2+1*sizeof(char));
		//		strcpy(resultsPath,results_path);
		//		strcat(resultsPath,timestamp);


	}   else {
		fprintf(stderr,"Error: wrong number of arguments, expecting 3\n");
		usage_EXP(stderr);
		exit(5);
	}




	int checkFILE=checkDirectory(dirGoldpath);


	if(checkFILE==0){


		FILE	*FP;
		const char *filename="ALLtargetsenses_ecludeDis";
		int len=strlen(dirPredpath);
		int len2 =strlen(filename);
		char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
		strcpy(dirnamepath,dirPredpath);
		strcat(dirnamepath,"/");
		strcat(dirnamepath, filename);
		strcat(dirnamepath,".tsv");
		FP = fopen(dirnamepath,"w");

		if(FP==NULL){
			//fprintf(stderr, "Path is %s", resultsPath);
			fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
			exit(1);
		}


		fprintf(FP, "target\tpred\tgold\teuclideanValue\n");

		fprintf(stderr, "I AM HERE FILE\n");
		unsigned int goldsize;
		unsigned int predsize;
		char *** goldData =read_probabilitiesvectors(dirGoldpath, &goldsize);
		char *** predData =read_probabilitiesvectors(dirPredpath, &predsize);
		if(goldsize==predsize){
			//createDirSafely(resultsPath);
			char *target=strdup("NA");
			calculate_euclideanDistance( goldData,  predData,goldsize, resultsPath,target, FP);
		}else{
			fprintf(stderr, "size is not equal!\n");
			exit(1);
		}

	}else{

		fprintf(stderr,"I AM HERE DIRECTORY\n");
		FILE	*FP;
		const char *filename="ALLtargetsenses_ecludeDis";
		int len=strlen(dirPredpath);
		int len2 =strlen(filename);
		char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
		strcpy(dirnamepath,dirPredpath);
		strcat(dirnamepath,"/");
		strcat(dirnamepath, filename);
		strcat(dirnamepath,".tsv");
		FP = fopen(dirnamepath,"w");

		if(FP==NULL){
			//fprintf(stderr, "Path is %s", resultsPath);
			fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
			exit(1);
		}


		fprintf(FP, "target\tpred\tgold\teuclideanValue\tnormSensebyyear\n");


		int targetDone=0;

		const char **preddirfiles =read_dir_content_FULL(dirPredpath);
		int i=0;
		for(int i=0; preddirfiles[i]!=NULL; i++){

			fprintf(stderr, "PredTARGET: %s\n", preddirfiles[i]);

			char Targetdirname[700];
			strcpy(Targetdirname, dirPredpath);
			strcat(Targetdirname,"/");
			strcat(Targetdirname,preddirfiles[i]);

			int	x	=is_regular_file(Targetdirname);//one means yes

			if(x==0){

				//	puts("0\n");
				//	fprintf(stderr, "THE TARGET PATH %s\n", Targetdirname);

				char *target=NULL;
				const char *	goldpathfile=get_Tgoldpath((const char *)dirGoldpath,Targetdirname, &target);

				const char **	predDirs=read_dir_content_FULL(Targetdirname);

				for(int i=0; predDirs[i]!=NULL; i++){

					char matchedGoldPred[700];
					strcpy(matchedGoldPred,Targetdirname);
					strcat(matchedGoldPred,"/");
					strcat(matchedGoldPred,predDirs[i]);

					if((strstr(predDirs[i],"matchedGoldPred")!=NULL)  && (is_regular_file(matchedGoldPred)==0)){


						fprintf(stderr, "THE MATCH PRED DIR %s\n", matchedGoldPred);
						const char **	files=read_dir_content_FULL(matchedGoldPred);

						int f=0;
						while(files[f]!=NULL){


							char predSensefile[700];
							strcpy(predSensefile, matchedGoldPred);
							strcat(predSensefile,"/");
							strcat(predSensefile,files[f]);

							if((strcmp(files[f],"learnedprobsensebasedmatchingforemergence.txt")==0)  && (is_regular_file(predSensefile)==1)){

								unsigned int goldsize;
								unsigned int predsize;

								char *** goldData =read_probabilitiesvectors((char *)goldpathfile, &goldsize);
								char *** predData =read_probabilitiesvectors(predSensefile, &predsize);

								if(goldsize==predsize){
									//fprintf(stderr, "DEBUG: %s\n", target);
									calculate_euclideanDistance( goldData,  predData,goldsize,matchedGoldPred, target, FP);

									targetDone++;
								}else{
									fprintf(stderr, "size is not equal!\n");
									exit(1);
								}
							}
							f++;
						}
					}
				}
			}
		}

		fprintf(stderr, "target numbers %d\n", targetDone);
	}



	return 0;
}
