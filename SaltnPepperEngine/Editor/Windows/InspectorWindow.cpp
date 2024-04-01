#include "InspectorWindow.h"
//#include "Engine/Core/System/Application/Application.h"
//#include "Engine/Utils/Loading/FileSystem.h"
//
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/Components/SceneComponents.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Physics/Collision/Colliders/Collider.h"
#include "Engine/Core/Physics/Collision/Colliders/BoxCollider_Deprecated.h"
#include "Engine/Core/Physics/Collision/Colliders/SphereCollider_Deprecated.h"
#include "Engine/Core/Physics/Collision/Colliders/CapsuleCollider_Deprecated.h"
#include "Engine/Core/Physics/Collision/Colliders/MeshCollider_Deprecated.h"

#include "Engine/Core/Rendering/Geometry/Model.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Material/Material.h"


#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Geometry/Primitives.h"

#include "Editor/ImGuiUtils.h"
#include "Editor/Editor.h"

#include "Editor/MDIcons.h"
#include "Engine/Core/System/Application/Application.h"




namespace MM
{
    using namespace SaltnPepperEngine;
   
    

    // Editor Widget for Transform
    template <>
    void ComponentEditorWidget<Transform>(entt::registry& reg, entt::registry::entity_type e)
    {
        Transform& transform = reg.get<Transform>(e);

        Vector3 rotation = transform.GetEulerRotation();
        Vector3 position = transform.GetPosition();
        Vector3 scale = transform.GetScale();
       

        float itemWidth = (ImGui::GetContentRegionAvail().x - (ImGui::GetFontSize() * 3.0f)) / 3.0f;

        // Call this to fix alignment with columns
        ImGui::AlignTextToFramePadding();

        if (ImGuiUtils::PropertyTransform("Position", position, itemWidth))
        {
            transform.SetPosition(position);
           // LOG_INFO("X:{0} Y:{1} Z:{2}", newPos.x, newPos.y, newPos.z);
        }

        ImGui::SameLine();

        if (ImGuiUtils::PropertyTransform("Rotation", rotation, itemWidth))
        {
            float pitch = Min(rotation.x, 89.9f);
            pitch = Max(pitch, -89.9f);
            transform.SetRotation(Quaternion(Radians(Vector3(pitch, rotation.y, rotation.z))));
        }

        ImGui::SameLine();
        if (ImGuiUtils::PropertyTransform("Scale", scale, itemWidth))
        {
            transform.SetScale(scale);
        }

        ImGui::Columns(1);
        ImGui::Separator();
    }


