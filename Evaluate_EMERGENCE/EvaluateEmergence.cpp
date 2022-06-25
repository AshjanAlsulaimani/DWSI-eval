/*
 * EvaluateEmergence.cpp
 *
 *  Created on: 5 Feb 2020
 *      Author: alsulaia
 */

#include "EvaluateEmergence.h"
//
//EvaluateEmergence::EvaluateEmergence() {
//	// TODO Auto-generated constructor stub
//
//}
//
//EvaluateEmergence::~EvaluateEmergence() {
//	// TODO Auto-generated destructor stub
//}
/*
 * ./EvaluateEmergence_test  ~/now/MeSHXMLFILES/MeshCoT/FILTERSTOPWORD/EMERGENCE_FILTEREDsw_NEO_someEliminated.0/GOLD_EMERGENCE_Tue_Mar_10_17\:25\:04_2020/SENSES_INFO_EVALUATIONINPUT/  ~/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERSTOPWORD/EdiSCAN_BIOsw/EMERGENCE_FILTERED_SCAN_someEliminated.0/PREDICTED_EMERGENCE_FILTERED_SCAN/ ~/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERSTOPWORD/EdiSCAN_BIOsw/EMERGENCE_FILTERED_SCAN_someEliminated.0/EMERGENCE_EVALUATION_SCAN 5
 *./EvaluateEmergence_test  ~/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERSTOPWORD/EMERGENCE_FILTEREDsw_NEO_someEliminated.0/GOLD_EMERGENCE_Tue_Mar_10_17\:25\:04_2020/SENSES_INFO_EVALUATIONINPUT/  ~/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERSTOPWORD/EMERGENCE_FILTEREDsw_NEO_someEliminated.0/Predicted_EMERGENCE/ ~/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERSTOPWORD/EMERGENCE_FILTEREDsw_NEO_someEliminated.0/EMERGENCE_EVALUATION_NEO 5
 *
 *./EvaluateEmergence_test -p /home/alsulaia/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERSTOPWORD//EMERGENCE_FILTEREDsw_NEO_someEliminated.0/EMERGENCE_EVALUATION_NEO_Tue_Apr_14_21\:42\:32_2020/goldpredtable.tsv /home/alsulaia/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERSTOPWORD/EdiSCAN_BIOsw/EMERGENCE_FILTERED_SCAN_someEliminated.0/EMERGENCE_EVALUATION_SCAN_Tue_Apr_14_21\:43\:48_2020/goldpredtable.tsv /home/alsulaia/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERSTOPWORD/EMERGENCE_FILTEREDsw_NEO_someEliminated.0/EMERGENCE_EVALUATION_NEO_Tue_Apr_14_21\:42\:32_2020/ 0
 */
char const *colonames_allemerEval[]= {"WSD_INFORMATION","CUI","NUM_SENSES","GOLD_EMERGENCE_YEAR","PREDICTED_EMERGENCE_YEAR","MinYearGoldSENSEdata","MaxYearGoldSENSEdata","MinYearPredictedSENSEdata","MaxYearPredictedSENSEdata","StartGoldData","EndGoldData","GoldYearRange","EvalStatus","IndivError","NormalisedError" ,"IndivByTargets","NotmalisedBytargets","GoldEMERGENCEstatus","PredictedEMERGENCEstatus", "GoldPredEMERGENCE"};
const unsigned long block_size= 10;
int numberstored=-1;
//const unsigned long block_size_print= 124;
// not finished
//void compare_Emergence_includeYears(char ***EvalNEOContent, char ***EvalSCANContent, int input1size, char *resultsPath){
//
//	const char *filename="NEO_SCAN_YEAREMERGENCE";
//	int len=strlen(resultsPath);
//	int len2 =strlen(filename);
//	char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
//	strcpy(dirnamepath,resultsPath);
//	strcat(dirnamepath,"/");
//	strcat(dirnamepath, filename);
//	strcat(dirnamepath,".tsv");
//	FILE *fp;
//	fp = fopen(dirnamepath,"w");
//	if(fp==NULL){
//		//fprintf(stderr, "Path is %s", resultsPath);
//		fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
//	}
//
//
//	//char const *colonames_allemerEval[]= {"WSD_INFORMATION(0)","CUI(1)","NUM_SENSES(2)","GOLD_EMERGENCE_YEAR(3)","PREDICTED_EMERGENCE_YEAR(4)","MinYearGoldSENSEdata(5)","MaxYearGoldSENSEdata(6)","MinYearPredictedSENSEdata(7)","MaxYearPredictedSENSEdata(8)","StartGoldData(9)","EndGoldData(10)","GoldYearRange(11)","EvalStatus(12)","IndivError(13)","NormalisedError(14)" ,"IndivByTargets(15)","NotmalisedBytargets(16)","GoldEMERGENCEstatus(17)","PredictedEMERGENCEstatus(18)", "GoldPredEMERGENCE(19)"};
//
//
//	fprintf(fp, "WSD_INFORMATION\tNUM_SENSES\tCUI\tGoldEMERG");
//
//	fprintf(fp,"\tPredictedEMERG(NEO)\tPredictedEMERG(SCAN)\tGoldPredEMERG(NEO)\tGoldPredEMERG(SCAN)\tGoldPred(NEOSCAN)\tPredEMERG(NEOSCAN)\n");
//
//
//	int i=1;
//	while(i<=input1size && EvalNEOContent[i]!=NULL && EvalSCANContent[i]!=NULL ){
//		int s=1;
//		fprintf(stderr, "searching target=%s, sense=%s\n", EvalNEOContent[i][0], EvalNEOContent[i][1]);
//		while (
//				(s<=input1size) && (
//						(strcmp(EvalNEOContent[i][0],EvalSCANContent[s][0]) != 0)
//						|| (strcmp(EvalNEOContent[i][1],EvalSCANContent[s][1])!= 0)
//				)
//		)
//		{ // find the corresponding target in 2nd file
//			s++;
//		}
//		if (s>input1size) {
//			fprintf(stderr, "Error: target '%s' not found in SCAN file %s\n", EvalNEOContent[i][0],EvalSCANContent[s][0]);
//			exit(1);
//		}
//
//
////{"WSD_INFORMATION(0)","CUI(1)","NUM_SENSES(2)","GOLD_EMERGENCE_YEAR(3)","PREDICTED_EMERGENCE_YEAR(4)",
//// "MinYearGoldSENSEdata(5)","MaxYearGoldSENSEdata(6)","MinYearPredictedSENSEdata(7)","MaxYearPredictedSENSEdata(8)",
////"StartGoldData(9)","EndGoldData(10)","GoldYearRange(11)",
////"EvalStatus(12)","IndivError(13)","NormalisedError(14)" ,"IndivByTargets(15)","NotmalisedBytargets(16)",
////"GoldEMERGENCEstatus(17)","PredictedEMERGENCEstatus(18)", "GoldPredEMERGENCE(19)"};
//
//
//
//
//		fprintf(fp,"%s",  EvalNEOContent[i][0]);//target
//		fprintf(fp, "\t%s",EvalNEOContent[i][2]); //same sense
//		fprintf(fp, "\t%s",EvalNEOContent[i][1]); // num senses
//
//		if(strcmp(EvalNEOContent[i][17],EvalSCANContent[s][17])!=0){
//			fprintf(stderr, "ERORR: must matched\n");
//			exit(1);
//		}
//		fprintf(fp, "\t%s",EvalNEOContent[i][17]);// gold emergence year
//
//
//		fprintf(fp, "\t%s",EvalNEOContent[i][4]);// predicted emergence year
//		fprintf(fp, "\t%s", EvalSCANContent[s][4]);// predicted emergence year
//		fprintf(fp, "\t%s",EvalNEOContent[i][19]);// GoldPredEMERGENCE
//		fprintf(fp, "\t%s", EvalSCANContent[s][19]);//GoldPredEMERGENCE
//		fprintf(fp, "\n");
//		i++;
//	}
//
//	//fprintf(stderr, "Datasize = %d",i-1 );
//	fclose(fp);
//
//
//
//}



