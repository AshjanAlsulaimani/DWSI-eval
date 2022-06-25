library(stringr)
library(ggplot2)
library(plyr)
#library(Tsdist)
library(dtwclust)
library(philentropy)
library("dtw")
library(ggrepel)
library(dplyr)


readTable <- function(filename) {
  # read the table as a "data frame" from the tsv file, with options: header included, separator tab, ignore quotes symbols in the text
  d <-read.table(filename, header=TRUE,sep='\t',quote="",comment.char = "",stringsAsFactors = FALSE)
  d
}


select_predgoldValues <- function(distanceDF, equal=TRUE){
  
  if(equal == TRUE){
    predgoldequal<-distanceDF[distanceDF$pred == distanceDF$gold,]
  }
  predgoldequal
}

## check if all the time you select and use gold
merge_dis_senseData<-function(distanceDF, senseOrTargetData, bySense=TRUE){
  
  #print(colnames(distanceDF))
  #print(colnames(senseOrTargetData))
  if (bySense) {
    mergedDistSenseOrTarget<-merge(senseOrTargetData, distanceDF, by.x=c('target', 'class'), by.y=c('target','gold'))
  } else {
    mergedDistSenseOrTarget<-merge(senseOrTargetData, distanceDF, by.x=c('target'), by.y=c('target'))
  }
  mergedDistSenseOrTarget
}

#http://www.sthda.com/english/wiki/ggplot2-point-shapes
plot_cor_Fscore_distanceBysense<-function(distanceDF, senseData,Dismeasure="dtw"){
  
  
  #  predgoldequal  <-select_predgoldValues(distanceDF)
  
  #  mergedDistSense <-merge_dis_senseData(distanceDF, senseData)
  
  if(Dismeasure=="dtw"){
    
    #g<- ggplot( mergedDistSense, aes_string(x= "normaDist_dtw", y= "f1score", color='nbSensesPred'))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
    # g<- ggplot( mergedDistSense, aes_string(x= "normaDist_dtw", y= "prop.gold", color='nbSensesPred'))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
    # g<- ggplot( mergedDistSense, aes_string(x= "normaDist_dtw", y= "PredGolddiff", color='nbSensesPred'))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
    g<- ggplot( mergedDistSense, aes_string(x= "normaDist_dtw", y= "prop.Pred", color='nbSensesPred'))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
    # 
    # g<- ggplot( mergedDistSense, aes(x= "dist_dtw", y= "f1score", color='nbSensesPred'))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
    # g<- ggplot( mergedDistSense, aes(x= "dist_dtw", y= "prop.gold", color='nbSensesPred'))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
    # g<- ggplot( mergedDistSense, aes(x= "dist_dtw", y= "PredGolddiff", color='nbSensesPred'))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
    # g<- ggplot( mergedDistSense, aes(x= "dist_dtw", y= "prop.Pred", color='nbSensesPred'))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
    
    g
  }else{
    
    
    
  }
  g
}



analyse_by_targets<-function(Dirname){
  
  
  
  
}

#distPlotBySenses(disprefsensesDF, numGoldSense="3", rank=TRUE, distcol='mean_dtw', perfcol="f1score", colourcol=NULL, method="dtw", outputPath=inputDir )

orderbyallSenses <-function(disprefsensesDF){
  
  orderedDF <-ddply(disprefsensesDF,"target",function(df){
    
    sensesorderedbyprop  <-df[order(df$prop.gold),]
    sensesorderedbyprop$position <-seq(1,nrow(sensesorderedbyprop)) 
    sensesorderedbyprop
    
  })
  
  
  # orderedDF <-ddply(disprefsensesDF,"target",function(df){
  
  #  sensesorderedbyprop  <-df[order(df$prop.gold),]
  #   sensesorderedbyprop$position <-seq(1,nrow(sensesorderedbyprop)) 
  #   sensesorderedbyprop
  
  # })
  
  orderedDF
  
}













distPlotBySenses <-function(disprefsensesMAEALLorderedDF, rank=TRUE, distcol=NULL, perfcol=NULL, colourcol="position", method=NULL, outputPath=NULL ){
  
  ## order within each target and plot
  
  disprefsensesMAEALLorderedDF[,colourcol] <- as.factor(disprefsensesMAEALLorderedDF[,colourcol])
  
  
  print(method)
  print("all senses")
  print(perfcol)
  
  print('Pearson')
  print(cor( disprefsensesMAEALLorderedDF[,distcol], disprefsensesMAEALLorderedDF[,perfcol]),method='pearson')
  print('Spearman')
  print(cor(disprefsensesMAEALLorderedDF[,distcol], disprefsensesMAEALLorderedDF[,perfcol]),method='spearman')
  
  
  
  g<- ggplot(disprefsensesMAEALLorderedDF, aes_string(x= distcol, y= perfcol, color=colourcol))+geom_point()+scale_color_discrete(name="rank of Senses")+theme_grey(base_size = 20) 
  
  #+  geom_label_repel(aes(label = class),box.padding   = 0.35, point.padding = 0.5,segment.color = 'grey50')+theme_grey(base_size = 20) 
  #geom_smooth(method="lm")
  
  
  if(!is.null(outputPath)){
    r<-paste0(outputPath,"/" ,method, perfcol, distcol,'by-Ranksense.pdf')
    ggsave(r,  g ,device='pdf') 
  }
  
  
  g
  
}














distPlotByNumSenses <-function(disprefsensesorderedDF, numGoldSense="3", rank=TRUE, distcol=NULL, perfcol=NULL, colourcol="position", method=NULL, outputPath=NULL ){
  
  if(numGoldSense!="0"){
    
    orderednumDF<- disprefsensesorderedDF[disprefsensesorderedDF$nbSensesGold== numGoldSense,]
    
    orderednumDF[,colourcol] <- as.factor(orderednumDF[,colourcol])
    
    g<- ggplot(orderednumDF, aes_string(x= distcol, y= perfcol, color=colourcol))+geom_point()+scale_color_discrete(name="rank of Senses")+  geom_label_repel(aes(label = class),
                                                                                                                                                              box.padding   = 0.35, 
                                                                                                                                                              point.padding = 0.5,
                                                                                                                                                              segment.color = 'grey50')+theme_grey(base_size = 20) 
    #geom_smooth(method="lm")
    
    
  }
  
  if(numGoldSense=="0"){
    
    disprefsensesorderedDF[,colourcol] <- as.factor(disprefsensesorderedDF[,colourcol])
    
    g<- ggplot(orderednumDF, aes_string(x= distcol, y= perfcol, color=colourcol))+geom_point()+scale_color_discrete(name="rank of Senses")+  geom_label_repel(aes(label = class),
                                                                                                                                                              box.padding   = 0.35, 
                                                                                                                                                              point.padding = 0.5,
                                                                                                                                                              segment.color = 'grey50')+theme_grey(base_size = 20) 
    #geom_smooth(method="lm")
  }
  
  
  if(!is.null(outputPath)){
    r<-paste0(outputPath,"/" ,method,numGoldSense ,perfcol, distcol,'by-RanksenseNum.pdf')
    ggsave(r,  g ,device='pdf') 
  }
  
  
  g
  
}


distPlotByTarget <- function(mergedDistpref, distcol='mean_dtw',perfcol='macfscore',colourcol='totalinstances', method=NULL, outputPath=NULL) {
  
  #predgoldequal  <-select_predgoldValues(distDF)
  #mergedDistSense <-merge_dis_senseData(predgoldequal, perfDF,FALSE)
  
  mergedDistpref[,colourcol] <- as.factor(mergedDistpref[,colourcol])
  print(method)
  print(perfcol)
  print('Pearson')
  print(cor(mergedDistpref[,distcol],mergedDistpref[,perfcol]),method='pearson')
  print('Spearman')
  print(cor(mergedDistpref[,distcol],mergedDistpref[,perfcol]),method='spearman')
  #ggplot(mergedDistSense,aes_string(distcol, perfcol,colour=colourcol))+geom_point()+scale_color_gradient( trans = "log10",labels=scales::comma_format(),low='#000000',high='#FF0000')+geom_smooth(method='lm')
  g <- ggplot(mergedDistpref,aes_string(distcol, perfcol,colour=colourcol))+geom_point()+scale_color_discrete(name="Num of Senses")+geom_smooth(method='lm')
  
  if(!is.null(outputPath)){
    r<-paste0(outputPath,"/" ,method, perfcol, distcol,'by-target.pdf')
    ggsave(r,  g ,device='pdf')
  }
  g
}


calculateMeanEuclidianDist <- function(euclideanDistBySenseDF, outputFilename=NULL) {
  oneBySenseDF <- euclideanDistBySenseDF[euclideanDistBySenseDF$gold==euclideanDistBySenseDF$pred,]
  byTargetDF <- ddply(oneBySenseDF, 'target', function(targetDF) {
    data.frame(euclideanValueMEAN=mean(targetDF$euclideanValue), normSensebyyearMEAN=mean(targetDF$normSensebyyear))
  })
  if (!is.null(outputFilename)) {
    
    write.table(byTargetDF,file=outputFilename,quote=FALSE,sep='\t',row.names=FALSE)
    
  }
  byTargetDF
}


