#!/bin/bash

#SBATCH -t 2-00:00:00
#SBATCH -p compute
#SBATCH -J ABSTRART


# NEW
# for d in ~/filter-abstracts/*; do  sbatch  $(pwd)/run-booleHyphenFull.sh "$d" ~/abstracts-indiv.hyphen.lower.full 5; done



umlsTermsFile="/home/alsulaia/MESH/MeshCode/READMETA_collectPMids/target-terms_lower-nodubplicate_labeled"
stopwords="/home/alsulaia/MESH/MeshCode/WSIPUBMED/NEWRESULTS_palywsi/GoldInstancesFromFilteredPMID/EXPERIMENT/FILTERSTOPWORD/stopwords-inspect.txt"
if [ $# -ne 3 ]; then
    echo "arg = <input target dir> <big output dir> <window length>" 1>&2
    echo "CAUTION: use full path" 1>&2 
    echo "CAUTION: first arg is the dir for a specific target but 2nd arg is big output dir " 1>&2 
    exit 1
fi
targetInputDir="$1"
outputDir="$2"
window="$3"

[ -d "$outputDir" ] || mkdir "$outputDir"

#target=$(basename "$targetInputDir")
#mkdir "$outputDir/$target" # create fake big dir supposed to contain many targets but actually only one
#pushd "$outputDir/$target" 
#ln -s "$targetInputDir"
#popd 
python2 /home/alsulaia/udocker/udocker run -v /home/alsulaia/ Spacy sh -c "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:/usr/local/lib/; /home/alsulaia/MESH/MeshCode/ABSTRACT_FULLVERSION/ABSTACTFULLVERSION_test -t \"$umlsTermsFile\"  -w \"$stopwords\" -r -n -l \"$targetInputDir\" \"$outputDir\" $window 1"

#rm -f "$outputDir/$target/$target" 
#rmdir "$outputDir/$target"
