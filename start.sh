#!/bin/bash
game_files=".game_files"
player_file="${game_files}/player_names.txt"
scores_file="${game_files}/scores.csv"
game_text_file="${game_files}/game_text.txt"



# create all neccessary files if they don't exist
[[ -d $game_files ]] || mkdir $game_files
[[ -f $player_file ]] || touch $player_file
[[ -f $scores_file ]] || touch $scores_file

# get player ip so they don't have to enter a name each time they play
ip_regex='^.*\((.*)\)$'
whomst=$(who am i)
#[[ -z $whomst ]] && whomst=
[[ $whomst =~ $ip_regex ]] && ip=${BASH_REMATCH[1]} || ip='not found'

# if the player doesn't exist in the player_file ask for a name
name=$(grep "$ip" $player_file | awk -F ',' '{print $1}')

[[ -z $1 ]] || name="$1"

if [[ -z $name ]]; then
    read -p "What's your name? " name 
    echo ${name}, ${ip} >> $player_file
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



: '

read -p "Hit the enter key when you are ready to start"

echo Game Starting in...
count=3
while [[ $count > 0 ]]; do
    echo $count
    ((count--))
    sleep 0.5
done

clear
'

cd pre-game
./start_game.sh "$name"

# ./game_loop.exe "$name" "$(date -u '+%Y-%m-%d %H:%M:%S')"
    
