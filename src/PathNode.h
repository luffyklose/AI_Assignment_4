#pragma once
#include <vector>
#include "GameObject.h"

class PathConnection;

class PathNode:public GameObject
{
public:
	PathNode();
	~PathNode();

	virtual void update() override;
	virtual void draw() override;
	virtual void clean() override;

	void setLOS(bool a);
	bool getLOS() const;
	void AddConnection(PathConnection* c);
	std::vector<PathConnection*>& GetConnections();
	glm::vec2 Pt() { return { getTransform()->position.x, getTransform()->position.y }; }
	float H() { return m_h; } // Getter.
	void SetH(double h) { m_h = h; } // Setter.
private:

public:

private:
	bool m_bLOS;
	float m_h; // Heuristic cost for node.
	std::vector<PathConnection*> m_connections;
};

class PathConnection
{
public:
	PathConnection(PathNode* f, PathNode* t, float cost = 1.0);
	~PathConnection() {}
	double GetCost() { return m_cost; }
	void SetCost(float cost) { m_cost = cost; }
	PathNode* GetFromNode() { return m_pFromNode; }
	PathNode* GetToNode() { return m_pToNode; }
private:
	float m_cost;
	PathNode* m_pFromNode, * m_pToNode;
};

struct NodeRecord
{ // Everything public by default.
	NodeRecord(PathNode* n = nullptr) :m_node(n), m_connection(nullptr), m_fromRecord(nullptr), m_costSoFar(0.0), m_totalCost(0.0) {	}
	PathNode* m_node;
	PathConnection* m_connection;
	NodeRecord* m_fromRecord;
	float m_costSoFar;
	float m_totalCost;
};

struct KeyNode
{
	KeyNode(PathNode* n=nullptr,KeyNode* last=nullptr,KeyNode* next=nullptr):m_keyNode(n),m_lastNode(last),m_nextNode(next){}
	PathNode* m_keyNode;
	KeyNode *m_lastNode,
		*m_nextNode;
};

//class KeyNodeList
//{
//private:
//	KeyNode* head,
//		* tail;
//public:
//	KeyNodeList();
//	void AddKeyNode(KeyNode* newKeyNode);
//};