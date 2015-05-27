#!/bin/bash -l
#$ -cwd
#$ -j y
#$ -o /dev/null
#$ -q idra
#
# optimal script: launch optimal solver and then the validator

prefix="-1"
if [ ! -z "$1" ]
then	prefix=$1
fi
./bin/optimalRoster n030w8 1 8 2 7 0 9 3 6 0 6 n030w8_1_2-7-0-9-3-6-0-6 $prefix $2 $3 $4 $5 > outfiles/Competition/n030w8_1_2-7-0-9-3-6-0-6/${1}Log.txt

instance=n030w8
weeksValue=(2 7 0 9 3 6 0 6 )

demand0="WD-${instance}-"
solutionFile="outfiles/Competition/n030w8_1_2-7-0-9-3-6-0-6/${1}sol-week"
weeks=""
sols=""
i=0

for var in ${weeksValue[*]}
do
demand[$i]="datasets/${instance}/${demand0}${var}.txt"
weeks="${weeks} ${demand[$i]}"
solution[$i]="${solutionFile}${i}.txt"
sols="${sols} ${solution[$i]}"
((i++))
done

java -jar validator.jar --sce datasets/n030w8/Sc-n030w8.txt --his datasets/n030w8/H0-n030w8-1.txt --weeks $weeks --sols $sols > outfiles/Competition/n030w8_1_2-7-0-9-3-6-0-6/${1}ValidatorOutput.txt 

exit 0;
