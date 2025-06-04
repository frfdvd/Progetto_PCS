#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>
#include <string>
#include <map>
#include <queue>
#include "UCDUtilities.hpp"

namespace PolygonalLibrary{
    bool ImportMesh(PolygonalMesh& mesh, const string& Poliedro)
{

    if(!ImportCell0Ds(mesh, Poliedro))
        return false;

    if(!ImportCell1Ds(mesh, Poliedro))
        return false;

    if(!ImportCell2Ds(mesh, Poliedro))
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

bool ImportCell2Ds(PolygonalMesh& mesh, const string& Poliedro){
    string NomeFile="./Cell2D"+Poliedro+".csv";
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

    // salvo e conto il numero di righe
    mesh.NumCell2Ds = listLines.size();

    // verifico che ci siano effettivamente dei lati 
    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.VettoreVertici.reserve(mesh.NumCell2Ds);
    mesh.VettoreLati.reserve(mesh.NumCell2Ds);

    // itero su ogni riga del csv
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string field;
        unsigned int id;
        unsigned int contaVert;
        unsigned int contaLati;
        vector<unsigned int> vecVert;
        vector<unsigned int> vecLati;
        
        // separo in base al ; e salvo i valori 
        getline(converter, field, ';');
        id = stoi(field);

        // mi segno quanto vale il numero di vertici
        getline(converter, field, ';');
        contaVert = stoi(field);
        vecVert.reserve(contaVert);

        // salvo gli id dei vertici dentro un vettore
        for(unsigned int k = 0; k<contaVert;k++){
            getline(converter, field, ';');
            vecVert.push_back(stoi(field));
        }

        mesh.VettoreVertici.push_back(vecVert);

        // mi segno quanto vale il numero di lati
        getline(converter, field, ';');
        contaLati = stoi(field);
        vecLati.reserve(contaLati);

        // salvo gli id dei lati dentro un vettore
        for(unsigned int k = 0; k<contaLati;k++){
            getline(converter, field, ';');
            vecLati.push_back(stoi(field));
        }
        
        mesh.VettoreLati.push_back(vecLati);
        mesh.Cell2DsId.push_back(id);
    }
    
    /*for (const auto& elemento : mesh.mapVertici) {
        std::cout << elemento.first << " -> " << elemento.second << std::endl;
    }

    for (const auto& elemento : mesh.mapLati) {
        std::cout << elemento.first << " -> " << elemento.second << std::endl;
    }*/
    
    return true;
}

/**********************************/

bool TestDuplicati(const MatrixXi& MatriceLati, const unsigned int& id1, const unsigned int& id2, unsigned int* PuntatoreLato){
    
    int id1intero = id1;
    int id2intero = id2;
    for(unsigned int i = 0; i < MatriceLati.cols(); i++){
        if( (MatriceLati(0,i) == id1intero || MatriceLati(1,i) == id1intero) & (MatriceLati(0,i) == id2intero || MatriceLati(1,i) == id2intero) ){
            if(PuntatoreLato != nullptr){
                *PuntatoreLato = i;
            }
            return true;
            break;
        }
    }
    return false;
    
}

/**********************************/

bool TestDuplicatiPunti(const MatrixXd& MatricePunti, const Vector3d& coordinate, unsigned int& idTrovato){
    
    for(unsigned int i = 0; i < MatricePunti.cols(); i++){
        if( (abs(MatricePunti(0,i) - coordinate(0)) <= numeric_limits<double>::epsilon()) & (abs(MatricePunti(1,i) - coordinate(1)) <=  numeric_limits<double>::epsilon()) & (abs(MatricePunti(2,i) - coordinate(2)) <=  numeric_limits<double>::epsilon())){
            idTrovato = i;
            return true;
            break;
        }
    }
    return false;

}

/**********************************/

bool inserisciLati(MatrixXi& MatriceLati, vector<unsigned int> VettoreIdLati, unsigned int& contaIdLati, const unsigned int& id1, const unsigned int& id2){
    if(!TestDuplicati(MatriceLati, id1, id2)){                  
        //cout << "primo lato base " << endl;
        MatriceLati(0, contaIdLati) = id1;
        MatriceLati(1, contaIdLati) = id2;
        VettoreIdLati.push_back(contaIdLati);
    
        cout << "lato " << contaIdLati << " con estremi " << id1 << ", " << id2 << endl;
        contaIdLati += 1;
        //cout << "primo lato base fatto " << endl;
    }

    return true;
}

/**********************************/

bool ProiettaPunti(MatrixXd& MatriceCoordinate){
    for(unsigned int i = 0; i < MatriceCoordinate.cols(); i++){
        Vector3d punto(MatriceCoordinate(0,i),MatriceCoordinate(1,i),MatriceCoordinate(2,i));

        Vector3d puntoNormalizzato = punto/punto.norm();

        MatriceCoordinate(0,i) = puntoNormalizzato(0);
        MatriceCoordinate(1,i) = puntoNormalizzato(1);
        MatriceCoordinate(2,i) = puntoNormalizzato(2);
    }

    return true;
}

/**********************************/

bool ImportTriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b, const unsigned int& q){

    if(!Cell0DTriangolazioneUno(mesh1, mesh2, b, q))
        return false;

    if(!Cell1DTriangolazioneUno(mesh1, mesh2, b))
        return false;

    return true;
}

bool Cell0DTriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b, const unsigned int& q){
    
    unsigned int T = pow(b,2);

    // salvo il numero di vertici, lati e facce usando le formule
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

    // alloco lo spazio necessario per inserire le coordinte e gli id dei vertici
    mesh2.Cell0DsCoordinates = MatrixXd::Zero(3, mesh2.NumCell0Ds);
    mesh2.Cell0DsId.reserve(mesh2.NumCell0Ds);
    
    // inizializzo un contatore per avere gli id dei nuovi vertici
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
        //cout << "id lato "<< id << endl;

        // memorizzo gli estremi del lato e li metto dentro dei vettori
        unsigned int idEstremo1 = mesh1.Cell1DsExtrema(0,id);
        unsigned int idEstremo2 = mesh1.Cell1DsExtrema(1,id);



        Vector3d Estremo1(mesh1.Cell0DsCoordinates(0,idEstremo1),mesh1.Cell0DsCoordinates(1,idEstremo1),mesh1.Cell0DsCoordinates(2,idEstremo1));
        Vector3d Estremo2(mesh1.Cell0DsCoordinates(0,idEstremo2),mesh1.Cell0DsCoordinates(1,idEstremo2),mesh1.Cell0DsCoordinates(2,idEstremo2));
        //cout << "estremo 1 " <<Estremo1(0) << Estremo1(1) << Estremo1(2) << endl;
        //cout << "estremo 2 " <<Estremo2(0) << Estremo2(1) << Estremo2(2) << endl;
        
        // vettore con la direzione del lato
        Vector3d VettoreDirezione = Estremo2 - Estremo1;
        //cout << "Vettore direzione " << VettoreDirezione(0) <<VettoreDirezione(1) <<VettoreDirezione(2) << endl;
        
        // trovo i punti in mezzo ai lati e li memorizzo
        for(unsigned int i = 0; i < b-1; i++){
            Vector3d punto = Estremo1 + VettoreDirezione * (i+1)/(double)(b); 
            //cout << "nuovo punto" <<punto << endl;

            contIdPunti = contIdPunti + 1;
            //cout << "id del nuovo punto "<<contIdPunti << endl;

            // salvo i punti sulle strutture dati 
            mesh2.Cell0DsId.push_back(contIdPunti);
            mesh2.Cell0DsCoordinates(0, contIdPunti) = punto(0);
            mesh2.Cell0DsCoordinates(1, contIdPunti) = punto(1);
            mesh2.Cell0DsCoordinates(2, contIdPunti) = punto(2);
        }
    }
    
    /*for (int i = 0; i < mesh2.Cell0DsCoordinates.rows(); ++i) {
        for (int j = 0; j < mesh2.Cell0DsCoordinates.cols(); ++j) {
            std::cout << mesh2.Cell0DsCoordinates(i,j) << " ";
        }
        cout << endl;
    }*/


    // aggiungo le coordinate dei punti interni alle facce
    for(unsigned int idFaccia : mesh1.Cell2DsId){
        // prendo l'id del primo lato della faccia idFaccia
         
        unsigned int idLato = mesh1.VettoreLati[idFaccia][0];
        cout << idLato << endl;

        // prendo gli id degli estremi del primo lato della faccia idFaccia
        unsigned int idEstremo1 = mesh1.Cell1DsExtrema(0,idLato);
        unsigned int idEstremo2 = mesh1.Cell1DsExtrema(1,idLato);

        // prendo l'id dell'estremo mancante 
        unsigned int idEstremo3 = mesh1.VettoreVertici[idFaccia][2];

        // FARE CONTROLLO/TEST CHE GLI ID SIANO DIVERSI

        // vado a prendere le coordinate dei punti
        Vector3d Estremo1(mesh1.Cell0DsCoordinates(0,idEstremo1),mesh1.Cell0DsCoordinates(1,idEstremo1),mesh1.Cell0DsCoordinates(2,idEstremo1));
        Vector3d Estremo2(mesh1.Cell0DsCoordinates(0,idEstremo2),mesh1.Cell0DsCoordinates(1,idEstremo2),mesh1.Cell0DsCoordinates(2,idEstremo2));
        Vector3d Estremo3(mesh1.Cell0DsCoordinates(0,idEstremo3),mesh1.Cell0DsCoordinates(1,idEstremo3),mesh1.Cell0DsCoordinates(2,idEstremo3));

        // trovo le direzioni lungo le quali muovermi
        Vector3d VettoreDirezione1 = Estremo3 - Estremo1;
        Vector3d VettoreDirezione2 = Estremo3 - Estremo2;

        for(unsigned int i = 0; i < b-1; i++){
            // inizializzo un contatore che calcola in quanti punti devo dividere il lato

            // trovo i punti sui lati nelle direzioni 
            Vector3d puntoDir1 = Estremo1 + VettoreDirezione1 * (i+1)/(double)(b); 
            Vector3d puntoDir2 = Estremo2 + VettoreDirezione2 * (i+1)/(double)(b);
            //cout << "nuovo punto" <<punto << endl;
            
            // trovo la direzione che congiunge i due punti alla stessa altezza
            Vector3d VettoreDirezione3 = puntoDir2 - puntoDir1;

            for(unsigned int k = 0; k < b-2-i; k++){
                // trovo i punti della triangolazione interna
                Vector3d punto = puntoDir1 + VettoreDirezione3 * (k+1)/(double)(b-i-1);
                

                // trovo l'id dei punti continuando il conteggio di prima
                contIdPunti = contIdPunti + 1;

                // inseriamo il punto nuovo nelle strutture dati
                mesh2.Cell0DsId.push_back(contIdPunti);
                mesh2.Cell0DsCoordinates(0, contIdPunti) = punto(0);
                mesh2.Cell0DsCoordinates(1, contIdPunti) = punto(1);
                mesh2.Cell0DsCoordinates(2, contIdPunti) = punto(2);
            }
        }      
    }
    return true;
}

