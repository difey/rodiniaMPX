ARCH=$1
MODE=$2
TESTNUM=$3
MPX=$4

if [ $MODE == "mpxacc" ]
then
    METHOD=$5
fi

for app in b+tree backprop bfs cfd heartwall hotspot hotspot3D lavaMD leukocyte lud knn kmeans nw particlefilter pathfinder srad streamcluster
do
    ./test.sh $ARCH $MODE $app $TESTNUM $MPX $METHOD
done