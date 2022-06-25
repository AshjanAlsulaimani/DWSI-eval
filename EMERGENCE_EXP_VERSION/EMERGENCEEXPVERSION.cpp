/*
 * EMERGENCEEXPVERSION.cpp
 *
 *  Created on: 20 Dec 2019
 *      Author: alsulaia
 */

#include "EMERGENCEEXPVERSION.h"

//EMERGENCE_EXP_VERSION::EMERGENCE_EXP_VERSION() {
//	// TODO Auto-generated constructor stub
//
//}
//
//EMERGENCE_EXP_VERSION::~EMERGENCE_EXP_VERSION() {
//	// TODO Auto-generated destructor stub
//}
//

/*
 * EMERGENCE.cpp
 *
 *  Created on: 18 Jul 2019
 *      Author: alsulaia
 */

//#include "EMERGENCE.h"

const unsigned long block_size= 10;
const unsigned long block_size_print= 124;
//const unsigned long allocated_size;


const unsigned long num_of_directories=9;
char const *dirpathcounts= "COUNTSby_SENSES_YEARS";
char const *dirpath_total= "TOTAL_COUNTSby_SENSES";
char const *dirpathsensesGyears="SENSE_GIVEN_YEAR_PROBABILITIES";
char const *dirname_valid="FILTEREDzero_COUNTSby_SENSES_YEARS";
char const *dirname_threshold="FILTEREDthreshold_COUNTSby_SENSES_YEARS";
char const *dirname_thresholdConProbabilities="FILTEREDthreshold_SENSE_GIVEN_YEAR_PROBABILITIES";
char const *dirname_infoemergence="SENSE_INFO_and_EMERGENCEYEAR";
char const *dirname_SENSESinforemergence="SENSES_ALL_EMERGENCEINFO";
char const *dirname_Parameters_values= "Parameters_Values";
char const *dirname_SenseSINFO= "SENSES_INFO_EVALUATIONINPUT";

char const *alldirectories[]={dirpathcounts, dirpath_total,dirpathsensesGyears, dirname_valid, dirname_threshold,
		dirname_thresholdConProbabilities, dirname_infoemergence, dirname_SENSESinforemergence,  dirname_SenseSINFO};


float step_increase_threshold_wrtMax= -1; //0.02;
float min_prop_years_with_increase_wrtWindowSize= 0.0;
float min_prop_previouslowyears_wrtoutsidetheWindow= 0.80;
float max_threshold_previouslowyearswrtMax= -1;//0.1;

float min_totalincrease_surges_wrtWindowSize=0.2;//default 20%


int const numoflabels= 8;
char const *emergenceprintinglabels[]={"StartYear(counts)", "StartYearValue(counts)", "LastYear(counts)", "LastYearValue(counts)",
		"MaxSensegivenYear(SGY)", "MaxSensegivenYearValue(SGY)", "EmergenceYear(SGY)", "EmergenceValue(SGY)" };


char const *colonames_allemer= "WSD_INFORMATION\tNUM_SENSES\tSENSE\tEMERGENCE_YEAR\tEMERGENCE_VALUE\tALLSURGES\tFIRST_YEAR_OCCURENCE\tDifferenceEMER-FIRST";
char const *colonames_firstYears= "WSD_INFORMATION\tNUM_SENSES\tSENSE\tFIRST_YEAR_OCCURENCE\tVALUE_FIRST_YEAR_OCCURENCE\tMaxSenseGivenYear\tMaxSenseGivenYearValue";
char const *colonames_EmergenceParameters="ThersholdValue\tWindowSize\tStep_increase_threshold_wrtMax\tMin_total_surges_window_increase_wrtMax\tMax_threshold_previouslowyears_wrtMax\tMin_prop_previouslowyears_wrtoutsidetheWindow";
char const *colonames_BL="WSD\tWSD_BASELINE_EMERGENCE\tSense\tSenseBASELINE_EMERGENCE\tYear";

char const *colonames_SENSESINFO="WSD_INFORMATION\tNUM_SENSES\tSENSE\tEMERGENCE_YEAR\tEMERGENCE_VALUE\tALLSURGES\tMinYearforSenseData\tMaxYearforSenseData\tDataStart\tDataEnd";

int bflag=0;


//EMERGENCE::EMERGENCE() {
//	// TODO Auto-generated constructor stub
//
//}
//
//EMERGENCE::~EMERGENCE() {
//	// TODO Auto-generated destructor stub
//}


//**********************************EMERGENCEPART**********************************************//







void print_parameters_values_EXP( char const *respath,unsigned int thershold_exp, unsigned int windowsize_exp, float step_increase_threshold_Max,
		float min_total_surges_window_increase, float max_threshold_previouslowyears_Max,
		float min_prop_previouslowyears_wrtoutsidetheWindow, char const *dirname_ParametersValues)
{


	int paradir=strlen(respath);
	int dirdir=strlen(dirname_ParametersValues);
	char *fname=(char *)malloc((paradir+dirdir+4)*sizeof(char));
	strcpy(fname,respath);
	strcat(fname,"/");
	strcat(fname,dirname_ParametersValues);


	//fprintf(stderr, "%s\n",fname);

	FILE *fl= write_to_FILE_EXP(fname, colonames_EmergenceParameters, "Emergence_Parameters");

	fprintf(fl,"%d\t", thershold_exp);

	fprintf(fl,"%d\t", windowsize_exp);

	fprintf(fl,"%lf\t", step_increase_threshold_Max);

	fprintf(fl,"%lf\t", min_total_surges_window_increase);

	fprintf(fl,"%lf\t", max_threshold_previouslowyears_Max);

	fprintf(fl,"%lf\n", min_prop_previouslowyears_wrtoutsidetheWindow);

	fclose(fl);

}









/*
 * We assume each increase between a surge must constitute at least 2% of the max whatever is our window size
 * Thus the total increase between the surges must be at least 2%*windowsize.
 *
 *
 */

void set_parameters_EXP(char const *resultspath, unsigned int thershold_exp, unsigned int windowsize_exp, float step_increase_threshold_Max ,float max_threshold_previouslowyears_Max)
{
	//max_threshold_previouslowyears= 0.1;
	//step_increase_threshold_wrtMax= 0.02;

	if(-1.000 == -1){
		fprintf(stderr, "FIND: yes -1.000 == -1\n");
	}


	if(step_increase_threshold_Max == -1){// if the user didnt give any value for total step propotion within a window size

		step_increase_threshold_Max= (float) min_totalincrease_surges_wrtWindowSize / (float) windowsize_exp;

		step_increase_threshold_wrtMax= step_increase_threshold_Max;

		fprintf(stderr,"FIND: INSIDE NOT SUPOOSED TO BE -1 step_increase_threshold_wrtMax %lf\n", step_increase_threshold_wrtMax);

	}else{

		if(step_increase_threshold_wrtMax == step_increase_threshold_Max){
			min_totalincrease_surges_wrtWindowSize= (float) step_increase_threshold_Max * (float) windowsize_exp;

		}
	}

	//Not UsED
	//	float total_prop_within_windowsize_of_max=(float)step_increase_threshold_wrtMax*(float) windowsize_exp;
	//	step_increase_threshold_wrtMax= (float) total_prop_within_windowsize_of_max / (float) windowsize_exp;

	min_prop_years_with_increase_wrtWindowSize= ((float)windowsize_exp -2)/((float)windowsize_exp-1);

	//min_prop_previouslowyears_wrtoutsidetheWindow= ((float)windowsize_exp -3)/(float)windowsize_exp-1;

	if(max_threshold_previouslowyears_Max == -1){// continue on this tomorrow

		max_threshold_previouslowyears_Max= 0.1;// let the user set this set it inside the code

		max_threshold_previouslowyearswrtMax= max_threshold_previouslowyears_Max;
	}



	fprintf(stderr,"FIND: the parameters values AFTER:\n");
	fprintf(stderr,"FIND: thershold_exp %d\n", thershold_exp);
	fprintf(stderr,"FIND: windowsize_exp %d\n", windowsize_exp);
	fprintf(stderr,"FIND: step_increase_threshold_wrtMax %lf\n", step_increase_threshold_wrtMax);
	fprintf(stderr,"FIND: max_threshold_previouslowyearswrtMax %lf\n", max_threshold_previouslowyearswrtMax);
	fprintf(stderr,"FIND: min_totalincrease_surges_wrtWindowSize %lf\n", min_totalincrease_surges_wrtWindowSize);

	print_parameters_values_EXP(resultspath,thershold_exp, windowsize_exp, step_increase_threshold_Max , min_totalincrease_surges_wrtWindowSize, max_threshold_previouslowyears_Max,  min_prop_previouslowyears_wrtoutsidetheWindow,dirname_Parameters_values);

}




/*
 * To check if the increase happened between two years about 0.2 or 0.3 of the max.
 * (thats should happen through all the run of the r interval on year-to-year basis)
 * the function will return 1==increase or 0==no increase.
 * the step (the slope) must be at least higher than 0.02 of the maxiumum.
 */
unsigned int Step_EXP(WSIEMERGENCE *wsi,SENSEEMERGENCE *senseinfo,int *is_FilteredorValid_year,int sensenum,int np)
{

	//unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;
	//change the first part of the code in a function so you recieve the information about this sense only once.

	long double maxx=0.0;

	//free this
	//char *senseID=get_sensestring_from_index(wsi,sensenum);

	//map<string, SENSEEMERGENCE*>::iterator itsen;
	//itsen = wsi->senses_min_max->find(string(senseID));

	//if(itsen ==  wsi->senses_min_max->end()){
	//fprintf(stderr, "ERROR: this sense %s must exist in the map previously!\n",senseID);
	//exit(1);
	//}else{

	//SENSEEMERGENCE *senseinfo= itsen->second;
	maxx=senseinfo->s_max_sense_given_year_value;

	fprintf(stderr, "FIND: the word is %s\n", wsi->wsdname);
	//fprintf(stderr, "FIND: the sense is %s\n", senseID);

	//free(senseID);

	fprintf(stderr, "FIND: the years for the comparison is %d-%d\n", np,(np-1));
	fprintf(stderr, "FIND: the max is %Lf\n", maxx);
	fprintf(stderr, "FIND: step_increase_threshold_wrtMax is %lf\n", step_increase_threshold_wrtMax);
	//fprintf(stderr, "FIND: the step supposed to be %Lf",step_increase_threshold_wrtMax )
	fprintf(stderr, "FIND: the slope value is %Lf\n", (wsi->sense_given_year[np][sensenum]-wsi->sense_given_year[np-1][sensenum]));
	fprintf(stderr, "FIND: the step_increase_threshold_wrtMax *max %Lf of the max \n",step_increase_threshold_wrtMax *maxx);

	if((wsi->sense_given_year[np][sensenum]-wsi->sense_given_year[np-1][sensenum]) >= (step_increase_threshold_wrtMax *maxx)){

		//fprintf(stderr, "FIND: there is an increase of %d and RETURN 1",wsi->sense_given_year[np][sensenum]-wsi->sense_given_year[np-1][sensenum]);
		return 1;
	}
	//}

	return 0;
}


//void print_emergencealgorithm(WSIEMERGENCE *wsi, int n, int np, int sensenum, int countSteps){
//
//
//	long double difference=wsi->sense_given_year[np][sensenum]-wsi->sense_given_year[np-1][sensenum];
//
//
//fprintf(stderr,"The slope value is %s", difference );
//
//
//
//
//}


/*
 *
 * if h and then l then true otherwise if false
 * the return value of this function is true =1, false=0
 */
unsigned int SurgeStrat_EXP(int n, int r, WSIEMERGENCE *wsi, int *is_FilteredorValid_year, int sensenum )
{

	long double max=0.0;
	SENSEEMERGENCE *senseinfo=NULL;
	char *senseID=get_sensestring_from_index_EXP(wsi,sensenum);
	map<string, SENSEEMERGENCE*>::iterator itsen;
	itsen = wsi->senses_min_max->find(string(senseID));

	fprintf(stderr,"FIND: sense number %d and the sense is %s\n", sensenum, senseID);

	if(itsen ==  wsi->senses_min_max->end()){
		fprintf(stderr, "ERROR: this sense %s must exist in the map previously!\n",senseID);
		exit(1);
	}else{
		senseinfo= itsen->second;
		max=senseinfo->s_max_sense_given_year_value;
		fprintf(stderr,"FIND: the max for this sense %s at YEARn %d is %Lf\n", senseID, n,max);

	}

	free(senseID);
	int *surgeYears=(int *)calloc(1,sizeof(int));

	int h=0;
	int l=0;

	unsigned int countSteps=0;



	int np=0;

	fprintf(stderr,"FIND: IM HERE BEFORE STEP LOOP, the values of np %d and n %d and the condition %d\n", np, n, n+r);


	for( np=n+1; np<n+r; np++ ){ // check the neologistic pattern

		fprintf(stderr,"FIND: IM HERE BEFORE STEP\n");

		if(Step_EXP(wsi, senseinfo, is_FilteredorValid_year, sensenum, np)){
			fprintf(stderr, "FIND: the year is %d and the previous year is %d there is an increase of %Lf and COUNT %d\n", np,n,
					wsi->sense_given_year[np][sensenum]-wsi->sense_given_year[np-1][sensenum],countSteps);
			countSteps++;
		}
	}

	long double propyearsincrease =(((long double)countSteps)/((long double)(r-1)));// within the windowsize

	fprintf(stderr, "FIND: the proportion of the years increasing to total year is %Lf\n",propyearsincrease);
	fprintf(stderr, "FIND: min_prop_years_with_increase_wrtWindowSize) is %lf\n",min_prop_years_with_increase_wrtWindowSize);

	if(propyearsincrease >= min_prop_years_with_increase_wrtWindowSize){
		fprintf(stderr, "FIND: the value of high test h %d", h);
		h=1;
	}

	// print_emergencealgorithm(wsi, n, np, sensenum, countSteps);


	if(n!=0){

		unsigned int countlow=0;// for checking the pattern before the neologistic pattern happen
		for(int npre=n-1; npre>=0; npre--){

			fprintf(stderr,"FIND: wsi->sense_given_year[npre][sensenum]%Lf\n",wsi->sense_given_year[npre][sensenum]);

			double maxthreshold_low_wrtMax=  max_threshold_previouslowyearswrtMax;

			if(!bflag){
				maxthreshold_low_wrtMax= maxthreshold_low_wrtMax*max;

			}

			if(wsi->sense_given_year[npre][sensenum]<= maxthreshold_low_wrtMax){
				countlow++;

				fprintf(stderr, "FIND: countlow %d\n", countlow);
			}
		}


		long double propyearsdecrease =(((long double)countlow)/((long double)(n)));
		fprintf(stderr, "FIND: countlow propyearsdecrease %Lf\n", propyearsdecrease);
		if(propyearsdecrease > min_prop_previouslowyears_wrtoutsidetheWindow){

			l=1;
			fprintf(stderr, "FIND: the year number is %d the value of the low test l is %d", n,l);
		}

	}else{

		fprintf(stderr, "FIND: the year number is %d the value of the low test l is %d", n,l);
		l=1;
	}


	fprintf(stderr, "FIND: the high low test value together is %d\n", h*l);
	return h && l;

	//	if(h && l){
	//		return 1;
	//	}else{
	//		return 0;
	//	}
}



