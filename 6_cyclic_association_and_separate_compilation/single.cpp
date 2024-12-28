//
//  main.cpp
//  Cyclic Association and Separate Compilation
//
//  Created by Brayton Lordianto on 3/17/22.
/*
 This program models a game of medieval times. Nobles and Warriors exist in this game. Nobles can hire and fire warriors, and can battle other nobles. Warriors have strength and their strength will be changed after engaging in battle.
 */

// Comment out these two includes for the single file solution
//#include "Noble.h"
//#include "Warrior.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Noble;

/*
 This class models a warrior with a strength and name.
 */
class Warrior {
    friend ostream& operator<<(ostream& os, const Warrior& warr);

public:
    const string& getName() const;
    double getStrength() const;
    void setNewStrengthWithRatio(double ratio);
    
private:
    string name;
    double strength;

    //MARK: members added/changed for hw06
public:
    Warrior(const string& name, double strength); // init list changed.
    bool isHired() const;
    bool runaway();
    void makeUnemployed();
    void setEmployer(Noble* noblePtr);
    bool employerIs(Noble* noblePtr) const;

private:
    Noble* employer;
};

/*
 This class models a noble with a name and an army of Warriors.
 */
class Noble {
    friend ostream& operator<<(ostream& os, const Noble& noble);
    
public:
    Noble(const string& name);
    const string& getName() const;
    void battle(Noble& opponent);
        
private:
    string name;
    vector<Warrior*> army;
    bool isAlive;
    
private:
    size_t getIndexToRemove(const Warrior& warr) const;
    double getArmyStrength() const;
    bool haveDead(const Noble& opp) const;
    void winOrDrawAgainst(Noble& loserOrEq, double ratio);
    void dieOff();
    
    //MARK: added and/or changed for hw06
public:
    bool removeWarrior(const Warrior& warr); // made public so warriors can call this on nobles when running away
    bool fire(Warrior& warr); // major changes to make it use removeWarrior and to make warrior unempoloyed
    bool hireValid(const Warrior& warr) const; // minor change to make use of isHired method
    bool hire(Warrior& warr); // minor change to use Warrior::setEmployer method
};

// Comment out this using namespace for the single file solution
//using namespace WarriorCraft;

int main() {
    Noble art("King Arthur");
    Noble lance("Lancelot du Lac");
    Noble jim("Jim");
    Noble linus("Linus Torvalds");
    Noble billie("Bill Gates");

    Warrior cheetah("Tarzan", 4);
    Warrior sky("Leia", 6);
    Warrior wizard("Merlin", 9);
    Warrior jaffa("Teal'c", 9);
    Warrior theGovernator("Conan", 12);
    Warrior nimoy("Spock", 15);
    Warrior lawless("Xena", 20);
    Warrior mrGreen("Hulk", 8);
    Warrior dylan("Hercules", 3);

    jim.hire(nimoy);
    lance.hire(theGovernator);
    art.hire(jaffa);
    art.hire(cheetah);
    art.hire(wizard);
    art.hire(sky);
    lance.hire(dylan);
    linus.hire(lawless);
    billie.hire(mrGreen);
    art.hire(nimoy);

    cout << "================ Initial Status ===============" << endl;
    cout << art << endl
         << lance << endl
         << jim << endl
         << linus << endl
         << billie << endl;
    cout << "===============================================\n\n";

    art.fire(cheetah);
    wizard.runaway();
    cout << endl << art << endl;

    art.battle(lance);
    jim.battle(lance);
    linus.battle(billie);
    billie.battle(lance);

    cout << "\n================ Final Status ===============" << endl;
    cout << art << endl
         << lance << endl
         << jim << endl
         << linus << endl
         << billie << endl;

    // Tarzan and Merlin should be unemployed
    cout << "Tarzan's Hire Status: " << boolalpha
         << cheetah.isHired() << endl;
    cout << "Merlin's Hire Status: " << boolalpha
         << wizard.isHired() << endl;
    cout << "===============================================\n\n";
}

/// ==================
/// MARK: WARRIOR CODE
/// ==================

/*
 prints the warrior passed as a param.
 */
ostream& operator<<(ostream& os, const Warrior& warr) {
    os << "\t" << warr.name << ": " << warr.strength;
    return os;
}

Warrior::Warrior(const string& name, double strength): name(name), strength(strength), employer(nullptr) { }

const string& Warrior::getName() const { return name; }

double Warrior::getStrength() const { return strength; }

/*
 updates the strength of a warrior after a battle, accepting the ratio of the weaker strength to the stronger strength as a param. 1-ratio is the updated result of the strength because the noble loses 'ratio' of his strength and thus is left with '1-ratio' of his strength.
 */
void Warrior::setNewStrengthWithRatio(double ratio) { strength *= (1-ratio); }

/*
 returns true if this warrior is employed and false otherwise.
 */
bool Warrior::isHired() const { return employer != nullptr; }

/*
 the warrior attempts to runaway. returns true if he/she can and succesfully does run away. else returns false.
 */
bool Warrior::runaway() {
    // if warrior is not employed they cannot run away or if they are dead.
    if (!isHired() || strength == 0) return false;
    
    cout << name << " flees in terror, abandoning his lord, " << employer->getName() << endl;
    
    // otherwise make it so he now has no employer, and the employer will remove employee
    if (employer->removeWarrior(*this)) { // just for safety.
        makeUnemployed();
        return true;
    }
    return false;
}

void Warrior::makeUnemployed() { setEmployer(nullptr); }

void Warrior::setEmployer(Noble* noblePtr) { employer = noblePtr; }

bool Warrior::employerIs(Noble* noblePtr) const { return employer == noblePtr; }

/// ==================
/// MARK: NOBLE CODE
/// ==================

