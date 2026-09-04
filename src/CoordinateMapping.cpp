#include "CoordinateMapping.h"

#include <cmath>

using namespace std;

//------------------------------------------------------------//
// Identity Mapping
//------------------------------------------------------------//

void CoordinateMapping::identity(Mesh& mesh)
{
    // No change
}

//------------------------------------------------------------//
// Trapezoidal Mapping
//------------------------------------------------------------//

void CoordinateMapping::trapezoidal
(
    Mesh& mesh,
    double topScale
)
{
    double ymin = mesh.ymin;
    double ymax = mesh.ymax;

    for(auto& node : mesh.nodes)
    {
        double eta = (node.y-ymin)/(ymax-ymin);

        node.x *= (1.0 + eta*(topScale-1.0));
    }
}

//------------------------------------------------------------//
// Sinusoidal Mapping
//------------------------------------------------------------//

void CoordinateMapping::sinusoidal
(
    Mesh& mesh,
    double amplitude
)
{
    const double pi = acos(-1.0);

    double xmin = mesh.xmin;
    double xmax = mesh.xmax;

    double L = xmax-xmin;

    for(auto& node : mesh.nodes)
    {
        node.y += amplitude*
                  sin(2.0*pi*(node.x-xmin)/L);
    }
}

//------------------------------------------------------------//
// Polar Mapping
//------------------------------------------------------------//

void CoordinateMapping::polar
(
    Mesh& mesh,
    double rInner,
    double rOuter
)
{
    double xmin = mesh.xmin;
    double xmax = mesh.xmax;

    double ymin = mesh.ymin;
    double ymax = mesh.ymax;

    for(auto& node : mesh.nodes)
    {
        double xi =
            (node.x-xmin)/(xmax-xmin);

        double eta =
            (node.y-ymin)/(ymax-ymin);

        double r =
            rInner +
            eta*(rOuter-rInner);

        double theta =
            xi*2.0*acos(-1.0);

        node.x =
            r*cos(theta);

        node.y =
            r*sin(theta);
    }
}

void CoordinateMapping::TFI(
    Mesh& mesh,
    std::function<std::pair<double,double>(double)> CB,
    std::function<std::pair<double,double>(double)> CT,
    std::function<std::pair<double,double>(double)> CL,
    std::function<std::pair<double,double>(double)> CR
)
{
    auto blCorner = CL(0.0); //b - bottom, t - top, l - left, r -right
    auto tlCorner = CT(0.0);
    auto trCorner = CR(1.0);
    auto brCorner = CB(1.0);

    for(auto& node : mesh.nodes){
        double xi = node.x;
        double eta = node.y;

        auto cb = CB(xi);
        auto ct = CT(xi);
        auto cl = CL(eta);
        auto cr = CR(eta);

        //could be written as a pair (X_correct)
        double x_correct = (1-xi) *(1-eta)* blCorner.first
                          +(1-xi) *  eta  * tlCorner.first
                          + xi    *  eta  * trCorner.first
                          +(1-eta)*   xi  * brCorner.first;

        double y_correct = (1-xi) *(1-eta)* blCorner.second
                          +(1-xi) *  eta  * tlCorner.second
                          + xi    *  eta  * trCorner.second
                          +(1-eta)*   xi  * brCorner.second;

        node.x = (1-xi)  * cl.first
                +  xi    * cr.first
                +(1-eta) * cb.first
                +  eta   * ct.first - x_correct;

        node.y = (1-xi)  * cl.second
                +  xi    * cr.second
                +(1-eta) * cb.second
                +  eta   * ct.second - y_correct;
    }
}