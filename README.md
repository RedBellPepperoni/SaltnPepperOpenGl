# Animation Project (Easing Functions)

This projects i sthe submission for the Easing Fucntion Assignemt


# Building Notes : 

Step 1 : Compile the [SaltnPepperEngine] and the [GraphicsApplication] project
Step 2 : Then setup the [GraphicApplication] project as startup project
Step 3 : Build + run the project

If the Project fails to run, its probably cuz the fmod and assimp dll file are missing from the build output folder [x64/Release] or [x64/Debug]
You can find these files in the [Dlls] folder at the root of the solution directory, just in case you want to copy them manually.

# Controls

[W] [A] [S] [D] to move the camera around

[RightClick] + [MousePan] to look around

[Space] to play Pause the Animations

[1] [2] [3] [4] [5] to change the animation speed to (1x, 2x ,3x ,4x and 5x)

[`] Key to launch the Editor incase needed

# General Notes: 

The Easing fucntions for all 3 Keyframe types are shown using Cubes
There are 12 cubes in total displaying each type of Easing (Linear, EaseIn, EaseOut, EaseInOut) for Position, Rotation and Scale Keyframe

The Lego Spaceman shows Animation events by changing Meshes to a lego skeleton and back 

All Events are done using [std::fucntion] and [std::bind]

All the Keyframes are sorted by time whenever a new Keyframe is added

Color Guide:

[Linear] easing    : [White] 
[EaseIn] easing    : [Red]
[EaseOut] easing   : [Yellow]
[EaseInOut] easing : [Green]