#!/bin/bash
./test_selected_app.sh cas mpxacc 200 special 8
./test_selected_app.sh cas ocoe 200 0
./test_selected_app.sh cas mpxori 200 special 0
# for ocoe in {1..7}
# do
#     ./test_selected_app.sh cas mpxacc 1 ocoe${ocoe} 8
# done