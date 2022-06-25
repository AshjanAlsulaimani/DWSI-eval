/*
 * ABSTACTFULLVERSION.cpp
 *
 *  Created on: 12 Dec 2019
 *      Author: alsulaia
 */

#include "ABSTACTFULLVERSION.h"

//ABSTACTFULLVERSION::ABSTACTFULLVERSION() {
//	// TODO Auto-generated constructor stub
//
//}
//
//ABSTACTFULLVERSION::~ABSTACTFULLVERSION() {
//	// TODO Auto-generated destructor stub
//}
//
int outputdirectoriesNum_full=1;
char const *parameters_full="Parameters";
char const *colonames_parameters_full= "FolderName\tWindowSizeValue\tTokenizerMethod\tCaseSensitive\tUMLSTermsOption\toutputDireNum\tPaddingOption\tR_Duplicate\tHyphenOption\tFilteredStopWord\n";

char const *dirname_CaseSensitiveFULL= "NCorpusCaseSensitiveFULL";
char const *dirname_Not_CaseSensitiveFULL= "NCorpusNotCaseSensitiveFULL";

unsigned long int BUFFSIZE= 100000000;





set<string> *readandstore_stopfile(char *StoPWordsfile){

	char buff[10000];
	FILE *fptr =NULL;
	fptr = fopen(StoPWordsfile,"r");

	if(fptr ==NULL){
		fprintf(stderr, "Error opening StoPWordsfile\n");
		exit(1);
	}

	set<string> *stopwords= new set<string> ();

	int line =0;
	while(fgets(buff, 10000, fptr)!=NULL){

		if(line !=0){

			buff[strcspn(buff, "\r\n")] = '\0';

			char **termstokns = tokenise_opt2(buff,"\t");

			if((termstokns[0]==NULL) || (termstokns[1]==NULL) ) {
				fprintf(stderr,"ERROR: must be 2 columns!\n");
				exit(1);
			}

			stopwords->insert(termstokns[1]);

		}
		line++;
	}

	fclose(fptr);
	return stopwords;
}



