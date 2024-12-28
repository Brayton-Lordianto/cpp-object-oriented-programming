//
//  Warrior.hpp
//  hw06 separate compilation files
//
//  Created by Brayton Lordianto on 3/19/22.
//

#ifndef Warrior_hpp
#define Warrior_hpp

#include <string>
#include <iostream>

namespace WarriorCraft {

class Noble;

/*
 This class models a warrior with a strength and name.
 */
class Warrior {
    friend std::ostream& operator<<(std::ostream& os, const Warrior& warr);

public:
    const std::string& getName() const;
    double getStrength() const;
    void setNewStrengthWithRatio(double ratio);
    
private:
    std::string name;
    double strength;

    //MARK: members added/changed for hw06
public:
    Warrior(const std::string& name, double strength); // init list changed.
    bool isHired() const;
    bool runaway();
    void makeUnemployed();
    void setEmployer(Noble* noblePtr);
    bool employerIs(Noble* noblePtr) const;

private:
    Noble* employer;
};

}

#endif /* Warrior_hpp */