void check_emergence_status(char ***EvalNEOContent){

	for(int i=1; EvalNEOContent[i]!=NULL; i++){


		if(EvalNEOContent[i][0]!=0){











		}

	}


}





void store_in_map(map<string, char ***>*Goldemercases, int i, int j,int currentTargetSENSEnum, char ***EvalContent ){

	char ***goldpredmaptable=NULL;
	map<string, char ***>::iterator itrG= Goldemercases->find(string(EvalContent[(i-1)-j][0]));
	if(itrG!=Goldemercases->end()){
		goldpredmaptable=itrG->second;
		int length	=nullterminatedlength((void **)goldpredmaptable);
		goldpredmaptable[length-1]=EvalContent[(i-1)-j];

	}else{
		char ***goldpredmaptable=(char ***)malloc((currentTargetSENSEnum+1)*sizeof( char**));
		goldpredmaptable[0]=EvalContent[(i-1)-j];
		goldpredmaptable[currentTargetSENSEnum]=NULL;
		Goldemercases->insert({string(EvalContent[(i-1)-j][0]),goldpredmaptable});
	}
}







void goldemergencesetbyTarget(char ***EvalContent,int size,map<string, char ***>*Goldemergence,map<string, char ***>*GoldNOemergence, int colnumcondition){

	puts("IM CALLING goldSenseemergenceset\n");

	char *currentTarget = NULL;
	int currentHasEmergence = 0;
	int currentTargetSENSEnum = 0;
	//char ***goldpredmaptable=NULL;
	int i=0;
	for(i=1; EvalContent[i]!=NULL; i++){

		if (currentTarget == NULL) { // first line
			currentTarget = EvalContent[i][0];
			currentTargetSENSEnum = atoi(EvalContent[i][2]);
			currentHasEmergence = 0;
		}


		if (strcmp(currentTarget,EvalContent[i][0])==0) {

			if(atoi(EvalContent[i][colnumcondition])!=0){ // does this sense at this line of the target has an emergence
				currentHasEmergence=1;
			}
		}
		else{// new target
			// 1) record the result for the target just finished
			if (currentHasEmergence) {

		//		fprintf(stderr, "DEBUG: Target with emergence: %s \n",EvalContent[i-1][0]);

				for(int j =0; j<currentTargetSENSEnum; j++){
					store_in_map(Goldemergence, i, j,currentTargetSENSEnum,EvalContent);
				}

			} else {
	//			fprintf(stderr, "DEBUG: Target with NO emergence: %s \n",EvalContent[i-1][0]);
				for(int j =0; j<currentTargetSENSEnum; j++){
					store_in_map(GoldNOemergence, i, j,currentTargetSENSEnum,EvalContent);
				}
				// add to no emer map
			}			// reinitialize for current target
			currentTarget = EvalContent[i][0];
			currentHasEmergence = 0;
			if(atoi(EvalContent[i][colnumcondition])!=0){ // does this sense at this line of the target has an emergence
				currentHasEmergence=1;
			}
		}
	}


	if(EvalContent[i]==NULL){
		if (currentHasEmergence) {
		//	fprintf(stderr, "DEBUG: Target with emergence: %s \n",EvalNEOContent[i-1][0]);
			for(int j =0; j<currentTargetSENSEnum; j++){
				store_in_map(Goldemergence, i, j,currentTargetSENSEnum,EvalContent);
			}
		}else{
			//fprintf(stderr, "DEBUG: Target with NO emergence: %s \n",EvalNEOContent[i-1][0]);
			for(int j =0; j<currentTargetSENSEnum; j++){
				store_in_map(GoldNOemergence, i, j,currentTargetSENSEnum,EvalContent);
			}
		}
	}
}







////{"WSD_INFORMATION(0)","CUI(1)","NUM_SENSES(2)","GOLD_EMERGENCE_YEAR(3)","PREDICTED_EMERGENCE_YEAR(4)",
//// "MinYearGoldSENSEdata(5)","MaxYearGoldSENSEdata(6)","MinYearPredictedSENSEdata(7)","MaxYearPredictedSENSEdata(8)",
////"StartGoldData(9)","EndGoldData(10)","GoldYearRange(11)",
////"EvalStatus(12)","IndivError(13)","NormalisedError(14)" ,"IndivByTargets(15)","NotmalisedBytargets(16)",
////"GoldEMERGENCEstatus(17)","PredictedEMERGENCEstatus(18)", "GoldPredEMERGENCE(19)"};
//

