#!/bin/bash

name="$@"

[[ -z $name ]] && echo must give name as argument && exit

# if game_control doesnt exist create it
game_control_file="game_control"
[[ -f $game_control_file ]] || echo 0 > game_control

countdown()
{
echo Game Starting in...    
count=3    
while [[ $count > 0 ]]; do    
    echo $count    
    ((count--))    
    sleep 1    
done 
}


clear

printf '    

                                              7GB?
                                             .#@@J
         :PBP7.                              .B@@7                                  :7!.                                                                                                                                                            ?G5:
         :P#@@&J.                            ~@@@^                                  P@@?                                                                                                                                                           .#@@7
            !B@@#7.                          ?@@B.                                  G@@J                                                                                                                                                           .#@@7
              7B@@#!                        .B@@J                                   G@@J                                                                                                                                                           .#@@7
                7&@@J       ^PGP7           !@@&:        :~?Y55555Y?!~:             G@@J                                                                   .~JYY?.                                                                                 .#@@7        .^^:      :!77777~:.
                 ^B@@G~    ?&@@@@G^         G@@Y       ~G&@@@&&&&&&@@@@5            G@@J       ~7??!^                 .:::::           .~J55Y!.          ~P#@@@@@&J.       .~7?5BBBBBBBG55?^.                                                      .#@@7 .^~7J5P#@@@!  :?P&@@@@@@@@&#G5J?!.
                  .J&@@Y. ^@@@7Y@@@P:      7@@&^      :&@@5~:. ....:J@@#.           G@@5    .?B@@@@@@7            !5PG&&@@@&BP!      ^Y#@@&&@@#?.     .7P@@@5!:~B@@B?   ~YB&@@@@#GGGGGG##@@@#P^                                                  :!?&@@#B&@@@@@##P7!  J&@@BY~^^^7JP#BB&@@@#J:
                    ^B@@G^G@@P  ^P@@#?    ?@@@!       !@@&.  .YPPPPP#@@P            ?@@#.  ~#@@#?:7P5^          ~B@@&&@#YJJP&@@Y    ^#@@G!..7#@@#7   ~#@@BY^    .7#@@5.5@@@GJ7^^.     ..:~5@@@7                                                 :&@@@@@@GPJ!~^:..   ^B@@B~         .  .^7G@@#.
                     :G@@@@@G.    7B@@P~ J@@&~        ~@@&!. :G#BBB#BP?.            :&@@! :B@@5                J@@&J..:.    5@@P   ~&@@5.    .7&@@J:J&@@J         :&@@#@@B~~B####&&&&&&##&@@&5:                                                  !?!^#@@7          .B@@Y                 !@@&:
                       7#@@#^      .5&@@#@@#~          J&@@#7^.             :YGY.    B@@P 5@@#~..              G@@Y        .B@@5   5@@P        ~B@@&@@B!           7@@@@#: :Y5555555555555J!:                                                        P@@Y          .&@@?            ..:!J#@@B.
                        .!7:         ^5#&#P:            .?B@@@&GJ7~~~~~~~!7P&@@#:    ~@@@77&@@@@&BBG55555GBP^  5@@&G55J????B@@P:   P@@5         :#@@@5.             P@@@#!.                                                                          5@@B.          J@@@&GP5J?????PB&&@@@&GJ^
                                       ...                 ^7JG@@@@@@@@@@@@@#Y~.      ^7!. .^!JYPGG########P^  .75G##&@@@@@@#Y.    ~PP~          ^JY!               ~@@@@@B?^.                                                                       ^&@@7           :75G##&@@@@@@@B5YY?^.
                                                               :^~!!!!!!!~^:.                       ......         ...::::^:.                                        ?PY7B@@@&5?~^^^^^^^^^^7J7.                                                       P@@B.              ...:::::^:
                                                                                                                                                                          ^!5#@@@@@@@@@@@@@@@&~                                                       ^GBP:
                                                                                                                                                                              :~7??????????7~.

\n'

