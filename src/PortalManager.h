#pragma once
#include "Portal.h"

class PortalManager
{
public:
	static PortalManager* Instance()
	{
		if (s_pInstance == nullptr)
			s_pInstance = new PortalManager();
		return s_pInstance;
	}

	void Init();
	void exit();
	void RemoveInvalid();
	void CreateCreatePortal(glm::vec2 pos);
	void CreateFleePortal(glm::vec2 pos);

	std::vector<Portal*> getCreatePortalVec(){ return m_CreatePortalVec; }
	std::vector<Portal*> getFleePortalVec() { return m_FleeProtalVec; }
private:
	PortalManager();
	~PortalManager();

	std::vector<Portal*> m_CreatePortalVec;
	std::vector<Portal*>m_FleeProtalVec;

	static PortalManager* s_pInstance;
};