bool Cell1DTriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b){
    
    //calcolo la lunghezza di un lato del poligono e poi di un lato della triangolazione
    Vector3d coord1(mesh1.Cell0DsCoordinates(0,0), mesh1.Cell0DsCoordinates(1,0), mesh1.Cell0DsCoordinates(2,0));
    Vector3d coord2(mesh1.Cell0DsCoordinates(0,1), mesh1.Cell0DsCoordinates(1,1), mesh1.Cell0DsCoordinates(2,1));
    double d = (coord2 - coord1).norm();
    double latotri = d / b ;

    // faccio una copia della matrice con le coordinate
    MatrixXd temp = mesh2.Cell0DsCoordinates;

    //collego i punti che si trovano a distanza latotri
    
    mesh2.Cell1DsId.reserve(mesh2.NumCell1Ds);
    // inizializzo il contatore per l'id dei lati 
    unsigned int contaIdLati = 0;

    // itero sulla matrice che contiene i punti 
    for(unsigned int id : mesh2.Cell0DsId) {
        Vector3d puntocfr(mesh2.Cell0DsCoordinates(0,id), mesh2.Cell0DsCoordinates(1,id), mesh2.Cell0DsCoordinates(2,id));
        // confronto le distanze tra i punti facendo in modo che non esistano duplicati 
        for(unsigned int idTemp = id; idTemp < temp.cols(); idTemp ++) {
            Vector3d puntotemp(mesh2.Cell0DsCoordinates(0,idTemp), mesh2.Cell0DsCoordinates(1,idTemp), mesh2.Cell0DsCoordinates(2,idTemp));
            double distanza = (puntocfr - puntotemp).norm();
            // se la distanza è pari a latotri aggiungo il lato a cell1dextrema
            if (abs(distanza - latotri) < numeric_limits<double>::epsilon()){  //non posso confrontare con 0!!!
                mesh2.Cell1DsId.push_back(contaIdLati);
                mesh2.Cell1DsExtrema(0,contaIdLati) = id;
                mesh2.Cell1DsExtrema(1,contaIdLati) = idTemp;
                contaIdLati = contaIdLati + 1;

            }     
            
        }
    }
    return true;

}

/**********************************/

