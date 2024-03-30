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
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Textures/Image.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Lights/Light.h"

//=============== CAMERA STUFF ===============
#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Rendering/Camera/CameraController.h"
#include "Engine/Core/Rendering/Camera/FlyCameraController.h"
#include "Engine/Core/Rendering/Camera/EditorCameraController.h"

// ========= SCENE and ECS STUFF ===================

#include "Engine/Core/Components/SceneComponents.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Scene/Scene.h" 
#include "Engine/Core/Scene/SceneGraph.h"
#include "Engine/Core/EntitySystem/Entity.h"
#include "Engine/Core/EntitySystem/EntityManager.h"



// ============= PHYSICS SYSTEM ==============
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingSphere.h"
#include "Engine/Core/Physics/Collision/BroadPhase/OctreeBroadPhase.h"
#include "Engine/Core/Physics/Collision/Colliders/SphereCollider_Deprecated.h"
#include "Engine/Core/Physics/Collision/Colliders/BoxCollider_Deprecated.h"
#include "Engine/Core/Physics/Collision/Colliders/CapsuleCollider_Deprecated.h"

#include "Engine/Core/Physics/PhysicsEngine/PhysicsEngine.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"


// ============ Bullet Physics System ==================
#include "Engine/Core/Physics/PhysicsSystem/Colliders/BaseCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/BoxCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/SphereCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/CapsuleCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/CylinderCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsAPI.h"
#include "Engine/Core/Physics/PhysicsSystem/RigidBody/RigidBody.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsSystem.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsUtils.h"




using namespace SaltnPepperEngine;

using namespace SaltnPepperEngine::Physics;