void store_abstract_hypen_FULL(char *pathAbstract, char  *pathresult, const  char *subdirResult, unsigned int windowssize,
		unsigned int caseSensitive, unsigned int Spacy, map<string, map<string,set<string> *> *> *UMLSterms, int padding,
		int r_duplicate, unsigned int hyphenOption, unsigned int lowercase, set<string> *stopwords)
{

	//READ THE DIR HERE!
	const char **dircontent = read_dir_content_FULL(pathAbstract);

	char const *filenameShort= NULL;



	// Spacy
	Spacy::Spacy spacy;
	// Spacy::load
	Spacy::Nlp nlp = spacy.load("en_core_web_sm");




	char *Tar = strrchr(pathAbstract, '/');

	if(strcmp(Tar,"/")==0 && strlen(Tar)==1){

		Tar[0]='\0';

		Tar = strrchr(pathAbstract, '/');
		assert(Tar);

	}

	fprintf(stderr, "Directory name  %s \n", Tar);


	// MAP NOT USED
	//	map<string, FILE *> *outputFiles = new map<string, FILE *> ();
	map<string, FILE *> *outputFiles = NULL;


	for(int i=0; dircontent[i]!=NULL; i++){// 2 files at least

		fprintf(stderr, " %d The file name  %s \n", i ,dircontent[i]);



		char const *filesInDir= concatenate(dircontent[i], pathAbstract, &filenameShort);
		fprintf(stderr, "the path with the filename%s \n",filesInDir );

		//const char **filesInDir =read_dir_content_FULL(Dirnamepath);
		const char *Targetterm =Tar+1;



		replaceCharInString((char *)Targetterm, '_' ,' ') ;



		char Targetdirname[500];
		strcpy(Targetdirname, subdirResult);
		strcat(Targetdirname,"/");
		strcat(Targetdirname,Targetterm);
		createDirSafely(Targetdirname);



		fprintf(stderr, "**** NOW %d THE TARGET TERM IS %s ****\n",i,Targetterm);



		char *buff=(char *)malloc(BUFFSIZE*sizeof(char));
		FILE *fptr =NULL;
		fptr = fopen(filesInDir ,"r");

		if(fptr ==NULL){
			fprintf(stderr, "Error opening target content %s number %d\n", filesInDir,i);
			exit(1);
		}




		char *sense=NULL;
		const char *poscui =strstr(dircontent[i],".tsv");
		sense=(char *)malloc(((poscui-dircontent[i])+1)*sizeof(char));
		strncpy(sense,dircontent[i], (poscui-dircontent[i]));
		(sense)[poscui-dircontent[i]]='\0';






		char *** termsTokens=NULL;
		if(UMLSterms!=NULL){

			//	fprintf(stderr,"******First: IM calling getUMLSterms for the sense %s and it will be tokenized by Spacy=%d*******\n", sense,Spacy);
			termsTokens= getUMLSterms_FULL(string(Targetterm),UMLSterms, Spacy, sense);
		} else {

			unsigned int termtokensnum;

			termsTokens= (char ***) malloc(2*sizeof(char**));

			//puts("IM calling tokenize sentense when UMLS is NULL option\n");
			termsTokens[0] = tokenize_sentence_FULL(string(Targetterm), Spacy, &termtokensnum);
			termsTokens[1]=NULL;
		}



		//FOR DEBUGGING
		//print_termsTokens_after_tokenising(termsTokens, Spacy);


		//YEAR	PMIDID	Descriptor/CONCEPT ID	TEXT	TYPE
		//YEAR	PMIDID	Target	CONCEPT	MeshID	MajorTopicYN	DescriptorName	TEXT	TYPE

		//puts("Im before the while loop!");




		int yearcol=-1;
		int pmcol=-1;
		int tarcol=-1;
		int concol=-1;
		int mescol=-1;
		int majtop=-1;
		int desNa=-1;
		int texcol=-1;
		int typcol=-1;

		int line =0;
		while(fgets(buff, BUFFSIZE, fptr)!=NULL){

			buff[strcspn(buff, "\r\n")] = '\0';



			if(line==0){

				char **coltokens= tokenise_opt2(buff,"\t");

				unsigned int len =nullterminatedlength(coltokens);



				fprintf(stderr,"size of len %d\n",len);

				for(int col=0; col<len ; col++){

					char *colname = coltokens[col];

					if(strcmp(colname,"YEAR")==0){
						yearcol=col;
						fprintf(stderr, "%d:%s\n",yearcol,colname);
					}


					if(strcmp(colname,"PMIDID")==0){
						pmcol=col;
						fprintf(stderr, "%d:%s\n",pmcol,colname );
					}

					if(strcmp(colname,"Target")==0){
						tarcol=col;
						fprintf(stderr, "%d:%s\n",tarcol,colname);
					}

					if(strcmp(colname,"CONCEPT")==0){
						concol= col;
						fprintf(stderr, "%d:%s\n",concol,colname);
					}

					if(strcmp(colname,"MeshID")==0){
						mescol=col;
						fprintf(stderr, "%d:%s\n",mescol,colname);
					}

					if(strcmp(colname,"MajorTopicYN")==0){
						majtop=col;
						fprintf(stderr, "%d:%s\n",majtop,colname);
					}


					if(strcmp(colname,"DescriptorName")==0){
						desNa=col;
						fprintf(stderr, "%d:%s\n",desNa,colname);
					}

					if(strcmp(colname,"TEXT")==0){
						texcol=col;
						fprintf(stderr, "%d:%s\n", texcol,colname);
					}

					if(strcmp(colname,"TYPE")==0){
						typcol=col;
						fprintf(stderr, "%d:%s\n", typcol,colname);
					}

				}

				if ( yearcol==-1|| pmcol==-1|| tarcol==-1|| concol==-1|| mescol==-1|| majtop==-1|| desNa==-1|| texcol==-1|| typcol==-1){

					fprintf(stderr, "ERROR: at least one column missing from the file\n");
					fprintf(stderr, "yearcol=%d|| pmcol=%d|| tarcol=%d|| concol=%d|| mescol=%d|| majtop=%d|| desNa=%d|| texcol=%d|| typcol=%d\n",
							yearcol, pmcol, tarcol, concol, mescol,  majtop, desNa, texcol, typcol);

					exit(1);
				}


				free_double_dimentions((void **)coltokens);

			}


			//fprintf(stderr, "NOW: yearcol=%d|| pmcol=%d|| tarcol=%d|| concol=%d|| mescol=%d|| majtop=%d|| desNa=%d|| texcol=%d|| typcol=%d\n",
			//	yearcol, pmcol, tarcol, concol, mescol,  majtop, desNa, texcol, typcol);


			if(line!=0){

				fprintf(stderr, "The line number is %d\n", line);

				FILE *f;

				char **abstractokns= tokenise_opt2(buff,"\t");

				//										for(int i=0; abstractokns[i]!=NULL; i++){
				//											printf("the abstract tokens %s\n", abstractokns[i]);
				//										}

				char * year=   NULL;
				char * pmidid= NULL;
				char * descid= NULL;
				char *targ= NULL;
				char *meshNa=NULL;
				char *major=NULL;
				char *desID=NULL;
				char *txTy=NULL;

				//YEAR	PMIDID	Target	CONCEPT	MeshID	MajorTopicYN	DescriptorName	TEXT	TYPE

				//	for(int j =0; abstractokns[j]!=NULL; j++){
				year= abstractokns[yearcol];
				pmidid= abstractokns[pmcol];
				//descid= abstractokns[concol];
				targ=abstractokns[tarcol];
				descid= abstractokns[concol];
				major=abstractokns[majtop];
				desID= abstractokns[mescol];
				txTy=abstractokns[typcol];
				meshNa=abstractokns[desNa];
				//	}


				assert(year);
				assert(pmidid);
				assert(targ);
				assert(descid);
				assert(major);
				assert(desID);
				assert(txTy);
				assert(meshNa);

				if(strcmp(descid, sense)!=0){

					fprintf(stderr, "ERORR: the sense must match in the filename %s and inside the coloum of that file %s\n",sense,descid);
					exit(1);
				}

				fprintf(stderr, "*************YEAR of the instance %s for SENSE %s\n", year, descid);

				// fprintf(stderr,"%s", term);

				unsigned int sizepara=0;


				//puts("Im after giving birth to tokensabstract!");


				if(abstractokns[texcol]==NULL || !contain_atleast_one_nonspace(abstractokns[texcol])){



					//	unsigned int x =nullterminatedlength(abstractokns);
					//cerr << "the size of this is: \n" << x << "\n";

					fprintf(stderr, "EMPTY ABSTRACT COLOUM, ABSTRACT NUMBER %d for Target %s \n", line,  Targetterm);
					//cerr << "EMPTY ABSTRACT COLOUM, ABSTRACT NUMBER " << line << " for TARGET "  << Targetterm << '\n';




				}else{

					//	fprintf(stderr, "**********THE RAW ABSTRACT %s *************\n", abstractokns[texcol]);

					if( hyphenOption==1){

						replaceCharInString(abstractokns[texcol], '-' ,' ') ;

						//fprintf(stderr, "**********THE RAW without HYPEN ABSTRACT %s *************\n", abstractokns[texcol]);
					}

					auto doc = nlp.parse(abstractokns[texcol]);

					char ***tokensAbstract =(char ***)malloc(((doc.sents().size())+1)*sizeof(char **));
					tokensAbstract[(doc.sents().size())]=NULL;


					//auto& sent;
					// std::vector<Spacy::Span> sents = doc.sents();
					//	Spacy::Span sent;


					//cerr << "the size is : "<<doc.sents().size() << "\n";


					int sentencenum=0;
					for ( auto& sent : doc.sents()) {
						sentencenum++;

						//std::cerr<<"\n" <<sentencenum << "** SENTENCE **\n" ;

						cerr << "SENTENCES SIZE "<< doc.sents().size() << "IN THE CURRENT ABSTRACT:" << line << "\n";

						cerr<<"  " << sent.text()<<"\n";


						string check =sent.text();
						fprintf(stderr, "***********  %s\n", check.c_str());


						//char *sntnc= strdup(check.c_str());
						//to get the occurence according to the context window.
						//auto doc2 = nlp.parse(check);




						unsigned int tokenum=0; // to count the tokens within a sentence


						tokensAbstract[(sentencenum-1)]= tokenize_sentence_FULL(sent, Spacy, &tokenum);



						//puts("IM calling find_target function\n");
						vector<pair<unsigned int,unsigned int>> targetposition =find_target_FULL(tokensAbstract[(sentencenum-1)],0, termsTokens);


						cerr << "number of targets found:" << targetposition.size()<< "\n";


						//
						//						for(int i=0; i< targetposition.size(); i++){
						//
						//							pair<unsigned int,unsigned int> position= targetposition[i];
						//
						//							int startpos =position.first;
						//							int endpos =position.second;
						//
						//							cerr << "start" <<startpos << "\n" <<"end" << endpos << "\n";
						//
						//						}


						if(r_duplicate==1){
							//fprintf(stderr,"Im calling remove_duplicate\n");
							targetposition =remove_duplicate_text_FULL(targetposition);
							cerr << "number of targets found after remove duplicate if any:" << targetposition.size()<< "\n";

						}

						vector<vector<string>> *contexts=NULL;

						if(stopwords!=NULL){

							replace_stopwords(targetposition, tokensAbstract[(sentencenum-1)],stopwords);


							contexts =extract_context_FULL_replace(Targetterm,tokensAbstract[(sentencenum-1)] ,tokenum , targetposition,  windowssize, padding,lowercase );



						}



						if(stopwords==NULL){

							//puts("IM calling extract context function\n");
							//$	vector<vector<string>> *contexts =extract_context(tokensAbstract[(sentencenum-1)], tokenum ,targetposition, windowssize);

							//contexts =extract_context_FULL_replace(Targetterm,tokensAbstract[(sentencenum-1)] ,tokenum , targetposition,  windowssize, padding,lowercase );





							contexts =extract_context_FULL(Targetterm, tokensAbstract[(sentencenum-1)] ,tokenum , targetposition,  windowssize, padding,lowercase );
							fprintf(stderr, "after extract context: %ld contexts\n",contexts->size());
						}
						assert(contexts);


						/*
						 * 	                year= abstractokns[yearcol];
					pmidid= abstractokns[pmcol];
					descid= abstractokns[concol];
					targ=abstractokns[tarcol];
					major=abstractokns[majtop];
					desID= abstractokns[mescol];
					txTy=abstractokns[typcol];
					meshNa=abstractokns[desNa];
						 */

						//puts("IM calling print_target_file function\n");
						print_target_file_FULL(outputFiles, Targetdirname,contexts, Targetterm, year, pmidid, descid, targ,major,desID,txTy,meshNa, sense);


						//free the context
						for(int e=0; e<contexts->size(); e++){

							//							for(int in=0; contexts[e].size(); in++){
							//
							//								//contexts[e][in].erase(contexts[e][in].begin(),contexts[e][in].end() );
							//
							//								//contexts[e][in].clear();
							//
							//							}
							contexts[e].clear();
						}
						contexts->clear();
						delete contexts;

					}//doc sentences




					//kill abstract by abstract
					for(int i =0; tokensAbstract[i]==NULL; i++){
						free_double_dimentions((void **)tokensAbstract[i]);// we dont need to free the char *** as we didnt allocate memeory for it.
					}

					//delete(doc);

				}	// if the abstract is empty we ignore it and thats the end of else

			}//line
			line++;

		}//while


		//free
		for(int i =0; termsTokens[i]==NULL; i++){
			free_double_dimentions((void **)termsTokens[i]);
		}
		free(buff);

		//}// loop for all the content of the main directory

		//		for(map<string, FILE *>::iterator itrfiles =outputFiles->begin(); itrfiles != outputFiles->end(); itrfiles++)
		//		{
		//			FILE *fl =itrfiles->second;
		//			fclose(fl);
		//		}


		fprintf(stderr, "*********FINISHED WITH DIRECTORY dircontent[%d]=%s *****************\n", i,dircontent[i]);

	}//end all the main directory

}//end of the function