/*
 prints the noble passed as a param.
 */
ostream& operator<<(ostream& os, const Noble& noble) {
    os << noble.name << " has an army of " << noble.army.size();
    for (size_t i=0; i<noble.army.size(); ++i)
        os << endl << *noble.army[i];
    return os << endl;
}

Noble::Noble(const string& name): name(name), isAlive(true) { }
const string& Noble::getName() const { return name; }

/*
 function to hire a warrior. Receives the warrior to hire as a param. Checks if warrior is valid to be hired and hires them if so; otherwise print corresponding error message. Returns true if the warrior was hired and false otherwise.
 */
bool Noble::hire(Warrior& warr) { // warr's employment status will be changed after hired.
    if (!hireValid(warr)) // if already employed or noble is dead.
        return false;
    
    // hire is valid. warrior should be made employed, and warrior should be added in noble's army.
    army.push_back(&warr); // push back address of warr.
    warr.setEmployer(this);
    return true;
}

/*
 function to fire a warrior. Receives the warrior to fire as a param. Checks if warrior is valid to be fired and fires them if so; otherwise print corresponding error message. Returns true if the warrior was fired and false otherwise.
 */
bool Noble::fire(Warrior& warr) { // warr's employment status will be changed after hired.
    // remove warrior takes care of error messages
    // simply do nothing if removal of warrior failed. checks first if the employer is indeed this employer
    if (!warr.employerIs(this) || !removeWarrior(warr)) return false;

    // warrior is removed. print successs msg.
    cout << "You don't work for me anymore " << warr.getName() << "! -- " << name << ".\n";
    
    // the warrior's employment status should be changed to unemployed
    warr.makeUnemployed();
    
    return true;
}

/*
 function to battle. receives the opponent that 'this' noble will fight as a param. checks if the two parties are eligible to battle. if so, commence the battle by changing the strengths of the army of both nobles in the battle. Print the appropriate messages.
 */
void Noble::battle(Noble& opponent) { // opponent's army's strengths will be changed if none are dead
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

/*
 a function that exists for the hire method only. checks if the hire of the given warrior is valid. If so, return true. else, return false, and print out the appropriate message. Cannot hire if warrior is already employed or the noble hiring is already dead.
 */
bool Noble::hireValid(const Warrior& warr) const {
    // * according to instructions, Nobles who are dead can neither hire nor fire anyone. (Note this will implicitly prevent dead Warriors from being hired.) so there is no need to check if the warrior passed to the hire is dead.
    // cant hire as dead people.
    if (!isAlive) { // true if the noble hiring is dead.
        cout << "Noble " << name << " is dead. No hires allowed." << endl;
        return false;
    }
    // cannot hire employed warriors.
    if (warr.isHired()) {
        cout << warr.getName() << " is already employed." << endl;
        return false;
    }
    return true;
}

/*
 a function that exists for the fire and removing warrior methods. checks if the removal of the given warrior is valid and returns the index of the warrior in the army vector to be removed if it is found in the army. If not found or the noble is dead, return vector size, and print out the appropriate message.
 */
size_t Noble::getIndexToRemove(const Warrior& warr) const {
    if (!isAlive) // if the noble calling is dead
        cout << name << " is dead." << endl;
    else {
        for (size_t i = 0; i<army.size(); ++i) // search for the warrior to renove
            if (army[i] == &warr)
                return i;
        cout << warr.getName() << " not found in army of " << name << ".\n";
    }
    return army.size(); // return this if the removal is not valid.
}

/*
 A function used for the battle function. returns the cumulative strength of all the warriors in the nobles army.
 */
double Noble::getArmyStrength() const {
    double totalStrength = 0;
    for (const Warrior* const troop: army) // the pointer should not be changed so it is the correct troop; the troop's data should not be modified.
        totalStrength += troop->getStrength();
    return totalStrength;
}
    
/*
 method that only exists for the battle method. Receives the opponent (const ref) of the battle as a param. Returns true if there is a dead noble between the nobles battling and false otherwise. Prints the corresponding message if one or more nobles are dead.
 */
bool Noble::haveDead(const Noble& opp) const {
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
void Noble::winOrDrawAgainst(Noble& loserOrEq, double ratio) {
    // the noble will be declared as dead. The noble that calls this method will also have their armies' troops set to zero strength
    loserOrEq.dieOff();
    
    // if it is a draw, kill off 'this' Noble
    if (ratio == 1) {
        dieOff();
        return;
    }
    
    // the winner will have all troops' strength changed according to the ratio.
    for (Warrior* const troop: army) troop->setNewStrengthWithRatio(ratio);
}

/*
 A function to be called by a noble that will be declared as dead. Takes no params. As it dies, all the warriors in the Noble's army will have their strengths set to zero. This will be achieved through the warrior method that changes their strength.
 */
void Noble::dieOff() {
    isAlive = false;
    for (Warrior* const troop: army)
        troop->setNewStrengthWithRatio(1); // by passing a ratio of 1, the new strength will be zero.
}

/*
 receives a warrior reference and attempts to remove the warrior from the army of the noble if valid. If valid, the warrior is removed and the method returns true, otherwise returns false.
 */
bool Noble::removeWarrior(const Warrior& warr) {
    size_t idx = getIndexToRemove(warr); // the index of the warrior to rmve. army.size() if invalid to remove.
    if (idx == army.size()) return false; // remove is not valid if noble is dead or does not exist in the specified noble's army

    // pop the back of the army after shifting the rest to the left to maintain order
    for (size_t leftIdx=idx; leftIdx<army.size()-1; ++leftIdx)
        army[leftIdx] = army[leftIdx + 1]; // copy down elements
    army.pop_back(); // remove the back of the vector now that everything after the troop to remove is copied down
    return true;
}


