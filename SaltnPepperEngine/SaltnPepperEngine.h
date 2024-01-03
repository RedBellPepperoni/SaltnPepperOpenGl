// SaltnPepperEngine.cpp : Defines the functions for the static library.
//


// ========= APPLICATION AND WINDOW =============
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/System/Window/Window.h"

// ========== Math Definitions and Other Utils ===
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Loading/FileSystem.h"
#include "Engine/Utils/Loading/ImageLoader.h"
#include "Engine/Utils/Maths/Random.h"

// ========== Memory Definitions ============
#include "Engine/Core/Memory/MemoryDefinitions.h"

// ========== MACROS =======================
#include "Engine/Macros.h"

#include "Engine/Core/Resources/ResourceManager.h"

// ============ INPUT SYSTEM ==============
#include "Engine/Core/System/Input/InputSystem.h"

// ============= RENDERING GEOMETRY ==============
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Geometry/Model.h"

// ============= RENDERING ESSENTAILS ================
#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Textures/Image.h"
#include "Engine/Core/Rendering/Textures/Texture.h"

// ========= SCENE and ECS STUFF ===================
#include "Engine/Core/Scene/Scene.h" 
#include "Engine/Core/Scene/SceneGraph.h"
#include "Engine/Core/EntitySystem/Entity.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/Components/SceneComponents.h"
#include "Engine/Core/Components/Transform.h"

#include "Engine/Core/Rendering/Lights/Light.h"


using namespace SaltnPepperEngine;