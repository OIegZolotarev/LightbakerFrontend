/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "imgui_popups.h"
#include "object_props.h"
#include "ui_options_pages.h"
#include "imgui_listview_ex.h"

class GameConfigurationListBinding : public IListBinder
{
    GameConfigurationWeakPtr m_SelectedConf;
    std::list<GameConfigurationPtr>::iterator m_Iterator;

    std::list<GameConfigurationPtr> &m_ConfigurationItems;

  public:
    GameConfigurationListBinding();


    void MoveItemDown() override;
    void MoveItemUp() override;
    void OpenItemEditor() override;
  
    void AddNewItem() override;
    void RemoveItem(size_t index) override;
    
    void SortItems(SortDirection dir) override;

    
    bool IsItemSelected() override;
    const char *ItemDescription() override;
    bool NextItem() override;
    void ResetIterator() override;
    void SetSelectedItem() override;

    void RemoveSelectedItem() override;

    bool IsEmpty() override;

    void RenderExtraCommands() override;
};

class OptionsDialog : public IImGUIPopup
{
    float m_flScale         = 0;
    float m_flScaleOriginal = 0;

    
    ListViewEx *m_pGameConfigurationsView;

  public:
    OptionsDialog();
    ~OptionsDialog();

    void Render() override;
    void OnOpen() override;

    int RenderingFlags() override;

  private:
    void OnOkPressed();
    void OnCancelPressed();
    void RenderFooter();
    bool RenderHeader();

    void RenderOptionsPages(ProgramOptions::uiOptionPage_t *page);
    void RenderGameConfigurationsPage();
};