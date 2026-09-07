//------------------------------------------------------------//
// File: testTFI.cpp
//
// Regression Tests
//
// Mesh Transformation
//
//------------------------------------------------------------//

#include <iostream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"

#include "Mesh.h"
#include "CoordinateMapping.h"

#include "Test.h"

using namespace std;

std::pair<double,double>CL(double eta){
        return {0.0,eta};
};

std::pair<double,double>CR(double eta){
        return {1.0,eta};
};

std::pair<double,double>CB(double xi){
        return {xi,0.0};
};

std::pair<double,double>CT(double xi){
        return {xi,1.0};
};

std::pair<double,double>CT_alt(double xi){
        return {xi,2.0-4.0*(xi-0.5)*(xi-0.5)};
};

int main()
{
    cout << "=========================================\n";
    cout << " Running Mesh Transformation Tests\n";
    cout << "=========================================\n\n";

    Test test;

    //--------------------------------------------------------
    // Geometry
    //--------------------------------------------------------

    Point p0(0,0.0,0.0);
    Point p1(1,1.0,0.0);
    Point p2(2,1.0,1.0);
    Point p3(3,0.0,1.0);

    Line l0(0,&p0,&p1);
    Line l1(1,&p1,&p2);
    Line l2(2,&p2,&p3);
    Line l3(3,&p3,&p0);

    Surface rectangle(0);

    rectangle.addBoundary(&l0);
    rectangle.addBoundary(&l1);
    rectangle.addBoundary(&l2);
    rectangle.addBoundary(&l3);

    //--------------------------------------------------------
    // Original Mesh
    //--------------------------------------------------------

    Mesh original;

    original.generateCartesian
    (
        rectangle,
        20,
        10
    );

    //--------------------------------------------------------
    // Translation
    //--------------------------------------------------------

    Mesh TFI1 = original;

    CoordinateMapping::TFI
    (
        TFI1,
        CB,
        CT,
        CL,
        CR
    );

    TFI1.computeCellAreas();

    test.expectEqual(TFI1.cellArea[32],
                     1.0/200.0,
                     1e-12,
                     "Correct area calc");

    test.expectEqual(TFI1.cellArea[54],
                     1.0/200.0,
                     1e-12,
                     "Correct area calc");

    test.expectEqual(TFI1.nodes[45].x,
                     original.nodes[45].x,
                     1e-12,
                     "Unit square preserved under TFI");

    test.expectEqual(TFI1.nodes[102].y,
                     original.nodes[102].y,
                     1e-12,
                     "Unit square preserved under TFI");

    Mesh TFI2 = original;
    CoordinateMapping::TFI
    (
        TFI2,
        CB,
        CT_alt,
        CL,
        CR
    );

    test.expectEqual(TFI2.nodes[12].x,
                     original.nodes[12].x,
                     1e-12,
                     "Square with curved top surface: x coordinates preserved");

    test.expectEqual(TFI2.nodes[190].x,
                     original.nodes[190].x,
                     1e-12,
                     "Square with curved top surface: x coordinates preserved");

    test.expectEqual(TFI2.nodes[21*10+5].y,
                     (CT_alt(original.nodes[21*10+5].x)).second,
                     1e-12,
                     "Square with curved top surface: top boundary preserved");

    test.expectEqual(TFI2.nodes[21*10+16].y,
                     (CT_alt(original.nodes[21*10+16].x)).second,
                     1e-12,
                     "Square with curved top surface: top boundary preserved");

    test.summary();

    return test.success() ? 0 : 1;
}
