ARCH=$1
MODE=$2
TESTNUM=$3
MPX=$4

if [ $MODE == "mpxacc" ]
then
    METHOD=$5
fi

for app in backprop lavaMD knn nw srad
do
    ./test.sh $ARCH $MODE $app $TESTNUM $MPX $METHOD
done