#include "files_listview_ex.h"
#include "popup_file_dialog.h"

FilesListViewBindings::FilesListViewBindings(TextUtils::AnsiStringList &container) : m_Items(container)
{
}

void FilesListViewBindings::AddNewItem()
{
    auto lfd = FileDialog::Instance();

    lfd->SetTitle(m_DialogTitle.c_str());
    lfd->SetFilters(m_FileFilter.c_str());
    lfd->SetOnSelectCallback([&](std::string &fileName) { 
        
            auto it = std::find(m_Items.begin(), m_Items.end(), fileName);

            if (it == m_Items.end())
                m_Items.push_back(fileName); 
        
        });

    lfd->Show();

}

bool FilesListViewBindings::IsEmpty()
{
    return m_Items.empty();
}

bool FilesListViewBindings::IsItemSelected()
{
    return (*m_Iterator == m_SelectedItem);
}

const char *FilesListViewBindings::ItemDescription()
{
    return (*m_Iterator).c_str();
}

void FilesListViewBindings::MoveItemDown()
{
    auto it = std::find(m_Items.begin(), m_Items.end(), m_SelectedItem);

    TextUtils::AnsiStringList::iterator next_it;

    next_it = it;
    next_it++;

    if (m_Items.end() == next_it)
    {
        next_it = m_Items.begin();
    }

    std::iter_swap(it, next_it);
}

void FilesListViewBindings::MoveItemUp()
{
    TextUtils::AnsiStringList::iterator it = std::find(m_Items.begin(), m_Items.end(), m_SelectedItem);

    TextUtils::AnsiStringList::iterator next_it;

    if (it == m_Items.begin())
    {
        next_it = m_Items.end();
        next_it--;
    }
    else
    {
        next_it = it;
        next_it--;
    }

    std::iter_swap(it, next_it);
}

bool FilesListViewBindings::NextItem()
{
    m_Iterator++;
    return m_Iterator != m_Items.end();
}

void FilesListViewBindings::OpenItemEditor()
{
    // throw std::logic_error("The method or operation is not implemented.");
}

void FilesListViewBindings::RemoveItem(size_t index)
{
    auto it = m_Items.begin();
    std::advance(it, index);
    m_Items.erase(it);
}

void FilesListViewBindings::RemoveSelectedItem()
{
    // TODO: implement confirmation dialog and file removal

    m_Items.remove_if([&](std::string &ptr) -> bool { return m_SelectedItem == ptr; });
}

void FilesListViewBindings::ResetIterator()
{
    m_Iterator = m_Items.begin();
}

void FilesListViewBindings::SetSelectedItem()
{
    m_SelectedItem = *m_Iterator;
}

void FilesListViewBindings::SortItems(SortDirection dir)
{
    m_Items.sort([&](const std::string &a, const std::string &b) {
        switch (dir)
        {
        case SortDirection::Ascending:
            return strcmp(a.c_str(), b.c_str()) < 0;
            break;
        case SortDirection::Descending:
            return strcmp(a.c_str(), b.c_str()) > 0;
            break;
        }

        return false;
    });
}

void FilesListViewBindings::SetDialogTitle(const char *title)
{
    m_DialogTitle = title;
}

void FilesListViewBindings::SetFileFilter(const char *fileFilter)
{
    m_FileFilter = fileFilter;
}
