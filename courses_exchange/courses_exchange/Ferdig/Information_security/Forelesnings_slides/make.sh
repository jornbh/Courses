END=13
for ind in $(seq 11 $END) 
do
	mkdir Forelesning_$ind
#	mv 0${ind}* Forelesning_$ind
	echo $ind
done
