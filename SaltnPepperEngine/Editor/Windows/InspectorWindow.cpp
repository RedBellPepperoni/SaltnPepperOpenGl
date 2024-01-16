#include "InspectorWindow.h"
//#include "Engine/Core/System/Application/Application.h"
//#include "Engine/Utils/Loading/FileSystem.h"
//
//#include "Engine/Core/Scene/Scene.h"
//#include "Engine/Core/EntitySystem/EntityManager.h"
//#include "Engine/Core/Components/SceneComponents.h"
//#include "Engine/Core/Components/Transform.h"
//#include "Engine/Core/Rendering"
// 

#include "Editor/ImGuiUtils.h"
#include "Editor/Editor.h"

// Change this later to the includes that are really needed
#include "SaltnPepperEngine.h"

namespace MM
{ 
    using namespace SaltnPepperEngine;

    using namespace Rendering;

    // Editor Widget for Transform
    template <>
    void ComponentEditorWidget<Components::Transform>(entt::registry& reg, entt::registry::entity_type e)
    {
        Components::Transform& transform = reg.get<Components::Transform>(e);

        Vector3 rotation = transform.GetEulerRotation();




        Vector3 position = transform.GetPosition();
        Vector3 scale = transform.GetScale();

        float itemWidth = (ImGui::GetContentRegionAvail().x - (ImGui::GetFontSize() * 3.0f)) / 3.0f;

        // Call this to fix alignment with columns
        ImGui::AlignTextToFramePadding();

        if (ImGuiUtils::PropertyTransform("Position", position, itemWidth))
            transform.SetPosition(position);

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
    void ComponentEditorWidget<Camera>(entt::registry& reg, entt::registry::entity_type e)
    {
        
        Camera& camera = reg.get<Camera>(e);

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

   
    //template <>
    //void ComponentEditorWidget<TextComponent>(entt::registry& reg, entt::registry::entity_type e)
    //{
    //    using namespace Lumos;
    //    
    //    auto& text = reg.get<TextComponent>(e);
    //    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    //    ImGui::Columns(2);
    //    ImGui::Separator();

    //    ImGuiUtilities::PropertyMultiline("Text String", text.TextString);
    //    if (text.FontHandle)
    //    {
    //        ImGuiUtilities::PropertyConst("FilePath", text.FontHandle->GetFilePath().c_str());
    //    }

    //    ImGuiUtilities::Property("Colour", text.Colour, true, ImGuiUtilities::PropertyFlag::ColourProperty);
    //    ImGuiUtilities::Property("Outline Colour", text.OutlineColour, true, ImGuiUtilities::PropertyFlag::ColourProperty);
    //    ImGuiUtilities::Property("Outline Width", text.OutlineWidth);

    //    ImGuiUtilities::Property("Line Spacing", text.LineSpacing);
    //    ImGuiUtilities::Property("Max Width", text.MaxWidth);

    //    ImGui::Columns(1);

    //    auto callback = std::bind(&TextComponent::LoadFont, &text, std::placeholders::_1);

    //    ImGui::Separator();

    //    if (ImGui::Button("Load Font File"))
    //    {
    //        Editor::GetEditor()->GetFileBrowserPanel().Open();
    //        Editor::GetEditor()->GetFileBrowserPanel().SetCallback(callback);
    //    }

    //    ImGui::Separator();

    //    ImGui::Columns(2);

    //    if (ImGui::TreeNode("Texture"))
    //    {
    //        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    //        ImGui::Columns(2);
    //        ImGui::Separator();

    //        bool flipImage = Graphics::Renderer::GetGraphicsContext()->FlipImGUITexture();

    //        ImGui::AlignTextToFramePadding();
    //        auto tex = text.FontHandle ? text.FontHandle->GetFontAtlas() : Font::GetDefaultFont()->GetFontAtlas();

    //        auto imageButtonSize = ImVec2(64, 64) * Application::Get().GetWindowDPI();
    //        const ImGuiPayload* payload = ImGui::GetDragDropPayload();
    //        auto min = ImGui::GetCurrentWindow()->DC.CursorPos;
    //        auto max = min + imageButtonSize + ImGui::GetStyle().FramePadding;

    //        bool hoveringButton = ImGui::IsMouseHoveringRect(min, max, false);
    //        bool showTexture = !(hoveringButton && (payload != NULL && payload->IsDataType("Font")));
    //        if (tex && showTexture)
    //        {
    //            if (ImGui::ImageButton((const char*)(tex.get()), tex->GetHandle(), imageButtonSize, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f)))
    //            {
    //                Editor::GetEditor()->GetFileBrowserPanel().Open();
    //                Editor::GetEditor()->GetFileBrowserPanel().SetCallback(callback);
    //            }

    //            if (ImGui::IsItemHovered() && tex)
    //            {
    //                ImGui::BeginTooltip();
    //                ImGui::Image(tex->GetHandle(), ImVec2(512, 512), ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
    //                ImGui::EndTooltip();
    //            }
    //        }
    //        else
    //        {
    //            if (ImGui::Button(tex ? "" : "Empty", imageButtonSize))
    //            {
    //                Editor::GetEditor()->GetFileBrowserPanel().Open();
    //                Editor::GetEditor()->GetFileBrowserPanel().SetCallback(callback);
    //            }
    //        }

    //        if (payload != NULL && payload->IsDataType("Font"))
    //        {
    //            auto filePath = std::string(reinterpret_cast<const char*>(payload->Data));
    //            if (Editor::GetEditor()->IsFontFile(filePath))
    //            {
    //                if (ImGui::BeginDragDropTarget())
    //                {
    //                    // Drop directly on to node and append to the end of it's children list.
    //                    if (ImGui::AcceptDragDropPayload("Font"))
    //                    {
    //                        Application::Get().GetFontLibrary()->Load(filePath, text.FontHandle);
    //                        ImGui::EndDragDropTarget();

    //                        ImGui::Columns(1);
    //                        ImGui::Separator();
    //                        ImGui::PopStyleVar(2);

    //                        ImGui::TreePop();
    //                        return;
    //                    }

    //                    ImGui::EndDragDropTarget();
    //                }
    //            }
    //        }

    //        ImGui::NextColumn();
    //        ImGui::PushItemWidth(-1);
    //        ImGui::TextUnformatted(tex ? tex->GetFilepath().c_str() : "No Texture");
    //        if (tex)
    //        {
    //            ImGuiUtilities::Tooltip(tex->GetFilepath().c_str());
    //            ImGui::Text("%u x %u", tex->GetWidth(), tex->GetHeight());
    //            ImGui::Text("Mip Levels : %u", tex->GetMipMapLevels());
    //        }

    //        ImGui::PopItemWidth();
    //        ImGui::NextColumn();

    //        ImGui::Columns(1);
    //        ImGui::Separator();
    //        ImGui::PopStyleVar();
    //        ImGui::TreePop();
    //    }

    //    ImGui::Columns(1);
    //    ImGui::Separator();
    //    ImGui::PopStyleVar();
    //}

using Rendering::Light;
using Rendering::LightType;

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
        ImGuiUtils::Property("Intensity", light.intensity, 0.0f, 4.0f);

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

    PrimitiveType GetPrimativeName(const std::string& type)
    {
        
        if (type == "Cube")
        {
            return PrimitiveType::Cube;
        }
        else if (type == "Quad")
        {
            return PrimitiveType::Quad;
        }
        else if (type == "Sphere")
        {
            return PrimitiveType::Sphere;
        }
        else if (type == "Capsule")
        {
            return PrimitiveType::Capsule;
        }
        else if (type == "Cylinder")
        {
            return PrimitiveType::Cylinder;
        }
        else if (type == "Terrain")
        {
            return PrimitiveType::Terrain;
        }

        LOG_ERROR("Primitive not supported");
        return PrimitiveType::Cube;
    };

    std::string GetPrimativeName(PrimitiveType type)
    {
        
        switch (type)
        {
        case PrimitiveType::Cube:
            return "Cube";
        case PrimitiveType::Plane:
            return "Plane";
        case PrimitiveType::Quad:
            return "Quad";
        case PrimitiveType::Sphere:
            return "Sphere"; 
        case PrimitiveType::Capsule:
            return "Capsule";
        case PrimitiveType::Cylinder:
            return "Cylinder";
        case PrimitiveType::Terrain:
            return "Terrain";
        case PrimitiveType::External:
            return "External";
        case PrimitiveType::None:
            return "None";
        }

        LOG_ERROR("Primitive not supported");
        return "";
    };

    void TextureWidget(const char* label, Material* material, Texture* tex, bool flipImage, float& usingMapProperty, glm::vec4& colourProperty, const std::function<void(const std::string&)>& callback, const ImVec2& imageButtonSize = ImVec2(64, 64))
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
                if (ImGui::ImageButton((const char*)(tex), (void*)tex->GetHandle(), imageButtonSize, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f)))
                {
                    /*Editor::GetEditor()->GetFileBrowserPanel().Open();
                    Editor::GetEditor()->GetFileBrowserPanel().SetCallback(callback);*/
                }

                if (ImGui::IsItemHovered() && tex)
                {
                   /* ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted(tex->GetFilepath().c_str());
                    ImGui::PopTextWrapPos();
                    ImGui::Image(tex->GetHandle(), imageButtonSize * 3.0f, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
                    ImGui::EndTooltip();*/
                }
            }
            else
            {
                if (ImGui::Button(tex ? "" : "Empty", imageButtonSize))
                {
                    /*Editor::GetEditor()->GetFileBrowserPanel().Open();
                    Editor::GetEditor()->GetFileBrowserPanel().SetCallback(callback);*/
                }
            }

