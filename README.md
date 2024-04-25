# Final game Jam (Term - 2  :Jan - April)

This Project deals with the Final game Jam


# Building Notes : 

Step 1 : Compile the [SaltnPepperEngine] and the [GraphicsApplication] project
Step 2 : Then setup the [GraphicApplication] project as startup project
Step 3 : Build + run the project

If the Project fails to run, its probably cuz the fmod and assimp dll file are missing from the build output folder [x64/Release] or [x64/Debug]
You can find these files in the [Dlls] folder at the root of the solution directory, just in case you want to copy them manually.

# Player Controls

[W] [A] [S] [D] to move Player

[MousePan] to look around

Use [LeftClick] to shoot zombies.
USe [R] to reload the gun
Point to the ground and press [F] to move Buddy-Bot to that location
Point to a zombie and press [F] to make buddy attack the zombie

Use [F6] to reset the Game



# Editor Controls
Press [`] Key to toggle the Editor Mode 
 
In the editor mode you can use gizmos to move stuff around

Selecting the Entity from the inspector and pressing [F] will focus the editor camera on that entity

# General Notes: 

The main focus of this gamejam is the implementation of navigational mesh.

The Navmesh class takes a mesh (eg: .fbx file) and creates a Graph of waypoint nodes 

Another feature is multiple floor in the navigation, the pathfinder differntaites between multiple heights


It also uses Douglus Pecker algorith to simplify the found path