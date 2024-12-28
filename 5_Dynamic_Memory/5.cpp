//
//  main.cpp
//  hw05: Dynamic Memory
//
//  Created by Brayton Lordianto on 2/26/22.
/*
 This program performs a set of commands listed in a file, delimited by line. The commands are expected to be Warrior, Battle, and Status, Hire, Fire, and Clear with a fixed corresponding format for expected arguments of each command. The program stores the warriors in a vector of a warrior struct, edits their strengths, and displays corresponding messages according to the commands being read.
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

//class is copied over from hw04
/*
 This class models a warrior with a strength and name.
 */
class Warrior {
    friend ostream& operator<<(ostream& os, const Warrior& warr);

public:
    Warrior(const string& name, double strength): name(name), strength(strength), isEmployed(false) { }
    const string& getName() const { return name; }
    double getStrength() const { return strength; }
    bool getIsEmployed() const { return isEmployed; }
    
    /*
     updates the strength of a warrior after a battle, accepting the ratio of the weaker strength to the stronger strength as a param. 1-ratio is the updated result of the strength because the noble loses 'ratio' of his strength and thus is left with '1-ratio' of his strength.
     */
    void setNewStrengthWithRatio(double ratio) { strength *= (1-ratio); }
    void toggleEmployed() { isEmployed ^= true; } // toggles the employed status of the warrior using the XOR operator
    
    
private:
    string name;
    double strength;
    bool isEmployed; // true if employed by a noble, false if not. will die employed. will start unemployed.
};

//class is copied over from hw04
/*
 This class models a noble with a name and an army of Warriors.
 */
class Noble {
    friend ostream& operator<<(ostream& os, const Noble& noble);
    
public:
    // no need to initialize vector - as the proffessor stated. Vector default constructor is what is wanted.
    Noble(const string& name): name(name), isAlive(true) { }
    const string& getName() const { return name; }
    
    /*
     function to hire a warrior. Receives the warrior to hire as a param. Checks if warrior is valid to be hired and hires them if so; otherwise print corresponding error message. Returns true if the warrior was hired and false otherwise.
     */
    bool hire(Warrior& warr) { // warr's employment status will be changed after hired.
        if (!hireValid(warr)) // if already employed or noble is dead.
            return false;
        
        // hire is valid. warrior should be made employed, and warrior should be added in noble's army.
        army.push_back(&warr); // push back address of warr.
        warr.toggleEmployed(); // warr is garunteed to be unemployed here. So, make employed.
        return true;
    }

    /*
     function to fire a warrior. Receives the warrior to fire as a param. Checks if warrior is valid to be fired and fires them if so; otherwise print corresponding error message. Returns true if the warrior was fired and false otherwise.
     */
    bool fire(Warrior& warr) { // warr's employment status will be changed after hired.
        size_t idx = getFiredIndex(warr); // the index of the warrior to fire. army.size() if invalid to fire.
        if (idx == army.size()) // fire is not valid if noble is dead or does not exist in the specified noble's army
            return false;

        // fire is valid. Commence the firing of the troop.
        cout << "You don't work for me anymore " << army[idx]->getName() << "! -- " << name << ".\n";
        
        // pop the back of the army after shifting the rest to the left to maintain order
        for (size_t leftIdx=idx; leftIdx<army.size()-1; ++leftIdx)
            army[leftIdx] = army[leftIdx + 1]; // copy down elements
        army.pop_back(); // remove the back of the vector now that everything after the troop to fire is copied down
        
        // the warrior's employment status should be changed to unemployed
        warr.toggleEmployed(); // warr is garunteed to be employed here as it is valid to be fired. So, make unemployed.
        
        return true;
    }
    
