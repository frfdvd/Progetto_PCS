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
    PolygonalMesh meshTriangolata;

    unsigned int b = 3;
    unsigned int p = 3;
    unsigned int q = 3;
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

                        
    if(!ImportTriangolazioneUno(mesh, meshTriangolata, b, q))
    {
        cerr << "error during triangolation" << endl;
        return 1;
    }

    string ParaviewPuntiTriangolati = "./Cell0D" + Poliedro + "Triang" + ".inp";
    string ParaviewPuntiTriangolati = "./Cell1D" + Poliedro + "Triang" + ".inp";

    utilities.ExportPoints(ParaviewPuntiTriangolati, 
                            meshTriangolata.Cell0DsCoordinates);
    utilities.ExportSegments(ParaviewSegmenti,
                             meshTriangolata.Cell0DsCoordinates,
                             meshTriangolata.Cell1DsExtrema);

    return 0;
}