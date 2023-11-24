/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

enum class SortDirection
{
    Ascending = 0,
    Descending
};

class IListBinder
{
  public:
    virtual ~IListBinder() = default;

    virtual void AddNewItem()                 = 0;
    virtual void RemoveItem(size_t index)                 = 0;
    virtual void MoveItemUp()                 = 0;
    virtual void MoveItemDown()               = 0;
    virtual void OpenItemEditor()             = 0;
    virtual void SortItems(SortDirection dir) = 0;
    
    virtual size_t ItemsCount() = 0;
    virtual const char *ItemDescription(size_t index) = 0;

    virtual void SetSelectedItem(size_t index) = 0;
};

class ListViewEx
{
    IListBinder * m_pListBinder;
  public:
    ListViewEx(IListBinder *pBinder);

    ~ListViewEx();
    void RenderGui();
};