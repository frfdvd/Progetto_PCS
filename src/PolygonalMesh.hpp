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

        // per ogni faccia ci sono contenuti gli id dei vertici e dei lati in modo ordinato
        vector<vector<unsigned int>> VettoreVertici;
        vector<vector<unsigned int>> VettoreLati;
    };
}

