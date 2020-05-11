#!/bin/bash
./test_all_app.sh cas mpxacc 1 10 7
for ocoe in {1..7}
do
    ./test_all_app.sh cas mpxacc 1 ocoe${ocoe} 7
done