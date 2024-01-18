#include "HierarchyWindow.h"
#include "Engine/Core/EntitySystem/Entity.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Editor/Editor.h"
#include "Editor/MDIcons.h"
#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"


namespace SaltnPepperEngine
{
    using namespace Physics;
	namespace Editor
	{
		HierarchyWindow::HierarchyWindow()
			: m_hadDroppedEntity(entt::null)
			, m_doubleClicked(entt::null)

		{
			m_name = "Hierarchy###hierarchy";
		}



        void HierarchyWindow::DrawNode(entt::entity node, entt::registry& registry)
        {
           
            bool show = true;

            if (!registry.valid(node))
                return;

            Entity nodeEntity = { node, Application::GetCurrent().GetCurrentScene() };

            static const char* defaultName = "Entity";
            const NameComponent* nameComponent = registry.try_get<NameComponent>(node);
            const char* name = nameComponent ? nameComponent->name.c_str() : defaultName; // StringUtilities::ToString(entt::to_integral(node));

            if (m_hierarchyFilter.IsActive())
            {
                if (!m_hierarchyFilter.PassFilter(name))
                {
                    show = false;
                }
            }

            if (show)
            {
                ImGui::PushID((int)node);
                auto hierarchyComponent = registry.try_get<Hierarchy>(node);
                bool noChildren = true;

                if (hierarchyComponent != nullptr && hierarchyComponent->First() != entt::null)
                    noChildren = false;

                ImGuiTreeNodeFlags nodeFlags = ((m_editorHandle->IsEntitySelected(node)) ? ImGuiTreeNodeFlags_Selected : 0);

                nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;

                if (noChildren)
                {
                    nodeFlags |= ImGuiTreeNodeFlags_Leaf;
                }

                // auto activeComponent = registry.try_get<ActiveComponent>(node);
                bool active = Entity(node, Application::GetCurrent().GetCurrentScene()).IsActive(); // activeComponent ? activeComponent->active : true;

                if (!active)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));

                bool doubleClicked = false;
                if (node == m_doubleClicked)
                {
                    doubleClicked = true;
                }

