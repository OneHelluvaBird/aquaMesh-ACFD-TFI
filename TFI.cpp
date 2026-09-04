/*
Compiling Instruction: g++ TFI.cpp src/*.cpp -Iinclude -std=c++17 -o TFI && ./TFI
*/

#ifndef M_PI
#define M_PI 3.14159265359 
#endif
#include<iostream>
#include<cmath>
#include<fstream>

double A=1.0;
double B=1.0;
double C=1.0;

std::pair<double,double>CB(double xi){
        std::pair<double,double> ans={xi,0.0};
        return ans;
};

std::pair<double,double>CT(double xi){
        std::pair<double,double> ans={xi,1.0+0.2*sin(M_PI*xi)};
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
    int N_xi=20;
    int N_eta=10;

    double delta_xi=1.0/(N_xi);
    double delta_eta=1.0/(N_eta);

    double currXi,currEta;

    std::pair<double,double> curveCoords;

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
    return 0;
}