    template <>
    void ComponentEditorWidget<Camera>(entt::registry& registry, entt::registry::entity_type entity)
    {

        Camera& camera = registry.get<Camera>(entity);

        ImGuiUtils::ScopedStyle(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        ImGui::Columns(2);
        ImGui::Separator();



        float aspect = camera.GetAspectRatio();
        if (ImGuiUtils::Property("Aspect", aspect, 0.0f, 10.0f))
            camera.SetAspectRatio(aspect);

        float fov = camera.GetFOV();
        if (ImGuiUtils::Property("Fov", fov, 1.0f, 120.0f))
            camera.SetFOV(fov);

        float n = camera.GetZNear();
        if (ImGuiUtils::Property("Near", n, 0.0f, 10.0f))
            camera.SetZNear(n);

        float f = camera.GetZFar();
        if (ImGuiUtils::Property("Far", f, 10.0f, 10000.0f))
            camera.SetZFar(f);



        bool ortho = camera.IsOrthographic();
        if (ImGuiUtils::Property("Orthograhic", ortho))
            camera.SetOrthographic(ortho);

        ImGui::Columns(1);
        ImGui::Separator();
    }


    template <>
    void ComponentEditorWidget<Light>(entt::registry& reg, entt::registry::entity_type e)
    {

        Light& light = reg.get<Light>(e);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        ImGui::Columns(2);
        ImGui::Separator();

        if (light.type != LightType::DirectionLight)
        {
            ImGuiUtils::Property("Position", light.position);
        }

        if (light.type != LightType::PointLight)
            ImGuiUtils::Property("Direction", light.direction);

        if (light.type != LightType::DirectionLight)
            ImGuiUtils::Property("Radius", light.radius, 0.0f, 100.0f);

        Vector4 color = Vector4(light.color, 1.0f);

        ImGuiUtils::Property("Color", color, true, ImGuiUtils::PropertyFlag::ColourProperty);

        light.color = Vector3(color);

        ImGuiUtils::Property("Intensity", light.intensity, 0.0f, 10.0f);

        if (light.type == LightType::SpotLight)
            ImGuiUtils::Property("Angle", light.innerAngle, -1.0f, 1.0f);

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Light Type");
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        const char* types[] = { "Directional", "Spot", "Point" };
        std::string light_current = Light::GetStringfromType(LightType(int(light.type)));
        if (ImGui::BeginCombo("", light_current.c_str(), 0)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < 3; n++)
            {
                bool is_selected = (light_current.c_str() == types[n]);
                if (ImGui::Selectable(types[n], light_current.c_str()))
                {
                    light.type = Light::GetTypeFromString(types[n]);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::PopStyleVar();
    }

    void TextureWidget(const char* label, Material* material, Texture* tex, bool flipImage, float& usingMapProperty, Vector4& colourProperty, const std::function<void(const std::string&)>& callback, const ImVec2& imageButtonSize = ImVec2(64, 64))
    {
        
        if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            // ImGui::Columns(2);
            ImGui::BeginColumns("TextureWidget", 2, ImGuiOldColumnFlags_NoResize);
            ImGui::SetColumnWidth(0, imageButtonSize.x + 10.0f);

            ImGui::Separator();

            ImGui::AlignTextToFramePadding();

            const ImGuiPayload* payload = ImGui::GetDragDropPayload();
            auto min = ImGui::GetCurrentWindow()->DC.CursorPos;
            auto max = min + imageButtonSize + ImGui::GetStyle().FramePadding;

            bool hoveringButton = ImGui::IsMouseHoveringRect(min, max, false);
            bool showTexture = !(hoveringButton && (payload != NULL && payload->IsDataType("AssetFile")));
            if (tex && showTexture)
            {
                //if (ImGui::ImageButton((const char*)(tex), (void*)tex->GetHandle(), imageButtonSize, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f)))
                if (ImGui::ImageButton((const char*)(tex), reinterpret_cast<ImTextureID>(tex->GetHandle()), imageButtonSize, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f)))
                {
                    Application::GetCurrent().GetEditor()->GetFileBrowser().Open();
                    Application::GetCurrent().GetEditor()->GetFileBrowser().SetCallback(callback);
                }

                if (ImGui::IsItemHovered() && tex)
                {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted(tex->GetFilePath().c_str());
                    ImGui::PopTextWrapPos();
                    ImGui::Image(reinterpret_cast<ImTextureID>(tex->GetHandle()), imageButtonSize * 3.0f, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
                    ImGui::EndTooltip();
                }
            }
            else
            {
                if (ImGui::Button(tex ? "" : "Empty", imageButtonSize))
                {
                    Application::GetCurrent().GetEditor()->GetFileBrowser().Open();
                    Application::GetCurrent().GetEditor()->GetFileBrowser().SetCallback(callback);
                }
            }

            //if (payload != NULL && payload->IsDataType("AssetFile"))
            //{
            //    auto filePath = std::string(reinterpret_cast<const char*>(payload->Data));
            //    if (RuntimeEditor::GetEditor()->IsTextureFile(filePath))
            //    {
            //        if (ImGui::BeginDragDropTarget())
            //        {
            //            // Drop directly on to node and append to the end of it's children list.
            //            if (ImGui::AcceptDragDropPayload("AssetFile"))
            //            {
            //                callback(filePath);
            //                ImGui::EndDragDropTarget();

            //                ImGui::Columns(1);
            //                ImGui::Separator();
            //                ImGui::PopStyleVar();

            //                ImGui::TreePop();
            //                return;
            //            }

            //            ImGui::EndDragDropTarget();
            //        }
            //    }
            //}

            ImGui::NextColumn();
            // ImGui::PushItemWidth(-1);

            if (tex)
            {
                ImGui::Text("%u x %u", tex->GetWidth(), tex->GetHeight());
            }

            // ImGui::TextUnformatted("Use Map");
            // ImGui::SameLine();
            // ImGui::PushItemWidth(-1);

            ImGui::SliderFloat(ImGuiUtils::GenerateLabelID("Use Map"), &usingMapProperty, 0.0f, 1.0f);

            ImGui::ColorEdit4(ImGuiUtils::GenerateLabelID("Colour"), glm::value_ptr(colourProperty));
            /*       ImGui::TextUnformatted("Value");
                   ImGui::SameLine();
                   ImGui::PushItemWidth(-1);*/

                   // ImGui::DragFloat(ImGuiUtils::GenerateID(), &amount, 0.0f, 20.0f);

                   // ImGui::PopItemWidth();
                   // ImGui::NextColumn();

                   // ImGuiUtilities::Property("Use Map", usingMapProperty, 0.0f, 1.0f);
                   // ImGuiUtilities::Property("Colour", colourProperty, 0.0f, 1.0f, false, ImGuiUtils::PropertyFlag::ColourProperty);

            ImGui::Columns(1);

            ImGui::Separator();
            ImGui::PopStyleVar();

            ImGui::TreePop();
        }
    }

    void TextureWidget(const char* label,Material* material, Texture* tex, bool flipImage, float& usingMapProperty, float& amount, bool hasAmountValue, const std::function<void(const std::string&)>& callback,const ImVec2& imageButtonSize = ImVec2(64, 64))
    {
       
        if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            ImGui::BeginColumns("TextureWidget", 2, ImGuiOldColumnFlags_NoResize);
            ImGui::SetColumnWidth(0, imageButtonSize.x + 10.0f);
            ImGui::Separator();

            ImGui::AlignTextToFramePadding();

            const ImGuiPayload* payload = ImGui::GetDragDropPayload();
            auto min = ImGui::GetCurrentWindow()->DC.CursorPos;
            auto max = min + imageButtonSize + ImGui::GetStyle().FramePadding;

            bool hoveringButton = ImGui::IsMouseHoveringRect(min, max, false);
            bool showTexture = !(hoveringButton && (payload != NULL && payload->IsDataType("AssetFile")));
            if (tex && showTexture)
            {
                if (ImGui::ImageButton((const char*)(tex), reinterpret_cast<ImTextureID>(tex->GetHandle()), imageButtonSize, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f)))
                {
                    Application::GetCurrent().GetEditor()->GetFileBrowser().Open();
                    Application::GetCurrent().GetEditor()->GetFileBrowser().SetCallback(callback);
                }

                if (ImGui::IsItemHovered() && tex)
                {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted(tex->GetFilePath().c_str());
                    ImGui::PopTextWrapPos();

                    ImGui::Image(reinterpret_cast<ImTextureID>(tex->GetHandle()), imageButtonSize * 3.0f, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
                    ImGui::EndTooltip();
                }
            }
            else
            {
                if (ImGui::Button(tex ? "" : "Empty", imageButtonSize))
                {
                    Application::GetCurrent().GetEditor()->GetFileBrowser().Open();
                    Application::GetCurrent().GetEditor()->GetFileBrowser().SetCallback(callback);
                }
            }

            //if (payload != NULL && payload->IsDataType("AssetFile"))
            //{
            //    auto filePath = std::string(reinterpret_cast<const char*>(payload->Data));
            //    if (RuntimeEditor::GetEditor()->IsTextureFile(filePath))
            //    {
            //        if (ImGui::BeginDragDropTarget())
            //        {
            //            // Drop directly on to node and append to the end of it's children list.
            //            if (ImGui::AcceptDragDropPayload("AssetFile"))
            //            {
            //                callback(filePath);
            //                ImGui::EndDragDropTarget();

            //                ImGui::Columns(1);
            //                ImGui::Separator();
            //                ImGui::PopStyleVar();

            //                ImGui::TreePop();
            //                return;
            //            }

            //            ImGui::EndDragDropTarget();
            //        }
            //    }
            //}

            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            if (tex)
            {
                ImGui::Text("%u x %u", tex->GetWidth(), tex->GetHeight());               
            }
            ImGui::PopItemWidth();
           
            ImGui::SliderFloat(ImGuiUtils::GenerateLabelID("Use Map"), &usingMapProperty, 0.0f, 1.0f);

            if (hasAmountValue)
            {
                float maxValue = 20.0f;
                if (std::strcmp(label, "Metallic") == 0 || std::strcmp(label, "Roughness") == 0)
                    maxValue = 1.0f;
                ImGui::SliderFloat(ImGuiUtils::GenerateLabelID("Value"), &amount, 0.0f, maxValue);
            }
           
            ImGui::NextColumn();

            ImGui::Columns(1);

            ImGui::Separator();
            ImGui::PopStyleVar();

            ImGui::TreePop();
        }
    }

    PrimitiveType GetPrimitive(const std::string& name)
    {

        //Plane = 1,
        //    Quad = 2,
        //    Sphere = 3,
        //    Cube = 4,
        //    Cylinder = 5,
        //    Capsule = 6,
        //    Terrain = 7,
        //    External = 8
        if (name == "Cube")
        {
            return PrimitiveType::Cube;
        }
        else if (name == "Plane")
        {
            return PrimitiveType::Plane;
        }
        else if (name == "Quad")
        {
            return PrimitiveType::Quad;
        }
        else if (name == "Sphere")
        {
            return PrimitiveType::Sphere;
        }
        else if (name == "Cylinder")
        {
            return PrimitiveType::Cylinder;
        }
        else if (name == "Capsule")
        {
            return PrimitiveType::Capsule;
        }
        else if (name == "Terrain")
        {
            return PrimitiveType::Terrain;
        }

        LOG_ERROR("Unsupported Primitive name");
        return PrimitiveType::Cube;
    }

    std::string GetPrimitiveName(const PrimitiveType type)
    {
        switch (type)
        {
        case SaltnPepperEngine::Rendering::PrimitiveType::None: return "None";
            break;
        case SaltnPepperEngine::Rendering::PrimitiveType::Plane: return "Plane";
            break;
        case SaltnPepperEngine::Rendering::PrimitiveType::Quad: return "Quad";
            break;
        case SaltnPepperEngine::Rendering::PrimitiveType::Sphere: return "Sphere";
            break;
        case SaltnPepperEngine::Rendering::PrimitiveType::Cube: return "Cube";
            break;
        case SaltnPepperEngine::Rendering::PrimitiveType::Cylinder: return "Cylinder";
            break;
        case SaltnPepperEngine::Rendering::PrimitiveType::Capsule: return "Capsule";
            break;
        case SaltnPepperEngine::Rendering::PrimitiveType::Terrain: return "Terrain";
            break;
        case SaltnPepperEngine::Rendering::PrimitiveType::External: return "External";
            break;
       
        }

        LOG_ERROR("Unsupported Primitive type");
        return "";
    }

    template <>
    void ComponentEditorWidget<ModelComponent>(entt::registry& reg, entt::registry::entity_type e)
    {
        
        Model& model = *reg.get<ModelComponent>(e).m_handle.get();

        PrimitiveType primitiveType = reg.get<ModelComponent>(e).m_handle ? model.GetPrimitiveType() : PrimitiveType::None;

        ImGuiUtils::PushID();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        ImGui::Columns(2);
        ImGui::Separator();

        ImGui::TextUnformatted("Primitive Type");

        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        const char* shapes[] = { "Sphere", "Cube", "External", "Quad", "None" };
       // const char* shapes[] = { "Sphere", "Cube", "Capsule", "Cylinder", "Terrain", "External", "Quad", "None" };
        std::string shape_current = GetPrimitiveName(primitiveType).c_str();
        if (ImGui::BeginCombo("", shape_current.c_str(), 0)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < 5; n++)
            {
                bool is_selected = (shape_current.c_str() == shapes[n]);
                if (ImGui::Selectable(shapes[n], shape_current.c_str()))
                {
                    if (reg.get<ModelComponent>(e).m_handle)
                        model.GetMeshes().clear();

                    if (strcmp(shapes[n], "External") != 0)
                    {
                        if (reg.get<ModelComponent>(e).m_handle)
                        {
                            model.GetMeshes().push_back(SharedPtr<Mesh>(GeneratePrimitive(GetPrimitive(shapes[n]))));
                            model.SetPrimitiveType(GetPrimitive(shapes[n]));
                        }
                        else
                        {
                            reg.get<ModelComponent>(e).LoadPrimitive(GetPrimitive(shapes[n]));
                        }
                    }
                    else
                    {
                        if (reg.get<ModelComponent>(e).m_handle)
                            model.SetPrimitiveType(PrimitiveType::External);
                    }
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        if (primitiveType == PrimitiveType::External)
        {
            ImGui::TextUnformatted("FilePath");

            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            ImGui::TextUnformatted(model.GetFilePath().c_str());
            ImGuiUtils::Tooltip(model.GetFilePath().c_str());

            ImGui::PopItemWidth();
            ImGui::NextColumn();
        }

        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::PopStyleVar();

        int matIndex = 0;

        SharedPtr<Model> modelRef = reg.get<ModelComponent>(e).m_handle;
        if (!modelRef)
        {
            ImGuiUtils::PopID();
            return;
        }

        ImGui::Separator();
        const auto& meshes = modelRef->GetMeshes();
        if (ImGui::TreeNode("Meshes"))
        {
            for (auto mesh : meshes)
            {
                if (!mesh->GetName().empty())
                    ImGui::TextUnformatted(mesh->GetName().c_str());
            }
            ImGui::TreePop();
        }

        ImGui::Separator();

       

        ImGui::Separator();
        if (ImGui::TreeNode("Materials"))
        {
            Material* MaterialShown[1000];
            uint32_t MaterialCount = 0;
            for (auto mesh : meshes)
            {
                auto material = mesh->GetMaterial();
                std::string matName = material ? material->GetName() : "";

                bool materialFound = false;
                for (uint32_t i = 0; i < MaterialCount; i++)
                {
                    if (MaterialShown[i] == material.get())
                        materialFound = true;
                }

                if (materialFound)
                    continue;

                MaterialShown[MaterialCount++] = material.get();

                if (matName.empty())
                {
                    matName = "Material";
                    matName += std::to_string(matIndex);
                }

                matName += "##" + std::to_string(matIndex);
                matIndex++;
                if (!material)
                {
                    ImGui::TextUnformatted("Empty Material");
                    if (ImGui::Button("Add Material", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
                        mesh->SetMaterial(MakeShared<Material>());
                }
                else if (ImGui::TreeNodeEx(matName.c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth))
                {
                   
                    //ImGui::Indent();
                    //if (ImGui::Button("Save to file"))
                    //{
                    //    std::string filePath = "//Meshes"; // Materials/" + matName + ".lmat";
                    //    std::string physicalPath;
                    //    if (ResolvePhysicalPath(filePath, physicalPath))
                    //    {
                    //        physicalPath += "/Materials/" + matName + ".lmat";
                    //        std::stringstream storage;

                    //        cereal::JSONOutputArchive output{ storage };
                    //        material->save(output);

                    //        FileSystem::WriteTextFile(physicalPath, storage.str());
                    //    }
                    //}
                    bool flipImage = true;

                    bool opaque = material->GetType() == MaterialType::Opaque;
                   

                    ImGui::Columns(2);
                    ImGui::Separator();

                    ImGui::AlignTextToFramePadding();

                    if (ImGuiUtils::Property("Opaque", opaque))
                    {
                        material->m_type = opaque ? MaterialType::Opaque : MaterialType::Transparent;
                    }

                   

                    ImGui::Columns(1);

                    Vector4 colour = Vector4();
                    float normal = 0.0f;
                    MaterialTextures& textures = material->textureMaps;

                    ImVec2 textureSize = ImVec2(100.0f, 100.0f);

                    TextureWidget("Albedo", material.get(), textures.albedoMap.get(), flipImage, material->albedoMapFactor, material->albedoColour, std::bind(&Material::SetAlbedoTexture, material, std::placeholders::_1), textureSize * 1.0f);
                    ImGui::Separator();

                    TextureWidget("Normal", material.get(), textures.normalMap.get(), flipImage, material->normalMapFactor, normal, false, std::bind(&Material::SetNormalTexture, material, std::placeholders::_1),  textureSize * 1.0f);
                    ImGui::Separator();

                    TextureWidget("Metallic", material.get(), textures.metallicMap.get(), flipImage, material->metallicMapFactor, material->metallic, true, std::bind(&Material::SetMetallicTexture, material, std::placeholders::_1),textureSize * 1.0f);
                    ImGui::Separator();

                    TextureWidget("Roughness", material.get(), textures.roughnessMap.get(), flipImage, material->roughnessMapFactor, material->roughness, true, std::bind(&Material::SetRoughnessTexture, material, std::placeholders::_1), textureSize * 1.0f);

                    if (ImGui::TreeNodeEx("Reflectance", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth))
                    {
                        ImGui::SliderFloat("##Reflectance", &material->reflectance, 0.0f, 1.0f);
                        ImGui::TreePop();
                    }



                    TextureWidget("AO",material.get(), textures.aoMap.get(), flipImage, material->aoMapFactor, normal, false, std::bind(&Material::SetAOTexture, material, std::placeholders::_1), textureSize * 1.0f);
                    ImGui::Separator();

                    TextureWidget("Emissive", material.get(), textures.emissiveMap.get(), flipImage, material->emissiveMapFactor, material->emissive, true, std::bind(&Material::SetEmissiveTexture, material, std::placeholders::_1),textureSize * 1.0f);

                    ImGui::Columns(2);

                   /* ImGui::AlignTextToFramePadding();
                    ImGui::TextUnformatted("WorkFlow");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);

                    int workFlow = (int)material->GetProperties()->workflow;

                    if (ImGui::DragInt("##WorkFlow", &workFlow, 0.3f, 0, 2))
                    {
                        material->GetProperties()->workflow = (float)workFlow;
                    }*/

                  /*  ImGui::PopItemWidth();
                    ImGui::NextColumn();

                    material->SetMaterialProperites(*prop);*/
                    ImGui::Columns(1);
                    ImGui::Unindent();
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        ImGuiUtils::PopID();
    }

    using namespace Physics;

    const char* CollisionShapeTypeToString(ColliderType type)
    {
        switch (type)
        {
 
        case ColliderType::BOX: return "Box";
            break;
        case ColliderType::SPHERE: return "Sphere";
            break;
        case ColliderType::CAPSULE: return "Capsule";
            break;
        case ColliderType::MESH: return "Mesh (Broken)";
            break;

        default:
            break;
        }
      

        return "Error";
    }

    ColliderType StringToCollisionShapeType(const std::string& type)
    {
        if (type == "Box")
        {
            return ColliderType::BOX;
        }
        if (type == "Sphere")
        {
            return ColliderType::SPHERE;
        }
        if (type == "Capsule")
        {
            return ColliderType::CAPSULE;
        }
        if (type == "Mesh (Broken)")
        {
            return ColliderType::MESH;
        }
        LOG_ERROR("Unsupported Collision shape {0}", type);
        return ColliderType::SPHERE;
    }

    //static void BoxColliderInspector(BoxCollider_Deprecated* collider, RigidBodyComponent& phys)
    //{
    //    ImGui::AlignTextToFramePadding();
    //    ImGui::TextUnformatted("Half Dimensions");
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);

    //    Vector3 size = collider->GetHalfDimensions();
    //    if (ImGui::DragFloat3("##CollisionShapeHalfDims", glm::value_ptr(size), 1.0f, 0.0f, 10000.0f, "%.2f"))
    //    {
    //        collider->SetHalfDimensions(size);
    //       // phys.GetRigidBody()->ColliderUpdated();
    //    }
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);
    //}

    //static void SphereColliderInspector(SphereCollider_Deprecated* collider, RigidBodyComponent& phys)
    //{
    //   
    //    ImGui::AlignTextToFramePadding();
    //    ImGui::TextUnformatted("Radius");
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);

    //    float radius = collider->GetRadius();
    //    if (ImGui::DragFloat("##CollisionShapeRadius", &radius, 1.0f, 0.0f, 10000.0f))
    //    {
    //        collider->SetRadius(radius);
    //      //  phys.GetRigidBody()->ColliderUpdated();
    //    }
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);
    //}

   
    //static void CapsuleColliderInspector(CapsuleCollider_Deprecated* collider, RigidBodyComponent& phys)
    //{
    //    ImGui::AlignTextToFramePadding();
    //    ImGui::TextUnformatted("Half Dimensions");
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);

    //    float radius = collider->GetRadius();
    //    if (ImGui::DragFloat("##CollisionShapeRadius", &radius, 1.0f, 0.0f, 10000.0f, "%.2f"))
    //    {
    //        collider->SetRadius(radius);
    //      //  phys.GetRigidBody()->ColliderUpdated();
    //    }

    //    float height = collider->GetHeight();
    //    if (ImGui::DragFloat("##CollisionShapeHeight", &height, 1.0f, 0.0f, 10000.0f, "%.2f"))
    //    {
    //        collider->SetHeight(height);
    //      //  phys.GetRigidBody()->ColliderUpdated();
    //    }
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);
    //}

    //static void MeshColliderInspector(MeshCollider_Deprecated* collider, RigidBodyComponent& phys)
    //{  
    //    // Work on this Later
    //    ImGui::TextUnformatted("Hull Collision Shape");
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);
    //}

    //template <>
    //void ComponentEditorWidget<RigidBodyComponent>(entt::registry& reg, entt::registry::entity_type e)
    //{
    //   
    //    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    //    ImGui::Columns(2);
    //    ImGui::Separator();
    //    RigidBodyComponent& phys = reg.get<RigidBodyComponent>(e);

    //    //Vector3 pos = phys.GetRigidBody()->GetPosition();
    //    //Vector3 force = phys.GetRigidBody()->GetForce();
    //    //Vector3 rotation = glm::eulerAngles(phys.GetRigidBody()->GetRotation());
    //    //
    //    //float friction = phys.GetRigidBody()->GetFriction();
    //    //bool isStatic = phys.GetRigidBody()->GetIsStatic();
    //    //bool isStationary = phys.GetRigidBody()->GetIsStationary();
    //    //float mass = 1.0f / phys.GetRigidBody()->GetInverseMass();
    //    //Vector3 velocity = phys.GetRigidBody()->GetVelocity();
    //    //float elasticity = phys.GetRigidBody()->GetElasticity();
    //    //SharedPtr<Collider> collisionShape = phys.GetRigidBody()->GetCollider();
    //    //auto UUID = phys.GetRigidBody()->GetUniqueId();

    //    //ImGuiUtils::Property("UniqueId", (uint32_t&)UUID, ImGuiUtils::PropertyFlag::ReadOnly);

    //    //if (ImGuiUtils::Property("Position", pos))
    //    //    phys.GetRigidBody()->SetPosition(pos);

    //    //if (ImGuiUtils::Property("Velocity", velocity))
    //    //    phys.GetRigidBody()->SetVelocity(velocity);

    //   
    //    //if (ImGuiUtils::Property("Rotation", rotation))
    //    //    phys.GetRigidBody()->SetRotation(Quaternion(rotation));

    //    //if (ImGuiUtils::Property("Force", force))
    //    //    phys.GetRigidBody()->SetForce(force);

    //    //if (ImGuiUtils::Property("Friction", friction, 0.0f, 1.0f))
    //    //    phys.GetRigidBody()->SetFriction(friction);

    //    //if (ImGuiUtils::Property("Mass", mass))
    //    //{
    //    //    mass = Max(mass, 0.0001f);
    //    //    phys.GetRigidBody()->SetInverseMass(1.0f / mass);
    //    //}

    //    //if (ImGuiUtils::Property("Elasticity", elasticity))
    //    //    phys.GetRigidBody()->SetElasticity(elasticity);

    //    //if (ImGuiUtils::Property("Static", isStatic))
    //    //    phys.GetRigidBody()->SetStatic(isStatic);

    //    //if (ImGuiUtils::Property("Stationary", isStationary))
    //    //    phys.GetRigidBody()->SetIsStationary(isStationary);

    //    ImGui::Columns(1);
    //    ImGui::Separator();
    //    ImGui::PopStyleVar();

    //   /* const char* shapes[4] = { "Box", "Sphere", "Capsule", "Mesh (Broken)" };
    //    int selectedIndex = 0;
    //    const char* shape_current = collisionShape ? CollisionShapeTypeToString(collisionShape->GetType()) : "";
    //    int index = 0;
    //    for (auto& shape : shapes)
    //    {
    //        if (shape == shape_current)
    //        {
    //            selectedIndex = index;
    //            break;
    //        }
    //        index++;
    //    }

    //    bool updated = ImGuiUtils::PropertyDropdown("Collision Shape", shapes, 4, &selectedIndex);*/

    //   // if (updated)
    //       // phys.GetRigidBody()->SetCollider(StringToCollisionShapeType(shapes[selectedIndex])); 

    ///*    if (collisionShape)
    //    {
    //        switch (collisionShape->GetType())
    //        {
    //        case ColliderType::BOX:
    //            BoxColliderInspector(reinterpret_cast<BoxCollider*>(collisionShape.get()), phys);
    //            break;
    //        case ColliderType::SPHERE:
    //            SphereColliderInspector(reinterpret_cast<SphereCollider*>(collisionShape.get()), phys);
    //            break;
    //        case ColliderType::CAPSULE:
    //            CapsuleColliderInspector(reinterpret_cast<CapsuleCollider*>(collisionShape.get()), phys);
    //            break;
    //        case ColliderType::MESH:
    //            MeshColliderInspector(reinterpret_cast<MeshCollider*>(collisionShape.get()), phys);
    //            break;
    //        default:
    //            ImGui::NextColumn();
    //            ImGui::PushItemWidth(-1);
    //            LOG_ERROR("Unsupported Collision shape");
    //            break;
    //        }
    //    }
    //    else
    //    {
    //        ImGui::NextColumn();
    //        ImGui::PushItemWidth(-1);
    //    }*/

    //    ImGui::PopItemWidth();
    //    ImGui::Columns(1);

    //    ImGui::Separator();
    //}


}


namespace SaltnPepperEngine
{
    using namespace Physics;
	
