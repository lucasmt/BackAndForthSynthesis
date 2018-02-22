#! /bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR


echo Dror here - Running a new script
for i in  Benchmarks/2QBF/*.qdimacs
do 
    echo $i;
    echo "----"
    #echo "Running:"
    echo " "
    ./decomp $i  $i.edge1 $i.edge2
  #  bin/compdegen < $i > /dev/null;
   # bin/printnm < $i > /dev/null;
   # echo " "
    echo "Done"
    #echo " "
  #  bin/qc --algorithm=tomita     --input-file=$i  > /dev/null
  
    #echo " "
    echo " "
done
