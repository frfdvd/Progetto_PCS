#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <string>

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
using namespace Gedim;




int main(){
    
    // creo un oggetto mesh
    PolygonalMesh mesh;

    unsigned int p = 3;
    unsigned int q = 5;
    string Poliedro = RiconosciPoliedro(q);

    cout << Poliedro << endl;

    // importo la mesh e verifico che avvenga correttamente
    if(!ImportMesh(mesh, Poliedro))
    {
        cerr << "file not found" << endl;
        return 1;
    }

    
    string ParaviewPunti = "./Cell0D" + Poliedro + ".inp";
    string ParaviewSegmenti = "./Cell1D" + Poliedro + ".inp"; 

    UCDUtilities utilities;
    utilities.ExportPoints(ParaviewPunti,
                           mesh.Cell0DsCoordinates);

    utilities.ExportSegments(ParaviewSegmenti,
                             mesh.Cell0DsCoordinates,
                             mesh.Cell1DsExtrema);

    return 0;
}