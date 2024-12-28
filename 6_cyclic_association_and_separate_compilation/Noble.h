//
//  Noble.hpp
//  hw06 separate compilation files
//
//  Created by Brayton Lordianto on 3/19/22.
//

#ifndef Noble_hpp
#define Noble_hpp

#include <iostream>
#include <string>
#include <vector>

namespace WarriorCraft {

class Warrior;

/*
 This class models a noble with a name and an army of Warriors.
 */
class Noble {
    friend std::ostream& operator<<(std::ostream& os, const Noble& noble);
    
public:
    Noble(const std::string& name);
    const std::string& getName() const;
    void battle(Noble& opponent);
        
private:
    std::string name;
    std::vector<Warrior*> army;
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

}

#endif /* Noble_hpp */