/*
 * this is a version with consecutive years on the run of year-to-year inspection
 */

map<int, set<int>*> *detect_WSI_SENSE_GIVEN_YEAR_EMERGENCE_EXP(WSIEMERGENCE *wsi, int *is_FilteredorValid_year, unsigned int value_to_filter,
		unsigned int windowsize)
																																																{


	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;// its not the difference between years its the number of years

	// windowsize=5;// read from the survey why did I choose five

	wsi->emergenceyearsbysenses=(int *)malloc((wsi->num_senses)*sizeof(int));


	//set<set<int>*>*wsisurges= new set<set<int>*>;


	map<int, set<int>*> *wsisurges= new map<int, set<int>*>;


	for(int sense=0; sense<wsi->num_senses; sense++){

		set<int>*surges = new set<int>;
		set<int>::iterator itrsetsur;
		//for(int n= wsi->minyear; n<= (wsi->maxyear-windowsize)+1 ; n++){//for each year we inspect 5 years

		for(int n= 0; n < (numofyears-windowsize) ; n++){// we cover from zero to (numberofyears-1)

			//		if((wsi->coutsallsenses_by_YEAR[i-wsi->minyear]) >= value_to_filter){

			//if(is_FilteredorValid_year[n-wsi->minyear]!=0){

			if(SurgeStrat_EXP(n, windowsize, wsi, is_FilteredorValid_year, sense)){


				fprintf(stderr, "FIND: the year to add to the surge is %d\n", n);
				surges->insert(n);// indexes of the year inside the set
			}
		}


		for(itrsetsur = surges->begin(); itrsetsur != surges->end(); itrsetsur++){

			fprintf(stderr, "FIND: the itrsetsur %d \n", (*itrsetsur));
		}


		fprintf(stderr, "FIND: surge size %lu\n", surges->size());
		if(surges->size()!=0){

			unsigned int emergenceindex = *(surges->begin());

			unsigned int emergenceyear= emergenceindex + wsi->minyear;


			fprintf(stderr, "FIND: the the index year of increasing emergenceindex %d, emergenceyear %d\n", emergenceindex, emergenceyear);


			wsi->emergenceyearsbysenses[sense]=emergenceyear;


			fprintf(stderr, "FIND: the wsi->emergenceyearsbysenses[sense] %d\n", wsi->emergenceyearsbysenses[sense]);



		}else{

			fprintf(stderr, "FIND: the the index year of increasing emergenceyear %d\n",0);

			wsi->emergenceyearsbysenses[sense]=0;
		}

		wsisurges->insert({sense,surges});

	}


	return wsisurges;

																																																}














void print_toFile_senseinfo_emergence_year_EXP(WSIEMERGENCE *wsi, char const *dir_emergence)
{


	createDirSafely(dir_emergence);

	char fname[500];
	strcpy(fname,dir_emergence);
	strcat(fname,"/");
	strcat(fname,wsi->wsdname);
	strcat(fname,"_SENSE_EMERGENCE");
	strcat(fname,".tsv");

	fprintf(stderr, "the name of the  file %s\n", fname);
	FILE *fl=NULL;
	fl= fopen(fname, "w");

	if(fl==NULL){
		fprintf(stderr, "ERROR OPENING FILE FOR SENSE_GIVEN_YEAR!\n");
		exit(1);
	}

	fprintf(fl,"SENSE INFORMATION\t");
	for(set<string>::iterator itrsen= wsi->senseorder->begin(); itrsen!=wsi->senseorder->end(); itrsen++){
		fprintf(stderr,"%s\t", (*itrsen).c_str());
		fprintf(fl,"%s\t", (*itrsen).c_str());
	}
	fprintf(fl,"\n");

	long double ***sense_arrinfo= layout_print_EXP(wsi);


	for(int l=0; l<numoflabels; l++){

		fprintf(fl,"%s",emergenceprintinglabels[l]);

		for(int s=0; s<wsi->num_senses; s++){

			fprintf(fl,"\t%Lf", sense_arrinfo[l][s][0]);
		}

		fprintf(fl,"\n");

		//		itr=wsi->senses_min_max->find(string(sensesinorder[s]));
		//		string senseID	=itr->first;
		//		SENSEEMERGENCE *senseinfo=  itr->second;
		//
		//fprintf(fl,"StartYear(counts)\t");
		//		//fprintf(fl,"StartYearValue(counts)\t");
		//		fprintf(fl,"%s\n", sense_arrinfo[l][s][0]);
		//
		//		//fprintf(fl,"LastYear(counts)\t");
		//		fprintf(fl,"%s\n", sense_arrinfo[l][s][0]);
		//
		//
		//		//fprintf(fl,"LastYearValue(counts)\t");
		//		fprintf(fl,"%s\n", sense_arrinfo[l][s][0]);
		//
		//
		//		//fprintf(fl,"MaxSensegivenYear(SGY)\t");
		//		fprintf(fl,"%s\n", sense_arrinfo[l][s][0]);
		//
		//
		//
		//		//fprintf(fl,"MaxSensegivenYearValue(SGY)\t");
		//		fprintf(fl,"%s\n", sense_arrinfo[l][s][0]);
		//
		//
		//
		//
		//	}

	}


	//	fprintf(fl,"EmergenceYear(SGY)");
	//	for(int s=0; s<wsi->num_senses; s++){
	//		fprintf(fl,"\t%d",wsi->emergenceyearsbysenses[s]);
	//	}
	//
	//	fprintf(fl,"\nEmergenceValue(SGY)");
	//	for(int s=0; s<wsi->num_senses; s++){
	//		fprintf(fl,"\t%Lf",wsi->sense_given_year[(wsi->emergenceyearsbysenses[s]-wsi->minyear)][s]);
	//	}
	//
	//	fprintf(fl,"\n");


	//	for(int f=0; f<numoflabels; f++){
	//		free_NOT_NULL_terminated_double_dimentions((void**)sense_arrinfo[f],numoflabels);
	//	}
	//	free(sense_arrinfo);// we dont need to free the triple star as its not malloced and its local.

}





long double ***layout_print_EXP(WSIEMERGENCE *wsi)
{

	long double ***printme=(long double ***)malloc((numoflabels)*sizeof(long double **));
	map<string, SENSEEMERGENCE *>::iterator itr;

	char **sensesinorder =get_sensesstrings_in_order_EXP(wsi);
	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;// its not the difference between years its the number of years


	for(int j=0; j<numoflabels; j++){
		printme[j]=(long double **)malloc((wsi->num_senses)*sizeof(long double *));
		for(int k=0; k<wsi->num_senses; k++){
			printme[j][k]=(long double *)malloc(sizeof(long double));
		}
	}





	//printme[i][s]=(int *)malloc(sizeof(int));
	for(int s=0 ;s<wsi->num_senses; s++){

		itr=wsi->senses_min_max->find(string(sensesinorder[s]));
		string senseID	=itr->first;
		SENSEEMERGENCE *senseinfo=  itr->second;
		unsigned int i=0;

		//fprintf(fl,"StartYear(counts)\t");
		printme[i][s][0]= senseinfo->s_min_year_counts;
		i++;
		//fprintf(fl,"StartYearValue(counts)\t");
		printme[i][s][0]=senseinfo->s_min_year_counts_value;
		i++;
		//fprintf(fl,"LastYear(counts)\t");
		printme[i][s][0]=senseinfo->s_max_year_counts;
		i++;

		//fprintf(fl,"LastYearValue(counts)\t");
		printme[i][s][0]=senseinfo->s_max_year_counts_value;
		i++;

		//fprintf(fl,"MaxSensegivenYear(SGY)\t");
		printme[i][s][0]=senseinfo->s_max_sense_given_year_year;
		i++;

		//fprintf(fl,"MaxSensegivenYearValue(SGY)\t");
		printme[i][s][0]=senseinfo->s_max_sense_given_year_value;
		i++;

		fprintf(stderr, "DEBUG: the wsi is %s and the sense %s \n", wsi->wsdname, senseID.c_str());
		fprintf(stderr, "DEBUG: the emergence year %d\n",wsi->emergenceyearsbysenses[s]);
		fprintf(stderr, "DEBUG: min year %d\n",wsi->minyear);

		//fprintf(fl,"EmergenceYear(SGY)\t");
		printme[i][s][0]=wsi->emergenceyearsbysenses[s];
		i++;
		if(wsi->emergenceyearsbysenses[s]!=0){
			//fprintf(fl,"EmergenceValue(SGY)\t");
			printme[i][s][0]=wsi->sense_given_year[(wsi->emergenceyearsbysenses[s]-wsi->minyear)][s];
			fprintf(stderr, "DEBUG: the value of the eme year %Lf\n",wsi->sense_given_year[(wsi->emergenceyearsbysenses[s]-wsi->minyear)][s]);

		}else{

			printme[i][s][0]=-1;

		}

	}


	return printme;
}
//***********************************DETECTBSELINEEMERGENCE***********************************//



/*
 * the insertion sorting algorithm is using the minimum in each left block.
 */
void sort_selection_years_EXP(WSIEMERGENCE *wsd)
{
	char **senseorder =get_sensesstrings_in_order_EXP(wsd);
	wsd->sortedyears=(unsigned int *)calloc(wsd->num_senses ,sizeof(unsigned int));
	map<string, SENSEEMERGENCE*>::iterator itrmaxmin;

	for(int i=0; i < wsd->num_senses ;i++)
	{
		itrmaxmin = wsd->senses_min_max->find(senseorder[i]);
		if(itrmaxmin != wsd->senses_min_max->end())
		{
			string sense =itrmaxmin->first;
			SENSEEMERGENCE *senseinfo =itrmaxmin->second;
			wsd->sortedyears[i]=senseinfo->s_min_year_counts;
		}
	}

	for(int i=0; i < wsd->num_senses; i++){
		fprintf(stderr,"DEBUG: the year before sorting %d\n",wsd->sortedyears[i]);
	}



	for(int i=0; i < wsd->num_senses; i++)
	{
		for(int j=i+1; j< wsd->num_senses; j++){

			if(wsd->sortedyears[i] > wsd->sortedyears[j]){
				int temp =wsd->sortedyears[j];
				wsd->sortedyears[j]=wsd->sortedyears[i];
				wsd->sortedyears[i]=temp;
			}
		}
	}



	for(int i=0; i < wsd->num_senses; i++){
		fprintf(stderr,"DEBUG: the sorted year before removing the dubplicate %d\n",wsd->sortedyears[i]);
	}


	remove_duplicate_EXP(wsd);

	for(int i=0; i < wsd->num_senses; i++){
		fprintf(stderr,"DEBUG: the sorted year after removing the dubplicate %d\n",wsd->sortedyears[i]);
	}

}



/*
 * the array must be sorted.
 */
void remove_duplicate_EXP(WSIEMERGENCE *wsd)
{
	int write=1;

	for(int read=1; read<wsd->num_senses;read++){

		if(wsd->sortedyears[read] != wsd->sortedyears[write-1]){

			wsd->sortedyears[write]=	wsd->sortedyears[read];
			write++;
		}
	}

	wsd->sortedyears =(unsigned int *)realloc(wsd->sortedyears,(write+1)*sizeof(unsigned int));

	wsd->size_sortedyear=(write);
	//wsd->sortedyears[write]=NULL;
}



//void create_rankYearArray(WSIEMERGENCE *wsd){
//
//
//	wsd->sensesOrderByStartYear=(char ***)malloc( (wsd->num_senses+1)*sizeof(char **));
//
//	map<string, SENSEEMERGENCE*>::iterator itrmaxmin;
//
//	sort_selection_years(wsd);
//
//	map<string, SENSEEMERGENCE*>::iterator itrmaxmin;
//
//	char **senseorder =get_sensesstrings_in_order(wsd);
//
//	for(int i=0; i < wsd->num_senses ;i++)
//	{
//		itrmaxmin = wsd->senses_min_max->find(senseorder[i]);
//
//		if(itrmaxmin != wsd->senses_min_max->end())
//		{
//			string sense =itrmaxmin->first;
//			SENSEEMERGENCE *senseinfo =itrmaxmin->second;
//
//
//
//		}
//
//
//	}
//
//
//
//
//
//}




