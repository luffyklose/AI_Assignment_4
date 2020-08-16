#include "DisplayObject.h"

DisplayObject::DisplayObject()
{
	m_bIsActive = false;
}

DisplayObject::~DisplayObject()
= default;

Scene * DisplayObject::getParent() const
{
	return m_pParentScene;
}

void DisplayObject::setParent(Scene * parent)
{
	m_pParentScene = parent;
}
