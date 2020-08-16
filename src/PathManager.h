#pragma once
#include "PathNode.h"
#include <vector>

class PathManager
{
public:
	static void GetShortestPath(PathNode* start, PathNode* goal);
	static NodeRecord* GetSmallestNode();
	static std::vector<NodeRecord*>& OpenList();
	static std::vector<NodeRecord*>& ClosedList();
	static bool ContainsNode(std::vector<NodeRecord*>& list, PathNode* n);
	static NodeRecord* GetNodeRecord(std::vector<NodeRecord*>& list, PathNode* n);
	static double HEuclid( PathNode* start,  PathNode* goal);
	static double HManhat( PathNode* start,  PathNode* goal);
	static std::vector < PathConnection* > getPath();
	static void DeletePath();
	static int getCost();
	
private:
	PathManager() {}
private:
	static std::vector<NodeRecord*> s_open;
	static std::vector<NodeRecord*> s_closed;
	static std::vector<PathConnection*> s_path;
	static int m_pathCost;
};

typedef PathManager PAMA;