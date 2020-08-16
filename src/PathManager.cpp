#include "PathManager.h"
#include"Util.h"
#include <iostream>
#include "Config.h"
#include <algorithm>

void PathManager::GetShortestPath(PathNode* start, PathNode* goal)
{
	// std::cout << "Starting pathing..." << std::endl;
	s_path.clear(); // Clear the old saved path so we can save a new one.
	s_path.shrink_to_fit();
	m_pathCost = 0;
	NodeRecord* currentRecord = new NodeRecord(start);
	currentRecord->m_totalCost = start->H(); // Already we see an A* addition.
	s_open.push_back(currentRecord);
	while (s_open.size() > 0)
	{
		currentRecord = GetSmallestNode();
		//std::cout << "Doing node at: " << currentRecord->m_node->getTransform()->position.x << ',' << currentRecord->m_node->getTransform()->position.y << std::endl;
		//std::cout << "Found " << currentRecord->m_node->GetConnections().size() << " connections!" << std::endl;
		if (currentRecord->m_node == goal)
		{
			s_open.erase(std::remove(s_open.begin(), s_open.end(), currentRecord), s_open.end());
			s_closed.push_back(currentRecord);
			m_pathCost = currentRecord->m_totalCost / 32;
			break; // We found the goal!
		}
		std::vector<PathConnection*> connections = currentRecord->m_node->GetConnections();
		for (unsigned i = 0; i < connections.size(); i++)
		{
			// std::cout << "Doing connection " << i << std::endl;
			PathNode* endNode = connections[i]->GetToNode();
			NodeRecord* endNodeRecord;
			double endNodeCost = currentRecord->m_costSoFar + connections[i]->GetCost();

			if (ContainsNode(s_closed, endNode))
			{
				endNodeRecord = GetNodeRecord(s_closed, endNode);
				if (endNodeRecord->m_costSoFar <= endNodeCost)
					continue;
				s_closed.erase(std::remove(s_closed.begin(), s_closed.end(), endNodeRecord), s_closed.end());
				// Node gets pushed into open list below... lines 56/57.
			}
			else if (ContainsNode(s_open, endNode))
			{
				endNodeRecord = GetNodeRecord(s_open, endNode);
				if (endNodeRecord->m_costSoFar <= endNodeCost)
					continue;
			}
			else
			{
				endNodeRecord = new NodeRecord();
				endNodeRecord->m_node = endNode;
			}
			endNodeRecord->m_costSoFar = endNodeCost;
			endNodeRecord->m_connection = connections[i];
			endNodeRecord->m_fromRecord = currentRecord;
			endNodeRecord->m_totalCost = endNodeCost + endNode->H();
			if (!ContainsNode(s_open, endNode))
				s_open.push_back(endNodeRecord);
		}
		s_open.erase(std::remove(s_open.begin(), s_open.end(), currentRecord), s_open.end());
		s_closed.push_back(currentRecord);
		// std::cout << "Open size: " << s_open.size() << std::endl;
	}
	// std::cout << "Done while loop, open size:" << s_open.size() << std::endl;
	if (currentRecord->m_node != goal)
	{
		std::cout << "Could not find path!" << std::endl;
	}
	else
	{
		// std::cout << "Found goal at: [" << currentRecord->m_node->y / 32 << ',' << currentRecord->m_node->x / 32 << ']' << std::endl;
		while (currentRecord->m_node != start)
		{
			//std::cout << "Doing record for: [" << currentRecord->m_node->y/32 << ',' << currentRecord->m_node->x/32 << ']' << std::endl;
			s_path.push_back(currentRecord->m_connection);
			currentRecord = currentRecord->m_fromRecord;
		}
		std::reverse(s_path.begin(), s_path.end());
	}
	// Clean up lists. Uncomment the cout below to see how many records we have to clean up.
	//std::cout << "Cleaning up..." << "open size: " << s_open.size() << " | closed size: " << s_closed.size() << std::endl;
	//std::cout << "Path Size: "<<s_path.size() << std::endl;
	for (unsigned i = 0; i < s_open.size(); i++)
	{
		delete s_open[i];
		s_open[i] = nullptr; // Wrangle your dangle.
	}
	s_open.clear();
	s_open.shrink_to_fit();
	for (unsigned i = 0; i < s_closed.size(); i++)
	{
		delete s_closed[i];
		s_closed[i] = nullptr; // Wrangle your dangle.
	}
	s_closed.clear();
	s_closed.shrink_to_fit();
}

NodeRecord* PathManager::GetSmallestNode()
{
	std::vector<NodeRecord*>::iterator smallest = s_open.begin();
	std::vector<NodeRecord*>::iterator current = s_open.begin();

	while (++current != s_open.end())
	{
		if ((*current)->m_totalCost < (*smallest)->m_totalCost)
			smallest = current;
		else if ((*current)->m_totalCost == (*smallest)->m_totalCost) // If equal, flip a coin!
			smallest = (rand() % 2 ? current : smallest);
	}
	return (*smallest);
}

std::vector<NodeRecord*>& PathManager::OpenList()
{
	return s_open;
}

std::vector<NodeRecord*>& PathManager::ClosedList()
{
	return s_closed;
}

bool PathManager::ContainsNode(std::vector<NodeRecord*>& list, PathNode* n)
{
	for (unsigned i = 0; i < list.size(); i++)
		if (list[i]->m_node == n)
			return true;
	return false;
}

NodeRecord* PathManager::GetNodeRecord(std::vector<NodeRecord*>& list, PathNode* n)
{
	for (unsigned i = 0; i < list.size(); i++)
		if (list[i]->m_node == n)
			return list[i];
	return nullptr;
}

double PathManager::HEuclid( PathNode* start,  PathNode* goal)
{
	return Util::distance({start->getTransform()->position.x,start->getTransform()->position.y}, {goal-> getTransform()->position.x, goal->getTransform()->position.y});
}

double PathManager::HManhat( PathNode* start,  PathNode* goal)
{
	return (std::abs(start->getTransform()->position.x - goal->getTransform()->position.x) + std::abs(start->getTransform()->position.y - goal->getTransform()->position.y));
}


std::vector<PathConnection*> PathManager::getPath()
{
	return s_path;
}

void PathManager::DeletePath()
{
	s_path.clear();
	s_path.shrink_to_fit();
}

int PathManager::getCost()
{
	return m_pathCost;
}


std::vector<NodeRecord*> PathManager::s_open;
std::vector<NodeRecord*> PathManager::s_closed;

// Returned path to go here?
std::vector<PathConnection*> PathManager::s_path;
int PathManager::m_pathCost;