void goldemergencesetbySense(char ***EvalContent,int size,multimap<string, char ***>*Goldemergence,multimap<string, char ***>*GoldNOemergence)
{

	puts("IM CALLING goldemergenceset\n");

	int Senseemer=0;
	int Sensenotemer=0;


	char ***goldpredmaptable=NULL;

	for(int i=1; EvalContent[i]!=NULL; i++){


		if(atoi(EvalContent[i][3])!=0){

		//	multimap<string, char ***>::iterator itrG= Goldemergence->find(string(EvalContent[i][1]));

			//fprintf(stderr, "year %d target %s line %d sense %s\n",atoi(EvalNEOContent[i][3]),EvalNEOContent[i][0], i, EvalNEOContent[i][2] );

//			if(itrG!=Goldemergence->end()){
//				goldpredmaptable=itrG->second;
//				int length	=nullterminatedlength((void **)goldpredmaptable);
//
//				//	fprintf(stderr, "length %d\n", length);
//				goldpredmaptable[length-1]=EvalContent[i];
//				//itrG->second=goldpredmaptable;
//
//			}else{
				char ***goldpredmaptable=(char ***)malloc(1*sizeof( char**));
				goldpredmaptable[0]=EvalContent[i];
				goldpredmaptable[1]=NULL;
				Goldemergence->insert({string(EvalContent[i][1]),goldpredmaptable});

			//}
			Senseemer++;
		}else{

		//	multimap<string, char ***>::iterator itrNG= GoldNOemergence->find(string(EvalContent[i][0]));
			//fprintf(stderr, "year %d target %s line %d sense %s\n",atoi(EvalNEOContent[i][3]),EvalNEOContent[i][0], i, EvalNEOContent[i][2] );


//			if(itrNG!=GoldNOemergence->end()){
//				goldpredmaptable=itrNG->second;
//				int length	=nullterminatedlength((void **)goldpredmaptable);
//				goldpredmaptable[length-1]=EvalContent[i];
//				// itrNG->second=goldpredmaptable;
//
//			}else{
				char ***goldpredmaptable=(char ***)malloc((1)*sizeof(char**));
				goldpredmaptable[0]=EvalContent[i];
				goldpredmaptable[1]=NULL;
				GoldNOemergence->insert({string(EvalContent[i][1]),goldpredmaptable});


			//}
			Sensenotemer++;
		}

	}

	fprintf(stderr, "Senseemer: %d Sensenotemer: %d  \n", Senseemer, Sensenotemer);
}



void percentageemergenceset(char ***EvalNEOContent,char ***EvalSCANContent){

	//	WSD_INFORMATION(0)","CUI(1)","NUM_SENSES(2)","GOLD_EMERGENCE_YEAR(3)","PREDICTED_EMERGENCE_YEAR(4)",
	//// "MinYearGoldSENSEdata(5)","MaxYearGoldSENSEdata(6)","MinYearPredictedSENSEdata(7)","MaxYearPredictedSENSEdata(8)",
	////"StartGoldData(9)","EndGoldData(10)","GoldYearRange(11)",
	////"EvalStatus(12)","IndivError(13)","NormalisedError(14)" ,"IndivByTargets(15)","NotmalisedBytargets(16)",
	////"GoldEMERGENCEstatus(17)","PredictedEMERGENCEstatus(18)", "GoldPredEMERGENCE(19)"};
	//

	for(int i=1; EvalNEOContent[i]!=NULL; i++){

		for(int j=0; EvalNEOContent[i][j]!=NULL; j++){




			if(EvalNEOContent[i][3]!=0){


				//	fprintf(stderr,)


			}
		}
	}
}




void compare_Emergence_tiddyTable(char ***EvalNEOContent, char ***EvalSCANContent, int input1size, char *resultsPath){


	const char *filename="NEO_SCAN_EMERGENCE";
	int len=strlen(resultsPath);
	int len2 =strlen(filename);
	char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
	strcpy(dirnamepath,resultsPath);
	strcat(dirnamepath,"/");
	strcat(dirnamepath, filename);
	strcat(dirnamepath,".tsv");
	FILE *fp;
	fp = fopen(dirnamepath,"w");
	if(fp==NULL){
		//fprintf(stderr, "Path is %s", resultsPath);
		fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
	}

	//char const *colonames_allemerEval[]= {"WSD_INFORMATION(0)","CUI(1)","NUM_SENSES(2)","GOLD_EMERGENCE_YEAR(3)","PREDICTED_EMERGENCE_YEAR(4)","MinYearGoldSENSEdata(5)","MaxYearGoldSENSEdata(6)","MinYearPredictedSENSEdata(7)","MaxYearPredictedSENSEdata(8)","StartGoldData(9)","EndGoldData(10)","GoldYearRange(11)","EvalStatus(12)","IndivError(13)","NormalisedError(14)" ,"IndivByTargets(15)","NotmalisedBytargets(16)","GoldEMERGENCEstatus(17)","PredictedEMERGENCEstatus(18)", "GoldPredEMERGENCE(19)"};

	fprintf(fp, "WSD_INFORMATION\tCUI\tNUM_SENSES\tGoldEMERG(NEO)\tGoldEMERG(SCAN)\tPredictedEMERG(NEO)\tPredictedEMERG(SCAN)\tGoldPredEMERG(NEO)\tGoldPredEMERG(SCAN)\n");

	int i=1;
	while(i<=input1size && EvalNEOContent[i]!=NULL && EvalSCANContent[i]!=NULL ){
		int s=1;
		fprintf(stderr, "searching target=%s, sense=%s\n", EvalNEOContent[i][0], EvalNEOContent[i][1]);
		while (
				(s<=input1size) && (
						(strcmp(EvalNEOContent[i][0],EvalSCANContent[s][0]) != 0)
						|| (strcmp(EvalNEOContent[i][1],EvalSCANContent[s][1])!= 0)
				)
		)
		{ // find the corresponding target in 2nd file
			s++;
		}
		if (s>input1size) {
			fprintf(stderr, "Error: target '%s' not found in SCAN file %s\n", EvalNEOContent[i][0],EvalSCANContent[s][0]);
			exit(1);
		}
		fprintf(fp,"%s",EvalNEOContent[i][0]);
		fprintf(fp, "\t%s",EvalNEOContent[i][1]); //same sense
		fprintf(fp, "\t%s",EvalNEOContent[i][2]); // num senses
		fprintf(fp, "\t%s",EvalNEOContent[i][17]);// gold emergence status
		fprintf(fp, "\t%s", EvalSCANContent[s][17]);// gold emergence status
		fprintf(fp, "\t%s",EvalNEOContent[i][18]);// predicted emergence status
		fprintf(fp, "\t%s", EvalSCANContent[s][18]);// predicted emergence status
		fprintf(fp, "\t%s",EvalNEOContent[i][19]);// both
		fprintf(fp, "\t%s", EvalSCANContent[s][19]);//both
		fprintf(fp, "\n");
		i++;
	}

	//fprintf(stderr, "Datasize = %d",i-1 );
	fclose(fp);
}




