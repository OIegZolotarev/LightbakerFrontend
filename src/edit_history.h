/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "object_props.h"
#include "scene_entity.h"

class IEditAction
{
protected:
	size_t m_SerialNumber;
public:
	IEditAction() = default;
	~IEditAction() = default;

	virtual void Undo() = 0;
	virtual void Redo() = 0;
};

class CPropertyChangeAction : public IEditAction
{
	VariantValue m_OldValue;
	VariantValue m_NewValue;

public:
    CPropertyChangeAction(size_t serialNumber, VariantValue oldValue, VariantValue newValue);
	
	void Redo() override;
	void Undo() override;
};

class CDeleteLightAction : public IEditAction
{
	SceneEntity* m_Object = nullptr;
public:
	CDeleteLightAction(SceneEntityPtr pObject);
	~CDeleteLightAction();

	void Redo() override;
	void Undo() override;
};

class EditTransaction: public IEditAction
{    
    std::list<IEditAction *> m_lstActions;
public:
    EditTransaction();
    ~EditTransaction();

    void Redo() override;
    void Undo() override;

	void AddAction(IEditAction * pAction)
    {
        m_lstActions.push_back(pAction);
	}
    
	bool Empty();
};

class CEditHistory
{
	std::list<IEditAction*> m_lstActions;
	ptrdiff_t m_Position = -1;
public:
	CEditHistory();
	~CEditHistory();

	void PushAction(IEditAction* pAction);

	void Undo();
	void Redo();
};


