#!/bin/bash
game_files=".game_files"
player_directory=".players"
player_names="${game_files}/player_names.txt"
scores_file="${game_files}/scores.csv"
game_text_file="${game_files}/game_text.txt"
gameInProgress="${game_files}/inProgress"
game_control_file="${game_files}/game_control"
game_count="${game_files}/game_count"


# create all neccessary files if they don't exist
[[ -d $game_files ]] || mkdir $game_files
[[ -d $player_directory ]] || mkdir $player_directory
[[ -f $player_names ]] || touch $player_names
[[ -f $scores_file ]] || touch $scores_file
[[ -f $gameInProgress ]] || echo false > $gameInProgress
[[ -f $game_control_file ]] || echo 0 > $game_control_file
[[ -f $game_count ]] || echo 0 > $game_count

# do not run this script if a game is in progress - it'll fuck everything up
if grep -q 'true' $gameInProgress; then
    echo "There is currently a game in progress. Please wait for it to finish"
    exit
fi

countdown()    
{    
    echo Game Starting in...        
    count=3    
    while [[ $count > 0 ]]; do    
        echo $count        
        ((count--))    
        sleep 0.5    
    done    
}  






# get player ip so they don't have to enter a name each time they play
ip_regex='^.*\((.*)\)$'
whomst=$(who am i)
#[[ -z $whomst ]] && whomst=
[[ $whomst =~ $ip_regex ]] && ip=${BASH_REMATCH[1]} || ip='not found'

# if the player doesn't exist in the player names file ask for a name
name=$(grep "$ip" $player_names | awk -F ',' '{print $1}')

[[ -z $1 ]] || name="$1"

if [[ -z $name ]]; then
    read -p "What's your name? " name 
    echo ${name}, ${ip} >> $player_names
else
    echo "Welcome back $name"
fi


strings=(    
    "Honey never spoils. Archaeologists have found pots of honey in ancient Egyptian tombs that are over 3,000 years old and still perfectly edible"
    "The world's oldest known recipe is for beer. It dates back over 5,000 years and was discovered on a Sumerian clay tablet in Mesopotamia"
    "The average person will spend about six months of their life waiting at red traffic lights"
    "In 2007, an American man named Corey Taylor set a world record by carrying 72 snails on his face for 10 seconds"
    "The oldest known living tree is a bristlecone pine named Methuselah in California it is estimated to be almost five thousand years old"
    "The longest recorded flight of a chicken is thirteen seconds"
    "The largest pyramid in the world is not in Egypt but in Cholula Mexico. The Great Pyramid of Cholula is the largest by volume and covers an area of over 16 hectares"    
)

strings2=(
"Dolphins are intelligent and sociable marine mammals known for their playful behavior and complex communication skills."
"The Great Wall of China stretches for thousands of kilometers, serving as a historic defensive fortification."
"The Eiffel Tower, an iconic landmark in Paris, attracts millions of visitors with its impressive architectural design."
"Bees are essential pollinators, playing a crucial role in the reproduction of many plant species."
"The Amazon rainforest covers vast areas of South America and is renowned as the world's largest tropical rainforest."
"Mount Everest, located in the Himalayas, stands as the highest peak on Earth, captivating adventurers and mountaineers."
"The Mona Lisa, created by Leonardo da Vinci, is an invaluable and renowned masterpiece in the world of art."
)
array_length=${#strings2[@]}    
random_index=$((RANDOM % array_length))    
echo ${strings2[$random_index]} > $game_text_file



#./start_game.sh "$name"


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
    gameCounter=$(cat $game_count)
    ((gameCounter++))
    $gameCounter > $game_count
    echo Game_ID:$gameCounter >> $scores_file
fi 



player_file="$player_directory/player_${num}"

printf "false, $name" > $player_file # its important this file has no newline character

./game_control.exe "$name" "$num"

echo true > $gameInProgress

# when each player exits the game control script they will
# A) set set gameInProgress = true
# B) increment the number in the game_control file
# once the number hits the total number of players the game will begin

players=$(grep -l 'true, ' .players/* | wc -l)

game_control_num=$(cat $game_control_file)

((game_control_num++))

echo $game_control_num > $game_control_file

while [[ "$(cat $game_control_file)" -le "$number_of_players" ]]; do
    continue
done

countdown

 ./game_loop.exe "$name" "$(date -u '+%Y-%m-%d %H:%M:%S')" "$num"

# post game

printFirst=true
while [[ ps -ef | grep -v 'grep' | grep -q 'game_loop.exe' ]]; do
        if $printFirst; then
            echo Please wait for the current game to finish
            printFirst=false
        fi
        sleep 1
    done

grep $(cat $game_count)


#reset for next game
echo 0 > $game_control_file
rm -f $player_directory/player_*   
echo false > $gameInProgress
rm -f $game_files/output_files/output_*