void rank_baseline_Sensestartyears_EXP(WSIEMERGENCE *wsd)
{

	puts("\nIm inside rank_baseline_Sensestartyears\n");

	sort_selection_years_EXP(wsd);

	map<string, SENSEEMERGENCE*>::iterator itrmaxmin;

	char **senseorder =get_sensesstrings_in_order_EXP(wsd);

	wsd->sensesOrderByStartYear=(char ***)malloc((wsd->size_sortedyear+1)*sizeof(char **));


	fprintf(stderr, "DEBUG: the sizeof sorted year %d \n", wsd->size_sortedyear);


	for(int i=0; i < wsd->size_sortedyear ;i++){
		//wsd->sensesOrderByStartYear[i]= (char **)malloc(sizeof(char *));
		wsd->sensesOrderByStartYear[i]=NULL;

		fprintf(stderr, "DEBUG the cell %d is %p\n", i,wsd->sensesOrderByStartYear[i]);
	}


	wsd->sensesOrderByStartYear[wsd->size_sortedyear]=NULL;

	wsd->wsdEmerged_BLparamter=0;

	int s_order=0;
	for(int j=0; j< wsd->size_sortedyear; j++){


		fprintf(stderr, "DEBUG: the value of j %d\n", j);


		for(int s=0; s < wsd->num_senses ;s++){

			itrmaxmin = wsd->senses_min_max->find(senseorder[s]);

			if(itrmaxmin != wsd->senses_min_max->end())
			{
				string sense =itrmaxmin->first;
				SENSEEMERGENCE *senseinfo =itrmaxmin->second;


				if(wsd->sortedyears[j] == senseinfo->s_min_year_counts){


					fprintf(stderr, "DEBUG %d : the year is %d\n", j,wsd->sortedyears[j]);
					fprintf(stderr, "DEBUG j=%d the sense is %s\n ", j,sense.c_str());


					if(wsd->sensesOrderByStartYear[j]==NULL){

						wsd->sensesOrderByStartYear[j]=(char **)malloc(2*sizeof(char *));

						// can I access a cell that doesnt exist?
						wsd->sensesOrderByStartYear[j][0]=strdup(sense.c_str());
						wsd->sensesOrderByStartYear[j][1]=NULL;

						fprintf(stderr, "DEBUG the sense is %s and the index in sensesOrderByStartYear[%d][0]\n",wsd->sensesOrderByStartYear[j][0],j);
						fprintf(stderr, "DEBUG the year %d\n",wsd->sortedyears[j]);

						fprintf(stderr, "DEBUG the value of %s, sensesOrderByStartYear[%d][1]\n",wsd->sensesOrderByStartYear[j][1],j);

						senseinfo->order_diff_table= s_order;
						s_order++;
						senseinfo->Senseemerged_BLprameter=0;

					}else{

						for(int k=0; wsd->sensesOrderByStartYear[j][k]!=NULL; k++){

							fprintf(stderr, "DEBUG First INSIDE K the sense is %s and now wsd->sensesOrderByStartYear[%d][%d]\n",sense.c_str(),j, k);
							fprintf(stderr, "DEBUG the year %d\n",wsd->sortedyears[j]);

							if(wsd->sensesOrderByStartYear[j][k+1]==NULL){

								fprintf(stderr, "DEBUG INSIDE INSIDE K the sense is %s and now wsd->sensesOrderByStartYear[%d][%d]\n",sense.c_str(),j, k+1);
								fprintf(stderr, "DEBUG the year %d\n",wsd->sortedyears[j]);

								wsd->sensesOrderByStartYear[j][k+1]=strdup(sense.c_str());

								wsd->sensesOrderByStartYear[j]=(char **)realloc(wsd->sensesOrderByStartYear[j] ,(k+2)*sizeof(char *));

								wsd->sensesOrderByStartYear[j][k+2]=NULL;

								senseinfo->order_diff_table= s_order;
								s_order++;
								senseinfo->Senseemerged_BLprameter=0;
								break;

							}
						}
					}
				}
			}
		}
	}

	for(int print=0; print<wsd->size_sortedyear; print++){
		for(int print2=0; wsd->sensesOrderByStartYear[print][print2]!=NULL; print2++){
			fprintf(stderr, "DEBUG: %s\n",wsd->sensesOrderByStartYear[print][print2]);
		}
	}

	//	create_rankYearArray(wsd);
	//
	//	for(int i=0; i < wsd->num_senses ;i++)
	//	{
	//		itrmaxmin = wsd->senses_min_max->find(senseorder[i]);
	//
	//		if(itrmaxmin != wsd->senses_min_max->end())
	//		{
	//			string sense =itrmaxmin->first;
	//			SENSEEMERGENCE *senseinfo =itrmaxmin->second;
	//
	//			int len =strlen(senseorder[i]);
	//
	//			//wsd->sensesOrderByStartYear[i]=(char *)malloc(len*sizeof(char));
	//
	//			for(int j=0; j <wsd->num_senses; j++){
	//
	//				if(wsd->sortedyears[j] == senseinfo->s_min_year_counts){
	//
	//					senseinfo->sensStartYearOrder=j;
	//
	//					//wsd->sensesOrderByStartYear[j]=(char *)malloc(len*sizeof(char));
	//
	//
	//					int f=0;
	//					if(wsd->sensesOrderByStartYear[j][f]==NULL){
	//						wsd->sensesOrderByStartYear[j][f]= strdup(senseorder[i]);
	//					}else{
	//
	//						f++;
	//
	//
	//					}
	//
	//					fprintf(stderr, "j= %d sense %s\n", j,wsd->sensesOrderByStartYear[j]);
	//
	//
	//				}
	//			}
	//		}
	//	}
	//
	//	wsd->sensesOrderByStartYear[wsd->num_senses]=NULL;

}








//
//void label_baseline_Emergence_inOrder(WSIEMERGENCE *wsd)
//{
//	map<string, SENSEEMERGENCE*>::iterator itrmaxmin;
//
//	for(int i=0; i < wsd->num_senses ;i++){
//
//		//wsd->sensesOrderByStartYear[i];
//
//		itrmaxmin = wsd->senses_min_max->find(wsd->sensesOrderByStartYear[i]);
//
//		if(itrmaxmin != wsd->senses_min_max->end())
//		{
//			string sense =itrmaxmin->first;
//			SENSEEMERGENCE *senseinfo =itrmaxmin->second;
//
//			if(senseinfo->sensStartYearOrder == 0){
//
//				senseinfo->catchBaselineEmergence=0;
//
//			}else if(senseinfo->sensStartYearOrder > 0){
//
//
//				senseinfo->catchBaselineEmergence=1;
//			}else{
//
//				fprintf(stderr, "the value must be higher or equal to zero!\n");
//				exit(1);
//			}
//		}
//	}
//}








//void filter_baseline_Emergence(WSIEMERGENCE *wsd, int emergenceinterval)
//{
//	/*
//	 *
//	 */
//
//	map<string, SENSEEMERGENCE*>::iterator itrmaxmin;
//
//		for(int i=0; i < wsd->num_senses ;i++){
//
//			//wsd->sensesOrderByStartYear[i];
//
//			itrmaxmin = wsd->senses_min_max->find(wsd->sensesOrderByStartYear[i]);
//
//			if(itrmaxmin != wsd->senses_min_max->end())
//			{
//				string sense =itrmaxmin->first;
//				SENSEEMERGENCE *senseinfo =itrmaxmin->second;
//
//
//				senseinfo->sensStartYearOrder;
//				//wsd->sensesOrderByStartYear();
//
//
//
//			}
//
//
//
//
//
//
//		}
//
//
//}



//	wsd->senses_filteredTrueEmeregence;
//
//	for(int a=0; a < (wsd->num_senses-1); a++){
//
//				for(int b=a+1; b < wsd->num_senses; b++){
//
//
//				wsd->diff_senses_firstyears[a][b];
//
//
//
//
//				}
//
//
//			}
//
//}





















//***********************************CALCULATINGandCOUNTINGPART******************************//



char *addlog_to_Directories_Paths_EXP_NOTMICS(char const *logResultPath, unsigned int thershold_exp, char const *dirname)
{

	if(strcmp(dirname,"")==0){
		fprintf(stderr, "ERORR: EMPTY DIRECTORY NAME\n");
		exit(1);
	}

	int len=strlen(logResultPath);
	len += strlen(dirname);

	char *dirnamepath=NULL;

	char const *dirthreshold=strstr(dirname,"threshold");
	if(dirthreshold!=NULL){

		dirnamepath = (char *)malloc((25+len)*sizeof(char));
		strcpy(dirnamepath,logResultPath);
		strcat(dirnamepath,"/");
		char name[150];
		strcpy(name,dirname);
		strcat(name,"_withvalue_");
		char num[50];
		sprintf(num,"%d_",thershold_exp);
		strcat(name,num);
		strcat(dirnamepath, name);

	}else{

		dirnamepath= (char *)malloc((2+len)*sizeof(char));
		strcpy(dirnamepath,logResultPath);
		strcat(dirnamepath,"/");
		strcat(dirnamepath, dirname);
	}

	if(strcmp(dirnamepath,"")==0){
		fprintf(stderr, "ERORR: EMPTY DIRECTORY NAME\n");
		exit(1);
	}

	return dirnamepath;
}



char const *create_LogRESULTS_Directory_EXP_NOTMISC(char const *results_path){

	fprintf(stderr,"DEBUG: results_path is %s\n",results_path);

	time_t rawtime;
	char mytime[150];
	time(&rawtime);
	sprintf(mytime,"%s",ctime(&rawtime));


	fprintf(stderr,"DEBUG: the time is %s\n",mytime);


	mytime[strcspn(mytime, "\r\n")] = '\0';

	fprintf(stderr,"DEBUG: the time is after removing the backslash %s\n",mytime);

	int len=strlen(results_path);
	len+=strlen(mytime);
	char *buffer=(char *)malloc((len+3)*sizeof(char));

	strcpy(buffer,results_path);
	strcat(buffer,"_");
	fprintf(stderr,"DEBUG: the path is %s\n",buffer);


	strcat(buffer,mytime);
	fprintf(stderr,"DEBUG: the path with time is %s\n",buffer);


	// Lets convert space to _ in

	char *p = buffer;
	for (; *p; ++p)
	{
		if (*p == ' ')
			*p = '_';
	}


	fprintf(stderr,"DEBUG: create_LogRESULTS_Directory %s\n",buffer);
	//fopen(buffer,"w");

	if(strcmp(buffer,"")==0){
		fprintf(stderr, "ERORR: EMPTY DIRECTORY NAME\n");
		exit(1);
	}

	createDirSafely(buffer);

	return buffer;
}



/*
 * FREE OUTSIDE
 * it create an array in the heap and return it with senses strings in order! Need to be freed if not needed.
 */

char **get_sensesstrings_in_order_EXP(WSIEMERGENCE *wsi){

	char **order=(char **)malloc(wsi->num_senses*sizeof(char*));
	set<string>::iterator itrset;
	int in=0;
	for(itrset= wsi->senseorder->begin(); itrset!=wsi->senseorder->end(); itrset++){
		int len=strlen((*itrset).c_str());
		order[in]=(char*)malloc(len+1*sizeof(char));
		order[in]=strdup((*itrset).c_str());
		in++;
	}

	return order;
}



char *get_sensestring_from_index_EXP(WSIEMERGENCE *wsi, int sensenum){


	char **ordersenses=get_sensesstrings_in_order_EXP(wsi);

	char *sensestring=strdup(ordersenses[sensenum]);


	free_NOT_NULL_terminated_double_dimentions((void**)ordersenses, wsi->num_senses);

	return sensestring;

}










void store_min_max_senses_years_from_CONDITIONAL_EXP(WSIEMERGENCE *wsi)
{

	char **order=(char **)malloc(wsi->num_senses*sizeof(char*));
	set<string>::iterator itrset;
	int in=0;
	for(itrset= wsi->senseorder->begin(); itrset!=wsi->senseorder->end(); itrset++){
		int len=strlen((*itrset).c_str());
		order[in]=(char*)malloc(len+1*sizeof(char));
		order[in]=strdup((*itrset).c_str());
		in++;
	}


	map<string, SENSEEMERGENCE*>::iterator itrmaxmin;

	for(int sense=0; sense< wsi->num_senses; sense++){

		fprintf(stderr, "the name of the AMBIGIOUS WORD is %s and the SENSE is %d\n", wsi->wsdname ,sense);


		long double currmax=0;
		unsigned int yearofmax=0;

		for(int v= wsi->minyear; v<= wsi->maxyear; v++){

			if(currmax < wsi->sense_given_year[v-wsi->minyear][sense]){

				fprintf(stderr, "the value of the current max is %Lf, and the year of that value is %d and the index of the year %d\n",
						currmax, v, v-wsi->minyear);

				currmax=wsi->sense_given_year[v-wsi->minyear][sense];
				yearofmax= v;
			}
		}


		itrmaxmin= wsi->senses_min_max->find(string(order[sense]));
		if(itrmaxmin != wsi->senses_min_max->end()){

			SENSEEMERGENCE *senseinfo=itrmaxmin->second;
			senseinfo->s_max_sense_given_year_value= currmax;
			senseinfo->s_max_sense_given_year_year= yearofmax;

		}else{

			fprintf(stderr, "This case must not happen as the map must be already initialised previously!\n");
			exit(1);
			SENSEEMERGENCE *senseinfo= (SENSEEMERGENCE *)malloc(sizeof(SENSEEMERGENCE ));
			senseinfo->s_max_sense_given_year_value= currmax;
			senseinfo->s_max_sense_given_year_year= yearofmax;

			wsi->senses_min_max->insert({string(order[sense]),senseinfo});
		}

	}

}















/*
 * this function assume that the counts data must be organised from minimum year to maximum year., to get the min year and the max year
 * for each sense independently.
 */