bool TriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b, const unsigned int& q){
    
    unsigned int T = pow(b,2);

    // salvo il numero di vertici, lati e facce usando le formule
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

    // alloco lo spazio necessario per inserire le coordinte e gli id dei vertici
    mesh2.Cell0DsCoordinates = MatrixXd::Zero(3, mesh2.NumCell0Ds);
    mesh2.Cell0DsId.reserve(mesh2.NumCell0Ds);
    
    // inizializzo un contatore per avere gli id dei nuovi vertici
    unsigned int contIdPunti = mesh1.NumCell0Ds - 1;

    // aggiungo a mesh2 le coordinate dei punti di mesh1
    for(unsigned int id : mesh1.Cell0DsId){
        mesh2.Cell0DsId.push_back(id);
        mesh2.Cell0DsCoordinates(0, id) = mesh1.Cell0DsCoordinates(0, id);
        mesh2.Cell0DsCoordinates(1, id) = mesh1.Cell0DsCoordinates(1, id);
        mesh2.Cell0DsCoordinates(2, id) = mesh1.Cell0DsCoordinates(2, id);
    }

    // contiene dei vettori fatti così [idEstremoTetraedro, ...idPuntiInterni..., idEstremoTetraedro2]
    vector<vector<unsigned int>> latiCompleti; 

    // aggiungo le coordinate nuove (triangolazione) dei punti che stanno sui lati
    for(unsigned int id : mesh1.Cell1DsId){
        //cout << "id lato "<< id << endl;

        // vettore così [idEstremoTetraedro, ...idPuntiInterni..., idEstremoTetraedro2]
        vector<unsigned int> latoCompleto;
        
        // memorizzo gli estremi del lato e li metto dentro dei vettori
        unsigned int idEstremo1 = mesh1.Cell1DsExtrema(0,id);
        unsigned int idEstremo2 = mesh1.Cell1DsExtrema(1,id);


        latoCompleto.push_back(idEstremo1);


        Vector3d Estremo1(mesh1.Cell0DsCoordinates(0,idEstremo1),mesh1.Cell0DsCoordinates(1,idEstremo1),mesh1.Cell0DsCoordinates(2,idEstremo1));
        Vector3d Estremo2(mesh1.Cell0DsCoordinates(0,idEstremo2),mesh1.Cell0DsCoordinates(1,idEstremo2),mesh1.Cell0DsCoordinates(2,idEstremo2));
        //cout << "estremo 1 " <<Estremo1(0) << Estremo1(1) << Estremo1(2) << endl;
        //cout << "estremo 2 " <<Estremo2(0) << Estremo2(1) << Estremo2(2) << endl;
        
        // vettore con la direzione del lato
        Vector3d VettoreDirezione = Estremo2 - Estremo1;
        //cout << "Vettore direzione " << VettoreDirezione(0) <<VettoreDirezione(1) <<VettoreDirezione(2) << endl;
        
        // trovo i punti in mezzo ai lati e li memorizzo
        for(unsigned int i = 0; i < b-1; i++){
            Vector3d punto = Estremo1 + VettoreDirezione * (i+1)/(double)(b); 
            //cout << "nuovo punto" <<punto << endl;

            contIdPunti = contIdPunti + 1;
            //cout << "id del nuovo punto "<<contIdPunti << endl;

            // salvo i punti sulle strutture dati 
            mesh2.Cell0DsId.push_back(contIdPunti);
            mesh2.Cell0DsCoordinates(0, contIdPunti) = punto(0);
            mesh2.Cell0DsCoordinates(1, contIdPunti) = punto(1);
            mesh2.Cell0DsCoordinates(2, contIdPunti) = punto(2);

            latoCompleto.push_back(contIdPunti);
        }

        latoCompleto.push_back(idEstremo2);
        latiCompleti.push_back(latoCompleto);

    }

    for(unsigned int s = 0; s<latiCompleti.size(); s++){
            for(unsigned int y = 0;y<latiCompleti[s].size();y++){
                cout << latiCompleti[s][y] << " ";
            }
        cout << endl;
        }


    for(unsigned int s = 0; s<mesh1.VettoreVertici.size(); s++){
            for(unsigned int y = 0;y<mesh1.VettoreVertici[s].size();y++){
                cout << mesh1.VettoreVertici[s][y] << " ";
            }
        cout << endl;
        }    

    // creo il dizionario che ha come chiave l'id della faccia del poligono iniziale e come valore i lati copleti di quella faccia
    
    map<unsigned int, vector<vector<unsigned int>>> mapFacce;

    for(unsigned int i = 0; i < mesh1.NumCell2Ds; i++){
        unsigned int id1 = mesh1.VettoreVertici[i][0];
        unsigned int id2 = mesh1.VettoreVertici[i][1];
        unsigned int id3 = mesh1.VettoreVertici[i][2];

        vector<vector<unsigned int>> lati;
        for(unsigned int k = 0; k<latiCompleti.size(); k++){
            if( (find(latiCompleti[k].begin(), latiCompleti[k].end(), id1) != latiCompleti[k].end()) & (find(latiCompleti[k].begin(), latiCompleti[k].end(), id2) != latiCompleti[k].end()) ){
                lati.push_back(latiCompleti[k]);
            } 

            if( (find(latiCompleti[k].begin(), latiCompleti[k].end(), id1) != latiCompleti[k].end()) & (find(latiCompleti[k].begin(), latiCompleti[k].end(), id3) != latiCompleti[k].end()) ){
                lati.push_back(latiCompleti[k]);
            }

            if( (find(latiCompleti[k].begin(), latiCompleti[k].end(), id2) != latiCompleti[k].end()) & (find(latiCompleti[k].begin(), latiCompleti[k].end(), id3) != latiCompleti[k].end()) ){
                lati.push_back(latiCompleti[k]);
            }
        }

        mapFacce[i] = lati;
        cout << "id faccia" << i << endl;

        for(unsigned int s = 0; s<lati.size(); s++){
            for(unsigned int y = 0;y<lati[s].size();y++){
                cout << lati[s][y] << " ";
            }
            cout << endl;
        }
        cout << endl;

        lati = {};

    }

    /*iniziamo a triangolare: prendiamo ogni faccia del poligono e i suoi vettori relativi ai lati completi. prendiamo due lati 
    (uno sarà la base e l'altro sarà quello su cui iteriamo) e iniziamo ad aggiungere i punti interni relativi a questi due lati (sfruttando il terzo lato
    per trovare il vettore direzione consono). dopo aver aggiunto i punti interni e aver salvato i segmenti e le facce, saliamo su nell'altezza e usiamo come 
    base quello che era il tetto del passaggio precedente. andiamo avanti così fino a che il tetto non è lungo due a quel punto uniamo i punti con l'estremo superiore*/

    // alloco lo spazio necessario per Cell1DsExtrema
    mesh2.Cell1DsExtrema = MatrixXi::Zero(2, mesh2.NumCell1Ds);
    mesh2.Cell1DsId.reserve(mesh2.NumCell1Ds);
    mesh2.VettoreVertici.reserve(mesh2.NumCell2Ds);
    mesh2.VettoreLati.reserve(mesh2.NumCell2Ds);

    // creo il contatore per contare l'id dei lati di mesh2
    unsigned int ContaIdPuntiMesh2 = contIdPunti + 1;
    unsigned int contaIdLatiMesh2 = 0;

    for(unsigned int idFaccia = 0; idFaccia < mesh1.NumCell2Ds; idFaccia++) {
        cout << endl;
        cout << "FACCIA NUMERO " << idFaccia << endl;
        cout << "il conta punti è arrivato a (ho già aggiunto uno in più) " << ContaIdPuntiMesh2 << endl;;

        // trovo la base e le altezze su cui salire
        vector<unsigned int> base = mapFacce.at(idFaccia)[0];
        vector<unsigned int> lato1 = mapFacce.at(idFaccia)[1];  
        vector<unsigned int> lato2 = mapFacce.at(idFaccia)[2];


        // inverto i vettori per avere compatibilità
        for(unsigned int i = 0; i < base.size(); i++){
            cout << "base " << base[i] << " ";
        }
        cout << endl;

        for(unsigned int i = 0; i < base.size(); i++){
         
            cout << "altezza1 " << lato1[i] << " ";
            
        }
        cout << endl;

        for(unsigned int i = 0; i < base.size(); i++){
            cout << "altezza2 " << lato2[i] << " ";
        }
        cout << endl;

        
        
        
        if ( (lato1[0] != base[0]) & (lato1[0] != base[base.size()-1])){
              reverse( lato1.begin(),  lato1.end());
        }

        for(unsigned int i = 0; i < base.size(); i++){
         
           cout << "altezza1 Nuova " << lato1[i] << " ";
            
        }
        cout << endl;

        if ( (lato2[0] != base[0]) & (lato2[0] != base[base.size()-1])){
              reverse( lato2.begin(),  lato2.end());
        }


        for(unsigned int i = 0; i < base.size(); i++){
            cout << "altezza2 Nuova " << lato2[i] << " ";
        }
        cout << endl;

        // aggiusto i vettori per fare in modo che altezza 1 inizi come base e altezza 2 inizi come finisce base
        vector<unsigned int> altezza1;
        vector<unsigned int> altezza2;
        if(lato1[0] == base[0]){
            altezza1 = lato1;
            altezza2 = lato2;
        }else{
            altezza1 = lato2;
            altezza2 = lato1;
        }



        cout << endl;
        for(unsigned int h = 1; h < altezza1.size()-1; h++){
            // trovo il vettore direzione del tetto
            cout << "mi muovo sulle altezze e h = " << h << endl;
            unsigned int idEstremo1 = altezza1[h];
            unsigned int idEstremo2 = altezza2[h];
            cout << "idEstremo1 " << idEstremo1 << " idEstremo2 " << idEstremo2 << endl;


            // prendiamo le coordinate degli estremi
            Vector3d Estremo1(mesh2.Cell0DsCoordinates(0,idEstremo1),mesh2.Cell0DsCoordinates(1,idEstremo1),mesh2.Cell0DsCoordinates(2,idEstremo1));
            Vector3d Estremo2(mesh2.Cell0DsCoordinates(0,idEstremo2),mesh2.Cell0DsCoordinates(1,idEstremo2),mesh2.Cell0DsCoordinates(2,idEstremo2));
            cout << "estremo 1 " <<Estremo1(0) << " " << Estremo1(1) << " " << Estremo1(2) << endl;
            cout << "estremo 2 " <<Estremo2(0) << " " << Estremo2(1) << " " << Estremo2(2) << endl;
        
            // vettore con la direzione del tetto
            Vector3d VettoreDirezione = Estremo2 - Estremo1;
            cout << "Vettore direzione " << " " << VettoreDirezione(0) << " " <<VettoreDirezione(1) << " " <<VettoreDirezione(2) << endl;


            vector<unsigned int> tetto = {altezza1[h]};
            cout << "tetto ";
            for(unsigned int indice = 0; indice < tetto.size(); indice++){
                cout << tetto[indice] << " "; 
            }
            cout << endl;
            int lunghezzaBase = base.size();
            for(int scorri = 0; scorri < 2*lunghezzaBase-2; scorri ++){
                 
                cout << "scorri " << scorri << endl;
                
                if(scorri % 2 == 0){
                    //stiamo scorrendo sulla base 
                    
                    unsigned int id1 = tetto[tetto.size()-1];
                    unsigned int id2 = base[scorri/2];
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    cout << "primo lato base fatto " << endl;

                    // inseriamo il secondo lato
                    
                    id1 = base[scorri/2];
                    id2 = base[scorri/2+1];
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    cout << "secondo lato base fatto" << endl;

                    // inserisco i vertici della faccia dentro a vettore vertici
                    vector<unsigned int> vecpunti = {base[scorri/2], tetto[tetto.size()-1],base[scorri/2+1]};
                    mesh2.VettoreVertici.push_back(vecpunti);

                }else{
                    // stiamo scorrendo sul tetto

                    // troviamo il nuovo punto di tetto, divido per b-h perchè il tetto è sempre diviso in meno parti, e lo inserisco 
                    if(scorri < 2*lunghezzaBase-5){
                        
                        cout << "SCORRI" <<scorri << "BASE"<< 2*lunghezzaBase-5<< endl;
                        Vector3d NuovoPuntoTetto = Estremo1 + VettoreDirezione * (scorri+1)/(2*(double)(b-h));

                        mesh2.Cell0DsCoordinates(0,ContaIdPuntiMesh2) = NuovoPuntoTetto(0);
                        mesh2.Cell0DsCoordinates(1,ContaIdPuntiMesh2) = NuovoPuntoTetto(1);
                        mesh2.Cell0DsCoordinates(2,ContaIdPuntiMesh2) = NuovoPuntoTetto(2);
                        mesh2.Cell0DsId.push_back(ContaIdPuntiMesh2);
                        
                        cout << "idNuovoPunto " << ContaIdPuntiMesh2 << endl;
                        tetto.push_back(ContaIdPuntiMesh2);
                        ContaIdPuntiMesh2 += 1;
                        // inserisco l'id di questo punto in tetto
                        
                        cout << "tetto ";
                        for(unsigned int indice = 0; indice < tetto.size(); indice++){
                            cout << tetto[indice] << " "; 
                        }
                        cout << endl;

                    }else{ 
                        tetto.push_back(altezza2[h]);
                        
                        cout << "arrivo all'ultimo punto di tetto" << endl;
                        cout << "tetto ";
                        for(unsigned int indice = 0; indice < tetto.size(); indice++){
                            cout << tetto[indice] << " "; 
                        }
                        cout << endl;
                    }

                    // inseriamo il primo lato (tetto con la base)
                    unsigned int id1 = base[scorri+2-tetto.size()];
                    unsigned int id2 = tetto[tetto.size()-2];
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    cout << "primo lato tetto fatto " << endl;

                    // inseriamo il secondo lato (tetto con il tetto) controllando che nell'ultima iterazione non metta tetto-tetto
                    id1 = tetto[tetto.size()-1];
                    id2 = tetto[tetto.size()-2];
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    cout << "secondo lato tetto fatto" << endl;

                    // inseriscoi vertici della faccia dentro al vettore vertici evitando di aggiungere una faccia quando arrivo all'ultimo elemento di tetto
                    if(scorri < 2*lunghezzaBase-3){
                        vector<unsigned int> vecpunti = {tetto[tetto.size()-2],base[scorri+2-tetto.size()],tetto[tetto.size()-1]};
                        mesh2.VettoreVertici.push_back(vecpunti);
                    
                    }


                }
            }       
        

            tetto.pop_back();
            base = tetto;
            for(unsigned int i = 0; i < base.size(); i++){
                cout << "base " << base[i] << " ";
            }
            cout << endl; 
    
        }        

    // inserisco a mano gli ultimi due lati
    unsigned int id1 = altezza1[altezza1.size()-1];
    unsigned int id2 = base[0];
    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
    cout << "primo lato tetto fatto " << endl;

    id1 = altezza1[altezza1.size()-1];
    id2 = base[1];
    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
    cout << "primo lato tetto fatto " << endl;

    // inserisco l'ultima faccia
    vector<unsigned int> vecpunti = {base[0],altezza1[altezza1.size()-1],base[1]};
    mesh2.VettoreVertici.push_back(vecpunti);

    
    }


    cout << endl;
    for (int i = 0; i < mesh2.Cell0DsCoordinates.cols(); i++) {
        for (int j = 0; j < mesh2.Cell0DsCoordinates.rows(); j++) {
            std::cout << mesh2.Cell0DsCoordinates(j, i) << " ";
        }
        std::cout << std::endl;
    }


    for (int i = 0; i < mesh2.Cell1DsExtrema.rows(); i++) {
        for (int j = 0; j < mesh2.Cell1DsExtrema.cols(); j++) {
            std::cout << mesh2.Cell1DsExtrema(i, j) << " ";
        }
        std::cout << std::endl;
    }

    cout << "stampo il vettore dei vertici" << endl;
    for (const auto& riga : mesh2.VettoreVertici) {
        for (const auto& elemento :riga) {
            cout << elemento << " ";
        }
        cout << endl;
    }


    // inserisco nel vettore lati i lati relativi a ogni faccia in ordine
    for(vector<unsigned int> vertici : mesh2.VettoreVertici){
        int id1 = vertici[0];
        int id2 = vertici[1];
        int id3 = vertici[2];

        vector<unsigned int> vettoreAggiuntivo;
        vettoreAggiuntivo = {0,0,0};
        
        // con questi if faccio in modo da mettere i lati in posizione corretta
        for(unsigned int idl = 0; idl < mesh2.Cell1DsExtrema.cols(); idl++){
            if( ( mesh2.Cell1DsExtrema(0,idl) == id1 ||  mesh2.Cell1DsExtrema(1,idl) == id1) & ( mesh2.Cell1DsExtrema(0,idl) == id2 ||  mesh2.Cell1DsExtrema(1,idl) == id2) ){
                vettoreAggiuntivo[0] = idl;
            }else if( ( mesh2.Cell1DsExtrema(0,idl) == id2 ||  mesh2.Cell1DsExtrema(1,idl) == id2) & ( mesh2.Cell1DsExtrema(0,idl) == id3 ||  mesh2.Cell1DsExtrema(1,idl) == id3) ){
                vettoreAggiuntivo[1] = idl;
            }else if(( mesh2.Cell1DsExtrema(0,idl) == id1 ||  mesh2.Cell1DsExtrema(1,idl) == id1) & ( mesh2.Cell1DsExtrema(0,idl) == id3 ||  mesh2.Cell1DsExtrema(1,idl) == id3)){
                vettoreAggiuntivo[2] = idl; 
            }
                
                
        }
        mesh2.VettoreLati.push_back(vettoreAggiuntivo);
    }
    
      
    cout << "stampo il vettore dei lati" << endl;
    unsigned int contatore = 0;
    for (const auto& riga : mesh2.VettoreLati) {
        cout << "faccia " << contatore << " ";
        contatore += 1;
        for (const auto& elemento :riga) {
            cout << elemento << " ";
        }
        cout << endl;
    }


    // metto i punti di cell0dscoordinates su una sfera

    ProiettaPunti(mesh2.Cell0DsCoordinates);

return true;
}

