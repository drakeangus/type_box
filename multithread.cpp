#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

// for box
#include <vector>

// for each file in directory
#include <filesystem>                                                             
#include <fstream>
#include <sstream>                    
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

    void drawVertical()
    {
        for (int i=0; i <= height-2; i++)
        {
            cout << selectedColour << "│";
            for (int j=0; j <= width-2; j++)
            {
                cout << " ";
            }
            cout << "│" << reset << endl;
        }
    }

    //void drawBox(int width, int height)
    void drawBox()
    {
        // top
        drawHorizontal("╭","╮");
        // sides
        drawVertical();
        // bottom
        drawHorizontal("╰","╯");
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

string getPlayerName(int player_number)    
{    
    
    string file_name = "pre-game/.players/player_" + to_string(player_number);    
    
    std::ifstream file(file_name);    
    if (file.is_open())            
    {    
        std::stringstream buffer;    
        buffer << file.rdbuf();    
        std::string fileContent = buffer.str();    
        file.close();    
    
        size_t pos = fileContent.find(", ");    
        return fileContent.substr(pos+2);    
    }    
    else    
        return "Uknown Player";    
}    
    
vector <string> getOtherPlayerText(int player_number)    
{    
    
    Colours colour;    
    std::string path = ".game_files/output_files";    
    string file_name;    
    //int  player_count = 0;        
    
    std::vector<std::string> playerText = {};    
    //allPlayerStatus.clear(); // make sure the vector is empty        
    for (const auto & entry : fs::directory_iterator(path))    
    {    
        file_name = entry.path().filename().string();    
    
        if (file_name == "output_"+to_string(player_number)) // we don't want to print for the player running the script    
        {    
            continue;    
        }    
         
        string player_name = getPlayerName(player_number);  // THIS SHOULD NOT BE DONE HERE. THIS PART WILL BE LOOPED ALOT AND WE ONLY NEED TO DO THIS ONCE     
    
        std::ifstream file(entry.path());    
    
        string ready_state, name, player_status;    
        if (file.is_open())    
        {    
            std::stringstream buffer;    
            buffer << file.rdbuf();    
            std::string fileContent = buffer.str();    
            file.close();    
    
    
            playerText.push_back(" " + colour.italic_grey + player_name + colour.reset + " : ");    
            playerText.push_back(" " + fileContent + " ");    
        }    
        else    
        {    
            cout << "ERROR - cannot read file" << endl;    
        }    
    }        

return playerText;    
}

mutex mtx;

int line1_count=0;


int players = 4 - 1; //-1 to ignore ourselves

// Function to be executed in a separate thread
void task(int lineNumber, const string& message, int interval) {
    Box box;
    Colours colour;
    string counter;
    while (true) {
        // Acquire the lock to print on the specific line
        unique_lock<mutex> lock(mtx);
        cout << "\033[" << (lineNumber) *( ( players * 2 ) + 2 + 2 ) << ";0H";  // Move the cursor to the specific line

        if (lineNumber==0) // top display
        {

        // top display : other player text display

        vector <string> strings = getOtherPlayerText(1);

        box.printToBox(strings, colour.blue); 

        }
        else if (lineNumber==1) // bottom display
        {

        // bottom display functions

        counter = to_string(line1_count++);
        
        cout << counter;






        }



        cout << "\033[" << 50 << ";0H" << flush; // <---------------- flush is used to immediately print to the terminal from the buffer meaning we can move the cursor, then flush to avoid a jittery cursor
        lock.unlock();

        // Sleep for the specified interval
        this_thread::sleep_for(chrono::milliseconds(interval));
    }
}

int main() {

    system("clear");

    const int line0 = 0;
    const int line1 = 1;
    const string message0 = "Process 0";
    const string message1 = "Process 1";
    const int interval0 = 50;  // 0.75 second
    const int interval1 = 20;   // 0.2 seconds

    thread thread1(task, line0, message0, interval0);
    thread thread2(task, line1, message1, interval1);

    // Wait for threads to finish
    thread1.join();
    thread2.join();

    return 0;
}

