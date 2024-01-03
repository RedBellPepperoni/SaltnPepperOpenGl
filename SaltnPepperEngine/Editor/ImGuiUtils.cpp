#include "ImGuiUtils.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Textures/CubeMap.h"
//#include "Engine/Utils/Maths/MathDefinitions.h"


namespace SaltnPepperEngine
{
    Vector4 SelectedColour = Vector4(0.28f, 0.56f, 0.9f, 1.0f);
    Vector4 IconColor = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
    static char* s_MultilineBuffer = nullptr;
    static uint32_t s_Counter = 0;
    static char s_IDBuffer[16] = "##";
    static char s_LabelIDBuffer[1024];
    static int s_UIContextID = 0;

    const char* ImGuiUtils::GenerateID()
    {
        //sprintf(s_IDBuffer + 2, "%x", s_Counter++);
        sprintf_s(s_IDBuffer + 2, 16, "%x", s_Counter++);

        //_itoa(s_Counter++, s_IDBuffer + 2, 16);
        return s_IDBuffer;
    }

    const char* ImGuiUtils::GenerateLabelID(std::string_view label)
    {
        *fmt::format_to_n(s_LabelIDBuffer, std::size(s_LabelIDBuffer), "{}##{}", label, s_Counter++).out = 0;
        return s_LabelIDBuffer;
    }

    void ImGuiUtils::PushID()
    {
        ImGui::PushID(s_UIContextID++);
        s_Counter = 0;
    }

    void ImGuiUtils::PopID()
    {
        ImGui::PopID();
        s_UIContextID--;
    }

    bool ImGuiUtils::ToggleButton(const char* label, bool state, ImVec2 size, float alpha, float pressedAlpha, ImGuiButtonFlags buttonFlags)
    {
        if (state)
        {
            ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];

            color.w = pressedAlpha;
            ImGui::PushStyleColor(ImGuiCol_Button, color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        }
        else
        {
            ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Button];
            ImVec4 hoveredColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            color.w = alpha;
            hoveredColor.w = pressedAlpha;
            ImGui::PushStyleColor(ImGuiCol_Button, color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
            color.w = pressedAlpha;
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        }

        bool clicked = ImGui::ButtonEx(label, size, buttonFlags);

        ImGui::PopStyleColor(3);

        return clicked;
    }

