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

bool TestDuplicati(const MatrixXi& MatriceLati, const unsigned int& id1, const unsigned int& id2){
    
    int id1intero = id1;
    int id2intero = id2;
    for(unsigned int i = 0; i < MatriceLati.cols(); i++){
        if( (MatriceLati(0,i) == id1intero || MatriceLati(1,i) == id1intero) & (MatriceLati(0,i) == id2intero || MatriceLati(1,i) == id2intero) ){
            return true;
            break;
        }
    }
    return false;
    
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



    // creo il dizionario che ha come chiave l'id della faccia del poligono iniziale e come valore i lati copleti di quella faccia
    
    map<unsigned int, vector<vector<unsigned int>>> mapFacce;

    for(unsigned int i = 0; i < mesh1.NumCell2Ds; i++){
        unsigned int id1 = mesh1.mapVertici.at(i)(0);
        unsigned int id2 = mesh1.mapVertici.at(i)(1);
        unsigned int id3 = mesh1.mapVertici.at(i)(2);

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
    unsigned int contaIdFacceMesh2 = 0;

    for(unsigned int idFaccia = 0; idFaccia < mesh1.NumCell2Ds; idFaccia++) {
        cout << endl;
        cout << "FACCCIA NUMERO " << idFaccia << endl;
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
                    
                    // inseriamo il primo lato
                    if(!TestDuplicati(mesh2.Cell1DsExtrema, tetto[tetto.size()-1], base[scorri/2])){
                        
                        cout << "primo lato base " << endl;
                        mesh2.Cell1DsExtrema(0, contaIdLatiMesh2) = tetto[tetto.size()-1];
                        mesh2.Cell1DsExtrema(1, contaIdLatiMesh2) = base[scorri/2];
                        mesh2.Cell1DsId.push_back(contaIdLatiMesh2);
                        
                        cout << "lato " << contaIdLatiMesh2 << " con estremi " << tetto[tetto.size()-1] << ", " << base[scorri/2] << endl;
                        contaIdLatiMesh2 += 1;
                        cout << "primo lato base fatto " << endl;
                    }

                    // inseriamo il secondo lato
                    if(!TestDuplicati(mesh2.Cell1DsExtrema, base[scorri/2], base[scorri/2+1])){
                        cout << "secondo lato base " << endl;
                    
                        mesh2.Cell1DsExtrema(0, contaIdLatiMesh2) = base[scorri/2];
                        mesh2.Cell1DsExtrema(1, contaIdLatiMesh2) = base[scorri/2 + 1];
                        mesh2.Cell1DsId.push_back(contaIdLatiMesh2);

                        cout << "lato " << contaIdLatiMesh2 << " con estremi " << base[scorri/2] << ", " << base[scorri/2 +1] << endl;
                        contaIdLatiMesh2 += 1;

                        cout << "secondo lato base fatto" << endl;
                    }
                    

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
                    if(!TestDuplicati(mesh2.Cell1DsExtrema, base[scorri+2-tetto.size()], tetto[tetto.size()-2])){
                        cout << "primo lato tetto " << endl;    

                        mesh2.Cell1DsExtrema(0, contaIdLatiMesh2) = tetto[tetto.size()-2];
                        mesh2.Cell1DsExtrema(1, contaIdLatiMesh2) = base[scorri+2-tetto.size()];
                        mesh2.Cell1DsId.push_back(contaIdLatiMesh2);

                        cout << "lato " << contaIdLatiMesh2 << " con estremi " << tetto[tetto.size()-2] << ", " << base[scorri+2-tetto.size()] << endl;
                        contaIdLatiMesh2 += 1;

                        cout << "primo lato tetto fatto " << endl;
                    }

                    // inseriamo il secondo lato (tetto con il tetto) controllando che nell'ultima iterazione non metta tetto-tetto
                    if(!TestDuplicati(mesh2.Cell1DsExtrema, tetto[tetto.size()-1], tetto[tetto.size()-2])){
                        if(scorri < 2*lunghezzaBase-3){
                            cout << "secondo lato tetto " << endl;
                            
                            mesh2.Cell1DsExtrema(0, contaIdLatiMesh2) = tetto[tetto.size()-2];
                            mesh2.Cell1DsExtrema(1, contaIdLatiMesh2) = tetto[tetto.size()-1];
                            mesh2.Cell1DsId.push_back(contaIdLatiMesh2);

                            cout << "lato " << contaIdLatiMesh2 << " con estremi " << tetto[tetto.size()-2] << ", " << tetto[tetto.size()-1] << endl;
                            contaIdLatiMesh2 += 1;

                            cout << "secondo lato tetto fatto" << endl;
                        }
                    }

                    // inseriscoi vertici della faccia dentro al vettore vertici
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
    
    // aggiungo a mano gli ultimi due lati
    if(!TestDuplicati(mesh2.Cell1DsExtrema, altezza1[altezza1.size()-1], base[0])){
        mesh2.Cell1DsExtrema(0, contaIdLatiMesh2) = base[0];
        mesh2.Cell1DsExtrema(1, contaIdLatiMesh2) = altezza1[altezza1.size()-1];
        mesh2.Cell1DsId.push_back(contaIdLatiMesh2);

        cout << "lato " << contaIdLatiMesh2 << " con estremi " << base[0] << ", " << altezza1[altezza1.size()-1] << endl;
        contaIdLatiMesh2 += 1;
    }

    if(!TestDuplicati(mesh2.Cell1DsExtrema, altezza1[altezza1.size()-1], base[1])){
        mesh2.Cell1DsExtrema(0, contaIdLatiMesh2) = base[1];
        mesh2.Cell1DsExtrema(1, contaIdLatiMesh2) = altezza1[altezza1.size()-1];
        mesh2.Cell1DsId.push_back(contaIdLatiMesh2);

        cout << "lato " << contaIdLatiMesh2 << " con estremi " << base[1] << ", " << altezza1[altezza1.size()-1] << endl;
        contaIdLatiMesh2 += 1;
    }

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


    for(vector<unsigned int> vertici : mesh2.VettoreVertici){
        int id1 = vertici[0];
        int id2 = vertici[1];
        int id3 = vertici[2];

        vector<unsigned int> vettoreAggiuntivo;
        //vettoreAggiuntivo.reserve(vertici.size());
        vettoreAggiuntivo = {0,0,0};
        
        
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
    
      
    cout << "stampo il vettore dei vertici" << endl;
    for (const auto& riga : mesh2.VettoreLati) {
        for (const auto& elemento :riga) {
            cout << elemento << " ";
        }
        cout << endl;
    }



return true;
}



bool CreaDuale(const PolygonalMesh& mesh1, const PolygonalMesh& mesh2){

    // creo un vector di vector con dentro l'id delle facce adiacenti per ogni id di un vertice
    vector<vector<unsigned int>> facceAdiacenti;
    facceAdiacenti.reserve(mesh1.NumCell0Ds);

    // cerco, per ogni vertice, chi sono le facce andiacenti 
    for(unsigned int idV; idV < mesh1.NumCell0Ds; idV++){
        // per ogni vertice itero sulle facce per scoprire se sono adiacenti    
        vector<unsigned int> faccePerVertice;
        
        // FAI IL RESERVE FATTO BENE
        faccePerVertice.reserve()
        for(unsigned int idF; idF < mesh1.NumCell2Ds; idF++){
            if (find(mesh1.VettoreVertici[idF].begin(), mesh1.VettoreVertici[idF].end(), idV){
                // se c'è il vertice dentro la faccia inserisco la faccia nel vettore
                faccePerVertice.push_back(idF);

            }
        facceAdiacenti.push_back(faccePerVertice);

        }
        

    }

}


}