void compare_Emergence(char ***EvalNEOContent, char ***EvalSCANContent, int input1size, char *resultsPath){


	const char *filename="NEO_SCAN_EMERGENCE";
	int len=strlen(resultsPath);
	int len2 =strlen(filename);
	char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
	strcpy(dirnamepath,resultsPath);
	strcat(dirnamepath,"/");
	strcat(dirnamepath, filename);
	strcat(dirnamepath,".tsv");
	FILE *fp;
	fp = fopen(dirnamepath,"w");
	if(fp==NULL){
		//fprintf(stderr, "Path is %s", resultsPath);
		fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
	}

	//char const *colonames_allemerEval[]= {"WSD_INFORMATION(0)","CUI(1)","NUM_SENSES(2)","GOLD_EMERGENCE_YEAR(3)","PREDICTED_EMERGENCE_YEAR(4)","MinYearGoldSENSEdata(5)","MaxYearGoldSENSEdata(6)","MinYearPredictedSENSEdata(7)","MaxYearPredictedSENSEdata(8)","StartGoldData(9)","EndGoldData(10)","GoldYearRange(11)","EvalStatus(12)","IndivError(13)","NormalisedError(14)" ,"IndivByTargets(15)","NotmalisedBytargets(16)","GoldEMERGENCEstatus(17)","PredictedEMERGENCEstatus(18)", "GoldPredEMERGENCE(19)"};

	fprintf(fp, "WSD_INFORMATION\tCUI\tNUM_SENSES\tGoldEMERG(NEO|SCAN)\tPredictedEMERG(NEO|SCAN)\tGoldPredEMERG(NEO|SCAN)\n");

	//for(int i=1; i<input1size; i++){

	//	int currentScantargetIndex=1;
	int i=1;

	while(i<input1size){
		int s=1;
		fprintf(stderr, "searching target=%s, sense=%s\n", EvalNEOContent[i][0], EvalNEOContent[i][1]);
		while ((s<input1size) && (
				(strcmp(EvalSCANContent[s][0],EvalNEOContent[i][0]) != 0)
				|| (strcmp(EvalSCANContent[s][1], EvalNEOContent[i][1])!= 0)
		)
		){ // find the corresponding target in 2nd file

			s++;
		}
		if (s>=input1size) {
			fprintf(stderr, "Error: target '%s' not found in SCAN file\n", EvalNEOContent[i][0]);
			exit(1);
		}

		fprintf(fp,"%s",EvalNEOContent[i][0]);
		fprintf(fp, "\t%s",EvalNEOContent[i][1]); //same sense
		fprintf(fp, "\t%s",EvalNEOContent[i][2]); // num senses
		fprintf(fp, "\t%s",EvalNEOContent[i][17]);// gold emergence status
		fprintf(fp, "|%s", EvalSCANContent[s][17]);// gold emergence status
		fprintf(fp, "\t%s",EvalNEOContent[i][18]);// predicted emergence status
		fprintf(fp, "|%s", EvalSCANContent[s][18]);// predicted emergence status
		fprintf(fp, "\t%s",EvalNEOContent[i][19]);// both
		fprintf(fp, "|%s", EvalSCANContent[s][19]);//both

		fprintf(fp, "\n");

		i++;
	}
	fclose(fp);
}