    bool ImGuiUtils::Property(const char* name, std::string& value, PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        ImGui::AlignTextToFramePadding();

        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::TextUnformatted(value.c_str());
        }
        else
        {
            if (ImGuiUtils::InputText(value))
            {
                updated = true;
            }
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    void ImGuiUtils::PropertyConst(const char* name, const char* value)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        ImGui::AlignTextToFramePadding();
        {
            ImGui::TextUnformatted(value);
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();
    }

    bool ImGuiUtils::PropertyMultiline(const char* label, std::string& value)
    {
        bool modified = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        ImGui::AlignTextToFramePadding();

        if (!s_MultilineBuffer)
        {
            s_MultilineBuffer = new char[1024 * 1024]; // 1KB
            memset(s_MultilineBuffer, 0, 1024 * 1024);
        }

       // strcpy(s_MultilineBuffer, value.c_str());

        strcpy_s(s_MultilineBuffer, 1024 * 1024, value.c_str());

        // std::string id = "##" + label;
        if (ImGui::InputTextMultiline(GenerateID(), s_MultilineBuffer, 1024 * 1024))
        {
            value = s_MultilineBuffer;
            modified = true;
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return modified;
    }

    bool ImGuiUtils::Property(const char* name, bool& value, PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::TextUnformatted(value ? "True" : "False");
        }
        else
        {
            // std::string id = "##" + std::string(name);
            if (ImGui::Checkbox(GenerateID(), &value))
                updated = true;
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    bool ImGuiUtils::Property(const char* name, int& value, ImGuiUtils::PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%i", value);
        }
        else
        {
            // std::string id = "##" + name;
            if (ImGui::DragInt(GenerateID(), &value))
                updated = true;
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    bool ImGuiUtils::Property(const char* name, uint32_t& value, ImGuiUtils::PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::Text("%d", value);
        }
        else
        {
            updated = ImGui::DragScalar(GenerateID(), ImGuiDataType_U32, &value);
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    bool ImGuiUtils::Property(const char* name, float& value, float min, float max, float delta, ImGuiUtils::PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::Text("%.2f", value);
        }
        else
        {
            // std::string id = "##" + name;
            if (ImGui::DragFloat(GenerateID(), &value, delta, min, max))
                updated = true;
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    bool ImGuiUtils::Property(const char* name, double& value, double min, double max, PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::Text("%.2f", (float)value);
        }
        else
        {
            // std::string id = "##" + name;
            if (ImGui::DragScalar(GenerateID(), ImGuiDataType_Double, &value))
                updated = true;
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    bool ImGuiUtils::Property(const char* name, int& value, int min, int max, PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::Text("%i", value);
        }
        else
        {
            // std::string id = "##" + name;
            if (ImGui::DragInt(GenerateID(), &value, 1, min, max))
                updated = true;
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    bool ImGuiUtils::Property(const char* name, Vector2& value, ImGuiUtils::PropertyFlag flags)
    {
        
        return ImGuiUtils::Property(name, value, -1.0f, 1.0f, flags);
    }

    bool ImGuiUtils::Property(const char* name, Vector2& value, float min, float max, ImGuiUtils::PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::Text("%.2f , %.2f", value.x, value.y);
        }
        else
        {
            // std::string id = "##" + name;
            if (ImGui::DragFloat2(GenerateID(), glm::value_ptr(value)))
                updated = true;
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    bool ImGuiUtils::Property(const char* name, glm::vec3& value, ImGuiUtils::PropertyFlag flags)
    {
        
        return ImGuiUtils::Property(name, value, -1.0f, 1.0f, flags);
    }

    bool ImGuiUtils::Property(const char* name, glm::vec3& value, float min, float max, ImGuiUtils::PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::Text("%.2f , %.2f, %.2f", value.x, value.y, value.z);
        }
        else
        {
            // std::string id = "##" + name;
            if ((int)flags & (int)PropertyFlag::ColourProperty)
            {
                if (ImGui::ColorEdit3(GenerateID(), glm::value_ptr(value)))
                    updated = true;
            }
            else
            {
                if (ImGui::DragFloat3(GenerateID(), glm::value_ptr(value)))
                    updated = true;
            }
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    bool ImGuiUtils::Property(const char* name, Vector4& value, bool exposeW, ImGuiUtils::PropertyFlag flags)
    {   
        return Property(name, value, -1.0f, 1.0f, exposeW, flags);
    }

    bool ImGuiUtils::Property(const char* name, Vector4& value, float min, float max, bool exposeW, ImGuiUtils::PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::Text("%.2f , %.2f, %.2f , %.2f", value.x, value.y, value.z, value.w);
        }
        else
        {

            // std::string id = "##" + name;
            if ((int)flags & (int)PropertyFlag::ColourProperty)
            {
                if (ImGui::ColorEdit4(GenerateID(), glm::value_ptr(value)))
                    updated = true;
            }
            else if ((exposeW ? ImGui::DragFloat4(GenerateID(), glm::value_ptr(value)) : ImGui::DragFloat4(GenerateID(), glm::value_ptr(value))))
                updated = true;
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    bool ImGuiUtils::PropertyTransform(const char* name, glm::vec3& vector, float width)
    {
        const float labelIndetation = ImGui::GetFontSize();
        bool updated = false;

        auto& style = ImGui::GetStyle();

        const auto showFloat = [&](int axis, float* value)
            {
                const float label_float_spacing = ImGui::GetFontSize();
                const float step = 0.01f;
                static const std::string format = "%.4f";

                ImGui::AlignTextToFramePadding();
                ImGui::TextUnformatted(axis == 0 ? "X" : axis == 1 ? "Y"
                    : "Z");
                ImGui::SameLine(label_float_spacing);

                ImVec2 imPos = ImGui::GetCursorScreenPos();

                Vector2 posPostLabel = Vector2(imPos.x, imPos.y);

                ImGui::PushItemWidth(width);
                ImGui::PushID(static_cast<int>(ImGui::GetCursorPosX() + ImGui::GetCursorPosY()));

                if (ImGui::InputFloat("##no_label", value, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), format.c_str()))
                    updated = true;

                ImGui::PopID();
                ImGui::PopItemWidth();

                static const ImU32 colourX = IM_COL32(168, 46, 2, 255);
                static const ImU32 colourY = IM_COL32(112, 162, 22, 255);
                static const ImU32 colourZ = IM_COL32(51, 122, 210, 255);

                const Vector2 size = Vector2(ImGui::GetFontSize() / 4.0f, ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y);
                posPostLabel = posPostLabel + Vector2(-1.0f, ImGui::GetStyle().FramePadding.y / 2.0f);
                ImRect axis_color_rect = ImRect(posPostLabel.x, posPostLabel.y, posPostLabel.x + size.x, posPostLabel.y + size.y);
                ImGui::GetWindowDrawList()->AddRectFilled(axis_color_rect.Min, axis_color_rect.Max, axis == 0 ? colourX : axis == 1 ? colourY
                    : colourZ);
            };

        ImGui::BeginGroup();
        ImGui::Indent(labelIndetation);
        ImGui::TextUnformatted(name);
        ImGui::Unindent(labelIndetation);
        showFloat(0, &vector.x);
        showFloat(1, &vector.y);
        showFloat(2, &vector.z);
        ImGui::EndGroup();

        return updated;
    }

    bool ImGuiUtils::Property(const char* name, glm::quat& value, ImGuiUtils::PropertyFlag flags)
    {
        
        bool updated = false;

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        if ((int)flags & (int)PropertyFlag::ReadOnly)
        {
            ImGui::Text("%.2f , %.2f, %.2f , %.2f", value.x, value.y, value.z, value.w);
        }
        else
        {

            // std::string id = "##" + name;
            if (ImGui::DragFloat4(GenerateID(), glm::value_ptr(value)))
                updated = true;
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return updated;
    }

    void ImGuiUtils::Tooltip(const char* text)
    {
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted(text);
            ImGui::EndTooltip();
        }

        ImGui::PopStyleVar();
    }

    void ImGuiUtils::Tooltip(Texture* texture, const Vector2& size)
    {
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();

            // Keeping it forced true for OpenGL
            bool flipImage = true;

            ImGui::Image(texture ? (void*)texture->GetHandle() : nullptr, ImVec2(size.x, size.y), ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
            ImGui::EndTooltip();
        }

        ImGui::PopStyleVar();
    }

    void ImGuiUtils::Tooltip(Texture* texture, const Vector2& size, const char* text)
    {
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();

            // Keeping it forced true for OpenGL
            bool flipImage = true;
            ImGui::Image(texture ? (void*)texture->GetHandle() : nullptr, ImVec2(size.x, size.y), ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
            ImGui::TextUnformatted(text);
            ImGui::EndTooltip();
        }

        ImGui::PopStyleVar();
    }

    
    void ImGuiUtils::Image(Texture* texture, const Vector2& size)
    {
        
        bool flipImage = true;
        ImGui::Image(texture ? (void*)texture->GetHandle() : nullptr, ImVec2(size.x, size.y), ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
    }

    void ImGuiUtils::Image(CubeMap* texture, const Vector2& size)
    {
        
        bool flipImage = true;
        ImGui::Image(texture ? (void*)texture->GetHandle() : nullptr, ImVec2(size.x, size.y), ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
    }

   
    bool ImGuiUtils::BufferingBar(const char* label, float value, const Vector2& size_arg, const uint32_t& bg_col, const uint32_t& fg_col)
    {
        
        auto g = ImGui::GetCurrentContext();
        auto drawList = ImGui::GetWindowDrawList();
        const ImGuiStyle& style = ImGui::GetStyle();
        const ImGuiID id = ImGui::GetID(label);

        ImVec2 pos = ImGui::GetCursorPos();
        ImVec2 size = { size_arg.x, size_arg.y };
        size.x -= style.FramePadding.x * 2;

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return false;

        // Render
        const float circleStart = size.x * 0.7f;
        const float circleEnd = size.x;
        const float circleWidth = circleEnd - circleStart;

        drawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
        drawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

        const float t = float(g->Time);
        const float r = size.y * 0.5f;
        const float speed = 1.5f;

        const float a = speed * 0.f;
        const float b = speed * 0.333f;
        const float c = speed * 0.666f;

        const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
        const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
        const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

        drawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
        drawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
        drawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

        return true;
    }

    bool ImGuiUtils::Spinner(const char* label, float radius, int thickness, const uint32_t& colour)
    {
        
        auto g = ImGui::GetCurrentContext();
        const ImGuiStyle& style = g->Style;
        const ImGuiID id = ImGui::GetID(label);
        auto drawList = ImGui::GetWindowDrawList();

        ImVec2 pos = ImGui::GetCursorPos();
        ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return false;

        // Render
        drawList->PathClear();

        int num_segments = 30;
        float start = abs(ImSin(float(g->Time) * 1.8f) * (num_segments - 5));

        const float a_min = IM_PI * 2.0f * (start / float(num_segments));
        const float a_max = IM_PI * 2.0f * (float(num_segments) - 3.0f) / (float)num_segments;

        const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

        for (int i = 0; i < num_segments; i++)
        {
            const float a = a_min + (float(i) / float(num_segments)) * (a_max - a_min);
            drawList->PathLineTo(ImVec2(centre.x + ImCos(a + float(g->Time) * 8) * radius,
                centre.y + ImSin(a + float(g->Time) * 8) * radius));
        }

        drawList->PathStroke(colour, false, float(thickness));

        return true;
    }

    void ImGuiUtils::DrawRowsBackground(int row_count, float line_height, float x1, float x2, float y_offset, ImU32 col_even, ImU32 col_odd)
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        float y0 = ImGui::GetCursorScreenPos().y + (float)(int)y_offset;

        int row_display_start = 0;
        int row_display_end = 0;
        // ImGui::CalcListClipping(row_count, line_height, &row_display_start, &row_display_end);
        for (int row_n = row_display_start; row_n < row_display_end; row_n++)
        {
            ImU32 col = (row_n & 1) ? col_odd : col_even;
            if ((col & IM_COL32_A_MASK) == 0)
                continue;
            float y1 = y0 + (line_height * row_n);
            float y2 = y1 + line_height;
            draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col);
        }
    }

    void ImGuiUtils::TextCentred(const char* text)
    {
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth = ImGui::CalcTextSize(text).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::TextUnformatted(text);
    }

    void ImGuiUtils::SetTheme(Theme theme)
    {
        static const float max = 255.0f;

        auto& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;
        SelectedColour = Vector4(0.28f, 0.56f, 0.9f, 1.0f);

        // colours[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        // colours[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

        if (theme == Black)
        {
            ImGui::StyleColorsDark();
            colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
            colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
            colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
            colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
            colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
            colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
            colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
            colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
            colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
            colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
            colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
            colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
            colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
            colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
            colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
            colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
            colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
            colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
            colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
            colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
            colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        }
        else if (theme == Dark)
        {
            ImGui::StyleColorsDark();
            ImVec4 Titlebar = ImVec4(40.0f / max, 42.0f / max, 54.0f / max, 1.0f);
            ImVec4 TabActive = ImVec4(52.0f / max, 54.0f / max, 64.0f / max, 1.0f);
            ImVec4 TabUnactive = ImVec4(35.0f / max, 43.0f / max, 59.0f / max, 1.0f);

            ImVec4 imColor = ImVec4(155.0f / 255.0f, 130.0f / 255.0f, 207.0f / 255.0f, 1.00f);

            SelectedColour = Vector4(imColor.x, imColor.y, imColor.z, imColor.w);
            colors[ImGuiCol_Text] = ImVec4(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.00f);
            colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);


            imColor = colors[ImGuiCol_Text];
            IconColor = Vector4(imColor.x, imColor.y, imColor.z, imColor.w);
            colors[ImGuiCol_WindowBg] = TabActive;
            colors[ImGuiCol_ChildBg] = TabActive;

            colors[ImGuiCol_PopupBg] = ImVec4(42.0f / 255.0f, 38.0f / 255.0f, 47.0f / 255.0f, 1.00f);
            colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
            colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_FrameBg] = ImVec4(65.0f / 255.0f, 79.0f / 255.0f, 92.0f / 255.0f, 1.00f);
            colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
            colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);

            colors[ImGuiCol_TitleBg] = Titlebar;
            colors[ImGuiCol_TitleBgActive] = Titlebar;
            colors[ImGuiCol_TitleBgCollapsed] = Titlebar;
            colors[ImGuiCol_MenuBarBg] = Titlebar;

            colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.8f, 0.8f, 0.8f, 1.00f);

            colors[ImGuiCol_CheckMark] = ImVec4(155.0f / 255.0f, 130.0f / 255.0f, 207.0f / 255.0f, 1.00f);
            colors[ImGuiCol_SliderGrab] = ImVec4(155.0f / 255.0f, 130.0f / 255.0f, 207.0f / 255.0f, 1.00f);
            colors[ImGuiCol_SliderGrabActive] = ImVec4(185.0f / 255.0f, 160.0f / 255.0f, 237.0f / 255.0f, 1.00f);
            colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
            colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f) + ImVec4(0.1f, 0.1f, 0.1f, 0.1f);
            colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f) + ImVec4(0.1f, 0.1f, 0.1f, 0.1f);

            colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
            colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
            colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);

            colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);

            colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

            colors[ImGuiCol_Header] = TabActive + ImVec4(0.1f, 0.1f, 0.1f, 0.1f);
            colors[ImGuiCol_HeaderHovered] = TabActive + ImVec4(0.1f, 0.1f, 0.1f, 0.1f);
            colors[ImGuiCol_HeaderActive] = TabActive + ImVec4(0.05f, 0.05f, 0.05f, 0.1f);

#ifdef IMGUI_HAS_DOCK

            colors[ImGuiCol_Tab] = TabUnactive;
            colors[ImGuiCol_TabHovered] = TabActive + ImVec4(0.1f, 0.1f, 0.1f, 0.1f);
            colors[ImGuiCol_TabActive] = TabActive;
            colors[ImGuiCol_TabUnfocused] = TabUnactive;
            colors[ImGuiCol_TabUnfocusedActive] = TabActive;
            colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
            colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);

#endif
        }
        
        else if (theme == Light)
        {
            ImGui::StyleColorsLight();
            colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);

            ImVec4 imColor = colors[ImGuiCol_Text];
            IconColor = Vector4(imColor.x, imColor.y, imColor.z, imColor.w);

            colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
            colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
            colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
            colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
            colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
            colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
            colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
            colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
            colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
            colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
            colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
            colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        }
        else if (theme == Cherry)
        {
            ImGui::StyleColorsDark();
#define HI(v) ImVec4(0.502f, 0.075f, 0.256f, v)
#define MED(v) ImVec4(0.455f, 0.198f, 0.301f, v)
#define LOW(v) ImVec4(0.232f, 0.201f, 0.271f, v)
#define BG(v) ImVec4(0.200f, 0.220f, 0.270f, v)
#define TEXTCol(v) ImVec4(0.860f, 0.930f, 0.890f, v)

            colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
            colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);

            ImVec4 imColor = colors[ImGuiCol_Text];
            IconColor = Vector4(imColor.x, imColor.y, imColor.z, imColor.w);
            colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
            colors[ImGuiCol_PopupBg] = BG(0.9f);
            colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
            colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_FrameBg] = BG(1.00f);
            colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
            colors[ImGuiCol_FrameBgActive] = MED(1.00f);
            colors[ImGuiCol_TitleBg] = LOW(1.00f);
            colors[ImGuiCol_TitleBgActive] = HI(1.00f);
            colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
            colors[ImGuiCol_MenuBarBg] = BG(0.47f);
            colors[ImGuiCol_ScrollbarBg] = BG(1.00f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
            colors[ImGuiCol_ScrollbarGrabActive] = MED(1.00f);
            colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
            colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
            colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
            colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
            colors[ImGuiCol_ButtonHovered] = MED(0.86f);
            colors[ImGuiCol_ButtonActive] = MED(1.00f);
            colors[ImGuiCol_Header] = MED(0.76f);
            colors[ImGuiCol_HeaderHovered] = MED(0.86f);
            colors[ImGuiCol_HeaderActive] = HI(1.00f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
            colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
            colors[ImGuiCol_ResizeGripActive] = MED(1.00f);
            colors[ImGuiCol_PlotLines] = TEXTCol(0.63f);
            colors[ImGuiCol_PlotLinesHovered] = MED(1.00f);
            colors[ImGuiCol_PlotHistogram] = TEXTCol(0.63f);
            colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
            colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
            colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
            colors[ImGuiCol_TabHovered] = colors[ImGuiCol_ButtonHovered];
        }
        
        else if (theme == Classic)
        {
            ImGui::StyleColorsClassic();

            ImVec4 imColor = colors[ImGuiCol_Text];
            IconColor = Vector4(imColor.x, imColor.y, imColor.z, imColor.w);
        }
       

        colors[ImGuiCol_Separator] = colors[ImGuiCol_TitleBg];
        colors[ImGuiCol_SeparatorActive] = colors[ImGuiCol_Separator];
        colors[ImGuiCol_SeparatorHovered] = colors[ImGuiCol_Separator];

        colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_WindowBg];
        colors[ImGuiCol_TabActive] = colors[ImGuiCol_WindowBg];
        colors[ImGuiCol_ChildBg] = colors[ImGuiCol_TabActive];
        colors[ImGuiCol_ScrollbarBg] = colors[ImGuiCol_TabActive];
        colors[ImGuiCol_TableHeaderBg] = colors[ImGuiCol_TabActive];