#gone <- ggplot(d,aes(strdGoldprop, macfscore,color=nbSensesGold))+geom_point()+geom_smooth(method="lm")+theme_grey(base_size = 20)  
#outputgraph<-paste(outputDir, "perfimbalance.pdf", sep="/")
#ggsave(outputgraph,gone,device='pdf')

#g <- ggplot(d,aes(totalinstances, macfscore))+geom_point()+theme_grey(base_size = 20)  
#outputgraph2<-paste(outputDir, "perf-nbinstances.pdf", sep="/")
#ggsave(outputgraph2,g,device='pdf')

#allcor<- data.frame(x=c('strdGoldprop','strdGoldprop','strdGoldprop','strdGoldprop','strdGoldprop','strdGoldprop','totalinstances','totalinstances','totalinstances','totalinstances'),
#                   y=c('macfscore','macfscore','micfscore','micfscore', 'strdPredprop','strdPredprop','macfscore','macfscore','micfscore','micfscore'),
#                   method=c('pearson','spearman','pearson','spearman','pearson','spearman','pearson','spearman','pearson','spearman'),
#                   cor=c(
#                     cor(d$strdGoldprop, d$macfscore),   cor(d$strdGoldprop, d$macfscore,method='spearman'),
##                     cor(d$strdGoldprop, d$micfscore),   cor(d$strdGoldprop, d$micfscore,method='spearman'),
#                    cor(d$strdGoldprop, d$strdPredprop),   cor(d$strdGoldprop, d$strdPredprop,method='spearman'),
#                     cor(d$totalinstances, d$macfscore),   cor(d$totalinstances, d$macfscore,method='spearman'),
#                     cor(d$totalinstances, d$micfscore),   cor(d$totalinstances, d$micfscore,method='spearman')
#                   )




caculate_corr_between_dtw_euclid<-function(dtwDF,euclidDF,bySenses=TRUE, outputPath=NULL){
  
  #euclideanValue dist_dtw
  # normSensebyyear normaDist_dtw
  
  #euclideanValueMEAN mean_dtw
  #normSensebyyearMEAN mean_normalised_dtw 
  
  #print(head(dtwDF))
  dtwDF$method <- rep("dtw",nrow(dtwDF))
  euclidDF$method <- rep("euclid",nrow(euclidDF))
  print("comparison caculate_corr_between_dtw_euclid")
  
  if(bySenses==TRUE){
    
    print('Pearson by senses')
    print(cor(dtwDF[,"normaDist_dtw"],euclidDF[,"normSensebyyear"]),method='pearson')
    print('Spearman by senses')
    print(cor(dtwDF[,"normaDist_dtw"],euclidDF[,"normSensebyyear"]),method='spearman')
    
    #  mergedSDtwEuclid<-merge(dtwDF, euclidDF, by.x=c('target', 'pred'), by.y=c('target','pred'))# look at how gold disapeared
    
    # mergedSDtwEuclid$f1score <- as.factor(mergedSDtwEuclid$f1score)
    
    #maybe the rank high F1 score 
    #  g <- ggplot(mergedSDtwEuclid,aes_string("normSensebyyear", "normaDist_dtw",colour="f1score"))+geom_point()+scale_color_discrete(name="f1score")+geom_smooth(method='lm')
    
    # if(!is.null(outputPath)){
    #    r<-paste0(outputPath,"/" , "normSensebyyear", "normaDist_dtw",'by-senses.pdf')
    #    ggsave(r,  g ,device='pdf')
    #  } 
    
    
  }else{
    
    
    print('Pearson by targets')
    print(cor(dtwDF[,"mean_normalised_dtw"],euclidDF[,"normSensebyyearMEAN"]),method='pearson')
    print('Spearman by targets')
    print(cor(dtwDF[,"mean_normalised_dtw"],euclidDF[,"normSensebyyearMEAN"]),method='spearman')
    
    #  mergedTDtwEuclid<-merge(dtwDF, euclidDF, by.x=c('target'), by.y=c('target'))
    
    #  mergedTDtwEuclid$macfscore <- as.factor(mergedTDtwEuclid$macfscore)
    #  g <- ggplot(mergedTDtwEuclid,aes_string("normSensebyyearMEAN", "mean_normalised_dtw", colour="macfscore"))+geom_point()+scale_color_discrete(name="macfscore")+geom_smooth(method='lm')
    
    # if(!is.null(outputPath)){
    #   r<-paste0(outputPath,"/" , "normSensebyyearMEAN", "mean_normaDist_dtw",'by-targets.pdf')
    #  ggsave(r,  g ,device='pdf')
    # } 
    
  }
  
  # g
  
}



rankand_label_all_possibilities_matching<-function(dtwDisbySenseDF, rankby="normaDist_dtw"){
  
  orderedpossibilitiesDF <-ddply(dtwDisbySenseDF,"target",function(df){
    sensesordered  <-df[order(df[,rankby]),]
    
    head(sensesordered)
    
    sensesordered$distposition <-seq(1,nrow(sensesordered)) 
    sensesordered$matchstatus <- sensesordered[,"pred"] == sensesordered[,"gold"]
    sensesordered
  })
  orderedpossibilitiesDF
  
  
}



select_according_to_ranking_and_label_matching<-function(rankedDistsenses, selectposition="1"){
  
  
  
  #print(nrow(rankedDistsenses))
  #print(rankedDistsenses)
  
  
  
  # DistPossense
  
  
  #  print(nrow(DistPossense))
  
  print("rank 1 and true")
  firstTrue <-rankedDistsenses[rankedDistsenses$matchstatus==TRUE & rankedDistsenses$distposition==selectposition,]
  print(nrow(firstTrue))
  print("rank not one and TRUE")
  notfirst <-rankedDistsenses[rankedDistsenses$matchstatus==TRUE & rankedDistsenses$distposition!=selectposition,]
  print(nrow(notfirst))
  print("F1 score")
  print(mean(firstTrue$f1score))
  print(mean(notfirst$f1score))
  wil<-wilcox.test(firstTrue$f1score, notfirst$f1score,alternative='two.sided', paired = FALSE,exact=TRUE)
  print(wil)
  print("distance")
  print(mean(firstTrue[,"normaDist_dtw"]))
  print(mean(notfirst[,"normaDist_dtw"]))
  wil<-wilcox.test(firstTrue[,"normaDist_dtw"], notfirst[,"normaDist_dtw"],alternative='two.sided', paired = FALSE,exact=TRUE)
  print(wil)
  
  
  
  # distance of the true one in general and f1 score: of course they are not the same distribution
  #print("distance of the true one in general and f1 score")
  #truematched<-rankedDistsenses[rankedDistsenses$matchstatus==TRUE,]
  # print(truematched)
  #wil<-wilcox.test( truematched[,"normaDist_dtw"],  truematched$f1score,alternative='two.sided', paired = FALSE,exact=TRUE)
  #print(wil)
  
  
  #stop("lala")
  DistPossense <-rankedDistsenses[rankedDistsenses$distposition==selectposition,]
  DistPossense 
  
}


analyse_distance_by_senses<-function(disprefsensesMAEALLorderedPOSdtwDF, method="dtw", outputPath=NULL){
  
  # if(!is.null(outputPath)){
  #  g <- ggplot(disprefsensesMAEALLorderedPOSdtwDF, aes_string(x="f1score", fill="matchstatus"))+geom_histogram(alpha=0.5, distposition="identity")+scale_color_discrete(name="matched status")
  #     r<-paste0(outputPath,"/" , "mached", method,'by-senses.pdf')
  #    ggsave(r,  g ,device='pdf')
  # }
  
  
  # print(head(disprefsensesMAEALLorderedPOSdtwDF))
  
  
  print("SIZE TRUE")
  sizeTRUEmatched<-disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus==TRUE,]
  print(length(sizeTRUEmatched$matchstatus))
  print("SIZE FALSE")
  sizeFALSEmatched<-disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus==FALSE,]
  print(length(sizeFALSEmatched$matchstatus))
  
  
  print("WilcoxonTest of F1score")
  print("TRUE:")
  print(mean(disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus,]$f1score))
  
  
  
  
  print("FALSE:")
  print(mean(disprefsensesMAEALLorderedPOSdtwDF[!disprefsensesMAEALLorderedPOSdtwDF$matchstatus,]$f1score))
  wil<-wilcox.test(disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus,]$f1score, disprefsensesMAEALLorderedPOSdtwDF[!disprefsensesMAEALLorderedPOSdtwDF$matchstatus,]$f1score,alternative='two.sided', paired = FALSE,exact=TRUE)
  print(wil$p.value)
  
  
  
  
  
  print("WilcoxonTest of Distance")
  print(mean(disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus==TRUE,]$normaDist_dtw))
  print(mean(disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus==FALSE,]$normaDist_dtw))
  
  wil<-wilcox.test(disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus==TRUE,]$normaDist_dtw, disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus==FALSE,]$normaDist_dtw,alternative='two.sided', paired = FALSE,exact=TRUE)
  print(wil$p.value)
  
  
  print("Correlation ")
  
  # print(head(disprefsensesMAEALLorderedPOSdtwDF))
  # among the selected 
  
  print('Pearson ')
  #  t<-disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus==TRUE,]$normaDist_dtw
  # f<-disprefsensesMAEALLorderedPOSdtwDF[disprefsensesMAEALLorderedPOSdtwDF$matchstatus==FALSE,]$normaDist_dtw
  print(cor(disprefsensesMAEALLorderedPOSdtwDF$normaDist_dtw,disprefsensesMAEALLorderedPOSdtwDF$f1score),method='pearson')
  #print(cor(t,f),method='pearson')
  
  print('Spearman')
  print(cor(disprefsensesMAEALLorderedPOSdtwDF$normaDist_dtw,disprefsensesMAEALLorderedPOSdtwDF$f1score),method='spearman')
  # print(cor(t, f),method='spearman')
  
  g
  
  
}