void replace_stopwords(vector<pair<unsigned int,unsigned int>> targetposition, char **sentencetokens, set<string> *stopwords)
{

	for (int Tpos=0; sentencetokens[Tpos] != NULL; Tpos++) {

		//fprintf(stderr, "pos %d, token=%s\n",Tpos,sentencetokens[Tpos]);
		int i=0;
		int insideTarget=0;
		for(int i=0; i < targetposition.size(); i++){// all the occurrences in the sentence
			//fprintf(stderr, "  target i=%d\n");
			pair<int,int> pos = targetposition[i];

			int start=pos.first;
			int end =pos.second;

			if ((Tpos>=start) && (Tpos<=end)) {
				insideTarget=1;
				break;
			}
		}
		//fprintf(stderr, "  insidetarget=%d\n", insideTarget);
		if (!insideTarget) {
			string token =	string(sentencetokens[Tpos]);
			boost::to_lower(token);
			set<string>::iterator itset = stopwords->find(token);

			if(itset!=stopwords->end()){
				free(sentencetokens[Tpos]);
				sentencetokens[Tpos]=NULL;
			}
		}
	}

}


////ANOTHE WAY NOT TESTED
//void replace_stopwords(vector<pair<unsigned int,unsigned int>> targetposition, char **sentencetokens, set<string> *stopwords){
//
//	int Tpos = 0;
//	for(int i=0; i < targetposition.size(); i++){// all the occurrences in the sentence
//
//		pair<int,int> pos = targetposition[i];
//
//		int start=pos.first;
//		int end =pos.second;
//		while(Tpos<start) {
//			string token =string(sentencetokens[Tpos]);
//			set<string>::iterator itset = stopwords->find(token);
//
//			if(itset!=stopwords->end()){
//				free(sentencetokens[Tpos]);
//				sentencetokens[Tpos]=NULL;
//
//			}
//			Tpos++;
//		}
//		if (Tpos<end+1) {
//			Tpos = end+1;
//		}
//	}
//	while(sentencetokens[Tpos] != NULL) {
//		string token =string(sentencetokens[Tpos]);
//		set<string>::iterator itset = stopwords->find(token);
//
//		if(itset!=stopwords->end()){
//			free(sentencetokens[Tpos]);
//			sentencetokens[Tpos]=NULL;
//
//		}
//		Tpos++;
//	}
//
//}