/**********************************/

bool CreaDuale(const PolygonalMesh& mesh1, PolygonalMesh& mesh2){

    // creo un vector di vector con dentro l'id delle facce adiacenti per ogni id di un vertice
    vector<vector<unsigned int>> facceAdiacenti;
    facceAdiacenti.reserve(mesh1.NumCell0Ds);

    // cerco, per ogni vertice, chi sono le facce adiacenti 
    for(unsigned int idV = 0; idV < mesh1.NumCell0Ds; idV++){
        // per ogni vertice itero sulle facce per scoprire se sono adiacenti    
        vector<unsigned int> faccePerVertice;
        //cout << "entro nel primo for" << endl;
        
        // il massimo è sempre sei così siamo sicuri
        faccePerVertice.reserve(6);
        for(unsigned int idF = 0; idF < mesh1.NumCell2Ds; idF++){
            //cout << "entro nel secondo for" << endl;
            if ( find(mesh1.VettoreVertici[idF].begin(), mesh1.VettoreVertici[idF].end(), idV) != mesh1.VettoreVertici[idF].end()){
                // se c'è il vertice dentro la faccia inserisco la faccia nel vettore
                faccePerVertice.push_back(idF);
                //cout << "entro nell'if" << endl;
            } 
        }


        // metto in ordine le facce per vertice affinchè siano tutte confinanti
        vector<unsigned int> faccePerVerticeOrdinate;
        unsigned int NumeroFacceXVertice = faccePerVertice.size();
        faccePerVerticeOrdinate.reserve(NumeroFacceXVertice);
        faccePerVerticeOrdinate.push_back(faccePerVertice[0]);


        for(unsigned int i = 0; i < NumeroFacceXVertice-1; i++){
            // i è l'elemento 1 da confrontare
            unsigned int id1 = faccePerVerticeOrdinate[i];

            // rimuovo l'id appena preso così faccio un confronto solo sugli altri
            faccePerVertice.erase(remove(faccePerVertice.begin(), faccePerVertice.end(), id1), faccePerVertice.end());
            cout << "faccia 1 " << id1 << endl;
            for(unsigned int j = 0; j<faccePerVertice.size(); j++){
                // j è l'elemento 2 da confrontare
                unsigned int id2 = faccePerVertice[j];
                cout << "faccia da confrontare " << id2 << endl;
                // trovo i lati delle due facce id1 e id2
                vector<unsigned int> latiId1 = mesh1.VettoreLati[id1];
                vector<unsigned int> latiId2 = mesh1.VettoreLati[id2];

                // scorro sui lati della prima faccia e vedo se trovo elementi in comune
                bool Trovato = false;
                for(unsigned int Lato : latiId1){
                    if( find(latiId2.begin(), latiId2.end(), Lato) != latiId2.end()){
                        Trovato = true;
                    }
                }

                if(Trovato){
                    faccePerVerticeOrdinate.push_back(id2);
                    cout << "facce per vertice ordinate ";
                    for(unsigned int s = 0; s < faccePerVerticeOrdinate.size(); s++){
                        cout << faccePerVerticeOrdinate[s] << " ";
                    }
                    cout << endl;
                    break;
                } 
            }
            
        }
        
        
        facceAdiacenti.push_back(faccePerVerticeOrdinate);
    }

    cout << "stampo il vettore delle facce adiacenti " << endl;
    for (const auto& riga : facceAdiacenti) {
        for (const auto& elemento :riga) {
            cout << elemento << " ";
        }
        cout << endl;
    }

    // inizializziamo gli elementi di mesh2 (quella del duale)
    mesh2.NumCell0Ds = mesh1.NumCell2Ds;
    mesh2.NumCell1Ds = mesh1.NumCell1Ds;
    mesh2.NumCell2Ds = mesh1.NumCell0Ds;


    mesh2.Cell0DsCoordinates = MatrixXd::Zero(3, mesh2.NumCell0Ds);
    mesh2.Cell1DsExtrema = MatrixXi::Zero(2, mesh2.NumCell1Ds);
    mesh2.Cell0DsId.reserve(mesh2.NumCell0Ds);
    mesh2.Cell1DsId.reserve(mesh2.NumCell1Ds);
    mesh2.Cell2DsId.reserve(mesh2.NumCell2Ds);

    // inizializzo un contatore per i punti
    unsigned int contaIdPunti = 0;

    // itero sui vettori di facce adiacenti
    for(unsigned int idV = 0; idV < facceAdiacenti.size(); idV++){
        cout << "id Vertice" << idV << endl;
        // inizializzo un vettore dove inserire, per ogni faccia del duale, i suoi vertici
        vector<unsigned int> vecVert;
        // tale vettore deve essere lungo quanto il numero di facce andiacenti a ogni vertice
        vecVert.reserve(facceAdiacenti[idV].size());

        // ora itero sulle facce dentro i vettori
        for(unsigned int idF : facceAdiacenti[idV]){
            cout << "id Faccia " << idF << endl;
            // vettore contenente i vertici della faccia
            vector<unsigned int> verticiPerFaccia = mesh1.VettoreVertici[idF];
            unsigned int idPunto1 = verticiPerFaccia[0];
            unsigned int idPunto2 = verticiPerFaccia[1];
            unsigned int idPunto3 = verticiPerFaccia[2];

            cout << "id punti della faccia " << idPunto1 << " " << idPunto2 << " " <<  idPunto3 << " " << endl;

            Vector3d punto1(mesh1.Cell0DsCoordinates(0,idPunto1),mesh1.Cell0DsCoordinates(1,idPunto1),mesh1.Cell0DsCoordinates(2,idPunto1));
            Vector3d punto2(mesh1.Cell0DsCoordinates(0,idPunto2),mesh1.Cell0DsCoordinates(1,idPunto2),mesh1.Cell0DsCoordinates(2,idPunto2));
            Vector3d punto3(mesh1.Cell0DsCoordinates(0,idPunto3),mesh1.Cell0DsCoordinates(1,idPunto3),mesh1.Cell0DsCoordinates(2,idPunto3));

            // calcolo il punto della figura duale
            Vector3d Baricentro = (punto1 + punto2 + punto3)/3;
            // se il punto non c'è già lo aggiungo
            unsigned int idPuntoDuale;
            if(!TestDuplicatiPunti(mesh2.Cell0DsCoordinates, Baricentro, idPuntoDuale)){    
                // inserisco il nuovo punto nella matrice con le coordinate
                mesh2.Cell0DsCoordinates(0, contaIdPunti) = Baricentro(0);
                mesh2.Cell0DsCoordinates(1, contaIdPunti) = Baricentro(1);
                mesh2.Cell0DsCoordinates(2, contaIdPunti) = Baricentro(2);
                mesh2.Cell0DsId.push_back(contaIdPunti);
                // inserisco nel VettoreVertici l'id del nuovo punto
                vecVert.push_back(contaIdPunti);

                contaIdPunti += 1;
                cout << "entrato nell'if" << endl;
            }else{
                // nel caso in cui il punto era già stato messo aggiungo il suo id
                vecVert.push_back(idPuntoDuale);
            }
        }

        // aggiungo al vettore della mesh il vettore che contiene i vertici di ogni faccia
        mesh2.VettoreVertici.push_back(vecVert);
        mesh2.Cell2DsId.push_back(idV);
    }


    // adesso riempio le strutture dati che contengono informazioni sui lati
    // devo riempire Cell1DsExtrema e VettoreLati 



    unsigned int contaIdLati = 0;
    for(vector<unsigned int> vertici : mesh2.VettoreVertici){
        // prendo la lista dei vertici di ogni faccia
        // scorro sui vertici di ogni faccia
        // i vertici di ogni faccia sono ordinati in modo che il successivo sia il vicino di quello prima
        vector<unsigned int> vecLati;
        vecLati.reserve(vertici.size());

        for(unsigned int i = 0; i < vertici.size(); i++){
            unsigned int idPunto1 = vertici[i];
            unsigned int idPunto2;

            // trovo l'id del seocondo punto, se mi trovo nell'ultima posizione prendo il primo 
            if(i < vertici.size()-1){
                idPunto2 = vertici[i+1];
            }else{
                idPunto2 = vertici[0];
            }

            unsigned int idLatoTrovato;
            // inserisco il nuovo punto dopo aver controllato che non esista già
            if(!TestDuplicati(mesh2.Cell1DsExtrema, idPunto1, idPunto2, &idLatoTrovato)){
                mesh2.Cell1DsExtrema(0, contaIdLati) = idPunto1;
                mesh2.Cell1DsExtrema(1, contaIdLati) = idPunto2;
                mesh2.Cell1DsId.push_back(contaIdLati);
                vecLati.push_back(contaIdLati);
                contaIdLati += 1;
            }else{
                vecLati.push_back(idLatoTrovato);   
            }
        } 

        mesh2.VettoreLati.push_back(vecLati);

    }

    // poietto i punti sulla sfera
    ProiettaPunti(mesh2.Cell0DsCoordinates);

    return true;
}