# the num of senses must be higher than 1, this function works for the num of senses not a general one, use calcuate mean for the general case
calculate_mean_byNumofSenses<-function(df, type="PRED",NumSenses=1){
  
  
  dfAll<- countEmergByPairforGoldandPred(df) 
  if(type=="PRED"){
    # correct selection/prediction:
    # when there is no emergence or emergence in both of them (basically when the predicted match the gold correctly)
    distemergpredDF<- dfAll[dfAll$type=="PRED_AND_GOLD",]
    
    print("CORRECT selection/PREDICTIONS:")
    # case when there is no an emergence in a pair 
    #noDF<-distemergpredDF[distemergpredDF$predAndGold_first==0,]
    zeroDF<-distemergpredDF[distemergpredDF$predAndGold_first==0 & distemergpredDF$predAndGold_second ==0,] 
    resno<- mean(zeroDF$normaDist_dtw)
    
    print( "mean no an emergence")
    print(resno)
    print("size")
    print(nrow(zeroDF))
    
    
    
    # case when there is only an emergence in a pair
    oneDF<-distemergpredDF[(distemergpredDF$predAndGold_first==1 & distemergpredDF$predAndGold_second ==0) | (distemergpredDF$predAndGold_first==0 & distemergpredDF$predAndGold_second ==1) ,] 
    resone<- mean(oneDF$normaDist_dtw)
    print( "mean only an emergence")
    print(resone)
    print("size")
    print(nrow(oneDF))
    
    
    
    if(NumSenses>=3){
      # case when there is only two emergence in a pair
      twoDF<-distemergpredDF[distemergpredDF$predAndGold_first==1 & distemergpredDF$predAndGold_second ==1,] 
      restwo<- mean(twoDF$normaDist_dtw)
      print( "mean only two emergence") 
      print(restwo) 
      
      print("size")
      print(nrow(twoDF))
      # test significant:
      
      wilzerothree<-wilcox.test(zeroDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
      wilonetwo<-wilcox.test(oneDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    }
    
    wilzeroone<-wilcox.test(zeroDF$normaDist_dtw, oneDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    print("Wilx zero one")
    print(wilzeroone$p.value)
    
    
    if(NumSenses>=3){
      print("Wilx zero two")
      print(wilzerothree$p.value)
      print("Wilx one two")
      print(wilonetwo$p.value)
      # correlation with perfpormance
    }
    
    
    # not correct selection/predictions:
    print( " PREDICTIONS ONLY:")
    distemergpredDF<- dfAll[dfAll$type=="PRED",]
    
    
    # case when there is no an emergence in a pair 
    #noDF<-distemergpredDF[distemergpredDF$predAndGold_first==0,]
    zeroDF<-distemergpredDF[distemergpredDF$nbEmergence==0,] 
    resno<- mean(zeroDF$normaDist_dtw)
    
    print( "mean no an emergence")
    print(resno)
    print("size")
    print(nrow(zeroDF))
    
    
    
    # case when there is only an emergence in a pair
    oneDF<-distemergpredDF[distemergpredDF$nbEmergence==1 ,] 
    resone<- mean(oneDF$normaDist_dtw)
    print( "mean only an emergence")
    print(resone)
    print("size")
    print(nrow(oneDF))
    
    
    if(NumSenses>=3){
      # case when there is only two emergence in a pair
      twoDF<-distemergpredDF[distemergpredDF$nbEmergence==2 ,] 
      restwo<- mean(twoDF$normaDist_dtw)
      print( "mean only two emergence") 
      print(restwo) 
      print("size")
      print(nrow(twoDF))
      
    }
    
    
    wilzeroone<-wilcox.test(zeroDF$normaDist_dtw, oneDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    
    if(NumSenses>=3){
      wilzerothree<-wilcox.test(zeroDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
      wilonetwo<-wilcox.test(oneDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    }
    
    print("Wilx zero one")
    print(wilzeroone$p.value)
    
    if(NumSenses>=3){ 
      print("Wilx zero two")
      print(wilzerothree$p.value)
      
      print("Wilx one two")
      print(wilonetwo$p.value)
      
    }
    
    
    print("GOLD EMERGENCE ON PREDICTION DISTANCE ")
    distemergpredgoldDF<- dfAll[dfAll$type=="GOLD",]
    
    #case when there is no an emergence in a pair 
    #noDF<-distemergpredDF[distemergpredDF$predAndGold_first==0,]
    zeroDF<-distemergpredgoldDF[distemergpredgoldDF$nbEmergence==0,] 
    resno<- mean(zeroDF$normaDist_dtw)
    
    print( "mean no an emergence")
    print(resno)
    print("size")
    print(nrow(zeroDF))
    
    
    
    # case when there is only an emergence in a pair
    oneDF<-distemergpredgoldDF[distemergpredgoldDF$nbEmergence==1 ,] 
    resone<- mean(oneDF$normaDist_dtw)
    print( "mean only an emergence")
    print(resone)
    print("size")
    print(nrow(oneDF))
    
    
    if(NumSenses>=3){
      # case when there is only two emergence in a pair
      twoDF<-distemergpredgoldDF[distemergpredgoldDF$nbEmergence==2 ,] 
      restwo<- mean(twoDF$normaDist_dtw)
      print( "mean only two emergence") 
      print(restwo) 
      print("size")
      print(nrow(twoDF))
      wilzerothree<-wilcox.test(zeroDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
      wilonetwo<-wilcox.test(oneDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    }
    
    wilzeroone<-wilcox.test(zeroDF$normaDist_dtw, oneDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    
    print("Wilx zero one")
    print(wilzeroone$p.value)
    if(NumSenses>=3){
      print("Wilx zero two")
      print(wilzerothree$p.value)
      print("Wilx one two")
      print(wilonetwo$p.value)
    }

  }else{
    
    # not correct selection/predictions:
    print( " GOLD ONLY:")
    distemerggoldDF<- dfAll[dfAll$type=="GOLD",]
    
    
    # case when there is no an emergence in a pair 
    zeroDF<-distemerggoldDF[distemerggoldDF$nbEmergence==0,] 
    resno<- mean(zeroDF$normaDist_dtw)
    
    print( "mean no an emergence")
    print(resno)
    print("size")
    print(nrow(zeroDF))
    
    
    
    # case when there is only an emergence in a pair
    oneDF<-distemerggoldDF[distemerggoldDF$nbEmergence==1 ,] 
    resone<- mean(oneDF$normaDist_dtw)
    print( "mean only an emergence")
    print(resone)
    print("size")
    print(nrow(oneDF))
    
    
    if(NumSenses>=3){
      # case when there is only two emergence in a pair
      twoDF<-distemerggoldDF[distemerggoldDF$nbEmergence==2 ,] 
      restwo<- mean(twoDF$normaDist_dtw)
      print( "mean only two emergence") 
      print(restwo) 
      print("size")
      print(nrow(twoDF))
      
      wilzerothree<-wilcox.test(zeroDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
      wilonetwo<-wilcox.test(oneDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    }
     wilzeroone<-wilcox.test(zeroDF$normaDist_dtw, oneDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    
    print("Wilx zero one")
    print(wilzeroone$p.value)
    if(NumSenses>=3){ 
      print("Wilx zero two")
      print(wilzerothree$p.value)
      
      print("Wilx one two")
      print(wilonetwo$p.value)
    }
    
  }
  
}



# this is for the general case of senses num
calculate_mean <- function(df, type="PRED"){
  
  dfAll<- countEmergByPairforGoldandPred(df) 
  if(type=="PRED"){
    # correct selection/prediction:
    # when there is no emergence or emergence in both of them (basically when the predicted match the gold correctly)
    distemergpredDF<- dfAll[dfAll$type=="PRED_AND_GOLD",]
    
    print("CORRECT selection/PREDICTIONS:")
    # case when there is no an emergence in a pair 
    #noDF<-distemergpredDF[distemergpredDF$predAndGold_first==0,]
    zeroDF<-distemergpredDF[distemergpredDF$predAndGold_first==0 & distemergpredDF$predAndGold_second ==0,] 
    resno<- mean(zeroDF$normaDist_dtw)
    
    print( "mean no an emergence")
    print(resno)
    print("size")
    print(nrow(zeroDF))
    # case when there is only an emergence in a pair
    oneDF<-distemergpredDF[(distemergpredDF$predAndGold_first==1 & distemergpredDF$predAndGold_second ==0) | (distemergpredDF$predAndGold_first==0 & distemergpredDF$predAndGold_second ==1) ,] 
    resone<- mean(oneDF$normaDist_dtw)
    print( "mean only an emergence")
    print(resone)
    print("size")
    print(nrow(oneDF))
    # case when there is only two emergence in a pair
    twoDF<-distemergpredDF[distemergpredDF$predAndGold_first==1 & distemergpredDF$predAndGold_second ==1,] 
    restwo<- mean(twoDF$normaDist_dtw)
    print( "mean only two emergence") 
    print(restwo) 
    
    print("size")
    print(nrow(twoDF))
    # test significant:
    
    wilzeroone<-wilcox.test(zeroDF$normaDist_dtw, oneDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    wilzerothree<-wilcox.test(zeroDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    wilonetwo<-wilcox.test(oneDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    
    
    print("Wilx zero one")
    print(wilzeroone$p.value)
    
    print("Wilx zero two")
    print(wilzerothree$p.value)
    
    print("Wilx one two")
    print(wilonetwo$p.value)
    # correlation with perfpormance
    
    
    
    
    
    # not correct selection/predictions:
    print( " PREDICTIONS ONLY:")
    distemergpredDF<- dfAll[dfAll$type=="PRED",]
    
    
    # case when there is no an emergence in a pair 
    #noDF<-distemergpredDF[distemergpredDF$predAndGold_first==0,]
    zeroDF<-distemergpredDF[distemergpredDF$nbEmergence==0,] 
    resno<- mean(zeroDF$normaDist_dtw)
    
    print( "mean no an emergence")
    print(resno)
    print("size")
    print(nrow(zeroDF))
    
    
    
    # case when there is only an emergence in a pair
    oneDF<-distemergpredDF[distemergpredDF$nbEmergence==1 ,] 
    resone<- mean(oneDF$normaDist_dtw)
    print( "mean only an emergence")
    print(resone)
    print("size")
    print(nrow(oneDF))
    
    
    
    # case when there is only two emergence in a pair
    twoDF<-distemergpredDF[distemergpredDF$nbEmergence==2 ,] 
    restwo<- mean(twoDF$normaDist_dtw)
    print( "mean only two emergence") 
    print(restwo) 
    print("size")
    print(nrow(twoDF))
    
    
    
    
    wilzeroone<-wilcox.test(zeroDF$normaDist_dtw, oneDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    wilzerothree<-wilcox.test(zeroDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    wilonetwo<-wilcox.test(oneDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    
    
    print("Wilx zero one")
    print(wilzeroone$p.value)
    
    print("Wilx zero two")
    print(wilzerothree$p.value)
    
    print("Wilx one two")
    print(wilonetwo$p.value)
    
    
    
    
    
    
    
    
    
    print("GOLD EMERGENCE ON PREDICTION DISTANCE ")
    distemergpredgoldDF<- dfAll[dfAll$type=="GOLD",]
    
    #case when there is no an emergence in a pair 
    #noDF<-distemergpredDF[distemergpredDF$predAndGold_first==0,]
    zeroDF<-distemergpredgoldDF[distemergpredgoldDF$nbEmergence==0,] 
    resno<- mean(zeroDF$normaDist_dtw)
    
    print( "mean no an emergence")
    print(resno)
    print("size")
    print(nrow(zeroDF))
    
    
    
    # case when there is only an emergence in a pair
    oneDF<-distemergpredgoldDF[distemergpredgoldDF$nbEmergence==1 ,] 
    resone<- mean(oneDF$normaDist_dtw)
    print( "mean only an emergence")
    print(resone)
    print("size")
    print(nrow(oneDF))
    
    
    
    # case when there is only two emergence in a pair
    twoDF<-distemergpredgoldDF[distemergpredgoldDF$nbEmergence==2 ,] 
    restwo<- mean(twoDF$normaDist_dtw)
    print( "mean only two emergence") 
    print(restwo) 
    print("size")
    print(nrow(twoDF))
    
    
    
    
    
    
    wilzeroone<-wilcox.test(zeroDF$normaDist_dtw, oneDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    wilzerothree<-wilcox.test(zeroDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    wilonetwo<-wilcox.test(oneDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    
    
    print("Wilx zero one")
    print(wilzeroone$p.value)
    
    print("Wilx zero two")
    print(wilzerothree$p.value)
    
    print("Wilx one two")
    print(wilonetwo$p.value)
    
    
    
    
    
    
    
  }else{
    
    
    
    
    # not correct selection/predictions:
    print( " GOLD ONLY:")
    distemerggoldDF<- dfAll[dfAll$type=="GOLD",]
    
    
    # case when there is no an emergence in a pair 
    zeroDF<-distemerggoldDF[distemerggoldDF$nbEmergence==0,] 
    resno<- mean(zeroDF$normaDist_dtw)
    
    print( "mean no an emergence")
    print(resno)
    print("size")
    print(nrow(zeroDF))
    
    
    
    # case when there is only an emergence in a pair
    oneDF<-distemerggoldDF[distemerggoldDF$nbEmergence==1 ,] 
    resone<- mean(oneDF$normaDist_dtw)
    print( "mean only an emergence")
    print(resone)
    print("size")
    print(nrow(oneDF))
    
    
    
    # case when there is only two emergence in a pair
    twoDF<-distemerggoldDF[distemerggoldDF$nbEmergence==2 ,] 
    restwo<- mean(twoDF$normaDist_dtw)
    print( "mean only two emergence") 
    print(restwo) 
    print("size")
    print(nrow(twoDF))
    
    
    
    wilzeroone<-wilcox.test(zeroDF$normaDist_dtw, oneDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    wilzerothree<-wilcox.test(zeroDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    wilonetwo<-wilcox.test(oneDF$normaDist_dtw, twoDF$normaDist_dtw ,alternative='two.sided', paired = FALSE,exact=TRUE)
    
    
    print("Wilx zero one")
    print(wilzeroone$p.value)
    
    print("Wilx zero two")
    print(wilzerothree$p.value)
    
    print("Wilx one two")
    print(wilonetwo$p.value)
    
    
  }
  
  
}


countEmergByPairforGoldandPred <- function(df){
  
  
  dfGold <- countEmergByPair(df,'goldemergence_CUI_first','goldemergence_CUI_second','nbEmergence')
  dfGold$type <- rep('GOLD', nrow(df))
  dfGold$predAndGold_first <- rep("NA", nrow(df))
  dfGold$predAndGold_second <-rep("NA", nrow(df))
  dfPred <- countEmergByPair(df,'predemergence_CUI_first','predemergence_CUI_second','nbEmergence')
  dfPred$type <- rep('PRED', nrow(df))
  dfPred$predAndGold_first <-rep("NA", nrow(df))
  dfPred$predAndGold_second <-rep("NA", nrow(df))
  
  
  
  # when the gold match the pred and they have emergence
  df$predAndGold_first <- case_when (
    df[,'goldemergence_CUI_first']>0 & df[,'predemergence_CUI_first']>0 ~ 1,
    df[,'goldemergence_CUI_first']==0 | df[,'predemergence_CUI_first']==0 ~ 0
  )
  df$predAndGold_second <- case_when (
    df[,'goldemergence_CUI_second']>0 & df[,'predemergence_CUI_second']>0 ~ 1,
    df[,'goldemergence_CUI_second']==0 | df[,'predemergence_CUI_second']==0 ~ 0
  )
  
  
  dfPredAndGold <- countEmergByPair(df,'predAndGold_first','predAndGold_second','nbEmergence')
  dfPredAndGold$type <- rep('PRED_AND_GOLD', nrow(df))
  #dfPredAndGold$predAndGold_first <- NULL
  #dfPredAndGold$predAndGold_second <- NULL
  dfAll <- rbind(dfGold, dfPred, dfPredAndGold) # to make it tiddy
  
  dfAll
}







countEmergByPair <- function(df, emergCol1, emergCol2, resultCol) {
  
  #  print(colnames(df))
  #  print(c(emergCol1, emergCol2, resultCol))
  df[,resultCol] <- case_when(
    df[,emergCol1]>0 & df[,emergCol2]>0 ~ 2,
    (df[,emergCol1]>0 & df[,emergCol2]==0) | (df[,emergCol1]==0 & df[,emergCol2]>0) ~ 1,
    df[,emergCol1]==0 & df[,emergCol2]==0 ~ 0
  )
  df
}



hist_distance_predsenses_emergence <-function(df, outputFilenameStart=NULL, NumbSenses=0,mybreaks=seq(0,1,.05)){
  # print(colnames(df))
  
  dfGold <- countEmergByPair(df,'goldemergence_CUI_first','goldemergence_CUI_second','nbEmergence')
  dfGold$type <- rep('GOLD', nrow(df))
  dfPred <- countEmergByPair(df,'predemergence_CUI_first','predemergence_CUI_second','nbEmergence')
  dfPred$type <- rep('PRED', nrow(df))
  
  
  
  # when the gold match the pred and they have emergence
  df$predAndGold_first <- case_when (
    df[,'goldemergence_CUI_first']>0 & df[,'predemergence_CUI_first']>0 ~ 1,
    df[,'goldemergence_CUI_first']==0 | df[,'predemergence_CUI_first']==0 ~ 0
  )
  df$predAndGold_second <- case_when (
    df[,'goldemergence_CUI_second']>0 & df[,'predemergence_CUI_second']>0 ~ 1,
    df[,'goldemergence_CUI_second']==0 | df[,'predemergence_CUI_second']==0 ~ 0
  )
  
  
  dfPredAndGold <- countEmergByPair(df,'predAndGold_first','predAndGold_second','nbEmergence')
  dfPredAndGold$type <- rep('PRED_AND_GOLD', nrow(df))
  dfPredAndGold$predAndGold_first <- NULL
  dfPredAndGold$predAndGold_second <- NULL
  dfAll <- rbind(dfGold, dfPred, dfPredAndGold) # to make it tiddy
  
  #print(head(dfAll))
  
  
  if(NumbSenses!=0){
    # test me
    message<-  paste0("STATS for ",NumbSenses,"for DISTANCE COMAPRISONS")
    print(message)
    calculate_mean_byNumofSenses(df, type="PRED",NumbSenses)
  }
  
  g<-ggplot(dfAll, aes(normaDist_dtw,fill=as.factor(nbEmergence)))+geom_histogram(alpha=.5,position='identity')+facet_grid(. ~ type)
  #+scale_x_discrete(label=function(x) abbreviate(x, minlength=33)) + facet_grid(. ~ type)
  #  g<-ggplot(dfAll, aes(normaDist_dtw,fill=as.factor(nbEmergence)))+geom_histogram(alpha=.5) + facet_grid(. ~ type)#,position='identity')
  ggsave(paste0(outputFilenameStart,'-hist.pdf'),g)
  
  dfAll$binDist <- cut(dfAll$normaDist_dtw, mybreaks)
  d<-ddply(dfAll, c('binDist','type'), function(subDF) {
    ddply(subDF, as.factor('nbEmergence'), function(subDF2) {
      data.frame(freq=nrow(subDF2),prop=nrow(subDF2)/nrow(subDF))
    })
  })
  g<-ggplot(d, aes(binDist,prop,fill=as.factor(nbEmergence)))+geom_col()+facet_grid(. ~ type)+theme(axis.text.x = element_text(angle = 90))
  ggsave(paste0(outputFilenameStart,'-propbar.pdf'),g)
  
}


hist_distance_predsenses_emergenceMultiNbSenses <-function(df, outputPath=NULL){
  #print(colnames(df))
  hist_distance_predsenses_emergence(df, paste0(outputPath,'/','emerg-pred-dist-gold-ALL'))
  
  #  calculate_mean(df, type="GOLD")
  #  calculate_mean(df, type="PRED")
  
  for (i in 2:3) {
    filteredDF <- ddply(df, 'target', function(subDF) {
      if (((i==2) & nrow(subDF)==1) | ((i==3) & nrow(subDF)==3)) {
        subDF
      }
    })
   # print(nrow(filteredDF))
   # print(filteredDF)
    
    #stop("lalla")
    hist_distance_predsenses_emergence(filteredDF, paste0(outputPath,'/','emerg-pred-dist-gold-',i),NumbSenses=i)
  }
}





#assess_distance_predsenses_emergence <-function(df, outputPath=NULL){
#  for (i in 2:3) {

#  filteredDF <- ddply(df, 'target', function(subDF) {
#     if (((i==2) & nrow(subDF)==1) | ((i==3) & nrow(subDF)==3)) {
#      subDF
#    }
#   })
# }
#}















# propbar_distance_predsenses_emergence <-function(mergedDTWDistpredictedSensesEmergence, mybreaks=seq(0,1,.05),gold=TRUE,outputPath=NULL)   {
# 
#   mergedDTWDistpredictedSensesEmergence$binDist <- cut(mergedDTWDistpredictedSensesEmergence$normaDist_dtw, mybreaks)
#   mergedDTWDistpredictedSensesEmergence$emerged <-  if (gold) {
#                   (mergedDTWDistpredictedSensesEmergence$GOLD_EMERGENCE_YEAR>0)
#   } else {
#               #    (mergedDTWDistpredictedSensesEmergence$PREDICTED_EMERGENCE_YEAR>0)
#     
#     mergedDTWDistpredictedSensesEmergence[,"PREDICTED_EMERGENCE_YEAR"]>0 & mergedDTWDistpredictedSensesEmergence[,"GOLD_EMERGENCE_YEAR"]>0
#     
#   }
#   d<-ddply(mergedDTWDistpredictedSensesEmergence, c('binDist'), function(subDF) {
#     ddply(subDF, 'emerged', function(subDF2) {
#       data.frame(freq=nrow(subDF2),prop=nrow(subDF2)/nrow(subDF))
#     })
#   })
#   g<-ggplot(d, aes(binDist,prop,fill=emerged))+geom_col()
#   if (gold) {
#     ggsave(paste0(outputPath,'/','emerg-pred-dist-stacked-gold.pdf'),g)
#   } else {
#     ggsave(paste0(outputPath,'/','emerg-pred-dist-stacked-pred.pdf'),g)
#   }


#}



#analyse_disatnce_parameters<-function(DF, method="dtw", outputPath=inputDir){






#}




test_sig_global_mean<-function(NeoDisbySenseDF, ScanDisbySenseDF, disCol=NULL,method=NULL){
  
  
  DF <- data.frame(firstsystemMean=character(), secondsystemMean=character(), dismeasure=character(), wilcox=double()) 
  
  predgoldNeoDF<-NeoDisbySenseDF[NeoDisbySenseDF[,"pred"] == NeoDisbySenseDF[,"gold"],]
  
  #print(predgoldNeoDF)
  meanNeo_bySenses<-mean(predgoldNeoDF[,disCol])
  
  predgoldScanDF<-ScanDisbySenseDF[ScanDisbySenseDF[,"pred"]==ScanDisbySenseDF[,"gold"],]
  
  meanSCAN_bySenses<-mean(predgoldScanDF[,disCol])
  
  wil<-wilcox.test(predgoldNeoDF[,disCol], predgoldScanDF[,disCol] ,alternative='two.sided', paired = TRUE,exact=TRUE)
  
  DF <- data.frame(firstsystemMean=meanNeo_bySenses, secondsystemMean=meanSCAN_bySenses, dismeasure= disCol, wilcox= wil$p.value)
  
  results <- paste0("p-value for global mean of scan and neo_",method)
  #print(results)
  
 # print(wil$p.value)
  #wil$p.value
  wil
  DF
}















combine_distance_with_emergence <-function(disdtw_BypredictedSensesFileDF, goldpredEmerFileDF,type="predicted",outputPath=NULL){
  
  disdtw_BypredictedSensesandEmergenceFileDF <-ddply(disdtw_BypredictedSensesFileDF,"target",function(df){
    
    emergoldpredtarget<-  goldpredEmerFileDF[goldpredEmerFileDF$WSD_INFORMATION == df$target,] 
    
    
    sizedf <-nrow(df)
    
    for(i in 1:sizedf){
      
      CUIemerdf_first<- emergoldpredtarget[ emergoldpredtarget$CUI == df[i,"CUI_fisrt"],]
      CUIemerdf_second<- emergoldpredtarget[ emergoldpredtarget$CUI == df[i,"CUI_second"],] 
      
      
      df$predemergence_CUI_first[i]<-CUIemerdf_first[1,"PREDICTED_EMERGENCE_YEAR"]
      
      df$predemergence_CUI_second[i]<-CUIemerdf_second[1,"PREDICTED_EMERGENCE_YEAR"]
      
      
      df$goldemergence_CUI_first[i]<-CUIemerdf_first[1,"GOLD_EMERGENCE_YEAR"]
      
      
      df$goldemergence_CUI_second[i]<-CUIemerdf_second[1,"GOLD_EMERGENCE_YEAR"]
      
    }
    df
  })
  if(!is.null(outputPath) & type=="predicted"){
    outputFilename <-paste0(outputPath, "/", "dtw_within_predicted_emergence_info.tsv")
    write.table(disdtw_BypredictedSensesandEmergenceFileDF, file =outputFilename, sep = "\t", row.names = FALSE, quote=FALSE)  
  }else{
    
    if(!is.null(outputPath) & type=="gold"){
      outputFilename <-paste0(outputPath, "/", "dtw_within_gold_emergence_info.tsv")
      write.table(disdtw_BypredictedSensesandEmergenceFileDF, file =outputFilename, sep = "\t", row.names = FALSE, quote=FALSE)  
      
    }
  }
  disdtw_BypredictedSensesandEmergenceFileDF  
}



claculate_mean_system <-function(DisbySenseDF, method=NULL, disCol=NULL ,outputPath=NULL){
  
  predgoldDF<-DisbySenseDF[DisbySenseDF[,"pred"]==DisbySenseDF[,"gold"],]
  
  meanSyatem_bySenses<-mean(predgoldDF[,disCol])
  
  d<-data.frame(systemMean=meanSyatem_bySenses, Distancemethod=method)
  
  if(!is.null( outputPath)){
    outfilen  <- paste0(outputPath,"/" ,outputFilename="mean_distance","disCol","_Global", method,".tsv")
    write.table(d, file =outfilen , sep = "\t", row.names = FALSE, quote=FALSE)  
  }
  d
}






plot_errorEm_errorDis <-function(DF, distCol=NULL,perfCol=NULL,methodD=NULL,outputPath=NULL){
  
  g <- ggplot(DF,aes_string(x=distCol, y=perfCol))+geom_point() 
  
  
  if(!is.null(outputPath)){
    f<-paste(outputPath, "DistVSERROR.pdf", sep="/")
    ggsave(f, g ,device='pdf')
  }
  g
}



analyse_distance_emergnece <-function(DF,distCol=NULL,perfCol=NULL, methodD=NULL,outputPath=NULL)
{
  
  
  
  # print(DF)
  #goldpredEme <- DF[(DF[,"GOLD_EMERGENCE_YEAR"]!=0   & DF[,"PREDICTED_EMERGENCE_YEAR"]!=0 ) | (DF[,"GOLD_EMERGENCE_YEAR"]==0   & DF[,"PREDICTED_EMERGENCE_YEAR"]==0 ) ,]
  #print(head(goldpredEme))
  #goldprednotok <- DF[(DF[,"GOLD_EMERGENCE_YEAR"]!=0   & DF[,"PREDICTED_EMERGENCE_YEAR"]==0 ) | (DF[,"GOLD_EMERGENCE_YEAR"]==0   & DF[,"PREDICTED_EMERGENCE_YEAR"]!=0 ) ,]
  #print(head(goldprednotok))
  
  #revise this
  goldpredEme <- DF[(DF[,"GOLD_EMERGENCE_YEAR"]!=0   & DF[,"PREDICTED_EMERGENCE_YEAR"]!=0 ) & (DF[,"EvalStatus"]=="TP") ,]
  goldprednotok <- DF[!((DF[,"GOLD_EMERGENCE_YEAR"]!=0   & DF[,"PREDICTED_EMERGENCE_YEAR"]!=0 ) & (DF[,"EvalStatus"]=="TP")),] 
  
  
  
  #goldpredEme <- DF[(DF[,"GOLD_EMERGENCE_YEAR"]!=0   & DF[,"PREDICTED_EMERGENCE_YEAR"]!=0 ) ,]
  # goldprednotok <- DF[!(DF[,"GOLD_EMERGENCE_YEAR"]!=0   & DF[,"PREDICTED_EMERGENCE_YEAR"]!=0 ),] 
  
  
  
  corValueEme<- cor( goldpredEme[,distCol], goldpredEme[,perfCol])
  corValueNOT<- cor( goldprednotok[,distCol], goldprednotok[,perfCol])
  
  
  #x<- cor( DF[,distCol], DF[,perfCol])
  #  y<-cor( DF[,distCol], DF[,perfCol], method='spearman')
  
  print("correlation between Emer error and distance error, first when TP vs rest")
  print(methodD)
  
  
  #print(x)
  
  # print(y)
  
  print(corValueEme)
  
  print(corValueNOT)
  
  
  print("means of the two category")
  print(mean(goldpredEme[,distCol]))
  print(mean(goldprednotok[,distCol]))
  
  
  print(mean(goldpredEme[,perfCol]))
  print(mean(goldprednotok[,perfCol]))
  
  g<-plot_errorEm_errorDis(DF,distCol,perfCol,methodD="dtw",outputPath)
  
  g
  
}




plot_Distsensepropotion_sense<-function(DF,distCol=NULL,method=NULL,outputPath=NULL )
{
  
  DF$EvalStatus <-as.factor(DF$EvalStatus)
  
  g<-ggplot(DF,aes(x=distCol))+geom_bar()+facet_grid(. ~ EvalStatus) 
  
  gg<-ggplot(DF,aes(x=distCol,y=prop.Pred))+geom_point()+facet_grid(. ~ EvalStatus) 
  
  #print(DF)
  #g<-ggplot(DF,aes(distCol))+geom_histogram()
  
  #gg<-ggplot(DF,aes(x=prop.Pred,y=distCol))+geom_point()
  
  if(!is.null(outputPath)){
    
    f<-paste0(outputPath,"/", distCol,method ,"propsenses_hist.pdf")
    ggsave(f ,g ,device='pdf')
    
    ff<-paste0(outputPath,"/", distCol,method ,"propsenses_point.pdf")
    ggsave(ff,gg ,device='pdf')
    
  }
  
}






calculate_dtwmean <- function(dtwDisbySenseDF, outputFilename=NULL){
  
  #  fDF  <- readTable(filename) 
  
  meanDTWDF<- ddply(dtwDisbySenseDF, "target" ,  function(df){
    predgold<-df[df$pred==df$gold,]
    #  print(predgold)
    meanpredgoldnorm <-  mean(predgold$normaDist_dtw)
    meanpredgold   <-  mean(predgold$dist_dtw)
    mDF<- data.frame(mean_normalised_dtw=meanpredgoldnorm, mean_dtw=meanpredgold)
    mDF
  })
  
  if(!is.null(outputFilename)){
    write.table(meanDTWDF, file =outputFilename, sep = "\t", row.names = FALSE, quote=FALSE)  
  }
  
  meanDTWDF
}



calculate_distmean_and_sig <-function(inputanotherDir, outputPath= NULL)
{
print("I'm calling the function calculate_distmean_and_sig")

#test the significant of means of all the current systems that are given in the filelist.
if(inputanotherDir=='' | file.exists(inputanotherDir))
{
  systemspathsDF <-read.table(inputanotherDir, header=FALSE,sep='\t',quote="",comment.char = "",stringsAsFactors = FALSE)  
  numofsystems<-nrow(systemspathsDF)
  systemsdistpathsDF <- data.frame(system=character(),dismeasure=character(),path=character())
 
  for(i in seq(1:numofsystems))    
  {
    sensedisdtwFILE <-paste(systemspathsDF[i,1], "predgold_distance_bySenses.tsv",sep="/")
    sensediseuclidFILE<-paste(systemspathsDF[i,1], "ALLtargetsenses_ecludeDis.tsv",sep="/")
    systemname <-paste(systemspathsDF[i,2])
    
    print(paste("I'm reading the results of the system ",systemname ))
 
    if(!file.exists(sensedisdtwFILE) | !file.exists(sensediseuclidFILE))
    {
      stop("check the files of the DTW and Euclidean distances if they exist!")
    }
    
    sysDTW<-data.frame(system=systemname,dismeasure='DTW',path=sensedisdtwFILE)
    sysEucl<-data.frame(system=systemname,dismeasure='Euclidean',path=sensediseuclidFILE)
    systemdis<-rbind(sysDTW, sysEucl)
    systemsdistpathsDF<- rbind(systemsdistpathsDF, systemdis)
  }
  
  currDF <- data.frame(firstsystem=character(), firstmean=double(), secondsystem=character(), secondmean=double(), dismeasure=character(), wilcox=double())
  currDF <- ddply(systemsdistpathsDF,c('dismeasure'),function(measureDF) 
    {
    ddply(measureDF,'system',function(distOneDF)
  {
    ddply(measureDF,'system',function(distTwoDF)
    { 
      sysnameone<-distOneDF[1,'system']
      sysnametwo<-distTwoDF[1,'system']
      
     # print(sysnameone)
      #print(sysnametwo)
      if(sysnameone!=sysnametwo)
      {
        sensedistOneDF <- readTable(as.character(distOneDF[1,'path']))
        sensedistTwoDF <- readTable(as.character(distTwoDF[1,'path']))
        
        #print("colOne: ")
        #print(colnames(sensedistOneDF))
        #print("colTwo: ")
        #print(colnames(sensedistTwoDF))
        measurename<-distTwoDF[1,'dismeasure']
        if(measurename == 'DTW')
        {
          ressigDF<-test_sig_global_mean(sensedistOneDF, sensedistTwoDF, disCol="normaDist_dtw",method="dtw")
        #  print("im here DTW")
        }
        if(measurename  == 'Euclidean')
        {
          ressigDF<-test_sig_global_mean( sensedistOneDF,  sensedistTwoDF, disCol="normSensebyyear",method="euclid")
        #  print("im here Euclidean")
        }
        #          print(names(ressig))
        currDF<- data.frame(firstsystem=sysnameone, firstmean=ressigDF$firstsystemMean, secondsystem=sysnametwo, secondmean=ressigDF$secondsystemMean, dismeasure=measurename, wilcox=ressigDF$wilcox)
      }
      currDF
      #         finalDF <- rbind(finalDF,currDF)
    })
  })
  })
  

  if(!is.null(outputPath))
  {
    res<-paste(outputPath, "distancemean_withsig_bysense.txt", sep="/")
    write.table(currDF, res,quote = FALSE, sep = "\t",row.names = FALSE)
  }
  
  
}else{
  stop("there is a bug!")
}

}





### MAIN ###

# save arguments in list 'args'
args=commandArgs(trailingOnly=TRUE)
#print(args)

if (length(args) == 4) {
  # collect the specific arguments
  inputDir=args[[1]]
  inputoption=args[[2]] # calculation for equal senses: either zero means "only equal senses" or one "all of them"
  inputEmerDir=args[[3]]
 # inputanotherDir=args[[4]] # the path for another system to compare the results of dwt and euclidean distances replaced by a file lists the paths of other systems 
  inputanotherDir=args[[4]]  
  
  pattern<- paste0(inputDir,'/*')
  inputDirFiles <- Sys.glob(pattern)
  #listFiles=list.files(pattern,recursive=FALSE)
  
  # to calculate the mean of dtw distance by targets and the output is a file
  dtwDisbySenseFile <-grep("predgold_distance_bySenses.tsv",inputDirFiles)
  dtwDisbySenseDF <- readTable(inputDirFiles[[dtwDisbySenseFile]])
  
  if(inputoption==0){
    dtwDisbySenseequalDF  <-select_predgoldValues(dtwDisbySenseDF, equal=TRUE)
    outfilepath  <- paste(inputDir, outputFilename="mean_dtw_distance_byTarget.tsv", sep="/")
    meanDTWDF <-calculate_dtwmean(dtwDisbySenseequalDF, outfilepath )
  }else{
    
    print("specify option")
    stop()
  }
  
  #to calculate the mean of euclidean distance by targets and the output is a file
  euclidDisbySenseFile <-grep("ALLtargetsenses_ecludeDis.tsv",inputDirFiles)
  euclidDisbySenseDF <- readTable(inputDirFiles[[euclidDisbySenseFile]])
  
  if(inputoption==0){
    euclidDisbySenseequalDF  <-select_predgoldValues(euclidDisbySenseDF,equal=TRUE)
    outfilepath  <- paste(inputDir, outputFilename="mean_euclid_distance_byTarget.tsv", sep="/")
    meanEUCLIDF <- calculateMeanEuclidianDist(euclidDisbySenseequalDF, outfilepath)
  }else{
    
    print("specify option")
    stop()
  }
  
  
  # to calculate the mean for the whole system
  
  print(" global mean by dtw")
  globalmeandtw<- claculate_mean_system(dtwDisbySenseDF, method="dtw", disCol="normaDist_dtw",outputPath= inputDir)
  print(globalmeandtw) 
  print(" global mean by euclid")
  globalmeaneuclid<-claculate_mean_system(euclidDisbySenseDF, method="euclid", disCol="normSensebyyear", outputPath= inputDir)
  print(globalmeaneuclid) 
  
  #test the significant of means of the current system and another one: e.g. NEO and SCAN
  #gives a path to the system of interest
#  if(inputanotherDir==''){
 # scansensedisdtwFILE<- paste0(inputanotherDir,"/","predgold_distance_bySenses.tsv")
  #scansensediseuclidFILE<- paste0(inputanotherDir,"/","ALLtargetsenses_ecludeDis.tsv")
  
#  print(scansensediseuclidFILE)
 # if(file.exists(scansensedisdtwFILE) ){
    
  #  if(file.exists(scansensediseuclidFILE)){
  #    scansensedisdtwDF <- readTable(scansensedisdtwFILE)
  #    test_sig_global_mean(dtwDisbySenseDF, scansensedisdtwDF, disCol="normaDist_dtw",method="dtw")
      
  #    scansensediseucDF <- readTable(scansensediseuclidFILE)
  #    test_sig_global_mean(euclidDisbySenseDF, scansensediseucDF, disCol="normSensebyyear",method="euclid")
  #  }
  #}else{
  #  print("file from Edin does not exits")
  #}
  #}
  
  #added 2022
  #test the significant of means of all the current systems that are given in the filelist.
  calculate_distmean_and_sig(inputanotherDir,inputDir)
  
  
  

  #to get the MAE pref
  MAE_BySensesFile <-grep("ALLMAE.tsv",inputDirFiles)
  MAE_BySensesDF <- readTable(inputDirFiles[[MAE_BySensesFile]])
  MAET <- MAE_BySensesDF[MAE_BySensesDF$CUI=="ALL", ]
  MAES <- MAE_BySensesDF[MAE_BySensesDF$CUI!="ALL", ]
 
  
  stop("Second")
  
  # to get the performance by senses
  perf_BySensesFile <-grep("perf-all-targets-by-sense.tsv",inputDirFiles)
  perf_BySensesDF <- readTable(inputDirFiles[[perf_BySensesFile]])
  
  
  # to get the performance by target
  perf_ByTargetsFile <-grep("stats-by-target.tsv",inputDirFiles)
  perf_ByTargetsDF <- readTable(inputDirFiles[[perf_ByTargetsFile]])
  
  
  
  #to get emergence eval file
  EmerFile<-paste0(inputEmerDir, "/goldpredtable.tsv")
  #goldpredEmerFile<- Sys.glob(EmerFile)
  if (basename(EmerFile) != "goldpredtable.tsv") {
    stop(paste('Error: no single match for ',EmerFile))
  }
  #print(EmerFile)
  goldpredEmerFileDF <- readTable(EmerFile)
  
  
  
  
  # merge senseperformance and distane by senses
  if(inputoption==0){
    mergedDTWDistPrefSense <-merge_dis_senseData(dtwDisbySenseequalDF, perf_BySensesDF)
    mergedECLUDDistPrefSense <-merge_dis_senseData(euclidDisbySenseequalDF, perf_BySensesDF)
    
    mergedDTWDistPrefSenseMAE <-merge(mergedDTWDistPrefSense, MAES, by.x=c('target', 'class'), by.y=c('target','CUI'))
    mergedECLUDDistPrefSenseMAE<-merge(mergedECLUDDistPrefSense, MAES, by.x=c('target', 'class'), by.y=c('target','CUI'))
    
    
  }else{
    
    print("specify option")
    stop()
  }
  
  #merge targetperformance and distance senses by targets
  mergedDTWDistPrefTarget <-merge_dis_senseData(meanDTWDF, perf_ByTargetsDF, bySense=FALSE)
  mergedECLUDDistPrefTarget <-merge_dis_senseData(meanEUCLIDF,  perf_ByTargetsDF, bySense=FALSE)
  
  mergedDTWDistPrefTargetMAE <- merge(mergedDTWDistPrefTarget, MAET, by.x=c('target' ), by.y=c('target'))
  mergedECLUDDistPrefTargetMAE <- merge(mergedECLUDDistPrefTarget, MAET, by.x=c('target'), by.y=c('target'))
  
  
  #DTW by targets
  g <- distPlotByTarget(mergedDTWDistPrefTarget , distcol='mean_normalised_dtw',perfcol='macfscore',colourcol='nbSensesGold',method="dtw", outputPath=inputDir) 
  g <- distPlotByTarget(mergedDTWDistPrefTarget , distcol='mean_dtw',perfcol='macfscore',colourcol='nbSensesGold',method="dtw", outputPath=inputDir) 
  
  g <- distPlotByTarget(mergedDTWDistPrefTarget , distcol='mean_normalised_dtw',perfcol='micfscore',colourcol='nbSensesGold',method="dtw", outputPath=inputDir) 
  g <- distPlotByTarget(mergedDTWDistPrefTarget , distcol='mean_dtw',perfcol='micfscore',colourcol='nbSensesGold',method="dtw", outputPath=inputDir) 
  
  
  
  #EUCLID by targets
  g<- distPlotByTarget(mergedECLUDDistPrefTarget , distcol='euclideanValueMEAN',perfcol='macfscore',colourcol='nbSensesGold',method="euclid", outputPath=inputDir)
  g<- distPlotByTarget(mergedECLUDDistPrefTarget , distcol='normSensebyyearMEAN',perfcol='macfscore',colourcol='nbSensesGold',method="euclid", outputPath=inputDir) #?
  
  #euclideanValue dist_dtw
  # normSensebyyear normaDist_dtw
  
  #normSensebyyearMEAN mean_normalised_dtw 
  #euclideanValueMEAN mean_dtw
  
  # concerning all the senses?
  
  disprefsensesMAEALLordereddtwDF <- orderbyallSenses(mergedDTWDistPrefSenseMAE)
  disprefsensesMAEALLorderedeuclidDF <- orderbyallSenses(mergedECLUDDistPrefSenseMAE)
  
  
 View(disprefsensesMAEALLordereddtwDF)
  distPlotBySenses( disprefsensesMAEALLordereddtwDF, rank=TRUE, distcol="normaDist_dtw", perfcol="f1score", colourcol="position", method="dtw", outputPath=inputDir )#?
 
 stop("lalaaaa")  
  
   distPlotBySenses( disprefsensesMAEALLorderedeuclidDF, rank=TRUE, distcol="normSensebyyear", perfcol="f1score", colourcol="position", method="euclid", outputPath=inputDir )
  
 
  
  
  # concerning number of senses
  
  disprefsensesordereddtwDF <- orderbyallSenses(mergedDTWDistPrefSense)
  distPlotByNumSenses(disprefsensesordereddtwDF, numGoldSense="3", rank=TRUE, distcol='normaDist_dtw', perfcol="f1score", colourcol="position", method="dtw", outputPath=inputDir )
 
  disprefsensesorderedeucldDF <- orderbyallSenses(mergedECLUDDistPrefSense)
  distPlotByNumSenses(disprefsensesorderedeucldDF, numGoldSense="3", rank=TRUE, distcol='normSensebyyear', perfcol="f1score", colourcol="position", method="euclid", outputPath=inputDir )
  
  
  

  
  # concerning the propotion of a sense
  
  
  
  
  #no analysing_propotion_sense(disprefsensesordereddtwDF,method=NULL,outputPath=NULL )
  
  mergedDisDTWPrefSensesEmer <-merge(mergedDTWDistPrefSense, goldpredEmerFileDF, by.x=c('target', 'pred' ), by.y=c('WSD_INFORMATION','CUI'))
  mergedDisEUCPrefSensesEmer <-merge(mergedECLUDDistPrefSense, goldpredEmerFileDF, by.x=c('target', 'pred' ), by.y=c('WSD_INFORMATION','CUI')) 
  
  # print(mergedDTWDistPrefSense)
  plot_Distsensepropotion_sense(mergedDisDTWPrefSensesEmer, distCol="normaDist_dtw", method="dtw", outputPath=inputDir)
  plot_Distsensepropotion_sense(mergedDisEUCPrefSensesEmer, distCol="normSensebyyear", method="euclid", outputPath=inputDir)
  
  
  # comparison between EUCLID and DTW
  
  caculate_corr_between_dtw_euclid(mergedDTWDistPrefSense, mergedECLUDDistPrefSense, bySenses=TRUE, outputPath=NULL)
  caculate_corr_between_dtw_euclid(mergedDTWDistPrefTarget , mergedECLUDDistPrefTarget, bySenses=FALSE, outputPath=NULL)
  
  
  
  
  
  
  
  # emergence between  gold and pred senses
  
  
  
  equaldtw<-select_predgoldValues(dtwDisbySenseDF, equal=TRUE)
  equaleuclid<- select_predgoldValues(euclidDisbySenseDF, equal=TRUE)
  
  
  dtwdistemerDF <-merge(equaldtw, goldpredEmerFileDF, by.x=c('target', 'pred'), by.y=c('WSD_INFORMATION','CUI'))
  #print(head(dtwdistemerDF))
  eucliddistemerDF <-merge(equaleuclid, goldpredEmerFileDF, by.x=c('target', 'pred'), by.y=c('WSD_INFORMATION','CUI'))
  # print(head(eucliddistemerDF))
  
  
  gdtw<- analyse_distance_emergnece(dtwdistemerDF, distCol="normaDist_dtw", perfCol="NormalisedError", methodD="dtw",outputPath=inputDir )
  
  geuclid<-analyse_distance_emergnece(eucliddistemerDF, distCol="normSensebyyear", perfCol="NormalisedError", methodD="euclid",outputPath=inputDir )
  
  
  
  
  
  
  
  
  # emergence?
  
  # emergence results
  EmerFile<-paste0(inputEmerDir, "/goldpredtable.tsv")
  #goldpredEmerFile<- Sys.glob(EmerFile)
  if (basename(EmerFile) != "goldpredtable.tsv") {
    stop(paste('Error: no single match for ',EmerFile))
  }
  goldpredEmerFileDF <- readTable(EmerFile)
  
  
  #dtw distance gold emergence
  disdtw_BygoldSensesFile <-grep("goldsenses_distance.tsv",inputDirFiles)
  disdtw_BygoldSensesFileDF <- readTable(inputDirFiles[[disdtw_BygoldSensesFile]])
  disdtw_BygoldSensesEmergenceFileDF<- combine_distance_with_emergence(disdtw_BygoldSensesFileDF, goldpredEmerFileDF,type="gold" ,outputPath=inputDir) 
  
  
  #dtw distance predicted emergence
  disdtw_BypredictedSensesFile <-grep("predsenses_distance.tsv",inputDirFiles)
  # print(disdtw_BypredictedSensesFile)
  disdtw_BypredictedSensesFileDF <- readTable(inputDirFiles[[disdtw_BypredictedSensesFile]])
  disdtw_BypredictedSensesEmergenceFileDF<- combine_distance_with_emergence(disdtw_BypredictedSensesFileDF, goldpredEmerFileDF, type="predicted",outputPath=inputDir  )
  
  
  
  
  
  
  
  
  
  
  
  #NOmergedDTWDistpredictedSensesEmergence <- merge(disdtw_BypredictedSensesFileDF,  goldpredEmerFileDF, by.x=c('target','CUI_fisrt' ), by.y=c('WSD_INFORMATION','CUI'))# thats not based on match, its between predicted senses
  #NOprint(head(mergedDTWDistpredictedSensesEmergence))
  
  hist_distance_predsenses_emergenceMultiNbSenses(disdtw_BypredictedSensesEmergenceFileDF, inputDir)    
  #NOpropbar_distance_predsenses_emergence(disdtw_BypredictedSensesEmergenceFileDF, seq(0,1,.05), inputDir)    
  #NOhist_distance_predsenses_emergence(disdtw_BypredictedSensesEmergenceFileDF, gold=FALSE, inputDir)    
  #NOpropbar_distance_predsenses_emergence(disdtw_BypredictedSensesEmergenceFileDF, seq(0,1,.05), gold=FALSE, inputDir)    
  
  # calculate_mean(disdtw_BypredictedSensesEmergenceFileDF, type="PRED")
  
  # calculate_mean(disdtw_BygoldSensesEmergenceFileDF, type="GOLD")
  
  
  # MAE senses
  print("MAE by senses")
  distPlotBySenses( disprefsensesMAEALLordereddtwDF, rank=TRUE, distcol='normaDist_dtw', perfcol="MAE", colourcol="position", method="dtw", outputPath=inputDir )#?
  distPlotBySenses( disprefsensesMAEALLorderedeuclidDF, rank=TRUE, distcol='normSensebyyear', perfcol="MAE", colourcol="position", method="euclid", outputPath=inputDir )
  
  # MAE targets
  
  #
  print("MAE by targets")
  g <- distPlotByTarget(mergedDTWDistPrefTargetMAE , distcol='mean_normalised_dtw',perfcol='MAE',colourcol='nbSensesGold',method="dtw", outputPath=inputDir) 
  g<- distPlotByTarget(mergedECLUDDistPrefTargetMAE , distcol='normSensebyyearMEAN',perfcol='MAE',colourcol='nbSensesGold',method="euclid", outputPath=inputDir) 
  
  print("position 1")
  # distance between predicted senses including all the possibilities of matching
  
  rankedDistsenses<- rankand_label_all_possibilities_matching(dtwDisbySenseDF, rankby="normaDist_dtw")
  
  
  disprefsensesMAEALLorderedPOSdtwDF<-merge( perf_BySensesDF , rankedDistsenses, by.x=c('target', 'class'), by.y=c('target','pred'))
  
  
  
  DistPossense <-select_according_to_ranking_and_label_matching(disprefsensesMAEALLorderedPOSdtwDF, selectposition="1")
  
  
  analyse_distance_by_senses(DistPossense,method="dtw",outputPath=inputDir)
  
  # disprefsensesMAEALLorderedPOSdtwDF<-merge( perf_BySensesDF , DistPossense, by.x=c('target', 'class'), by.y=c('target','pred'))
  
  
  
  
  
  
  
  
  #disprefsensesMAEALLorderedPOSdtwDF<-merge(disprefsensesMAEALLordereddtwDF, DistPossense, by.x=c('target', 'class'), by.y=c('target','pred'))
  #  print(nrow(disprefsensesMAEALLorderedPOSdtwDF))
  # print(head(disprefsensesMAEALLorderedPOSdtwDF))
  
  #g <-  analyse_distance_by_senses(disprefsensesMAEALLorderedPOSdtwDF,method="dtw",outputPath=inputDir)
  
  
  #analyse_disatnce_parameters(, method="dtw", outputPath=inputDir)
}else{
  print("the  number of arguments is not correct, it expects 4 arguments!")
  print(" ")
  print("inputDir= the input Dir of the system") 
  print("inputoption= calculation  equal senses: either zero means only equal senses or one all of them")
  print("inputEmerDir= the input Dir of the emergence output")
  #print("inputanotherDir=  give the path for another system to compare the results of dwt and euclidean distances with the current system or an empty string")
  print("inputanotherDir=  give the path for the file that lists the paths of other systems to compare the results of dwt and euclidean distances with the current system or an empty string")
  
}
