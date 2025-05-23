#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>
#include <string>
#include <map>

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

    // itero su ogni riga del csv
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string field;
        unsigned int id;
        unsigned int contaVert;
        unsigned int contaLati;
        Vector3i vecVert;
        Vector3i vecLati;
        
        // separo in base al ; e salvo i valori 
        getline(converter, field, ';');
        id = stoi(field);

        // mi segno quanto vale il numero di vertici
        getline(converter, field, ';');
        contaVert = stoi(field);

        // salvo gli id dei vertici dentro un vettore
        for(unsigned int k = 0; k<contaVert;k++){
            getline(converter, field, ';');
            vecVert(k) = stoi(field);
        }

        mesh.mapVertici[id] = vecVert;

        // mi segno quanto vale il numero di lati
        getline(converter, field, ';');
        contaLati = stoi(field);

        // salvo gli id dei lati dentro un vettore
        for(unsigned int k = 0; k<contaLati;k++){
            getline(converter, field, ';');
            vecLati(k) = stoi(field);
        }
        
        mesh.mapLati[id] = vecLati;
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


bool ImportTriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b, const unsigned int& q){

    if(!Cell0DTriangolazioneUno(mesh1, mesh2, b, q))
        return false;

    if(!Cell1DTriangolazioneUno(mesh1, mesh2, b))
        return false;

    return true;
}

/**********************************/

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
         
        unsigned int idLato = mesh1.mapLati.at(idFaccia)(0);
        cout << idLato << endl;

        // prendo gli id degli estremi del primo lato della faccia idFaccia
        unsigned int idEstremo1 = mesh1.Cell1DsExtrema(0,idLato);
        unsigned int idEstremo2 = mesh1.Cell1DsExtrema(1,idLato);

        // prendo l'id dell'estremo mancante 
        unsigned int idEstremo3 = mesh1.mapVertici.at(idFaccia)(2);

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
    double d = norm(coord2 - coord1);
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
        for(unsigned int idTemp = id; idTemp < temp.cols; idTemp ++) {
            Vector3d puntotemp(mesh2.Cell0DsCoordinates(0,idTemp), mesh2.Cell0DsCoordinates(1,idTemp), mesh2.Cell0DsCoordinates(2,idTemp));
            double distanza = norm(puntocfr - puntotemp);
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

    // creo il dizionario che ha come chiave l'id della faccia del poligono iniziale e come valore i lati copleti di quella faccia


    /*iniziamo a triangolare: prendiamo ogni faccia del poligono e i suoi vettori relativi ai lati completi. prendiamo due lati 
    (uno sarà la base e l'altro sarà quello su cui iteriamo) e iniziamo ad aggiungere i punti interni relativi a questi due lati (sfruttando il terzo lato
    per trovare il vettore direzione consono). dopo aver aggiunto i punti interni e aver salvato i segmenti e le facce, saliamo su nell'altezza e usiamo come 
    base quello che era il tetto del passaggio precedente. andiamo avanti così fino a che il tetto non è lungo due a quel punto uniamo i punti con l'estremo superiore*/



}




}