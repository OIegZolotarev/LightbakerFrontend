/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "imgui_popups.h"
#include "user_presentable_object.h"

typedef std::function<void(IUserPresentableObject*)> pfnSelectDialogCallback;

class SelectFromListDialog : public IImGUIPopup
{
    IUserPresentableObject *m_pSelected = nullptr;

    std::vector<IUserPresentableObject *> m_Items;
    pfnSelectDialogCallback m_Callback;

public:
    SelectFromListDialog();
    ~SelectFromListDialog();

    void Render() override;

    void SetCallback(pfnSelectDialogCallback callback);

    void AddItem(IUserPresentableObject *pObject);
    void ClearItems();
};