//void print_target_file_FULL(map<string, FILE *> *outputFiles, char *Targetdirname ,vector<vector<string>> *contexts, const char *Targetterm,
//		char *year, char *pmidid, char *descid, char *sense)


void print_target_file_FULL(map<string, FILE *> *outputFiles, char *Targetdirname ,vector<vector<string>> *contexts, const char *Targetterm,
		char *year, char *pmidid, char *descid, char *targ,char *major,char *desID,char *txTy, char *meshNa,char *sense)

{

	//	puts("IM INSIDE PRINT TARGET FILE");


	// need to add to close it
	//	map<string, FILE *>::iterator itrfiles = outputFiles->find(string(year));



	FILE *fldes = NULL;
	//	if(itrfiles == outputFiles->end()){
	char fname[500];
	strcpy(fname, Targetdirname);
	strcat(fname,"/");
	strcat(fname,year);

	//		fprintf(stderr, "creating new file %s\n", fname);
	//		fldes= fopen(fname,"w");
	fldes= fopen(fname,"a");

	if(fldes==NULL){
		fprintf(stderr, "ERROR: OPENING FILE '%s'!\n",fname);
		exit(1);
	}
	//		outputFiles->insert({string(year), fldes});




	//	} else {
	//
	//		string currfname=itrfiles->first.c_str();
	//		fprintf(stderr, "getting existing file from map %s\n",currfname.c_str());
	//		fldes= itrfiles->second;
	//	}

	//puts("IM here before printing the context\n");


	for(int i=0; i< contexts->size(); i++){

		fprintf(stderr,"DEBUG: printing context %d...",i);
		if (contexts->at(i).size() ==0) {
			fprintf(stderr, "Error: empty context vector\n");
			exit(1);
		}
		fprintf(fldes, "%s", contexts->at(i)[0].c_str());

		//fprintf(stderr, "the word: %s", contexts->at(i)[0].c_str());


		for(int j=1; j<contexts->at(i).size(); j++){
			fprintf(fldes, " %s", contexts->at(i)[j].c_str());

			//fprintf(stderr, " the word: %s", contexts->at(i)[j].c_str());

		}

		if(year!=NULL && pmidid!=NULL &&  descid!=NULL && targ!= NULL && major!=NULL && desID!=NULL && txTy!=NULL && meshNa!=NULL){

			fprintf(fldes, "\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", year, pmidid,  descid,targ,major,desID,txTy,meshNa); // add the senses

		}else{
			fprintf(stderr, "ERROR: one of the additional column is not supposed to be empty\n");
			exit(1);
		}
	}
	fclose(fldes);


}



