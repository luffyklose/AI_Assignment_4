#include "PortalManager.h"

PortalManager* PortalManager::s_pInstance = nullptr;

PortalManager::PortalManager()
{
}

PortalManager::~PortalManager()
{
}

void PortalManager::Init()
{
}

void PortalManager::exit()
{

	for (auto portal : m_CreatePortalVec)
	{
		delete portal;
		portal = nullptr;
	}
	for (auto portal : m_FleeProtalVec)
	{
		delete portal;
		portal = nullptr;
	}

	m_CreatePortalVec.clear();
	m_CreatePortalVec.shrink_to_fit();
	m_FleeProtalVec.clear();
	m_FleeProtalVec.shrink_to_fit();
}

void PortalManager::RemoveInvalid()
{
	for (auto portal = m_CreatePortalVec.begin(); portal != m_CreatePortalVec.end();)
	{
		if (!(*portal)->isActive())
		{
			portal = m_CreatePortalVec.erase(portal);
		}
		else
		{
			++portal;
		}
	}

	for (auto portal = m_FleeProtalVec.begin(); portal != m_FleeProtalVec.end();)
	{
		if (!(*portal)->isActive())
		{
			portal = m_FleeProtalVec.erase(portal);
		}
		else
		{
			++portal;
		}
	}
}

void PortalManager::CreateCreatePortal(glm::vec2 pos)
{
	auto portal = new Portal();
	portal->getTransform()->position = pos;
	m_CreatePortalVec.push_back(portal);
}

void PortalManager::CreateFleePortal(glm::vec2 pos)
{
	auto portal = new Portal();
	portal->getTransform()->position = pos;
	m_FleeProtalVec.push_back(portal);
}


