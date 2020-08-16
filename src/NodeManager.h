#pragma once
#include <vector>
#include "PathNode.h"

class NodeManager
{
public:
	NodeManager();
	~NodeManager();
	static void AddPathNode(PathNode* node);
	static void AddFleeNode(PathNode* node);
	static void setFleeNode();

	//getter
	static std::vector<PathNode*> getPathNodeVec();
	static std::vector<PathNode*> getFleeNodeVec();
private:

public:

private:
	static std::vector<PathNode*> PathNodeVec;
	static std::vector<PathNode*> FleeNodeVec;
};

typedef NodeManager NDMA;