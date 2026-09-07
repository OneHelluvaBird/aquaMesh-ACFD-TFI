//------------------------------------------------------------//
// File: Mesh.cpp
//
// Description:
// Generates a structured Cartesian mesh from a rectangular
// surface.
//
// Workflow:
//
// Surface
//      ↓
// Bounding Box
//      ↓
// Generate Nodes
//      ↓
// Generate Cells
//------------------------------------------------------------//

#include "Mesh.h"

#include <iostream>
#include <algorithm>

using namespace std;


//------------------------------------------------------------//
// Constructor
//------------------------------------------------------------//

Mesh::Mesh()
{
    xmin = 0.0;
    xmax = 0.0;

    ymin = 0.0;
    ymax = 0.0;

    Nx = 0;
    Ny = 0;

    dx = 0.0;
    dy = 0.0;
}


//------------------------------------------------------------//
// Compute Bounding Box
//------------------------------------------------------------//

void Mesh::computeBoundingBox(const Surface& surface)
{
    xmin = surface.boundaries[0]->start->x;
    xmax = xmin;

    ymin = surface.boundaries[0]->start->y;
    ymax = ymin;

    for(auto line : surface.boundaries)
    {
        xmin = min(xmin,line->start->x);
        xmin = min(xmin,line->end->x);

        xmax = max(xmax,line->start->x);
        xmax = max(xmax,line->end->x);

        ymin = min(ymin,line->start->y);
        ymin = min(ymin,line->end->y);

        ymax = max(ymax,line->start->y);
        ymax = max(ymax,line->end->y);
    }

    dx = (xmax-xmin)/Nx;
    dy = (ymax-ymin)/Ny;
}


//------------------------------------------------------------//
// Generate Nodes
//------------------------------------------------------------//

void Mesh::generateNodes()
{
    nodes.clear();

    nodes.reserve((Nx+1)*(Ny+1));

    int id = 0;

    for(int j=0;j<=Ny;j++)
    {
        double y = ymin + j*dy;

        for(int i=0;i<=Nx;i++)
        {
            double x = xmin + i*dx;

            nodes.emplace_back(id,x,y);

            id++;
        }
    }
}


//------------------------------------------------------------//
// Generate Cells
//------------------------------------------------------------//

void Mesh::generateCells()
{
    cells.clear();

    cells.reserve(Nx*Ny);

    int id = 0;

    for(int j=0;j<Ny;j++)
    {
        for(int i=0;i<Nx;i++)
        {
            int n0 = j*(Nx+1)+i;

            int n1 = n0+1;

            int n2 = n1+(Nx+1);

            int n3 = n0+(Nx+1);

            cells.emplace_back
            (
                id,
                n0,
                n1,
                n2,
                n3
            );

            id++;
        }
    }
}


//------------------------------------------------------------//
// Generate Cartesian Mesh
//------------------------------------------------------------//

void Mesh::generateCartesian
(
    const Surface& surface,
    int Nx_,
    int Ny_
)
{
    Nx = Nx_;
    Ny = Ny_;

    computeBoundingBox(surface);

    generateNodes();

    generateCells();

    cout << "\n";
    cout << "Cartesian Mesh Generated\n";
    cout << "------------------------\n";

    cout << "Nodes : "
         << nodes.size()
         << endl;

    cout << "Cells : "
         << cells.size()
         << endl;
}
//------------------------------------------------------------//
// Number of Nodes
//------------------------------------------------------------//

int Mesh::getNumberOfNodes() const
{
    return static_cast<int>(nodes.size());
}


//------------------------------------------------------------//
// Number of Cells
//------------------------------------------------------------//

int Mesh::getNumberOfCells() const
{
    return static_cast<int>(cells.size());
}

//------------------------------------------------------------//
// Compute Bounding Box from Mesh Nodes
//------------------------------------------------------------//

void Mesh::computeBoundingBox()
{
    if(nodes.empty())
    {
        xmin = xmax = 0.0;
        ymin = ymax = 0.0;
        return;
    }

    xmin = xmax = nodes[0].x;
    ymin = ymax = nodes[0].y;

    for(const auto& node : nodes)
    {
        xmin = std::min(xmin, node.x);
        xmax = std::max(xmax, node.x);

        ymin = std::min(ymin, node.y);
        ymax = std::max(ymax, node.y);
    }
}

double Mesh::shoelaceArea(std::vector<int> nodesIn){
        nodesIn.push_back(nodesIn[0]);
        int N=nodesIn.size();

        double ans=0.0;

        for(int n=0;n<N-1;n++){ 
                ans=ans+nodes[nodesIn[n]].x*nodes[nodesIn[n+1]].y;
                ans=ans-nodes[nodesIn[n]].y*nodes[nodesIn[n+1]].x;
        }

        ans=ans*0.5;
        return ans;
}

void Mesh::computeCellAreas(){
	cellArea.clear();
	cellArea.reserve(cells.size());
	for (auto& cell:cells)
	{
		cellArea.push_back(shoelaceArea(cell.nodeIDs));
	}
}

void Mesh::computeJacobians(){
	Jacobian.clear();
	Jacobian.reserve(cells.size());
        int cellID=0;
        double currArea;
	for (auto& cell:cells)
	{
                currArea=shoelaceArea(cell.nodeIDs);
		Jacobian.push_back(currArea/cellArea[cellID]);
                cellArea[cellID]=currArea;
                cellID=cellID+1;
	}
}
