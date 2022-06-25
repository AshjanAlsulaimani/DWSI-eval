/*
 * ABSTACTFULLVERSION_test.cpp
 *
 *  Created on: 12 Dec 2019
 *      Author: alsulaia
 */

#include "ABSTACTFULLVERSION.h"

void usage(FILE *stream){

	fprintf(stderr, "Usage: ./ABSTRACTS_test  [Options] <PATH TO ABSTRACT DIRECTORY> <PATH TO RESULTS> <Window Size Value> <TOKENIZATION> \n");

	fprintf(stderr, "Required arguments\n\n");
	//fprintf(stderr,  "<Path to the UMLS Terms> contains the terms for each concept collected from UMLS Meta\n");
	fprintf(stderr,  "<PATH TO ABSTRACT Directory> contains the abstracts that are collected for each target word and its concepts\n");
	fprintf(stderr,  "<PATH TO RESULTS> provides the path where the output results should be stored\n");
	fprintf(stderr,  "<Window Size Value> gives the window size value, which corresponds to the number of years you take into account: WindowSize (5) [default]\n");
	//	fprintf(stderr,  "<CASESENSITIVE> gives  whether its case sensitive (1) or not (0): Case sensitive (1) [default] \n");
	fprintf(stderr,  "<TOKENIZATION> gives  whether its Spacy tokenizer (1) or not (0): Spacy (1) [default] \n");
	//    fprintf(stderr, "<UMLSTERMS as Target> gives whether to use UMLS terms as target (1) or not (0): UMLS Terms (0) [default]\n");

	fprintf(stderr, "Options\n\n");
	fprintf(stderr, "-h help message ");
	fprintf(stderr, "-s CASE SENSITIVE for detecting the targets only [default not used] \n");
	fprintf(stderr, "-t <UMLSTERMS PATH> UMLS Terms [default not used]\n");
	//fprintf(stderr, "-f <Target Terms PATH> GOLD Terms [default not used]\n");
	fprintf(stderr, "-p Padding [default not used]\n");
	fprintf(stderr, "-r Duplicate [default not used]\n");
	fprintf(stderr, "-w Remove highly frequent words and punctuation [default not used]\n");
	fprintf(stderr, "-n Remove hyphen [default not used]\n");
	fprintf(stderr, "-l lowercase all text [default not used]\n");
}






