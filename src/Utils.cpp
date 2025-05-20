#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>
#include <string>

namespace PolygonalLibrary{
    bool ImportMesh(PolygonalMesh& mesh, const string& Poliedro)
{

    if(!ImportCell0Ds(mesh, Poliedro))
        return false;

    if(!ImportCell1Ds(mesh, Poliedro))
        return false;
        
    return true;

}

/**********************************/

string RiconosciPoliedro(const unsigned int& q){
    string Poliedro;
    if(q==3){
        Poliedro="Tetraedro";
    }
    else if(q==4){
        Poliedro="Ottaedro";
    }
    else if(q==5){
        Poliedro="Icosaedro";
    }
    return Poliedro;
}

/**********************************/

bool ImportCell0Ds(PolygonalMesh& mesh, const string& Poliedro)
{
    string NomeFile="./Cell0D"+Poliedro+".csv";
    cout << NomeFile << endl;
    ifstream file(NomeFile);

    if(file.fail())
        return false;

    // creo una lista di stringhe che conterrà tutte le righe del csv
    list<string> listLines;

    string line;
    
    // inserisco in listLines le linee del file 
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // tolgo l'header
    listLines.pop_front();

    // il numero di punti è uguale alla lunghezza della lista
    mesh.NumCell0Ds = listLines.size();

    // se tale numero è 0 c'è un problema
    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    //INIZIA A CAMBIARE DA QUI

    // alloco lo spazio adatto per le mie matrici 
    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    
    // leggo riga per riga leggendo ogni valore separato da ;
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string field;
        unsigned int id;

        // separo ogni valore e lo inserisco delle strutture dati preposte 
        getline(converter, field, ';');
        id = stoi(field);

        getline(converter, field, ';');
        mesh.Cell0DsCoordinates(0, id) = stod(field);

        getline(converter, field, ';');
        mesh.Cell0DsCoordinates(1, id) = stod(field);

        getline(converter, field, ';');
        mesh.Cell0DsCoordinates(2, id) = stod(field);

        mesh.Cell0DsId.push_back(id);

    }

    return true;
}

/**********************************/

bool ImportCell1Ds(PolygonalMesh& mesh, const string& Poliedro)
{
    string NomeFile="./Cell1D"+Poliedro+".csv";
    ifstream file(NomeFile);

    if(file.fail())
        return false;
    
    
    list<string> listLines;
    string line;
    
    // salvo tutte le righe del csv in una lista di stringhe
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // tolgo l'header
    listLines.pop_front();

    // salvo e conto il numero di righe
    mesh.NumCell1Ds = listLines.size();

    // verifico che ci siano effettivamente dei lati 
    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }



    // alloco lo spazio necessario
    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi::Zero(2, mesh.NumCell1Ds);

    // itero su ogni riga del csv
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string field;
        unsigned int id;
        
        // separo in base al ; e salvo i valori 
        getline(converter, field, ';');
        id = stoi(field);

        getline(converter, field, ';');
        mesh.Cell1DsExtrema(0, id) = stoi(field);

        getline(converter, field, ';');
        mesh.Cell1DsExtrema(1, id) = stoi(field);
        
        
        mesh.Cell1DsId.push_back(id);
    }
    return true;
}
/**********************************/

bool TriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b, const unsigned int& q){
    
    unsigned int T = pow(b,2);
    cout << T << endl;

    if(q==3){
        mesh2.NumCell0Ds = 2*T + 2;
        mesh2.NumCell1Ds = 6*T;
        mesh2.NumCell2Ds = 4*T;
    }
    else if(q==4){
        mesh2.NumCell0Ds = 4*T + 2;
        mesh2.NumCell1Ds = 12*T;
        mesh2.NumCell2Ds = 8*T;
    }
    else if(q==5){
        mesh2.NumCell0Ds = 10*T + 2;
        mesh2.NumCell1Ds = 30*T;
        mesh2.NumCell2Ds = 20*T;
    }

    mesh2.Cell0DsCoordinates = MatrixXd::Zero(3, mesh2.NumCell0Ds);
    mesh2.Cell0DsId.reserve(mesh2.NumCell0Ds);
    unsigned int contIdPunti = mesh1.NumCell0Ds - 1;

    // aggiungo a mesh2 le coordinate dei punti di mesh1
    for(unsigned int id : mesh1.Cell0DsId){
        mesh2.Cell0DsId.push_back(id);
        mesh2.Cell0DsCoordinates(0, id) = mesh1.Cell0DsCoordinates(0, id);
        mesh2.Cell0DsCoordinates(1, id) = mesh1.Cell0DsCoordinates(1, id);
        mesh2.Cell0DsCoordinates(2, id) = mesh1.Cell0DsCoordinates(2, id);
    }
    
    // aggiungo le coordinate nuove (triangolazione) dei punti che stanno sui lati
    for(unsigned int id : mesh1.Cell1DsId){
        cout << "id lato "<< id << endl;

        // memorizzo gli estremi del lato e li metto dentro dei vettori
        unsigned int idEstremo1 = mesh1.Cell1DsExtrema(0,id);
        unsigned int idEstremo2 = mesh1.Cell1DsExtrema(1,id);



        Vector3d Estremo1(mesh1.Cell0DsCoordinates(0,idEstremo1),mesh1.Cell0DsCoordinates(1,idEstremo1),mesh1.Cell0DsCoordinates(2,idEstremo1));
        Vector3d Estremo2(mesh1.Cell0DsCoordinates(0,idEstremo2),mesh1.Cell0DsCoordinates(1,idEstremo2),mesh1.Cell0DsCoordinates(2,idEstremo2));
        cout << "estremo 1 " <<Estremo1(0) << Estremo1(1) << Estremo1(2) << endl;
        cout << "estremo 2 " <<Estremo2(0) << Estremo2(1) << Estremo2(2) << endl;
        
        // vettore con la direzione del lato
        Vector3d VettoreDirezione = Estremo2 - Estremo1;
        cout << "Vettore direzione " << VettoreDirezione(0) <<VettoreDirezione(1) <<VettoreDirezione(2) << endl;
        
        // trovo i punti in mezzo ai lati e li memorizzo
        for(unsigned int i = 0; i < b-1; i++){
            Vector3d punto = Estremo1 + VettoreDirezione * (i+1)/(double)(b - 1); 
            cout << "nuovo punto" <<punto << endl;
            
            // metto il punto sulla sfera di raggio 1
            double normaPunto = punto.norm();
            Vector3d puntoNormalizzato = punto/normaPunto;
            cout << "nuovo punto normalizzato "<< puntoNormalizzato(0) << " " << puntoNormalizzato(1) << " " << puntoNormalizzato(2) << " " << endl;

            contIdPunti = contIdPunti + 1;
            cout << "id del nuovo punto "<<contIdPunti << endl;

            // salvo i punti sulle strutture dati 
            mesh2.Cell0DsId.push_back(contIdPunti);
            mesh2.Cell0DsCoordinates(0, contIdPunti) = puntoNormalizzato(0);
            mesh2.Cell0DsCoordinates(1, contIdPunti) = puntoNormalizzato(1);
            mesh2.Cell0DsCoordinates(2, contIdPunti) = puntoNormalizzato(2);
        }


    }
    
    for (int i = 0; i < mesh2.Cell0DsCoordinates.rows(); ++i) {
        for (int j = 0; j < mesh2.Cell0DsCoordinates.cols(); ++j) {
            std::cout << mesh2.Cell0DsCoordinates(i,j) << " ";
        }
        cout << endl;
    }
    return true;
}

}