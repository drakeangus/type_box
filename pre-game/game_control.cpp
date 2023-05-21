#include <string>
#include <iostream>    
#include <filesystem>
#include <fstream>
#include <sstream>

// for sleep
#include <chrono>
#include <thread>

// for repeating get user input calls
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

namespace fs = std::filesystem;    
using namespace std;    
 

void printReadyUp()
{

    cout << R"(
                                                                                                 :^.                                                                                                                                                                      
                                                                              ~J?.              7&@G.                                                                                                                                                                     
                                                                             :&@@?              J@@&:                                                                                                                                                                     
                                                                 !55^        :&@@J              J@@&:                                                                                                                                                                     
                                                                .B@@5        :&@@J              J@@&:                                                                                                                      .7?^                                           
                                                                .B@@5        :&@@J     YB5.     J@@&:                                                                  !GG7                                                Y@@B.                                          
                                                                .B@@P...:~7?YP@@@~     JGP.     J@@&:                                    :!?7!^                       :&@@Y                       .......                  5@@#.                   .^~:                   
                                                                .B@@@&&&@@@@@@@@&:              J@@&:                                   J&@@@@@G:        .^!JYY!      ~@@@!        .~77~.      .?G&&&&&&&G.                5@@B.                 .^#@@@G!                 
                                                                .B@@#PPPPY?~~B@@G.   .:.        J@@&::?Y7.                             ?@@@JY@@@~       ?#@@@@@@^     ~@@@!      :P&@@@@&~    :B@@&PPPPPPJ.                5@@B..:~JGG5:      :JG#&@&P@@@7                
                                                                .B@@5       .#@@Y   ~&@G.       J@@&G&@@&^                            :&@@5 7@@@~      J@@@P7#@@5     ~@@@Y!!!JPYB@@#Y@@@7    5@@&^                        5@@&B&@@@@&G:     7&@@#B#Y^&@@?                
                                                                .B@@5       ^&@@J   ?@@@!       J@@@@@GJ:                             ?@@@Y5&@@B:      P@@B  J@@@~    Y@@@@@@@@@@@@@?Y@@@^    G@@G                        :#@@@&B5J7^.      7@@@J    J@@@~                
                                                                .#@@P       ?@@@^   :&@@Y       J@@@P^                                J@@@&@@GJ.       P@@B  .#@@G    !@@@Y!777~5@@@#@@&?     J@@&^                       ^&@@Y.            B@@#???YB@@@P                 
                                                                 7GG!       ^B&G.   :#@@P       J@@&.                                 J@@@5!~:.:7PB5.  P@@B   J@@@^   ~@@@!     7@@@@#5:      7@@@~                       :&@@?             5@@@@@@@@BY!.                 
                                                                              .     .B@@5       J@@&:                                 .5@@@@&&&@@@@B:  ^Y5~   :&@@P   ~@@@!      Y@@@Y^:.^YG5.7@@@~                       :&@@J             .^~~~~~~^                     
                                                                                     .~^        7@@@!                                   ^J555555Y7:            P@@G   ~@@@!       ?#@@@@&@@@#:^#&B:                       :&@@J                                           
                                                                                                 ^!^                                                           :??^   .JPY.        .^7YPPP57.  .:.                         ?P5:         .:.                               
                                                                                                                                                                                                                                       :B@#^                              
                                                                                                                                   75Y:                                                                                                !@@@!                              
                                                                                                                                  :&@@J                                                                                                !@@@!                              
                                                                                    ....                                          :&@@J                                                                              .!Y5PBBP?:        !@@@!                              
                                                                                :?YG&&&&5                                         :&@@J                                                                           .JB&@@@@#&@@#?.      ~@@@7                              
                                                                             .?B@@@@&GPP?    .75GGGGY^                            :&@@J                                                        ::   .?J~          ?@@@B?^:..J@@@5      .#@@5                              
                                                                            ^#@@&P7:.       !&@@&#@@@G     !PPPP5~                .&@@J                                                      !B&@Y  5@@#.         ?@@@J    ^5@@@5       G@@P                              
                                                                            ?@@@!        ^~!#@@B~^B@@B    7@@@@@@#Y5!         ^!!~7&@@J              :~:                                    .#@@B^ 7@@@Y          ^&@@G?JYG@@@B7.       5@@&^                             
                                                                            ?@@&^       ~@@@@@@@&@@@&J   ^@@@G7P@@@@@^     .7G@@@@@@@@?     ^YY~    ~&@&^                                   ^@@@7 J@@@5           :&@@@@@@@#P!          !@@@^                             
                                                                            7@@@~       .7Y&@@@BPYJ!.    .P@@@&&@@@@@Y    J&@@&P77&@@B.     P@@#.  .B@@@G:                                  ^@@@GB@@@Y            :&@@P!^^^.             ~7~                              
                                                                            .#@@G.         ~B@@@#PYYY5Y?:  ~J5555?Y@@@Y. ?@@@5.  ?@@@7      ^&@@BJ?B@@@@@Y                                  .Y&&&&#G!             7@@@!                                                   
                                                                             7@@@5           !JB&@@@@@@&!          J&@@G P@@@BGG#@@&?        ^5&@@@@@PG@@#:                                   .:::.               J@@&:                                                   
                                                                              5##Y              .::::::.            :?J~ :YB#####G?.           .~!!!^ ~@@@!                                                       J@@&:                                                   
                                                                                                                            ....                      ^&@@?                                                       J@@&:                                                   
                                                                                                                                                      ^&@@?                                                       !&@B.                    .                              
                                                                                                                                                      ^&@@?                                                        :^.                    Y&#^                            
                                                                                                                                               .   .:!5@@@!                                                                               ~JJ:                            
                                                                                                                                              5##GB#@@@@G7                                                                                                                
                                                                                                                                              Y####BPJ?^                                                                                                                  
)";
}


