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
    PolygonalMesh meshTriangolata1;
    PolygonalMesh meshTriangolata2;
    PolygonalMesh meshDuale1;
    PolygonalMesh meshDuale2;


    unsigned int b = 3;
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

    
    if(!TriangolazioneDue(mesh, meshTriangolata2, b, q))
    {
        cerr << "error during triangolation" << endl;
        return 1;
    }
                             
    string ParaviewPuntiTriangolati2 = "./Cell0DTriang2.inp";
    string ParaviewSegmentiTriangolati2 = "./Cell1DTriang2.inp";

    utilities.ExportPoints(ParaviewPuntiTriangolati2, 
                            meshTriangolata2.Cell0DsCoordinates);
    utilities.ExportSegments(ParaviewSegmentiTriangolati2,
                             meshTriangolata2.Cell0DsCoordinates,
                             meshTriangolata2.Cell1DsExtrema);


    if(!TriangolazioneUno(mesh, meshTriangolata1, b, q))
    {
        cerr << "error during triangolation" << endl;
        return 1;
    }

    
    string ParaviewPuntiTriangolati1 = "./Cell0DTriang1.inp";
    string ParaviewSegmentiTriangolati1 = "./Cell1DTriang1.inp";

    utilities.ExportPoints(ParaviewPuntiTriangolati1, 
                            meshTriangolata1.Cell0DsCoordinates);
    utilities.ExportSegments(ParaviewSegmentiTriangolati1,
                             meshTriangolata1.Cell0DsCoordinates,
                             meshTriangolata1.Cell1DsExtrema);
 


    //calcolo il cammino minimo
    if(!CamminoMinimo(meshTriangolata1, id1, id2, ParaviewPuntiTriangolati1, ParaviewSegmentiTriangolati1)){
        return false;
    }

    if(!CamminoMinimo(meshTriangolata2, id1, id2, ParaviewPuntiTriangolati2, ParaviewSegmentiTriangolati2)){
        return false;
    }

    unsigned int qDuale = p;
    unsigned int pDuale = q;
    if(!CreaDuale(meshTriangolata1, meshDuale1))
    {
        cerr << "error during triangolation" << endl;
        return 1;
    }

    

    string ParaviewPuntiDuale1 = "./Cell0DDuale1.inp";
    string ParaviewSegmentiDuale1 = "./Cell1DDuale1.inp";
    
    utilities.ExportPoints(ParaviewPuntiDuale1, 
                            meshDuale1.Cell0DsCoordinates);
    utilities.ExportSegments(ParaviewSegmentiDuale1,
                             meshDuale1.Cell0DsCoordinates,
                             meshDuale1.Cell1DsExtrema);

    
    if(!CreaDuale(meshTriangolata2, meshDuale2))
    {
        cerr << "error during triangolation" << endl;
        return 1;
    }

    

    string ParaviewPuntiDuale2 = "./Cell0DDuale2.inp";
    string ParaviewSegmentiDuale2 = "./Cell1DDuale2.inp";
    
    utilities.ExportPoints(ParaviewPuntiDuale2, 
                            meshDuale2.Cell0DsCoordinates);
    utilities.ExportSegments(ParaviewSegmentiDuale2,
                            meshDuale2.Cell0DsCoordinates,
                            meshDuale2.Cell1DsExtrema);

    // qua metto cammino minimo
    if(!CamminoMinimo(meshDuale1, id1, id2, ParaviewPuntiDuale1, ParaviewSegmentiDuale1)){
        return false;
    }
    
    if(!CamminoMinimo(meshDuale2, id1, id2, ParaviewPuntiDuale2, ParaviewSegmentiDuale2)){
        return false;
    }

    return 0;
}