# Game Integration Project (Sprint 1)

This projects deals with the basic setup for the game "Dead Energy"


# Building Notes : 

Step 1 : Compile the [SaltnPepperEngine] and the [GraphicsApplication] project
Step 2 : Then setup the [GraphicApplication] project as startup project
Step 3 : Build + run the project

If the Project fails to run, its probably cuz the fmod and assimp dll file are missing from the build output folder [x64/Release] or [x64/Debug]
You can find these files in the [Dlls] folder at the root of the solution directory, just in case you want to copy them manually.

# Player Controls

[W] [A] [S] [D] to move Player

[MousePan] to look around
[Space] to jump

# Editor Controls
Press [`] Key to toggle the Editor Mode 
 
In the editor mode you can use gizmos to move characters around

Selecting the Entity from the inspector and pressing [F] will focus the editor camera on that entity

# General Notes: 

The basic physics System has been implemented along with Barebones platforming logic