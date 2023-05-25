#include <string>
#include <vector>
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

struct Colours
{
    string green = "\033[0;32m", italic_green = "\033[3;32m",
           red = "\033[0;31m", italic_red = "\033[3;31m",
           yellow = "\033[0;33m", bold_yellow = "\033[1;33m", italic_yellow = "\033[3;33m",
           blue = "\033[0;34m",
           magenta = "\033[0;35m",
           cyan = "\033[0;36m",
           italic_grey = "\033[3;90m", reset = "\033[0m";
};

void printReadyUp()
{
    Colours c;
    cout << c.red << R"(
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
)" << c.reset;
}

void printWaitingFor()
{

Colours c;
    cout << c.green << R"(

                                                                                                                                                                 ~##?                                                                                                                                   ~5P5J!.                                                 
                                                                                                                                      !?:                        ^@@B                         .~^        ?GY.                                                                                          ^@@&B@@#~                                                
                                                                                                                                     ^@@P                        :&@G                         ~&B.       B@@:                                                                                          ?@@J ?@@Y                                                
                                                                                                                                     ^&@B                        :&@G          .~!!!!!^         .        G@&:            ^P5.                                                                          7@@J .JY^                                                
                                                                                                                                      G@@^         ~?!.          ^&@G        :Y&@@&&@@&YY~    ^7^        G@&:            :JJ.      .~!^.             ....                                              7@@J                                                     
                                                                                                                                      !@@P       7G@@@5          7@@Y       :#@&J~^^^Y@@@P   .#@B.       G@&:                    .J&@@@&5~        .!P#&&#P.                                            7@@J                                 :~~~~::             
                                                                                                                                       5@@?    :5@@P5@@G^        P@@!       Y@@@####&@@@@P   .#@#.       P@@7~~!JPG!            :#@&J^7#@@7      ^B@@P?JG5.                                            7@@J                               .Y&@@@@@&7            
                                                                                                                                       .#@&^ ^Y&@#7  !G@@5^... :Y@@5        :7???????!!@@G.  .#@B.       J@@@@@@&BP~    .^:     7@@J   :G@@?    .#@@7...::~YP7                                         ?@@J ..:^!?Y!   .~~~:^77~         ^B@@J:^~~~.            
                                                                                                                                        ^#@&G@@B?.     7B@@####@@#?                    5@@5  .B@&^       ~@@#!::.       Y@&^    7@@J    .#@&:   :#@@&@@@@@@@@B                                        J&@@&##&@@@&&J  !#@@&BB@@&:        B@&~                   
                                                                                                                                         :PBBG!          ^7?????!.                      ^!:   7##7        G@&^          P@@~    7@@Y     ?@@J    :~!!!!!!~7&@B                                        ^P@@G????!^..  ?@@B!!B@@G~        .B@#.                   
                                                                                                                                                                                               ..         !@@5          P@@~    .#@&:    ~@@#^            !@@5                                         7@@?          Y@@&#@@#!           B@&!                   
                                                                                                                                                                                                          .#@B          P@@~     5@@~     7GB!            ?@@?                                         ?@@B~          ~!?J?~.            ^#@@~                  
                                                      .!777~~^::.                                                                                                                                          ^!:          ?&B:     .!~                      P@@~                                         ^&@@?                              :Y5^                  
                                                     !&@@@@@@@&&#^                                                                                                                                                       .                               !@@B                                           ^!~                                                     
                                                     .5@@@@@BJ!!~.                                                                                                                                                                             .!77777?YP@@B!                                                                                                   
                                                      :&@#JB@@P~                                                                             :^.                                                                                               P@@&&&&&#G5?.                                                                                                    
                                                       P@@! ~P@@BJ^                                                                         Y&@5                                                                                               :^:.....                                                                                                         
                                                       !#&7   ^JB@@G7.                                                 !GP:                Y@@P.                                                                                                                                                                                                                
                                                        ..       ^5&@&Y~                                               G@@~               ~@@B.                                                                                                                                                                                                                 
                                                                   .7G@@BJ^                                           .#@#.               G@@^                                                                                                                                                                                                                  
                                                                      ^JB@@G?:                                        .#@B     ....^~~.  ~@@G            :^:.                                                                                                                                                                                                   
                                                                         ~Y#@@BJ^.                                    .#@&P5PB#&&&&@@&!  G@@~          :P@@@#J         :^^^:.               .:^^^.                                                   .~!~^.                   75!                                                                               
                                                                            ~JB@@&G?^                                 .#@@BGG5Y???7!^:  J@@J .~?5G#Y .J&@#J&@&.       5@@@@@#~            ^Y#@&&@&J                                                 :B@@@@&7      .G#J       .&@#.  .77.                 .:^^^^:.                                               
                                                                               :!Y#@@BY!^.                            .#@#.            ^&@&JP&@@#@@#?#@@@#B@@5        7#@@@@G^           ^&@@GJY&@@:                                                ~@@G^!YBP^    ^@@G       ~@@G   J@@7                ?#@&&&@@G                                               
                                                                                   ^JG&@@&GJ~.                        .#@#.            P@@@@#5!^G@@B@@#7JYYY!           ^?G@@#P7^        !@@@&&&&&P.                                                ^@@#!.5@@?    ^@@G       P@@!   J@@J       7Y!     7@@B777?Y7.                                              
                                                                                      .^7YB@@&GJ~                     .&@#.           :#@#J^  .P@@7:5&@&57~~^::~~:         :7G@@@7       :&@&7^::.                                                   ~P@@@@@@P    .B@@57!~~~P@@J    :B@@Y????JB@@B     ^G@@&&&&&&B^                                             
                                                                                           :!YB&P.                     JGJ             .:.    B@@P   :7P#@@@@@@@@P  .:::::~~?5@@&~        !B@@#5YYY??????!.                                            :~~7&@B     .?B&&@@@@@&Y      :5B&&&&&#B@@B       :^~~~~7&@#.                                            
                                                                                               .                                              JGP^       ::^~!!!^. :#@@@@@@@&BP?.           ^?PBBB#&&&&&&B:                                                G@@~       .:::::::          ..... :&@B         ::   J@@P                                            
                                                                                                                                                                    ~!!!!!^^:                      ......                                                  ?@@J                               :&@B       Y&@@##&&@@B                                            
                                                                                                                                                                                                                                          :5P?~^:.         7@@J              !PP~..           ^&@B       ~J??JJ?!!!:                                            
                                                                                                                                                                                                                                          :G#@@@@#BPJ7~:.  7@@J              Y@@@&#B5?~^:.   .G@@!                                                              
                                                                                                                                                                                                                                             :^~7JPB#@@@&##&@@J               ^~!7JP#&@@&####@@&J                                                               
                                                                                                                                                                                                                                                    .:^!?JJJJ?:                      .^~7JJJJJ!.                                                                
                                                                                                                                                                                                                                                                                                                                                                
)" << c.reset;

}

