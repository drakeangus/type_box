#include "game_loop.h"

double CalculateWordsPerMinuite(int numberOfLetters, double seconds)
{
    double minutes = seconds / 60;
    double words = numberOfLetters / 5;
    return words / minutes;
}

mutex mtx;
int cursorPosX;


std::string ReadInputFile()
{
    ifstream f(Files::inputFile); //taking file as inputstream    
    if(f) 
    {    
        ostringstream ss;    
        ss << f.rdbuf(); // reading data    
        string str = ss.str();    
        str.pop_back();
        return str; 
    }
    else 
    {
        return "The quick brown fox jumps over the lazy dog";
    }

}

void GetPlayerNameAndTime(Text& textObj)
{
    
    for (std::string input; std::getline(std::cin, input);) 
    {
        if (input.empty()) 
        {
            textObj.playerName = "John Doe" ;
            textObj.date = "1997-01-01 00:00:00";
        }
        else
        {

            size_t pos_del = input.find(",");
            textObj.playerName = input.substr(0,pos_del);
            textObj.date = input.substr(pos_del+1);

        }
    }


}

string getPlayerName(string number)
{

    string file_name = "pre-game/.players/player_" + number;

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
        
        size_t pos = file_name.find("_");
        string other_player_number = file_name.substr(pos+1);

        if (file_name == "output_"+to_string(player_number)) // we don't want to print for the player running the script
        {
            continue;
        }
        string player_name = getPlayerName(other_player_number);  // THIS SHOULD NOT BE DONE HERE. THIS PART WILL BE LOOPED ALOT AND WE ONLY NEED TO DO THIS ONCE

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

std::atomic<bool> terminateFlag(false);
int character_count=0;
int playerCount;

//void displayTextThread(int playerNumber, int interval, Text& textObj)
void displayTextThread(int playerNumber, int interval, int width)
{

    Box box;    
    Colours colour;    
    //string counter;    
    while (terminateFlag) {    
        // Acquire the lock to print on the specific line    
        unique_lock<mutex> lock(mtx);    
        cout << "\033[" << 0 << ";0H";  // Move the cursor to the specific line    

        //counter = to_string(line1_count++);    
        vector <string> strings = getOtherPlayerText(playerNumber);    


        // TO DO : The box should take a size as an agrument so we can draw it to the correct size
        // also an improvement here would be to only draw the box once then just print inside
        // use some trickery to check if its the first time drawing the box 
        
        //int width = textObj.text.size() + 2;
        
        box.printToBox(strings, width, colour.blue);    

        // ** 20 is for the row number - need to fix this 
        cout << "\033[" << (playerCount * 2) + 2 + 1 << ";" << character_count + 1  << "H" << flush; // <---------------- flush is used to immediately print to the terminal from the buffer meaning we can move the cursor, then flush        
        lock.unlock();     

        // Sleep for the specified interval    
        this_thread::sleep_for(chrono::milliseconds(interval));    
    } 
return;
}

vector <string> drawPace(int characterCount, int targetCount)
{
    string output_front = "", output_back = "", middle_text="";

    string middle_text_full= "Snail boy";
    
    for (int i=0; i < characterCount && i < middle_text_full.size(); i++)
    {
        middle_text += middle_text_full[i];
    }

 /*   for (int i=0; i<targetCount - characterCount - middle_text.size(); i++)
    {
        output_back +=  ".";
    }
    */

    for (int i=middle_text_full.size(); i<characterCount; i++)
    {
        output_front += "_";

    }
    string output = output_front + middle_text + output_back;
    return {output};
    

   // return {"~~~~~~~~~~~~~~~~~~~~~~~~~~ Snail Boy" + to_string(characterCount) + " " + to_string(targetCount) + " ............................."};
}


void printSpeed(int interval, int width)
{
    
    Box box;    
    Colours colour;    
    //string counter;    
    while (terminateFlag) {    
        // Acquire the lock to print on the specific line    
        unique_lock<mutex> lock(mtx);    
        cout << "\033[" << 0 << ";0H";  // Move the cursor to the specific line    

        //counter = to_string(line1_count++);    
        vector <string> strings = drawPace(character_count, width - 2 );    

        box.printToBox(strings, width, colour.blue);    

        // ** 20 is for the row number - need to fix this 
        cout << "\033[" << (playerCount * 2) + 2 + 1 << ";" << character_count + 1  << "H" << flush; // <---------------- flush is used to immediately print to the terminal from the buffer meaning we can move the cursor, then flush        
        lock.unlock();     

        // Sleep for the specified interval    
        this_thread::sleep_for(chrono::milliseconds(interval));    
    } 
return;


}

string simpleDetectKeyPress(Text& textObj, CursorPosition& position)
{

    Colours colour;

        Getchar mygch;    
    char ch;    
    int int_ch;    
    
    std::string content ="";


std::ofstream myfile;    
    myfile.open (Files::outputFile, std::ios::trunc); // open and empty file    
    myfile.close(); 


    auto live_t1 = high_resolution_clock::now(); // outside while loop with t2 inside - Every itteration we'll calculate the new wpm

    while(terminateFlag)
    {

        ch = mygch.getch();
        int_ch = (int)ch;

        // Acquire the lock to print on the specific line
        unique_lock<mutex> lock(mtx);

        if (int_ch == 127) // delete character
        {
            character_count--; //global variable
            content = content.substr(0, content.size()-1);

            position.delete_character(textObj.text[content.size()]);

            myfile.open(Files::outputFile);    
            myfile << content;    
            myfile.close();

        }

        if (int_ch == 7) // escape character    
        {    
            cout << "escape" << endl;    
            return content;    
        } 

        if (int_ch != 127) // non delete
        {

            myfile.open (Files::outputFile, std::ios::app);
            myfile << ch;
            myfile.close();

            content += ch;
            character_count++; // global variable
            position.line_pos = content.size();

            if (textObj.text[content.size()-1] == ch) // correct input character
            {
                // perhaps this should be a function of its own
                // I could move to the correct line and position from it
                cout << colour.green << ch << colour.reset;
            }
            else  // incorrect input character
            {
                cout << colour.red << ch << colour.reset;
            }

            auto live_t2 = high_resolution_clock::now();

            // print live wpm every key press
        auto ms_int = duration_cast<milliseconds>(live_t2 - live_t1);
        double timeTaken_seconds = (double)ms_int.count()/1000;

        double wpm = CalculateWordsPerMinuite(character_count, timeTaken_seconds);

        position.newline();
        cout << colour.italic_yellow << wpm << colour.reset;
        position.toTypeRowEnd(character_count);

        
        }

        lock.unlock();

        if (character_count == textObj.size())
        {
            terminateFlag = false;
        }

    }

cout << endl << endl;

    return content;
}


void mainGameThread()
{

    Text textObj;
    CursorPosition position;
    

    position.line_pos = 0;

    position.typedRow = (playerCount * 2) + 2 + 1 ; //two lines per player + top/bottom of the box + a space inbetween + another 2 because the previous wasn't enough


    unique_lock<mutex> lock(mtx); // acquire the lock to print on the right line
    position.goTo(0,position.typedRow);

    // print game text from input file    
    textObj.text = ReadInputFile();    
    textObj.print_text();
   
    lock.unlock(); // unlock

    // start timer at t1
    auto t1 = high_resolution_clock::now();
    textObj.userInput = simpleDetectKeyPress(textObj, position); 

   // stop timer at t2    
    auto t2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(t2 - t1);    
    double timeTaken_seconds = (double)ms_int.count()/1000;    
    
    double wpm = CalculateWordsPerMinuite(textObj.size(), timeTaken_seconds);

    std::ofstream myfile;    
    myfile.open(Files::scores, std::ios::app);    
    myfile << textObj.playerName << "," << textObj.date << "," << wpm << "," << timeTaken_seconds << "," << textObj.UserInputMistakeCount() << endl;
    myfile.close();

    cout << endl;
    cout << "Name: " << textObj.playerName << endl;
    cout << "Total time taken: " << timeTaken_seconds << endl;
    cout << "Mistakes: " << textObj.UserInputMistakeCount() << endl; 
    cout << "WPM: " << wpm << endl;

    double score = wpm - ( textObj.UserInputMistakeCount() * 0.05 * wpm);

    cout << "Score: " << score << endl;


    return;
}

int getPlayerCount()
{
    string file_name = Files::game_control;

    std::ifstream file(file_name);
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        int playerCount = stoi(buffer.str());
        file.close();

        return playerCount;
    }
    else
    {
        int defaultPlayerCount = 6;
        return defaultPlayerCount;
    }
}

int main( int argc,      // Number of strings in array argv
          char *argv[],   // Array of command-line argument strings
          char *envp[] )  // Array of environment variable strings
{

    Text textObj;

    textObj.text = ReadInputFile();

    textObj.playerName = argv[1];
    textObj.date = argv[2];
    textObj.playerNumber = argv[3];
    int int_playerNumber = stoi(textObj.playerNumber);
    int width = textObj.text.size() + 2;

    // used for line spacing of the typed text
    playerCount = getPlayerCount();
    
    Files::outputFile =".game_files/output_files/output_"+textObj.playerNumber;

    system("clear");

    const int interval0 = 100;  // 0.75 second    

    terminateFlag=true; // allow while loops 
   
    thread thread1;
    if (playerCount > 1)
    {
        thread thread1(displayTextThread, int_playerNumber, interval0, width); 
    thread thread2(mainGameThread);
    // Wait for threads to finish    
    thread1.join();    
    thread2.join();
    }
    else    
    {
        thread thread1(printSpeed, interval0, width);
        thread thread2(mainGameThread);
        // Wait for threads to finish    
        thread1.join();    
        thread2.join();
    }



    cout << endl << endl;



    return 0;
}



