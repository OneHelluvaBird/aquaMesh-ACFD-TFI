#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Point.h"
#include "Cell.h"
#include "Surface.h"

class Mesh
{
public:

    //--------------------------------------------------
    // Data
    //--------------------------------------------------

    std::vector<Point> nodes;
    std::vector<Cell> cells;

    double xmin;
    double xmax;

    double ymin;
    double ymax;

    int Nx;
    int Ny;

    double dx;
    double dy;

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------

    Mesh();

    //--------------------------------------------------
    // Mesh Generation
    //--------------------------------------------------

    void generateCartesian
    (
        const Surface& surface,
        int Nx_,
        int Ny_
    );

    //--------------------------------------------------
    // Utility Functions
    //--------------------------------------------------

    int getNumberOfNodes() const;

    int getNumberOfCells() const;

    // mesh quality(signed area/jacobianper cell)
    std::vector<double>cellArea;
    double shoelaceArea(std::vector<int> nodesIn);
    void computeCellAreas();


protected:

    void computeBoundingBox(const Surface& surface);
    void computeBoundingBox();

private:

    void generateNodes();
    void generateCells();
};

#endif