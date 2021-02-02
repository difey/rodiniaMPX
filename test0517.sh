#!/bin/bash
./test_selected_app.sh cas mpxacc 10 special 0
./test_selected_app.sh cas mpxacc 10 special 8
./test_selected_app.sh cas mpxacc 10 special 16
./test_selected_app.sh cas mpxacc 10 special 32
./test_selected_app.sh cas mpxacc 10 special 64
# for ocoe in {1..7}
# do
#     ./test_selected_app.sh cas mpxacc 1 ocoe${ocoe} 8
# done