                if (doubleClicked)
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 1.0f, 2.0f });

                if (m_hadDroppedEntity == node)
                {
                    ImGui::SetNextItemOpen(true);
                    m_hadDroppedEntity = entt::null;
                }

                std::string icon = ICON_MDI_CUBE_OUTLINE;
                auto& iconMap = m_editorHandle->GetComponentIconMap();

                if (registry.all_of<Camera>(node))
                {
                    if (iconMap.find(typeid(Camera).hash_code()) != iconMap.end())
                        icon = iconMap[typeid(Camera).hash_code()];
                }
               
               
                else if (registry.all_of<RigidBody3D>(node))
                {
                    if (iconMap.find(typeid(RigidBody3D).hash_code()) != iconMap.end())
                        icon = iconMap[typeid(RigidBody3D).hash_code()];
                }
                else if (registry.all_of<Light>(node))
                {
                    if (iconMap.find(typeid(Light).hash_code()) != iconMap.end())
                        icon = iconMap[typeid(Light).hash_code()];
                }
               
               /* else if (registry.all_of<TextComponent>(node))
                {
                    if (iconMap.find(typeid(TextComponent).hash_code()) != iconMap.end())
                        icon = iconMap[typeid(TextComponent).hash_code()];
                }*/

                Vector4 color = ImGuiUtils::GetIconColour();
                ImVec4 iconColor = ImVec4(color.x, color.y, color.z, color.w);


                ImGui::PushStyleColor(ImGuiCol_Text, iconColor);
                // ImGui::BeginGroup();
                bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entt::to_integral(node), nodeFlags, "%s", icon.c_str());
                {
                    // Allow clicking of icon and text. Need twice as they are separated
                    if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen())
                    {
                        bool ctrlDown = InputSystem::GetInstance().GetKeyHeld(Key::LeftControl) || InputSystem::GetInstance().GetKeyHeld(Key::RightControl);
                        if (!ctrlDown)
                            m_editorHandle->ClearSelectedEntity();

                        if (!m_editorHandle->IsEntitySelected(node))
                            m_editorHandle->SelectEntity(node);
                        else
                            m_editorHandle->UnselectEntity(node);
                    }
                    else if (m_doubleClicked == node && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered(ImGuiHoveredFlags_None))
                        m_doubleClicked = entt::null;
                }
                ImGui::PopStyleColor();
                ImGui::SameLine();
                if (!doubleClicked)
                {
                    bool isPrefab = false;
                    if (registry.any_of<PrefabComponent>(node))
                        isPrefab = true;
                    else
                    {
                        auto Parent = nodeEntity.GetParent();
                        while (Parent && Parent.IsValid())
                        {
                            if (Parent.HasComponent<PrefabComponent>())
                            {
                                isPrefab = true;
                                Parent = {};
                            }
                            else
                            {
                                Parent = Parent.GetParent();
                            }
                        }
                    }

                    if (isPrefab)
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_CheckMark));
                    ImGui::TextUnformatted(name);
                    if (isPrefab)
                        ImGui::PopStyleColor();
                }
                // ImGui::EndGroup();

                if (doubleClicked)
                {
                    static char objName[128];
                    strcpy_s(objName, name);
                    ImGui::PushItemWidth(-1);
                    if (ImGui::InputText("##Name", objName, IM_ARRAYSIZE(objName), 0))
                        registry.get_or_emplace<NameComponent>(node).name = objName;
                    ImGui::PopStyleVar();
                }

                if (!active)
                    ImGui::PopStyleColor();

                bool deleteEntity = false;
                if (ImGui::BeginPopupContextItem(name))
                {
                    if (ImGui::Selectable("Copy"))
                    {
                        if (!m_editorHandle->IsEntitySelected(node))
                        {
                            m_editorHandle->SetCopiedEntity(node);
                        }
                        for (auto entity : m_editorHandle->GetSelectedEntities())
                            m_editorHandle->SetCopiedEntity(entity);
                    }

                    if (ImGui::Selectable("Cut"))
                    {
                        for (auto entity : m_editorHandle->GetSelectedEntities())
                            m_editorHandle->SetCopiedEntity(node, true);
                    }

                    if (m_editorHandle->GetCopiedEntity().size() > 0 && registry.valid(m_editorHandle->GetCopiedEntity().front()))
                    {
                        if (ImGui::Selectable("Paste"))
                        {
                            for (auto entity : m_editorHandle->GetCopiedEntity())
                            {
                                auto scene = Application::GetCurrent().GetCurrentScene();
                                Entity copiedEntity = { entity, scene };
                                if (!copiedEntity.IsValid())
                                {
                                    m_editorHandle->SetCopiedEntity(entt::null);
                                }
                                else
                                {
                                    scene->Duplicate(copiedEntity, { node, scene });

                                    if (m_editorHandle->GetCutCopyEntity())
                                        deleteEntity = true;
                                }
                            }
                        }
                    }
                    else
                    {
                        ImGui::TextDisabled("Paste");
                    }

                    ImGui::Separator();

                    if (ImGui::Selectable("Duplicate"))
                    {
                        auto scene = Application::GetCurrent().GetCurrentScene();
                        scene->Duplicate({ node, scene });
                    }
                    if (ImGui::Selectable("Delete"))
                        deleteEntity = true;
                    // if(m_editorHandle->IsSelected(node))
                    //   m_editorHandle->UnSelect(node);
                    ImGui::Separator();
                    if (ImGui::Selectable("Rename"))
                        m_doubleClicked = node;
                    ImGui::Separator();

                    if (ImGui::Selectable("Add Child"))
                    {
                        auto scene = Application::GetCurrent().GetCurrentScene();
                        auto child = scene->CreateEntity();
                        child.SetParent({ node, scene });
                    }

                    if (ImGui::Selectable("Zoom to"))
                    {
                        // if(Application::Get().GetEditorState() == EditorState::Preview)
                        {
                            auto transform = registry.try_get<Transform>(node);
                            if (transform)
                                m_editorHandle->FocusCamera(transform->GetWorldPosition(), 2.0f, 2.0f);
                        }
                    }
                    ImGui::EndPopup();
                }

                if (!doubleClicked && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    auto ptr = node;
                    ImGui::SetDragDropPayload("Drag_Entity", &ptr, sizeof(entt::entity*));
                    ImGui::Text(ICON_MDI_ARROW_UP);
                    ImGui::EndDragDropSource();
                }

                const ImGuiPayload* payload = ImGui::GetDragDropPayload();
                if (payload != NULL && payload->IsDataType("Drag_Entity"))
                {
                    bool acceptable;

                    if (payload->DataSize != sizeof(entt::entity*))
                    { LOG_ERROR("Error ImGUI drag entity"); }

                    auto entity = *reinterpret_cast<entt::entity*>(payload->Data);
                    auto hierarchyComponent = registry.try_get<Hierarchy>(entity);
                    if (hierarchyComponent != nullptr)
                    {
                        acceptable = entity != node && (!IsParentOfEntity(entity, node, registry)) && (hierarchyComponent->Parent() != node);
                    }
                    else
                        acceptable = entity != node;

                    if (ImGui::BeginDragDropTarget())
                    {
                        // Drop directly on to node and append to the end of it's children list.
                        if (ImGui::AcceptDragDropPayload("Drag_Entity"))
                        {
                            if (acceptable)
                            {
                                if (hierarchyComponent)
                                    Hierarchy::Reparent(entity, node, registry, *hierarchyComponent);
                                else
                                {
                                    registry.emplace<Hierarchy>(entity, node);
                                }
                                m_hadDroppedEntity = node;
                            }
                        }

                        ImGui::EndDragDropTarget();
                    }

                    if (m_editorHandle->IsEntitySelected(entity))
                        m_editorHandle->UnselectEntity(entity);
                }

                if (ImGui::IsItemClicked() && !deleteEntity)
                    m_editorHandle->SelectEntity(node);
                else if (m_doubleClicked == node && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered(ImGuiHoveredFlags_None))
                    m_doubleClicked = entt::null;

                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
                {
                    m_doubleClicked = node;
                    if (m_editorHandle->GetEditorState() == EditorState::Preview)
                    {
                        auto transform = registry.try_get<Transform>(node);
                        if (transform)
                            m_editorHandle->FocusCamera(transform->GetWorldPosition(), 2.0f, 2.0f);
                    }
                }

                if (deleteEntity)
                {
                    nodeEntity.GetScene()->DestroyEntity(nodeEntity);
                    if (nodeOpen)
                        ImGui::TreePop();

                    ImGui::PopID();
                    return;
                }

                if (m_selectUp)
                {
                    if (!m_editorHandle->GetSelectedEntities().empty() && m_editorHandle->GetSelectedEntities().front() == node && registry.valid(m_currentPrevious))
                    {
                        m_selectUp = false;
                        m_editorHandle->SelectEntity(m_currentPrevious);
                    }
                }

                if (m_selectDown)
                {
                    if (!m_editorHandle->GetSelectedEntities().empty() && registry.valid(m_currentPrevious) && m_currentPrevious == m_editorHandle->GetSelectedEntities().front())
                    {
                        m_selectDown = false;
                        m_editorHandle->SelectEntity(node);
                    }
                }

                m_currentPrevious = node;

