//
//  Warrior.cpp
//  hw06 separate compilation files
//
//  Created by Brayton Lordianto on 3/19/22.
//

#include "Warrior.h"
#include "Noble.h"

using namespace std;

namespace WarriorCraft {

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

}