void moveCursorUp(int rows)
{
    for (int i; i < rows; i++)
    {
        cout << "\x1b[A";
    }
}


bool printStatus(bool skipClearLine = false)
{
    std::string path = ".players";
    string file_name;
    int  player_count = 0;
    
    bool allPass = true;

    for (const auto & entry : fs::directory_iterator(path))
    {
        player_count++;
        //file_name = entry.path();
        //std::cout << entry.path() << std::endl;
        //std::cout << file_name << std::endl;

        std::ifstream file(entry.path());      

        string ready_state, name;
        if (file.is_open())    
        {    
            std::stringstream buffer;    
            buffer << file.rdbuf();    
            std::string fileContent = buffer.str();    
            file.close();    
            
            if (!skipClearLine)
            {
                // clear current line - this should help with ghost letters 
                printf("\33[2K\r");
            }

            int pos = fileContent.find(", ");
            ready_state = fileContent.substr(0, pos);
            name = fileContent.substr(pos + 2);
            
            if (ready_state == "true")
            {
                cout << "✅";
            }
            else
            {
                cout << "❌";
                allPass = false;
            }
            cout << " " << name;    
        }    
        else    
        {    
            cout << "ERROR - cannot read file" << endl;    
        }    
    }

    if (allPass)
    {
        return true;
    }

    cout << endl;
    moveCursorUp(player_count + 1); // plus 1 for the line above
    return false;
}

void updatePlayerFile(string& player_name, string& player_num)
{
    string filename = ".players/player_" + player_num;
    string contents = "true, " + player_name;
    std::ofstream myfile;    
    myfile.open (filename, std::ios::trunc); // open and empty file    
    myfile << contents << endl; 
    myfile.close();
}

bool getUserInput(string& player_name, string& player_num)    
{    
    std::string userInput;    
    bool skip = false;    
    
    //std::cout << "Please enter your input within 5 seconds: " << endl;    
    
    fd_set readfds;    
    FD_ZERO(&readfds);    
    FD_SET(STDIN_FILENO, &readfds);    
    
    struct timeval timeout;    
    timeout.tv_sec = 0.5;    
    
    timeout.tv_usec = 0;    
    
    int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);    
    
    if (ready == -1) {    
        //std::cout << "Error in select." << std::endl;    
        return false;    
    }    
    else if (ready == 0) {    
        //std::cout << "Time limit exceeded. Skipping..." << std::endl;    
        skip = true;    
        return false;    
    }    
    else if (FD_ISSET(STDIN_FILENO, &readfds)) {    
        std::getline(std::cin, userInput);    
    }    
    
    if (!skip) {    
        //std::cout << "User input: " << userInput << std::endl;    
        // Further processing or actions based on user input    

        updatePlayerFile(player_name, player_num);
    }    
return true;    
}

int main(int argc, char *argv[]){    

    string player_name = argv[1];
    string player_num = argv[2];


   system("clear");     

   printReadyUp();
 
cout << endl;
    moveCursorUp(38);


    bool skipClearLine = true;
    while (!getUserInput(player_name, player_num))
    {
        printStatus(skipClearLine);
        std::this_thread::sleep_for(500ms);
    }

    system("clear");     

    cout << "The game will start when all players are ready." << endl;
    bool allPlayersReady;
    while (true)
    {
        allPlayersReady = printStatus();
        if (allPlayersReady)
        {
            break;
        }
        std::this_thread::sleep_for(500ms);
    }

    system("clear");
    cout << "ITS TIME" << endl;
    return 0;  
}
