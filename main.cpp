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
    PolygonalMesh meshDuale;

    unsigned int b = 4;
    unsigned int p = 3;
    unsigned int q = 3;
    unsigned int id1 = 0;
    unsigned int id2 = 5;
    string Poliedro = RiconosciPoliedro(q);

    cout << Poliedro << endl;

    // importo la mesh e verifico che avvenga correttamente
    if(!ImportMesh(mesh, Poliedro))
    {
        cerr << "file not found" << endl;
        return 1;
    }

 
    string ParaviewPunti = "./Cell0D.inp";
    string ParaviewSegmenti = "./Cell1D.inp"; 

    UCDUtilities utilities;
    utilities.ExportPoints(ParaviewPunti,
                           mesh.Cell0DsCoordinates);

    utilities.ExportSegments(ParaviewSegmenti,
                             mesh.Cell0DsCoordinates,
                             mesh.Cell1DsExtrema);

                        
    if(!TriangolazioneUno(mesh, meshTriangolata, b, q))
    {
        cerr << "error during triangolation" << endl;
        return 1;
    }

    
    string ParaviewPuntiTriangolati = "./Cell0DTriang.inp";
    string ParaviewSegmentiTriangolati = "./Cell1DTriang.inp";

    utilities.ExportPoints(ParaviewPuntiTriangolati, 
                            meshTriangolata.Cell0DsCoordinates);
    utilities.ExportSegments(ParaviewSegmentiTriangolati,
                             meshTriangolata.Cell0DsCoordinates,
                             meshTriangolata.Cell1DsExtrema);


    // calcolo il cammino minimo
    if(!CamminoMinimo(meshTriangolata, id1, id2, ParaviewPuntiTriangolati, ParaviewSegmentiTriangolati)){
        return false;
    }

    unsigned int qDuale = p;
    unsigned int pDuale = q;
    if(!CreaDuale(meshTriangolata, meshDuale))
    {
        cerr << "error during triangolation" << endl;
        return 1;
    }

    

    string ParaviewPuntiDuale = "./Cell0DDuale.inp";
    string ParaviewSegmentiDuale = "./Cell1DDuale.inp";
    
    utilities.ExportPoints(ParaviewPuntiDuale, 
                            meshDuale.Cell0DsCoordinates);
    utilities.ExportSegments(ParaviewSegmentiDuale,
                             meshDuale.Cell0DsCoordinates,
                             meshDuale.Cell1DsExtrema);

    
    // qua metto cammino minimo
    if(!CamminoMinimo(meshDuale, id1, id2, ParaviewPuntiDuale, ParaviewSegmentiDuale)){
        return false;
    }


    return 0;
}