/**********************************/

bool Dijkstra(const unsigned int& n,const vector<vector<unsigned int>>& LA, const unsigned int& start, const unsigned int& end, MatrixXd& matrice, vector<unsigned int>& path){
    vector<int> pred;
    vector<double> dist;
    pred.reserve(n);
    dist.reserve(n);
    double inf = numeric_limits<double>::infinity();
   
    for(unsigned int i = 0; i < n; i++){
        pred.push_back(-1);
        dist.push_back(inf);
    }

    pred[start] = start;
    dist[start] = 0.0;
    cout << "ok1" << endl;
    
    // creo la coda con priorità, sarà ordinata in ordine crescente
    priority_queue<pair<int, double>, vector<pair<int, double>>, greater<pair<int, double>>> PQ;
    for(unsigned int i = 0; i < n; i++){
		PQ.push({i, dist[i]});
    }
    cout << "ok2" << endl;

    while(!PQ.empty()){
        int u = PQ.top().first;
		int p = PQ.top().second;
		PQ.pop();
        for(unsigned int w : LA[u]){
            if(dist[w] > dist[u] + matrice(u,w)){
                dist[w] = dist[u] + matrice(u,w);
                pred[w] = u;
                PQ.push({w,dist[w]});
            }
        }
    }
    cout << "pred " << endl;
    for(unsigned int elemento : pred){
        cout << elemento << " ";
    }
    cout << endl;

    unsigned int v = end;
	while(v != start){
		path.push_back(v);
		v = pred[v];
	} 
    path.push_back(start);

    cout << "path " << endl;
    for(unsigned int elemento : path){
        cout << elemento << " ";
    }
    cout << endl;

    cout << "ok4" << endl;
    return true;

}

/**********************************/

bool CamminoMinimo(const PolygonalMesh& mesh, const unsigned int& id1, const unsigned int& id2, const string& nomefilepunti, const string& nomefilelati){

    //controllo che gli id dei vertici passati esistano 
    if( (id1 > mesh.NumCell0Ds) && (id2 > mesh.NumCell0Ds) ){
        cerr << "gli id non sono validi" << endl;
        return false;
    }

    // creo la lista di adiacenza
    vector<vector<unsigned int>> listaAdiacenza;
    listaAdiacenza.reserve(mesh.NumCell0Ds);

    // ora la riempio
    for(unsigned int idVertice = 0; idVertice < mesh.NumCell0Ds; idVertice++){
        vector<unsigned int> vettoreAdiacenza;

        for(unsigned int idAdiacente = 0; idAdiacente < mesh.Cell1DsExtrema.cols(); idAdiacente++){
            int idVerticeIntero = idVertice;
            if( idVerticeIntero == mesh.Cell1DsExtrema(0, idAdiacente)){
                cout << "entrato nell'if" << endl;
                vettoreAdiacenza.push_back(mesh.Cell1DsExtrema(1, idAdiacente));
                cout << "uscito nell'if" << endl;
            } 
            
            if(idVerticeIntero == mesh.Cell1DsExtrema(1, idAdiacente)){
                cout << "entrato nell'if" << endl;
                vettoreAdiacenza.push_back(mesh.Cell1DsExtrema(0, idAdiacente));
                cout << "uscito nell'if" << endl;
            }
        }
        listaAdiacenza.push_back(vettoreAdiacenza);
    }
   
    cout << "stampo il lista adiacenza " << endl;
    for (const auto& riga : listaAdiacenza) {
        for (const auto& elemento :riga) {
            cout << elemento << " ";
        }
        cout << endl;
    }
    //calcolo la matrice di adiacenza
    MatrixXd matricePesi = MatrixXd::Zero(mesh.NumCell0Ds, mesh.NumCell0Ds);
    
    for(unsigned int v = 0; v < listaAdiacenza.size(); v++) {
        cout << "v = " << v << endl;
        for(unsigned int v1 :listaAdiacenza[v]) {
            cout << "v1 = " << v1 << endl;
            Vector3d punto(mesh.Cell0DsCoordinates(0, v), mesh.Cell0DsCoordinates(1, v), mesh.Cell0DsCoordinates(2, v));
            Vector3d punto1(mesh.Cell0DsCoordinates(0, v1), mesh.Cell0DsCoordinates(1, v1), mesh.Cell0DsCoordinates(2, v1));
            double dist = (punto1 - punto).norm();
            matricePesi(v, v1) = dist;
        }
    }

    // iniziamo l'algoritmo
    vector<unsigned int> path;
    path.reserve(mesh.NumCell0Ds);
    Dijkstra(mesh.NumCell0Ds, listaAdiacenza, id1, id2, matricePesi, path);

    // coloriamo i punti relativi al percorso
    vector<double> ProprietaPuntiPercorso(mesh.NumCell0Ds, 0.0);
	for(unsigned int punto : path){
		ProprietaPuntiPercorso[punto] = 1.0;
    }

    cout << "ok5" << endl;
    
    Gedim::UCDProperty<double> ProprietaPercorsoMinimo;
    ProprietaPercorsoMinimo.Label = "percorso minimo";
    ProprietaPercorsoMinimo.UnitLabel = "";
    ProprietaPercorsoMinimo.Size = ProprietaPuntiPercorso.size();
    ProprietaPercorsoMinimo.NumComponents = 1;
    ProprietaPercorsoMinimo.Data = ProprietaPuntiPercorso.data();

    vector<Gedim::UCDProperty<double>> ProprietaPunti;
	ProprietaPunti.push_back(ProprietaPercorsoMinimo);

    Gedim::UCDUtilities utilities;
	utilities.ExportPoints(nomefilepunti,
						    mesh.Cell0DsCoordinates,
							ProprietaPunti);


    // coloriamo i lati relativi al percorso e calcoliamo la lunghezza del percorso
    vector<unsigned int> latiPercorso;
    //latiPercorso.reserve(path.size()-1);
    vector<double> ProprietaLatiPercorso(mesh.NumCell1Ds, 0.0);

    cout << "ok6" << endl;
    cout << path.size() << endl;
    double lunghezzaPercorso = 0.0;
    for(unsigned int i = 0; i < path.size()-1; i++){
        unsigned int punto1 = path[i];
        unsigned int punto2 = path[i+1];

        lunghezzaPercorso += matricePesi(punto1, punto2);
        for( unsigned int idLato = 0; idLato < mesh.Cell1DsExtrema.cols(); idLato++){
            int punto1Intero = punto1;
            int punto2Intero = punto2;
            if( (mesh.Cell1DsExtrema(0,idLato) == punto1Intero && mesh.Cell1DsExtrema(1,idLato) == punto2Intero) || (mesh.Cell1DsExtrema(0,idLato) == punto2Intero && mesh.Cell1DsExtrema(1,idLato) == punto1Intero) ){
                latiPercorso.push_back(idLato);
                ProprietaLatiPercorso[idLato] = 1.0;
                
            }
        } 
    }



    ProprietaPercorsoMinimo.Label = "Percorso minimo";
    ProprietaPercorsoMinimo.Size = ProprietaLatiPercorso.size();
    ProprietaPercorsoMinimo.NumComponents = 1;
    ProprietaPercorsoMinimo.Data = ProprietaLatiPercorso.data();  

    vector<Gedim::UCDProperty<double>> EdgesProperties;
    EdgesProperties.push_back(ProprietaPercorsoMinimo);
    utilities.ExportSegments(nomefilelati,
                    mesh.Cell0DsCoordinates,
                    mesh.Cell1DsExtrema,
                    {},
                    EdgesProperties);


return true;
}