void store_min_max_senses_years_from_COUNTS_EXP(WSIEMERGENCE *wsi)
{

	//to find the order of the senses
	char **order=(char **)malloc(wsi->num_senses*sizeof(char*));
	set<string>::iterator itrset;
	int in=0;
	for(itrset= wsi->senseorder->begin(); itrset!=wsi->senseorder->end(); itrset++){
		int len=strlen((*itrset).c_str());
		order[in]=(char*)malloc(len+1*sizeof(char));
		order[in]=strdup((*itrset).c_str());

		fprintf(stderr, "DEBUG the senses in order[%d]= %s\t",in, order[in]);
		in++;
	}

	puts("\n");

	map<string, SENSEEMERGENCE*>::iterator itrmaxmin;
	puts("GETTING THE MIN YEAR AND VALUE FROM COUNTS!\n");

	//to get the min year where this sense start to exist from the counts data
	for(int sense=0; sense< wsi->num_senses; sense++){

		fprintf(stderr, "DEBUG: main loop number %d sense num %d and the min year overall %d \n", sense ,sense,wsi->minyear);

		for(int i= wsi->minyear; i<= wsi->maxyear ; i++){

			if(wsi->senses_years_counts[i-wsi->minyear][sense]>0){

				itrmaxmin =wsi->senses_min_max->find(string(order[sense]));

				fprintf(stderr, "DEBUG: inner loop number %d ***the word is %s the sense is %s and the first year %d and its value %d***\n",
						i-wsi->minyear,wsi->wsdname,order[sense], (i), wsi->senses_years_counts[i-wsi->minyear][sense]);

				if(itrmaxmin == wsi->senses_min_max->end()){

					fprintf(stderr, "DEBUG *** Im here not found %s *** \n",order[sense]);

					SENSEEMERGENCE *senseinfo= (SENSEEMERGENCE *)malloc(sizeof(SENSEEMERGENCE ));

					fprintf(stderr, "DEBUG: senseinfo->s_min_year_counts %d, senseinfo->s_min_year_counts_value %d\n"
							,i,wsi->senses_years_counts[i-wsi->minyear][sense]);
					senseinfo->s_min_year_counts = i;
					senseinfo->s_min_year_counts_value = wsi->senses_years_counts[i-wsi->minyear][sense];

					wsi->senses_min_max->insert({string(order[sense]), senseinfo});

					break;

				}else{

					fprintf(stderr,"Error: this sense must not exist previously in this min_max map!\n");
					exit(1);
					SENSEEMERGENCE *senseinfo=	itrmaxmin->second;
					senseinfo->s_min_year_counts =wsi->senses_years_counts[i-wsi->minyear][sense];
					break;
				}
			}
		}


		puts("GETTING THE MAX YEAR AND VALUE FROM COUNTS!\n");
		//to get the max
		unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;

		//	for(int x= wsi->maxyear; x>= wsi->minyear ; x--){

		for(int x=numofyears-1; x>= 0; x--){

			fprintf(stderr, "DEBUG: inner loop number %d ***the word is %s the sense is %s and the last year %d and its index %d and its value %d***\n",
					(numofyears-1)-x, wsi->wsdname, order[sense], (wsi->minyear+x), x, wsi->senses_years_counts[x][sense]);

			if(wsi->senses_years_counts[x][sense]>0){

				itrmaxmin =	wsi->senses_min_max->find(string(order[sense]));

				if(itrmaxmin != wsi->senses_min_max->end()){

					fprintf(stderr, "DEBUG *** Im here found %s *** \n",order[sense]);
					SENSEEMERGENCE *senseinfo=	itrmaxmin->second;

					fprintf(stderr, "DEBUG: senseinfo->s_max_year_counts %d, senseinfo->s_max_year_counts_value %d\n"
							, (wsi->minyear+x), wsi->senses_years_counts[x][sense]);

					senseinfo->s_max_year_counts= (wsi->minyear+x);
					senseinfo->s_max_year_counts_value= wsi->senses_years_counts[x][sense];

					// I dont need to insert here!
					break;

				}else{
					fprintf(stderr,"Error: this sense must exist in this min_max map as it was previously initialised!\n");
					exit(1);
					SENSEEMERGENCE *senseinfo= (SENSEEMERGENCE *)malloc(sizeof(SENSEEMERGENCE ));
					senseinfo->s_max_year_counts= x;
					senseinfo->s_max_year_counts_value= wsi->senses_years_counts[wsi->maxyear-x][sense];
					wsi->senses_min_max->insert({string(order[sense]),senseinfo});
					break;
				}
			}
		}
	}


	free_NOT_NULL_terminated_double_dimentions((void**)order, wsi->num_senses);
}



void min_max_senses_by_Year_forWSI_EXP(map<string, WSIEMERGENCE *> *WSI_EMERGENCE_INFO, char **targetNames,char ****targetContent,
		unsigned int numofWSIS, char const *dirname_min_max)
{
	createDirSafely(dirname_min_max);

	for(map<string, WSIEMERGENCE *>::iterator itrmap=WSI_EMERGENCE_INFO->begin(); itrmap!=WSI_EMERGENCE_INFO->end(); itrmap++){

		int nameIndex=0;
		for(; targetNames[nameIndex]!=NULL;nameIndex++){
			if(strcmp(itrmap->first.c_str(), targetNames[nameIndex])==0){
				break;
			}
		}

		WSIEMERGENCE *WSI_COUNTS=itrmap->second;

		char fname[500];
		strcpy(fname,dirname_min_max);
		strcat(fname,"/");
		strcat(fname,itrmap->first.c_str());
		strcat(fname,"/");

		for(set<string>::iterator itrset=WSI_COUNTS->senseorder->begin(); itrset!=WSI_COUNTS->senseorder->end(); itrset++){

			strcat(fname,(*itrset).c_str());
			strcat(fname,"_min_max_senses_by_Year");
			strcat(fname,".tsv");

			fprintf(stderr, "the name of the  file %s\n", fname);
			FILE *fl=NULL;
			fl= fopen(fname, "w");

			if(fl==NULL){
				fprintf(stderr, "ERROR OPENING FILE FOR COUNTS_BY_YEAR_AND_SENSE!\n");
				exit(1);
			}

			fprintf(fl,"YEAR\t");

		}

	}
}


///*
// * creates an boolean array which says weather a year has zero data or some data
// */
//void eliminate_years_with_zero(WSIEMERGENCE *wsi)
//{
//	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;
//	wsi->is_valid_year=(int *)malloc(numofyears*sizeof(int));
//
//	for(int i= wsi->minyear; i<= wsi->maxyear ; i++){
//
//		if(wsi->coutsallsenses_by_YEAR[i-wsi->minyear] != 0){
//
//			wsi->is_valid_year[i-wsi->minyear]=1;
//
//		}else{
//
//			wsi->is_valid_year[i-wsi->minyear]=0;
//		}
//
//	}
//}




/*
 * creates an boolean array which says weather a year either (has zero data or some data) or (lower than a threshold value)
 */

void lable_years_with_certain_value_EXP(WSIEMERGENCE *wsi, int **is_included, int value_to_filter){


	fprintf(stderr, "DEBUGP: the value it recived is %p while the address  %p",is_included, &is_included );
	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;
	(*is_included)=(int *)malloc((numofyears)*sizeof(int));

	if((*is_included)==NULL){
		fprintf(stderr, "the malloc function returned NULL in function lable_years_with_certain_value!\n");
		exit(1);
	}


	for(int i= wsi->minyear; i<= wsi->maxyear ; i++){

		if((wsi->coutsallsenses_by_YEAR[i-wsi->minyear]) != 0 || ((wsi->coutsallsenses_by_YEAR[i-wsi->minyear]) > value_to_filter)){

			(*is_included)[i-wsi->minyear]=1;

		}else{

			(*is_included)[i-wsi->minyear]=0;
		}

	}

}





void print_sense_given_year_matrix_EXP(WSIEMERGENCE *wsi, char const *dirname_sense_given_year)
{


	createDirSafely(dirname_sense_given_year);

	char fname[500];
	strcpy(fname,dirname_sense_given_year);
	strcat(fname,"/");
	strcat(fname,wsi->wsdname);
	strcat(fname,"_SENSE_GIVEN_YEAR");
	strcat(fname,".tsv");


	fprintf(stderr, "the name of the  file %s\n", fname);
	FILE *fl=NULL;
	fl= fopen(fname, "w");

	if(fl==NULL){
		fprintf(stderr, "ERROR OPENING FILE FOR SENSE_GIVEN_YEAR!\n");
		exit(1);
	}


	fprintf(fl, "YEAR\t");
	for(set<string>::iterator itrsen= wsi->senseorder->begin(); itrsen!=wsi->senseorder->end(); itrsen++){
		//fprintf(stderr,"%s\t", (*itrsen).c_str());
		fprintf(fl,"%s\t", (*itrsen).c_str());
	}
	fprintf(fl,"\n");



	for(int i=wsi->minyear; i<= wsi->maxyear ; i++){

		fprintf(fl,"%d\t", i);

		for(int j=0; j<wsi->num_senses; j++){


			fprintf(fl, "%Lf\t",wsi->sense_given_year[i-wsi->minyear][j]);

		}
		fprintf(fl,"\n");
	}

	fclose(fl);
}







void print_sense_given_year_matrix_after_elimintated_years_EXP(WSIEMERGENCE *wsi, int *is_included, int value_to_filter,
		char const *dirname_conditional_eliminated_years)
{


	createDirSafely(dirname_conditional_eliminated_years);

	char fname[500];
	strcpy(fname,dirname_conditional_eliminated_years);
	strcat(fname,"/");
	strcat(fname,wsi->wsdname);
	strcat(fname,"_CONDITIONAL_ELIMINATED_YEARS");
	strcat(fname,".tsv");


	fprintf(stderr, "the name of the  file %s\n", fname);
	FILE *fl=NULL;
	fl= fopen(fname, "w");

	if(fl==NULL){
		fprintf(stderr, "ERROR OPENING FILE FOR COUNTS_BY_YEAR_AND_SENSE!\n");
		exit(1);
	}


	fprintf(fl,"YEAR\t");
	for(set<string>::iterator itrsen= wsi->senseorder->begin(); itrsen!=wsi->senseorder->end(); itrsen++){
		fprintf(stderr,"%s\t", (*itrsen).c_str());
		fprintf(fl,"%s\t", (*itrsen).c_str());
	}
	fprintf(fl,"\n");



	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;

	for(int i=wsi->minyear; i<= wsi->maxyear ; i++){

		if(is_included[i-wsi->minyear]==1){

			fprintf(fl,"%d\t", i);

			for(int sense=0; sense< wsi->num_senses; sense++){
				fprintf(fl,"%Lf\t",wsi->sense_given_year[i-wsi->minyear][sense]);
			}
			fprintf(fl,"\n");
		}
	}

}






void generate_sense_given_year_matrix_EXP(WSIEMERGENCE *wsi)
{
	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;

	wsi->sense_given_year=(long double **)malloc((numofyears)*sizeof(long double *));


	for(int year=0; year<numofyears; year++){

		wsi->sense_given_year[year]=(long double*)calloc((wsi->num_senses),sizeof(long double));


		for(int sense=0; sense< wsi->num_senses; sense++){

			long double conditonalprob =calculate_sense_given_year_EXP(wsi, year, sense);

			wsi->sense_given_year[year][sense]=conditonalprob;

		}
	}

}



long double calculate_sense_given_year_EXP( WSIEMERGENCE *wsi, int yearIndex, int senseIndex)
{

	if((long double)wsi->coutsallsenses_by_YEAR[yearIndex]!=0){

		return((long double)wsi->senses_years_counts[yearIndex][senseIndex]/(long double)wsi->coutsallsenses_by_YEAR[yearIndex]);


	}else{

		return 0;
	}

}



void read_COUNTS_By_YEAR_and_SENSE_EXP(map<string, WSIEMERGENCE *> *WSI_EMERGENCE_INFO, char const *dirname_print)
{
	puts("IM CALLING read_COUNTS\n");


	char **targetNames=NULL;
	char ****targetContent=NULL;

	unsigned int numofWSIS = read_COUNTS_EXP(dirname_print,targetContent,targetNames);


	//min_max_senses_by_Year_forWSI(WSI_EMERGENCE_INFO, targetContent,targetNames,numofWSIS, dirname_min_max);

}


unsigned int read_COUNTS_EXP(char const  *dirname, char ****targetContent, char **targetNames)
{

	//READ THE DIR HERE!
	//check if its created!
	//createDirSafely(dirname);

	fprintf(stderr, "dirname %s\n", dirname);
	const char **dircontent = read_dir_Targetscontent_EXP(dirname);

	targetNames=(char **)malloc(block_size*sizeof(char *));
	targetContent= (char ****)malloc(block_size*sizeof(char ***));
	targetNames[0]=NULL;
	targetContent[0]=NULL;

	//		puts("IM PRINTING THE DIRE ARRAY FILES\n");
	//        print_stringarray(dircontent);

	int f=0;
	for(; dircontent[f]!=NULL; f++){


		fprintf(stderr, "%d: %s\n",f,dircontent[f]);

		char buff[10000];
		FILE *fptr =NULL;
		fptr = fopen(dircontent[f],"r");

		if(fptr ==NULL){
			fprintf(stderr, "Error opening the file\n");
			exit(1);
		}

		char const *startname =strrchr(dircontent[f],'/');
		char const *start= startname+1;
		//fprintf(stderr, "DEBUG: start %p\n",start);
		//fprintf(stderr, "DEBUG: start %s\n",start);
		char const *endname =strchr(dircontent[f],'_');
		char const *end=endname;
		//fprintf(stderr, "DEBUG: end %p\n",endname);
		//fprintf(stderr, "DEBUG: end %s\n",end);
		unsigned int length=end-start;
		//fprintf(stderr, "DEBUG: length %d\n", length);
		char *wsi=(char *)malloc(length*sizeof(char));
		strncpy(wsi,start,length);
		wsi[length]='\0';
		//fprintf(stderr,"DEBUG: WSI%s\n",wsi);
		//already done

		// we can add the content from the map that we stored before


		if(((f+1)%block_size)==0){
			targetContent=(char ****)realloc(targetContent,(f+1+block_size)*sizeof(char ***));
			targetNames=(char **)realloc(targetNames,(f+1+block_size)*sizeof(char*));
		}


		targetNames[f]=(char *)malloc((length+1)*sizeof(char));
		strcpy(targetNames[f], wsi);
		targetNames[f][length]='\0';
		//fprintf(stderr, "%s the last result\n ", targetNames[f+block_size]);


		int j=0;
		targetContent[f]= (char ***)realloc(targetContent[f],(block_size)*sizeof(char **));
		while(fgets(buff, 10000, fptr)!=NULL){

			buff[strcspn(buff, "\r\n")] = '\0';
			char **WSItokns = tokenise_opt2(buff,"	");
			//			for(int i=0; WSItokns[i]!=NULL; i++){
			//				printf("the %d string from the WSI_GOLD is %s\n",i, WSItokns[i]);
			//			}


			if(((j+1)%block_size)==0){
				targetContent[f]= (char ***)realloc(targetContent[f],(j+1+block_size)*sizeof(char **));
				//(*targetContent)[j+block_size]=NULL;WRONG CUZ SOMETIMES I DONT FILL THE WHOLE 10 BLOCKS
			}

			targetContent[f][j]= WSItokns;
			j++;
			targetContent[f][j]=NULL;
		}

		targetContent[f+1]=NULL;
		targetNames[f+1]=NULL;

	}

	//targetContent[f]=NULL;

	return f;
}

