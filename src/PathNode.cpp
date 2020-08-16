#include "PathNode.h"


PathNode::PathNode()
{
	m_bLOS = false;
	setWidth(40);
	setHeight(40);
	setType(PATH_NODE);
}

PathNode::~PathNode()
{
	for (unsigned i = 0; i < m_connections.size(); i++)
	{
		delete m_connections[i];
		m_connections[i] = nullptr; // Wrangle your dangle.
	}
	m_connections.clear();
	m_connections.shrink_to_fit();
}

void PathNode::update()
{

}

void PathNode::draw()
{
	
}

void PathNode::clean()
{
	
}

void PathNode::setLOS(const bool a)
{
	m_bLOS = a;
}

bool PathNode::getLOS() const
{
	return m_bLOS;
}

void PathNode::AddConnection(PathConnection* c)
{
	m_connections.push_back(c);
}

std::vector<PathConnection*>& PathNode::GetConnections()
{
	return m_connections;
}

PathConnection::PathConnection(PathNode* f, PathNode* t, float cost) : m_cost(cost), m_pFromNode(f), m_pToNode(t) {}
