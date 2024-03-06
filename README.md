# AI Navigation Project TWO (Wander behavior)

This projects deals with the Different AI agent Wander behaviours


# Building Notes : 

Step 1 : Compile the [SaltnPepperEngine] and the [GraphicsApplication] project
Step 2 : Then setup the [GraphicApplication] project as startup project
Step 3 : Build + run the project

If the Project fails to run, its probably cuz the fmod and assimp dll file are missing from the build output folder [x64/Release] or [x64/Debug]
You can find these files in the [Dlls] folder at the root of the solution directory, just in case you want to copy them manually.

# Player Controls

[W] [A] [S] [D] to move Player

[RightClick] + [MousePan] to look around


# Editor Controls
Press [`] Key to toggle the Editor Mode 
 
In the editor mode you can use gizmos to move characters around

Selecting the Entity from the inspector and pressing [F] will focus the editor camera on that entity

# General Notes: 

There are 5 AI agents each displaying a differnt behaviour

1) Goblin : [Wander_Short] The goblin will Has a short wander waypoint range but also a short decision time

2) Sheep  : [Wander_Medium] The Sheep has a short primary radius and a lon secondary radius with a slightly larger decision time

3) Cat    : [Wander_Long] The cat has the longest radius for both the primary and secondary waypint and the longest decision time

The Spawning can be handing using the  [CreateWanderAI()] function in the [GraphicsApplication/Source/GraphicRuntime.cpp]

The [CreateWanderAI()] takes 3 Parameters :
1) WanderParam : this defines the primary and secondary search radius and the decision time
2) EnemyModel : this defines what model is going to beapplied for the enemy
3) SpawnPosition : this defines where the AI agent will spawn in the world


