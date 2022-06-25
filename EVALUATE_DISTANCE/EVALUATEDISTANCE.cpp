/*
 * EVALUATEDISTANCE.cpp
 *
 *  Created on: 8 May 2020
 *      Author: alsulaia
 */

#include "EVALUATEDISTANCE.h"

//EVALUATE_DISTANCE::EVALUATE_DISTANCE() {
//	// TODO Auto-generated constructor stub
//
//}
//
//EVALUATE_DISTANCE::~EVALUATE_DISTANCE() {
//	// TODO Auto-generated destructor stub
//}


void calculate_euclideanDistance( char ***goldData,  char ***predData, unsigned int sizeData, char *resultsPath, char *target, FILE*FP){


	//int senseindex=-1;
	FILE	*fp;
	const char *filename="ecludeDis";
	int len=strlen(resultsPath);
	int len2 =strlen(filename);
	char *dirnamepath= (char *)malloc((len2+len+10)*sizeof(char));
	strcpy(dirnamepath,resultsPath);
	strcat(dirnamepath,"/");
	strcat(dirnamepath, filename);
	strcat(dirnamepath,".tsv");
	fp = fopen(dirnamepath,"w");

	if(fp==NULL){
		//fprintf(stderr, "Path is %s", resultsPath);
		fprintf(stderr, "ERORR, opening file %s\n",dirnamepath);
		exit(1);
	}

	fprintf(fp, "target\tpred\tgold\teuclideanValue\tnormSensebyyear\n");


	for(int l=1; predData[0][l]!=NULL && (strcmp(predData[0][l], "WSD")!=0); l++){

		for(int k=1; goldData[0][k]!=NULL;  k++){


			fprintf(stderr, "PredSense %s\t",predData[0][l]);
			fprintf(stderr, "GoldSense %s\n",goldData[0][k]);

			double value =0.00;

			for(int i=0; i<sizeData; i++ ){

				char *ptrOne;
				char *ptrTwo;

				double	a1 = strtod(predData[i][l], &ptrOne);
				double	a2 = strtod(goldData[i][k], &ptrTwo);

				value =calculate_differenceSquared( a1,a2, value);

				// take the square root
				double disvalue=sqrt(value);

			}// end of for

			store_res_to_file(value,predData[0][l],goldData[0][k] ,fp, target, FP, sizeData);
		}
	}

	fclose(fp);
}





double calculate_differenceSquared( double a, double b, double presult ){
	double res = (double)a-(double)b;
	res=(double)res*(double)res;

	// (double)res =pow(res, 2);
	double	 result=presult+res;
	return  result;
}



void store_res_to_file(double disvalue, char *predSense, char *goldSense,FILE	*fp, char *target, FILE*FP, unsigned int sizeData){

	double normsense=(double)disvalue/(double)sizeData;
	fprintf(fp, "%s\t%s\t%s\t%lf\t%lf\n",target,predSense,goldSense, disvalue,normsense );


	fprintf(FP, "%s\t%s\t%s\t%lf\t%lf\n",target,predSense,goldSense, disvalue, normsense );



}


const char ** read_dir_content_FULL(const char *dirpath){

	DIR *d;
	struct dirent *dir;
	d = opendir(dirpath);

	if(d==NULL){

		fprintf(stderr,"Error! Unable to open the directory. %s \n", dirpath);
		exit(1);
	}

	char** dircontent = (char**)malloc(sizeof(char*));
	unsigned int index=0;
	while ((dir = readdir(d)) != NULL){//printf("%s\n", dir->d_name);// i dont need to use rewcursion!!

		if( (strcmp(dir->d_name,".")!=0) && (strcmp(dir->d_name, "..") !=0)){

			unsigned int len = strlen(dir->d_name);

			dircontent[index]=(char*)malloc((len+1)*sizeof(char));

			strcpy(dircontent[index],dir->d_name);

			index++;

			dircontent = (char**)realloc(dircontent,(index+1)*sizeof(char*));
			//printf("index=%d, size=%d, added entry '%s'\n", index,index+1, dircontent[index-1]);
		}
	}
	closedir(d);
	dircontent[index]=NULL;

	return (const char**) dircontent;
}