        colors[ImGuiCol_TitleBgActive] = colors[ImGuiCol_TitleBg];
        colors[ImGuiCol_TitleBgCollapsed] = colors[ImGuiCol_TitleBg];
        colors[ImGuiCol_MenuBarBg] = colors[ImGuiCol_TitleBg];
        colors[ImGuiCol_PopupBg] = colors[ImGuiCol_WindowBg] + ImVec4(0.05f, 0.05f, 0.05f, 0.0f);

        colors[ImGuiCol_Tab] = colors[ImGuiCol_MenuBarBg];
        colors[ImGuiCol_TabUnfocused] = colors[ImGuiCol_MenuBarBg];

        colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 0.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    }

    Vector4 ImGuiUtils::GetSelectedColour()
    {
        return SelectedColour;
    }

    Vector4 ImGuiUtils::GetIconColour()
    {
        return IconColor;
    }

    bool ImGuiUtils::PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected)
    {
        const char* current = options[*selected];
        ImGui::TextUnformatted(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        auto drawItemActivityOutline = [](float rounding = 0.0f, bool drawWhenInactive = false)
            {
                auto* drawList = ImGui::GetWindowDrawList();
                if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
                {
                    drawList->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                        ImColor(60, 60, 60), rounding, 0, 1.5f);
                }
                if (ImGui::IsItemActive())
                {
                    drawList->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                        ImColor(80, 80, 80), rounding, 0, 1.0f);
                }
                else if (!ImGui::IsItemHovered() && drawWhenInactive)
                {
                    drawList->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                        ImColor(50, 50, 50), rounding, 0, 1.0f);
                }
            };

        bool changed = false;

        // const std::string id = "##" + std::string(label);

        if (ImGui::BeginCombo(GenerateID(), current))
        {
            for (int i = 0; i < optionCount; i++)
            {
                const bool is_selected = (current == options[i]);
                if (ImGui::Selectable(options[i], is_selected))
                {
                    current = options[i];
                    *selected = i;
                    changed = true;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        drawItemActivityOutline(2.5f);

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return changed;
    }

    void ImGuiUtils::DrawItemActivityOutline(float rounding, bool drawWhenInactive, ImColor colourWhenActive)
    {
        auto* drawList = ImGui::GetWindowDrawList();

        ImRect expandedRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
        expandedRect.Min.x -= 1.0f;
        expandedRect.Min.y -= 1.0f;
        expandedRect.Max.x += 1.0f;
        expandedRect.Max.y += 1.0f;

        const ImRect rect = expandedRect;
        if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
        {
            drawList->AddRect(rect.Min, rect.Max,
                ImColor(60, 60, 60), rounding, 0, 1.5f);
        }
        if (ImGui::IsItemActive())
        {
            drawList->AddRect(rect.Min, rect.Max,
                colourWhenActive, rounding, 0, 1.0f);
        }
        else if (!ImGui::IsItemHovered() && drawWhenInactive)
        {
            drawList->AddRect(rect.Min, rect.Max,
                ImColor(50, 50, 50), rounding, 0, 1.0f);
        }
    }

    bool ImGuiUtils::InputText(std::string& currentText)
    {
        ImGuiUtils::ScopedStyle frameBorder(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGuiUtils::ScopedColour frameColour(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
        char buffer[256];
        memset(buffer, 0, 256);
        memcpy(buffer, currentText.c_str(), currentText.length());
        ImGui::PushID(currentText.c_str());
        bool updated = ImGui::InputText("##SceneName", buffer, 256);

        ImGuiUtils::DrawItemActivityOutline(2.0f, false);

        if (updated)
            currentText = std::string(buffer);
        ImGui::PopID();
        return updated;
    }

    void ImGuiUtils::ClippedText(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect, float wrap_width)
    {
        const char* text_display_end = ImGui::FindRenderedTextEnd(text, text_end);
        const int text_len = static_cast<int>(text_display_end - text);
        if (text_len == 0)
            return;

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGuiUtils::ClippedText(window->DrawList, pos_min, pos_max, text, text_display_end, text_size_if_known, align, clip_rect, wrap_width);
        if (g.LogEnabled)
            ImGui::LogRenderedText(&pos_min, text, text_display_end);
    }

    void ImGuiUtils::ClippedText(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_display_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect, float wrap_width)
    {
        // Perform CPU side clipping for single clipped element to avoid using scissor state
        ImVec2 pos = pos_min;
        const ImVec2 text_size = text_size_if_known ? *text_size_if_known : ImGui::CalcTextSize(text, text_display_end, false, wrap_width);

        const ImVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
        const ImVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;

        // Align whole block. We should defer that to the better rendering function when we'll have support for individual line alignment.
        if (align.x > 0.0f)
            pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);

        if (align.y > 0.0f)
            pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

        // Render
        ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
        draw_list->AddText(nullptr, 0.0f, pos, ImGui::GetColorU32(ImGuiCol_Text), text, text_display_end, wrap_width, &fine_clip_rect);
    }

    // from https://github.com/ocornut/imgui/issues/2668
    void ImGuiUtils::AlternatingRowsBackground(float lineHeight)
    {
        const ImU32 im_color = ImGui::GetColorU32(ImGuiCol_TableRowBgAlt);

        auto* draw_list = ImGui::GetWindowDrawList();
        const auto& style = ImGui::GetStyle();

        if (lineHeight < 0)
        {
            lineHeight = ImGui::GetTextLineHeight();
        }

        lineHeight += style.ItemSpacing.y;

        float scroll_offset_h = ImGui::GetScrollX();
        float scroll_offset_v = ImGui::GetScrollY();
        float scrolled_out_lines = std::floor(scroll_offset_v / lineHeight);
        scroll_offset_v -= lineHeight * scrolled_out_lines;

        ImVec2 clip_rect_min(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
        ImVec2 clip_rect_max(clip_rect_min.x + ImGui::GetWindowWidth(), clip_rect_min.y + ImGui::GetWindowHeight());

        if (ImGui::GetScrollMaxX() > 0)
        {
            clip_rect_max.y -= style.ScrollbarSize;
        }

        draw_list->PushClipRect(clip_rect_min, clip_rect_max);

        const float y_min = clip_rect_min.y - scroll_offset_v + ImGui::GetCursorPosY();
        const float y_max = clip_rect_max.y - scroll_offset_v + lineHeight;
        const float x_min = clip_rect_min.x + scroll_offset_h + ImGui::GetWindowContentRegionMin().x;
        const float x_max = clip_rect_min.x + scroll_offset_h + ImGui::GetWindowContentRegionMax().x;

        bool is_odd = (static_cast<int>(scrolled_out_lines) % 2) == 0;
        for (float y = y_min; y < y_max; y += lineHeight, is_odd = !is_odd)
        {
            if (is_odd)
            {
                draw_list->AddRectFilled({ x_min, y - style.ItemSpacing.y }, { x_max, y + lineHeight }, im_color);
            }
        }

        draw_list->PopClipRect();
    }

    ImRect ImGuiUtils::GetItemRect()
    {
        return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    }

    ImRect ImGuiUtils::RectExpanded(const ImRect& rect, float x, float y)
    {
        ImRect result = rect;
        result.Min.x -= x;
        result.Min.y -= y;
        result.Max.x += x;
        result.Max.y += y;
        return result;
    }

    ImRect ImGuiUtils::RectOffset(const ImRect& rect, float x, float y)
    {
        ImRect result = rect;
        result.Min.x += x;
        result.Min.y += y;
        result.Max.x += x;
        result.Max.y += y;
        return result;
    }

    ImRect ImGuiUtils::RectOffset(const ImRect& rect, ImVec2 xy)
    {
        return RectOffset(rect, xy.x, xy.y);
    }

    void ImGuiUtils::DrawBorder(ImVec2 rectMin, ImVec2 rectMax, const ImVec4& borderColour, float thickness, float offsetX, float offsetY)
    {
        auto min = rectMin;
        min.x -= thickness;
        min.y -= thickness;
        min.x += offsetX;
        min.y += offsetY;
        auto max = rectMax;
        max.x += thickness;
        max.y += thickness;
        max.x += offsetX;
        max.y += offsetY;

        auto* drawList = ImGui::GetWindowDrawList();
        drawList->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(borderColour), 0.0f, 0, thickness);
    }

    void ImGuiUtils::DrawBorder(const ImVec4& borderColour, float thickness, float offsetX, float offsetY)
    {
        DrawBorder(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), borderColour, thickness, offsetX, offsetY);
    }

    void ImGuiUtils::DrawBorder(float thickness, float offsetX, float offsetY)
    {
        DrawBorder(ImGui::GetStyleColorVec4(ImGuiCol_Border), thickness, offsetX, offsetY);
    }

    void ImGuiUtils::DrawBorder(ImVec2 rectMin, ImVec2 rectMax, float thickness, float offsetX, float offsetY)
    {
        DrawBorder(rectMin, rectMax, ImGui::GetStyleColorVec4(ImGuiCol_Border), thickness, offsetX, offsetY);
    }

    void ImGuiUtils::DrawBorder(ImRect rect, float thickness, float rounding, float offsetX, float offsetY)
    {
        auto min = rect.Min;
        min.x -= thickness;
        min.y -= thickness;
        min.x += offsetX;
        min.y += offsetY;
        auto max = rect.Max;
        max.x += thickness;
        max.y += thickness;
        max.x += offsetX;
        max.y += offsetY;

        auto* drawList = ImGui::GetWindowDrawList();
        drawList->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Border)), rounding, 0, thickness);
    }
}

