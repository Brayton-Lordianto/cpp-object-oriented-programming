# Caesar Cypher

## Focus
- Vectors
- Strings
- Functions
- Standard I/O
- Clean code!

## Task
We are given a file of text that has been encrypted. For simplicity, we will assume the name is "encrypted.txt". Luckily we know what algorithm was used, the classic Caesar cypher. Each lowercase alphabetic character has been modified by “rotating” it through the alphabet some number of steps.

For example:
- If the number of steps was three and the original letter was 'b', then the encrypted file would instead have a 'e' in the same place, since 'e' is three letters past 'b' in the alphabet.
- If we have to encrypt the letter 'y' with the number of steps specified as three, the first step takes us to 'z'. The second step wraps around back to 'a'. The third and final step takes us to 'b'.

Your program will read an encrypted file and decrypt it. The first item in the file will be a positive int less than 26, the number of steps that was used in the rotation. The following lines will contain the text that you are to decode. Only lowercase alphabetic characters have been encrypted. The rest, e.g. uppercase, digits, and punctuation all appear in the encrypted file the same way they did in the original.

Additionally, the lines in the file have been reversed. So what should be the first line shows up last, etc.

### Example
Suppose this is the contents of the file you were asked to decrypt:
5
Asi ymj rtrjwfymjx tzylwfgj.
Aqq rnrxd bjwj ymj gtwtlwtajx
Dni ldwj fsi ldrgqj ns ymj bfgj.
Tbfx gwnqqnl fsi ymj xqnymjd ytajx
- That says that the original file was encoded with a Caesar cypher using a rotation of five. Remember that the lines have also been reversed meaning that the first line of text we see is the encoded version of what was the last line of code in the original file, etc. When we decrypt it, we should get:

Twas brillig and the slithey toves
Did gyre and gymble in the wabe.
All mimsy were the borogroves
And the momerathes outgrabe.

## Further Constraints
- Only use what we have covered
    - Specifically, do not use additional library features that we have not covered. Most such "abuses" will come from students who cannot think how to solve the problem with what they have learned, so they do a "search" for a solution. At this point, we are trying to get you to exercise your programming skills, not your web search skills.
    - Everything you need should be available in the online lecture notes. If you think what you need is not there, then talk to us.

- Pay attention to the "focus"
    - Each assignment starts with a list of topics that are the focus of the assignment.
    - E.g. in this assignment the focus list includes vectors. If you are not making use of vectors here, then you are missing something, possibly a big something. How would you be using a vector in this assignment? A file consists of lines, and we will need to display them in reverse order. A vector of the lines seems like an obvious thing to use.

- Functions
    - A good program will have the task nicely broken up into functions. Certainly the solution to this assignment could be written with all of the code in main and take up barely 20 lines. But our goal throughout the semester is to make a practice of good programming style and that means, among other things, of making good use of functions.
    - Function definitions should appear after the function main. Before main, you should provide function prototypes for each of the functions.
    - If you are starting this assignment before function prototypes have been covered, then just put the definitions before main. Adding the prototypes, and moving the function definitions, will only take a few minutes.
    - For this assignment we will actually tell you a couple of functions to provide. Usually we would leave it up to you to come up with them. After all, programming is supposed to be fun and it is less fun when someone is telling you what to do every step of the way.
        - First, write a function that takes an encrypted character and the rotation distance, and returns the corresponding decrypted character. Note that only lowercase characters will result in a change. For all other characters, the function will return the input value. (Note that the function's return type will need to be char.)
            - No, don't use the function islower or anything similar. Part of what you are learning here is that characters are handled differently in C++, along with many other languages, than in Python.
        - Second, write a function that takes a string and the rotation distance. It will modify the string that is passed in, using the above function, changing it into its unencrypted form. Note that unlike in some languages strings in C++ are mutable, meaning that we can directly change their contents. This function is passed the string to be modified along with the rotation distance and changes the string in place. It does not return anything, i.e. it has a void return type.
    - Neither of the above functions prints anything. That will be the task of main, or another function if you see some merit in further breaking up the program.
    - If you have not covered functions and parameter passing in your lecture when this assignment is posted, you can still take a first pass at solving the problem, putting everything in main, and then when functions are covered, add it to your solution. Once you know how, the changes shouldn't take long to add to a correctly working program. You can read up on functions and parameter passing in the lecture notes.
