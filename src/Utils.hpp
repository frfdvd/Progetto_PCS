#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"
#include <string>

using namespace std;

namespace PolygonalLibrary{
    
    // chiama a cascata le altre funzioni che importano Cell0Ds, Cell1Ds, Cell2Ds e
    bool ImportMesh(PolygonalMesh& mesh, const string& Poliedro);

    // importa i punti e salva tutto nelle strutture dati migliori
    bool ImportCell0Ds(PolygonalMesh& mesh, const string& Poliedro);

    // importa i segmenti e salva tutto nelle strutture dati migliori
    bool ImportCell1Ds(PolygonalMesh& mesh, const string& Poliedro);

    // riconosce il poliedro che viene inserito
    string RiconosciPoliedro(const unsigned int& q);

}