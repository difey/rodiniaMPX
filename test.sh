TEST_ROOT=$(pwd)
RODINIA_ROOT=${TEST_ROOT}/Benchmark/hpcbenchmark-collection/Rodinia
ARCH=$1
MODE=$2
APP=$3
TESTNUM=$4
MPX=$5
POSTMETHOD=0

if [ $MODE == "mpxacc" ]
then
    POSTMETHOD=$6
fi

make CC=icc METHOD=$POSTMETHOD

echo "testing $APP for $TESTNUM times"

if [[ $APP == "b+tree" || $APP == "backprop" || $APP == "cfd" || $APP == "heartwall" || $APP == "hotspot3D" || $APP == "lavaMD" || $APP == "leukocyte" || $APP == "nw" || $APP == "particlefilter" || $APP == "pathfinder" || $APP == "srad" || $APP == "streamcluster" ]]
then
    cd ${RODINIA_ROOT}/$APP
    ./compile.sh $ARCH
    mkdir result
    mycount=0; while (( $mycount < $TESTNUM )); do ./papi_test.sh $ARCH $MODE $MPX;suffix=$(date "+%Y%m%d-%H%M%S"); mv raw.bin result/$APP.$MPX.$suffix.bin; ((mycount = $mycount+1)); done;
elif [ $APP == "bfs" ]
then
    cd ${RODINIA_ROOT}/bfs/inputGen
    make
    ./gen_dataset.sh
    cd ${RODINIA_ROOT}/bfs
    ./compile.sh $ARCH
    mkdir result
    mycount=0; while (( $mycount < $TESTNUM )); do ./papi_test.sh $ARCH $MODE $MPX;suffix=$(date "+%Y%m%d-%H%M%S"); mv raw.bin result/$APP.$MPX.$suffix.bin; ((mycount = $mycount+1)); done; 
elif [ $APP == "hotspot" ]
then
    cd ${RODINIA_ROOT}/hotspot/data/inputGen
    make
    sh datagen.sh
    cd ${RODINIA_ROOT}/hotspot
    ./compile.sh $ARCH
    mkdir result
    mycount=0; while (( $mycount < $TESTNUM )); do ./papi_test.sh $ARCH $MODE $MPX;suffix=$(date "+%Y%m%d-%H%M%S"); mv raw.bin result/$APP.$MPX.$suffix.bin; ((mycount = $mycount+1)); done;
elif [[ $APP == "knn" || $APP == "kmeans" ]]
then
    cd ${RODINIA_ROOT}/$APP/data/inputGen
    make
    ./gen_dataset.sh
    cd ${RODINIA_ROOT}/$APP
    ./compile.sh $ARCH
    mkdir result
    mycount=0; while (( $mycount < $TESTNUM )); do ./papi_test.sh $ARCH $MODE $MPX;suffix=$(date "+%Y%m%d-%H%M%S"); mv raw.bin result/$APP.$MPX.$suffix.bin; ((mycount = $mycount+1)); done;
elif [ $APP == "lud" ]
then
    cd ${RODINIA_ROOT}/lud/tools
    make
    ./gendata.sh
    cd ${RODINIA_ROOT}/lud
    ./compile.sh $ARCH
    mkdir result
    mycount=0; while (( $mycount < $TESTNUM )); do ./papi_test.sh $ARCH $MODE $MPX;suffix=$(date "+%Y%m%d-%H%M%S"); mv raw.bin result/$APP.$MPX.$suffix.bin; ((mycount = $mycount+1)); done;
fi

suffix=$(date "+%Y%m%d-%H%M%S")
mv $APP.log result/$APP.$ARCH.$MODE.events${MPX}.methods${POSTMETHOD}.testtime${TESTNUM}.$suffix.log

cd ${TEST_ROOT}
make clean