vector<string> *find_context_rightSide(char **tokensSentence, int startposition, unsigned int windowsize, int padding, int sentlen ){

	int pos= startposition;
	vector<string> *rightcontext= new vector<string>();

	while((pos < sentlen) && ((rightcontext->size() < windowsize) || (windowsize==0))){
//		fprintf(stderr,"R pos=%d\n",pos);
		if(tokensSentence[pos]!=NULL){
	//		fprintf(stderr,"  R push\n");

			rightcontext->push_back(string(tokensSentence[pos]));

		}
		pos++;
	}


	if(padding){

		while(rightcontext->size() < windowsize){

			rightcontext->push_back("#RIGHT#");
		}
	}

	return rightcontext;
}


vector<string> *find_context_leftSide(char **tokensSentence, int startposition, unsigned int windowsize, int padding ){

	int pos= startposition;
	vector<string> *leftcontext= new vector<string>();

	while((pos >=0 ) && ((leftcontext->size() < windowsize) || (windowsize==0))){
		//fprintf(stderr,"L pos=%d\n",pos);

		if(tokensSentence[pos]!=NULL){
			//fprintf(stderr,"  L push\n");

			leftcontext->push_back(string(tokensSentence[pos]));

		}
		pos--;
	}


	if(padding){

		while(leftcontext->size() < windowsize){

			leftcontext->push_back("#LEFT#");
		}
	}

	return leftcontext;
}




//map<string, FILE *> *outputFiles, char *Targetdirname ,vector<vector<string>> *contexts, const char *Targetterm,
//char *year, char *pmidid, char *descid, char *targ,char *major,char *desID,char *txTy, char *meshNa,char *sense)

vector<vector<string>>* extract_context_FULL_replace(const char *Targetterm, char **tokensSentence , int tokenum,
		vector<pair<unsigned int,unsigned int>> targetposition, unsigned int windowsize, int padding, unsigned int lowercase) {

	//fprintf(stderr, "TargetPosition %ld", targetposition.size());

	vector<vector<string>> *Sentencecontext= new vector<vector<string>> ();

	for(int i=0; i < targetposition.size(); i++){// all the occurrences in the sentence

		pair<int,int> pos = targetposition[i];

		vector<string> currContext;

		vector<string> *left = find_context_leftSide(tokensSentence, pos.first-1, windowsize, padding );
		for (int l=left->size()-1 ; l>=0; l--) {
			if(lowercase==1){

				boost::to_lower(left->at(l));

				currContext.push_back(left->at(l));
			} else {
				currContext.push_back(left->at(l));
			}
		}
		left->clear();
		delete left;


		if(lowercase==1){
			string target=string(Targetterm);
			boost::to_lower(target);
			currContext.push_back(target);

		} else {
			string target= string(Targetterm);
			currContext.push_back(target);
		}


		vector<string> *right = find_context_rightSide(tokensSentence, pos.second+1, windowsize, padding,tokenum );
		for (int r=0; r<right->size();r++) {
			if(lowercase==1){

				boost::to_lower(right->at(r));
				currContext.push_back(right->at(r));
			} else {
				currContext.push_back(right->at(r));
			}
		}
		right->clear();
		delete right;

		Sentencecontext->push_back(currContext);// as we can have many targets in the sentence.

	}// end of the big for loop
	return Sentencecontext;
}























//map<string, FILE *> *outputFiles, char *Targetdirname ,vector<vector<string>> *contexts, const char *Targetterm,
//char *year, char *pmidid, char *descid, char *targ,char *major,char *desID,char *txTy, char *meshNa,char *sense)

