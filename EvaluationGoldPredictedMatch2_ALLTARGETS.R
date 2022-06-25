library(plyr)
library(ROCR)
library(ggplot2)
#library(Hmisc)
library(xts)
library(lubridate)
#library(gRbase)
library(bnlearn)
library(bnviewer)
library(gridExtra)
library(dplyr)
library(DescTools)
#source("/home/alsulaia/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERDATA/WSDGraphs.R")
#Rscript EvaluationGoldPredictedMatch2_ALLTARGETS.R ~/now/MeSHXMLFILES/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERDATA/FINALTRY_someEliminated.0/ 1
PATHENVR <- Sys.getenv("ENVR")
source(paste(PATHENVR,"WSDGraphs.R",sep="/"))
#source(paste(PATHENVR,"EvaluationGoldPredictedMatch_regression.R",sep="/"))


readInstancesTable <- function(filename) {
  # read the table as a "data frame" from the tsv file, with options: header included, separator tab, ignore quotes symbols in the text
  d <-read.table(filename, header=TRUE,sep='\t',quote="",comment.char = "",stringsAsFactors = FALSE)
  
}

reademergenceTable<- function(filename){
  
  d <-read.table(filename, header=TRUE,sep='\t',quote="",comment.char = "",stringsAsFactors = FALSE)
}

# class = one of the possible senses as target for evaluation
evalForOneClass <- function(df, class, goldColumnName='CUI', predColumnName='Predicted_CUI_based_on_Match') {
  

  # df[,xxx] means "the column xxx from df"
  # df[condition1,] means "the rows of df which satisfy condition"
  # nrow(df) means "number of rows in df" (or number of rows selected if there is a condition)
  
  # so for instance here we count the rows where the gold is the target class and the pred is also the target class:
  tp <- nrow(df[df[,goldColumnName] == class & df[,predColumnName] == class,])
  
  fp <- nrow(df[df[,goldColumnName] != class & df[,predColumnName] == class,])
  
  fn <- nrow(df[df[,goldColumnName] == class & df[,predColumnName] != class,])

  tn <- nrow(df[df[,goldColumnName] != class & df[,predColumnName] != class,])

  nbSensesGold <- length(unique(df[,goldColumnName]))
  nbSensesPred <- length(unique(df[,predColumnName]))
  
  goldPos <- tp + fn
  propGold <- goldPos / nrow(df)
  predPos <- tp + fp
  propPred <- predPos / nrow(df)
  predgolddiff <-predPos-goldPos
  
  
  prec <- tp / (tp+fp)
  rec <- tp / (tp+fn)
  f1 <- 2 * (prec*rec) / (prec+rec)
  # the last element in the functions is automatically returned
  # here we return the 7 values in a new data frame
  data.frame(gold.pos=goldPos, prop.gold=propGold, pred.pos=predPos, prop.Pred=propPred , PredGolddiff=predgolddiff ,tp=tp, fp=fp, fn=fn, tn=tn, precision=prec,recall=rec,f1score=f1, nbSensesGold=nbSensesGold,nbSensesPred=nbSensesPred)
 # data.frame(gold.pos=goldPos, prop.gold=propGold, pred.pos=predPos, tp=tp, fp=fp, fn=fn, tn=tn, precision=prec,recall=rec,f1score=f1, nbSensesGold=nbSensesGold,nbSensesPred=nbSensesPred)
  
}

#
# df = the output of readInstancesTable
#
evalForAllClasses <- function(df,goldColumnName='CUI', predColumnName='Predicted_CUI_based_on_Match') {
  
  # extract all the possible classes from the gold column
  allClasses <- unique(df[,goldColumnName])
  
  # ldply: loops over all the classes, apply the sub-function to it which returns a data frame.
  # ldply concatenates together the data frames obtained for each class
  ldply(allClasses, function(thisClass) {
    evalld <- evalForOneClass(df, thisClass, goldColumnName, predColumnName)
   # totalinstances <- sum(evalld$gold.pos)
    #strdGoldprop=sdN(evalld$prop.gold)
    cbind(data.frame(class=thisClass),evalld)
  })
  
}


# input = result from evalForAllClasses

evalMicroMacro <- function(perfDF, goldColumnName='CUI', predColumnName='Predicted_CUI_based_on_Match') {
  microP = sum(perfDF$tp)/(sum(perfDF$tp)+sum(perfDF$fp))
  microR = sum(perfDF$tp)/(sum(perfDF$tp)+sum(perfDF$fn))
  macroP = mean(perfDF$precision)
  macroR = mean(perfDF$recall)
  data.frame(evalType=c('micro', 'macro'), 
             precision=c(microP ,  macroP),
                recall=c(microR ,  macroR),
             f1score=c(2 * (microP*microR) / (microP+microR), 2 * (macroP*macroR) / (macroP+macroR))
  )
}



errorByInstanceDF <- function(df, goldColumnName='CUI', predAsCUIColumnName='Predicted_CUI_based_on_Match', predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER',predProbsColumnName='Inferred_Posterior_by_Sense'){
  # extract matching between sense as int and a CUI
  matchingDF <- unique(df[,c(predAsIntColumnName,predAsCUIColumnName)])
  print("matching:")
  print(matchingDF)

  # split the column of the probs (originally as strings) and convert it to data frame of doubles
  predProbsListStrings <- strsplit(df[,predProbsColumnName],"|",fixed=TRUE)
  predProbsDF0 <- ldply(predProbsListStrings, function(x) {as.numeric(x)})
  
  print(colnames(predProbsDF0))
  print(matchingDF[order(matchingDF[,predAsIntColumnName]),predAsCUIColumnName])
  
  # below we give the column names for the sense as CUIs, taking care of the order using the matching DF
  colnames(predProbsDF0) <- matchingDF[order(matchingDF[,predAsIntColumnName]),predAsCUIColumnName]
  
  #print(predProbsDF0)
  
 
  predProbsDF <- cbind(df,predProbsDF0)

#  print(head(predProbsDF))
  
  # split the df by gold class and for each class assign the error for all the individual instances
  ddply(predProbsDF, goldColumnName, function(instancesForThisClassDF) {
    class <- unique(instancesForThisClassDF[,goldColumnName])
    r<-cbind(instancesForThisClassDF, error = 1 - instancesForThisClassDF[,class])
  #  r<-cbind(instancesForThisClassDF, error = 1 - instancesForThisClassDF[,class],errorSquared=(1 - instancesForThisClassDF[,class])^2)
    # print(head(r))
    r
  } )

}


evalMAE <- function(df, goldColumnName='CUI', predAsCUIColumnName='Predicted_CUI_based_on_Match', predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER',predProbsColumnName='Inferred_Posterior_by_Sense'){
  errorDF <- errorByInstanceDF(df, goldColumnName, predAsCUIColumnName, predAsIntColumnName,predProbsColumnName)
  r<-ddply(errorDF, goldColumnName, function(instancesForThisClassDF) {
#    class <- unique(instancesForThisClassDF[,goldColumnName])
 #squared   data.frame(MAE=mean(instancesForThisClassDF$error), MSE=mean(instancesForThisClassDF$errorSquared))
# original 
    data.frame(MAE=mean(instancesForThisClassDF$error))
    
  })# inside ddply we work on the subdatafarme for each sense  note that ddlpy itself add the argument of goldcolumnName next to the MAE
# original
 allDF<-data.frame(gold='ALL', MAE=mean(errorDF$error))
#  allDF<-data.frame(gold='ALL', MAE=mean(errorDF$error), MSE=mean(errorDF$errorSquared))
  #original#
 colnames(allDF) <- c(goldColumnName, 'MAE')
#   colnames(allDF) <- c(goldColumnName, 'MAE','MSE')
  #print(colnames(allDF))
  #print(colnames(r))
  rbind(r, allDF )
}


