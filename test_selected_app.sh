ARCH=$1
MODE=$2
TESTNUM=$3
MPX=$4

if [ $MODE == "mpxacc" ]
then
    METHOD=$5
fi

#for app in backprop lavaMD knn nw srad
# for app in backprop b+tree bfs cfd heartwall hotspot hotspot3D kmeans lavaMD lud knn nw srad streamcluster
for app in cfd heartwall hotspot hotspot3D kmeans lavaMD lud knn nw srad streamcluster
do
    ./test.sh $ARCH $MODE $app $TESTNUM $MPX $METHOD
done