#!/bin/bash

success_count=0
expected_count=166694770  #  actual expected count

for i in {1..100}; do
    count=$(./count_ones 32 | grep "Total count" | awk '{print $3}')
    
    # Check if the result is correct
    if [ "$count" -eq "$expected_count" ]; then
        ((success_count++))
    fi
done

echo "Number of successful runs: $success_count"