#if 1
                ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize(ICON_MDI_EYE).x * 2.0f);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.0f));
                if (ImGui::Button(active ? ICON_MDI_EYE : ICON_MDI_EYE_OFF))
                {
                    auto& activeComponent = registry.get_or_emplace<ActiveComponent>(node);

                    activeComponent.active = !active;
                }
                ImGui::PopStyleColor();
#endif

                if (nodeOpen == false)
                {
                    ImGui::PopID();
                    return;
                }

                const ImColor TreeLineColor = ImColor(128, 128, 128, 128);
                const float SmallOffsetX = 6.0f * 1.0f;
                ImDrawList* drawList = ImGui::GetWindowDrawList();

                ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
                verticalLineStart.x += SmallOffsetX; // to nicely line up with the arrow symbol
                ImVec2 verticalLineEnd = verticalLineStart;

                if (!noChildren)
                {
                    entt::entity child = hierarchyComponent->First();
                    while (child != entt::null && registry.valid(child))
                    {
                        float HorizontalTreeLineSize = 20.0f * 1.0f; // chosen arbitrarily
                        auto currentPos = ImGui::GetCursorScreenPos();
                        ImGui::Indent(10.0f);

                        auto childHerarchyComponent = registry.try_get<Hierarchy>(child);

                        if (childHerarchyComponent)
                        {
                            entt::entity firstChild = childHerarchyComponent->First();
                            if (firstChild != entt::null && registry.valid(firstChild))
                            {
                                HorizontalTreeLineSize *= 0.1f;
                            }
                        }
                        DrawNode(child, registry);
                        ImGui::Unindent(10.0f);

                        const ImRect childRect = ImRect(currentPos, currentPos + ImVec2(0.0f, ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y));

                        const float midpoint = (childRect.Min.y + childRect.Max.y) * 0.5f;
                        drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + HorizontalTreeLineSize, midpoint), TreeLineColor);
                        verticalLineEnd.y = midpoint;

                        if (registry.valid(child))
                        {
                            auto hierarchyComponent = registry.try_get<Hierarchy>(child);
                            child = hierarchyComponent ? hierarchyComponent->Next() : entt::null;
                        }
                    }
                }

                drawList->AddLine(verticalLineStart, verticalLineEnd, TreeLineColor);

                ImGui::TreePop();
                ImGui::PopID();
            }
        }

		void HierarchyWindow::OnImgui()
		{
			

            
            auto flags = ImGuiWindowFlags_NoCollapse;
            m_currentPrevious = entt::null;
            m_selectUp = false;
            m_selectDown = false;

            m_selectUp = InputSystem::GetInstance().GetKeyDown(Key::Up);
            m_selectDown = InputSystem::GetInstance().GetKeyDown(Key::Down);

            ImGui::Begin(m_name.c_str(), &m_active, flags);
            {
                auto scene = Application::GetCurrent().GetCurrentScene();

                if (!scene)
                {
                    ImGui::End();
                    return;
                }
                auto& registry = scene->GetRegistry();

                auto AddEntity = [scene]()
                    {
                        if (ImGui::Selectable("Add Empty Entity"))
                        {
                            scene->CreateEntity();
                        }

                        if (ImGui::Selectable("Add Light"))
                        {
                            auto entity = scene->CreateEntity("Light");
                            entity.AddComponent<Light>();
                            entity.GetOrAddComponent<Transform>();
                        }

                        if (ImGui::Selectable("Add Rigid Body"))
                        {
                          /*  auto entity = scene->CreateEntity("RigidBody");
                            entity.AddComponent<RigidBody3D>();
                            entity.GetOrAddComponent<Transform>();
                          
                            entity.GetComponent<RigidBody3DComponent>().GetRigidBody()->SetCollisionShape(CollisionShapeType::CollisionCuboid);*/
                        }

                        if (ImGui::Selectable("Add Camera"))
                        {
                            auto entity = scene->CreateEntity("Camera");
                            entity.AddComponent<Camera>();
                            entity.GetOrAddComponent<Transform>();
                        }

                       
                    };

                ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));

                if (ImGui::Button(ICON_MDI_PLUS))
                {
                    // Add Entity Menu
                    ImGui::OpenPopup("AddEntity");
                }

                if (ImGui::BeginPopup("AddEntity"))
                {
                    AddEntity();
                    ImGui::EndPopup();
                }

                ImGui::SameLine();
                ImGui::TextUnformatted(ICON_MDI_MAGNIFY);
                ImGui::SameLine();

                {
                    ImGuiUtils::ScopedFont boldFont(ImGui::GetIO().Fonts->Fonts[1]);
                    ImGuiUtils::ScopedStyle frameBorder(ImGuiStyleVar_FrameBorderSize, 0.0f);
                    ImGuiUtils::ScopedColour frameColour(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
                    m_hierarchyFilter.Draw("##HierarchyFilter", ImGui::GetContentRegionAvail().x - ImGui::GetStyle().IndentSpacing);
                    ImGuiUtils::DrawItemActivityOutline(2.0f, false);
                }

                if (!m_hierarchyFilter.IsActive())
                {
                    ImGui::SameLine();
                    ImGuiUtils::ScopedFont boldFont(ImGui::GetIO().Fonts->Fonts[1]);
                    ImGui::SetCursorPosX(ImGui::GetFontSize() * 4.0f);
                    ImGuiUtils::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(0.0f, ImGui::GetStyle().FramePadding.y));
                    ImGui::TextUnformatted("Search...");
                }

                ImGui::PopStyleColor();
                ImGui::Unindent();

                ImGui::Separator();

                ImGui::BeginChild("Nodes");

                if (ImGui::BeginPopupContextWindow())
                {
                    if (!m_editorHandle->GetCopiedEntity().empty() && registry.valid(m_editorHandle->GetCopiedEntity().front()))
                    {
                        if (ImGui::Selectable("Paste"))
                        {
                            for (auto entity : m_editorHandle->GetCopiedEntity())
                            {
                                auto scene = Application::GetCurrent().GetCurrentScene();
                                Entity copiedEntity = { entity, scene };
                                if (!copiedEntity.IsValid())
                                {
                                    m_editorHandle->SetCopiedEntity(entt::null);
                                }
                                else
                                {
                                    scene->Duplicate(copiedEntity);

                                    if (m_editorHandle->GetCutCopyEntity())
                                    {
                                        copiedEntity.GetScene()->DestroyEntity(copiedEntity);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        ImGui::TextDisabled("Paste");
                    }

                    ImGui::Separator();

                    AddEntity();

                    ImGui::EndPopup();
                }
                {
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Drag_Entity"))
                        {
                            if (payload->DataSize != sizeof(entt::entity*)) { LOG_ERROR("Error ImGUI drag entity"); }
                            auto entity = *reinterpret_cast<entt::entity*>(payload->Data);
                            auto hierarchyComponent = registry.try_get<Hierarchy>(entity);
                            if (hierarchyComponent)
                            {
                                Hierarchy::Reparent(entity, entt::null, registry, *hierarchyComponent);
                            }
                        }
                        ImGui::EndDragDropTarget();
                    }

                   
                    ImGui::Indent();

                   
                    for (auto [entity] : registry.storage<entt::entity>().each())
                    {
                        if (registry.valid(entity))
                        {
                            auto hierarchyComponent = registry.try_get<Hierarchy>(entity);

                            if (!hierarchyComponent || hierarchyComponent->Parent() == entt::null)
                                DrawNode(entity, registry);
                        }
                    }

                    // Only supports one scene
                    ImVec2 min_space = ImGui::GetWindowContentRegionMin();
                    ImVec2 max_space = ImGui::GetWindowContentRegionMax();

                    float yOffset = Max(45.0f, ImGui::GetScrollY()); // Dont include search bar
                    min_space.x += ImGui::GetWindowPos().x + 1.0f;
                    min_space.y += ImGui::GetWindowPos().y + 1.0f + yOffset;
                    max_space.x += ImGui::GetWindowPos().x - 1.0f;
                    max_space.y += ImGui::GetWindowPos().y - 1.0f + ImGui::GetScrollY();
                    ImRect bb{ min_space, max_space };

                    const ImGuiPayload* payload = ImGui::GetDragDropPayload();
                    if (payload != NULL && payload->IsDataType("Drag_Entity"))
                    {
                        bool acceptable = false;

                        if (payload->DataSize != sizeof(entt::entity*)) {LOG_ERROR( "Error ImGUI drag entity"); }
                        auto entity = *reinterpret_cast<entt::entity*>(payload->Data);
                        auto hierarchyComponent = registry.try_get<Hierarchy>(entity);
                        if (hierarchyComponent)
                        {
                            acceptable = hierarchyComponent->Parent() != entt::null;
                        }

                        if (acceptable && ImGui::BeginDragDropTargetCustom(bb, ImGui::GetID("Panel Hierarchy")))
                        {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Drag_Entity"))
                            {
                                if (payload->DataSize != sizeof(entt::entity*)) { LOG_ERROR("Error ImGUI drag entity"); }
                                auto entity = *reinterpret_cast<entt::entity*>(payload->Data);
                                auto hierarchyComponent = registry.try_get<Hierarchy>(entity);
                                if (hierarchyComponent)
                                {
                                    Hierarchy::Reparent(entity, entt::null, registry, *hierarchyComponent);
                                    Entity e(entity, scene);
                                    e.RemoveComponent<Hierarchy>();
                                }
                            }
                            ImGui::EndDragDropTarget();
                        }
                    }
                }
                ImGui::EndChild();
            }
            ImGui::End();
		}
	
		bool HierarchyWindow::IsParentOfEntity(entt::entity entity, entt::entity child, entt::registry& registry)
		{
			auto nodeHierarchyComponent = registry.try_get<Hierarchy>(child);
			if (nodeHierarchyComponent)
			{
				auto parent = nodeHierarchyComponent->Parent();
				while (parent != entt::null)
				{
					if (parent == entity)
					{
						return true;
					}
					else
					{
						nodeHierarchyComponent = registry.try_get<Hierarchy>(parent);
						parent = nodeHierarchyComponent ? nodeHierarchyComponent->Parent() : entt::null;
					}
				}
			}

			return false;
		}
	}
}