void moveCursorUp(int rows)
{
    for (int i=0; i < rows; i++)
    {
        cout << "\x1b[A";
    }
}

struct Box : public Colours    
{    
    
    string selectedColour;    
    
    int width;    
    int height;    
    
    int line_count = 0;    
    
    void drawHorizontal(std::string first, std::string last)    
    {    
        for (int i=0; i <= width; i++)    
        {    
            if (i==0)    
            {    
                cout << selectedColour << first; // eg "╭";    
            }    
            else if (i == width)    
            {    
                cout << last << reset << endl; // eg "╮"     
            }    
            else    
            {    
                cout << "─";    
            }    
        }    
    }


    void setBoxColour(std::string colour)
    {
        selectedColour = colour;
    }

    void printToBox(vector<std::string> strings, std::string colour = "\033[0m" ) // reset colour = "\033[0m"
    {
        setBoxColour(colour);
        // find the length of the longest string
        size_t maxLength = 0;

        for (const std::string& str : strings)
        {
            if (str.length() > maxLength)
            {
                maxLength = str.length()+1;
            }
        }

        width = maxLength;
        drawHorizontal("╭","╮");

        int whiteSpace;
        for (const std::string& str : strings)
        {
            for (int j=0; j <= width-1; j++)
            {
                cout << " ";
            }
            // first draw the right edge of the box
            cout << selectedColour << "│";
           // then go to the start of the line and draw the content
            cout << "\r│" << reset << str << endl;
        }
        drawHorizontal("╰","╯");
    }

};

bool printStatus(bool skipClearLine = false)
{
    Colours colour;
    Box statusBox;

    std::string path = ".players";
    string file_name;
    int  player_count = 0;
    bool allPass = true;

    std::vector<std::string> allPlayerStatus = {" The game will start when all players are ready "};
    //allPlayerStatus.clear(); // make sure the vector is empty
    for (const auto & entry : fs::directory_iterator(path))
    {

        player_count++;

        std::ifstream file(entry.path());      

        string ready_state, name, player_status; 
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
                player_status = "✅ ";
            }
            else
            {
                player_status = "❌ ";
                allPass = false;
            }
            player_status += name;  

            allPlayerStatus.push_back(" "+player_status+" ");
        }    
        else    
        {    
            cout << "ERROR - cannot read file" << endl;    
        }    
    }

      statusBox.printToBox(allPlayerStatus, colour.blue);

    if (allPass)
    {
        return true;
    }

    //cout << "debgu message " <<  endl;
    //moveCursorUp(player_count + 1); // plus 1 for the line above
    moveCursorUp(allPlayerStatus.size()  + 2); // plus 1 for the line above, plus 2 for the top and bottom of the box
    return false;
}

void updatePlayerFile(string& player_name, string& player_num)
{
    string filename = ".players/player_" + player_num;
    string contents = "true, " + player_name;
    std::ofstream myfile;    
    myfile.open (filename, std::ios::trunc); // open and empty file    
    myfile << contents; //its important there is no newline character at the end of this 
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
    
printWaitingFor();
 
   cout << endl;
    moveCursorUp(38);

    //cout << "The game will start when all players are ready." << endl;
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