	namespace Editor
	{



        static bool init = false;

		InspectorWindow::InspectorWindow()
		{
            m_name = ICON_MDI_INFORMATION " Inspector###inspector";
		}

		void InspectorWindow::OnImgui()
		{
            std::vector<entt::entity> selectedEntities = m_editorHandle->GetSelectedEntities();
		
            if (ImGui::Begin("Inspector", &m_active))
            {
                ImGuiUtils::PushID();

                Scene* currentScene = Application::GetCurrent().GetCurrentScene();

                if (!currentScene)
                {
                    m_editorHandle->SelectEntity(entt::null);
                    ImGuiUtils::PopID();
                    ImGui::End();
                    return;
                }

                entt::registry& registry = currentScene->GetRegistry();

                if (selectedEntities.size() != 1 || !registry.valid(selectedEntities.front()))
                {
                    m_editorHandle->SelectEntity(entt::null);
                    ImGuiUtils::PopID();
                    ImGui::End();
                    return;
                }

                entt::entity selected = selectedEntities.front();
                Entity SelectedEntity = { selected, currentScene };


                ActiveComponent* activeComponent = registry.try_get<ActiveComponent>(selected);
                bool active = activeComponent ? activeComponent->active : true;

                if (ImGui::Checkbox("##ActiveCheckbox", &active))
                {
                    if (!activeComponent)
                    {
                        registry.emplace<ActiveComponent>(selected, active);
                    }
                    else
                    {
                        activeComponent->active = active;
                    }
                }

                ImGui::SameLine();
                ImGui::TextUnformatted(ICON_MDI_CUBE);
                ImGui::SameLine();


                bool hasName = registry.all_of<NameComponent>(selected);
                std::string name;

                if (hasName)
                {
                    name = registry.get<NameComponent>(selected).name;
                }
                else
                {
                    name = std::to_string(entt::to_integral(selected));
                }

                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetFontSize() * 4.0f);
                {
                    ImGuiUtils::ScopedFont boldFont(ImGui::GetIO().Fonts->Fonts[1]);
                    if (ImGuiUtils::InputText(name, "##InspectorNameChange"))
                    {
                        registry.get_or_emplace<NameComponent>(selected).name = name;
                    }
                }

                //ImGui::SameLine();
                ImGui::Separator();
                //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.0f));

                ImGui::BeginChild("Components", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_None);
                m_EntityEditor.RenderImGui(registry, selected);
                ImGui::EndChild();
                ImGuiUtils::PopID();
            }
        
