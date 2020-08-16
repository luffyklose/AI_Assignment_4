#include "NodeManager.h"

std::vector<PathNode*> NodeManager::PathNodeVec;
std::vector<PathNode*> NodeManager::FleeNodeVec;

NodeManager::NodeManager()
{
}

NodeManager::~NodeManager()
{
}

std::vector<PathNode*> NodeManager::getPathNodeVec()
{
	return PathNodeVec;
}

std::vector<PathNode*> NodeManager::getFleeNodeVec()
{
	return FleeNodeVec;
}

void NodeManager::setFleeNode()
{
	PathNode* temp1 = new PathNode();
	temp1->getTransform()->position = glm::vec2(400, -100);
	AddFleeNode(temp1);
	temp1 = nullptr;

	PathNode* temp2 = new PathNode();
	temp2->getTransform()->position = glm::vec2(400, 700);
	AddFleeNode(temp2);
	temp2 = nullptr;

	PathNode* temp3 = new PathNode();
	temp3->getTransform()->position = glm::vec2(-100, 300);
	AddFleeNode(temp3);
	temp3 = nullptr;

	PathNode* temp4 = new PathNode();
	temp4->getTransform()->position = glm::vec2(900, 300);
	AddFleeNode(temp4);
	temp4 = nullptr;
}


void NodeManager::AddPathNode(PathNode* node)
{
	PathNodeVec.push_back(node);
}

void NodeManager::AddFleeNode(PathNode* node)
{
	FleeNodeVec.push_back(node);
}