printf '
                .^?PB####BGJ:               ~?~    !GG7    :7?????????!.               .:~~!?????YG#&@@@&!.^~::::::^~~7??!.                                                                                                          .:::.               .^!?5G#@@@~
              :5&@@@#GGGG&@@~              ^@@@~   G@@G    Y@@@@@@@@@@@!              P@@@@@@@@@@@&BG@@@@@@@@@&@@@@@@@@@@@7                   ^^.                                :~~~~~~~!7????J5PP5!  .^^                     .~?5B#&@@&&G!        ^7YGB&@@@@&BGP?.
             :#@@@P?^.   .^.               ~@@@!   G@@G    .^^?@@@7^^^:               !YYY77#@@G^^.  JGPYYY#@@@5YYYY?7!^^:        JGP~      7B@@G                               ~@@@@@@@@@@@@@@@@&&&Y  P@@G!.     :JGB?      :Y#@@@&BG5YB@@&:      :&@@@#G5?!^.
             .P&@@@@@&#BBPY?7~:            ~@@@!  .G@@G       !@@@~                         B@@5           ?@@&:                 .B@@&5~   ?@@@P^                               .?Y?77777J@@@J^^:...   ~P&@@&BY^:?&@@&?      J@@@B^.    Y@@&:      !@@@7
               :~77?YPGB#&@@@@@#5!:        7@@@5JG&@@@G       !@@@~                        .B@@P           ?@@&:                  .7P@@@#5G@@&?                                          ^&@@7           :7YB@@@&@@&?.       ~@@@?  .^?G@@@Y       Y@@#.      .:~^:
                         .:~!JG&@@&5:     .B@@@@@@&&@@G       !@@@~                        .B@@P           ?@@&:                     ~Y#@@@&Y:                                           ^&@@7               7&@@@J:          B@@G^Y#@@@#Y~       .B@@B~~~!?5B&@@@&^
                               .7&@@P      !@@@?^^.G@@G       !@@@~                        .B@@P           ?@@&:                     :5&@@P:                                             ^&@@7             :J&@@G~            5@@@@@@G?^.         .#@@@@@@@@@#P5Y5J^
               :5GPJ7~^^^^^^^^~~7#@@G      7@@@!   G@@G       !@@@~                        .B@@P           ?@@&:                   .?#@@B!                                               ^&@@7            7&@@#?              5@@@&5~             .#@@G77??JJY5B&@@@?
               ~#@@@@@@@@@@@@@@@@@&B7      ?&&P.   G@@G       ~&@@5^!777777?Y!              B@@P           ?@@&:                  !#@@&J.                                                ^@@@7         ~JG@@@5.               7@@@!               .#@@BJ5B&@@@@&B5J7:
                .:^~!7?????????7~^:         ..     J&&Y     Y&&@@@@@@@@@@@@@@B.            ^@@@5           ^GBY.                  ~GB5^                                                  :#@@!        .#@@@P^                 ~@@@!                5&&&@@#GY7^:.
                                                    ..      7PP555J?7~~~~~~~~.             .JPJ:                                                                                          :~^          JBB7                   ~@@@?                 :::::.
                                                                                                                                                                                                                              ~@@@5
                                                                                                                                                                                                                              :B@#^
                                                                                                                                                                                                                               .:.
\n'


player_directory=".players"
[[ -d $player_directroy ]] && mkdir $player_directory

if [[ -n "$(\ls -A $player_directory)" ]]; then
    # file exists in the directory 
    # increment from the last player number

    last_file=$(\ls $player_directory/player_[0-9]* 2> /dev/null | tail -1)

    ## BUG - ^ the ls command above doesn't order the files correctly so player_9 will always show as the last

    file_name_regex='.*_([0-9]*)'
    [[ $last_file =~ $file_name_regex ]] && num=${BASH_REMATCH[1]}

    ((num++))

else
    num=1

    read -p "Wait for other players? [y]/n: " input
    
    [[ $input == "n" || $input == "N" ]] && single_player=true

    
fi



player_file="$player_directory/player_${num}"

echo $name = player_$num

printf "false, $name" > $player_file # its important this file has no newline character


./game_control.exe "$name" "$num" 


# when each player exits the game control script they will increment the number in the game_control file
# once the number hits the total number of players the game will begin

players=$(grep -l 'true, ' .players/* | wc -l)

game_control_num=$(cat game_control)

((game_control_num++))

echo $game_control_num > game_control

while [[ "$(cat game_control)" -le "$number_of_players" ]]; do
    sleep 0.01
done    

countdown



echo GAME START

clear


cd .. # need to fix this 
./game_loop.exe "$name" "$(date -u '+%Y-%m-%d %H:%M:%S')" "$num"
cd pre-game # need to fix this

echo [ DEBUG ] - back to start_game.sh - resetting game files

#reset for next game
echo 0 > game_control
rm -f $player_directory/player_*
