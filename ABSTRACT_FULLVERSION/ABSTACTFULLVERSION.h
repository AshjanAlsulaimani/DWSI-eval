/*
 * ABSTACTFULLVERSION.h
 *
 *  Created on: 12 Dec 2019
 *      Author: alsulaia
 */

//#ifndef ABSTRACT_FULLVERSION_ABSTACTFULLVERSION_H_
//#define ABSTRACT_FULLVERSION_ABSTACTFULLVERSION_H_
//
//class ABSTACTFULLVERSION {
//public:
//	ABSTACTFULLVERSION();
//	virtual ~ABSTACTFULLVERSION();
//};
//
//#endif /* ABSTRACT_FULLVERSION_ABSTACTFULLVERSION_H_ */

//g++ -c ABSTACTFULLVERSION.cpp -I/usr/include/python2.7 -lpython2.7 -I/usr/local/include/spacy -lspacy
//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/alsulaia/now/projects/nlp/spacy-cpp/cpp/build/

//g++ -o ABSTACTFULLVERSION_test ABSTACTFULLVERSION_test.cpp ../misc.o ABSTACTFULLVERSION.o -I/usr/include/python2.7 -lpython2.7 -I/usr/local/include/spacy -lspacy

//./ABSTACTFULLVERSION_test -t ../READMETA_collectPMids/target-terms_lower-nodubplicate_labeled -r -n -l ../FILTERPMID/toy_overlap_filter/ ./RESULTS 5 1

extern int outputdirectoriesNum_full;
extern char const *parameters_full;
extern char const *colonames_parameters_full;
extern char const *dirname_CaseSensitiveFULL;
extern char const *dirname_Not_CaseSensitiveFULL;

extern unsigned long int BUFFSIZE;



#include <map>
#include <string>
#include <spacy/spacy>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "../misc.h"
#include <assert.h>
#include <vector>
#include <string.h>
//#include <boost/tokenizer.hpp>
#include <algorithm>
#include "boost/algorithm/string.hpp"
using namespace std;


vector<string> *find_context_leftSide(char **tokensSentence, int startposition, unsigned int windowsize, int padding );
vector<string> *find_context_rightSide(char **tokensSentence, int startposition, unsigned int windowsize, int padding, int sentlen );

void replace_stopwords(vector<pair<unsigned int,unsigned int>> targetposition, char **sentencetokens,set<string> *stopwords);

vector<vector<string>>* extract_context_FULL_replace(const char *Targetterm, char **tokensSentence , int tokenum,
		vector<pair<unsigned int,unsigned int>> targetposition, unsigned int windowsize, int padding, unsigned int lowercase);
set<string> *readandstore_stopfile(char *StoPWordsfile);



map<string, map<string,set<string> *> *> *read_UMLSterms_FULL(char *pathTerms, unsigned int hyphenOption);



void print_parameters_file_FULL(char *mainDirectory, char const *filename, int outputdirectoriesNum,unsigned int windowsize,
			unsigned int tokeniseOption,unsigned int caseSensitive,unsigned int UMLSoption, unsigned int paddingOption, unsigned int r_duplicateOption,
			unsigned int hyphenOption, unsigned int FilteredStopWord);



//vector<pair<unsigned int,unsigned int>> remove_duplicate_text_FULL(vector<pair<unsigned int,unsigned int>> positions);

/*
 * year= abstractokns[yearcol];
					pmidid= abstractokns[pmcol];
					descid= abstractokns[concol];
					targ=abstractokns[tarcol];
					major=abstractokns[majtop];
					desID= abstractokns[mescol];
					txTy=abstractokns[typcol];
					meshNa=abstractokns[desNa];
 */

void print_target_file_FULL(map<string, FILE *> *outputFiles, char *Targetdirname ,vector<vector<string>> *contexts, const char *Targetterm,
		char *year, char *pmidid, char *descid, char *targ,char *major,char *desID,char *txTy, char *meshNa,char *sense);
vector<vector<string>>* extract_context_FULL(const char *Targetterm, char **tokensSentence , int tokenum, vector<pair<unsigned int,unsigned int>> targetposition, unsigned int windowssize, int padding, unsigned int lowercase);


void store_abstract_hypen_FULL(char *pathAbstract, char  *pathresult, const char *subdirResult, unsigned int windowssize,
		unsigned int caseSensitive, unsigned int Spacy, map<string, map<string,set<string> *> *> *UMLSterms, int padding,
		int r_duplicate, unsigned int hyphenOption, unsigned int lowercase, set<string> *stopwords);


vector<pair<unsigned int,unsigned int>> remove_duplicate_text_FULL(vector<pair<unsigned int,unsigned int>> positions);
vector<pair<unsigned int,unsigned int>> find_target_FULL(char **sentence, int caseSensitive, char *** termsTokens);
void print_paragrah_FULL(char ***tokensAbstract);
const char **read_dir_Targetscontent_FULL(const char *dirname);
const char ** read_dir_content_FULL(const char *dirpath);

int match_tokens_FULL(char *x, char *y, int caseSensitive);
char ***getUMLSterms_FULL(string Targetterm, map<string, map<string,set<string> *> *> *UMLSterms, unsigned int Spacy, char *sense);
char **tokenize_sentence_FULL(string sent, unsigned int Spacy, unsigned int *tokenum);
char **tokenize_sentence_FULL(Spacy::Span sent, unsigned int Spacy, unsigned int *tokenum);