void MAE_format(char ***EvalContent , char *resultsPath){


	const char *filename="EMERGENCE_ALLMAE";
	int len=strlen(resultsPath);
	int len2 =strlen(filename);
	char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
	strcpy(dirnamepath,resultsPath);
	strcat(dirnamepath,"/");
	strcat(dirnamepath, filename);
	strcat(dirnamepath,".tsv");
	FILE *fp;
	fp = fopen(dirnamepath,"w");
	if(fp==NULL){
		fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
	}
	// the idea is to relay on the targets
	fprintf(fp, "target\tCUI\tMAE\n");
	char *currentTarget = NULL;
	char *currentTargetMAE = NULL;
	for(int i=1; EvalContent[i]!=NULL; i++){

		if (currentTarget == NULL) { // first line
			currentTarget = EvalContent[i][0];
			currentTargetMAE = EvalContent[i][16];
		}
		if (strcmp(currentTarget,EvalContent[i][0])!=0) {  // starting new target
			fprintf(fp, "%s",currentTarget);
			fprintf(fp, "\tALL");
			fprintf(fp, "\t%s", currentTargetMAE);
			fprintf(fp, "\n");
			currentTarget = EvalContent[i][0];
			currentTargetMAE = EvalContent[i][16];
		}
		fprintf(fp, "%s",EvalContent[i][0]);
		fprintf(fp, "\t%s",EvalContent[i][1]);
		//NormalisedError(14)" ,"NotmalisedBytargets(16
		fprintf(fp, "\t%s",EvalContent[i][14]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "%s",currentTarget);
	fprintf(fp, "\tALL");
	fprintf(fp, "\t%s",currentTargetMAE);
	fprintf(fp, "\n");

	fclose(fp);
}








void print_performance(double* performance,double Globalmeanabsoluteerror,double GlobalnormalisedmeanabsoluteError,char *resDir){

	const char *filename="Performance_Emergence";
	int len=strlen(resDir);
	int len2 =strlen(filename);
	char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
	strcpy(dirnamepath,resDir);
	strcat(dirnamepath,"/");
	strcat(dirnamepath, filename);
	strcat(dirnamepath,".tsv");
	FILE *fp;
	fp = fopen(dirnamepath,"w");
	if(fp==NULL){
		fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
	}

	fprintf(fp,"%s\t%s\t%s\t%s\t%s\n", "Emergence_Precision", "Emergence_Recall", "Emergence_F1Score", "Global_Mean_absolute_Error","Normalised_Global_Mean_absolute_Error");
	fprintf(fp, "%f",performance[0]);
	for(int i=1; i<3; i++){
		fprintf(fp, "\t%f",performance[i]);
	}
	fprintf(fp, "\t%f",Globalmeanabsoluteerror);
	fprintf(fp, "\t%f",GlobalnormalisedmeanabsoluteError);
	fprintf(fp,"\n");

	fclose(fp);
}


double normalised_meanabsoluteError(char *** goldpredemergencetable, int NormalisedErrorIndex ){

	double norabsoluteerror=0;
	double normeanabsoluteerror=0.0;

	for(int i=0; goldpredemergencetable[i]!=NULL; i++){

		norabsoluteerror+=atof(goldpredemergencetable[i][NormalisedErrorIndex]);
	}
	return (double)norabsoluteerror/(double)(numberstored-1);
	//return normeanabsoluteerror;
}




double sum_meanabsoluteError(char ***goldpredemergencetable, int IndivErrorIndex){


	puts("IM calling sum mean absolute \n");
	double absoluteerror=0;
	double meanabsoluteerror=0.0;

	for(int i=0; goldpredemergencetable[i]!=NULL; i++){
		//fprintf(stderr, "number is %d\n", atoi(goldpredemergencetable[i][IndivErrorIndex]));
		absoluteerror+=atof(goldpredemergencetable[i][IndivErrorIndex]);
	}
	//	fprintf(stderr, "numbered stored HERE %d\n", numberstored);

	return (double)absoluteerror/(double)(numberstored-1);

	//return meanabsoluteerror;
}



void print_binaryEmergenceMatrix(unsigned int **emergencetable,char *resDir){

	const char *filename="binaryEmergenceMatrix";
	int len=strlen(resDir);
	int len2 =strlen(filename);
	char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
	strcpy(dirnamepath,resDir);
	strcat(dirnamepath,"/");
	strcat(dirnamepath, filename);
	strcat(dirnamepath,".tsv");
	FILE *fp;
	fp = fopen(dirnamepath,"w");
	if(fp==NULL){
		fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
	}


	fprintf(fp,"\t%s\t%s\n", "EMERGENCE", "NO_EMERGENCE");// gold class(columns)
	for(int i =0; i<2; i++){

		if(i==0){
			fprintf(fp,"%s", "EMERGENCE");

		}else{
			fprintf(fp,"%s", "NO_EMERGENCE");
		}
		for(int j=0; j<2; j++){
			fprintf(stderr, "i=%d,j=%d -> emergencetable[i][j] = %d\n", i,j,emergencetable[i][j]);
			fprintf(fp,"\t%d", emergencetable[i][j]);
		}

		fprintf(fp,"\n");
	}
	fclose(fp);
}






double* calculate_performance(unsigned int **emergencebinarymatrix)
{
	int	TP=0;
	int FP=0;
	int	FN=0;
	int	TN=0;
	double percision= 0.0;
	double recall= 0.0;
	double f1score= 0.0;


	for(int i=0; i<2; i++){

		for(int j=0; j<2; j++){


			if(i==0 && j==0){
				TP=emergencebinarymatrix[i][j];
			}


			if(i==0 && j ==1){
				FP=emergencebinarymatrix[i][j];
			}


			if(i==1 && j==0){
				FN=emergencebinarymatrix[i][j];
			}


			if(i==1 && j==1){
				TN=emergencebinarymatrix[i][j];
			}


		}

	}


	percision=(double)TP/double(TP+FP);
	recall=(double)TP/double(TP+FN);
	f1score=2*((double)percision*recall)/double(percision+recall);

	//fprintf(stderr, "%lf\t%lf\t%lf",percision, recall, f1score);

	double *performance=(double*)malloc(3*sizeof(double));


	performance[0]=percision;
	performance[1]=recall;
	performance[2]=f1score;



	fprintf(stderr, "performance[0]=%lf\tperformance[1]=%lf\tperformance[2]=%lf", performance[0],performance[1],performance[2]);
	return performance;

}





unsigned int** counts_performance(char ***goldpredemergencetable, int EvalStatusIndex)
{


	assert(goldpredemergencetable);
	assert(goldpredemergencetable[0]);

	unsigned int **emergencetable=(unsigned int  **)malloc(2*sizeof(unsigned int  *));

	for(int i=0; i<2; i++){
		emergencetable[i]=(unsigned int *)calloc(2,sizeof(unsigned int ));

	}


	for(int i=1; goldpredemergencetable[i]!=NULL; i++){

		//	fprintf(stderr, "DEBUG: Start DEBUG %s\n",goldpredemergencetable[i][1]);
		if(strcmp(goldpredemergencetable[i][EvalStatusIndex], "TP")==0){

			emergencetable[0][0]++;

				fprintf(stderr, "DEBUG: Print TP emergencetable[0][0]=%d &&(goldpredemergencetable[%d][10]= %s)\n",emergencetable[0][0],i,goldpredemergencetable[i][10]);

		}

		else if (strcmp(goldpredemergencetable[i][EvalStatusIndex], "FP")==0){

			emergencetable[0][1]++;
				fprintf(stderr, "DEBUG: Print FP emergencetable[0][1]=%d && (goldpredemergencetable[%d][10]=%s)\n",emergencetable[0][1],i,goldpredemergencetable[i][10]);
		}

		else if(strcmp(goldpredemergencetable[i][EvalStatusIndex], "FN")==0){

			emergencetable[1][0]++;

					fprintf(stderr, "DEBUG: Print FN emergencetable[1][0]=%d && (goldpredemergencetable[%d][10]=%s)\n",emergencetable[1][0],i,goldpredemergencetable[i][10]);


		}else if(strcmp(goldpredemergencetable[i][EvalStatusIndex], "TN")==0){

			emergencetable[1][1]++;

				fprintf(stderr, "DEBUG: Print TN emergencetable[1][1]=%d && (goldpredemergencetable[%d][10]=%s)\n",emergencetable[1][1],i,goldpredemergencetable[i][10]);

		}else{

			fprintf(stderr, "ERROR: NO MATCHED VALUE FOR THE BINARY EMERGENCE TABLE\n");
			exit(1);
		}


		//	fprintf(stderr, "DEBUG %s\n",goldpredemergencetable[i][1]);

	}


	return emergencetable;
}





void print_goldpredtable(char ***goldpredemergencetable,char *resDir){


	puts("Im calling print_goldpredtable\n");


	const char *filename="goldpredtable";
	int len=strlen(resDir);
	int len2 =strlen(filename);
	char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
	strcpy(dirnamepath,resDir);
	strcat(dirnamepath,"/");
	strcat(dirnamepath, filename);
	strcat(dirnamepath,".tsv");
	FILE *fp;
	fp = fopen(dirnamepath,"w");
	if(fp==NULL){
		fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
	}
	//fprintf(stderr, "the name of the  file %s\n", dirnamepath);

	//assert(goldpredemergencetable);

	//assert(goldpredemergencetable[0]);
	for(int i=0; goldpredemergencetable[i]!=NULL; i++){
		//assert(goldpredemergencetable[i][0]);

		fprintf(fp,"%s", goldpredemergencetable[i][0]);

		for(int j=1; j<20; j++){
			assert(goldpredemergencetable[i][j]);

			//	fprintf(stderr, "%d, %d\n", i,j);


			//if(goldpredemergencetable[i][j]!=NULL){

			fprintf(fp,"\t%s", goldpredemergencetable[i][j]);
			//	}
		}

		fprintf(fp, "\n");

	}


	fclose(fp);
}


const char *get_eval_status(char *GoldEmerYear,char *PredEmerYear,unsigned int windowsizeemergenceeval)
{

	int goldyear=atoi(GoldEmerYear);
	int predyear=atoi(PredEmerYear);

	if(goldyear==0){

		if(predyear==0){

			return "TN";

		}else{
			return "FP";
		}

	}else{

		if(predyear==0){

			return "FN";

		}else{

			if( abs(goldyear-predyear)<=windowsizeemergenceeval){


				fprintf(stderr, "abs(%d- %d)=%d\n", goldyear,predyear,abs(goldyear-predyear));


				return "TP";

			}else{

				return "FP";

				//return "TP";

			}
		}
	}
}
/*
 * General Error Case (both has emergence):
 * Return how far from the gold emergence year is the predicted one
 *
 * Specific Error Case (one has emergence but not both)
 * Return the max error possible that the range of the data (endyear-startyear) can allow.
 *
 * Specific Error Case (both of them has no emergence)
 * Return 0 which means no error
 *
 */

unsigned int get_error(char *GoldEmerYear,char *PredEmerYear, unsigned int maxError)
{

	int goldyear=atoi(GoldEmerYear);
	int predyear=atoi(PredEmerYear);

	if(goldyear==0){


		if(predyear==0){

			return 0;

		}else{

			return maxError;
		}

	}else{

		if(predyear==0){

			return maxError;

		}else{

			return abs(goldyear-predyear);
		}
	}

}





/*
 * we will follow the predicted cui order as its order taken by the match method.
 */


int find_RowGoldCUIIndex(char * PredCUI,char ***GoldEmerContent){

	for(int i=1; GoldEmerContent[i]!=NULL; i++ ){
		if(strcmp(GoldEmerContent[i][2],PredCUI)==0){
			return i;
		}
	}
	fprintf(stderr, "the Gold CUI is not found for the predicted CUI%s \n", PredCUI);
	exit(1);

}




char *** create_goldpredtable(map<string, char ***>*GoldEmergenceContent, map<string, char ***>*PredEmergenceContent, unsigned int windowsizeemergenceeval)
{

	//"0WSD_INFORMATION\t1CUI\t2NUM_SENSES\t3GOLD_EMERGENCE_YEAR\t4PREDICTED_EMERGENCE_YEAR\t5MinYearGoldSENSEdata\t6MaxYearSENSEdata\tStartData\t7EndData\tEvalStatus\t8IndivError";


	map<string,char ***>::iterator itrmapGold;
	map<string,char ***>::iterator itrmapPred;

	char ***goldpredtable=(char ***)malloc(block_size*sizeof(char**));

	goldpredtable[0]=(char**)colonames_allemerEval;

	//	for(int i=0; i<12; i++){
	//		fprintf(stderr, "(%d)%s\n", i,goldpredtable[0][i]);
	//	}


	numberstored=1;
	//int numberallocated=block_size;

	for(map<string,char ***>::iterator itrmapGold=GoldEmergenceContent->begin();  itrmapGold!=GoldEmergenceContent->end(); itrmapGold++)
	{


		string targetGold=itrmapGold->first;
		char ***GoldEmerContent=itrmapGold->second;

		//fprintf(stderr, "%s\n",targetGold.c_str());

		itrmapPred=PredEmergenceContent->find(targetGold);
		if(itrmapPred!=PredEmergenceContent->end()){


			string targetPred=itrmapPred->first;
			char ***PredEmerContent=itrmapPred->second;

			assert(PredEmerContent[1]);
			for(int i=1; i<=atoi(PredEmerContent[1][1]); i++ ){

				int GoldIndex=find_RowGoldCUIIndex(PredEmerContent[i][2],GoldEmerContent);


				if(strcmp(GoldEmerContent[GoldIndex][2],PredEmerContent[i][2])!=0){
					fprintf(stderr, "the predicted sense %s does not match the gold sense %s \n",PredEmerContent[i][2],GoldEmerContent[GoldIndex][2]);
					exit(1);
				}


				if((numberstored+1)%block_size==0){

					goldpredtable=(char ***)realloc(goldpredtable,((numberstored+1)+block_size)*sizeof(char **));
				}


				//	fprintf(stderr, "\ngold index %d, PredCUI %s \n",GoldIndex, PredEmerContent[i][2]);


				goldpredtable[numberstored] =(char**)malloc(20*sizeof(char *));

				goldpredtable[numberstored][0]=strdup(targetPred.c_str()); // target

				fprintf(stderr, "DEBUG: goldpredtable[%d][%d] %s\n",numberstored, 0, goldpredtable[numberstored][0]);

				goldpredtable[numberstored][1]=PredEmerContent[i][2];// predcui (following the matching order)

				fprintf(stderr, "DEBUG: goldpredtable[%d][%d] %s\n",numberstored, 1, PredEmerContent[i][2]);
				//????
				goldpredtable[numberstored][2]=GoldEmerContent[GoldIndex][1];// num sense


				//	fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 2, GoldEmerContent[i][1]);




				goldpredtable[numberstored][3]=GoldEmerContent[GoldIndex][3];// gold emergence year
				//	fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 3, GoldEmerContent[i][3]);
				//	goldpredtable[numberstored][3]=GoldEmerContent[i][3];// gold emergence year

				//		fprintf(stderr, "DEBUG: goldpredtable[%d][%d] %s| Target %s| goldSENSE  %s ?==? predSENSE %s \n",GoldIndex, 2, GoldEmerContent[GoldIndex][3], targetPred.c_str(),GoldEmerContent[GoldIndex][2],PredEmerContent[i][2] );


				goldpredtable[numberstored][4]=PredEmerContent[i][3];// predicted emergence year
				//	fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 4, PredEmerContent[i][3]);

				goldpredtable[numberstored][5]=GoldEmerContent[GoldIndex][6];// min year sense data
				//	fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 5, GoldEmerContent[i][6]);

				goldpredtable[numberstored][6]=GoldEmerContent[GoldIndex][7];// max year sense data
				//	fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 6, GoldEmerContent[i][7]);


				goldpredtable[numberstored][7]=PredEmerContent[i][6];
				goldpredtable[numberstored][8]=PredEmerContent[i][6];

				// check every 7 and 8 to 9 and 10

				goldpredtable[numberstored][9]=GoldEmerContent[GoldIndex][8];// data start
				//	fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 7, GoldEmerContent[i][8]);


				goldpredtable[numberstored][10]=GoldEmerContent[GoldIndex][9];// data end
				//	fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 8, GoldEmerContent[i][9]);



				char buffer[10];

				unsigned int rangeYears = atoi(GoldEmerContent[GoldIndex][9])-atoi(GoldEmerContent[GoldIndex][8]);
				sprintf(buffer,"%d",rangeYears);
				goldpredtable[numberstored][11]=strdup(buffer);

				//	fprintf(stderr, "goldpredtable[%d][%d] %d\n",numberstored, 9, atoi(GoldEmerContent[i][9])-atoi(GoldEmerContent[i][8]));


				const char * status= get_eval_status(goldpredtable[numberstored][3],goldpredtable[numberstored][4],windowsizeemergenceeval);

				//???
				goldpredtable[numberstored][12]=(char *)status;

				//	fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 10,(char *)status);


				unsigned int errVal = get_error(goldpredtable[numberstored][3],goldpredtable[numberstored][4], rangeYears);
				sprintf(buffer,"%d",errVal);
				goldpredtable[numberstored][13]=strdup(buffer);


				char dbuffer[20];
				double	normError=(double)errVal/(double)rangeYears;
				sprintf(dbuffer,"%lf",normError);

				goldpredtable[numberstored][14]=strdup(dbuffer);

				// to calculate mean error by tagets

				if(i==atoi(PredEmerContent[1][1])){
					int errorTarget=0.0;
					for(int l=0; l<atoi(PredEmerContent[1][1]); l++){
						//	fprintf(stderr, "goldpredtable[ %d - %d ][13]=%d\n",numberstored,l, atoi(goldpredtable[numberstored-l][13]));
						errorTarget	+=atoi(goldpredtable[numberstored-l][13]);
					}
					//	fprintf(stderr, "errorTarget= %d\n", errorTarget);
					char ddbuffer[20];
					double Errt=(double)errorTarget/(double)(atoi(PredEmerContent[1][1]));
					sprintf(ddbuffer,"%lf",Errt);
					goldpredtable[numberstored-i+1][15]= strdup(ddbuffer);

					const char *x= "";
					for(int b=0; b<i-1; b++){
						goldpredtable[numberstored-b][15]= (char *)(x);
					}
				}


				// to calculate the normalized mean error by targets

				if(i==atoi(PredEmerContent[1][1])){
					double norerrorTarget=0.0;
					for(int l=0; l<atoi(PredEmerContent[1][1]); l++){
						//	fprintf(stderr, "goldpredtable[ %d - %d ][14]=%d\n",numberstored,l, atoi(goldpredtable[numberstored-l][14]));
						norerrorTarget	+=atof(goldpredtable[numberstored-l][14]);
					}
					//			fprintf(stderr, "norerrorTarget= %d\n", norerrorTarget);
					char nddbuffer[20];
					double nErrt=(double)norerrorTarget/(double)(atoi(PredEmerContent[1][1]));
					sprintf(nddbuffer,"%lf",nErrt);
					goldpredtable[numberstored-i+1][16]= strdup(nddbuffer);
					const char *x= "-";
					for(int b=0; b<i-1; b++){
						goldpredtable[numberstored-b][16]= (char *)(x);
					}
				}




				if(atoi(goldpredtable[numberstored][3])!=0){
					goldpredtable[numberstored][17]= (char *)"Gold_EMERGENCE";
				}else{
					goldpredtable[numberstored][17]=(char *)"-";
				}

				if(atoi(goldpredtable[numberstored][4])!=0){
					goldpredtable[numberstored][18]= (char *)"Predicted_EMERGENCE";
				}else{
					goldpredtable[numberstored][18]= (char*)"-";
				}





				if(strcmp(status,"TP")==0){

					const char * emergStatus= (char *)"MATCHED_Emergence";
					goldpredtable[numberstored][19]= (char *)(emergStatus);
					//		fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 12, goldpredtable[numberstored][12]);
				}else{

					goldpredtable[numberstored][19]= (char *)"-";

				}



				//fprintf(stderr, "goldpredtable[%d][%d] %s\n",numberstored, 11,goldpredtable[numberstored][11]);


				numberstored++;
				//if numberstored == allocated or the % option
				fprintf(stderr, "the number stored is %d\n", numberstored);
				goldpredtable[numberstored]=NULL;

			}
		}else{

			fprintf(stderr, "the Gold target %s must exist in the Predicted Target\n",targetGold.c_str());
			exit(1);
		}

	}


	//	goldpredtable=(char ***)realloc(goldpredtable,numberstored*sizeof(char **));
	//	goldpredtable[numberstored]=NULL;

	return goldpredtable;
}











unsigned int store_WSIDATA_EVAL(char const *filename, char ****targetContent)
{

	char buff[100000];

	//fprintf(stderr, "the file %s\n",filename);

	FILE *fptr =NULL;
	fptr = fopen(filename,"r");

	if(fptr ==NULL){
		fprintf(stderr, "Error opening the file %s\n",filename);
		exit(1);
	}

	int j=0;
	while(fgets(buff, 100000, fptr)!=NULL){

		//fprintf(stderr, "DEBUG: buff %s \n",buff);

		if(((j+1)%block_size)==0){
			(*targetContent)= (char ***)realloc((*targetContent),(j+1+block_size)*sizeof(char **));
			//(*targetContent)[j+block_size]=NULL;WRONG CUZ SOMETIMES I DONT FILL THE WHOLE 10 BLOCKS
		}

		//puts("im here \n");

		buff[strcspn(buff, "\r\n")] = '\0';
		char **WSItokns = tokenise_opt2(buff,"\t",1);


		//				for(int i=0; WSItokns[i]!=NULL; i++){
		//					printf("the %d string from the WSI_GOLD is %s\n",i, WSItokns[i]);
		//				}


		(*targetContent)[j]= WSItokns;
		j++;
		(*targetContent)[j]=NULL;

	}

	j=j-1;
	fprintf(stderr, "the size of data %d\n", j);

	//	for(int p=0; p<j+1; p++){
	//		print_stringarray((*targetContent)[p]);
	//	}

	return j;
}







map<string, char ***>* read_emergence( char *emerPath)
																																																																										{
	puts("Im Calling read_emergence()\n");

	map<string, char ***>*GoldPredEmergenceContent=new  map<string,char ***>();

	map<string,char ***>::iterator itrmap;

	const char **Dircontent = read_dir_Targetscontent_EVAL(emerPath);

	if(Dircontent[0]==NULL){

		fprintf(stderr, "ERORR: No data in the subdirectories of %s\n", emerPath);
		exit(1);

	}



	for(int f=0; Dircontent[f]!=NULL; f++){

		fprintf(stderr, "Target number %d\n",f );

		//	fprintf(stderr, "Dircontent[f]%s\n",Dircontent[f] );




		char ***TContent= (char ***)malloc(block_size*sizeof(char **));
		TContent[block_size-1]=NULL;

		store_WSIDATA_EVAL(Dircontent[f], &TContent);

		const char *posUnderscore=strrchr(Dircontent[f],'_');

		const char * poslash=strrchr(Dircontent[f],'/');// if not null

		if(poslash==NULL){

			poslash=Dircontent[f];

		}else{

			poslash++;

		}
		unsigned int lengthT= posUnderscore-poslash;
		char *TargetName=(char *)malloc((lengthT+1)*sizeof(char));
		strncpy(TargetName, poslash, lengthT);
		TargetName[lengthT]='\0';
		fprintf(stderr, "TargetName %s\n",TargetName);

		itrmap=GoldPredEmergenceContent->find(string(TargetName));

		if(itrmap==GoldPredEmergenceContent->end()){
			GoldPredEmergenceContent->insert({string(TargetName),TContent});
		}else{
			fprintf(stderr, "this target %s must be not repeated in the file ", TargetName);
			exit(1);
		}

	}

	return GoldPredEmergenceContent;
}




const char **read_dir_Targetscontent_EVAL(const char *dirname)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);

	if(d==NULL){
		fprintf(stderr,"Error! Unable to open the directory.\n");
		exit(1);
	}

	char** dircontent = (char**)malloc(sizeof(char*));
	unsigned int index= 0;


	while ((dir = readdir(d)) != NULL){
		//	printf("DEBUG: %s\n", dir->d_name);// i dont need to use rewcursion!!

		char const *filenameShort=NULL;

		const char * concatenatedName =concatenate(dir->d_name, dirname, &filenameShort);


		int checkregfile=  is_regular_file(concatenatedName);

		if( (strcmp(dir->d_name,".")!=0) && (strcmp(dir->d_name, "..") !=0) &&  checkregfile){
			//puts("DEBUG: Im after the checkregfile if\n");

			char *postsv =strchr(dir->d_name,'.');

			if((postsv != NULL) && strcmp(postsv, ".tsv")==0){

				//puts("Im after the posxml != NULL \n");

				unsigned int len = strlen(concatenatedName);

				dircontent[index]=(char*)malloc((len+1)*sizeof(char));

				strcpy(dircontent[index],concatenatedName);

				index++;

				dircontent = (char**)realloc(dircontent,(index+1)*sizeof(char*));
				//printf("DEBUG: index=%d, size=%d, added entry '%s'\n", index,index+1, dircontent[index-1]);
			}
		}
	}
	closedir(d);
	dircontent[index]=NULL;

	return (const char**) dircontent;
}