int main (int argc, char * argv[]){


	int index=0;
	unsigned int lowercase=0;
	unsigned int hyphen=0;
	unsigned int caseSensitive =0;
	unsigned int UMLSoption=0;
	unsigned int FilteredUMLSoption=0;
	unsigned int padding =0;
	unsigned int r_duplicate=0;
	unsigned int FilteredStopWord=0;
	char *pathTerms = NULL;
	char *TargetpathTerms= NULL;
	char *StoPWordsfile=NULL;
	int c;
	opterr = 0;
	while ((c = getopt (argc, argv, "hst:f:prw:nl")) != -1)
		switch (c)
		{

		case 'h':
			usage(stdout);
			exit(0);


		case 's':
			caseSensitive = 1;
			break;

			//			caseSensitive= atof(optarg);
			//			fprintf(stderr,"CASE SENSITIVE %d\n",caseSensitive);
			//			break;


		case 't':
			UMLSoption=1;
			pathTerms= optarg;
			fprintf(stderr, "UMLSTERMS PATH to enable UMLS TERMS AS TARGETS %s\n", pathTerms);
			break;




		case 'w':
			FilteredStopWord= 1;
			StoPWordsfile= optarg;
			fprintf(stderr, "PATH TO STOP WORD FILE %s\n", StoPWordsfile);
			break;




		case 'n':
			hyphen= 1;
			fprintf(stderr, "HYPHEN IS USED! %d\n", hyphen);
			break;



			//		case 'f':
			//			FilteredUMLSoption= 1;
			//			TargetpathTerms= optarg;
			//			fprintf(stderr, "Gold Terms PATH to enable UMLS TERMS AS TARGETS %s\n", TargetpathTerms);
			//			break;
			//



		case 'p':
			padding =1;
			fprintf(stderr, "Padding option is %d\n", padding);
			break;



		case 'r':
			r_duplicate= 1;
			fprintf(stderr, "r_duplicate option is %d\n",  r_duplicate);
			break;



		case 'l':
			lowercase =1;
			fprintf(stderr, "lowercase option is %d\n", lowercase);
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
			usage(stderr);

			return 1;
		default:
			abort ();
		}


	for (index = optind; index < argc; index++){
		fprintf (stderr, "%d Non-option argument that are given %s\n", index ,argv[index]);
	}


	char *pathAbstract = NULL;
	char *pathresult= NULL;
	unsigned int windowsize = 5;
	unsigned int tokeniseOption= 1;




	if (optind+4==argc) {

		//		int lenp =strlen(argv[1]);
		//		pathTerms = (char *)malloc((1+lenp)*sizeof(char));
		//		strcpy(pathTerms, argv[1]);

		int lenp2 =strlen(argv[optind]);
		pathAbstract = (char *)malloc((1+lenp2)*sizeof(char));
		strcpy(pathAbstract, argv[optind]);

		int lenp3 =strlen(argv[optind+1]);
		pathresult = ( char *)malloc((1+lenp3)*sizeof(char));
		strcpy(pathresult, argv[optind+1]);

		windowsize= atoi(argv[optind+2]);
		tokeniseOption= atoi(argv[optind+3]);

		//caseSensitive= atoi(argv[5]);
		//UMLSoption= atoi(argv[7]);

	}else{
		fprintf(stderr, "ERROR IN NUMBER OF MANDATORY ARGUMENTS, EXPECTING 4\n");
		usage(stderr);
		exit(1);
	}

	createDirSafely(pathresult);
	char const *outputdirectories[0];

	//	char const *logResultPath=NULL;
	//	logResultPath=LogRESULTS_Directory(pathresult);
	//
	//
	//	char **DIRECTORIESwithPATH= (char **)malloc(outputdirectoriesNum*sizeof(char *));
	//	for(int i=0; i<outputdirectoriesNum; i++){
	//

	char *dirnamepath=NULL;

	if(caseSensitive == 1){
		outputdirectories[0]= {dirname_CaseSensitiveFULL};
		//			DIRECTORIESwithPATH[i]= addlogtoDirectoriesPaths(logResultPath, outputdirectories[i]);
		//			fprintf(stderr, "DIRECTORIESwithPATH name %d\t%s\n", i,DIRECTORIESwithPATH[i]);



		int len = strlen(pathresult);
		dirnamepath = (char *)malloc((55+len)*sizeof(char));
		strcpy(dirnamepath,pathresult);
		strcat(dirnamepath,"/");
		strcat(dirnamepath, outputdirectories[0]);

		createDirSafely(dirnamepath);
	}
	//
	if(caseSensitive == 0 ){
		outputdirectories[0]= {dirname_Not_CaseSensitiveFULL};
		//			DIRECTORIESwithPATH[i]= addlogtoDirectoriesPaths(logResultPath, outputdirectories[i]);
		//			fprintf(stderr, "DIRECTORIESwithPATH name %d\t%s\n", i,DIRECTORIESwithPATH[i]);
		//


		int len = strlen(pathresult);
		dirnamepath = (char *)malloc((55+len)*sizeof(char));
		strcpy(dirnamepath,pathresult);
		strcat(dirnamepath,"/");
		strcat(dirnamepath, outputdirectories[0]);

		createDirSafely(dirnamepath);


	}
	//	}



	fprintf(stderr, "*****PREPARING UMLSterms*****\n");
	map<string, map<string,set<string> *> *> *UMLSterms= NULL;
	if(UMLSoption){

		//fprintf(stderr, "UMLSoption, %d", UMLSoption);
		UMLSterms = read_UMLSterms_FULL(pathTerms,hyphen );

		//		if(FilteredStopWord==1){// remove punctuation from here
		//
		////here we can use our stopword list,
		//
		//
		//		}
		//
	}else{
		UMLSterms =NULL;
	}

set<string> *stopwords=NULL;
if(FilteredStopWord==1){
fprintf(stderr, "*****READING STOPWORDS FILE*****\n");
	// load the file
	stopwords=readandstore_stopfile(StoPWordsfile);


}

	//map<string, string> pmcAbstracts= pmcPMIDS_list(pathPMCLIST);

	print_parameters_file_FULL(pathresult, parameters_full,outputdirectoriesNum_full,windowsize,tokeniseOption,caseSensitive,UMLSoption, padding, r_duplicate,hyphen, FilteredStopWord);

	//fprintf(stderr, "log the main directory %s\n", logResultPath );
	//fprintf(stderr, " the subdirectory in the main DIRwithPath %s\n",DIRECTORIESwithPATH[0] );
	//CORRECTstore_abstract(pathAbstract,logResultPath ,DIRECTORIESwithPATH[0], windowsize, caseSensitive, tokeniseOption,UMLSterms, padding, r_duplicate);




	//NOTFINISHEDread_data_absarts_no_hyphen(pathAbstract,logResultPath ,DIRECTORIESwithPATH[0], windowsize, caseSensitive, tokeniseOption, UMLSterms, padding,r_duplicate, hyphen);
	//	void store_abstract_hypen(char *pathAbstract, char const *pathresult, char *subdirResult, unsigned int windowssize,
	//			unsigned int caseSensitive, unsigned int Spacy, map<string, map<string,set<string> *> *> *UMLSterms, int padding,
	//			int r_duplicate, unsigned int hyphenOption, unsigned int lowercase)

	store_abstract_hypen_FULL(pathAbstract,pathresult ,dirnamepath,  windowsize,caseSensitive,  tokeniseOption, UMLSterms,  padding,r_duplicate,hyphen, lowercase, stopwords);


	puts("Im FINSHIED HERE!\n");

	return 0;
}