const char **read_dir_Targetscontent_EXP(const char *dirname)
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


int getSenseIndex_set_EXP(string sense, set<string> *senseorder)
{
	set<string>::iterator itrorder;
	int i=0;
	for(itrorder= senseorder->begin(); itrorder != senseorder->end(); itrorder++){
		string s=*itrorder;
		if (s.compare(sense) == 0){
			return i;
		}
		i++;
	}

	fprintf(stderr, "ERROR: this sense must has an index!\n");
	exit(1);
}






void print_toFile_allemergence_Sense_EXP(WSIEMERGENCE *wsi, char const *colonamesSENSESINFO,map<int, set<int> *> *wsi_surges,char const *dirnamesense)
{

	// FILE *sensesinfo=write_to_FILE_EXP(dirname_allemergence,colonames_SENSESINFO,"SensesInfoEmergence");
	createDirSafely(dirnamesense);

	char fname[500];
	strcpy(fname,dirnamesense);
	strcat(fname,"/");
	strcat(fname,wsi->wsdname);
	strcat(fname,"_emergence.tsv");


	fprintf(stderr, "the name of the  file %s\n", fname);
	FILE *fl=NULL;
	fl= fopen(fname, "w");

	if(fl==NULL){
		fprintf(stderr, "ERROR OPENING FILE FOR %s %s!\n", wsi->wsdname, fname);
		exit(1);
	}

	//"WSD_INFORMATION\tNUM_SENSES\tSENSE\tEMERGENCE_YEAR\tEMERGENCE_VALUE\tALLSURGES\tMinYearforSenseData\tMazYearforSenseData\tDataStart\tDataEnd";

	fprintf(fl, "%s\n", colonamesSENSESINFO);

	for(int s=0 ;s<wsi->num_senses; s++){

		//		for(set<string>::iterator itrsen= wsi->senseorder->begin(); itrsen!=wsi->senseorder->end(); itrsen++){
		//				fprintf(stderr,"%s\t", (*itrsen).c_str());
		//			fprintf(fl, "%s\t%s", wsi->wsdname, wsi->num_senses );
		//				fprintf(fl,"\t%s", (*itrsen).c_str());
		//if(wsi->emergenceyearsbysenses[s]!=0){

		fprintf(fl,"%s" ,wsi->wsdname);
		fprintf(fl,"\t");

		fprintf(fl,"%d" ,wsi->num_senses);
		fprintf(fl,"\t");

		char *sensestring= get_sensestring_from_index_EXP(wsi,s);

		char **sensestrings= get_sensesstrings_in_order_EXP(wsi);

		fprintf(fl,"%s" ,sensestring);
		fprintf(fl,"\t");

		fprintf(fl,"%d" ,wsi->emergenceyearsbysenses[s]);
		fprintf(fl,"\t");


		if(wsi->emergenceyearsbysenses[s] > 0){

			fprintf(fl, "%Lf", wsi->sense_given_year[(wsi->emergenceyearsbysenses[s]-wsi->minyear)][s]);
			fprintf(fl,"\t");

			for(map<int, set<int> *>::iterator itrmap= wsi_surges->begin(); itrmap!= wsi_surges->end(); itrmap++ ){

				if((itrmap->first) == s){

					for(set<int>::iterator itrset = itrmap->second->begin();itrset!= itrmap->second->end(); itrset++){

						unsigned int yearindex= *itrset;
						unsigned int yearvalue =yearindex+ wsi->minyear;

						fprintf(fl, "%d", yearvalue);
						fprintf(fl,"|");

					}
				}
			}
		}else{
			fprintf(fl,"NA\tNA");
		}
		fprintf(fl,"\t");

		map<string, SENSEEMERGENCE*>::iterator itrseninfo= wsi->senses_min_max->find(string(sensestrings[s]));

		if(itrseninfo!= wsi->senses_min_max->end()){

			SENSEEMERGENCE *senseemr =itrseninfo->second;

			fprintf(fl,"%d\t",senseemr->s_min_year_counts);
			fprintf(fl,"%d\t",senseemr->s_max_year_counts);

			fprintf(fl,"%d\t",wsi->minyear);
			fprintf(fl,"%d",wsi->maxyear);

			//		fprintf(fl,"%d\t" ,wsi->emergenceyearsbysenses[s]-senseemr->s_min_year_counts);

		}else{
			fprintf(stderr, "ERROR: this sense must be found!");
			exit(1);
		}

		fprintf(fl,"\n");
		//free dont forget it
	}
}



void print_generate_counts_bySenseandYear_EXP(WSIEMERGENCE *wsi, char const *dirname_print)
{
	createDirSafely(dirname_print);

	char fname[500];
	strcpy(fname,dirname_print);
	strcat(fname,"/");
	strcat(fname,wsi->wsdname);
	strcat(fname,"_COUNTS_BY_YEAR_AND_SENSE");
	strcat(fname,".tsv");


	fprintf(stderr, "the name of the  file %s\n", fname);
	FILE *fl=NULL;
	fl= fopen(fname, "w");

	if(fl==NULL){
		fprintf(stderr, "ERROR OPENING FILE FOR COUNTS_BY_YEAR_AND_SENSE!\n");
		exit(1);
	}

	fprintf(fl,"YEAR\t");
	for(set<string>::iterator itrsen= wsi->senseorder->begin(); itrsen!=wsi->senseorder->end(); itrsen++){
		fprintf(stderr,"%s\t", (*itrsen).c_str());
		fprintf(fl,"%s\t", (*itrsen).c_str());
	}
	fprintf(fl,"\n");

	//int minyear=wsi->minyear;

	for(int i=wsi->minyear; i<= wsi->maxyear ; i++){

		fprintf(fl,"%d\t", i);

		for(int j=0; j<wsi->num_senses; j++){

			//fprintf(stderr,"senseloop%d    %d\t",j, wsi->senses_years_counts[i-wsi->minyear][j]);

			fprintf(fl,"%d\t", wsi->senses_years_counts[i-wsi->minyear][j]);
		}
		fprintf(fl,"\n");
	}

	fclose(fl);
}


/*
 * it creates a matrix for each WSI that contains the frequency for each year and sense
 */

void generate_counts_bySenseandYear_EXP(char ***targetContent, unsigned int size , WSIEMERGENCE *wsi, int col_yearIndex, int col_wsdIndex){

	puts("Im inside the generate_counts_bySenseandYear\n");
	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;// so we dont get the index 0 if we only have one year

	//	int **sensesyearscounts= (int**)malloc(numofyears*sizeof(int*));// raws=years
	//	int i=0;
	//	for(;i<numofyears;i++){
	//		sensesyearscounts[i]= (int*)malloc(((wsi->num_senses))*sizeof(int));// columns=senses
	//	}

	fprintf(stderr,"DEBUG: num of years %d\n",numofyears );
	wsi->senses_years_counts= (int**)malloc(numofyears*sizeof(int*));// raws=years
	//sensesyearscounts[numofyears]=NULL;
	int i=0;
	int print=0;
	for(;i<numofyears;i++){
		wsi->senses_years_counts[i]= (int*)calloc(((wsi->num_senses)),sizeof(int));// columns=senses
		fprintf(stderr,"DEBUG !!!!!!!!!!!the initial value %d!!!!!!!!!!!!\n",wsi->senses_years_counts[i][print]);
	}



	int raw=1;

	for( ;raw<size; raw++){

		fprintf(stderr,"the YEAR col %d\n", col_yearIndex);
		fprintf(stderr,"the WSD col %d\n", col_wsdIndex);
		fprintf(stderr,"the targetContent[raw][col_wsdIndex] is %s \n",targetContent[raw][col_wsdIndex]);

		int senseIndex =getSenseIndex_set_EXP(string(targetContent[raw][col_wsdIndex]), wsi->senseorder);

		fprintf(stderr,"the WSD col %d\n", col_wsdIndex);

		fprintf(stderr,"the sense index %d\n",senseIndex);

		unsigned int result_yearIndex =atoi(targetContent[raw][col_yearIndex])-wsi->minyear;

		fprintf(stderr,"the result year Index %d\n",result_yearIndex);

		wsi->senses_years_counts[result_yearIndex][senseIndex]++;

		fprintf(stderr, "the count value is %d\n", wsi->senses_years_counts[result_yearIndex][senseIndex]);


		//sleep (1);

	}


	//return sensesyearscounts;
}








/*
 * This function will do the counting for a wsi
 */

// if I want it genaric I return a set fro each coloum or an array
// the size including the first line but staring from zero so the size id correct
WSIEMERGENCE *store_WSDContent_EXP(char ***targetContent, unsigned int size, unsigned int thershold_exp)
{

	WSIEMERGENCE *wsi= (WSIEMERGENCE *)malloc(sizeof(WSIEMERGENCE));
	wsi->sensescounts= new map<string, int>();
	wsi->senseorder= new set<string>();
	wsi->is_valid_year= NULL;
	wsi->is_filtered_year= NULL;
	wsi->size_of_threshold_data= 0;
	wsi->senses_min_max=new map<string, SENSEEMERGENCE*>;

	int year_ColIndex=-1;
	int sense_ColIndex=-1;
	set<int> *years= new set<int>();


	char **namecols=targetContent[0];
	unsigned int  colsnum=nullterminatedlength((void **)namecols);

	//print_stringarray(namecols);

	int check=0;
	for(int n=0; n<colsnum; n++){

		fprintf(stderr,"DEBUG: %d namescol: %s\n",n,namecols[n]);



		if(strcmp(namecols[n],"YEAR")==0){

			int m=1;

			for( ;m<size; m++){

				fprintf(stderr,"targetContent[m][n]= %s\n",targetContent[m][n]);
				int year =atoi(targetContent[m][n]);
				fprintf(stderr,"m =%d \t n =%d \t year=%d\n",m,n,year);


				check=0;
				while((targetContent[m][n]!=NULL) && (isdigit(targetContent[m][n][check]))){
					check++;
				}

				if(check==4){
					years->insert(year);
				}

			}

			wsi->total_num_instances= m;
			year_ColIndex =n;
		}




		//		for(int c=0 ; targetContent[c][1]!= NULL ; c++){

		if((strcmp(namecols[n],"WSD")==0)){

			// if((strcmp(targetContent[0][0],"YEAR")==0) && ){

			wsi->wsdname=targetContent[1][n];
			fprintf(stderr, "DEBUG: wsi->wsdname %s\n",wsi->wsdname );

			//}
		}

		//sleep(1);
		//	}

		if((strcmp(namecols[n],"CUI")==0)){

			//fprintf(stderr,"I am CUI: %s\n",namecols[n]);

			int l=1;
			for(; l<size; l++){
				//fprintf(stderr, "DEBUG:*******LOOP NUMBER******* %d\n", l);

				map<string, int>::iterator itrsens;

				itrsens= wsi->sensescounts->find(string(targetContent[l][n]));

				//fprintf(stderr,"targetContent[l][n] %s\n",targetContent[l][n]);


				if(itrsens!=wsi->sensescounts->end()){

					itrsens->second++;
					//wsi->sensescounts->insert({string(targetContent[l][n]), count});
					//					fprintf(stderr,"DEBUG found:  %s %d\n",itrsens->first.c_str(),itrsens->second);
					//					fprintf(stderr,"DEBUG found:  %s %d\n",itrsens->first.c_str(),itrsens->second);
				}else{
					wsi->sensescounts->insert({string(targetContent[l][n]), 1});

					//				fprintf(stderr, "DEBUG: insert key %s\n",targetContent[l][n] );
					//				fprintf(stderr, "DEBUG: insert value %d\n",1);
					wsi->senseorder->insert(string(targetContent[l][n]));

				}

			}
			wsi->num_senses= wsi->sensescounts->size();
			fprintf(stderr, "DEBUG the num of sense %d\n",wsi->num_senses);
			sense_ColIndex =n;
		}






		//		for(map<string, int>::iterator itrsen=wsi->sensescounts->begin(); itrsen!=wsi->sensescounts->end(); itrsen++){
		//		fprintf(stderr, "DEBUG: key %s\n",itrsen->first.c_str() );
		//		fprintf(stderr, "DEBUG: value %d\n",itrsen->second);
		//		}
		for(set<string>::iterator itrset=wsi->senseorder->begin();itrset!=wsi->senseorder->end(); itrset++){
			fprintf(stderr, "DEBUG: values in set %s\n",(*itrset).c_str());
		}


	}


	wsi->minyear = *years->begin();
	wsi->maxyear= *years->rbegin();

	fprintf(stderr, "DEBUG: wsi->minyear %d\n", wsi->minyear);
	fprintf(stderr, "DEBUG: wsi->maxyear %d\n", wsi->maxyear);

	//	puts("I am exsiting!\n");
	//exit(1);

	//puts("IM CALLING generate_counts_bySenseandYear\n");
	generate_counts_bySenseandYear_EXP(targetContent, size ,wsi,year_ColIndex, sense_ColIndex);

	//exit(1);
	store_min_max_senses_years_from_COUNTS_EXP(wsi);// not debugged


	generate_total_byYear_EXP(wsi);


	generate_sense_given_year_matrix_EXP(wsi);


	store_min_max_senses_years_from_CONDITIONAL_EXP(wsi);



	/*
	 * at this stage we have our data on consecutive years as we want to exclude the years that we introduced and we have no data
	 * for them. These years will be considered as missing years from the data. Please note that different WSD encounter different
	 * years of missing data, as well as different missing data for some senses BUT also please take into account that
	 * the neologistic pattern follow zero data at the start.
	 *
	 */


	fprintf(stderr, "DEBUGP: the value of the array %p and the address is %p\n", wsi->is_valid_year, &wsi->is_valid_year);
	lable_years_with_certain_value_EXP(wsi, &(wsi->is_valid_year), 0);





	if(thershold_exp!=0){
		wsi->size_of_threshold_data	=filter_minFreq_by_YEAR_EXP(wsi,thershold_exp);

	}

	years->clear();
	delete(years);

	return wsi;
}