            ImGui::End();
        
        }

		void InspectorWindow::OnInit()
		{
            if (init)
            {
                return;
            }

            init = true;

            entt::registry& registry = Application::GetCurrent().GetCurrentScene()->GetRegistry();
            std::unordered_map<size_t, const char*>& iconMap = m_editorHandle->GetComponentIconMap();
		    

#define ICON_COMPONENT(ComponentType, ComponentName)                         \
    {                                                                        \
        std::string Name;                                                    \
        if(iconMap.find(typeid(ComponentType).hash_code()) != iconMap.end()) \
            Name += iconMap[typeid(ComponentType).hash_code()];              \
        else                                                                 \
            Name += iconMap[typeid(RuntimeEditor).hash_code()];                     \
        Name += "\t";                                                        \
        Name += (ComponentName);                                             \
        m_EntityEditor.registerComponent<ComponentType>(Name.c_str());       \
    }


            ICON_COMPONENT(Transform, "Transform");
            ICON_COMPONENT(ModelComponent, "ModelComponent");
            ICON_COMPONENT(Camera, "Camera");
           // ICON_COMPONENT(RigidBodyComponent, "RigidBody");
            ICON_COMPONENT(Light, "Light");   
        }

		void InspectorWindow::OnUpdate()
		{

		}

		void InspectorWindow::SetDebugMode(bool mode)
		{

		}

		bool InspectorWindow::GetIsDebug() const
		{
			return false;
		}
	}
}
