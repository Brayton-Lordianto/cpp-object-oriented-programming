//
//  main.cpp
//  hw02: functions, structs, vectors
//
//  Created by Brayton Lordianto on 2/7/22.
/*
 This program performs a set of commands listed in a file, delimited by line. The commands are expected to be Warrior, Battle, and Status, with a fixed corresponding format for expected arguments of each command. The program stores the warriors in a vector of a warrior struct, edits their strengths, and displays corresponding messages according to the commands being read.
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;


struct Warrior {
    string name;
    int strength;
};

// function prototypes
void open(ifstream& f);
void warriorCmd(ifstream& file, vector<Warrior>& vec);
void battleCmd(ifstream& file, vector<Warrior>& vec);
bool setIndices(size_t& idx1, size_t& idx2, const string& name1, const string& name2, const vector<Warrior> vec);
bool haveDead(const Warrior& warr1, const Warrior& warr2);
void setNewStrengths(int& strongerOrEq, int& weakerOrEq);
void statusCmd(const vector<Warrior>& vec);

int main(int argc, const char * argv[]) {
    // initalize variables
    ifstream file;
    vector<Warrior> vec;
    string cmd;
    
    // open the file
    open(file);
    
    // carry out each command
    while (file >> cmd)
        if (cmd == "Warrior")
            warriorCmd(file, vec);
        else if (cmd == "Battle")
            battleCmd(file, vec);
        else if (cmd == "Status")
            statusCmd(vec);
        else
            cerr << "no such cmd";

    return 0;
}

/*
 opens the filestream with a file name inputted by the user. The function will repeatedly call for a file name until the file can open.
 */
void open(ifstream& f) {
    do {
        f.clear();
        cout << "Enter a working file name: ";
        string str;
        cin >> str;
        f.open(str);
    } while (!f);
}

/*
Performs the warrior command. The arguments passed in to the warrior command is a warrior name and strength, which is passed from the filestream. This function stores the warrior into the vector if the name passed is unique.
*/
void warriorCmd(ifstream& file, vector<Warrior>& vec) {
    // get the name and strength of the warrior to add.
    string name;
    int strength;
    file >> name >> strength;
    
    // check if warrior already exists. if so, display an error.
    for (Warrior warr: vec)
        if (warr.name == name) {
            cerr << "Name Already Exists" << endl;
            return; // skip cmd
        }
    
    // else, make the new warrior
      // vec.emplace_back(<#_Args &&__args...#>) // don't use constructors
    Warrior warr;
    warr.name = name;
    warr.strength = strength;
    vec.push_back(warr);
}

/*
Performs the battle command. The arguments passed in to the battle command are the two names of the two warriors, passed from the filestream. strengths are distributed accordingly if both are alive. corresponding messages are shown.
*/
void battleCmd(ifstream& file, vector<Warrior>& vec) {
    // get the names of the warriors
    string name1;
    string name2;
    file >> name1 >> name2;
    
    // get index of both warriors
    size_t idx1 = vec.size();
    size_t idx2 = vec.size();
    
    // skip the cmd if it doesn't exist, otherwise the indices will be set
    if (!setIndices(idx1, idx2, name1, name2, vec))
        return; // skip cmd
    
    cout << name1 << " battles " << name2 << endl;
    
    // check if any are dead. If so, display the appropriate message
    if (haveDead(vec[idx1], vec[idx2]))
        return; // skip cmd
    
    // otherwise, both are alive, thus commence the battle.
    // the battle will recalculate strengths based on the stronger or weaker ones
    if (vec[idx1].strength > vec[idx2].strength) {
        setNewStrengths(vec[idx1].strength, vec[idx2].strength);
        cout << name1 << " defeats " << name2 << endl;
    } else if (vec[idx1].strength < vec[idx2].strength) {
        setNewStrengths(vec[idx2].strength, vec[idx1].strength);
        cout << name2 << " defeats " << name1 << endl;
    } else {
        setNewStrengths(vec[idx1].strength, vec[idx2].strength);
        cout << "Mutual Annihilation: " << name1 << " and " << name2 << endl;
    }
}

/*
 A function that, if the warrior exists, edits sets the indexes of the warriors with the two name parameters of the battle command. If it does not exist, the appropriate message is displayed. The function returns a boolean that determines whether the indices are fonud or not. This will be used to determine whether the battle should commence or not.
 */
bool setIndices(size_t& idx1, size_t& idx2, const string& name1, const string& name2, const vector<Warrior> vec) {
    for (size_t i=0; i<vec.size(); ++i) { // concurrent search
        if (vec[i].name == name1)
            idx1 = i;
        else if (vec[i].name == name2)
            idx2 = i;
    }
    
    // if they don't exist, then output an error.
    if (idx1 == vec.size() || idx2 == vec.size()) {
        cerr << "One or more of these warriors don't exist. No battle will commence!";
        return false;
    }
    return true;
}

/*
 A function that takes in two warriors in battle as parameters. handles the output in cases where one or more warriors passed in the battle parameter is dead. It will return a value that determines if the warriors are alive or dead, which will be used to determine if the battle will commence.
 */
bool haveDead(const Warrior& warr1, const Warrior& warr2) {
    bool firstDead = warr1.strength == 0;
    bool secondDead = warr2.strength == 0;
    
    if (!firstDead && !secondDead) // both are alive
        return false;
    
    // if either or both are dead, display the corresponding output
    if (firstDead && secondDead)
        cout << "Oh, NO! They're both dead! Yuck!" << endl;
    else if (firstDead)
        cout << "He's dead, " << warr2.name << endl;
    else if (secondDead)
        cout << "He's dead, " << warr1.name << endl;
    
    return true; // and return true because there are casualties
}

/*
 takes the references of the stronger and weaker strengths as parameters and recalculates them after the battle. The weaker or equal strength will be zero as the warrior would die, while the stronger or other equal strength will result in a strength reduced by the weaker or equal strength. Equal strengths would result in zero and the order of parameters will not matter.
 */
void setNewStrengths(int& strongerOrEq, int& weakerOrEq) {
    strongerOrEq -= weakerOrEq;
    weakerOrEq = 0;
}

/*
 performs the status command. Accepts the vector of warriors as a parameter and displays the information of all the warriors in the vector.
 */
void statusCmd(const vector<Warrior>& vec) {
    cout << "There are: " << vec.size() << " warriors" << endl;
    for (const Warrior& warr: vec) {
        cout << "Warrior: " << warr.name << ", strength: " << warr.strength << endl;
    }
}
