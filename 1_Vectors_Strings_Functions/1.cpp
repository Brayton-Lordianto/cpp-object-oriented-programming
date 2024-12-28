//
//   Vectors, strings and functions
//
//  Created by Brayton Lordianto on 1/30/22.
/*
This program decrypts an encrypted file that follows the Caesar Cypher. The main function receives as input the given rotation of the cypher in the file and the string contents that will be decrypted. The decrypted contents are then decrypted and the result is printed as output.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// function prototypes
char decryptChar(int rotation, char character);
void decryptLine(int rotation, string& str);

int main() {
    // initialize variables
    ifstream file("encrypted.txt"); //file stream
    vector<string> lineVec; //vector of lines
    string line; //string to store each line
    int rotation; //the rotation
    string decrypted = ""; //the final decrypted result
    
    if (!file) { // check error opening file and return error if failed.
        cerr << "error loading file!" << endl;
        exit(1);
    }
    
    // get rotation
    file >> rotation;
    if (rotation >= 26) { // reject rotation more than 25 as in instruction
        cerr << "rotation constraint is to be less than 26" << endl;
        exit(2);
    }
    getline(file, line); //extra line to bypass \n
    
    // decrypting process
    while (getline(file, line)) { // get each line
        decryptLine(rotation, line); // decrypt each line, binded.
        lineVec.push_back(line); // add the decrypted line in the vector.
    }
    
    // reorganize lines
    for (size_t idx=lineVec.size(); idx>1; --idx) { // add each line from bottom to top , EXCEPT the last
        decrypted += lineVec[idx - 1] + '\n'; // with a line break between each line
    }
    // then add the last (which is first in the vector) individually so as to not have an extra line in the decrypted at the end.
    decrypted += lineVec[0];

    // print decrypted
    cout << decrypted << endl;
    
    return 0;
}

/*
 Given a rotation, this function rotates a given encrypted Caesar Cypher character by the rotation to the left; which decrypts the character and returns it.
 */
char decryptChar(int rotation, char character) {
    if (character >= 'a' && character <= 'z') { // if char is between lowercase a and z, decrypt it
        if (character - rotation < 'a') // cycle back if under 'a' - this means encryption went over 'z'
            return character - rotation - 'a' + 1 + 'z'; // minus 'a' plus 1 will yield offset from 'z'. plus 'z' to apply offset on 'z'.
        return character - rotation; // else, just the normal rotation deducted
    }
    return character; // else, nothing to decrypt
}

/*
 Given a rotation, this function takes in a line and decrypts it character per character; editing the encrypted character with the decrypted character.
 */
void decryptLine(int rotation, string& str) {
    for (char& c: str)
        c = decryptChar(rotation, c); // replace each character with its decrypted form.
}
