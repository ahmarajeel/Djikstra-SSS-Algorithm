#include <iostream>
#include <fstream>
using namespace std;
class DijktraSSS 
{
	public:
		int numNodes;
		int sourceNode;
		int minNode;
		int currentNode;
		int newCost;
		int **costMatrix;
		int *fatherAry;
		int *markedAry;
		int *bestCostAry;
		
		DijktraSSS(int nodes)
		{
			numNodes = nodes;
			costMatrix = new int*[numNodes];
			fatherAry = new int[numNodes];
			markedAry = new int[numNodes];
			bestCostAry = new int[numNodes];
			//allocation of cost matrix
			for(int i = 1; i <= numNodes; i++)
			{
				costMatrix[i] = new int[numNodes];
				fatherAry[i] = i;   //intialize to fatherAry[i] = i
				markedAry[i] = 0;   //initialize to 0
				bestCostAry[i] = 99999;  //intialize to infinity i.e 99999
			}
			
			//intialization of cost matrix
			for(int j = 1; j <= numNodes; j++)
			{
				for(int k = 1; k <= numNodes; k++)
				{
					if(j == k)
					{
						costMatrix[j][k] = 0;
					}
					else
					{
						costMatrix[j][k] = 99999;
					}	
				}
			}
		}
		
		void loadCostMatrix(ifstream& inFile)
		{
			int from, to, cost;
			while(inFile >> from >> to >> cost)
			{
				costMatrix[from][to] = cost;
			}
		}
		
		void loadBestCostAry(int source)
		{
			for(int i = 1; i <= numNodes; i++)
			{
				bestCostAry[i] = costMatrix[source][i];
			}
		}
		
		void loadFatherAry(int source)
		{
			for(int i = 0; i <= numNodes; i++)
			{
				fatherAry[i] = source;	
			}	
		}
		
		void loadMarkedAry()
		{
			for(int i = 0; i <= numNodes; i++)
			{
				markedAry[i] = 0;	
			}	
		}
		
		int computeCost(int minimumNode, int currNode)
		{
			return bestCostAry[minimumNode] +  costMatrix[minimumNode][currNode];		
		} 
		
		void markMinNode(int minimumNode)
		{
			markedAry[minimumNode] = 1;
		} 
		
		void changeFather(int node, int minimumNode)
		{
			fatherAry[node] = minimumNode;
		}
		
		void changeCost(int node, int cost)
		{
			bestCostAry[node] = cost;
		}
		
		void debugPrint(ofstream& outFile2)
		{
			outFile2 << "The sourceNode is: " << sourceNode << endl;
			outFile2 << "The fatherAry is: ";
			print_1D(fatherAry, outFile2);
			outFile2 << "The bestCostAry is: ";
			print_1D(bestCostAry, outFile2);
			outFile2 << "The markedAry is: ";
			print_1D(markedAry, outFile2);
			outFile2 << endl;
		
		}
		
		void printShortestPaths(int current, ofstream& outFile1)
		{
			int i = 1, count = 1;
			int *temp = new int[numNodes];
			temp[0] = current;
			while(fatherAry[current] != sourceNode)
			{
				temp[i] = fatherAry[current];
				current = fatherAry[current];	
				count++;
				i++;
			}
			
			
			outFile1 << "The shortest path from " << sourceNode << " to " << temp[0] << " is: " << sourceNode ;
			for(int i = count - 1; i >= 0; i--)
			{
				outFile1 << "-->" << temp[i];	
			}
			outFile1 << " and cost is: " << bestCostAry[temp[0]] << endl;  					
		}
		
		int findUnmarkedMinNode()
		{
			int result;
			int cost = 999999;
			for(int i = 1; i <= numNodes; i++)
			{
				if(cost > bestCostAry[i] && markedAry[i] == 0)
				{
					cost = bestCostAry[i];
					result = i;
				}
			}
			return result;
		}
		
		bool isAllMarked()
		{
			for(int i = 1; i <= numNodes; i++)
			{
				if(markedAry[i] == 0)
				return false;
			}
			return true;
		}
		
		void print_1D(int *ary, ofstream& outFile2)
		{
			for(int i = 1; i <= numNodes; i++)
			{
				outFile2 << ary[i] << " ";
			}
			outFile2 << endl;
		}
		
		void dijktraAlgorith(ofstream& outFile1, ofstream& outFile2)
		{
			int count = 1;
			sourceNode = count;
			while(sourceNode <= numNodes)
			{	
				loadBestCostAry(sourceNode);
				loadFatherAry(sourceNode);
				loadMarkedAry();
				minNode = sourceNode;
				markMinNode(minNode);
				debugPrint(outFile2);
				
				while(!isAllMarked())
				{
					minNode = findUnmarkedMinNode();
					markMinNode(minNode);
					debugPrint(outFile2);
					
					for(int i = 1; i <= numNodes; i++)
					{
						if(markedAry[i] == 0)
						{
							currentNode = i;
						}
						else
						{
							continue;	
						}
						newCost = computeCost(minNode, currentNode);
						
						if(newCost < bestCostAry[currentNode])
						{
							changeFather(currentNode, minNode);
							changeCost(currentNode, newCost);
							debugPrint(outFile2);
						}
							
					}
				}
				currentNode = 1;
				outFile1 << "The sourceNode is: " << sourceNode << endl;
				while(currentNode <= numNodes)
				{
					printShortestPaths(currentNode, outFile1);
					currentNode++;
				}
				outFile1 << endl;
				sourceNode++;
			}
	
		}
};

int main(int argc, char* argv[])
{
	ifstream inFile;
	ofstream outFile1, outFile2;
	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	
	int item;
	inFile >> item;
	
	DijktraSSS graph(item);
	graph.loadCostMatrix(inFile);
	graph.dijktraAlgorith(outFile1, outFile2);
	
	inFile.close();
	outFile1.close();
	outFile2.close();
}