    /*
     function to battle. receives the opponent that 'this' noble will fight as a param. checks if the two parties are eligible to battle. if so, commence the battle by changing the strengths of the army of both nobles in the battle. Print the appropriate messages.
     */
    void battle(Noble& opponent) { // opponent's army's strengths will be changed if none are dead
        cout << name << " battles " << opponent.name << endl;
        
        // check if there are dead. If so, print the appropriate message
        if (haveDead(opponent))
            return; // skip
        
        // otherwise both are alive.
        double thisArmyStrength = getArmyStrength();
        double oppArmyStrength = opponent.getArmyStrength();
        if (oppArmyStrength < thisArmyStrength) {
            winOrDrawAgainst(opponent, oppArmyStrength / thisArmyStrength); // ratio will be weaker/stronger, as it will be multiplied with the stronger to reduce it.
            cout << name << " defeats " << opponent.name << endl;
        } else if (thisArmyStrength < oppArmyStrength) {
            opponent.winOrDrawAgainst(*this, thisArmyStrength / oppArmyStrength);
            cout << opponent.name << " defeats " << name << endl;
        } else {
            winOrDrawAgainst(opponent, 1); // ratio between the two is one if they are equal.
            cout << "Mutual Annihilation: " << name << " and " << opponent.name << " die at each other's hands" << endl;
        }
    }
        
private:
    string name;
    vector<Warrior*> army;
    bool isAlive;
    
private:
    /*
     a function that exists for the hire method only. checks if the hire of the given warrior is valid. If so, return true. else, return false, and print out the appropriate message. Cannot hire if warrior is already employed or the noble hiring is already dead.
     */
    bool hireValid(const Warrior& warr) const {
        // * according to instructions, Nobles who are dead can neither hire nor fire anyone. (Note this will implicitly prevent dead Warriors from being hired.) so there is no need to check if the warrior passed to the hire is dead.
        // cant hire as dead people.
        if (!isAlive) { // true if the noble hiring is dead.
            cout << "Noble " << name << " is dead. No hires allowed." << endl;
            return false;
        }
        // cannot hire employed warriors.
        if (warr.getIsEmployed()) {
            cout << warr.getName() << " is already employed." << endl;
            return false;
        }
        return true;
    }

    /*
     a function that exists for the fire method only. checks if the fire of the given warrior is valid and returns the index of the warrior in the army vector to be fired if it is found in the army. If not found or the noble is dead, return vector size, and print out the appropriate message.
     */
    size_t getFiredIndex(const Warrior& warr) const {
        if (!isAlive)
            cout << name << " is dead. No fires allowed." << endl;
        else {
            for (size_t i = 0; i<army.size(); ++i) // search for the warrior to fire
                if (army[i] == &warr)
                    return i;
            cout << warr.getName() << " not found in army of " << name << ".\n";
        }
        return army.size(); // return this if the fire is not valid.
    }
    
    /*
     A function used for the battle function. returns the cumulative strength of all the warriors in the nobles army.
     */
    double getArmyStrength() const {
        double totalStrength = 0;
        for (const Warrior* const troop: army) // the pointer should not be changed so it is the correct troop; the troop's data should not be modified.
            totalStrength += troop->getStrength();
        return totalStrength;
    }
        
    /*
     method that only exists for the battle method. Receives the opponent (const ref) of the battle as a param. Returns true if there is a dead noble between the nobles battling and false otherwise. Prints the corresponding message if one or more nobles are dead.
     */
    bool haveDead(const Noble& opp) const {
        bool firstDead = !isAlive;
        bool secondDead = !opp.isAlive;
        
        if (!firstDead && !secondDead) // both are alive
            return false;
        
        // if either are dead, display the corresponding output
        if (firstDead && secondDead)
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
        else if (firstDead)
            cout << "He's dead, " << opp.name << endl;
        else // second dead.
            cout << "He's dead, " << name << endl;
        return true;
    }
    
    /*
     A function that only exists for the battle method. This function will be called by the noble with a higher or equal army strength. The noble with the weaker or equal army strength, along with the ratio of (weaker : stronger) will be passed in as the params. The weaker or equal army's strengths will be zero as they would die, while the stronger or other equal one will result in strengths reduced by the ratio between the two strengths. Equal strengths would result in both army's strengths to be zero and the order of parameters in that case will not matter, and the ratio passed in will be one. There will be a check for a draw, which will cause 'this' noble to die off.
     */
    void winOrDrawAgainst(Noble& loserOrEq, double ratio) {
        // the noble will be declared as dead. The noble that calls this method will also have their armies' troops set to zero strength
        loserOrEq.dieOff();
        
        // if it is a draw, kill off 'this' Noble
        if (ratio == 1) {
            dieOff();
            return;
        }
        
        // the winner will have all troops' strength changed according to the ratio.
        for (Warrior* const troop: army)
            troop->setNewStrengthWithRatio(ratio);
    }
    