plotmaxpredprob <- function(df, predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER', maxpredProb='INFERRED_PROBABILITY'){
#  ggplot(df, aes_string(x=maxpredProb, fill=predAsIntColumnName)) + geom_histogram() + facet_grid(MOST_PROBABLE_INFERRED_SENSE_NUMBER ~ .)
  ggplot(df, aes(x=INFERRED_PROBABILITY, fill=as.factor(Predicted_CUI_based_on_Match))) + geom_histogram(alpha=.5,position='identity',binwidth=.01)+ labs(fill="Pred CUI")
}


plotmaxpredprobdesity<-function(df, predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER', maxpredProb='INFERRED_PROBABILITY'){
ggplot(df,aes(x=INFERRED_PROBABILITY))+geom_histogram(aes(y=..density..))
}



plotmaxpredprobdesity2<-function(df, predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER', maxpredProb='INFERRED_PROBABILITY'){
  ggplot(df,aes(x=MOST_PROBABLE_INFERRED_SENSE_NUMBER))+geom_histogram(aes(y=..density..))
}


graphpredFreq <- function(d) {
  ggplot(d,aes(YEAR,fill=as.factor(MOST_PROBABLE_INFERRED_SENSE_NUMBER)))+geom_bar() + labs(fill="Pred Sense")
}

calc_PredProb <- function(d){
  probDF <- getProbDF(d, "YEAR", "MOST_PROBABLE_INFERRED_SENSE_NUMBER")
}

graphpredProb <- function(d) {
  probDF <- calc_PredProb(d)
  print('debug')
  print(head(d))
  print(head(probDF))
 ggplot(probDF,aes(YEAR,pSenseGivenYear,colour=as.factor(MOST_PROBABLE_INFERRED_SENSE_NUMBER)))+geom_line()+geom_point()+ labs(colour="Pred Sense")
  #ggplot(probDF,aes(YEAR,pSenseGivenYear,colour=as.factor(MOST_PROBABLE_INFERRED_SENSE_NUMBER)))+geom_line()+labs(colour='pred sense')
}


maxprobbyyear<-function(d){
  
  ggplot(d,aes(YEAR,INFERRED_PROBABILITY,group = YEAR))+geom_boxplot()
  
}
maxprobbyyearpersense<-function(d){
  
  ggplot(d,aes(YEAR,INFERRED_PROBABILITY,group = YEAR,fill=as.factor(MOST_PROBABLE_INFERRED_SENSE_NUMBER)))+geom_boxplot(position='dodge')+ labs(fill="Pred Sense")
  
}
maxprobbyyearsense<-function(d){
  
  ggplot(d,aes(YEAR,INFERRED_PROBABILITY,group = YEAR))+geom_boxplot()+facet_grid(.~MOST_PROBABLE_INFERRED_SENSE_NUMBER)
  
}

# the input data is the data frame stored in 'stats-by-target.tsv' 

analysisDataByTarget <- function(d, outputDir=NULL) {
  
  d$nbSensesGold <- as.factor(d$nbSensesGold)
  
  gone <- ggplot(d,aes(strdGoldprop, macfscore,color=nbSensesGold))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
  outputgraph<-paste(outputDir, "perfimbalance.pdf", sep="/")
  ggsave(outputgraph,gone,device='pdf')
  
  g <- ggplot(d,aes(totalinstances, macfscore))+geom_point()+theme_grey(base_size = 20)  
  outputgraph2<-paste(outputDir, "perf-nbinstances.pdf", sep="/")
  ggsave(outputgraph2,g,device='pdf')
  
  allcor<- data.frame(x=c('strdGoldprop','strdGoldprop','strdGoldprop','strdGoldprop','strdGoldprop','strdGoldprop','totalinstances','totalinstances','totalinstances','totalinstances'),
             y=c('macfscore','macfscore','micfscore','micfscore', 'strdPredprop','strdPredprop','macfscore','macfscore','micfscore','micfscore'),
             method=c('pearson','spearman','pearson','spearman','pearson','spearman','pearson','spearman','pearson','spearman'),
             cor=c(
               cor(d$strdGoldprop, d$macfscore),   cor(d$strdGoldprop, d$macfscore,method='spearman'),
               cor(d$strdGoldprop, d$micfscore),   cor(d$strdGoldprop, d$micfscore,method='spearman'),
               cor(d$strdGoldprop, d$strdPredprop),   cor(d$strdGoldprop, d$strdPredprop,method='spearman'),
               cor(d$totalinstances, d$macfscore),   cor(d$totalinstances, d$macfscore,method='spearman'),
               cor(d$totalinstances, d$micfscore),   cor(d$totalinstances, d$micfscore,method='spearman')
             )
  )
  
  outputcor<-paste(outputDir, "correlation-values.tsv", sep="/")
  write.table(allcor, outputcor,quote = FALSE, sep = "\t",row.names = FALSE)
  #gone 
}



sdN <- function(x) {
  sqrt(sum((x-mean(x))**2)/length(x))
}






# the input data is the data frame stored in 'stats-by-target.tsv' for both NEO and SCAN this is done for emnlp 

analysisDataByTargetNEOSCAN <- function(NeoDF, ScanDF,outputDir=NULL) {
  
  
  NeoDF$system<-"NEO"
  ScanDF$system <- "SCAN"
  
  NeoScanDF<-rbind(NeoDF, ScanDF)
  
  NeoScanDF$nbSensesGold <- as.factor(NeoScanDF$nbSensesGold)
  
 
  gneoscan <- ggplot(NeoScanDF,aes(strdGoldprop, macfscore,color=nbSensesGold))+geom_point()+xlab("SD Gold Propotion")+ylab("Macro F1-score")+theme_grey(base_size = 16)+scale_color_discrete(name = "Num Gold Senses")+theme(legend.position = "bottom", legend.text=element_text(size=16))+geom_smooth(method="lm",se = FALSE)+facet_grid(. ~ system)  
  
  if(!is.null(outputDir)){
  outputgraph<-paste(outputDir, "perfimbalanceNeoScan.pdf", sep="/")
  ggsave(outputgraph,  gneoscan ,device='pdf')
}
  gneoscan
}












#join_emergenceSensedata<-function(SenseData, AllemergenceData)





merge_senseprefData_MAEData<-function(senseData, MAEData){
  merge(senseData, MAEData[MAEData$CUI !='ALL',],by.x=c('target', 'class'), by.y=c('target','CUI'))
}
correlationFscoreMAE <- function(merged_senseprefData_MAEData,PathResults=NULL) {
  print("correlation:")
  print(cor(merged_senseprefData_MAEData$MAE,merged_senseprefData_MAEData$f1score))
  correlationf1scoreMAEbysenses <- ggplot(merged_senseprefData_MAEData,aes(x=MAE,y=f1score))+geom_point()
  
  if(!is.null(PathResults)){
    r<-paste(PathResults, "correlationf1scoreMAEbysenses.pdf", sep="/")
    ggsave(r, correlationf1scoreMAEbysenses ,device='pdf')
  } 
  
  correlationf1scoreMAEbysenses
}


