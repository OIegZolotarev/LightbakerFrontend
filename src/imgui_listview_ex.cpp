/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "imgui_listview_ex.h"

ListViewEx::ListViewEx(IListBinder *pBinder)
{
    m_pListBinder = pBinder;
}

ListViewEx::~ListViewEx()
{
}

void ListViewEx::RenderGui()
{
}