    /*
     A function to be called by a noble that will be declared as dead. Takes no params. As it dies, all the warriors in the Noble's army will have their strengths set to zero. This will be achieved through the warrior method that changes their strength.
     */
    void dieOff() {
        isAlive = false;
        for (Warrior* const troop: army)
            troop->setNewStrengthWithRatio(1); // by passing a ratio of 1, the new strength will be zero.
    }
};

// function prototypes
size_t findWarriorIdx(const vector<Warrior*>& warrs, const string& warrName);
size_t findNobleIdx(const vector<Noble*>& nobles, const string& nobleName);
bool nobleCmd(ifstream& file, vector<Noble*>& nobles);
bool warriorCmd(ifstream& file, vector<Warrior*>& warrs);
bool fireOrHireCmd(ifstream& file, vector<Noble*>& nobles, vector<Warrior*>& warrs, const string& hireOrFire);
void battleCmd(ifstream& file, vector<Noble*> nobles);
void statusCmd(const vector<Warrior*>& warrs, const vector<Noble*>& nobles);
void clearCmd(vector<Warrior*>& warrs, vector<Noble*>& nobles);

/*
 carries out commands from the file nobleWarriors.txt
 */
int main() {
    // initalize variables
    ifstream file("nobleWarriors.txt"); // open the file
    vector<Warrior*> warrs;
    vector<Noble*> nobles;
    string cmd;

    // carry out each command
    while (file >> cmd)
        if (cmd == "Noble")
            nobleCmd(file, nobles);
        else if (cmd == "Warrior")
            warriorCmd(file, warrs);
        else if (cmd == "Hire")
            fireOrHireCmd(file, nobles, warrs, "hire");
        else if (cmd == "Fire")
            fireOrHireCmd(file, nobles, warrs, "fire");
        else if (cmd == "Battle")
            battleCmd(file, nobles);
        else if (cmd == "Status")
            statusCmd(warrs, nobles);
        else if (cmd == "Clear")
            clearCmd(warrs, nobles);
        else
            cerr << "Command Not Found!\n";
    
    return 0;
}

/*
 Receives the file stream reference and the existing nobles vector. attempts to create a Noble on the heap. Returns true if the noble is succesfully added, or false if it failed.
 */
bool nobleCmd(ifstream& file, vector<Noble*>& nobles) {
    // get the name of the noble to add.
    string name;
    file >> name;
    
    // check if a noble with the name already exists. If so, print an error and skip.
    if (findNobleIdx(nobles, name) != nobles.size()) {
        cout << "Noble with the name " << name << " already exists. Thus, it will not be added.\n";
        return false;
    }
    
    // warrior can be added, so add it
    nobles.push_back(new Noble(name));
    return true;
}

/*
 Receives the file stream reference and the existing warriors vector. attempts to create a Warrior on the heap. Returns true if the warrior is succesfully added, or false if it failed.
 */
bool warriorCmd(ifstream& file, vector<Warrior*>& warrs) {
    // get the name and strength of the warrior to add.
    string name;
    int strength;
    file >> name >> strength;
    
    // check if a warrior with the name already exists. If so, print an error and skip.
    if (findWarriorIdx(warrs, name) != warrs.size()) {
        cout << "Warrior with the name " << name << " already exists. Thus, it will not be added.\n";
        return false;
    }
    
    // warrior can be added, so add it
    warrs.push_back(new Warrior(name, strength));
    return true;
}

/*
 Attempts to call the Noble's hire or fire method. Receives the file stream and the existing warriors vector of ptrs and the nobles'. returns true if the hire or fire method can be called succesfully and false otherwise.
 */
bool fireOrHireCmd(ifstream& file, vector<Noble*>& nobles, vector<Warrior*>& warrs, const string& hireOrFire) {
    // get the noble name calling the hire or fire and the warrior name to hire or fire
    string nobleName, warrName;
    file >> nobleName >> warrName;
    
    // check if a Noble tries to hire or fire a Warrior and either of them do not exist or the Warrior is already hired or is not in the noble's army.
    // error checking of already employed warriors and warriors not in the noble's army is taken care of through method hire and fire.
    size_t warrIdx = findWarriorIdx(warrs, warrName);
    size_t nobleIdx = findNobleIdx(nobles, nobleName);
    if (warrIdx == warrs.size()) {
        cout << "Attempting to " << hireOrFire << " using unknown warrior: " << warrName << endl;
        return false;
    }
    if (nobleIdx == nobles.size()) {
        cout << "Noble with the name " << nobleName << " does not exist... thus cannot " << hireOrFire << ".\n";
        return false;
    }
    
    // can be hired or fired. so hire or fire.
    if (hireOrFire == "hire")
        return nobles[nobleIdx]->hire(*warrs[warrIdx]);
    else
        return nobles[nobleIdx]->fire(*warrs[warrIdx]);
}