#perfbyseneses file (classification performance by senses) and all_emergence.tsv file (gold emergence file) and goldpredtable.tsv (emergence output evaluation)
join_emergenceSensedata<-function(SenseData, AllemergenceData, goldpredEmergenceData){
  
  senseemrg<-ddply(SenseData, c("class", "target") ,function(sense){
    
 #   print(sense)
    
    selected <- AllemergenceData[as.character(AllemergenceData$SENSE)==as.character(sense$class) & as.character(AllemergenceData$WSD_INFORMATION) == as.character(sense$target),]
    if (nrow(selected)==1) {
      cbind(sense, GoldEmergence="emerged")
    } else {
      if (nrow(selected)==0) {
        cbind(sense, GoldEmergence="notemerged")
      } else {
        stop("bug: should be one row")
      }
    }
  })
  
  senseGoldPredemrg<-ddply(senseemrg, c("class", "target") ,function(sense){
    
    selectedpred <-  goldpredEmergenceData[as.character(goldpredEmergenceData$CUI)==as.character(sense$class) & as.character(goldpredEmergenceData$WSD_INFORMATION) == as.character(sense$target),]
    
    if (selectedpred$PREDICTED_EMERGENCE_YEAR!=0) {
      cbind(sense, PredEmergence="emerged", EmergenceEvalStatus=selectedpred$EvalStatus)
    } else {
      if (selectedpred$PREDICTED_EMERGENCE_YEAR==0) {
        cbind(sense, PredEmergence="notemerged", EmergenceEvalStatus=selectedpred$EvalStatus)
      } else {
        stop("bug: should matched in both files")
      }
    }
     
  })
  senseGoldPredemrg
}





plot_senseperfEm<-function(SenseEmData){
  
  ggplot(SenseEmData,aes(x=prop.gold,y=f1score))+geom_point()+facet_grid(. ~ GoldEmergence) 
}


plot_NeoScansenseperfEmgoldpred<-function(NeoScanSenseEmData, PathResults=NULL){
  
  neoscan_emr_propgold_f1score<- ggplot(NeoScanSenseEmData,aes(x=prop.gold,y=f1score,color=EmergenceEvalStatus))+geom_point(alpha=.6)+facet_grid(system ~ GoldEmergence)
  #neoscan_emr_propgold_f1score.pdf
  
  if(!is.null(PathResults)){
    r<-paste(PathResults, "neoscan_emr_propgold_f1score.pdf", sep="/")
    ggsave(r,  neoscan_emr_propgold_f1score ,device='pdf')
  } 
  neoscan_emr_propgold_f1score 
}


#plot_NeoScansenseperfNumSensegoldpred<-function(NeoScanSenseNumSensesData, PathResults=NULL){
  
 # neoscan_num_propgold_f1score<- ggplot(NeoScanSenseNumSensesData,aes(x=prop.gold,y=f1score,color=nbSensesGold))+geom_point(alpha=.8)+scale_colour_discrete(name="NumGoldSenses")+facet_grid(system ~ GoldEmergence)
  #neoscan_emr_propgold_f1score.pdf
  
 # if(!is.null(PathResults)){
#    r<-paste(PathResults, "neoscan_num_propgold_f1score.pdf", sep="/")
#    ggsave(r,  neoscan_num_propgold_f1score ,device='pdf')
#  } 
#  neoscan_num_propgold_f1score 
#}

plot_senseperfEmgoldpred<-function(SenseEmData){
  ggplot(SenseEmData,aes(x=prop.gold,y=f1score,color=EmergenceEvalStatus))+geom_point(alpha=.6)+facet_grid(. ~ GoldEmergence) 
}


plot_senseperfEmgoldpredwithMAE<-function(SenseEmDataMAE, breaks=seq(from=0,to=1,by=.1), PathResults=NULL){
  
  SenseEmDataMAE$binMAE <- cut(SenseEmDataMAE$MAE,breaks)
  fscore_propGold_EvalEmergence_emergence <-ggplot(SenseEmDataMAE,aes(x=prop.gold,y=f1score,color=EmergenceEvalStatus))+geom_point(alpha=.6)+facet_grid(binMAE ~ GoldEmergence) 
  
  if(!is.null(PathResults)){
    r<-paste(PathResults, "fscore_propGold_EvalEmergence_emergence.pdf", sep="/")
    ggsave(r,  fscore_propGold_numSenes_emergence ,device='pdf')
  } 
  fscore_propGold_EvalEmergence_emergence
}



#plot_senseperfEmgoldpredwithMAE(neosempmae,seq(0,1,.1) )
plot_senseperfEmgoldNumSensewithMAE<-function(SenseEmDataMAE, breaks=seq(from=0,to=1,by=.1), PathResults=NULL){
  SenseEmDataMAE$binMAE <- cut(SenseEmDataMAE$MAE,breaks)
  SenseEmDataMAE$nbSensesGold<- as.factor(SenseEmDataMAE$nbSensesGold)
  fscore_propGold_numSenes_emergence <-ggplot(SenseEmDataMAE,aes(x=prop.gold,y=f1score,color=nbSensesGold))+geom_point(alpha=.8)+scale_colour_discrete(name="NumGoldSenses")+facet_grid(binMAE ~ GoldEmergence) 
  if(!is.null(PathResults)){
  r<-paste(PathResults, "fscore_propGold_numSenes_emergence.pdf", sep="/")
  ggsave(r,  fscore_propGold_numSenes_emergence ,device='pdf')
  } 
  fscore_propGold_numSenes_emergence
}


plot_senseperfNumSensewithMAEONLY<-function(SenseEmDataMAE,PathResults=NULL){
  SenseEmDataMAE$nbSensesGold<- as.factor(SenseEmDataMAE$nbSensesGold)
  propGold_numSenes_MAE <-ggplot(SenseEmDataMAE,aes(x=prop.gold,y=MAE,color=nbSensesGold))+geom_point(alpha=.8)+scale_colour_discrete(name="NumGoldSenses")+facet_grid(. ~ GoldEmergence) 
  if(!is.null(PathResults)){
    r<-paste(PathResults, "propGold_numSenes_MAE.pdf", sep="/")
    ggsave(r,  propGold_numSenes_MAE ,device='pdf')
  } 
  propGold_numSenes_MAE

  
}


#plot_senseperf<-function(SenseData){
  
 # ggplot(SenseData,aes(x=prop.gold,y=NormalisedError))+geom_point()+facet_grid(EvalStatus ~ systemName) 
  
#}




NeoScanjoin<-function(NeoData, ScanData){
  
  NeoData$system<-rep("NEO", nrow(NeoData))
  ScanData$system<-rep("SCAN", nrow(ScanData))
  NeoScanjoint<- rbind(NeoData,ScanData)
  NeoScanjoint
}

