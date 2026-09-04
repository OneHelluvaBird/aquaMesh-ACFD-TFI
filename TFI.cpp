/*
Compiling Instruction: g++ TFI.cpp src/*.cpp -Iinclude -std=c++17 -o TFI && ./TFI
*/
#include<iostream>
#include<cmath>
#include<fstream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "Mesh.h"
#include "CoordinateMapping.h"
#include "MeshWriter.h"

std::pair<double,double>CB(double xi){
        std::pair<double,double> ans={xi,0.0};
        return ans;
};

std::pair<double,double>CT(double xi){
        std::pair<double,double> ans={xi,1.0+0.2*sin(acos(-1.0)*xi)};
        return ans;
};

std::pair<double,double>CL(double eta){
        std::pair<double,double> ans={0.0,eta};
        return ans;
};

std::pair<double,double>CR(double eta){
        std::pair<double,double> ans={1.0,eta+0.2*eta*(1.0-eta)};
        return ans;
};

int main()
{
    //i) To generate and visualize the boundary
    int N_xi=20;
    int N_eta=10;

    double delta_xi=1.0/(N_xi);
    double delta_eta=1.0/(N_eta);

    double currXi,currEta;

    std::pair<double,double> curveCoords;
    
    //Boundary points stored in a textfile to plot using plotter.py
    std::ofstream outfile("output.dat");

    for(int i=0;i<=N_xi;i++){
                currXi=i*delta_xi;
                curveCoords=CB(currXi);
                outfile<<curveCoords.first<<"\t"<<curveCoords.second<<std::endl;
    };

    for(int i=0;i<=N_xi;i++){
                currXi=i*delta_xi;
                curveCoords=CT(currXi);
                outfile<<curveCoords.first<<"\t"<<curveCoords.second<<std::endl;
    };

    for(int i=0;i<=N_eta;i++){
                currEta=i*delta_eta;
                curveCoords=CL(currEta);
                outfile<<curveCoords.first<<"\t"<<curveCoords.second<<std::endl;
    };

    for(int i=0;i<=N_eta;i++){
                currEta=i*delta_eta;
                curveCoords=CR(currEta);
                outfile<<curveCoords.first<<"\t"<<curveCoords.second<<std::endl;
    };

    outfile.close();
    
    std::cout << "Boundary contour is saved in output.dat\n";
    std::cout << "Run plotter.py to visualize\n";
    //ii) Transfinite Interpolation to generate interior points 
    Point p0(0,0.0,0.0);
    Point p1(1,1.0,0.0);
    Point p2(2,1.0,1.0);
    Point p3(3,0.0,1.0); 

    Line l0(0,&p0,&p1);
    Line l1(1,&p1,&p2);
    Line l2(2,&p2,&p3);
    Line l3(3,&p3,&p0);

    Surface xi_eta_plane(0);

    xi_eta_plane.addBoundary(&l0);
    xi_eta_plane.addBoundary(&l1);
    xi_eta_plane.addBoundary(&l2);
    xi_eta_plane.addBoundary(&l3);

    Mesh xi_eta_mesh, tfi_mesh;

    xi_eta_mesh.generateCartesian(
        xi_eta_plane,
        N_xi,
        N_eta
    );

    tfi_mesh = xi_eta_mesh;

    CoordinateMapping::TFI(
        tfi_mesh,
        CB,
        CT,
        CL,
        CR
    );

    MeshWriter::writeVTK(
        xi_eta_mesh,
        "xi_eta.vtk"
    );

    MeshWriter::writeVTK(
        tfi_mesh,
        "tfi.vtk"
    );

    // Verification and Mesh Quality is left to do
    return 0;
}