namespace ImGui
{
    bool DragFloatN_Coloured(const char* label, float* v, int components, float v_speed, float v_min, float v_max, const char* display_format)
    {
        
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        bool value_changed = false;
        BeginGroup();
        PushID(label);
        PushMultiItemsWidths(components, CalcItemWidth());
        for (int i = 0; i < components; i++)
        {
            static const ImU32 colours[] = {
                0xBB0000FF, // red
                0xBB00FF00, // green
                0xBBFF0000, // blue
                0xBBFFFFFF, // white for alpha?
            };

            PushID(i);
            value_changed |= DragFloat("##v", &v[i], v_speed, v_min, v_max, display_format);

            const ImVec2 min = GetItemRectMin();
            const ImVec2 max = GetItemRectMax();
            const float spacing = g.Style.FrameRounding;
            const float halfSpacing = spacing / 2;

            // This is the main change
            window->DrawList->AddLine({ min.x + spacing, max.y - halfSpacing }, { max.x - spacing, max.y - halfSpacing }, colours[i], 4);

            SameLine(0, g.Style.ItemInnerSpacing.x);
            PopID();
            PopItemWidth();
        }
        PopID();

        TextUnformatted(label, FindRenderedTextEnd(label));
        EndGroup();

        return value_changed;
    }