plot_propgoldproppred_doublehist<-function(NeoSenseData, ScanSenseData){
  NeoScanSensejoint<-NeoScanjoin(NeoSenseData, ScanSenseData)
mNeoScansense<-melt(NeoScanSensejoint,measure.vars=c("prop.gold", "prop.Pred"), variable.name= "variable",value.name="sensepropotion")
ggplot(mNeoScansense, aes(x=sensepropotion, fill=variable))+geom_histogram(position='identity',alpha=0.5)+facet_grid(system ~.)
}


plot_propgoldproppred <-function(SenseData, PathResults=NULL){
  
  hist_top <- ggplot()+geom_histogram(aes(SenseData$prop.gold))+theme_grey(base_size=12)+theme(axis.title.x=element_blank(),axis.text.x=element_blank())
  empty <- ggplot()+geom_point(aes(1,1), colour="white")+
    theme(axis.ticks=element_blank(), 
          panel.background=element_blank(), 
          axis.text.x=element_blank(), axis.text.y=element_blank(),           
          axis.title.x=element_blank(), axis.title.y=element_blank())
  SenseData$nbSensesGold <- as.factor(SenseData$nbSensesGold)
  scatter <- ggplot()+geom_point(aes(SenseData$prop.gold, SenseData$prop.Pred, color=SenseData$nbSensesGold))+theme(legend.position = c(0.62, 0.07),legend.text=element_text(size=10),legend.direction = "horizontal")+xlab("Gold propotion")+ylab("Prediction propotion")+scale_colour_discrete(name="NumGoldSenses") + theme(legend.title = element_text(size=10))
  hist_right <- ggplot()+geom_histogram(aes(SenseData$prop.Pred))+coord_flip()+theme_grey(base_size=12)+theme(axis.title.y=element_blank(),axis.text.y=element_blank())
  goldpredpropotionpointandhist  <-grid.arrange( hist_top, empty , scatter,hist_right, ncol=2, nrow=2, widths=c(4, 1), heights=c(1, 4))
 
  if(!is.null(PathResults)){
    r<-paste(PathResults, "goldpredpropotionpointandhist.pdf", sep="/")
    ggsave(r,  goldpredpropotionpointandhist ,device='pdf')
  }
  goldpredpropotionpointandhist
}




plot_propgoldproppredEMNLP <-function(NeosenseDF, ScansenseDF ,PathResults=NULL,fontsize=16){
  
  hist_top <- ggplot()+geom_histogram(aes(NeosenseDF$prop.gold))+theme_grey(base_size=fontsize)+theme(axis.title.x=element_blank(),axis.text.x=element_blank())
  
  empty <- ggplot()+geom_point(aes(0,0), colour="white")+
    theme(axis.ticks=element_blank(), 
          panel.background=element_blank(), 
          axis.text.x=element_blank(), axis.text.y=element_blank(),           
          axis.title.x=element_blank(), axis.title.y=element_blank())
  
  NeosenseDF$nbSensesGold <- as.factor(NeosenseDF$nbSensesGold)
  Neoscatter <- ggplot()+geom_point(aes(NeosenseDF$prop.gold, NeosenseDF$prop.Pred, color=NeosenseDF$nbSensesGold))+theme_grey(base_size=fontsize)+theme(legend.position = c(0.62, 0.07),legend.text=element_text(size=12),legend.direction = "horizontal")+xlab("Gold proportion")+ylab("Prediction proportion")+scale_colour_discrete(name="Num Gold Senses") + theme(legend.title = element_text(size=12))
  Neohist_right <- ggplot()+geom_histogram(aes(NeosenseDF$prop.Pred))+coord_flip()+theme_grey(base_size=fontsize)+theme(axis.title.y=element_blank(),axis.text.y=element_blank())
  
  ScansenseDF$nbSensesGold <- as.factor(ScansenseDF$nbSensesGold)
  Scanscatter <- ggplot()+geom_point(aes(ScansenseDF$prop.gold,  ScansenseDF$prop.Pred, color= ScansenseDF$nbSensesGold))+theme_grey(base_size=fontsize)+theme(legend.position = c(0.62, 0.07),legend.text=element_text(size=12),legend.direction = "horizontal")+xlab("Gold proportion")+ylab("Prediction proportion")+scale_colour_discrete(name="Num Gold Senses") + theme(legend.title = element_text(size=12))
  Scanhist_right <- ggplot()+geom_histogram(aes(ScansenseDF$prop.Pred))+coord_flip()+theme_grey(base_size=fontsize)+theme(axis.title.y=element_blank(),axis.text.y=element_blank())
  
  goldpredpropotionpointandhist  <-grid.arrange( hist_top, empty,Neoscatter,Neohist_right, Scanscatter,Scanhist_right,ncol=2, nrow=3, widths=c(6, 1), heights=c(1,4 ,4))
  
  if(!is.null(PathResults)){
    r<-paste(PathResults, "goldpredpropotionpointandhistNeoScan.pdf", sep="/")
    ggsave(r,  goldpredpropotionpointandhist ,device='pdf')
  }
  goldpredpropotionpointandhist
}



















calculate_percentagebytarget<-function(targetsNeoData, targetsScanData, VariableOne=NULL, VariableTwo=NULL, onemin=0,onemax=1 ,twomin=0,twomax=1)
{

subNeotargets<-targetsNeoData[targetsNeoData[,VariableOne]>=onemin& targetsNeoData[,VariableOne]<=onemax &targetsNeoData[,VariableTwo]>=twomin   &targetsNeoData[,VariableTwo]<=twomax,]
subScantargets<-targetsScanData[targetsScanData[,VariableOne]>=onemin& targetsScanData[,VariableOne]<=onemax &targetsScanData[,VariableTwo]>=twomin &targetsScanData[,VariableTwo]<=twomax,]


percentageNeotarget  <- (nrow(subNeotargets)/nrow(targetsNeoData))*100
percentageScantarget <- (nrow(subScantargets)/nrow(targetsScanData))*100 

print(paste0("percentage of NEO targets : ", percentageNeotarget))
print(paste0("percentage of SCAN targets : ", percentageScantarget))
print(paste0("NEO targets are : ", subNeotargets[,"target"]))
print(paste0("SCAN targets are : ", subScantargets[,"target"]))


# tragetsincommon<-lapply(subNeotargets[,"target"], function(target){
# 
#   if(target %in% subScantargets[,"target"]){
#     
#     target
#   }
#   
# })
# 
# tragetsincommon
# }


tragetsincommon<-ddply(subNeotargets,"target", function(targetDF){
  
  target <- targetDF[1,"target"]

     scanRes <- subScantargets[target==subScantargets[,"target"],c("target",VariableOne)]
    if (nrow(scanRes) >0) {
      cbind(scanRes, macfscoreNeo = targetDF[1,VariableOne])
    }
})

tragetsincommon

#tragetsincommon/nrow(tragetsincommon)

}

