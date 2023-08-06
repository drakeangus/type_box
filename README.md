# type_box
 The synchronous multiplayer speed typing game in your terminal. 

![image](https://github.com/drakeangus/type_box/assets/65454619/d3f1774c-e832-4251-9341-9bfe15d58143)

Prints up to 10 players progress in real time while you all type the random phrase.

![image](https://github.com/drakeangus/type_box/assets/65454619/849a645d-9da6-4dc1-9416-399fb40a3839)

Since we're working in the terminal the main consideration of the code is cursor placement - we can only print to one place at any one time yet we have more than 10 lines of text updating all at once.

With multithreading, the main game loop is conveniently split into two parts, and with the use of output buffers and flush we can instantly print text to the screen without a jittery cursor jumping around the screen.

The first thread handles printing other players' text and runs on an interval. Every n milliseconds the thread acquires a lock on the cursor, it moves the cursor to the correct terminal coordinates then prints the box containing the player's strings to a buffer along with instructions to move back to the original location. Once this is done we flush the buffer, remove the lock on the cursor, and sleep the thread for a set interval.

While this is happening, on the second thread, we capture the key presses of the current user (including non-letter inputs like backspace) and check them against the text for the round of the game. If the letter is correct, we print it in green, and if not, red. Certain characters are mapped to different ones for ease of play (eg. space -> underscore). For the case where the user enters a backspace, we need to make a few considerations. The cursor position must move to the coordinates equivalent to one space back on the current line, we must print whitespace over the existing character to remove it from view and then move back over the previous space for the user to type a new letter. In addition, the pointer to the current letter in the reference text must be moved back as well otherwise every subsequent letter will be checked incorrectly.

We use a high-resolution clock to measure words per minute after each key press. Then we manipulate the cursor position again to print the speed in a third location in the player's terminal.