vector<vector<string>>* extract_context_FULL(const char *Targetterm, char **tokensSentence , int tokenum, vector<pair<unsigned int,unsigned int>> targetposition, unsigned int windowssize, int padding, unsigned int lowercase)
																												{

	//fprintf(stderr, "TargetPosition %ld", targetposition.size());

	vector<vector<string>> *Sentencecontext= new vector<vector<string>> ();

	for(int i=0; i < targetposition.size(); i++){// all the occurrences in the sentence

		pair<int,int> pos = targetposition[i];

		vector<string> currContext;
		//		cerr << "pos "<<i<< " : "<<pos.first<<","<<pos.second << "  : ";
		//cerr << "from "<< pos.first- (int) windowssize << " to "<<pos.second+ (int) windowssize<<endl;


		int start;
		int end;

		if(windowssize ==0){

			start= 0;
			end = tokenum-1;

		}else{
			start = pos.first- (int)windowssize;
			end =pos.second+ (int)windowssize;
		}


		//	fprintf(stderr, "sentence length=%d, start %d, end %d\n", tokenum, start, end);


		for(int j =start; j<=end ; j++){


			fprintf(stderr, "a j=%d\n", j);

			if (j<0 || j>=tokenum) { // outside sentence
				//fprintf(stderr, "out of range\n");
				if(padding){
					//fprintf(stderr, "padding\n");
					currContext.push_back("#");
				} // otherwise ignore
			} else {	// inside the scope of the sentence

				//fprintf(stderr, "b j=%d\n", j);

				if(pos.first <=j && pos.second >=j){
					//fprintf(stderr, "inside target pos...\n");

					if(pos.first==j){

						if(lowercase==1){

							assert(Targetterm);

							string targstr= string(Targetterm);

							boost::to_lower(targstr);

							currContext.push_back(targstr);


						}else{
							currContext.push_back(string(Targetterm));
						}
						//	fprintf(stderr, "pushing target...\n");

					}

				}else{

					assert(tokensSentence[j]);
					if(lowercase==1){
						string tknsstStr= 	string(tokensSentence[j]);
						boost::to_lower(tknsstStr);
						currContext.push_back(tknsstStr);

					}else{

						currContext.push_back(string(tokensSentence[j]));

					}

					//fprintf(stderr, "push currContext tokensSentence[%d]= %s\n",j,tokensSentence[j]);
				}
				//			cerr << currContext.back() << "; ";

			}

			//		cerr << endl;
		}

		Sentencecontext->push_back(currContext);// as we can have many targets in the sentence.

	}// end of the big for loop
	return Sentencecontext;
																												}












void print_parameters_file_FULL( char  *mainDirectory, char const *filename, int outputdirectoriesNum,unsigned int windowsize,
		unsigned int tokeniseOption,unsigned int caseSensitive,unsigned int UMLSoption, unsigned int paddingOption,
		unsigned int r_duplicateOption,unsigned int hyphenOption, unsigned int FilteredStopWord)

{
	int i=0;
	FILE *fptr;
	char dirfile[500];

	//"FolderName\tWindowSizeValue\tTokenizerMethod\tCaseSensitive\tUMLSTermsOption\outputDireNum";

	strcpy(dirfile, mainDirectory);
	strcat(dirfile,"/");
	strcat(dirfile, filename);

	fptr = fopen(dirfile,"w");
	if(fptr ==NULL){
		fprintf(stderr, "Error opening the file\n");
		exit(1);
	}

	fprintf(fptr, colonames_parameters_full,"w");
	fprintf(fptr, "%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",dirfile, windowsize,tokeniseOption, caseSensitive, UMLSoption,outputdirectoriesNum_full,paddingOption, r_duplicateOption,hyphenOption,FilteredStopWord);
	fclose(fptr);

}



map<string, map<string,set<string> *> *> *read_UMLSterms_FULL(char *pathTerms, unsigned int hyphenOption){


	char buff[10000];
	FILE *fptr =NULL;
	fptr = fopen(pathTerms,"r");

	if(fptr ==NULL){
		fprintf(stderr, "Error opening target-terms\n");
		exit(1);
	}



	map<string, map<string,set<string> *> *> *alltargets= new 	map<string, map<string,set<string>*>*> ();


	int line =0;
	while(fgets(buff, 10000, fptr)!=NULL){

		//puts(buff);

		if(line !=0){

			buff[strcspn(buff, "\r\n")] = '\0';

			char **termstokns = tokenise_opt2(buff,"\t");



			if((termstokns[0]==NULL) || (termstokns[1]==NULL) || (termstokns[2]==NULL)) {
				fprintf(stderr,"ERROR: must be 3 columns!\n");
				exit(1);
			}


			string targetword = termstokns[0];
			string cui = termstokns[1];
			char **umlsterms = tokenise_opt2(termstokns[2],"|");


			if( hyphenOption==1 && umlsterms!=NULL && umlsterms[0]!=NULL){

				replaceCharInString(termstokns[2], '-' ,' ') ;

			}

			 print_stringarray(termstokns);

			map<string, map<string,set<string>*>* >::iterator iter = alltargets->find(targetword);
			map<string,set<string>*> *allcuis;
			if (iter != alltargets->end()) {
				allcuis = iter->second;
			} else {
				allcuis = new map<string,set<string>*>();
				alltargets->insert({ targetword, allcuis});
			}
			map<string,set<string>*>::iterator iter2 = allcuis->find(cui);
			set<string>* terms;
			if (iter2 != allcuis->end()) {
				terms = iter2->second;
				fprintf(stderr,"warning: not supposed to already have CUI here!");
			} else {
				terms = new set<string>();
				allcuis->insert({ cui, terms});
			}

			char **curr =umlsterms;
			while (curr[0] != NULL) {
				terms->insert(string(curr[0]));
				curr++;
			}

		}
		line++;

//		//FOR DEBUGING
//				for (auto iter = alltargets->begin(); iter != alltargets->end(); iter++) {
//					cerr << "TARGET = " << iter->first <<endl;
//					map<string,set<string>*> *allcuis = iter->second;
//					for (auto iter2 = allcuis->begin(); iter2 != allcuis->end(); iter2++) {
//						cerr << "  CUI = " << iter2->first <<" : ";
//						set<string>* terms = iter2->second;
//						for (string t : *terms) {
//							cerr << t << " | ";
//						}
//						cerr << endl;
//					}
//				}


	}

	//FOR DEBUGING
	//	for (auto iter = alltargets->begin(); iter != alltargets->end(); iter++) {
	//		cerr << "TARGET = " << iter->first <<endl;
	//		map<string,set<string>*> *allcuis = iter->second;
	//		for (auto iter2 = allcuis->begin(); iter2 != allcuis->end(); iter2++) {
	//			cerr << "  CUI = " << iter2->first <<" : ";
	//			set<string>* terms = iter2->second;
	//			for (string t : *terms) {
	//				cerr << t << " | ";
	//			}
	//			cerr << endl;
	//		}
	//	}

	return alltargets;
}



