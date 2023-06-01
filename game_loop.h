#ifndef Game_loop_H
#define Game_loop_H

#include <iomanip>    
#include <cstdio>    
#include <cstring>    
#include <termios.h>    
#include <unistd.h>    
#include <iostream>    
#include <string>    
    
// for each file in directory    
#include <filesystem>    
#include <fstream>    
#include <sstream>    
namespace fs = std::filesystem;    
    
// for box        
#include <vector>    
    
// for u_sleep    
#include <chrono>    
    
// for multithreading    
#include <thread>    
#include <mutex>    
    
    
// for terminateFlag
#include <atomic>    
    
using namespace std;    
    
// to simplify the timer    
using std::chrono::high_resolution_clock;    
using std::chrono::duration_cast;    
using std::chrono::duration;    
using std::chrono::milliseconds;


namespace Files
{
    //string outputFile = ".game_files/output.txt";
    string outputFile; // This needs to be unique per player
    string inputFile = ".game_files/game_text.txt";
    string scores = ".game_files/scores.csv";
    string game_control = ".game_files/game_control";
    string player_file = ".players/player_";
}


struct Colours
{
    string green = "\033[0;32m", italic_green = "\033[3;32m",
           red = "\033[0;31m", italic_red = "\033[3;31m",
           yellow = "\033[0;33m", bold_yellow = "\033[1;33m", italic_yellow = "\033[3;33m",
           blue = "\033[0;34m",
           magenta = "\033[0;35m",
           cyan = "\033[0;36m",
           italic_grey = "\033[3;90m", reset = "\033[0m",

            target_text_colour = italic_grey;
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

    void printToBox(vector<std::string> strings, int defaultWidth = 0, std::string colour = "\033[0m") // reset colour = "\033[0m"
    {
        setBoxColour(colour);
        // find the length of the longest string
        size_t maxLength = 0;

        if (defaultWidth == 0) //ie no width is given set the maxLength to the longest string
        {

            for (const std::string& str : strings)
            {
                if (str.length() > maxLength)
                {
                    maxLength = str.length()+1;
                }
            }

        }
        else
        {
            maxLength = defaultWidth;
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


// required to capture key presses 
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

struct Text : public Colours
{
    string text;
    string userInput;
    string playerName;
    string date;
    string playerNumber;

    void print_text()
    {
        cout << target_text_colour << text << reset;
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


class CursorPosition : public Colours
{
    public :
        int line_pos;
        int row=1;

        int last_x, last_y;

        int typedRow;

        void goTo(int x, int y)
        {
            printf("%c[%d;%df",0x1B,y,x);

        }

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

        void delete_character(char replace_character)
        {
            // This janky -1 / +1 lets us do multiple deletes on a row

            // In order to delete a character we need to decrease the line position OF THE LAST NON DEL CHARACTER by 1
            line_pos--;

            // move the cursor position to one space after that
            printf("%c[%d;%df",0x1B,typedRow,line_pos+1);

            // replace the character with a new one
            cout << target_text_colour << replace_character;

            // move back over the character we replaced
            printf("%c[%d;%df",0x1B,typedRow,line_pos+1);

        }
};





#endif
