ARCH=$1
INCLUDE_PATH="../../.."
LIB_PATH="../../../lib"

if [ $ARCH == "kp920" ] 
then
    gcc -O3 -march=armv8.2-a+simd -mtune=tsv110 -I$INCLUDE_PATH stream_triad.c -o stream_triad -L$LIB_PATH -lmypapi -lpapi
fi

mv stream_triad ../../../bin/