/*
 Attempts to call the Noble's battle method. Receives the file stream and the existing nobles vector of ptrs as params. returns true if the battle succesfully commences, and false if not, in the cases that one or both the nobles in battle do not exist.
 */
void battleCmd(ifstream& file, vector<Noble*> nobles) {
    // get the two names of the nobles to battle
    string name1, name2;
    file >> name1 >> name2;
    
    // check if the nobles with said names exist
    size_t idx1 = findNobleIdx(nobles, name1);
    size_t idx2 = findNobleIdx(nobles, name2);
    if (idx1 == nobles.size() && idx2 == nobles.size()) {
        cout << "Both nobles with names " << name1 << " and " << name2 << " does not exist! Battle not commenced.\n";
        return;
    }
    if (idx1 == nobles.size()) {
        cout << "Noble with name " << name1 << " does not exist! Battle not commenced.\n";
        return;
    }
    if (idx2 == nobles.size()) {
        cout << "Noble with name " << name2 << " does not exist! Battle not commenced.\n";
        return;
    }

    // nobles exist. commence battle
    nobles[idx1]->battle(*nobles[idx2]);
}

/*
 Receives the warrior's name and the existing warriors ptr vector as params. Finds a warrior's name in the vector of existing warrior ptrs and returns it. If not found, returns the size of the vector.
 */
size_t findWarriorIdx(const vector<Warrior*>& warrs, const string& warrName) {
    for (size_t idx=0; idx<warrs.size(); ++idx)
        if (warrs[idx]->getName() == warrName)
            return idx;
    return warrs.size();
}

/*
 Receives the noble's name and the existing nobles ptr vector as params. Finds a noble's name in the vector of existing noble ptrs and returns it. If not found, returns the size of the vector.
 */
size_t findNobleIdx(const vector<Noble*>& nobles, const string& nobleName) {
    for (size_t idx=0; idx<nobles.size(); ++idx)
        if (nobles[idx]->getName() == nobleName)
            return idx;
    return nobles.size();
}

/*
 Receives the warrior and noble ptr vectors. Prints all existing nobles and warriors in the vector.
 */
void statusCmd(const vector<Warrior*>& warrs, const vector<Noble*>& nobles) {
    cout << "Status\n======\n";
    cout << "Nobles:\n";
    if (nobles.size() == 0)
        cout << "NONE\n";
    else
        for (const Noble* const noble: nobles)
            cout << *noble << endl;
    
    cout << "\nUnemployed Warriors:\n";
    bool notNone = false;
    
    // const warrior so the warrior in the vector is not changed. const ptr so ptr stays the same.
    for (const Warrior* const warr: warrs)
        if (!warr->getIsEmployed()) { // if the warrior is unemployed
            notNone = true;
            cout << *warr << endl;
        }
    if (!notNone)
        cout << "NONE\n";
}

/*
 prints the warrior passed as a param.
 */
ostream& operator<<(ostream& os, const Warrior& warr) {
    os << "\t" << warr.name << ": " << warr.strength;
    return os;
}

/*
 prints the noble passed as a param.
 */
ostream& operator<<(ostream& os, const Noble& noble) {
    os << noble.name << " has an army of " << noble.army.size();
    for (size_t i=0; i<noble.army.size(); ++i)
        os << endl << *noble.army[i];
    return os;
}

/*
 clears the warrior and noble vector. removes the warriors and nobles on the heap.
 */
void clearCmd(vector<Warrior*>& warrs, vector<Noble*>& nobles) {
    for (const Noble* const noble: nobles)
        delete noble;
    nobles.clear();
    
    for (const Warrior* const warr: warrs)
        delete warr;
    warrs.clear();
}
