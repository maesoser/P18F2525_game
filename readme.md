This project consists on the generation of an increasing random sequence of numbers (0-9) and letters (A-F), which will be visualized in a 7 segment display, so that the player will have to input that sequence into a matrix keyboard when the system is done showing the sequence. Each time the player completes the input of the correct sequence, the same sequence will be shown again with an additional value (number/letter) at the end of it.

In the case the player fails to introduce the correct sequence in the correct order (as shown by the system), the system will visually indicate that the player has lost.

#### The game will state the following stages:
- The game will start by showing the first sequence, composed by a unique number/letter. This value will be randomly selected and shown in the display.

- The player will press the correspondent key at the matrix keyboard.

- After validation of the sequence introduced by the player, the microprocessor will show the same sequence but adding a new random number/letter, so that this new value will be shown at the end of the sequence.

- In the case the user fails to reproduce the shown sequence, an error signal will be visualized, and the game will be over.

- Different sounds for the keys will be configured, including initial and ending songs for the game.


## Step 1: Reading the matrix keyboard

In this second session one should implement a keyboard decoder to detect which key has been pressed. Key- debouncing should be taking into account to
correctly detect the stroked key. Annex 2 describes the way the keyboard works and describes the procedure to implement the reading method.
To control this procedure, a 7-segment display should be connected to the microcontroller ports. Each time a keystroke is detected the display will indicate the number/letter that represents the pressed key. This display will shine until a new key is pressed. It is recommended to use a BCD to 7 segment decoder (74LS47 is a good example) in order to simplify this step.

### Previous work:
- Flux diagram of the program including the keyboard routine, thedebouncing routine and the decoding of the pressed key.
- Flux diagram of the main program including the visualization of the pressed key as the result of the keyboard pressing.

### Laboratory work:
- Identify the pressed key on the matrix keyboard, avoiding possible bouncing and solving all existing problems with the debugging functions.
- Visualize the correspondent number/letter in the 7-segment display, verifying all of the possible cases.

## Step 2: Complete game without sounds.

The idea of the third step is to implement the full system without including the sound reproduction function. The generation of the random sequences, the visualization of them and the interpretation of the pressed keys are the base of the program to be completed in this session.
The game will have an start mode to be determined by the programmer (pressing a key, after a few seconds, after playing some initial music, etc), after this, a first random value will be shown for the player to replicate in a short time span (<10 seconds) between key strokes. After the player presses the key a new increased sequence will be shown if the pressed key is correct, or will show an error visualization sequence for the case the pressed key is incorrect or the player took a long time to press the key.


### Previous work:
- Flux diagram of the complete game where each routine shall be clearly specified according to its function.
- Description of the initial mode of the game, and also the process to show the player he/she has failed when introducing the sequence.

### Laboratory work:
- Consolidate the full game, verifying the debouncing function of the keyboard and taking into account the time between the multiple actions of the game.
- Use the debug environment of the MPLAB software in order to correct any errors in your program.

# Pinout
![Image](https://raw.githubusercontent.com/maesoser/P18F2525_game/master/pinout.png)

## (Maybe this is easier to understand...)

![Fritzing](https://raw.githubusercontent.com/maesoser/P18F2525_game/master/project.png)