const char **read_dir_Targetscontent(const char *dirname){

	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);

	if(d==NULL){
		fprintf(stderr,"Error! Unable to open the directory.\n");
		exit(1);
	}

	char** dircontent = (char**)malloc(sizeof(char*));
	unsigned int index= 0;


	while ((dir = readdir(d)) != NULL){//printf("%s\n", dir->d_name);// i dont need to use rewcursion!!

		char const *filenameShort=NULL;

		const char * concatenatedName =concatenate(dir->d_name, dirname, &filenameShort);


		int checkregfile=  is_regular_file(concatenatedName);

		if( (strcmp(dir->d_name,".")!=0) && (strcmp(dir->d_name, "..") !=0) &&  checkregfile){
			//puts("Im after the checkregfile if\n");

			char *postsv =strchr(dir->d_name,'.');

			if((postsv != NULL) && strcmp(postsv, ".tsv")==0){

				//puts("Im after the posxml != NULL \n");

				unsigned int len = strlen(concatenatedName);

				dircontent[index]=(char*)malloc((len+1)*sizeof(char));

				strcpy(dircontent[index],concatenatedName);

				index++;

				dircontent = (char**)realloc(dircontent,(index+1)*sizeof(char*));
				//printf("index=%d, size=%d, added entry '%s'\n", index,index+1, dircontent[index-1]);
			}
		}
	}
	closedir(d);
	dircontent[index]=NULL;

	return (const char**) dircontent;
}



const char *get_Tgoldpath(const char *dirGoldpath,const char *Targetdirname, char **target){


	fprintf(stderr, "dirGoldpath= %s\n\n", dirGoldpath);
	fprintf(stderr, "Targetdirname= %s\n\n", Targetdirname);
	const char ** goldallfiles =read_dir_Targetscontent(dirGoldpath);


	const char *posone=strrchr(Targetdirname,'/');

	const char *postwo=strrchr(Targetdirname,'.');

	int len=postwo-posone;
	char predtarget[100];
	strncpy(predtarget,posone+1,len);
	predtarget[len-1] = '\0';
	fprintf(stderr, "predtarget:::: %s\n\n", predtarget);



	for(int i=0; goldallfiles[i]!=NULL; i++){
		//fprintf(stderr, "goldallfiles[i]= %s\n\n", goldallfiles[i]);

		const char *posONE=strrchr(goldallfiles[i],'/');
		const char *posTWO=strchr(posONE,'_');

		int goldlen=posTWO-posONE;
		char goldtarget[100];
		strncpy(goldtarget,posONE+1,goldlen);
		goldtarget[goldlen-1] = '\0';
	//	fprintf(stderr, "goldtarget:::: %s\n\n",goldtarget);

		if(strcmp(predtarget,goldtarget )==0){
			fprintf(stderr, "goldtarget:::: %s\n\n",goldtarget);
//			puts("im returning\n");
//			exit(1);

			*target=strdup(predtarget);
		return goldallfiles[i];

		}

	}


}




char *** read_probabilitiesvectors(char *probfile, unsigned int *sizeD){

	const unsigned long block_size=10;

	char ***probf= (char ***)malloc(block_size*sizeof(char **));
	probf[block_size-1]=NULL;

	char buff[100000];

	//fprintf(stderr, "the file %s\n",filename);

	FILE *fptr =NULL;
	fptr = fopen(probfile,"r");

	if(fptr ==NULL){
		fprintf(stderr, "Error opening the file %s\n",probfile);
		exit(1);
	}

	int j=0;
	while(fgets(buff, 100000, fptr)!=NULL){

		//fprintf(stderr, "DEBUG: buff %s \n",buff);

		if(((j+1)%block_size)==0){
			(probf)= (char ***)realloc((probf),(j+1+block_size)*sizeof(char **));
			//(*targetContent)[j+block_size]=NULL;WRONG CUZ SOMETIMES I DONT FILL THE WHOLE 10 BLOCKS
		}

		//puts("im here \n");

		buff[strcspn(buff, "\r\n")] = '\0';
		char **propbtokns = tokenise_opt2(buff,"\t",1);


		//				for(int i=0; WSItokns[i]!=NULL; i++){
		//					printf("the %d string from the WSI_GOLD is %s\n",i, WSItokns[i]);
		//				}


		(probf)[j]= propbtokns;
		j++;
		(probf)[j]=NULL;

	}

	j=j-1;
	(*sizeD)=j;
	fprintf(stderr, "the size of data %d\n", j);

	//	for(int p=0; p<j+1; p++){
	//		print_stringarray((*targetContent)[p]);
	//	}

	return probf;

}
