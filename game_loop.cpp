#include <iomanip>
#include <cstdio>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;

// to simplify the timer
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


namespace Files
{
    string outputFile = ".game_files/output.txt";
    string inputFile = ".game_files/game_text.txt";
    string scores = ".game_files/scores.csv";
}
class Getchar {
private:
    termios   oldterm;
    termios   newterm;
    char getch_(bool echo);
public:
    char getch(void);
    void initTermios(bool echo);
    void resetTermios(void);
};

char Getchar::getch(void)
{
  return getch_(false);
}

void Getchar::initTermios(bool echo)
{
  tcgetattr(0, &oldterm); //grab old terminal i/o settings
  newterm = oldterm; //make new settings same as old settings
  newterm.c_lflag &= ~ICANON; //disable buffered i/o
  newterm.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &newterm); //apply terminal io settings
}

/* Restore old terminal i/o settings */
void Getchar::resetTermios(void)
{
  tcsetattr(0, TCSANOW, &oldterm);
}

char Getchar::getch_(bool echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

struct Text
{
    string text;

    string userInput;

    string playerName;
    string date;

    void print_text()
    {
        cout << text;
    }

    int size()
    {
        return text.size();
    }

    int UserInputMistakeCount()
    {
        int mistakeCount = 0;
        for (int i=0; i < text.size(); i++)
        {
            if (text[i] != userInput[i])
            {
                mistakeCount++;
            }
        }
        return mistakeCount;
    }
};

class Game
{
    public :

        bool state;

        void clear_screen(void)    
        {    
            system("clear");    
        }
};

class CursorPosition
{
    public :
        int line_pos;
        int row=1;

        int last_x, last_y;

        int typedRow;

        void newline(int num = 1)
        {
            last_x = line_pos;
            last_y = row;

            row += num;
            line_pos = 0;
            printf("%c[%d;%df",0x1B,row,line_pos);
        }

        void toTypeRowEnd(int position)
        {
            last_x = line_pos;
            last_y = row;

            line_pos = position;
            row = typedRow;

            printf("%c[%d;%df", 0x1B, typedRow, position+1);
        }

        void delete_character()
        {
            // This janky -1 / +1 lets us do multiple deletes on a row

            // In order to delete a character we need to decrease the line position OF THE LAST NON DEL CHARACTER by 1
            line_pos--;
            
            // move the cursor position to one space after that
            printf("%c[%d;%df",0x1B,row,line_pos+1);
            
            // replace the character with a new one
            cout << " ";
            
            // move back over the character we replaced
            printf("%c[%d;%df",0x1B,row,line_pos+1);
            
        }
};

struct Colours
{
    string green = "\033[0;32m", italic_green = "\033[3;32m", red = "\033[0;31m", italic_red = "\033[3;31m", yellow = "\033[0;33m", blue = "\033[0;34m", magenta = "\033[0;35m", cyan = "\033[0;36m", italic_grey = "\033[3;90m", reset = "\033[0m";
};

std::string detectKeyPress(Game& game, CursorPosition& position, Text& textObj)
{

    Colours colour;
    
    bool lastCharCorrect;

    Getchar mygch;
    char ch;
    int int_ch;
    int escape_ch = 7;

    int character_count=0; 

    std::string content ="";

    std::ofstream myfile;
    myfile.open (Files::outputFile, std::ios::trunc); // open and empty file
    myfile.close();

    game.state = true;

  while (game.state) {

    
     // get single  key press
    ch = mygch.getch();
    int_ch = (int)ch;
    
    //if (int_ch == 127 && !lastCharCorrect ) //delete character
    if (int_ch == 127) //delete character
    {

        //content = content.substr(0, content.size()-2); //remove last character
        content = content.substr(0, content.size()-1); //remove last character

        character_count--;
        position.delete_character(); 

        myfile.open(Files::outputFile);
        myfile << content;
        myfile.close();
    }

    if (int_ch == 7) // escape character
    {
        cout << "escape" << endl;
        return "";
    }

    //if (int_ch != 7 || int_ch != 127) //ie not ESC or delete
    if (int_ch != 127) //ie not ESC or delete
    {
        myfile.open (Files::outputFile, std::ios::app);
        myfile << ch;
        myfile.close();
        
        content += ch;
        
        character_count++;
        //position.line_pos++;
        position.line_pos = content.size();

        //if (textObj.text[character_count-1] == ch)
        if (textObj.text[content.size()-1] == ch)
        {
            lastCharCorrect = true;
            cout << colour.green << ch << colour.reset;

            /* 
            // print if correct character
            position.newline();
            cout << colour.italic_green << "Correct character  " << colour.reset;
            position.toTypeRowEnd(character_count);
            */
        }
        else
        {
            lastCharCorrect = false;
            cout << colour.red << ch << colour.reset;
            
            /*
            // print if incorrect character
            position.newline();
            cout << colour.italic_red << "Incorrect character" << colour.reset;
            position.toTypeRowEnd(character_count);
            */

        }
        

        
        if (character_count == textObj.size())
        {
            game.state = false;
        }
    } 
    
  
  } //while
    return content;
} // detectKeyPress()

double CalculateWordsPerMinuite(int numberOfLetters, double seconds)
{
    double minutes = seconds / 60;
    double words = numberOfLetters / 5;
    return words / minutes;
}

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

int main( int argc,      // Number of strings in array argv
          char *argv[],   // Array of command-line argument strings
          char *envp[] )  // Array of environment variable strings
{



    Text textObj;

    //GetPlayerNameAndTime(textObj);
    
            //textObj.playerName = "John Doe" ;
            //textObj.date = "1997-01-01 00:00:00";
            textObj.playerName = argv[1] ;
            textObj.date = argv[2];

    Game game;
    //game.clear_screen();

    Colours colour;

    CursorPosition position;
    position.line_pos = 0;
    
    
    textObj.text = ReadInputFile();

    // print game text
    textObj.print_text();
    position.newline(2);


    position.typedRow = position.row;


    // start timer at t1
    auto t1 = high_resolution_clock::now();

    // Main game loop
    textObj.userInput = detectKeyPress(game, position, textObj);
    

    // stop timer at t2
    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */    
    auto ms_int = duration_cast<milliseconds>(t2 - t1);  
    double timeTaken_seconds = (double)ms_int.count()/1000;

    double wpm = CalculateWordsPerMinuite(textObj.size(), timeTaken_seconds); 

    position.newline(2);
    cout << colour.italic_grey << "GAME OVER." << colour.reset << endl;



    std::ofstream myfile;
    myfile.open(Files::scores, std::ios::app);
    myfile << textObj.playerName << "," << textObj.date << "," << wpm << "," << timeTaken_seconds << "," << textObj.UserInputMistakeCount() << endl;;
    myfile.close();

    cout << "Name: " << textObj.playerName << endl;
    cout << "Date: " << textObj.date << endl;
    cout << "Mistakes: " << textObj.UserInputMistakeCount() << endl; 

    cout << "Time taken: " << timeTaken_seconds  << "s" << endl;
 
    cout << "WPM: " << wpm << endl;


  return 0;
}
