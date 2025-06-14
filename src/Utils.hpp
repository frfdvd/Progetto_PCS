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

    // verifica la presenza di duplicati nei lati, se c'è già ritorna true altrimenti false
    // costo computazionale O(n) con n numero di lati nella matrice
    bool TestDuplicati(const MatrixXi& MatriceLati, const unsigned int& id1, const unsigned int& id2, unsigned int* PuntatoreLato = nullptr);

    // verifica la presenza di duplicati nei punti, se c'è già ritorna true altrimenti false
    // costo computazionale O(n) con n numero di punti nella matrice
    bool TestDuplicatiPunti(const MatrixXi& MatricePunti, const Vector3d& coordinate, unsigned int& idTrovato);

    // inserisce i lati all'interno della matrice Cell1DsExtrema verificando tramite test duplicati che tale lato non esista
    // costo computazionale O(n) con n numero di lati perchè chiama TestDuplicati
    bool inserisciLati(MatrixXi& MatriceLati, vector<unsigned int> VettoreIdLati, unsigned int& contaIdLati, const unsigned int& id1, const unsigned int& id2);

    // proietta i punti sulla sfera unitaria
    // costo computazionale O(n) con n numero di punti 
    bool ProiettaPunti(MatrixXd& MatriceCoordinate);

    // riconosce il poliedro che viene inserito
    // costo computazionale O(1)
    string RiconosciPoliedro(const unsigned int& q);

    // trova la triangolazione di tipo uno dei punti di un poligono
    // costo computazionale O(C*b^4) con C > 60 almeno
    bool TriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b, const unsigned int& q);

    // crea il duale
    // costo computazionale O(C*b^4) con C > 30 almeno
    bool CreaDuale(const PolygonalMesh& mesh1, PolygonalMesh& mesh2);

    // calcola il cammino minimo
    bool CamminoMinimo(const PolygonalMesh& mesh, const unsigned int& id1, const unsigned int& id2, const string& nomefilepunti, const string& nomefilelati);

    // implementa l'algoritmo BFS
    bool Dijkstra(const unsigned int& n,const vector<vector<unsigned int>>& LA, const unsigned int& start, const unsigned int& end, MatrixXd& matrice, vector<unsigned int>& path);

    // crea il baricentro nella triangolazione di tipo due, lo aggiunge a meshtri e anche alla mappa dei baricentri
    // costo computazionale O(1)
    bool CreaBaricentro(PolygonalMesh& meshTri,const vector<unsigned int>& vecpunti, const unsigned int& IdBar, map<unsigned int, vector<unsigned int>>& map);

    // controlla se sul bordo si deve aggiungere un punto e nel caso trova le sue coordinate
    bool ControllaBordi(const vector<vector<unsigned int>>& latiCompleti, const unsigned int& id1, const unsigned int& id2, Vector3d& Medio, PolygonalMesh& meshTri, unsigned int& contapunti, bool& Duplicato, unsigned int& IdTrovato);
    
    // congiunge il baricentro di ogni faccina con i suoi estremi
    bool AggiungiLati(PolygonalMesh& meshTri,const vector<unsigned int>& vecpunti, const unsigned int& IdBar, unsigned int& contatore);

    // collega i baricenti dei triangolini
    bool CollegaBaricentri(const map<unsigned int, vector<unsigned int>>& MapBaricentri,const unsigned int& IdBaricentro, PolygonalMesh& meshTri, unsigned int& contaLati, const vector<unsigned int>& baricentri, unsigned int& contaIdFacce);

    // trova la triangolazione di tipo due dei punti di un poligono
    bool TriangolazioneDue(const PolygonalMesh& mesh1, PolygonalMesh& meshTri, const unsigned int& b, const unsigned int& q);

    //stampa su file
    bool StampasuFile(const PolygonalMesh& mesh);
}