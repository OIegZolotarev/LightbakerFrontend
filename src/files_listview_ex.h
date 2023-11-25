/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "imgui_listview_ex.h"

#include "text_utils.h"

class FilesListViewBindings : public IListBinder
{
    TextUtils::AnsiStringList & m_Items;

    std::string m_SelectedItem;
    TextUtils::AnsiStringList::iterator m_Iterator;

    std::string m_DialogTitle;
    std::string m_FileFilter;

  public:
    FilesListViewBindings(TextUtils::AnsiStringList &container);

    void AddNewItem() override;

    bool IsEmpty() override;
    bool IsItemSelected() override;

    const char *ItemDescription() override;

    void MoveItemDown() override;
    void MoveItemUp() override;

    void ResetIterator() override;
    bool NextItem() override;
    void OpenItemEditor() override;

    void RemoveItem(size_t index) override;
    void RemoveSelectedItem() override;
    
    void SetSelectedItem() override;
    void SortItems(SortDirection dir) override;

    void SetDialogTitle(const char *title);
    void SetFileFilter(const char *fileFilter);

};