/*
 * we assume that the positions are ordered in the order of the sentence, we always compare two consecutive ones, we check if two consecutive ones
 * have an overlap and if yes we remove the shortest
 */

vector<pair<unsigned int,unsigned int>> remove_duplicate_text_FULL(vector<pair<unsigned int,unsigned int>> positions)
																														{

	//fprintf(stderr,"remove_duplicate_text\n");

	int i =1;
	while( i<positions.size()){

		if(positions[i-1].second >= positions[i].first){


			//fprintf(stderr," positions[%d].second: %d , positions[%d].first: %d\n", i-1,positions[i-1].second, i,positions[i].first);


			//fprintf(stderr," positions[%d].second: %d , positions[%d].first: %d\n", i-1,positions[i-1].second, i,positions[i].first);


			int lengthone=positions[i-1].second - positions[i-1].first +1;

			//fprintf(stderr,"lengthone= %d\n",lengthone);


			int lengthtwo=positions[i].second - positions[i].first +1;


			//fprintf(stderr,"lengthtwo= %d\n",lengthtwo);


			if(lengthone > lengthtwo){// we dont increase i as we can compare the next with the previous

				fprintf(stderr,  "Removing duplicate lengthone > lengthtwo\n");
				positions.erase(positions.begin()+i);

			}else{
				positions.erase(positions.begin()+(i-1));
				fprintf(stderr,  "Removing duplicate lengthone < lengthtwo\n");
			}

		}else{
			i++;// as the pairs are stored in order so we compare adjutant words
		}

	}
	return positions;

																														}





vector<pair<unsigned int,unsigned int>> find_target_FULL(char **sentence, int caseSensitive, char *** termsTokens){


	//fprintf(stderr, "the CURRENT sentence is %s\n", sentence);



	vector<pair<unsigned int,unsigned int>> targetposition;

	for(unsigned int Spos=0; sentence[Spos]!=NULL; Spos++){


		//fprintf(stderr, "the sentence[%d] is %s\n", Spos,sentence[Spos]);




		for(int i=0; termsTokens[i]!=NULL; i++){// we are going to push them in the vector in the order of the sentence thats why we loop over the sentnece fisrt and then we loop over each token till you find it.

			char **targetokens = termsTokens[i];
			//  fprintf(stderr, "the termTokens loop in find is %d\n", i);
			unsigned int targetpos;

			for(targetpos=0; (targetokens[targetpos]!=NULL) &&
			(sentence[Spos+targetpos]!=NULL) &&
			(match_tokens_FULL(targetokens[targetpos], sentence[Spos+targetpos], caseSensitive)); targetpos++){

				//	fprintf(stderr,"I have matched at least %s \n",targetokens[targetpos]);

			}


			if (targetokens[targetpos]==NULL) {
				//cerr << "found target '"<<target<<"': "<<Spos<<" to "<<Spos+targetpos-1<< " in ";
				print_stringarray(sentence);
				targetposition.push_back(pair<int,int>(Spos, Spos+targetpos-1));
			}
		}
	}



	return targetposition;
}


int match_tokens_FULL(char *x, char *y, int caseSensitive) {

	//fprintf(stderr, "Inside Match Function x: %s and y: %s\n", x, y);

	int i=0;
	int j=0;


	while ((x[i] != '\0') && (y[j] != '\0')) {

		if (x[i] == '.') {

			i++;

		} else if (y[j] == '.') {

			j++;

		} else if(  (x[i] ==y[j]) || (!caseSensitive && (tolower(x[i])== tolower(y[j])))){

			i++;
			j++;

		}else{
			return 0;
		}
	}

	while ((x[i] != '\0') && (x[i] == '.')) {
		i++;
	}
	while ((y[j] != '\0') && (y[j] == '.')) {
		j++;
	}
	if((x[i] == '\0') && (y[j] == '\0')){

		//puts("True\n");
		return 1;

	}else{

		//puts("False\n");
		return 0;
	}



}



/*
 * it gets the UMLs Terms from the map and it tokenized them based on the option given
 */

