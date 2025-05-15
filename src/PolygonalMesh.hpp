#pragma once

#include <iostream>
#include <vector>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary{

    struct PolygonalMesh
    {
        unsigned int NumCell0Ds;
        unsigned int NumCell1Ds;
        unsigned int NumCell2Ds;

        vector<unsigned int> Cell0DsId;
        vector<unsigned int> Cell1DsId;
        vector<unsigned int> Cell2DsId;

        MatrixXd Cell0DsCoordinates;
        MatrixXi Cell1DsExtrema;

        // vediamo se tenerli
        vector<vector<unsigned int>> Cell2DsVertices;
        vector<vector<unsigned int>> Cell2DsEdges;
    };

}

