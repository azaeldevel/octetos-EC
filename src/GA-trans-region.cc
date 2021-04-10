#include "GA-trans.hh"

namespace ae::ga
{

void TransEnviroment::creteRegion()
{
	region = new nodes::Region(++countID,"Tesing");
	nodes::Target* target1;
	nodes::Node* nodeA;
	nodes::Node* nodeB;
	nodes::Edge* edge;

	nodeA = region->newStreet(++countID);//O
	nodeB = region->newStreet(++countID);//66
	region->newEdge(600,nodeA,nodeB);
	nodes::Node* node_66 = nodeB;

	nodeA = nodeB;//66
	nodeB = region->newStreet(++countID);//2
	region->newEdge(200,nodeA,nodeB);

	nodeA = nodeB;//2
	nodeB = region->newStreet(++countID);//3
	region->newEdge(500,nodeA,nodeB);
	
	nodeA = nodeB;//3
	nodeB = region->newStreet(++countID);//8
	region->newEdge(600,nodeA,nodeB);
	
	nodeA = nodeB;//8
	nodeB = region->newStreet(++countID);//9
	region->newEdge(400,nodeA,nodeB);
	
	nodeA = nodeB;//9
	nodeB = region->newStreet(++countID);//10
	region->newEdge(300,nodeA,nodeB);
	
	nodeA = nodeB;//10
	nodeB = region->newStreet(++countID);//11
	region->newEdge(400,nodeA,nodeB);
	
	nodeA = nodeB;//11
	nodeB = region->newStreet(++countID);//41
	region->newEdge(300,nodeA,nodeB);
		
	nodeA = node_66;//66
	nodeB = region->newStreet(++countID);//67
	region->newEdge(300,nodeA,nodeB);	
		
	nodeA = nodeB;//67
	nodeB = region->newStreet(++countID);//69
	region->newEdge(300,nodeA,nodeB);
		
	nodeA = nodeB;//69
	nodeB = region->newStreet(++countID);//73
	region->newEdge(200,nodeA,nodeB);
	nodes::Node* node_73 = nodeB;
		
	nodeA = nodeB;//73
	nodeB = region->newStreet(++countID);//72
	region->newEdge(200,nodeA,nodeB);
	nodes::Node* node_72 = nodeB;
	
	nodeA = node_73;//73
	nodeB = region->newStreet(++countID);//83
	region->newEdge(800,nodeA,nodeB);
	nodes::Node* node_83 = nodeB;
	
	nodeA = nodeB;//83
	nodeB = region->newStreet(++countID);//84
	region->newEdge(300,nodeA,nodeB);

	nodeA = node_83;//83
	nodeB = region->newStreet(++countID);//85
	region->newEdge(200,nodeA,nodeB);

	nodeA = nodeB;//85
	nodeB = region->newStreet(++countID);//86
	region->newEdge(300,nodeA,nodeB);
}

}