# by target dataframe for Neo and Scan
mergeNeoandScan<-function(NeoTargetData, ScanTargetData,breaks=c(0,.1,.2,.3,.4,.5),stdDevColName="strdGoldprop", PathResults=NULL)
{
  
  NeoTargetData$system<-rep("NEO", nrow(NeoTargetData))
  ScanTargetData$system<-rep("SCAN", nrow(ScanTargetData))
  
  NeoScanjoint<- rbind(NeoTargetData,ScanTargetData)
  NeoScanjoint$stdDevGoldPropBin <- cut(NeoScanjoint[,stdDevColName],breaks)
  
 TargetSTDgoldpropMacFscore<- ggplot(NeoScanjoint,aes(stdDevGoldPropBin,macfscore,fill=system))+geom_boxplot()+xlab("SD Gold Proportion")+ylab("macro F1 score")+theme_gray(base_size = 15)
  #ggplot(NeoScanjoint,aes(stdDevGoldPropBin,macfscore,fill=system))+geom_violin()
  #mergeNeoandScan(neot,scant,breaks=seq(from=0,to=.5,by=.05))
  
  if(!is.null(PathResults)){
    r<-paste(PathResults, "SDropGold_macfscore_bytargets_NeoScan.pdf", sep="/")
    ggsave(r,  TargetSTDgoldpropMacFscore ,device='pdf')
  }
 TargetSTDgoldpropMacFscore 
}


plot_MAE_FSCORE_with_CUI<-function(SenseEmDataMAE, minMAE=0, maxMAE=1){

  df <- SenseEmDataMAE[SenseEmDataMAE$MAE >= minMAE & SenseEmDataMAE$MAE <= maxMAE,]
  ggplot(df, aes(x=reorder(class, MAE),y=f1score,fill=GoldEmergence))+geom_col()+theme(axis.text.x = element_text(angle = 90, hjust = 1))

}

plot_MAE_FSCORE_withCUIByBin<-function(SenseEmDataMAE,breaks=seq(from=0,to=1,by=.1)){
  
  SenseEmDataMAE$binMAE <- cut(SenseEmDataMAE$MAE,breaks)
  l<-llply(levels(SenseEmDataMAE$binMAE), function(intervalMAE) {
    df <- SenseEmDataMAE[SenseEmDataMAE$binMAE == intervalMAE,]
    ggplot(df, aes(x=reorder(class, MAE),y=f1score,fill=GoldEmergence))+geom_col()+theme(axis.text.x = element_text(angle = 90, hjust = 1))+ggtitle(intervalMAE)
  })
  l
  
}



choose_sensesBy_MAE_EvalMeasure<-function(senseData, minMAE, maxMAE,breaks=seq(from=0,to=1,by=.1)){
  
 MAEdf<- senseData[senseData$MAE >=minMAE & senseData$MAE <=maxMAE,]
  
 MAEdf$binFscore <- cut(MAEdf$f1score,breaks)
 
 intervalsdf<-llply(levels(MAEdf$binFscore), function(intervalFscore){
   df <- MAEdf[MAEdf$binFscore == intervalFscore,]
  df 
 })
  
 intervalsdf 
}



add_classification_status <- function(df, class, goldColumnName='CUI', predColumnName='Predicted_CUI_based_on_Match'){
  
  df$classifcationStatus <- case_when(
    df[,goldColumnName] == class & df[,predColumnName] == class ~ "TP",
    df[,goldColumnName] != class & df[,predColumnName] == class ~ "FP",
    df[,goldColumnName] == class & df[,predColumnName] != class ~ "FN",
    df[,goldColumnName] != class & df[,predColumnName] != class ~ "TN"
  )
  df
  
}


#check_outliers_byinstances<-function(senseData,TargetData){
  
  
  
  
  
  
  
  
#}


 # investigate_by_senses<-function(TargetsDir){
 # 
 # 
 #   pattern<- paste0(TargetsDir,'/*/matchedGoldPred*/Gold_Predicted_InstancesMatch.tsv')
 #   inputFiles <- Sys.glob(pattern)
 # 
 #   print(inputFiles)
 #  stop()
 # 
 #   intrvDF<-choose_sensesBy_MAE_EvalMeasure(senseData, minMAE=0.0, maxMAE=0.3,breaks=seq(from=0,to=1,by=.1))
 # 
 # 
 #   # see the index of elements which have failed
 #  # which(vapply(intrvDF, is.null, logical(1)))
 # 
 #   # remove NULL entries
 # 
 #   #  intrvDF <- compact(NULL)
 # 
 # 
 #  # i<-1
 #  # while(i <= length(intrvDF) & nrow(intrvDF[[i]]) == 0){
 #  #   i<-i+1
 #  # }
 # 
 # 
 #   <-llply(intrvDF,function(intrvSenseDF){ # this loop goes over the bins
 # 
 #    intrvSenseDF<- intrvSenseDF[order(intrvSenseDF$f1score),]
 # 
 #     if(nrow(intrvSenseDF) > 0 ){
 # 
 #     ddply(intrvSenseDF, c("target","class"),function(senseDF){ # we loop within an interval by sense
 # 
 # 
 # 
 #       targetname <-senseDF$target
 #       senseName <- senseDF$class
 # 
 #       filename <- grep(paste0('/',targetname,'.'),inputFiles,fixed = TRUE,value=TRUE)
 #      if (length(filename)==1) {
 # 
 # 
 # 
 # 
 # 
 # 
 #       } else {
 #        print(filename)
 #        stop('Error: should find exactly one filename')
 #       }
 # 
 #     })
 # 
 #     }
 # 
 #   })
 # 
 # 
 # # for(i in length(intrvDF)){
 # 
 # 
 # 
 # 
 # 
 #   #}
 # #  <-llply(intrvDF, )
 # 
 # 
 # 
 # 
 # }