////NOTUSED
//void filter_senses_years_counts_by_threshold(WSIEMERGENCE *wsi, int year)
//{
//	for(int sense=0; sense<wsi->num_senses;sense++){
//
//		wsi->senses_years_counts[year][sense]=0;
//	}
//}

void print_toFile_label_years_with_certain_value_EXP(WSIEMERGENCE *wsi, int *is_included, int value_to_filter, char const *dirname_filtered)
{
	createDirSafely(dirname_filtered);

	char fname[500];
	strcpy(fname,dirname_filtered);
	strcat(fname,"/");
	strcat(fname,wsi->wsdname);
	strcat(fname,"_FILTERED_");
	char num[100];
	sprintf(num, "%d",value_to_filter);
	strcat(fname,num);
	strcat(fname,".tsv");


	fprintf(stderr, "the name of the  file %s\n", fname);
	FILE *fl=NULL;
	fl= fopen(fname, "w");

	if(fl==NULL){
		fprintf(stderr, "ERROR OPENING FILE FOR _FILTERED_%d !\n",value_to_filter);
		exit(1);
	}


	fprintf(fl,"YEAR\t");
	for(set<string>::iterator itrsen= wsi->senseorder->begin(); itrsen!=wsi->senseorder->end(); itrsen++){
		fprintf(stderr,"%s\t", (*itrsen).c_str());
		fprintf(fl,"%s\t", (*itrsen).c_str());
	}
	fprintf(fl,"\n");

	//int minyear=wsi->minyear;


	//	for(int c=wsi->minyear; c<= wsi->maxyear ; c++){
	//		fprintf(stderr, "DEBUG: valid year %d value= %d\n", c, is_included[c-wsi->minyear]);
	//	}


	for(int i=wsi->minyear; i<= wsi->maxyear ; i++){

		if(is_included[i-wsi->minyear]==1){

			fprintf(fl,"%d\t", i);

			for(int j=0; j<wsi->num_senses; j++){

				//fprintf(stderr,"senseloop%d    %d\t",j, wsi->senses_years_counts[i-wsi->minyear][j]);

				fprintf(fl,"%d\t", wsi->senses_years_counts[i-wsi->minyear][j]);

			}
			fprintf(fl,"\n");
		}
	}

	fclose(fl);
}


void print_sense_given_year_FILTERED_threshold_matrix_EXP(WSIEMERGENCE *wsi, int *is_FilteredorValid_year, unsigned int thershold_exp, char const *dirname_FiltredConProbabilities)
{
	createDirSafely(dirname_FiltredConProbabilities);

	char num[100];
	char fname[500];
	strcpy(fname,dirname_FiltredConProbabilities);
	strcat(fname,"/");
	strcat(fname,wsi->wsdname);
	strcat(fname,"_threshold_");
	sprintf(fname, "%d",thershold_exp);
	strcat(fname,"_SENSE_GIVEN_YEAR");
	strcat(fname,".tsv");


	fprintf(stderr, "the name of the  file %s\n", fname);
	FILE *fl=NULL;
	fl= fopen(fname, "w");

	if(fl==NULL){
		fprintf(stderr, "ERROR OPENING FILE FOR threshold_SENSE_GIVEN_YEAR!\n");
		exit(1);
	}


	fprintf(fl, "YEAR\t");
	for(set<string>::iterator itrsen= wsi->senseorder->begin(); itrsen!=wsi->senseorder->end(); itrsen++){
		//fprintf(stderr,"%s\t", (*itrsen).c_str());
		fprintf(fl,"%s\t", (*itrsen).c_str());
	}
	fprintf(fl,"\n");


	for(int i=wsi->minyear; i<= wsi->maxyear ; i++){

		if(is_FilteredorValid_year[i-wsi->minyear]==1){

			fprintf(fl,"%d\t", i);

			for(int j=0; j<wsi->num_senses; j++){

				fprintf(fl, "%Lf\t",wsi->sense_given_year[i-wsi->minyear][j]);
			}
			fprintf(fl,"\n");
		}
	}

	fclose(fl);

}


// lastYear_tobe_used: example 2018 (as real year, not index)
// return the first year (as real year, not index) of the consecutive period with at least threshold_exprmnt instances each year
unsigned int get_start_consecutive_years(WSIEMERGENCE *wsi, unsigned int threshold_exprmnt, unsigned int lastYear_tobe_used){



	//unsigned int SIZE_datafoundwiththreshold=0;
	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;
	unsigned int lastYearIndex_tobe_used = lastYear_tobe_used - wsi->minyear;

	//wsi->is_filtered_year=(int *)malloc(numofyears*sizeof(int));

	int year=lastYearIndex_tobe_used;
	while (year>=0) {

		if(wsi->coutsallsenses_by_YEAR[year] < threshold_exprmnt){
			return year+wsi->minyear;
		}
		year--;
	}

	return wsi->minyear;


}



//check if this year taken into account or not when you give a threshold value and return the size of the data, it will
//return 0 if no data found, otherwise the size of the data found.
unsigned int filter_minFreq_by_YEAR_EXP(WSIEMERGENCE *wsi, unsigned int threshold_exprmnt)
{

	unsigned int SIZE_datafoundwiththreshold=0;
	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;

	//wsi->is_filtered_year=(int *)malloc(numofyears*sizeof(int));

	for(int year=0; year<numofyears; year++){

		if(wsi->coutsallsenses_by_YEAR[year] >= threshold_exprmnt){

			SIZE_datafoundwiththreshold++;
			lable_years_with_certain_value_EXP(wsi, &(wsi->is_filtered_year), threshold_exprmnt);



			//////wsi->coutsallsenses_by_YEAR[year] =0;

			fprintf(stdout,"WARNING: %s removing year %d\n", wsi->wsdname, (wsi->minyear+year));
		}

		//filter_senses_years_counts_by_threshold(wsi,year);
	}

	return SIZE_datafoundwiththreshold;
}



/*
 * create an array for WSI to store the frequency of all the senses by year
 */

void generate_total_byYear_EXP(WSIEMERGENCE *wsi)
{

	unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;
	wsi->coutsallsenses_by_YEAR=(int *)calloc((numofyears+1),sizeof(int));

	int sense=0;

	for(int year=0 ;year<numofyears; year++){
		unsigned int tot_senses_by_YEAR=0;
		for(int sense=0; sense<wsi->num_senses;sense++){
			//fprintf(stderr, "DEBUG: wsi->senses_years_counts[year][sense]%d\n",wsi->senses_years_counts[year][sense]);
			tot_senses_by_YEAR += wsi->senses_years_counts[year][sense];
		}
		wsi->coutsallsenses_by_YEAR[year]=tot_senses_by_YEAR;
	}

	//	for(int y=wsi->minyear ;y<=wsi->maxyear; y++){
	//		fprintf(stderr,"DEBUG: wsi->coutsallsenses_by_YEAR[%d]= %d\n",y,wsi->coutsallsenses_by_YEAR[y-wsi->minyear]);
	//	}
}





void store_WSIEMEREGENCE_map_EXP(WSIEMERGENCE *WSICONTENT, map<string, WSIEMERGENCE* > *WSI_EMERGENCE_INFO)
{

	map<string, WSIEMERGENCE *>::iterator itrmp;
	itrmp =WSI_EMERGENCE_INFO->find(string(WSICONTENT->wsdname));
	if(itrmp == WSI_EMERGENCE_INFO->end()){

		WSI_EMERGENCE_INFO->insert({string(WSICONTENT->wsdname),WSICONTENT});

	}else{
		fprintf(stderr, "WARNING: WE SHOULD ONLY HAVE THE WSI ONCE IN THE MAP\n");
		exit(1);
	}

}




FILE *write_to_FILE_EXP(char const *dirname_allemergence, char const *colonames, char const *filename ){

	createDirSafely(dirname_allemergence);

	int l=strlen(dirname_allemergence);
	int l2=strlen(filename);

	char *fname=(char *)malloc((l+l2+10)*sizeof(char));
	strcpy(fname,dirname_allemergence);
	strcat(fname,"/");
	strcat(fname,filename);
	strcat(fname,".tsv");


	//fprintf(stderr, "the name of the  file %s\n", fname);

	FILE *fl=NULL;
	fl= fopen(fname, "w");

	if(fl==NULL){
		fprintf(stderr, "ERROR OPENING FILE FOR %s!\n", filename);
		exit(1);
	}

	if(colonames!=NULL){
		fprintf(fl,"%s",colonames);

		fprintf(fl,"\n");
	}
	return fl;

}







void detect_baseline_Emergence_EXP(WSIEMERGENCE *wsi, int timeInterval_from_start)
{
	map<string, SENSEEMERGENCE*>::iterator itrseninfo;

	for(int i=0; i< wsi->size_sortedyear; i++){

		for(int j=0; wsi->sensesOrderByStartYear[i][j]!=NULL; j++){

			map<string, SENSEEMERGENCE*>::iterator itrseninfo= wsi->senses_min_max->find(string(wsi->sensesOrderByStartYear[i][j]));

			if(itrseninfo!= wsi->senses_min_max->end()){

				SENSEEMERGENCE *senseinfo =itrseninfo->second;

				fprintf(stderr,"%s(%d)\t", wsi->sensesOrderByStartYear[i][j], senseinfo->s_min_year_counts);

			}
		}
	}



	map<string, SENSEEMERGENCE *>::iterator itr;

	int matrixI= 0;

	for(int syear=1; syear < wsi->size_sortedyear; syear++){

		for(int sensOne=0; wsi->sensesOrderByStartYear[syear][sensOne]!=NULL; sensOne++){

			itr= wsi->senses_min_max->find(string(wsi->sensesOrderByStartYear[syear][sensOne]));

			string senseID	=itr->first;
			SENSEEMERGENCE *senseinfone=  itr->second;

			int matrixJ= 0;

			if(wsi->diff_senses_firstyears[0][senseinfone->order_diff_table] >= timeInterval_from_start){

				senseinfone->Senseemerged_BLprameter=1;
				wsi->wsdEmerged_BLparamter=1;

			}else{
				senseinfone->Senseemerged_BLprameter=0;
			}
		}
	}

}





void print_OnlyDetected_BLemergence_EXP(WSIEMERGENCE *wsi, FILE *BLTrueonly)
{

	if(wsi->wsdEmerged_BLparamter!=0){
		map<string, SENSEEMERGENCE *>::iterator itr;
		for(int syear= 0; syear<wsi->size_sortedyear; syear++){

			for(int sensOne= 0; wsi->sensesOrderByStartYear[syear][sensOne]!=NULL; sensOne++){

				itr= wsi->senses_min_max->find(string(wsi->sensesOrderByStartYear[syear][sensOne]));

				string senseID= itr->first;
				SENSEEMERGENCE *senseinfone=  itr->second;

				fprintf(BLTrueonly, "%s\t", wsi->wsdname);

				fprintf(BLTrueonly,"%d\t", wsi->wsdEmerged_BLparamter);
				fprintf(BLTrueonly,"%s\t", senseID.c_str());

				fprintf(BLTrueonly,"%d\t", senseinfone->Senseemerged_BLprameter);

				fprintf(BLTrueonly,"%d\n", senseinfone->s_min_year_counts);

			}
		}
	}
}







void print_detectedORnot_BLemergence_EXP(WSIEMERGENCE *wsi, FILE *BLfile)
{
	map<string, SENSEEMERGENCE *>::iterator itr;

	for(int syear= 0; syear<wsi->size_sortedyear; syear++){

		for(int sensOne= 0; wsi->sensesOrderByStartYear[syear][sensOne]!=NULL; sensOne++){

			itr= wsi->senses_min_max->find(string(wsi->sensesOrderByStartYear[syear][sensOne]));

			string senseID= itr->first;
			SENSEEMERGENCE *senseinfone=  itr->second;


			fprintf(BLfile, "%s\t",wsi->wsdname);

			fprintf(BLfile,"%d\t",wsi->wsdEmerged_BLparamter);
			fprintf(BLfile,"%s\t",senseID.c_str());

			fprintf(BLfile,"%d\t",senseinfone->Senseemerged_BLprameter);

			fprintf(BLfile,"%d\n",senseinfone->s_min_year_counts);

		}

	}
}