/**********************************/

bool CreaBaricentro(PolygonalMesh& meshTri, const PolygonalMesh& mesh2 ,const vector<unsigned int>& vecpunti, const unsigned int& IdBar, map<unsigned int, vector<unsigned int>>& map){
    unsigned int id0 = vecpunti[0];
    unsigned int id1 = vecpunti[1];
    unsigned int id2 = vecpunti[2];

    Vector3d punto0(mesh2.Cell0DsCoordinates(0,id0),mesh2.Cell0DsCoordinates(1,id0),mesh2.Cell0DsCoordinates(2,id0));
    Vector3d punto1(mesh2.Cell0DsCoordinates(0,id1),mesh2.Cell0DsCoordinates(1,id1),mesh2.Cell0DsCoordinates(2,id1));
    Vector3d punto2(mesh2.Cell0DsCoordinates(0,id2),mesh2.Cell0DsCoordinates(1,id2),mesh2.Cell0DsCoordinates(2,id2));
    
    Vector3d baricentro;
    baricentro = (punto0 + punto1 + punto2)/3;

    meshTri.Cell0DsCoordinates(0, IdBar) = baricentro(0);
    meshTri.Cell0DsCoordinates(1, IdBar) = baricentro(1);
    meshTri.Cell0DsCoordinates(2, IdBar) = baricentro(2);
    meshTri.Cell0DsId.push_back(IdBar);

    map[IdBar] = vecpunti;
    cout << "id dek baricentro " << IdBar << endl;
    return true;
}

/**********************************/

bool ControllaBordi(const vector<vector<unsigned int>>& latiCompleti, const unsigned int& id1, const unsigned int& id2, const MatrixXd& Coordinate, Vector3d& Medio, PolygonalMesh& meshTri, const unsigned int& contapunti){
    for(vector<unsigned int> lato : latiCompleti){
        if( (find(lato.begin(), lato.end(), id1) != lato.end()) & (find(lato.begin(), lato.end(), id2) != lato.end()) ){
            Vector3d punto1(Coordinate(0,id1),Coordinate(1,id1),Coordinate(2,id1));
            Vector3d punto2(Coordinate(0,id2),Coordinate(1,id2),Coordinate(2,id2));
            Medio = (punto1 + punto2)/2;
            unsigned int idTrovato;
            if(TestDuplicatiPunti(Coordinate, Medio, idTrovato)){
                return false;
                break;
            }else{
                meshTri.Cell0DsCoordinates(0, contapunti) = Medio(0);
                meshTri.Cell0DsCoordinates(1, contapunti) = Medio(1);
                meshTri.Cell0DsCoordinates(2, contapunti) = Medio(2);
                meshTri.Cell0DsId.push_back(contapunti);
                return true;
                break;
            }
        }
    }
    return false;
}

/**********************************/

bool AggiungiLati(PolygonalMesh& meshTri,const vector<unsigned int>& vecpunti, const unsigned int& IdBar, unsigned int& contatore){
    int idBarIntero = IdBar;
    for(unsigned int idPunto : vecpunti){
        if(!TestDuplicati(meshTri.Cell1DsExtrema, IdBar, idPunto)){
            int idPuntoIntero = idPunto;
            meshTri.Cell1DsExtrema(0, contatore) = idBarIntero;
            meshTri.Cell1DsExtrema(1, contatore) = idPuntoIntero;
            meshTri.Cell1DsId.push_back(contatore);
            contatore += 1;
        }
    }
    return true;
}

/**********************************/

bool CollegaBaricentri(const map<unsigned int, vector<unsigned int>>& MapBaricentri,const unsigned int& IdBaricentro, PolygonalMesh& meshTri, unsigned int& contaLati){

    vector<unsigned int> vettore1 = MapBaricentri.at(IdBaricentro);
    unsigned int quanti = 0;
    for(const auto& chiave : MapBaricentri){
        vector<unsigned int> vettore2 = chiave.second;
        for(unsigned int elemento1 : vettore1){
            for(unsigned int elemento2 : vettore2){
                if(elemento1 == elemento2){
                    quanti += 1;
                }
            }
        }

        if(quanti == 2){
            // aggiungiamo il lato che collega i baricenti
            meshTri.Cell1DsExtrema(0, contaLati) = IdBaricentro;
            meshTri.Cell1DsExtrema(1, contaLati) = chiave.first;
            meshTri.Cell1DsId.push_back(contaLati);
            contaLati += 1;
        }

        quanti = 0;
    }

    

    return true;
}


/**********************************/