char ***getUMLSterms_FULL(string Targetterm, map<string, map<string,set<string> *> *> *UMLSterms, unsigned int Spacy, char *sense){


	assert(sense);

	//puts("IM INSIDE getUMLSterms");

	char ***	termsTokens=NULL;
	map<string, map<string,set<string> *> *>::iterator itr1= UMLSterms->find(Targetterm);

	if( itr1== UMLSterms->end()){

		fprintf(stderr, "Error, the target word %s must be in UMLS Terms file\n",Targetterm.c_str());
		exit(1);

	}else{

		string target =itr1->first;
		map<string,set<string> *> *cuiterms= itr1->second;

		//		const char *poscui =strstr(cui,".tsv");
		//		*sense=(char *)malloc(((poscui-cui)+1)*sizeof(char));
		//		strncpy(*sense,cui, (poscui-cui));
		//		(*sense)[poscui-cui]='\0';

		//				puts("*********DEBUGGING************");
		//
		//				for(map<string,set<string> *>::iterator itr2 = cuiterms->begin();  itr2 != cuiterms->end(); itr2++){
		//
		//					string cui	=itr2->first;
		//					set<string> *terms = itr2->second;
		//
		//					fprintf(stderr, "the passed target is '%s' and the target is '%s':\n", Targetterm.c_str(), target.c_str());
		//					fprintf(stderr, "the passed sense is '%s' and the sense is : '%s' \t", sense,cui.c_str());
		//					fprintf(stderr, "the terms are:\n");
		//					for(set<string>::iterator itr3= terms->begin(); itr3!=terms->end(); itr3++){
		//
		//						string terms=*itr3;
		//
		//						fprintf(stderr, "'%s'|", terms.c_str());
		//
		//					}
		//
		//					fprintf(stderr,"\n");
		//
		//				}

		map<string,set<string> *>::iterator itr2 =cuiterms->find(string(sense));

		if(itr2 ==cuiterms->end()){

			fprintf(stderr, "Error, the CUI %s of target word %s must be in UMLS Terms file\n",sense,Targetterm.c_str());
			exit(1);

		}else{

			set<string> *terms = itr2->second;

			termsTokens=(char ***)malloc((terms->size()+1)*sizeof(char **));
			termsTokens[terms->size()]=NULL;
			unsigned int numterm=0;


			if(terms==NULL){

				fprintf(stderr, "Error, the terms of CUI %s of target word %s must be in UMLS Terms file\n",sense,Targetterm.c_str());
				exit(1);
			}else{

				for(string T : *terms){
					unsigned int tokenum=0;
					termsTokens[numterm] =tokenize_sentence_FULL(T, Spacy, &tokenum);
					numterm++;
				}
			}
		}
	}
	return termsTokens;
}



/* One function takes a string as an input and the other one takes spacy obejct as an input for a sentence or term.
 * Both functions can use the Spacy toknization or white space tokenization.
 *  if we toknize the sentence by spacy then its better to toknize the terms by spacy as well (that are going to be extracted from those tokenized setence).
 */
char **tokenize_sentence_FULL(string sent, unsigned int Spacy, unsigned int *tokenum){

	//puts("IM INSDIE TOKEIZE SENTENCE with string sent method\n");

	if(Spacy){
		// Spacy
		Spacy::Spacy spacy;
		// Spacy::load
		Spacy::Nlp nlp = spacy.load("en_core_web_sm");

		auto doc = nlp.parse(sent);

		*tokenum=0;

		char **tokensSentence= (char **)malloc((doc.tokens().size()+1)*sizeof(char *));
		tokensSentence[doc.tokens().size()]=NULL;
		for (auto& token : doc.tokens()) {

			string tkn= token.text();

			(*tokenum)++;
			//cerr<<"   \n" << token.text();

			tokensSentence[((*tokenum)-1)]=strdup(tkn.c_str());

			//fprintf(stderr,"%s\n", tokensSentence[((*tokenum)-1)]);

			//print_paragrah(tokensAbstract);

		}//tokens sentence

		//puts("IM RETURING now!\n ");
		return tokensSentence;

	}else{


		char **tokens= tokenise_opt2(sent.c_str(), " ", 0);
		*tokenum = nullterminatedlength(tokens);
		return tokens;


	}
}

char **tokenize_sentence_FULL(Spacy::Span sent, unsigned int Spacy, unsigned int *tokenum){

	//puts("Im inside tokenize snetnece SPACY method option\n");


	//cerr<< "the sentence by Spacy method: \n" << sent.text();

	*tokenum=0;

	char **tokensSentence= (char **)malloc((sent.tokens().size()+1)*sizeof(char *));
	tokensSentence[(sent.tokens().size())]=NULL;

	if(Spacy){


		for (auto& token : sent.tokens()) {

			string tkn= token.text();

			(*tokenum)++;
			//cerr<<"   \n" << token.text();

			tokensSentence[((*tokenum)-1)]=strdup(tkn.c_str());

			//	fprintf(stderr,"%s\n", tokensSentence[((*tokenum)-1)]);

			//print_paragrah(tokensAbstract);

		}//tokens sentence


		//cerr << "size of tokenum" << (*tokenum) << "size of sent token size" << (sent.tokens().size()) << "\n";


		return tokensSentence;

	}else{

		string sentstring =sent.text();


		char **tokens= tokenise_opt2(sentstring.c_str(), " ", 0);
		*tokenum = nullterminatedlength(tokens);
		return tokens;

	}

}



void print_paragrah_FULL(char ***tokensAbstract){

	while(tokensAbstract!=NULL){
		for(int i=0; tokensAbstract[i]!=NULL; i++){
			for(int j=0; tokensAbstract[i][j]!=NULL; j++){
				fprintf(stderr, "%s", tokensAbstract[i][j]);
			}
		}
	}
}






const char **read_dir_Targetscontent_FULL(const char *dirname)
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
