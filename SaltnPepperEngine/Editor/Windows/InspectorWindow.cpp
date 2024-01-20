#include "InspectorWindow.h"
//#include "Engine/Core/System/Application/Application.h"
//#include "Engine/Utils/Loading/FileSystem.h"
//
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/Components/SceneComponents.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"


#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
// 

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
                if (ImGui::ImageButton((const char*)(tex), (void*)tex->GetHandle(), imageButtonSize, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f)))
                {
                   /* RuntimeEditor::GetEditor()->GetFileBrowserPanel().Open();
                    RuntimeEditor::GetEditor()->GetFileBrowserPanel().SetCallback(callback);*/
                }

                if (ImGui::IsItemHovered() && tex)
                {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted(tex->GetFilePath().c_str());
                    ImGui::PopTextWrapPos();
                    ImGui::Image((ImTextureID*)tex->GetHandle(), imageButtonSize * 3.0f, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
                    ImGui::EndTooltip();
                }
            }
            else
            {
                if (ImGui::Button(tex ? "" : "Empty", imageButtonSize))
                {
                    /*RuntimeEditor::GetEditor()->GetFileBrowserPanel().Open();
                    Runtime:Editor::GetEditor()->GetFileBrowserPanel().SetCallback(callback);*/
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

                   // ImGui::DragFloat(Lumos::ImGuiUtilities::GenerateID(), &amount, 0.0f, 20.0f);

                   // ImGui::PopItemWidth();
                   // ImGui::NextColumn();

                   // ImGuiUtilities::Property("Use Map", usingMapProperty, 0.0f, 1.0f);
                   // ImGuiUtilities::Property("Colour", colourProperty, 0.0f, 1.0f, false, Lumos::ImGuiUtilities::PropertyFlag::ColourProperty);

            ImGui::Columns(1);

            ImGui::Separator();
            ImGui::PopStyleVar();

            ImGui::TreePop();
        }
    }

    void TextureWidget(const char* label,Material* material, Texture* tex, bool flipImage, float& usingMapProperty, float& amount, bool hasAmountValue, const std::function<void(const std::string&)>& callback, const ImVec2& imageButtonSize = ImVec2(64, 64))
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
                   /* RuntimeEditor::GetEditor()->GetFileBrowserPanel().Open();
                    RuntimeEditor::GetEditor()->GetFileBrowserPanel().SetCallback(callback);*/
                }

                if (ImGui::IsItemHovered() && tex)
                {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted(tex->GetFilePath().c_str());
                    ImGui::PopTextWrapPos();

                    ImGui::Image((ImTextureID*)tex->GetHandle(), imageButtonSize * 3.0f, ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
                    ImGui::EndTooltip();
                }
            }
            else
            {
                if (ImGui::Button(tex ? "" : "Empty", imageButtonSize))
                {
                  /*  RuntimeEditor::GetEditor()->GetFileBrowserPanel().Open();
                    RuntimeEditor::GetEditor()->GetFileBrowserPanel().SetCallback(callback);*/
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
                    if (ImGuiUtils::InputText(name))
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
            ICON_COMPONENT(RigidBody3D, "RigidBody3D");
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
