#pragma once

namespace ImGuiHelpers
{
template <class T> void EnumCombobox(T *value, const char *label);

template <class T> void EnumCombobox(T *value, const char *label)
{
    const char *v = value->_to_string();

    if (ImGui::BeginCombo(label, v))
    {
        for (auto &it : value->_values())
        {
            if (ImGui::Selectable(it._to_string(), it == value->_value, 0))
            {
                value->_value = it._value;
            }
        }

        ImGui::EndCombo();
    }
}

} // namespace ImGuiHelpers