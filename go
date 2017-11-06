#!/bin/bash

function usage {
	echo "Usage: $0 -d dim -p startPortNumber [-v]"
	echo "       Arguments are mandatory."
	echo "        * dim indicates the matrix dimension (4 < dim < 21)."
	echo "        * startPortNumber must be greater than 1024."
  echo "        * -v shows the connection log of all slaves."
    exit 1
}

if [ $# -lt 4 ]
then
    usage
fi


log=false
while getopts ":d:p:v" opt; do
  case $opt in
    d)
      dim=${OPTARG}
      ;;
    p)
      portnum=${OPTARG}
      ;;
    v)
      log=true
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
echo "Starting master..."

x-terminal-emulator --hide-menubar -t MASTER -e "./exec/master -d $dim -p $portnum"

echo "Starting slaves..."

echo "Press any key to stop..."

for i in `seq 1 $(($dim * $dim))` 
do
  if $log
  then
	 ./exec/slave -i $i -d $dim -p $(($portnum + $i)) &
  else
   ./exec/slave -i $i -d $dim -p $(($portnum + $i)) > /dev/null & 
  fi
done
 
read -n 1 -s

killall -9 master
killall -9 slave