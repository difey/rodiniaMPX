#!/bin/sh
PAPI_DIR=/lustre/home/acct-hpc/hpcwj/tmp/papi-x86/src/install
export PATH=${PAPI_DIR}/bin:$PATH
export CPATH=${PAPI_DIR}/include:$CPATH
export C_INCLUDE_PATH=${PAPI_DIR}/include:$C_INCLUDE_PATH
export LIBPATH=${PAPI_DIR}/lib:$LIBPATH
export LIBRARY_PATH=${PAPI_DIR}/lib:$LIBRARY_PATH
export LD_LIBRARY_PATH=${PAPI_DIR}/lib:$LD_LIBRARY_PATH