    void PushMultiItemsWidthsAndLabels(const char* labels[], int components, float w_full)
    {
        ImGuiWindow* window = GetCurrentWindow();
        const ImGuiStyle& style = GImGui->Style;
        if (w_full <= 0.0f)
            w_full = GetContentRegionAvail().x;

        const float w_item_one = ImMax(1.0f, (w_full - (style.ItemInnerSpacing.x * 2.0f) * (components - 1)) / (float)components) - style.ItemInnerSpacing.x;
        for (int i = 0; i < components; i++)
            window->DC.ItemWidthStack.push_back(w_item_one - CalcTextSize(labels[i]).x);
        window->DC.ItemWidth = window->DC.ItemWidthStack.back();
    }

    bool DragFloatNEx(const char* labels[], float* v, int components, float v_speed, float v_min, float v_max,
        const char* display_format)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        bool value_changed = false;
        BeginGroup();

        PushMultiItemsWidthsAndLabels(labels, components, 0.0f);
        for (int i = 0; i < components; i++)
        {
            PushID(labels[i]);
            PushID(i);
            TextUnformatted(labels[i], FindRenderedTextEnd(labels[i]));
            SameLine();
            value_changed |= DragFloat("", &v[i], v_speed, v_min, v_max, display_format);
            SameLine(0, g.Style.ItemInnerSpacing.x);
            PopID();
            PopID();
            PopItemWidth();
        }

        EndGroup();

        return value_changed;
    }

    bool DragFloat3Coloured(const char* label, float* v, float v_speed, float v_min, float v_max)
    {
        
        return DragFloatN_Coloured(label, v, 3, v_speed, v_min, v_max);
    }

    bool DragFloat4Coloured(const char* label, float* v, float v_speed, float v_min, float v_max)
    {
        
        return DragFloatN_Coloured(label, v, 4, v_speed, v_min, v_max);
    }

    bool DragFloat2Coloured(const char* label, float* v, float v_speed, float v_min, float v_max)
    {
        
        return DragFloatN_Coloured(label, v, 2, v_speed, v_min, v_max);
    }
}
