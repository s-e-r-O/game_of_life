#!/bin/bash

function usage {
	echo "Usage: $0 -d dim -p startPortNumber"
	echo "       Arguments are mandatory."
	echo "        * dim indicates the matrix dimension (4 < dim < 21)."
	echo "        * startPortNumber must be greater than 1024."
    exit 1
}

if [ $# -ne 4 ]
then
    usage
fi

while getopts ":d:p:" opt; do
  case $opt in
    d)
      dim=${OPTARG}
      ;;
    p)
      portnum=${OPTARG}
      ;;
    \?)
      echo "Invalid option: -$OPTARG"
      usage
      ;;
  esac
done

if [ "$dim" -eq "$dim" -o "$portnum" -eq "$portnum" ] 2>/dev/null; then
  if [ $dim -gt 20 -o $dim -lt 5 -o $portnum -le 1024 ] 
  then
  	usage
  fi
else
  usage
fi

#clear
echo "Starting Game of Life Simulation"
echo
echo "Starting master: "
./master -p $portnum &

echo
echo "Starting slaves: "
for i in `seq 1 $(($dim * $dim))` 
do
	./slave -i $i -d $dim -p $(($portnum + $i)) &
done

read -n 1 -s