void create_RankedBLYearsDIFF_table_EXP(WSIEMERGENCE *wsi, char ***sensesorder)
{

	puts("Im inside create_BLYearsDIFF_table\n");

	wsi->diff_senses_firstyears=(long int **)malloc((wsi->num_senses)*sizeof(long int *));
	map<string, SENSEEMERGENCE *>::iterator itr;


	for(int j=0; j<wsi->num_senses; j++){
		wsi->diff_senses_firstyears[j]=(long int *)malloc((wsi->num_senses)*sizeof(long int ));
	}






	int matrixI=0;
	for(int syear=0; syear<wsi->size_sortedyear; syear++){

		for(int sensOne=0; wsi->sensesOrderByStartYear[syear][sensOne]!=NULL; sensOne++){

			itr= wsi->senses_min_max->find(string(sensesorder[syear][sensOne]));

			string senseID	=itr->first;
			SENSEEMERGENCE *senseinfone=  itr->second;
			int matrixJ=0;

			for(int syear=0; syear<wsi->size_sortedyear; syear++){

				for(int sensTwo=0; wsi->sensesOrderByStartYear[syear][sensTwo]!=NULL; sensTwo++){

					itr= wsi->senses_min_max->find(string(sensesorder[syear][sensTwo]));
					string senseID	=itr->first;
					SENSEEMERGENCE *senseinftwo=  itr->second;

					wsi->diff_senses_firstyears[matrixI][matrixJ]=abs(senseinfone->s_min_year_counts-senseinftwo->s_min_year_counts);
					fprintf(stderr,"m[%d,%d] = %ld\n", matrixI, matrixJ,wsi->diff_senses_firstyears[matrixI][matrixJ]);
					matrixJ++;
				}
			}
			matrixI++;
		}
	}
}





void print_toFile_RankedBLYearsDIFF_EXP(WSIEMERGENCE *wsi, FILE *wf)
{

	puts("im inside print_toFile_RankedBLYearsDIFF\n");
	map<string, SENSEEMERGENCE *>::iterator itr;

	fprintf(stderr,"%s(%d)(%dTO%d)\t", wsi->wsdname, wsi->num_senses, wsi->minyear, wsi->maxyear);


	fprintf(wf,"%s(%d)(%dTO%d)\t", wsi->wsdname, wsi->num_senses, wsi->minyear, wsi->maxyear);



	//	for(int print=0; print<wsd->size_sortedyear; print++){
	//			for(int print2=0; wsd->sensesOrderByStartYear[print][print2]!=NULL; print2++){
	//				fprintf(stderr, "DEBUG: %s\n",wsd->sensesOrderByStartYear[print][print2]);
	//			}
	//		}

	//int order=0;
	for(int i=0; i< wsi->size_sortedyear; i++){

		for(int j=0; wsi->sensesOrderByStartYear[i][j]!=NULL; j++){

			map<string, SENSEEMERGENCE*>::iterator itrseninfo= wsi->senses_min_max->find(string(wsi->sensesOrderByStartYear[i][j]));

			if(itrseninfo!= wsi->senses_min_max->end()){

				SENSEEMERGENCE *senseinfo =itrseninfo->second;

				fprintf(stderr,"%s(%d)\t", wsi->sensesOrderByStartYear[i][j], senseinfo->s_min_year_counts);


				//wsi->order_diff_table=;

				fprintf(wf,"%s(%d)\t", wsi->sensesOrderByStartYear[i][j], senseinfo->s_min_year_counts);
			}
		}
	}

	fprintf(wf,"\n");


	create_RankedBLYearsDIFF_table_EXP(wsi, wsi->sensesOrderByStartYear);// debug it

	int matrixI=0;

	for(int syear=0; syear<wsi->size_sortedyear; syear++){

		for(int sensOne=0; wsi->sensesOrderByStartYear[syear][sensOne]!=NULL; sensOne++){
			itr=wsi->senses_min_max->find(string(wsi->sensesOrderByStartYear[syear][sensOne]));
			string senseID	=itr->first;
			SENSEEMERGENCE *senseinfone=  itr->second;

			fprintf(stderr,"%s(%d)\t",wsi->sensesOrderByStartYear[syear][sensOne],senseinfone->s_min_year_counts);
			fprintf(wf,"%s(%d)\t",wsi->sensesOrderByStartYear[syear][sensOne],senseinfone->s_min_year_counts);



			int matrixJ=0;

			for(int syear=0; syear<wsi->size_sortedyear; syear++){

				for(int sensTwo=0; wsi->sensesOrderByStartYear[syear][sensTwo]!=NULL; sensTwo++){


					fprintf(stderr,"%ld\t",wsi->diff_senses_firstyears[matrixI][matrixJ]);

					fprintf(wf,"%ld\t",wsi->diff_senses_firstyears[matrixI][matrixJ]);
					matrixJ++;
				}
			}
			fprintf(wf,"\n");
			matrixI++;

		}

	}


}

//void print_toFile_BLYearsDIFF(WSIEMERGENCE *wsi, FILE *wf)
//{
//
//	puts("IM inside print_toFile_BLYearsDIFF!\n");
//
//	char **sensestrings= get_sensesstrings_in_order(wsi);
//
//	//char **sensesinorder =get_sensesstrings_in_order(wsi);
//	//unsigned int numofyears= (wsi->maxyear-wsi->minyear)+1;// its not the difference between years its the number of years
//
//
//
//	map<string, SENSEEMERGENCE *>::iterator itr;
//
//	fprintf(wf,"%s(%d)(%dTO%d)\t", wsi->wsdname, wsi->num_senses, wsi->minyear, wsi->maxyear);
//
//	for(set<string>::iterator itrsen= wsi->senseorder->begin(); itrsen!=wsi->senseorder->end(); itrsen++){
//
//		//fprintf(stderr,"%s\t", (*itrsen).c_str());
//
//		map<string, SENSEEMERGENCE*>::iterator itrseninfo= wsi->senses_min_max->find(string((*itrsen).c_str()));
//
//		if(itrseninfo!= wsi->senses_min_max->end()){
//
//			SENSEEMERGENCE *senseinfo =itrseninfo->second;
//
//			fprintf(wf,"%s(%d)\t", (*itrsen).c_str(), senseinfo->s_min_year_counts);
//
//		}
//	}
//
//	fprintf(wf,"\n");
//
//
//	create_BLYearsDIFF_table(wsi, sensestrings);
//
//
//
//	for(int a=0; a< (wsi->num_senses); a++){
//
//		itr=wsi->senses_min_max->find(string(sensestrings[a]));
//		string senseID	=itr->first;
//		SENSEEMERGENCE *senseinfone=  itr->second;
//		fprintf(wf,"%s(%d)\t",sensestrings[a],senseinfone->s_min_year_counts);
//
//		for(int b=0; b< wsi->num_senses; b++){
//
//			fprintf(wf,"%ld\t",wsi->diff_senses_firstyears[a][b]);
//
//		}
//		fprintf(wf,"\n");
//	}
//
//
//	//	for(int a=0; a< (wsi->num_senses); a++){
//	//
//	//		itr=wsi->senses_min_max->find(string(sensestrings[a]));
//	//		string senseID	=itr->first;
//	//		SENSEEMERGENCE *senseinfone=  itr->second;
//	//
//	//
//	//		fprintf(wf,"%s(%d)\t",sensestrings[a],senseinfone->s_min_year_counts);
//	//
//	//
//	//		for(int b=0; b< wsi->num_senses; b++){
//	//
//	//			itr=wsi->senses_min_max->find(string(sensestrings[b]));
//	//			string senseID	=itr->first;
//	//			SENSEEMERGENCE *senseinftwo=  itr->second;
//	//
//	//
//	//			fprintf(wf,"%d\t" ,(senseinfone->s_min_year_counts-senseinftwo->s_min_year_counts));
//	//
//	//		}
//	//		fprintf(wf,"\n");
//	//	}
//
//}







void print_toFile_allemergence_year_EXP(WSIEMERGENCE *wsi, FILE *writefile, map<int, set<int> *> *wsi_surges)
{

	puts("IM inside print_toFile_allemergence_year!\n");


	for(int s=0 ;s<wsi->num_senses; s++){
		fprintf(stdout,"PICK:%d",wsi->emergenceyearsbysenses[s]);
	}


	for(int s=0 ;s<wsi->num_senses; s++){

		if(wsi->emergenceyearsbysenses[s]!=0){

			fprintf(writefile,"%s" ,wsi->wsdname);
			fprintf(writefile,"\t");

			fprintf(writefile,"%d" ,wsi->num_senses);
			fprintf(writefile,"\t");

			char *sensestring= get_sensestring_from_index_EXP(wsi,s);

			char **sensestrings= get_sensesstrings_in_order_EXP(wsi);

			fprintf(writefile,"%s" ,sensestring);
			fprintf(writefile,"\t");

			fprintf(writefile,"%d" ,wsi->emergenceyearsbysenses[s]);
			fprintf(writefile,"\t");

			fprintf(writefile, "%Lf", wsi->sense_given_year[(wsi->emergenceyearsbysenses[s]-wsi->minyear)][s]);
			fprintf(writefile,"\t");



			for(map<int, set<int> *>::iterator itrmap= wsi_surges->begin(); itrmap!= wsi_surges->end(); itrmap++ ){

				if((itrmap->first) == s){

					for(set<int>::iterator itrset = itrmap->second->begin();itrset!= itrmap->second->end(); itrset++){


						unsigned int yearindex= *itrset;
						unsigned int yearvalue =yearindex+ wsi->minyear;

						fprintf(writefile, "%d", yearvalue);
						fprintf(writefile,"|");

					}
				}
			}

			fprintf(writefile,"\t");

			map<string, SENSEEMERGENCE*>::iterator itrseninfo= wsi->senses_min_max->find(string(sensestrings[s]));

			if(itrseninfo!= wsi->senses_min_max->end()){

				SENSEEMERGENCE *senseemr =itrseninfo->second;

				fprintf(writefile,"%d\t",senseemr->s_min_year_counts);


				fprintf(writefile,"%d\t" ,wsi->emergenceyearsbysenses[s]-senseemr->s_min_year_counts);



			}else{
				fprintf(stderr, "ERROR: this sense must be found!");
				exit(1);
			}


			fprintf(writefile,"\n");
			//free dont forget it

		}
	}
}







void print_toFile_StartYearOCCURENCEONLY_EXP(WSIEMERGENCE *wsi, FILE *writefile_startyear)
{

	//fprintf(fl,"WSD_INFORMATION\tNUM_SENSES\tSENSE\tFIRST_YEAR_OCCURENCE\tVALUE_FIRST_YEAR_OCCURENCE\tMaxSenseGivenYear\tMaxSenseGivenYearValue\n");

	char **sensestrings= get_sensesstrings_in_order_EXP(wsi);


	for(int s=0 ;s<wsi->num_senses; s++){

		fprintf(writefile_startyear,"%s", wsi->wsdname);
		fprintf(writefile_startyear,"\t");

		fprintf(writefile_startyear,"%d", wsi->num_senses);
		fprintf(writefile_startyear,"\t");

		fprintf(writefile_startyear,"%s",sensestrings[s]);
		fprintf(writefile_startyear,"\t");



		map<string, SENSEEMERGENCE*>::iterator itrseninfo= wsi->senses_min_max->find(string(sensestrings[s]));

		if(itrseninfo!= wsi->senses_min_max->end()){

			SENSEEMERGENCE *senseemr =itrseninfo->second;

			fprintf(writefile_startyear,"%d",senseemr->s_min_year_counts);
			fprintf(writefile_startyear,"\t");
			fprintf(writefile_startyear,"%d",senseemr->s_min_year_counts_value);
			fprintf(writefile_startyear,"\t");
			fprintf(writefile_startyear,"%d",senseemr->s_max_sense_given_year_year);
			fprintf(writefile_startyear,"\t");
			fprintf(writefile_startyear,"%Lf",senseemr->s_max_sense_given_year_value);
			fprintf(writefile_startyear,"\t");
		}else{

			fprintf(stderr, "ERROR: this sense must be found!");
			exit(1);

		}

		fprintf(writefile_startyear,"\n");

	}
}



/*
 * Read WSI data (from WSIPUBMED) for each ambiguous word
 * The thershold_exp value is the minimum value (number of citations in a year) that we can have in our data.
 * it returns a map of each wsi and its counts
 *
 * it also creates a file counting all the emergence cases with thier details and the non emergence cases.
 */
//map<string, WSIEMERGENCE *> *read_WSI(char const *dirname, char const *dirname_print,
//		unsigned int thershold_exp, char const *dirname_sense_given_year,char const *dirname_valid, char const *dirname_filtered){

