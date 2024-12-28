# Cyclic Association and Separate Compilation

## Focus
- Cyclic Association
- Operator overloading
- Separate compilation
- Namespace

## Problem
We are revisiting the world of Nobles and Warriors (hoping you aren't sick of it...), now adding one new feature: not only can warriors be fired, they can also run away! Of course, when a warrior runs away, he has to inform his noble. This means the warrior has to know who hired him and be able to communicate with him, which means he will need a pointer to his noble. Good thing we learned about forward class declarations!

Of course, your Noble and Warrior classes should have all the same functionality that they had in the prior exercises, e.g., nobles can fire warriors.

Take this opportunity to clean up any "sloppy" code from before. For example, your battle method should not have redundant code. Make good use of small methods.

You will provide two solutions:
1. The first is a single file, `hw06-single.cpp`, that solves the problem of the cyclic association and leaves no method or function definitions inside the class definition, just the prototypes and the field definitions.
2. The second splits the first into separate files for separate compilation.

You may not have covered how to do this yet. You will in plenty of time to finish the assignment. Meanwhile, get to work on the first part above! None of the code should change between the first and second implementations. There should be separate header and implementation files for each class. For the separate compilation solution, place the code in the `WarriorCraft` namespace.
