//
//  main.cpp
//  hw04
//
//  Created by Brayton Lordianto on 2/22/22.
/*
 This program models a game of medieval times. Nobles and Warriors exist in this game. Nobles can hire and fire warriors, and can battle other nobles. Warriors have strength and their strength will be changed after engaging in battle.
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

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

class Noble {
    friend ostream& operator<<(ostream& os, const Noble& noble);
    
public:
    // no need to initialize vector - as the proffessor stated. Vector default constructor is what is wanted.
    Noble(const string& name): name(name), isAlive(true) { }
    
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

int main() {
    Noble art("King Arthur");
    Noble lance("Lancelot du Lac");
    Noble jim("Jim");
    Noble linus("Linus Torvalds");
    Noble billie("Bill Gates");
        
    Warrior cheetah("Tarzan", 10);
    Warrior wizard("Merlin", 15);
    Warrior theGovernator("Conan", 12);
    Warrior nimoy("Spock", 15);
    Warrior lawless("Xena", 20);
    Warrior mrGreen("Hulk", 8);
    Warrior dylan("Hercules", 3);
    
    jim.hire(nimoy);
    lance.hire(theGovernator);
    art.hire(wizard);
    lance.hire(dylan);
    linus.hire(lawless);
    billie.hire(mrGreen);
    art.hire(cheetah);
    
    cout << "==========\n"
         << "Status before all battles, etc.\n";
    cout << jim << endl;
    cout << lance << endl;
    cout << art << endl;
    cout << linus << endl;
    cout << billie << endl;
    cout << "==========\n";
        
    art.fire(cheetah);
    cout << art << endl;
    
    art.battle(lance);
    jim.battle(lance);
    linus.battle(billie);
    billie.battle(lance);
    
    cout << "==========\n"
         << "Status after all battles, etc.\n";
    cout << jim << endl;
    cout << lance << endl;
    cout << art << endl;
    cout << linus << endl;
    cout << billie << endl;
    cout << "==========\n";
}

/*
 print operator for a warrior. the operator function takes an ostream reference and the const warrior reference to print.
 */
ostream& operator<<(ostream& os, const Warrior& warr) {
    os << "\t" << warr.name << ": " << warr.strength;
    return os;
}

/*
 print operator for a noble. the operator function takes an ostream reference and the const noble reference to print.
 */
ostream& operator<<(ostream& os, const Noble& noble) {
    os << noble.name << " has an army of " << noble.army.size();
    for (size_t i=0; i<noble.army.size(); ++i)
        os << endl << *noble.army[i];
    return os;
}