map<string, WSIEMERGENCE *> *read_WSI_EXP(char const *dirname, char const *dirname_counts,
		unsigned int thershold_exp, unsigned int windowsize_exp, char const *dirname_sense_given_year, char const *dirname_valid,
		char const *dirname_filtered, char const *dirname_thresholdConProbabilities,char const *dirname_emergence, char const *dirname_allemergence, char const *dirname_senseinfo){



	//	fprintf(stderr,
	//	"DEBUGF: Im read_WSI and the parameters for the directories: dirname_counts\t%s\n,dirname_sense_given_year\t%s\n,dirname_valid\t%s\n,dirname_filtered\t%s\n,dirname_thresholdConProbabilities\t%s\n",
	//					dirname_counts, dirname_sense_given_year,dirname_valid, dirname_filtered,
	//					dirname_thresholdConProbabilities);

	const char **dircontent = read_dir_Targetscontent_EXP(dirname);

	//puts("DEBUG: IM PRINTING THE DIRE ARRAY FILES\n");
	//print_stringarray(dircontent);

	map<string, WSIEMERGENCE *> *WSI_EMERGENCE_INFO = new map<string, WSIEMERGENCE *>;

	//to create two files to store emergence info for all the wsi together
	FILE *writefile= write_to_FILE_EXP(dirname_allemergence, colonames_allemer, "ALL_EMERGENCE");
	FILE *wfile_firstYear= write_to_FILE_EXP(dirname_allemergence, colonames_firstYears,"ALL_EMERGENCE_FIRST_YEAR_OCCURENCE");
	FILE *wrfl= write_to_FILE_EXP(dirname_allemergence,NULL,"DIFFERENCE_FOREACHSENSE_FIRSTYEAR");
	FILE *wfble= write_to_FILE_EXP(dirname_allemergence,colonames_BL,"BASELINE_EMERGENCE_DETECTION");
	FILE *wfonly=write_to_FILE_EXP(dirname_allemergence,colonames_BL,"BASELINE_TRUEEMERGENCE_DETECTION");
	// FILE *sensesinfo=write_to_FILE_EXP(dirname_allemergence,colonames_SENSESINFO,"SensesInfoEmergence");


	//FILE *wrfl= write_to_FILE(dirname_allemergence, );

	for(int f=0; dircontent[f]!=NULL; f++){


		char ***targetContent= (char ***)malloc(block_size*sizeof(char **));
		targetContent[block_size-1]=NULL;

		fprintf(stderr, "DEBUG: %d: %s\n",f,dircontent[f]);


		FILE *fptr =NULL;
		fptr = fopen(dircontent[f],"r");

		if(fptr ==NULL){
			fprintf(stderr, "Error opening the file\n");
			exit(1);
		}


		puts("IM CALLING store_WSIDATA\n");
		unsigned int size= store_WSIDATA_EXP(dircontent[f], &targetContent);


		// if the file is not empty
		if(size>1){

			//		for(int kk=0; targetContent[kk]!=NULL; kk++){
			//		print_stringarray(targetContent[kk]);
			//		}


			puts("IM CALLING store_WSDContent\n");
			WSIEMERGENCE *WSICONTENT= store_WSDContent_EXP(targetContent, size, thershold_exp);



			puts("IM CALLING store_WSIEMEREGENCE_map\n");
			store_WSIEMEREGENCE_map_EXP(WSICONTENT,WSI_EMERGENCE_INFO);


			puts("IM CALLING print_generate_counts_bySenseandYear\n");
			print_generate_counts_bySenseandYear_EXP(WSICONTENT, dirname_counts);






			puts("IM CALLING print_sense_given_year_matrix\n");
			print_sense_given_year_matrix_EXP(WSICONTENT, dirname_sense_given_year);


			puts("IM CALLING print_toFile_label_years_with_certain_value\n");
			print_toFile_label_years_with_certain_value_EXP(WSICONTENT, WSICONTENT->is_valid_year, 0, dirname_valid);


			if(thershold_exp!=0 && WSICONTENT->size_of_threshold_data!=0){
				puts("IM CALLING print_toFile_label_years_with_certain_value\n");
				print_toFile_label_years_with_certain_value_EXP(WSICONTENT, WSICONTENT->is_filtered_year, thershold_exp, dirname_filtered);
				puts("IM CALIING print_sense_given_year_FILTERED_threshold_matrix\n");
				print_sense_given_year_FILTERED_threshold_matrix_EXP(WSICONTENT, WSICONTENT->is_filtered_year, thershold_exp, dirname_thresholdConProbabilities);

			}

			puts("IM CALLING detect_WSI_SENSE_GIVEN_YEAR_EMERGENCE\n");


			//exit(1);


			//binary vector add it
			map<int, set<int> *> *wsi_surges= detect_WSI_SENSE_GIVEN_YEAR_EMERGENCE_EXP(WSICONTENT, NULL, 0,windowsize_exp);



			for(int s=0 ;s<WSICONTENT->num_senses; s++){
				fprintf(stdout,"PICKBEFORE:%d",WSICONTENT->emergenceyearsbysenses[s]);
			}


			print_toFile_senseinfo_emergence_year_EXP(WSICONTENT,dirname_emergence);


			for(int s=0 ;s<WSICONTENT->num_senses; s++){
				fprintf(stdout,"PICKOUT:%d",WSICONTENT->emergenceyearsbysenses[s]);
			}

			fprintf(stdout,"PICKOUT: IM CALIING print_toFile_allemergence_year\n");


			print_toFile_allemergence_year_EXP(WSICONTENT, writefile, wsi_surges);

			print_toFile_StartYearOCCURENCEONLY_EXP(WSICONTENT, wfile_firstYear);

			//**************************************************************************************//



			rank_baseline_Sensestartyears_EXP(WSICONTENT);


			print_toFile_RankedBLYearsDIFF_EXP(WSICONTENT, wrfl);

			//print_toFile_BLYearsDIFF(WSICONTENT, wrfl);


			detect_baseline_Emergence_EXP(WSICONTENT, 5);

			print_detectedORnot_BLemergence_EXP(WSICONTENT,wfble);

			print_OnlyDetected_BLemergence_EXP(WSICONTENT, wfonly);

			//wrongprint_toFile_Filtred_byStartDate(WSICONTENT,wrfl);



			print_toFile_allemergence_Sense_EXP(WSICONTENT, colonames_SENSESINFO,wsi_surges,dirname_senseinfo);


		}else{
			fprintf(stderr,"WARNING: THERE IS NO DATA AT ALL IN WSDFILE %s\n",dircontent[f]);
		}

		/*
		 * To free
		 */
		for(int f=0; f<size; f++){
			free_double_dimentions((void **)targetContent[f]);
		}
		free(targetContent);
		fclose(fptr);
	}


	fclose(writefile);
	fclose(wfile_firstYear);
	fclose(wrfl);
	fclose(wfble);
	fclose(wfonly);

	//can I free dir content one by one whithin the loop
	//	for(int dr=0; dircontent[dr]!=NULL; dr++){
	//		free_double_dimentions((void **)dircontent[dr]);
	//	}
	//	free(dircontent);


	return WSI_EMERGENCE_INFO;
}



/*
 * the following method is to read the actual of a single wsi data and return the size of the data including the raw of the columns information
 */
unsigned int store_WSIDATA_EXP(char const *dircontent, char ****targetContent){


	char buff[100000];

	FILE *fptr =NULL;
	fptr = fopen(dircontent,"r");

	if(fptr ==NULL){
		fprintf(stderr, "Error opening the file\n");
		exit(1);
	}

	int j=0;
	while(fgets(buff, 100000, fptr)!=NULL){

		//fprintf(stderr, "DEBUG: buff %s \n",buff);


		if(((j+1)%block_size)==0){
			(*targetContent)= (char ***)realloc((*targetContent),(j+1+block_size)*sizeof(char **));
			//(*targetContent)[j+block_size]=NULL;WRONG CUZ SOMETIMES I DONT FILL THE WHOLE 10 BLOCKS

		}

		puts("in here \n");

		buff[strcspn(buff, "\r\n")] = '\0';
		char **WSItokns = tokenise_opt2(buff,"	");


		//		for(int i=0; WSItokns[i]!=NULL; i++){
		//			printf("the %d string from the WSI_GOLD is %s\n",i, WSItokns[i]);
		//		}

		(*targetContent)[j]= WSItokns;
		j++;
		(*targetContent)[j]=NULL;

	}

	fprintf(stderr, "the size of j %d\n", j);

	return j;

}



void print_WSI_EMERGENCE_INFO_EXP(map<string, WSIEMERGENCE *> *WSI_EMERGENCE_INFO,char const *dirname_print_map){


	createDirSafely(dirname_print_map);

	char fname[500];
	strcpy(fname,dirname_print_map);
	strcat(fname,"/");
	strcat(fname,"total_couts_per_sense");
	strcat(fname,".tex");


	fprintf(stderr, "the name of the  file %s\n", fname);
	FILE *fl=NULL;
	fl= fopen(fname, "w");

	if(fl==NULL){
		fprintf(stderr, "ERROR OPENING FILE FOR total_couts_per_sense!\n");
		exit(1);
	}

	int size=1;// increase by every time we realloc
	int prnt=0;// number of elements

	int *sensescounts=(int *)calloc((size*block_size_print),sizeof(int));
	char **sensesnames=(char **)malloc((size*block_size_print)*sizeof(char *));
	char **sensesWDS=(char **)malloc((size*block_size_print)*sizeof(char *));


	map<string, WSIEMERGENCE *>::iterator itrwsi;
	map<string, int>::iterator itrsen;

	for(itrwsi=WSI_EMERGENCE_INFO->begin(); itrwsi!=WSI_EMERGENCE_INFO->end(); itrwsi++){

		WSIEMERGENCE *wsinfo= itrwsi->second;

		map<string, int> *sensecounts= wsinfo->sensescounts;


		for(itrsen=sensecounts->begin(); itrsen!=sensecounts->end(); itrsen++){
			fprintf(stderr, "DEBUG: %s %s\n", itrwsi->first.c_str(), itrsen->first.c_str());

			if(prnt==size*block_size_print){
				size++;
				sensescounts=(int *)realloc(sensescounts,(size*block_size_print)*sizeof(int));
				sensesnames=(char **)realloc(sensesnames,(size*block_size_print)*sizeof(char *));
				sensesWDS=(char **)realloc(sensesWDS,(size*block_size_print)*sizeof(char *));
				//	printf("DEBUG: block size added new allocated size=%ld\n",size*block_size_print);
			}

			int senlen =strlen(itrsen->first.c_str());
			int wsdlen =strlen(itrwsi->first.c_str());
			sensesnames[prnt]=(char *)malloc((senlen+1) *sizeof(char));
			strcpy(sensesnames[prnt],itrsen->first.c_str());

			//fprintf(stderr, "DEBUG: the name is %s\n", itrwsi->first.c_str());

			sensesWDS[prnt]=(char *)malloc((wsdlen+1) *sizeof(char));
			strcpy(sensesWDS[prnt],itrwsi->first.c_str());

			//fprintf(stderr, "DEBUG: sensesWDS[prnt] %s\n", sensesWDS[prnt]);

			//fprintf(stdout, "DEBUG: %s", sensesnames[prnt]);
			sensescounts[prnt]= itrsen->second;
			prnt++;
			//printf("DEBUG: element added new size=%d\n",prnt);

		}
	}


	if(block_size_print*size>prnt){
		sensescounts=(int *)realloc(sensescounts,(prnt)*sizeof(int));
		sensesnames=(char **)realloc(sensesnames,(prnt)*sizeof(char *));
		sensesWDS=(char **)realloc(sensesWDS,(prnt)*sizeof(char *));
	}



	int nbCols = 4;
	int nbLines = prnt / nbCols;
	if (nbLines *nbCols < prnt) {
		nbLines++;
	}

	for(int i=0; i<nbLines; i++){

		for (int bigCol=0; bigCol<nbCols; bigCol++) {
			int originalIndex = bigCol * nbLines + i;
			//printf("DEBUG: %d\n",originalIndex);
			if (bigCol>0) { // to avoid printing & for the first column
				fprintf(fl,"&");
			}

			if (originalIndex < prnt) {
				fprintf(fl,"%s\t",sensesnames[originalIndex]);
				fprintf(fl,"&%d\t",(sensescounts[originalIndex]));
				fprintf(fl,"&%s\t",(sensesWDS[originalIndex]));
			}

			//			if (bigCol<2) { // to avoid printing & for the first column
			//				fprintf(fl,"&");
			//			} else {
			//			}

		}
		fprintf(fl,"\\\\\n");

	}

	fclose(fl);


	for(int fr=0; fr<prnt;fr++){
		free(sensesnames[fr]);
		free(sensesWDS[fr]);
	}
	free(sensesnames);
	free(sensesWDS);
	free(sensescounts);
}





//we need to read the data first and then we store everything accordingly.
//seperate the data according to their senses
char **** read_WSI_Targets_EXP(char const  *dirname){

	//READ THE DIR HERE!
	const char **dircontent = read_dir_Targetscontent_EXP(dirname);

	char **targetNames=(char **)malloc(block_size*sizeof(char *));
	char ****targetContent= (char ****)malloc(block_size*sizeof(char ***));



	for(int f=0; dircontent[f]!=NULL; f++){


		fprintf(stderr, "%d: %s\n",f,dircontent[f]);

		char buff[10000];
		FILE *fptr =NULL;
		fptr = fopen(dircontent[f],"r");

		if(fptr ==NULL){
			fprintf(stderr, "Error opening the file\n");
			exit(1);
		}


		char const *nametsv =strrchr(dircontent[f],'/');
		char const *tsv =strrchr(nametsv+1, '.');
		int length =tsv-(nametsv+1);


		if(((f+1)%block_size)!=0){

			targetContent[f]= (char ***)malloc(sizeof(char **));
			targetNames[f]=(char *)malloc((length+1)*sizeof(char));

		}else{
			targetContent=(char ****)realloc(targetContent,(f+1+block_size)*sizeof(char ***));
			targetContent[f]= (char ***)malloc(sizeof(char **));


			targetNames=(char **)realloc(targetNames,(f+1+block_size)*sizeof(char*));
			targetNames[f]=(char *)malloc((length+1)*sizeof(char));

		}


		strncpy(targetNames[f], nametsv+1,length);
		targetNames[f][length]='\0';
		fprintf(stderr, "%s the last result\n ", targetNames[f]);


		int j=0;
		while(fgets(buff, 10000, fptr)!=NULL){


			buff[strcspn(buff, "\r\n")] = '\0';
			char **WSItokns = tokenise_opt2(buff,"	");
			for(int i=0; WSItokns[i]!=NULL; i++){
				printf("the %d string from the WSI_GOLD is %s\n",i, WSItokns[i]);
			}

			targetContent[f][j]= WSItokns;
			j++;

			targetContent[f]= (char ***)realloc(targetContent[f],(j+1)*sizeof(char **));
			targetContent[f][j]=NULL;


		}

	}

	return targetContent;

}