bool TriangolazioneDue(const PolygonalMesh& mesh1, PolygonalMesh& meshTri, const unsigned int& b, const unsigned int& q){
    
    // mesh temporanea che permette di creare la triangolazione di tipo due
    PolygonalMesh mesh2;
    unsigned int T = pow(b,2);

    // salvo il numero di vertici, lati e facce usando le formule per la triangolazione temporanea
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

    // salvo il numero di vertici, lati e facce usando le formule per la triamgolazione di tipo due
    meshTri.NumCell0Ds = mesh1.NumCell0Ds + mesh1.NumCell1Ds*(2*b - 1) + mesh1.NumCell2Ds*( (3*T - 3*b + 2)/2 );
    meshTri.NumCell1Ds = mesh1.NumCell1Ds*(2*b) + mesh1.NumCell2Ds*((9*T+3*b)/2);
    meshTri.NumCell2Ds = mesh1.NumCell2Ds*(3*T+3*b);



    // alloco lo spazio necessario per inserire le coordinte e gli id dei vertici per la mesh temporanea
    mesh2.Cell0DsCoordinates = MatrixXd::Zero(3, mesh2.NumCell0Ds);
    mesh2.Cell0DsId.reserve(mesh2.NumCell0Ds);
    
    // alloco lo spazio necessario per inserire le coordinte e gli id dei vertici per la mesh di tipo due
    meshTri.Cell0DsCoordinates = MatrixXd::Zero(3, meshTri.NumCell0Ds);
    meshTri.Cell0DsId.reserve(meshTri.NumCell0Ds); 



    // inizializzo un contatore per avere gli id dei nuovi vertici
    unsigned int contIdPunti = mesh1.NumCell0Ds - 1;

    // aggiungo a mesh2 temporanea le coordinate dei punti di mesh1
    for(unsigned int id : mesh1.Cell0DsId){
        mesh2.Cell0DsId.push_back(id);
        mesh2.Cell0DsCoordinates(0, id) = mesh1.Cell0DsCoordinates(0, id);
        mesh2.Cell0DsCoordinates(1, id) = mesh1.Cell0DsCoordinates(1, id);
        mesh2.Cell0DsCoordinates(2, id) = mesh1.Cell0DsCoordinates(2, id);
    }

    // faccio lo stesso per la mesh tri
    for(unsigned int id : mesh1.Cell0DsId){
        meshTri.Cell0DsId.push_back(id);
        meshTri.Cell0DsCoordinates(0, id) = mesh1.Cell0DsCoordinates(0, id);
        meshTri.Cell0DsCoordinates(1, id) = mesh1.Cell0DsCoordinates(1, id);
        meshTri.Cell0DsCoordinates(2, id) = mesh1.Cell0DsCoordinates(2, id);
    }


    // contiene dei vettori fatti così [idEstremoTetraedro, ...idPuntiInterni..., idEstremoTetraedro2]
    vector<vector<unsigned int>> latiCompleti; 
    // aggiungo le coordinate nuove (triangolazione) dei punti che stanno sui lati
    for(unsigned int id : mesh1.Cell1DsId){
        //cout << "id lato "<< id << endl;

        // vettore così [idEstremoTetraedro, ...idPuntiInterni..., idEstremoTetraedro2]
        vector<unsigned int> latoCompleto;
        
        // memorizzo gli estremi del lato e li metto dentro dei vettori
        unsigned int idEstremo1 = mesh1.Cell1DsExtrema(0,id);
        unsigned int idEstremo2 = mesh1.Cell1DsExtrema(1,id);


        latoCompleto.push_back(idEstremo1);


        Vector3d Estremo1(mesh1.Cell0DsCoordinates(0,idEstremo1),mesh1.Cell0DsCoordinates(1,idEstremo1),mesh1.Cell0DsCoordinates(2,idEstremo1));
        Vector3d Estremo2(mesh1.Cell0DsCoordinates(0,idEstremo2),mesh1.Cell0DsCoordinates(1,idEstremo2),mesh1.Cell0DsCoordinates(2,idEstremo2));
        //cout << "estremo 1 " <<Estremo1(0) << Estremo1(1) << Estremo1(2) << endl;
        //cout << "estremo 2 " <<Estremo2(0) << Estremo2(1) << Estremo2(2) << endl;
        
        // vettore con la direzione del lato
        Vector3d VettoreDirezione = Estremo2 - Estremo1;
        //cout << "Vettore direzione " << VettoreDirezione(0) <<VettoreDirezione(1) <<VettoreDirezione(2) << endl;
        
        // trovo i punti in mezzo ai lati e li memorizzo
        for(unsigned int i = 0; i < b-1; i++){
            Vector3d punto = Estremo1 + VettoreDirezione * (i+1)/(double)(b); 
            //cout << "nuovo punto" <<punto << endl;

            contIdPunti = contIdPunti + 1;
            //cout << "id del nuovo punto "<<contIdPunti << endl;

            // salvo i punti sulla mesh2
            mesh2.Cell0DsId.push_back(contIdPunti);
            mesh2.Cell0DsCoordinates(0, contIdPunti) = punto(0);
            mesh2.Cell0DsCoordinates(1, contIdPunti) = punto(1);
            mesh2.Cell0DsCoordinates(2, contIdPunti) = punto(2);

            // salvo i punti per la tirangolazione di tipo due
            meshTri.Cell0DsId.push_back(contIdPunti);
            meshTri.Cell0DsCoordinates(0, contIdPunti) = punto(0);
            meshTri.Cell0DsCoordinates(1, contIdPunti) = punto(1);
            meshTri.Cell0DsCoordinates(2, contIdPunti) = punto(2);

            latoCompleto.push_back(contIdPunti);
        }

        latoCompleto.push_back(idEstremo2);
        latiCompleti.push_back(latoCompleto);

    }

    // stampiamo lati completi
    for(unsigned int s = 0; s<latiCompleti.size(); s++){
            for(unsigned int y = 0;y<latiCompleti[s].size();y++){
                cout << latiCompleti[s][y] << " ";
            }
        cout << endl;
        }

    // stampiamo vettore vertici per vedere se abbiamo fatto giusto
    for(unsigned int s = 0; s<mesh1.VettoreVertici.size(); s++){
            for(unsigned int y = 0;y<mesh1.VettoreVertici[s].size();y++){
                cout << mesh1.VettoreVertici[s][y] << " ";
            }
        cout << endl;
        }    

    // creo il dizionario che ha come chiave l'id della faccia del poligono iniziale e come valore i lati copleti di quella faccia
    vector<vector<vector<unsigned int>>> vecFacce;

    for(unsigned int i = 0; i < mesh1.NumCell2Ds; i++){
        unsigned int id1 = mesh1.VettoreVertici[i][0];
        unsigned int id2 = mesh1.VettoreVertici[i][1];
        unsigned int id3 = mesh1.VettoreVertici[i][2];

        vector<vector<unsigned int>> lati;
        for(unsigned int k = 0; k<latiCompleti.size(); k++){
            if( (find(latiCompleti[k].begin(), latiCompleti[k].end(), id1) != latiCompleti[k].end()) & (find(latiCompleti[k].begin(), latiCompleti[k].end(), id2) != latiCompleti[k].end()) ){
                lati.push_back(latiCompleti[k]);
            } 

            if( (find(latiCompleti[k].begin(), latiCompleti[k].end(), id1) != latiCompleti[k].end()) & (find(latiCompleti[k].begin(), latiCompleti[k].end(), id3) != latiCompleti[k].end()) ){
                lati.push_back(latiCompleti[k]);
            }

            if( (find(latiCompleti[k].begin(), latiCompleti[k].end(), id2) != latiCompleti[k].end()) & (find(latiCompleti[k].begin(), latiCompleti[k].end(), id3) != latiCompleti[k].end()) ){
                lati.push_back(latiCompleti[k]);
            }
        }

        vecFacce.push_back(lati);
        cout << "id faccia" << i << endl;

        for(unsigned int s = 0; s<lati.size(); s++){
            for(unsigned int y = 0;y<lati[s].size();y++){
                cout << lati[s][y] << " ";
            }
            cout << endl;
        }
        cout << endl;

        lati = {};

    }

    /*iniziamo a triangolare: prendiamo ogni faccia del poligono e i suoi vettori relativi ai lati completi. prendiamo due lati 
    (uno sarà la base e l'altro sarà quello su cui iteriamo) e iniziamo ad aggiungere i punti interni relativi a questi due lati (sfruttando il terzo lato
    per trovare il vettore direzione consono). dopo aver aggiunto i punti interni e aver salvato i segmenti e le facce, saliamo su nell'altezza e usiamo come 
    base quello che era il tetto del passaggio precedente. andiamo avanti così fino a che il tetto non è lungo due a quel punto uniamo i punti con l'estremo superiore*/

    // alloco lo spazio necessario per Cell1DsExtrema per la mesh temporanea
    mesh2.Cell1DsExtrema = MatrixXi::Zero(2, mesh2.NumCell1Ds);
    mesh2.Cell1DsId.reserve(mesh2.NumCell1Ds);
    mesh2.VettoreVertici.reserve(mesh2.NumCell2Ds);
    mesh2.VettoreLati.reserve(mesh2.NumCell2Ds);

    // alloco lo spazio necessario per la mesh tirangolazione tipo due
    meshTri.Cell1DsExtrema = MatrixXi::Zero(2, meshTri.NumCell1Ds);
    meshTri.Cell1DsId.reserve(meshTri.NumCell1Ds);
    meshTri.VettoreVertici.reserve(meshTri.NumCell2Ds);
    meshTri.VettoreLati.reserve(meshTri.NumCell2Ds);

    // creo il contatore per contare l'id dei lati di mesh2 temporanea
    unsigned int ContaIdPuntiMesh2 = contIdPunti + 1;
    unsigned int contaIdLatiMesh2 = 0;

    // creo il contatore per contare l'id dei lati di meshTri
    unsigned int IdPuntiMeshTri = contIdPunti + 1;
    unsigned int IdLatiMeshTri = 0;
    // creo una map che contiene come chiavi gli id dei baricentri e come valore un vettore che contiene gli id dei punti che l'hanno generato
    map<unsigned int, vector<unsigned int>> MapBaricentri;

    for(unsigned int idFaccia = 0; idFaccia < mesh1.NumCell2Ds; idFaccia++) {
        cout << endl;
        cout << "FACCIA NUMERO " << idFaccia << endl;
        cout << "il conta punti è arrivato a (ho già aggiunto uno in più) " << ContaIdPuntiMesh2 << endl;;

        // trovo la base e le altezze su cui salire, CAMBIARE IL DIZIONARIO IN UN VECTOR DI VECTOR
        vector<unsigned int> base = vecFacce[idFaccia][0];
        vector<unsigned int> lato1 = vecFacce[idFaccia][1];  
        vector<unsigned int> lato2 = vecFacce[idFaccia][2];


        // inverto i vettori per avere compatibilità
        for(unsigned int i = 0; i < base.size(); i++){
            cout << "base " << base[i] << " ";
        }
        cout << endl;

        for(unsigned int i = 0; i < base.size(); i++){
         
            cout << "altezza1 " << lato1[i] << " ";
            
        }
        cout << endl;

        for(unsigned int i = 0; i < base.size(); i++){
            cout << "altezza2 " << lato2[i] << " ";
        }
        cout << endl;
 
        if ( (lato1[0] != base[0]) & (lato1[0] != base[base.size()-1])){
              reverse( lato1.begin(),  lato1.end());
        }

        for(unsigned int i = 0; i < base.size(); i++){
         
           cout << "altezza1 Nuova " << lato1[i] << " ";
            
        }
        cout << endl;

        if ( (lato2[0] != base[0]) & (lato2[0] != base[base.size()-1])){
              reverse( lato2.begin(),  lato2.end());
        }

        for(unsigned int i = 0; i < base.size(); i++){
            cout << "altezza2 Nuova " << lato2[i] << " ";
        }
        cout << endl;

        // aggiusto i vettori per fare in modo che altezza 1 inizi come base e altezza 2 inizi come finisce base
        vector<unsigned int> altezza1;
        vector<unsigned int> altezza2;
        if(lato1[0] == base[0]){
            altezza1 = lato1;
            altezza2 = lato2;
        }else{
            altezza1 = lato2;
            altezza2 = lato1;
        }



        cout << endl;
        for(unsigned int h = 1; h < altezza1.size()-1; h++){
            // trovo il vettore direzione del tetto
            cout << "mi muovo sulle altezze e h = " << h << endl;
            unsigned int idEstremo1 = altezza1[h];
            unsigned int idEstremo2 = altezza2[h];
            cout << "idEstremo1 " << idEstremo1 << " idEstremo2 " << idEstremo2 << endl;

            // prendiamo le coordinate degli estremi
            Vector3d Estremo1(mesh2.Cell0DsCoordinates(0,idEstremo1),mesh2.Cell0DsCoordinates(1,idEstremo1),mesh2.Cell0DsCoordinates(2,idEstremo1));
            Vector3d Estremo2(mesh2.Cell0DsCoordinates(0,idEstremo2),mesh2.Cell0DsCoordinates(1,idEstremo2),mesh2.Cell0DsCoordinates(2,idEstremo2));
            cout << "estremo 1 " <<Estremo1(0) << " " << Estremo1(1) << " " << Estremo1(2) << endl;
            cout << "estremo 2 " <<Estremo2(0) << " " << Estremo2(1) << " " << Estremo2(2) << endl;
        
            // vettore con la direzione del tetto
            Vector3d VettoreDirezione = Estremo2 - Estremo1;
            cout << "Vettore direzione " << " " << VettoreDirezione(0) << " " <<VettoreDirezione(1) << " " <<VettoreDirezione(2) << endl;


            vector<unsigned int> tetto = {altezza1[h]};
            cout << "tetto ";
            for(unsigned int indice = 0; indice < tetto.size(); indice++){
                cout << tetto[indice] << " "; 
            }
            cout << endl;
            
            int lunghezzaBase = base.size();
            for(int scorri = 0; scorri < 2*lunghezzaBase-2; scorri ++){
                 
                cout << "scorri " << scorri << endl;
                
                if(scorri % 2 == 0){
                    // MESH TEMPORANEA
                    //stiamo scorrendo sulla base 
                    unsigned int id1 = tetto[tetto.size()-1];
                    unsigned int id2 = base[scorri/2];
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    cout << "primo lato base fatto " << endl;
                    // inseriamo il secondo lato
                    id1 = base[scorri/2];
                    id2 = base[scorri/2+1];
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    cout << "secondo lato base fatto" << endl;
                    // inserisco i vertici della faccia dentro a vettore vertici
                    vector<unsigned int> vecpunti = {base[scorri/2], tetto[tetto.size()-1], base[scorri/2+1]};
                    mesh2.VettoreVertici.push_back(vecpunti);

                    // TRIANGOLAZIONE DI TIPO DUE
                    // creo il baricentro relativo alla faccia triangolare piccolina e lo salvo
                    CreaBaricentro(meshTri, mesh2 ,vecpunti, IdPuntiMeshTri, MapBaricentri);
                    // creo i lati che collegano il baricentro con i tre vertici che lo creano
                    AggiungiLati(meshTri, vecpunti, IdPuntiMeshTri, IdLatiMeshTri);
                    unsigned int IdBaricentro = IdPuntiMeshTri;
                    IdPuntiMeshTri += 1;
                    
                    // aggiungo i possibili punti sui bordi del poligono platonico
                    id1 = tetto[tetto.size()-1];
                    id2 = base[scorri/2];
                    Vector3d Medio;
                    if(ControllaBordi(latiCompleti, id1, id2, meshTri.Cell0DsCoordinates, Medio, meshTri, IdPuntiMeshTri)){
                        // aggiungo i lati che si creano con il bordo e il baricentro
                        vector<unsigned int> vecpunti1 = {id1, id2, IdBaricentro};
                        AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);
                        cout << "id del punto sul bordo " << IdPuntiMeshTri << endl;  
                        IdPuntiMeshTri += 1;
                    }

                    id1 = base[scorri/2];
                    id2 = base[scorri/2+1];
                    if(ControllaBordi(latiCompleti, id1, id2, meshTri.Cell0DsCoordinates, Medio, meshTri, IdPuntiMeshTri)){
                        // aggiungo i lati che si creano con il bordo e il baricentro
                        vector<unsigned int> vecpunti1 = {id1, id2, IdBaricentro};
                        AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri); 
                        cout << "id del punto sul bordo " << IdPuntiMeshTri << endl; 
                        IdPuntiMeshTri += 1;
                    }


                    // tratto a parte l'ultimo lato obliquo
                    if(scorri == 2*lunghezzaBase-4){
                        id1 = tetto[tetto.size()-1];
                        id2 = base[base.size()-1];
                        Vector3d Medio;
                        if(ControllaBordi(latiCompleti, id1, id2, meshTri.Cell0DsCoordinates, Medio, meshTri, IdPuntiMeshTri)){
                            // aggiungo i lati che si creano con il bordo e il baricentro
                            vector<unsigned int> vecpunti1 = {id1, id2, IdBaricentro};
                            AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri); 
                            cout << "id del punto sul bordo " << IdPuntiMeshTri << endl;
                            IdPuntiMeshTri += 1;
                        }
                    }

                    // aggiungo i lati che uniscono i due baricentri
                    CollegaBaricentri(MapBaricentri, IdBaricentro, meshTri, IdLatiMeshTri);


                }else{
                    // stiamo scorrendo sul tetto

                    // TRIANGOLAZIONE 1
                    // troviamo il nuovo punto di tetto, divido per b-h perchè il tetto è sempre diviso in meno parti, e lo inserisco 
                    if(scorri < 2*lunghezzaBase-5){
                        
                        cout << "SCORRI" <<scorri << "BASE"<< 2*lunghezzaBase-5<< endl;
                        Vector3d NuovoPuntoTetto = Estremo1 + VettoreDirezione * (scorri+1)/(2*(double)(b-h));

                        mesh2.Cell0DsCoordinates(0,ContaIdPuntiMesh2) = NuovoPuntoTetto(0);
                        mesh2.Cell0DsCoordinates(1,ContaIdPuntiMesh2) = NuovoPuntoTetto(1);
                        mesh2.Cell0DsCoordinates(2,ContaIdPuntiMesh2) = NuovoPuntoTetto(2);
                        mesh2.Cell0DsId.push_back(ContaIdPuntiMesh2);
                        
                        cout << "idNuovoPunto " << ContaIdPuntiMesh2 << endl;
                        tetto.push_back(ContaIdPuntiMesh2);
                        ContaIdPuntiMesh2 += 1;
                        // inserisco l'id di questo punto in tetto
                        
                        cout << "tetto ";
                        for(unsigned int indice = 0; indice < tetto.size(); indice++){
                            cout << tetto[indice] << " "; 
                        }
                        cout << endl;

                    }else{ 
                        tetto.push_back(altezza2[h]);
                        
                        cout << "arrivo all'ultimo punto di tetto" << endl;
                        cout << "tetto ";
                        for(unsigned int indice = 0; indice < tetto.size(); indice++){
                            cout << tetto[indice] << " "; 
                        }
                        cout << endl;
                    }
                    // inseriamo il primo lato (tetto con la base)
                    unsigned int id1 = base[scorri+2-tetto.size()];
                    unsigned int id2 = tetto[tetto.size()-2];
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    cout << "primo lato tetto fatto " << endl;
                    // inseriamo il secondo lato (tetto con il tetto) controllando che nell'ultima iterazione non metta tetto-tetto
                    id1 = tetto[tetto.size()-1];
                    id2 = tetto[tetto.size()-2];
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    cout << "secondo lato tetto fatto" << endl;
                    // inserisco i vertici della faccia dentro al vettore vertici evitando di aggiungere una faccia quando arrivo all'ultimo elemento di tetto
                    vector<unsigned int> vecpunti;
                    if(scorri < 2*lunghezzaBase-3){
                        vecpunti = {tetto[tetto.size()-2],base[scorri+2-tetto.size()],tetto[tetto.size()-1]};
                        mesh2.VettoreVertici.push_back(vecpunti);
                    
                    }


                    // TRIANGOLAZIONE  DI TIPO 2
                    // creo solo il baricentro perchè non ci saranno punti a contatto con i lati del poliedro
                    // evito l'ultimo lato (quando scorri assume l'ultimo valore del tetto mi darebbe errore)
                    if(scorri < 2*lunghezzaBase-3){
                        // trovo il baricentro della faccia e lo aggiungo 
                        CreaBaricentro(meshTri, mesh2 ,vecpunti, IdPuntiMeshTri, MapBaricentri);
                        // aggiungo i lati che dal baricentro vanno agli estremi del tiangolo
                        AggiungiLati(meshTri, vecpunti, IdPuntiMeshTri, IdLatiMeshTri);
                        unsigned int IdBaricentro = IdPuntiMeshTri;
                        IdPuntiMeshTri += 1;

                        CollegaBaricentri(MapBaricentri, IdBaricentro, meshTri, IdLatiMeshTri);
                    }
                }
            }       
            tetto.pop_back();
            base = tetto;
            for(unsigned int i = 0; i < base.size(); i++){
                cout << "base " << base[i] << " ";
            }
            cout << endl; 
        }        

    // TRIANGOLAZIONE TEMPORANEA
    // inserisco a mano gli ultimi due lati
    unsigned int id1 = altezza1[altezza1.size()-1];
    unsigned int id2 = base[0];
    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
    cout << "primo lato tetto fatto " << endl;
    id1 = altezza1[altezza1.size()-1];
    id2 = base[1];
    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
    cout << "primo lato tetto fatto " << endl;
    // inserisco l'ultima faccia
    vector<unsigned int> vecpunti = {base[0],altezza1[altezza1.size()-1],base[1]};
    mesh2.VettoreVertici.push_back(vecpunti);


    // TRIANGOLAZIONE DI TIPO DUE
    // creo il baricentro relativo alla faccia triangolare piccolina e lo salvo
    CreaBaricentro(meshTri, mesh2 ,vecpunti, IdPuntiMeshTri, MapBaricentri);
    // creo i lati che collegano il baricentro con i tre vertici che lo creano
    AggiungiLati(meshTri, vecpunti, IdPuntiMeshTri, IdLatiMeshTri);
    unsigned int IdBaricentro = IdPuntiMeshTri;
    IdPuntiMeshTri += 1;
    
    // aggiungo i possibili punti sui bordi del poligono platonico
    id1 = altezza1[altezza1.size()-1];
    id2 = base[0];
    Vector3d Medio;
    if(ControllaBordi(latiCompleti, id1, id2, meshTri.Cell0DsCoordinates, Medio, meshTri, IdPuntiMeshTri)){
        // aggiungo i lati che si creano con il bordo e il baricentro
        vector<unsigned int> vecpunti1 = {id1, id2, IdBaricentro};
        AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);  
        IdPuntiMeshTri += 1;
    }

    id1 = altezza1[altezza1.size()-1];
    id2 = base[1];
    if(ControllaBordi(latiCompleti, id1, id2, meshTri.Cell0DsCoordinates, Medio, meshTri, IdPuntiMeshTri)){
        // aggiungo i lati che si creano con il bordo e il baricentro
        vector<unsigned int> vecpunti1 = {id1, id2, IdBaricentro};
        AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);  
        IdPuntiMeshTri += 1;
    }
    
}

return true;
}

}