check_outliers_byinstances<-function(senseData,TargetData){
  
  intrvDF<-choose_sensesBy_MAE_EvalMeasure(senseData, minMAE=0.0, maxMAE=0.3,breaks=seq(from=0,to=1,by=.1))
  
   i<-1
  while(i <= length(intrvDF) & nrow(intrvDF[[i]]) == 0){
    i<-i+1
  }
  
   DF<-intrvDF[[i]] ## do a loop to loop all of them
 
  DF<- DF[order(DF$f1score),]
 
  print(DF[1,])
   #c<-1
 # for(c in nrow(DF)){
     
  #  ldply(,)
                         
  class<-DF[1,"class"]
  
 # print(DF[1,])
#  print(colnames(DF))
 # print(DF[1,"target"])
  #print(class)
  
 errorbytarget<- errorByInstanceDF(TargetData, goldColumnName='CUI', predAsCUIColumnName='Predicted_CUI_based_on_Match', predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER',predProbsColumnName='Inferred_Posterior_by_Sense')

 print(evalMAE(errorbytarget))
# print(head(errorbytarget))
 
 #errorbytarget<-errorbytarget[errorbytarget$CUI==class,]
 
 
 
 errorbyclass<-add_classification_status(errorbytarget, class, goldColumnName='CUI', predColumnName='Predicted_CUI_based_on_Match')
 
# stop()
 
 errorbyclass$indivErrorBin <-  cut(errorbyclass$error,seq(0,1,.05))


 binCounts <- ddply(errorbyclass,c('classifcationStatus','indivErrorBin') ,function(smallDF){
  data.frame(count=nrow(smallDF))
   
 })
 # }
 ggplot(binCounts , aes(x =indivErrorBin , y = count, fill =classifcationStatus ))+ geom_col( position = "stack")+theme(axis.text.x = element_text(angle = 90, hjust = 1))
 
 
 
}








perf_by_minmaxprop<-function(NeoSenseData, ScanSenseData){
  
  NeoSenseData$system <- rep("NEO", nrow(NeoSenseData))
  ScanSenseData$system <- rep("SCAN", nrow(ScanSenseData)) 
  SenseData<-  rbind(NeoSenseData,ScanSenseData)
  
  sensesbypropandposition  <-ddply(SenseData, c("target", "system"), function(sensesdf){# all the senses for a single target labelling them by ascending order
    
  sensesorderedbyprop  <-sensesdf[order(sensesdf$prop.gold),]
    
  sensesorderedbyprop$position <-seq(1,nrow(sensesorderedbyprop))
  
  sensesorderedbyprop
  })
  
    
    summary<-ddply(sensesbypropandposition, c("nbSensesGold", "position", "system"), function(nsensedf){
    data.frame(meanFscore=mean(nsensedf$f1score))
      #dcast(summary,nbSensesGold+position~system)  # to reshape the table after returing the data frame from ddply
    })
    
  

#     #nop dcast(sensedata,nbSensesGold+position~system) 
     allcases <- ldply(c('NEO','SCAN'), function(system) {
       first <- mean(sensesbypropandposition[sensesbypropandposition$position==1 & sensesbypropandposition$system==system ,]$f1score)
       last <- mean(sensesbypropandposition[sensesbypropandposition$position==sensesbypropandposition$nbSensesGold & sensesbypropandposition$system==system ,]$f1score)
       all <- data.frame(nbSensesGold=c(0,0),position=c(1,10), system=c(system,system),meanFscore=c(first, last))
       all
     })
     
   # allcases

     rbind(summary, allcases)


# # # nop   summary
   # we want to do a paired test, this is why we have to align the rows for NEO and SCAN by target+sense pair.
   # This is done with dcast which 'merges' the NEO rows with the SCAN rows
   # the selection of columns in ddply is meant to avoid any confusion about the other unused columns in the merging.
     
   pvaluesDF <- ddply(sensesbypropandposition[,c('target','class',"nbSensesGold", "position",'system','f1score')], c("nbSensesGold", "position"), function(nsensedf){
   print(head(nsensedf))
   df<-dcast(nsensedf,value.var='f1score', target+class+nbSensesGold+position~system) 
   print(head(df))
   print(nrow(df))
   wil<-wilcox.test(df$NEO, df$SCAN,alternative='two.sided', paired = TRUE,exact=TRUE)
   wil$p.value
      
     })
 
     df <- dcast(sensesbypropandposition[,c('target','class',"nbSensesGold", "position",'system','f1score')],value.var='f1score', target+class+nbSensesGold+position~system)
     firstNEO <-df[df$position==1,]$NEO
    firstSCAN <-df[df$position==1,]$SCAN
    lastNEO <- df[df$position==df$nbSensesGold,]$NEO
   lastSCAN <- df[df$position==df$nbSensesGold,]$SCAN
   wilFirst<-wilcox.test(firstNEO, firstSCAN,alternative='two.sided', paired = TRUE,exact=TRUE)
     wilLast<-wilcox.test(lastNEO, lastSCAN,alternative='two.sided', paired = TRUE,exact=TRUE)
     all <- data.frame(nbSensesGold=c(0,0),position=c(1,10), V1=c(wilFirst$p.value,wilLast$p.value))
     rbind(pvaluesDF,all)
#  # nop   df
     
}



test_macromicro_bytarget<-function(NeotargetData, ScantargetData){
  
 NeoScant<- merge(NeotargetData,ScantargetData,by='target',suffixes = c('.NEO','.SCAN'))
 wilcox.test(NeoScant$macfscore.NEO,NeoScant$macfscore.SCAN,paired=TRUE,alternative = 'two.sided')
 wilcox.test(NeoScant$micfscore.NEO,NeoScant$micfscore.SCAN,paired=TRUE,alternative = 'two.sided') 
 
 
}






label_senses_pertarget_byascending<-function(SenseData){

 # sensesbypropandposition  <-ddply(SenseData, c("target", "system"), function(sensesdf){# all the senses for a single target labelling them by ascending order
    
 sensesbypropandposition  <-ddply(SenseData, c("target"), function(sensesdf){
    sensesorderedbyprop  <-sensesdf[order(sensesdf$prop.gold),]
    sensesorderedbyprop$position <-seq(1,nrow(sensesorderedbyprop))
    sensesorderedbyprop
  })
  
 sensesbypropandposition
}






perf_largesmallsenses<-function(NeoSenseData, ScanSenseData){
  
  NeoSenseOrder <- label_senses_pertarget_byascending(NeoSenseData)
  ScanSenseOrder <- label_senses_pertarget_byascending(ScanSenseData)
  SenseData <- NeoScanjoin( NeoSenseOrder, ScanSenseOrder)
  
 
  propallstatus <- ddply(SenseData, "system",function(systemData){
 #   print(nrow(systemData))
  first <- systemData[systemData$position==1, ]
   increaseF <- first[first$PredGolddiff > 0,]
   decreaseF <- first[first$PredGolddiff < 0,]
   sameF <-  first[first$PredGolddiff == 0,]
   firstDF <- data.frame(increase=nrow(increaseF),
                         decrease=nrow(decreaseF),
                         same= nrow(sameF))
 #  print(firstDF)
   last <- systemData[systemData$position==systemData$nbSensesGold, ]
   increaseL<-last[last$PredGolddiff > 0, ]
   decreaseL<-last[last$PredGolddiff < 0, ]
   sameL<-last[last$PredGolddiff == 0, ]
    lastDF <- data.frame(increase=nrow(increaseL),
                         decrease=nrow(decreaseL),
                         same=nrow(sameL))
  #  print(firstDF)
    
  propsystemstatus <- ldply(c('increase', 'decrease', 'same'), function(props) {
    propstatus<-  data.frame(nbSensesGold=c(0,0),position=c(1,10), system=c(systemData[1,"system"],systemData[1,"system"]),propstatus=c(props,props), freq=c(firstDF[,props], lastDF[,props]))
    propstatus
     })
  propsystemstatus
    
    
  })
  
#  propallstatus
  
 nSenses<-ddply(SenseData, c("nbSensesGold", "position", "system"), function(nsensedf){
   
   increase <- nsensedf[nsensedf$PredGolddiff > 0,]
   decrease <- nsensedf[nsensedf$PredGolddiff < 0,]
   same <-  nsensedf[nsensedf$PredGolddiff == 0,]
   senseposDF <- data.frame(propstatus=c('increase','decrease','same'), freq=c(nrow(increase),nrow(decrease), nrow(same)))
   senseposDF 
  })
  
  
 allsensesfreq<-rbind(nSenses, propallstatus)
  
 allsensesfreq
}


chisqtestSensePos <- function(allsensesfreq) {
  
  ddply(allsensesfreq, c('nbSensesGold','system'), function(df) {
   # print(df)
   # stop()
    tableDF <- df[ ,c('position', 'propstatus', 'freq')]
  #  print(tableDF)
  #  stop()
  tableDF2 <- dcast(tableDF,value.var='freq',propstatus ~ position)
  #print(tableDF2)
  # stop()
    tableDF3 <- tableDF2[rowSums(tableDF2[, -1])>0, ] 
  #  print(tableDF3)
   #stop()
    tableMatrix <- as.matrix(tableDF3[,2:ncol(tableDF3)])
    row.names(tableMatrix) <- tableDF3[,1]
    print(df[1,'system'])
    print(tableMatrix)
    
    testres <- chisq.test(tableMatrix)
    print(testres)
#    print(testres$p.value)
    testres$p.value
  })
}



chisqtestSystempos <- function(allsensesfreq){
  
  ddply(allsensesfreq, c('nbSensesGold','position'), function(df) {
    
   # if(df[1,"nbSensesGold"] <5){
    
    tableDF <- df[ ,c('system', 'propstatus', 'freq')] 
    tableDF2 <- dcast(tableDF,value.var='freq',propstatus ~ system)
    #print(tableDF2)
    #stop()
    tableDF3 <- tableDF2[rowSums(tableDF2[, -1])>0, ] 
   # print(tableDF3)
    
    tableMatrix <- as.matrix(tableDF3[,2:ncol(tableDF3)])
    row.names(tableMatrix) <- tableDF3[,1]
    print(tableMatrix)
    
    testres <- chisq.test(tableMatrix)
 #   testres<-fisher.test(tableMatrix)
  #  testres<-GTest(tableMatrix)
    
   # print(testres)
    #    print(testres$p.value)
    testres$p.value
  #  }  
  })
  
}


wilcox_by_sys<-function(NeoSenseData, ScanSenseData){
  
  NeoSenseOrder <- label_senses_pertarget_byascending(NeoSenseData)
  ScanSenseOrder <- label_senses_pertarget_byascending(ScanSenseData)
  SenseData <- NeoScanjoin( NeoSenseOrder, ScanSenseOrder)
  
  
  
  pvaluesDF <- ddply(SenseData[,c('target','class',"nbSensesGold", "position",'system','PredGolddiff')], c("nbSensesGold", "position"), function(nsensedf){
 #   print(head(nsensedf))
    df<-dcast(nsensedf,value.var='PredGolddiff', target+class+nbSensesGold+position~system) # to pair them
    #print(head(df))
   # print(nrow(df))
   # stop("lala")
    
    meanNeo<-mean(df$NEO)
    meanScan<-mean(df$SCAN)
   print( paste0("NEOmean",df[1,"position"],": ",meanNeo))
   print( paste0("SCANmean",df$position[1],": ",meanScan))
    wil<-wilcox.test(df$NEO, df$SCAN,alternative='two.sided', paired = TRUE,exact=TRUE)
    wil$p.value
    
  })
  
  df <- dcast(SenseData[,c('target','class',"nbSensesGold", "position",'system','PredGolddiff')],value.var='PredGolddiff', target+class+nbSensesGold+position~system)
  firstNEO <-df[df$position==1,]$NEO
  firstSCAN <-df[df$position==1,]$SCAN
  lastNEO <- df[df$position==df$nbSensesGold,]$NEO
  lastSCAN <- df[df$position==df$nbSensesGold,]$SCAN
  
  print( paste0("NEOmean First: ", meanfNeo<-mean(firstNEO)))
  print( paste0("NEOmean Last: ", meanlNeo<-mean(lastNEO)))
  print( paste0("SCANmean First: ", meanfScan<-mean(firstSCAN)))
  print( paste0("SCANmean Last: " , meanlScan<-mean(lastSCAN)))
  
  wilFirst<-wilcox.test(firstNEO, firstSCAN,alternative='two.sided', paired = TRUE,exact=TRUE)
  wilLast<-wilcox.test(lastNEO, lastSCAN,alternative='two.sided', paired = TRUE,exact=TRUE)
  all <- data.frame(nbSensesGold=c(0,0),position=c(1,10), V1=c(wilFirst$p.value,wilLast$p.value))
  rbind(pvaluesDF,all)
  #  # nop   df
  
}



wilcox_by_sense<-function(NeoSenseData, ScanSenseData){
  
  
  NeoSenseOrder <- label_senses_pertarget_byascending(NeoSenseData)
  ScanSenseOrder <- label_senses_pertarget_byascending(ScanSenseData)
  SenseData <- NeoScanjoin( NeoSenseOrder, ScanSenseOrder)
  
  #print(typeof(SenseData$position))
  
  pvaluesDF <- ddply(SenseData[,c('target','class',"nbSensesGold", "position",'system','PredGolddiff')], c("nbSensesGold", "system"), function(nsensedf){
    if (nsensedf[1,"nbSensesGold"] < 5) {
   # print(head(nsensedf))
#    ddply(nsensedf, 'target', function(targetDF) {
      
 #   })
    df<-dcast(nsensedf,value.var='PredGolddiff', target+nbSensesGold+system~position) 
    #print(head(df))
    #print(nrow(df))
    
    
    ldply(unique(nsensedf$position),function(pos1){
      ldply(unique(nsensedf$position),function(pos2){ 
        
        if(pos1<pos2){
          wil<-wilcox.test(df[,as.character(pos1)], df[,as.character(pos2)],alternative='two.sided', paired = TRUE,exact=TRUE)
          data.frame(pos1=pos1, pos2=pos2,p.value=wil$p.value)
        }
      })
    })
    }
  })
  
  dataAllSys <- ddply(SenseData[,c('target','class',"nbSensesGold", "position",'system','PredGolddiff')], 'system', function(systemDF) {
   dataAll <-ddply(systemDF, c("target"), function(targetdf){
    first <-targetdf[targetdf$position == 1,] 
    if (nrow(first) != 1) {
      stop("pbm first")
    }
    last <- targetdf[targetdf$position == targetdf[1,"nbSensesGold"],]
    if (nrow(last) != 1) {
      stop("pbm last")
    }
    data.frame(first=first$PredGolddiff, last=last$PredGolddiff)
  })
  wil<-wilcox.test(dataAll[,'first'], dataAll[,'last'],alternative='two.sided', paired = TRUE,exact=TRUE)
  wil$p.value
  })

  #print(wil$p.value)
   
  
  final <-rbind(pvaluesDF, data.frame(nbSensesGold=c(0,0),system=c('NEO','SCAN'),pos1=c(0,0),pos2=c(10,10),p.value=c(dataAllSys[dataAllSys$system=='NEO','V1'],dataAllSys[dataAllSys$system=='SCAN','V1'])))
  final
  
  
  
}











  ### MAIN 
  
  # save arguments in list 'args'
args=commandArgs(trailingOnly=TRUE)
#print(args)

if (length(args) == 2) {
  # collect the specific arguments
  inputDir=args[[1]]
  optGraphs=args[[2]]
#  outputDirPath=args[[2]]
  
    
  pattern<- paste0(inputDir,'/*/matchedGoldPred*/Gold_Predicted_InstancesMatch.tsv')
  inputFiles <- Sys.glob(pattern)
  
  # to sum MAE for all the instances
sumMAE <- 0
totalInstancesALL <- 0

  outputPerfAll <- ldply(inputFiles, function(filename) {
      
    outputDirPath <- dirname(filename)
    
    
    if(dir.exists(outputDirPath)){
      print(paste('input=',filename))
      print(paste('output dir=',outputDirPath))
      
      df <- readInstancesTable(filename)
      target <- df[1,'WSD']
    
      evdf <- evalForAllClasses(df,goldColumnName='CUI', predColumnName='Predicted_CUI_based_on_Match')
      evdf$target <- rep(target,nrow(evdf))
      output1<-paste(outputDirPath, "perf1.csv", sep="/");
      write.table(evdf, output1,quote = FALSE, sep ='\t',row.names = FALSE)
      
      
      mimacdf <- evalMicroMacro(evdf,goldColumnName='CUI', predColumnName='Predicted_CUI_based_on_Match') 
      output2<-paste(outputDirPath, "perf2.csv", sep="/");
      write.table(mimacdf, output2,quote = FALSE, sep = "\t",row.names = FALSE)

      
      alleval<-evalMAE(df, goldColumnName='CUI', predAsCUIColumnName='Predicted_CUI_based_on_Match', predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER',predProbsColumnName='Inferred_Posterior_by_Sense')
      output3<-paste(outputDirPath, "perfreggoldpred3.csv", sep="/")
      write.table(alleval, output3,quote = FALSE, sep = "\t", row.names = FALSE)
      
      errorInstance <- errorByInstanceDF(df, goldColumnName='CUI', predAsCUIColumnName='Predicted_CUI_based_on_Match', predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER',predProbsColumnName='Inferred_Posterior_by_Sense')
      sumMAE <<- sumMAE+sum(errorInstance$error)
      totalInstancesALL <<- totalInstancesALL+nrow(errorInstance)
      print(sumMAE)
      print(totalInstancesALL)
      
      
      if (optGraphs==1) {
        maxpredg<-plotmaxpredprob(df, predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER', maxpredProb='INFERRED_PROBABILITY')
        
        output4<-paste(outputDirPath, "maxpredprob4", sep="/")
        ggsave(output4,maxpredg,device='png')
        
        
        output5<-paste(outputDirPath, "freqmax", sep="/")
        
        freqg<-graphpredFreq(df)   
          ggsave(output5,freqg,device='png')
         
         
        # ggsave(filename=output100, ggplot(freqg, aes(x=wt, y=mpg)) +
        # geom_point(size=2, shape=23) + theme_bw(base_size = 10),
        # width = 5, height = 4, dpi = 300, units = "in", device='png')
          
          
       
        #print("predprobacalc values:")
        #print(head(predprobcalc))
        
        predprobg<-graphpredProb(df)
        output6<-paste(outputDirPath, "PredProb5", sep="/") 
        ggsave(output6,predprobg,device='png')
         
        
        
        probdensity<-plotmaxpredprobdesity(df, predAsIntColumnName='MOST_PROBABLE_INFERRED_SENSE_NUMBER', maxpredProb='INFERRED_PROBABILITY')
         
        output7<-paste(outputDirPath, "maxprobdesity", sep="/")
        ggsave(output7,probdensity,device='png')
        
        
        
        
        
        maxprobyear<-maxprobbyyear(df)
        
        output8<-paste(outputDirPath, "maxprobbyyear", sep="/")
        ggsave(output8,maxprobyear,device='png')
        
        
        
        persense<-maxprobbyyearpersense(df)
        
        output9<-paste(outputDirPath, "maxprobpersense", sep="/")
        
        ggsave(output9,persense,device='png')
        
        maxsense<-maxprobbyyearsense(df)
          
        output10<-paste(outputDirPath, "maxprobsenseonly", sep="/")
        
        ggsave(output10,maxsense,device='png')
      }    

  } else {
    stop(paste0("the directory at this path doesnt exists: ",outputDirPath))
    
  }

    evdf # return perf data frame by sense for each target
  })    
  
  
  outputMAE<-paste(inputDir, "MAEbyInstances.csv", sep="/")
  write.table(data.frame(globalMAE=sumMAE/totalInstancesALL), outputMAE,quote = FALSE, sep = "\t", row.names = FALSE)
  
  ALL_MICROMACRO <- evalMicroMacro(outputPerfAll)
  
  outputALLBySense <- paste0(inputDir,'/perf-all-targets-by-sense.tsv')
  outputALLMicroMacro <- paste0(inputDir,'/perf-all-targets-micro-macro.tsv')
  write.table(outputPerfAll, outputALLBySense,quote = FALSE, sep = "\t",row.names = FALSE)
  write.table(ALL_MICROMACRO, outputALLMicroMacro,quote = FALSE, sep = "\t",row.names = FALSE)
 

  statsbyTarget <- ddply(outputPerfAll, "target", function(subdf){
    
    totalinstances <- sum(subdf$gold.pos)
    micMacDF <-  evalMicroMacro(subdf)
    

  #  data.frame(totalinstances=totalinstances, strdGoldprop=sd(subdf$prop.gold), strdPredprop=sd(subdf$pred.pos/totalinstances),nbSensesGold=subdf[1,"nbSensesGold"], nbSensesPred=subdf[1,"nbSensesPred"], macroprec=micMacDF[micMacDF$evalType=='macro','precision'], macrecall=micMacDF[micMacDF$evalType=='macro','recall'], macfscore=micMacDF[micMacDF$evalType=='macro','f1score'], microprec=micMacDF[micMacDF$evalType=='micro','precision'], micrecall=micMacDF[micMacDF$evalType=='micro','recall'], micfscore=micMacDF[micMacDF$evalType=='micro','f1score'])
    data.frame(totalinstances=totalinstances, strdGoldprop=sdN(subdf$prop.gold), strdPredprop=sdN(subdf$pred.pos/totalinstances),nbSensesGold=subdf[1,"nbSensesGold"], nbSensesPred=subdf[1,"nbSensesPred"], macroprec=micMacDF[micMacDF$evalType=='macro','precision'], macrecall=micMacDF[micMacDF$evalType=='macro','recall'], macfscore=micMacDF[micMacDF$evalType=='macro','f1score'], microprec=micMacDF[micMacDF$evalType=='micro','precision'], micrecall=micMacDF[micMacDF$evalType=='micro','recall'], micfscore=micMacDF[micMacDF$evalType=='micro','f1score'])
    
  })
  
  outputName <- paste0(inputDir,'/stats-by-target.tsv')
  write.table(statsbyTarget[order(statsbyTarget$strdGoldprop),], outputName,quote = FALSE, sep = "\t",row.names = FALSE)
  
  analysisDataByTarget(statsbyTarget, inputDir)
  
  
  
}else {
  print("Wrong number of arguments provided, not executing script.")
  print("Usage: <input dir> <opt print graphs>")
  print("input dir: contains all the target dirs of the neologism/meaning chnage outputs including thier matching outputs")
  print("  <opt print graphs>: 0 =  don't print graphs ; 1 = print graphs")
 
}

  
  
# graph
#  ggplot(x,aes(as.factor(YEAR),error,colour=CUI))+geom_boxplot()
# examples:
# read a file
# d<-readInstancesTable('')
# look at the content (first few lines)
# head(d)
# eval for one class only:
# evalForOneClass(d,'C0007022')
# now for all classes:
# perf <- evalForAllClasses(d)
# print the perf table
# perf
# calculate micro/macro eval:
# evalMicroMacro(d)
# write a data frame to a file, for instance the 'perf' one obtained above:
# write.table(perf, 'perf.csv',quote = FALSE)
