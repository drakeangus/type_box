#!/bin/bash

echo 0 > .game_files/game_control
echo false > .game_files/inProgress
rm -f .players/player_*
rm -f .game_files/output_files/output_*  