            //if (payload != NULL && payload->IsDataType("AssetFile"))
            //{
            //    auto filePath = std::string(reinterpret_cast<const char*>(payload->Data));
            //    if (Editor::GetEditor()->IsTextureFile(filePath))
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
           

            if (tex)
            {
                ImGui::Text("%u x %u", tex->GetWidth(), tex->GetHeight());
            }

            ImGui::SliderFloat(ImGuiUtils::GenerateLabelID("Use Map"), &usingMapProperty, 0.0f, 1.0f);

            ImGui::ColorEdit4(ImGuiUtils::GenerateLabelID("Colour"), glm::value_ptr(colourProperty));
        

            ImGui::Columns(1);

            ImGui::Separator();
            ImGui::PopStyleVar();

            ImGui::TreePop();
        }
    }

    void TextureWidget(const char* label, Material* material, Texture* tex, bool flipImage, float& usingMapProperty, float& amount, bool hasAmountValue, const std::function<void(const std::string&)>& callback, const ImVec2& imageButtonSize = ImVec2(64, 64))
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
                if (ImGui::ImageButton((const char*)(tex), (void*)tex->GetHandle(), imageButtonSize, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f)))
                {
                  
                }

                if (ImGui::IsItemHovered() && tex)
                {
                   
                }
            }
            else
            {
                if (ImGui::Button(tex ? "" : "Empty", imageButtonSize))
                {
                    
                }
            }

            //if (payload != NULL && payload->IsDataType("AssetFile"))
            //{
            //    auto filePath = std::string(reinterpret_cast<const char*>(payload->Data));
            //    if (Editor::GetEditor()->IsTextureFile(filePath))
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

    //template <>
    //void ComponentEditorWidget<ModelComponent>(entt::registry& reg, entt::registry::entity_type e)
    //{
    //    
    //    auto& model = *reg.get<ModelComponent>(e).ModelRef.get();

    //    auto primitiveType = reg.get<ModelComponent>(e).ModelRef ? model.GetPrimitiveType() : PrimitiveType::None;

    //    ImGuiUtilities::PushID();
    //    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    //    ImGui::Columns(2);
    //    ImGui::Separator();

    //    ImGui::TextUnformatted("Primitive Type");

    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);

    //    const char* shapes[] = { "Sphere", "Cube", "Pyramid", "Capsule", "Cylinder", "Terrain", "File", "Quad", "None" };
    //    std::string shape_current = GetPrimativeName(primitiveType).c_str();
    //    if (ImGui::BeginCombo("", shape_current.c_str(), 0)) // The second parameter is the label previewed before opening the combo.
    //    {
    //        for (int n = 0; n < 8; n++)
    //        {
    //            bool is_selected = (shape_current.c_str() == shapes[n]);
    //            if (ImGui::Selectable(shapes[n], shape_current.c_str()))
    //            {
    //                if (reg.get<ModelComponent>(e).ModelRef)
    //                    model.GetMeshesRef().clear();

    //                if (strcmp(shapes[n], "File") != 0)
    //                {
    //                    if (reg.get<ModelComponent>(e).ModelRef)
    //                    {
    //                        model.GetMeshesRef().push_back(SharedPtr<Mesh>(CreatePrimative(GetPrimativeName(shapes[n]))));
    //                        model.SetPrimitiveType(GetPrimativeName(shapes[n]));
    //                    }
    //                    else
    //                    {
    //                        reg.get<ModelComponent>(e).LoadPrimitive(GetPrimativeName(shapes[n]));
    //                    }
    //                }
    //                else
    //                {
    //                    if (reg.get<ModelComponent>(e).ModelRef)
    //                        model.SetPrimitiveType(PrimitiveType::File);
    //                }
    //            }
    //            if (is_selected)
    //                ImGui::SetItemDefaultFocus();
    //        }
    //        ImGui::EndCombo();
    //    }

    //    ImGui::PopItemWidth();
    //    ImGui::NextColumn();

    //    if (primitiveType == PrimitiveType::File)
    //    {
    //        ImGui::TextUnformatted("FilePath");

    //        ImGui::NextColumn();
    //        ImGui::PushItemWidth(-1);
    //        ImGui::TextUnformatted(model.GetFilePath().c_str());
    //        ImGuiUtilities::Tooltip(model.GetFilePath().c_str());

    //        ImGui::PopItemWidth();
    //        ImGui::NextColumn();
    //    }

    //    ImGui::Columns(1);
    //    ImGui::Separator();
    //    ImGui::PopStyleVar();

    //    int matIndex = 0;

    //    auto modelRef = reg.get<ModelComponent>(e).ModelRef;
    //    if (!modelRef)
    //    {
    //        ImGuiUtilities::PopID();
    //        return;
    //    }

    //    ImGui::Separator();
    //    const auto& meshes = modelRef->GetMeshes();
    //    if (ImGui::TreeNode("Meshes"))
    //    {
    //        for (auto mesh : meshes)
    //        {
    //            if (!mesh->GetName().empty())
    //                ImGui::TextUnformatted(mesh->GetName().c_str());
    //        }
    //        ImGui::TreePop();
    //    }

    //    ImGui::Separator();

    //    auto Skeleton = modelRef->GetSkeleton();

    //    if (Skeleton)
    //    {
    //        ImGui::TextUnformatted("Animation");

    //        auto jointNames = Skeleton->joint_names();
    //        for (auto& joint : jointNames)
    //        {
    //            ImGui::TextUnformatted(joint);
    //        }

    //        const auto& animations = modelRef->GetAnimations();
    //    }

    //    ImGui::Separator();
    //    if (ImGui::TreeNode("Materials"))
    //    {
    //        Material* MaterialShown[1000];
    //        uint32_t MaterialCount = 0;
    //        for (auto mesh : meshes)
    //        {
    //            auto material = mesh->GetMaterial();
    //            std::string matName = material ? material->GetName() : "";

    //            bool materialFound = false;
    //            for (uint32_t i = 0; i < MaterialCount; i++)
    //            {
    //                if (MaterialShown[i] == material.get())
    //                    materialFound = true;
    //            }

    //            if (materialFound)
    //                continue;

    //            MaterialShown[MaterialCount++] = material.get();

    //            if (matName.empty())
    //            {
    //                matName = "Material";
    //                matName += std::to_string(matIndex);
    //            }

    //            matName += "##" + std::to_string(matIndex);
    //            matIndex++;
    //            if (!material)
    //            {
    //                ImGui::TextUnformatted("Empty Material");
    //                if (ImGui::Button("Add Material", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
    //                    mesh->SetMaterial(CreateSharedPtr<Material>());
    //            }
    //            else if (ImGui::TreeNodeEx(matName.c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth))
    //            {
    //                using namespace Lumos;
    //                ImGui::Indent();
    //                if (ImGui::Button("Save to file"))
    //                {
    //                    std::string filePath = "//Meshes"; // Materials/" + matName + ".lmat";
    //                    std::string physicalPath;
    //                    if (VFS::Get().ResolvePhysicalPath(filePath, physicalPath))
    //                    {
    //                        physicalPath += "/Materials/" + matName + ".lmat";
    //                        std::stringstream storage;

    //                        cereal::JSONOutputArchive output{ storage };
    //                        material->save(output);

    //                        FileSystem::WriteTextFile(physicalPath, storage.str());
    //                    }
    //                }
    //                bool flipImage = Graphics::Renderer::GetGraphicsContext()->FlipImGUITexture();

    //                bool twoSided = material->GetFlag(Material::RenderFlags::TWOSIDED);
    //                bool depthTested = material->GetFlag(Material::RenderFlags::DEPTHTEST);
    //                bool alphaBlended = material->GetFlag(Material::RenderFlags::ALPHABLEND);

    //                ImGui::Columns(2);
    //                ImGui::Separator();

    //                ImGui::AlignTextToFramePadding();

    //                if (ImGuiUtilities::Property("Alpha Blended", alphaBlended))
    //                    material->SetFlag(Material::RenderFlags::ALPHABLEND, alphaBlended);

    //                if (ImGuiUtilities::Property("Two Sided", twoSided))
    //                    material->SetFlag(Material::RenderFlags::TWOSIDED, twoSided);

    //                if (ImGuiUtilities::Property("Depth Tested", depthTested))
    //                    material->SetFlag(Material::RenderFlags::DEPTHTEST, depthTested);

    //                ImGui::Columns(1);

    //                Graphics::MaterialProperties* prop = material->GetProperties();
    //                auto colour = glm::vec4();
    //                float normal = 0.0f;
    //                auto& textures = material->GetTextures();
    //                glm::vec2 textureSize = glm::vec2(100.0f, 100.0f);
    //                TextureWidget("Albedo", material.get(), textures.albedo.get(), flipImage, prop->albedoMapFactor, prop->albedoColour, std::bind(&Graphics::Material::SetAlbedoTexture, material, std::placeholders::_1), textureSize * Application::Get().GetWindowDPI());
    //                ImGui::Separator();

    //                TextureWidget("Normal", material.get(), textures.normal.get(), flipImage, prop->normalMapFactor, normal, false, std::bind(&Graphics::Material::SetNormalTexture, material, std::placeholders::_1), textureSize * Application::Get().GetWindowDPI());
    //                ImGui::Separator();

    //                TextureWidget("Metallic", material.get(), textures.metallic.get(), flipImage, prop->metallicMapFactor, prop->metallic, true, std::bind(&Graphics::Material::SetMetallicTexture, material, std::placeholders::_1), textureSize * Application::Get().GetWindowDPI());
    //                ImGui::Separator();

    //                TextureWidget("Roughness", material.get(), textures.roughness.get(), flipImage, prop->roughnessMapFactor, prop->roughness, true, std::bind(&Graphics::Material::SetRoughnessTexture, material, std::placeholders::_1), textureSize * Application::Get().GetWindowDPI());

    //                if (ImGui::TreeNodeEx("Reflectance", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth))
    //                {
    //                    ImGui::SliderFloat("##Reflectance", &prop->reflectance, 0.0f, 1.0f);
    //                    ImGui::TreePop();
    //                }

    //                ImGui::Separator();

    //                TextureWidget("AO", material.get(), textures.ao.get(), flipImage, prop->occlusionMapFactor, normal, false, std::bind(&Graphics::Material::SetAOTexture, material, std::placeholders::_1), textureSize * Application::Get().GetWindowDPI());
    //                ImGui::Separator();

    //                TextureWidget("Emissive", material.get(), textures.emissive.get(), flipImage, prop->emissiveMapFactor, prop->emissive, true, std::bind(&Graphics::Material::SetEmissiveTexture, material, std::placeholders::_1), textureSize * Application::Get().GetWindowDPI());

    //                ImGui::Columns(2);

    //                ImGui::AlignTextToFramePadding();
    //                ImGui::TextUnformatted("WorkFlow");
    //                ImGui::NextColumn();
    //                ImGui::PushItemWidth(-1);

    //                int workFlow = (int)material->GetProperties()->workflow;

    //                if (ImGui::DragInt("##WorkFlow", &workFlow, 0.3f, 0, 2))
    //                {
    //                    material->GetProperties()->workflow = (float)workFlow;
    //                }

    //                ImGui::PopItemWidth();
    //                ImGui::NextColumn();

    //                material->SetMaterialProperites(*prop);
    //                ImGui::Columns(1);
    //                ImGui::Unindent();
    //                ImGui::TreePop();
    //            }
    //        }
    //        ImGui::TreePop();
    //    }

    //    ImGuiUtilities::PopID();
    //}

    //template <>
    //void ComponentEditorWidget<Environment>(entt::registry& reg, entt::registry::entity_type e)
    //{
    //    
    //    auto& environment = reg.get<Environment>(e);
    //    // Disable image until texturecube is supported
    //    // ImGuiUtilities::Image(environment.GetEnvironmentMap(), glm::vec2(200, 200));

    //    uint8_t mode = environment.GetMode();
    //    glm::vec4 params = environment.GetParameters();
    //    ImGui::PushItemWidth(-1);

    //    const char* modes[] = { "Textures", "Preetham", "Generic" };
    //    const char* mode_current = modes[mode];
    //    if (ImGui::BeginCombo("", mode_current, 0)) // The second parameter is the label previewed before opening the combo.
    //    {
    //        for (int n = 0; n < 3; n++)
    //        {
    //            bool is_selected = (mode_current == modes[n]);
    //            if (ImGui::Selectable(modes[n], mode_current))
    //            {
    //                environment.SetMode(n);
    //            }
    //            if (is_selected)
    //                ImGui::SetItemDefaultFocus();
    //        }
    //        ImGui::EndCombo();
    //    }

    //    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    //    ImGui::Columns(2);
    //    ImGui::Separator();

    //    if (mode == 0)
    //    {
    //        ImGui::TextUnformatted("File Path");
    //        ImGui::NextColumn();
    //        ImGui::PushItemWidth(-1);

    //        static char filePath[INPUT_BUF_SIZE];
    //        strcpy(filePath, environment.GetFilePath().c_str());

    //        if (ImGui::InputText("##filePath", filePath, IM_ARRAYSIZE(filePath), 0))
    //        {
    //            environment.SetFilePath(filePath);
    //        }

    //        ImGui::PopItemWidth();
    //        ImGui::NextColumn();

    //        ImGui::AlignTextToFramePadding();
    //        ImGui::TextUnformatted("File Type");
    //        ImGui::NextColumn();
    //        ImGui::PushItemWidth(-1);

    //        static char fileType[INPUT_BUF_SIZE];
    //        strcpy(fileType, environment.GetFileType().c_str());

    //        if (ImGui::InputText("##fileType", fileType, IM_ARRAYSIZE(fileType), 0))
    //        {
    //            environment.SetFileType(fileType);
    //        }

    //        ImGui::PopItemWidth();
    //        ImGui::NextColumn();

    //        ImGui::AlignTextToFramePadding();
    //        ImGui::TextUnformatted("Width");
    //        ImGui::NextColumn();
    //        ImGui::PushItemWidth(-1);
    //        int width = environment.GetWidth();

    //        if (ImGui::DragInt("##Width", &width))
    //        {
    //            environment.SetWidth(width);
    //        }

    //        ImGui::PopItemWidth();
    //        ImGui::NextColumn();

    //        ImGui::AlignTextToFramePadding();
    //        ImGui::TextUnformatted("Height");
    //        ImGui::NextColumn();
    //        ImGui::PushItemWidth(-1);
    //        int height = environment.GetHeight();

    //        if (ImGui::DragInt("##Height", &height))
    //        {
    //            environment.SetHeight(height);
    //        }

    //        ImGui::PopItemWidth();
    //        ImGui::NextColumn();

    //        ImGui::AlignTextToFramePadding();
    //        ImGui::TextUnformatted("Num Mips");
    //        ImGui::NextColumn();
    //        ImGui::PushItemWidth(-1);
    //        int numMips = environment.GetNumMips();
    //        if (ImGui::InputInt("##NumMips", &numMips))
    //        {
    //            environment.SetNumMips(numMips);
    //        }

    //        ImGui::PopItemWidth();
    //        ImGui::NextColumn();
    //    }
    //    else if (mode == 1)
    //    {
    //        bool valueUpdated = false;
    //        valueUpdated |= ImGuiUtilities::Property("Turbidity", params.x, 1.7f, 100.0f, 0.01f);
    //        valueUpdated |= ImGuiUtilities::Property("Azimuth", params.y, -1000.0f, 1000.f, 0.01f);
    //        valueUpdated |= ImGuiUtilities::Property("Inclination", params.z, -1000.0f, 1000.f, 0.01f);

    //        if (valueUpdated)
    //            environment.SetParameters(params);
    //    }

    //    ImGui::Columns(1);
    //    if (ImGui::Button("Reload", ImVec2(ImGui::GetContentRegionAvail().x, 0.0)))
    //        environment.Load();

    //    ImGui::Separator();
    //    ImGui::PopStyleVar();
    //}

    //template <>
    //void ComponentEditorWidget<TextureMatrixComponent>(entt::registry& reg, entt::registry::entity_type e)
    //{
    //    
    //    auto& textureMatrix = reg.get<TextureMatrixComponent>(e);
    //    glm::mat4& mat = textureMatrix.GetMatrix();

    //    glm::vec3 skew;
    //    glm::vec3 position;
    //    glm::vec3 scale;
    //    glm::vec4 perspective;
    //    glm::quat rotation;
    //    glm::decompose(mat, scale, rotation, position, skew, perspective);

    //    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    //    ImGui::Columns(2);
    //    ImGui::Separator();

    //    ImGui::AlignTextToFramePadding();
    //    ImGui::TextUnformatted("Position");
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);
    //    if (ImGui::DragFloat3("##Position", glm::value_ptr(position)))
    //    {
    //        Maths::SetTranslation(mat, position);
    //    }

    //    ImGui::PopItemWidth();
    //    ImGui::NextColumn();

    //    ImGui::AlignTextToFramePadding();
    //    ImGui::TextUnformatted("Rotation");
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);
    //    if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation)))
    //    {
    //        float pitch = Maths::Min(rotation.x, 89.9f);
    //        pitch = Maths::Max(pitch, -89.9f);
    //        Maths::SetRotation(mat, glm::vec3(pitch, rotation.y, rotation.z));
    //    }

    //    ImGui::PopItemWidth();
    //    ImGui::NextColumn();

    //    ImGui::AlignTextToFramePadding();
    //    ImGui::TextUnformatted("Scale");
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);
    //    if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f))
    //    {
    //        Maths::SetScale(mat, scale);
    //    }

    //    ImGui::PopItemWidth();
    //    ImGui::NextColumn();

    //    ImGui::Columns(1);
    //    ImGui::Separator();
    //    ImGui::PopStyleVar();
    //}

    //template <>
    //void ComponentEditorWidget<DefaultCameraController>(entt::registry& reg, entt::registry::entity_type e)
    //{
    //    
    //    auto& controllerComp = reg.get<DefaultCameraController>(e);
    //    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    //    ImGui::Columns(2);
    //    ImGui::Separator();

    //    ImGui::AlignTextToFramePadding();
    //    ImGui::TextUnformatted("Controller Type");
    //    ImGui::NextColumn();
    //    ImGui::PushItemWidth(-1);

    //    const char* controllerTypes[] = { "Editor", "FPS", "ThirdPerson", "2D", "Custom" };
    //    std::string currentController = DefaultCameraController::CameraControllerTypeToString(controllerComp.GetType());
    //    if (ImGui::BeginCombo("", currentController.c_str(), 0)) // The second parameter is the label previewed before opening the combo.
    //    {
    //        for (int n = 0; n < 5; n++)
    //        {
    //            bool is_selected = (currentController.c_str() == controllerTypes[n]);
    //            if (ImGui::Selectable(controllerTypes[n], currentController.c_str()))
    //            {
    //                controllerComp.SetControllerType(DefaultCameraController::StringToControllerType(controllerTypes[n]));
    //            }
    //            if (is_selected)
    //                ImGui::SetItemDefaultFocus();
    //        }
    //        ImGui::EndCombo();
    //    }

    //    if (controllerComp.GetController())
    //        controllerComp.GetController()->OnImGui();

    //    ImGui::Columns(1);
    //    ImGui::Separator();
    //    ImGui::PopStyleVar();
    //}

    //template <>
    //void ComponentEditorWidget<Listener>(entt::registry& reg, entt::registry::entity_type e)
    //{
    //}
}


