# AI Navigation Project (AI behavior)

This projects deals with the Differnt AI bagent behaviours


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

1) Goblin : [Seek] The goblin will continously move towards the player's position
2) Sheep  : [Flee] The Sheep will move away from the player when player is in range
3) Spider : [Pursue] The Spider will move towards where the player is heading, and will try to corner the player [try to get in fron tof the player ]
4) Deer   : [Evade] The Deer will flee from the player's curent heading direction [when in range]
4) Cat    : [Approach] The Cat will try to go near the player until a set distance then will stop. if the player closes the distance, the cat will move away 

The Spawning can be handing using the  [CreateEnemyAI()] function in the [GraphicsApplication/Source/GraphicRuntime.cpp]

The [CreateEnemyAI()] takes 3 Parameters :
1) AI::BehaviorState : this defines the behaviour of the Created Enemy
2) EnemyModel : this defines what model is going to beapplied for the enemy
3) SpawnPosition : this defines where the AI agent will spawn in the world# SaltnPepper
 Refactored Engine from the First Semester of GDP-1 in Fanshawe
