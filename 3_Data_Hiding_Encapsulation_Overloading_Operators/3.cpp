//
//  main.cpp
//  hw03: Data hiding, encapsulation, delegation, overloading the output operator
//
//  Created by Brayton Lordianto on 2/14/22.
//
/*
 This program performs a set of commands listed in a file, delimited by line. The commands are expected to be Warrior, Battle, and Status, with a fixed corresponding format for expected arguments of each command. The program stores the warriors in a vector of a warrior struct, edits their strengths, and displays corresponding messages according to the commands being read.
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

/*
 This class depicts a Warrior. A Warrior HAS A Weapon. A Warrior battles another warrior.
 */
class Warrior {
    friend ostream& operator<<(ostream& os, const Warrior& warr); // friend the warrior print operator
    // MARK: private Weapon class
    /*
     This class depicts a Weapon. A weapon always belongs to a warrior, so it is nested.
     */
    class Weapon {

        friend ostream& operator<<(ostream& os, const Weapon& wpn); // friend the weapon print operator
    public:
        // constructor
        Weapon(const string& name, int strength): name(name), strength(strength) {};
        
        // getters
        string getName() const { return name; }
        int getStrength() const { return strength; }
        
        // setters
        void setStrength(int newStrength) { strength = newStrength; }
        
    private:
        // Weapon fields
        string name;
        int strength;
    };
    // MARK: End of Weapon class
    friend ostream& operator<<(ostream& os, const Warrior::Weapon& wpn); // friend the weapon print operator, but in the Warrior class, so the Weapon class becomes accessible
public:
    // constructor - weapon name and strength is born with the weapon, thus all info about the weapon should be in the warrior constructor
    Warrior(const string& warrName, const string& wpnName, int wpnStr): name(warrName), weapon(wpnName, wpnStr) {};
    
    // getters
    string getName() const { return name; }
    Weapon getWeapon() const { return weapon; }
    
    // other behaviors
    /*
     Battle between two existing warriors, 'this' and the 'opponent'. Weapon strength changes happen during the battle. Appropriate messages are displayed.
     */
    void battleAgainst(Warrior& opponent) {
        cout << name << " battles " << opponent.name << endl;

        // check if any are dead. If so, display the appropriate message
        if (haveDead(opponent))
            return; // skip cmd

        // otherwise, both are alive, thus commence the battle.
        // the battle will recalculate strengths based on the stronger or weaker ones
        if (weapon.getStrength() > opponent.weapon.getStrength()) {
            setNewStrengths(weapon, opponent.weapon);
            cout << name << " defeats " << opponent.name << endl;
        } else if (weapon.getStrength() < opponent.weapon.getStrength()) {
            setNewStrengths(opponent.weapon, weapon);
            cout << name << " defeats " << opponent.name << endl;
        } else {
            setNewStrengths(opponent.weapon, weapon);
            cout << "Mutual Annihilation: " << name << " and " << opponent.name << "die at each other's hands" << endl;
        }
    }
    
private:
    // fields
    string name;
    Weapon weapon;
    
    /*
     private behavior that only exists for the battleAgainst function. A function that takes in two warriors in battle as parameters. handles the output in cases where one or more warriors passed in the battle parameter is dead. It will return a value that determines if the warriors are alive or dead, which will be used to determine if the battle will commence.
     */
    bool haveDead(const Warrior& opp) const {
        bool firstDead = weapon.getStrength() == 0;
        bool secondDead = opp.getWeapon().getStrength() == 0;
        
        if (!firstDead && !secondDead) // both are alive
            return false;

        // if either or both are dead, display the corresponding output
        if (firstDead && secondDead)
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
        else if (firstDead)
            cout << "He's dead, " << opp.name << endl;
        else if (secondDead)
            cout << "He's dead, " << name << endl;
        
        return true; // and return true because there are casualties
    }
    
    /*
     private behavior that only exists for the battleAgainst function. takes the references of the stronger and weaker strengths as parameters and recalculates them after the battle. The weaker or equal strength will be zero as the warrior would die, while the stronger or other equal strength will result in a strength reduced by the weaker or equal strength. Equal strengths would result in zero and the order of parameters will not matter.
     */
    void setNewStrengths(Weapon& strongerOrEq, Weapon& weakerOrEq) {
        strongerOrEq.setStrength(strongerOrEq.getStrength() - weakerOrEq.getStrength());
        weakerOrEq.setStrength(0);
    }
};

// MARK: function prototypes
void open(ifstream& f);
ostream& operator<<(ostream& os, const Warrior& warr);
ostream& operator<<(ostream& os, const Warrior::Weapon& wpn);
void battleCmd(ifstream& file, vector<Warrior>& vec);
bool setIndices(size_t& idx1, size_t& idx2, const string& name1, const string& name2, const vector<Warrior> vec);
void warriorCmd(ifstream& file, vector<Warrior>& vec);
void statusCmd(const vector<Warrior>& vec);

// MARK: main
int main() {
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
        else if (cmd == "Battle") {
            battleCmd(file, vec);
        }
        else if (cmd == "Status")
            statusCmd(vec);
        else
            cerr << "no such cmd" << endl;



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
 the print operator for printing a Warrior object. It's name and weapon information will be printed.
 */
ostream& operator<<(ostream& os, const Warrior& warr) {
    cout << "Warrior: " << warr.name << ", " << warr.weapon;
    return os;
}

/*
 the print operator for printing a Weapon object. It's name and strength will be printed.
 */
ostream& operator<<(ostream& os, const Warrior::Weapon& wpn) {
    cout << "weapon: " << wpn.name << ", " << wpn.strength;
    return os;
}

/*
Performs the warrior command. The arguments passed in to the warrior command is a warrior name and strength, which is passed from the filestream. This function stores the warrior into the vector if the name passed is unique.
*/
void warriorCmd(ifstream& file, vector<Warrior>& vec) {
    // get the name and strength of the warrior to add.
    string warrName;
    string wpnName;
    int strength;
    file >> warrName >> wpnName >> strength;
    
    // check if warrior already exists. if so, display an error.
    for (Warrior warr: vec)
        if (warr.getName() == warrName) {
            cerr << "Name Already Exists" << endl;
            return; // skip cmd
        }
    
    // else, make the new warrior
    vec.emplace_back(warrName, wpnName, strength);
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
    
    // warriors battle
    vec[idx1].battleAgainst(vec[idx2]);
}


/*
 A function that, if the warrior exists, edits sets the indexes of the warriors with the two name parameters of the battle command. If it does not exist, the appropriate message is displayed. The function returns a boolean that determines whether the indices are fonud or not. This will be used to determine whether the battle should commence or not.
 */
bool setIndices(size_t& idx1, size_t& idx2, const string& name1, const string& name2, const vector<Warrior> vec) {
    for (size_t i=0; i<vec.size(); ++i) { // concurrent search
        if (vec[i].getName() == name1)
            idx1 = i;
        else if (vec[i].getName() == name2)
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
 performs the status command. Accepts the vector of warriors as a parameter and displays the information of all the warriors in the vector.
 */
void statusCmd(const vector<Warrior>& vec) {
    cout << "There are: " << vec.size() << " warriors" << endl;
    for (const Warrior& warr: vec) {
        cout << warr << endl;
    }
}
