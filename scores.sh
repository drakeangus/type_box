#!/bin/bash

game_files=".game_files"
scores="${game_files}/scores.csv"

print_scores()
{
    echo "Name,Date,Score,WPM,Total Time,Mistakes"
    cat $scores | sort -k3 -n -r -t ',' 
}

print_scores | column -t -s ','
    
