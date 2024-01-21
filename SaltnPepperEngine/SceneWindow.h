#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include "Editor/EditorWindow.h"
#include "Editor/Editor.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Frustum.h"
#include "Editor/ImGuiUtils.h"
#include "Engine/Macros.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"

#include <imgui/imgui.h>
DISABLE_PUSH_WARNING
DISABLE_WARNING_CONVERSION_TO_SMALLER_TYPE
#include <entt/entt.hpp>
DISABLE_POP_WARNING

namespace SaltnPepperEngine
{
    namespace Rendering
    {
        class Texture;
    }



    using namespace Rendering;
    using namespace Components;

	namespace Editor
	{

		class SceneWindow : public EditorWindow
		{
		public:

			SceneWindow();
			~SceneWindow() = default;


			virtual void OnInit();
			virtual void OnImgui() override;

			void ToolBar();
			void Resize(uint32_t width, uint32_t height);

		private:

            template <typename T>
            void ShowComponentGizmo(float width, float height, float xpos, float ypos, const Matrix4& viewProj, const Frustum& frustum, entt::registry& registry)
            {
                if (m_ShowComponentGizmoMap[typeid(T).hash_code()])
                {
                    auto group = registry.group<T>(entt::get<Transform>);

                    for (auto entity : group)
                    {
                        const auto& [component, trans] = group.template get<T, Transform>(entity);

                        Vector3 pos = trans.GetWorldPosition();

                        auto inside = frustum.Intersects(pos);

                        if (inside == IntersectionResult::OUTSIDE)
                            continue;

                        Vector2 screenPos = WorldToScreen(pos, viewProj, width, height, xpos, ypos);
                        ImGui::SetCursorPos({ screenPos.x - ImGui::GetFontSize() * 0.5f, screenPos.y - ImGui::GetFontSize() * 0.5f });
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.0f));


                        ImGui::TextUnformatted(m_editorHandle->GetComponentIconMap()[typeid(T).hash_code()]);
                        ImGui::PopStyleColor();
                        ImGuiUtils::Tooltip(typeid(T).name().c_str());
                   
                    
                    }
                }
            }


            std::unordered_map<size_t, bool> m_ShowComponentGizmoMap;

            bool m_ShowStats = false;
            SharedPtr<Texture> m_GameViewTexture = nullptr;
            Scene* m_CurrentScene = nullptr;
            uint32_t m_Width, m_Height;




		};
	}
}


#endif // !SCENEWINDOW_H


