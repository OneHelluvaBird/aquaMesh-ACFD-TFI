/*
Compiling Instruction: g++ TFI.cpp src/*.cpp -Iinclude -std=c++17 -o TFI && ./TFI
*/
#include<iostream>
#include<cmath>
#include<fstream>
#include<algorithm>
#include<string>
#include<utility>
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
    const std::pair<int, int> meshSizes[] = {
        {10, 5},
        {20, 10},
        {40, 20}
    };

    for (const auto& [N_xi, N_eta] : meshSizes)
    {
        const std::string suffix = "_" + std::to_string(N_xi)
                                 + "_" + std::to_string(N_eta);
        const std::string boundaryFile = "output" + suffix + ".dat";
        const std::string cartesianFile = "xi_eta" + suffix + ".vtk";
        const std::string tfiFile = "tfi" + suffix + ".vtk";
        const std::string modifiedFile = "tfi_modi" + suffix + ".vtk";

        std::cout << "\n========================================\n";
        std::cout << "Mesh size: N_xi = " << N_xi
                  << ", N_eta = " << N_eta << '\n';
        std::cout << "========================================\n";

        //i) To generate and visualize the boundary

        double delta_xi=1.0/(N_xi);
        double delta_eta=1.0/(N_eta);

        double currXi,currEta;

        std::pair<double,double> curveCoords;
        
        //Boundary points stored in a textfile to plot using plotter.py
        std::ofstream outfile(boundaryFile);

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
        
        std::cout << "Boundary contour is saved in " << boundaryFile << '\n';
        std::cout << "Use this boundary file as input to plotter.py to visualize\n";
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

        xi_eta_mesh.computeCellAreas();

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
            cartesianFile.c_str()
        );

        MeshWriter::writeVTK(
            tfi_mesh,
            tfiFile.c_str()
        );

        // Mesh quality
        tfi_mesh.computeJacobians();
        
        double jmin=tfi_mesh.Jacobian[0];
        double jmax=tfi_mesh.Jacobian[0];
        int inval=-1;// no invalid found 

        for (size_t c=0; c<tfi_mesh.Jacobian.size();c++)
        {
    	double j=tfi_mesh.Jacobian[c];
    	if(j<jmin) jmin=j;
    	if(j>jmax) jmax=j;
    	if(j<=0.0 && inval==-1)
    	{
    		inval=static_cast<int>(c);
    	}
         } 
         std::cout<< "\nMesh Quality (TFI mesh)\n";
         std::cout<< "------------------------\n";
         std::cout<< "Jmin = " << jmin << std::endl;
         std::cout<< "Jmax = " << jmax << std::endl;
         
         if (inval==-1)
         {
    	std::cout<< "All cells have J > 0 : mesh is valid.\n";
         }
         else 
         {
    	auto nodeIDs=tfi_mesh.cells[inval].nodeIDs;
    	double cx=0.0,cy=0.0;
    	for (int n:nodeIDs)
    	{
    		cx+=tfi_mesh.nodes[n].x;
    		cy+=tfi_mesh.nodes[n].y;
    	}
    	cx/=nodeIDs.size();
    	cy/=nodeIDs.size();
    	std::cout<<"INVALID cell found!\n";
            std::cout<<"Cell ID:" <<inval<<"\n";
            std::cout<<"Jacobian:"<<tfi_mesh.Jacobian[inval]<<"\n";
            std::cout<<"Location:("<<cx<<","<<cy<<")\n";
        }

        // modify bc
        
        auto CT_modi=[](double xi){
        	std::pair<double,double>ans={xi,1.0+0.8*sin(acos(-1.0)*xi)};
    	return ans;
        };
        Mesh tfi_mesh_modi=xi_eta_mesh;
        CoordinateMapping::TFI(tfi_mesh_modi,CB,CT_modi,CL,CR);
        tfi_mesh_modi.computeJacobians();
        double jmin1=*std::min_element(tfi_mesh_modi.Jacobian.begin(),tfi_mesh_modi.Jacobian.end());
        double jmax1=*std::max_element(tfi_mesh_modi.Jacobian.begin(),tfi_mesh_modi.Jacobian.end());
        
        std::cout<<"\nMesh Quality (modified CT amplitude 0.2 -> 0.8)\n";
        std::cout<< "------------------------------------------------\n";
        std::cout<<"Jmin="<<jmin1<<std::endl;
        std::cout<<"Jmax="<<jmax1<<std::endl;
        std::cout<<(jmin1>0.0 ? "All cells have J > 0 : mesh is valid.\n"
                                 : "Some cells have J <= 0 : mesh is INVALID.\n");
        MeshWriter::writeVTK(tfi_mesh_modi,modifiedFile.c_str());
       
    } // Repeat all steps for the next mesh size.

    return 0;
}
