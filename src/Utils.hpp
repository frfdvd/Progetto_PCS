#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"
#include <string>
#include <map>

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary{
    
    // chiama a cascata le altre funzioni che importano Cell0Ds, Cell1Ds, Cell2Ds 
    bool ImportMesh(PolygonalMesh& mesh, const string& Poliedro);

    // importa i punti e salva tutto nelle strutture dati migliori
    bool ImportCell0Ds(PolygonalMesh& mesh, const string& Poliedro);

    // importa i segmenti e salva tutto nelle strutture dati migliori
    bool ImportCell1Ds(PolygonalMesh& mesh, const string& Poliedro);

    // importa le facce e salva tutto in dei dizionari comodi comodi 
    bool ImportCell2Ds(PolygonalMesh& mesh, const string& Poliedro);

    // riconosce il poliedro che viene inserito
    string RiconosciPoliedro(const unsigned int& q);

    // chiama a cascata le altre funzioni che importano la triangolazione di tipo uno di punti e lati
    bool ImportTriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b, const unsigned int& q);

    // trova la triangolazione di tipo uno dei punti di un poligono
    bool TriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b, const unsigned int& q);

    // trova la triangolazione di tipo uno dei lati di un poligono  
    bool Cell1DTriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b);

}