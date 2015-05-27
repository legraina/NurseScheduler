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
./bin/optimalRoster n050w8 1 8 1 7 8 5 7 4 1 8 n050w8_1_1-7-8-5-7-4-1-8 $prefix $2 $3 $4 $5 > outfiles/Competition/n050w8_1_1-7-8-5-7-4-1-8/${1}Log.txt

instance=n050w8
weeksValue=(1 7 8 5 7 4 1 8 )

demand0="WD-${instance}-"
solutionFile="outfiles/Competition/n050w8_1_1-7-8-5-7-4-1-8/${1}sol-week"
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

java -jar validator.jar --sce datasets/n050w8/Sc-n050w8.txt --his datasets/n050w8/H0-n050w8-1.txt --weeks $weeks --sols $sols > outfiles/Competition/n050w8_1_1-7-8-5-7-4-1-8/${1}ValidatorOutput.txt 

exit 0;
