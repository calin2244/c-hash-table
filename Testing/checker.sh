#!/bin/bash

# for i in {1..100}; do
#     output=$(./bi l)
#     if [[ "$output" != "All extensive tests passed!" ]]; then
#         echo "Test failed on iteration $i with output: $output"
#         exit 1
#     fi
# done

# echo "All 100 tests passed!"
echo -e "\033[31mNEW TEST CHAINING\033[0m"
output=$(valgrind ./coll_res)
echo $output

echo -e "\033[31mNEW TEST LP\033[0m"
output=$(valgrind ./coll_res l)
echo $output

echo -e "\033[31mNEW TEST QP\033[0m"
output=$(valgrind ./coll_res q)
echo $output

echo -e "\033[31mNEW TEST DOUBLE HASHING\033[0m"
output=$(valgrind ./coll_res d)
echo $output