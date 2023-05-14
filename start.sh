#!/bin/bash



echo Game Starting in...
count=3
while [[ $count > 0 ]]; do
    echo $count
    ((count--))
    sleep 0.5
done

clear

./game_loop.exe
    