namespace SaltnPepperEngine
{
	
	namespace Editor
	{
        static bool init = false;

		InspectorWindow::InspectorWindow()
		{
            m_name = "Inspector";
		}

		void InspectorWindow::OnImgui()
		{
            auto selectedEntities = m_editorHandle->GetSelectedEntities();

            if (ImGui::Begin(m_name.c_str(), &m_active))
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
                Entity selectedEntity = { selected, currentScene };

                ActiveComponent* activeComp = registry.try_get<ActiveComponent>(selected);
                bool active = activeComp ? activeComp->active : true;

                if (ImGui::Checkbox("Active?", &active))
                {
                    if (!activeComp)
                    {
                        registry.emplace<ActiveComponent>(selected, active);
                    }
                    else
                    {
                        activeComp->active = active;
                    }

                    bool hasName = registry.all_of<NameComponent>(selected);
                    std::string name;

                    if (hasName)
                    {
                        name = registry.get<NameComponent>(selected).name;
                    }
                    else
                    {
                        name = std::to_string((entt::to_integral(selected)));
                    }

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetFontSize() * 4.0f);
                    {
                        ImGuiUtils::ScopedFont boldFont(ImGui::GetIO().Fonts->Fonts[1]);
                        if (ImGuiUtils::InputText(name))
                            registry.get_or_emplace<NameComponent>(selected).name = name;
                    }
                    ImGui::SameLine();

                    //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.0f));
                    
                        
                }

               
               
            }

            ImGui::End();
            
		}

		void InspectorWindow::OnInit()
		{

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
