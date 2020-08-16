#include "Scene.h"
#include <iostream>
#include "DisplayObject.h"

Scene::Scene()
= default;

Scene::~Scene()
{
	removeAllChildren();
}


void Scene::addChild(DisplayObject * child)
{
	m_displayList.push_back(child);
}

void Scene::removeAllChildren()
{
	for (int count = 0; count < m_displayList.size(); ++count)
	{
		m_displayList[count] = nullptr;
	}

	m_displayList.clear();
	m_displayList.resize(0);
	m_displayList.shrink_to_fit();
}

void Scene::removeNullPointer()
{
	//m_displayList.erase(remove(m_displayList.begin(), m_displayList.end(), nullptr), m_displayList.end());
	for (auto obj = m_displayList.begin(); obj != m_displayList.end(); )
	{
		if (*obj == nullptr)
		{
			delete* obj;
			obj = m_displayList.erase(obj);
		}
		else
		{
			++obj;
		}
	}
}


int Scene::numberOfChildren() const
{
	return m_displayList.size();
}

void Scene::updateDisplayList()
{
	for (auto count = 0; count < m_displayList.size(); ++count)
	{
		if (m_displayList[count] != nullptr)
		{
			m_displayList[count]->update();
		}
	}	
}

void Scene::drawDisplayList()
{
	//std::cout << "Number: " << m_displayList.size() << std::endl;
	for (auto count = 0; count < m_displayList.size(); ++count)
	{
		if (m_displayList[count] != nullptr)
		{
			/*std::cout << count << std::endl;
			if(m_displayList[count]==nullptr)
			{
				std::cout << "cannot draw" << std::endl;
			}*/
			m_displayList[count]->draw();
		}
	}

}
