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
  bool ImportMesh(PolygonalMesh& mesh, const string& Poliedro){

    if(!ImportCell0Ds(mesh, Poliedro))
        return false;

    if(!ImportCell1Ds(mesh, Poliedro))
        return false;

    if(!ImportCell2Ds(mesh, Poliedro))
        return false;
        
    return true;
}

/**********************************/
bool StampasuFile(const PolygonalMesh& mesh){
    ofstream outFile0("Cell0Ds.txt");

    if (!outFile0) {
        cerr << "Errore nell'apertura del file Cell0Ds.txt." << endl;
        return false;
    }
    outFile0 <<"id;x;y;z"<<endl;
    for(unsigned int idPunto = 0; idPunto < mesh.Cell0DsCoordinates.cols(); idPunto ++) {
        double x = mesh.Cell0DsCoordinates(0, idPunto);
        double y = mesh.Cell0DsCoordinates(1, idPunto);
        double z = mesh.Cell0DsCoordinates(2, idPunto);
       
        outFile0 << idPunto << ";" << x << ";" << y << ";" << z << endl;

    }
    outFile0.close(); 

    ofstream outFile1("Cell1Ds.txt");
    if (!outFile1) {
        cerr << "Errore nell'apertura del file Cell1Ds.txt." << endl;
        return false;
    }
    outFile1<<"id;origin;end"<<endl;
    for(unsigned int idLato = 0; idLato < mesh.Cell1DsExtrema.cols(); idLato ++) {
        double id1 = mesh.Cell1DsExtrema(0, idLato);
        double id2 = mesh.Cell1DsExtrema(1, idLato);

        outFile1 << idLato <<";"<<id1 << ";" << id2 << endl;
    }
    outFile1.close(); 
    
    ofstream outFile2("Cell2Ds.txt");
    if (!outFile2) {
        cerr << "Errore nell'apertura del file Cell2Ds.txt." << endl;
        return false;
    }
    outFile2<<"id;NumVertices;Vertices;NumEdges;Edges"<<endl;
    for(unsigned idFaccia = 0; idFaccia < mesh.VettoreVertici.size(); idFaccia++){
        outFile2<<idFaccia<<";"<<mesh.VettoreVertici[idFaccia].size()<<";";
        for(unsigned int v = 0; v < mesh.VettoreVertici[idFaccia].size(); v++ ){
            outFile2<<mesh.VettoreVertici[idFaccia][v]<<";";
        }
        outFile2<<mesh.VettoreLati[idFaccia].size()<<";";
        for(unsigned int y = 0; y < mesh.VettoreLati[idFaccia].size(); y++ ){
            if (y<mesh.VettoreLati[idFaccia].size()-1){
                outFile2<<mesh.VettoreLati[idFaccia][y]<<";";
            }else{
                outFile2<<mesh.VettoreLati[idFaccia][y];
            }
        }
        outFile2<<endl;
    }
    outFile2.close(); 

    ofstream outFile3("Cell3Ds.txt");
    if (!outFile3) {
        cerr << "Errore nell'apertura del file Cell3Ds.txt." << endl;
        return false;
    }
    outFile3<<"id;NumVertices;Vertices;NumEdges;Edges;NumFaces;Faces"<<endl;
    outFile3<<0<<";"<<mesh.NumCell0Ds<<";"<<mesh.NumCell1Ds<<";"<<mesh.NumCell2Ds;

    outFile3.close(); 
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
        MatriceLati(0, contaIdLati) = id1;
        MatriceLati(1, contaIdLati) = id2;
        VettoreIdLati.push_back(contaIdLati);
    
        contaIdLati += 1;
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

bool TriangolazioneUno(const PolygonalMesh& mesh1, PolygonalMesh& mesh2, const unsigned int& b, const unsigned int& q){
    
    // tratto subito a parte il caso b = 1
    if(b == 1){
        mesh2 = mesh1;
        return true;
    }
    
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
    // sono al massimo 12 operazioni (icosaedro) --> O(1)
    for(unsigned int id : mesh1.Cell0DsId){
        mesh2.Cell0DsId.push_back(id);
        mesh2.Cell0DsCoordinates(0, id) = mesh1.Cell0DsCoordinates(0, id);
        mesh2.Cell0DsCoordinates(1, id) = mesh1.Cell0DsCoordinates(1, id);
        mesh2.Cell0DsCoordinates(2, id) = mesh1.Cell0DsCoordinates(2, id);
    }

    // contiene dei vettori fatti così [idEstremoTetraedro, ...idPuntiInterni..., idEstremoTetraedro2]
    vector<vector<unsigned int>> latiCompleti; 

    // aggiungo le coordinate nuove (triangolazione) dei punti che stanno sui lati
    // il costo totale di questo for è O(b)
    for(unsigned int id : mesh1.Cell1DsId){
        // vettore così [idEstremoTetraedro, ...idPuntiInterni..., idEstremoTetraedro2]
        vector<unsigned int> latoCompleto;
        
        // memorizzo gli estremi del lato e li metto dentro dei vettori
        unsigned int idEstremo1 = mesh1.Cell1DsExtrema(0,id);
        unsigned int idEstremo2 = mesh1.Cell1DsExtrema(1,id);

        latoCompleto.push_back(idEstremo1);

        Vector3d Estremo1(mesh1.Cell0DsCoordinates(0,idEstremo1),mesh1.Cell0DsCoordinates(1,idEstremo1),mesh1.Cell0DsCoordinates(2,idEstremo1));
        Vector3d Estremo2(mesh1.Cell0DsCoordinates(0,idEstremo2),mesh1.Cell0DsCoordinates(1,idEstremo2),mesh1.Cell0DsCoordinates(2,idEstremo2));
        
        // vettore con la direzione del lato
        Vector3d VettoreDirezione = Estremo2 - Estremo1;
        
        // trovo i punti in mezzo ai lati e li memorizzo
        // costo di questo for O(b)
        for(unsigned int i = 0; i < b-1; i++){
            Vector3d punto = Estremo1 + VettoreDirezione * (i+1)/(double)(b); 
            contIdPunti = contIdPunti + 1;

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

    // creo il "dizionario" che ha nella posizione i l'id della faccia del poligono iniziale e come valore i lati copleti di quella faccia
    vector<vector<vector<unsigned int>>> vecFacce;

    // scorro sui lati del poligono platonico
    // costo di questo ciclo for O(b^2)
    for(unsigned int i = 0; i < mesh1.NumCell2Ds; i++){
        unsigned int id1 = mesh1.VettoreVertici[i][0];
        unsigned int id2 = mesh1.VettoreVertici[i][1];
        unsigned int id3 = mesh1.VettoreVertici[i][2];

        vector<vector<unsigned int>> lati;
        
        // scorro dentro lati completi O(b+1)
        for(unsigned int k = 0; k<latiCompleti.size(); k++){
            // cerco se il lato contiene gli id di inizio e di fine O(2b) per tre volte
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

    // scorro su ogni faccia
    // il costo di tutto questo ciclo for è O(Cb^5) con C > 50 almeno
    for(unsigned int idFaccia = 0; idFaccia < mesh1.NumCell2Ds; idFaccia++) {
        
        // trovo la base e le altezze su cui salire
        vector<unsigned int> base = vecFacce[idFaccia][0];
        vector<unsigned int> lato1 = vecFacce[idFaccia][1];  
        vector<unsigned int> lato2 = vecFacce[idFaccia][2];

        // inverto i vettori per avere compatibilità
        // costo O(b+1)
        if ( (lato1[0] != base[0]) & (lato1[0] != base[base.size()-1])){
              reverse( lato1.begin(),  lato1.end());
        }

        if ( (lato2[0] != base[0]) & (lato2[0] != base[base.size()-1])){
              reverse( lato2.begin(),  lato2.end());
        }

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

        // scorro sulle altezze 
        // costo O(b)
        for(unsigned int h = 1; h < altezza1.size()-1; h++){
            // trovo il vettore direzione del tetto
            unsigned int idEstremo1 = altezza1[h];
            unsigned int idEstremo2 = altezza2[h];

            // prendiamo le coordinate degli estremi
            Vector3d Estremo1(mesh2.Cell0DsCoordinates(0,idEstremo1),mesh2.Cell0DsCoordinates(1,idEstremo1),mesh2.Cell0DsCoordinates(2,idEstremo1));
            Vector3d Estremo2(mesh2.Cell0DsCoordinates(0,idEstremo2),mesh2.Cell0DsCoordinates(1,idEstremo2),mesh2.Cell0DsCoordinates(2,idEstremo2));
        
            // vettore con la direzione del tetto
            Vector3d VettoreDirezione = Estremo2 - Estremo1;

            vector<unsigned int> tetto = {altezza1[h]};
            int lunghezzaBase = base.size();
            // costo di questo for O(b^2) (la prima base costa 2*b-2 e da li in poi costa di meno)
            for(int scorri = 0; scorri < 2*lunghezzaBase-2; scorri ++){
                if(scorri % 2 == 0){
                    //stiamo scorrendo sulla base 
                    // inseriamo il primo lato
                    unsigned int id1 = tetto[tetto.size()-1];
                    unsigned int id2 = base[scorri/2];
                    // costo di inserisci lati O(30b^2) nel caso in cui scorre tutta la matrice + icosaedro
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);

                    // inseriamo il secondo lato
                    id1 = base[scorri/2];
                    id2 = base[scorri/2+1];
                    // costo di inserisci lati O(30b^2) nel caso in cui scorre tutta la matrice + icosaedro
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);

                    // inserisco i vertici della faccia dentro a vettore vertici
                    vector<unsigned int> vecpunti = {base[scorri/2], tetto[tetto.size()-1],base[scorri/2+1]};
                    mesh2.VettoreVertici.push_back(vecpunti);

                }else{
                    // stiamo scorrendo sul tetto
                    // troviamo il nuovo punto di tetto, divido per b-h perchè il tetto è sempre diviso in meno parti, e lo inserisco 
                    if(scorri < 2*lunghezzaBase-5){
                        Vector3d NuovoPuntoTetto = Estremo1 + VettoreDirezione * (scorri+1)/(2*(double)(b-h));

                        mesh2.Cell0DsCoordinates(0,ContaIdPuntiMesh2) = NuovoPuntoTetto(0);
                        mesh2.Cell0DsCoordinates(1,ContaIdPuntiMesh2) = NuovoPuntoTetto(1);
                        mesh2.Cell0DsCoordinates(2,ContaIdPuntiMesh2) = NuovoPuntoTetto(2);
                        mesh2.Cell0DsId.push_back(ContaIdPuntiMesh2);
                        
                        // inserisco l'id di questo punto in tetto
                        tetto.push_back(ContaIdPuntiMesh2);
                        ContaIdPuntiMesh2 += 1;

                    }else if(scorri == 2*lunghezzaBase-5){ 
                        // non devo aggiungere punti in più ma solo quello che sta sul lato opposto
                        tetto.push_back(altezza2[h]);
                    }

                    // inseriscoi vertici della faccia dentro al vettore vertici evitando di aggiungere una faccia quando arrivo all'ultimo elemento di tetto
                    if(scorri < 2*lunghezzaBase-3){
                        // inseriamo il primo lato (tetto con la base)
                        unsigned int id1 = base[scorri+2-tetto.size()];
                        unsigned int id2 = tetto[tetto.size()-2];
                        // costo di inserisci lati O(30b^2) nel caso in cui scorre tutta la matrice + icosaedro
                        inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);

                        // inseriamo il secondo lato (tetto con il tetto) controllando che nell'ultima iterazione non metta tetto-tetto
                        id1 = tetto[tetto.size()-1];
                        id2 = tetto[tetto.size()-2];
                        // costo di inserisci lati O(30b^2) nel caso in cui scorre tutta la matrice + icosaedro
                        inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                        
                        vector<unsigned int> vecpunti = {tetto[tetto.size()-2],base[scorri+2-tetto.size()],tetto[tetto.size()-1]};
                        mesh2.VettoreVertici.push_back(vecpunti);
                    
                    }else if(scorri == 2*lunghezzaBase-3){ 
                        // inseriamo il primo lato (tetto con la base)
                        unsigned int id1 = base[scorri+1-tetto.size()];
                        unsigned int id2 = tetto[tetto.size()-1];
                        // costo di inserisci lati O(30b^2) nel caso in cui scorre tutta la matrice + icosaedro
                        inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    }

                }
            }       
        
            // costo di questa copia O(b) nel caso peggiore
            base = tetto;
        }        

    // inserisco a mano gli ultimi due lati
    unsigned int id1 = altezza1[altezza1.size()-1];
    unsigned int id2 = base[0];
    // costo di inserisci lati O(30b^2) nel caso in cui scorre tutta la matrice + icosaedro
    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
    

    id1 = altezza1[altezza1.size()-1];
    id2 = base[1];
    // costo di inserisci lati O(30b^2) nel caso in cui scorre tutta la matrice + icosaedro
    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
    
    // inserisco l'ultima faccia
    vector<unsigned int> vecpunti = {base[0],altezza1[altezza1.size()-1],base[1]};
    mesh2.VettoreVertici.push_back(vecpunti);

    }

    // inserisco nel vettore lati i lati relativi a ogni faccia in ordine
    // costo computazionale O(b^4)
    for(vector<unsigned int> vertici : mesh2.VettoreVertici){
        int id1 = vertici[0];
        int id2 = vertici[1];
        int id3 = vertici[2];

        vector<unsigned int> vettoreAggiuntivo;
        vettoreAggiuntivo = {0,0,0};
        
        // con questi if faccio in modo da mettere i lati in posizione corretta
        // questo for costa O(b^2) nel caso peggiore
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
    
    // costo computazionale O(b^2)
    ProiettaPunti(mesh2.Cell0DsCoordinates);

return true;
}

/**********************************/

bool CreaDuale(const PolygonalMesh& mesh1, PolygonalMesh& mesh2){

    // creo un vector di vector con dentro l'id delle facce adiacenti per ogni id di un vertice
    vector<vector<unsigned int>> facceAdiacenti;
    facceAdiacenti.reserve(mesh1.NumCell0Ds);

    // cerco, per ogni vertice, chi sono le facce adiacenti 
    // costo di questo for O(b^4)
    for(unsigned int idV = 0; idV < mesh1.NumCell0Ds; idV++){
        // per ogni vertice itero sulle facce per scoprire se sono adiacenti    
        vector<unsigned int> faccePerVertice;
        
        // il massimo è sempre sei così siamo sicuri
        faccePerVertice.reserve(6);

        // questo for è O(b^2)
        for(unsigned int idF = 0; idF < mesh1.NumCell2Ds; idF++){
            // questo find è O(1) (cerca su tre elementi)
            if ( find(mesh1.VettoreVertici[idF].begin(), mesh1.VettoreVertici[idF].end(), idV) != mesh1.VettoreVertici[idF].end()){
                // se c'è il vertice dentro la faccia inserisco la faccia nel vettore
                faccePerVertice.push_back(idF);
            } 
        }

        // metto in ordine le facce per vertice affinchè siano tutte confinanti
        vector<unsigned int> faccePerVerticeOrdinate;
        unsigned int NumeroFacceXVertice = faccePerVertice.size();
        faccePerVerticeOrdinate.reserve(NumeroFacceXVertice);
        faccePerVerticeOrdinate.push_back(faccePerVertice[0]);

        // queste iterazioni non dipendono da b quindi sono O(1)
        // sono al massimo 6
        for(unsigned int i = 0; i < NumeroFacceXVertice-1; i++){
            // i è l'elemento 1 da confrontare
            unsigned int id1 = faccePerVerticeOrdinate[i];

            // rimuovo l'id appena preso così faccio un confronto solo sugli altri
            // è sempre il primo quindi O(1)
            faccePerVertice.erase(remove(faccePerVertice.begin(), faccePerVertice.end(), id1), faccePerVertice.end());
            // fa al massimo 6 iterate
            for(unsigned int j = 0; j<faccePerVertice.size(); j++){
                // j è l'elemento 2 da confrontare
                unsigned int id2 = faccePerVertice[j];
                // trovo i lati delle due facce id1 e id2
                vector<unsigned int> latiId1 = mesh1.VettoreLati[id1];
                vector<unsigned int> latiId2 = mesh1.VettoreLati[id2];

                // scorro sui lati della prima faccia e vedo se trovo elementi in comune
                bool Trovato = false;
                // fa tre iterazioni
                for(unsigned int Lato : latiId1){
                    // questi find costano 3 al massimo
                    if( find(latiId2.begin(), latiId2.end(), Lato) != latiId2.end()){
                        Trovato = true;
                    }
                }

                if(Trovato){
                    faccePerVerticeOrdinate.push_back(id2);
                    break;
                } 
            }
            
        }
        
        
        facceAdiacenti.push_back(faccePerVerticeOrdinate);
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
    // questo for ha costo O(C*b^4) C > 50
    for(unsigned int idV = 0; idV < facceAdiacenti.size(); idV++){
        // inizializzo un vettore dove inserire, per ogni faccia del duale, i suoi vertici
        vector<unsigned int> vecVert;
        // tale vettore deve essere lungo quanto il numero di facce andiacenti a ogni vertice
        vecVert.reserve(facceAdiacenti[idV].size());

        // ora itero sulle facce dentro i vettori
        // per un vertice ci sono al massimo 6 facce O(1)
        for(unsigned int idF : facceAdiacenti[idV]){
            // vettore contenente i vertici della faccia
            vector<unsigned int> verticiPerFaccia = mesh1.VettoreVertici[idF];
            unsigned int idPunto1 = verticiPerFaccia[0];
            unsigned int idPunto2 = verticiPerFaccia[1];
            unsigned int idPunto3 = verticiPerFaccia[2];

            Vector3d punto1(mesh1.Cell0DsCoordinates(0,idPunto1),mesh1.Cell0DsCoordinates(1,idPunto1),mesh1.Cell0DsCoordinates(2,idPunto1));
            Vector3d punto2(mesh1.Cell0DsCoordinates(0,idPunto2),mesh1.Cell0DsCoordinates(1,idPunto2),mesh1.Cell0DsCoordinates(2,idPunto2));
            Vector3d punto3(mesh1.Cell0DsCoordinates(0,idPunto3),mesh1.Cell0DsCoordinates(1,idPunto3),mesh1.Cell0DsCoordinates(2,idPunto3));

            // calcolo il punto della figura duale
            Vector3d Baricentro = (punto1 + punto2 + punto3)/3;
            // se il punto non c'è già lo aggiungo
            unsigned int idPuntoDuale;
            // test duplicati ha costo O(b^2) nel caso peggiore
            if(!TestDuplicatiPunti(mesh2.Cell0DsCoordinates, Baricentro, idPuntoDuale)){    
                // inserisco il nuovo punto nella matrice con le coordinate
                mesh2.Cell0DsCoordinates(0, contaIdPunti) = Baricentro(0);
                mesh2.Cell0DsCoordinates(1, contaIdPunti) = Baricentro(1);
                mesh2.Cell0DsCoordinates(2, contaIdPunti) = Baricentro(2);
                mesh2.Cell0DsId.push_back(contaIdPunti);
                // inserisco nel VettoreVertici l'id del nuovo punto
                vecVert.push_back(contaIdPunti);

                contaIdPunti += 1;
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
    // il costo di questo for è O(b^4)
    for(vector<unsigned int> vertici : mesh2.VettoreVertici){
        // prendo la lista dei vertici di ogni faccia
        // scorro sui vertici di ogni faccia
        // i vertici di ogni faccia sono ordinati in modo che il successivo sia il vicino di quello prima
        vector<unsigned int> vecLati;
        vecLati.reserve(vertici.size());

        // ci sono al massimo 6 vertici per ogni faccia
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
            // test duplicati costa O(b^2) nel caso peggiore
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
    // costo O(b^2)
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
   
    // O(b^2)
    for(unsigned int i = 0; i < n; i++){
        pred.push_back(-1);
        dist.push_back(inf);
    }

    pred[start] = start;
    dist[start] = 0.0;
    
    // creo la coda con priorità, sarà ordinata in ordine crescente
    priority_queue<pair<int, double>, vector<pair<int, double>>, greater<pair<int, double>>> PQ;
    // O(b^2)
    for(unsigned int i = 0; i < n; i++){
		PQ.push({i, dist[i]});
    }

    // O(b^2)
    while(!PQ.empty()){
        int u = PQ.top().first;
		//int p = PQ.top().second;
		PQ.pop();
        for(unsigned int w : LA[u]){
            if(dist[w] > dist[u] + matrice(u,w)){
                dist[w] = dist[u] + matrice(u,w);
                pred[w] = u;
                PQ.push({w,dist[w]});
            }
        }
    }

    // in totale O(b^2*log(b))
    unsigned int v = end;
	while(v != start){
		path.push_back(v);
		v = pred[v];
	} 
    path.push_back(start);

    return true;

}

/**********************************/

bool CamminoMinimo(const PolygonalMesh& mesh, const unsigned int& id1, const unsigned int& id2, const string& nomefilepunti, const string& nomefilelati){

    // creo la lista di adiacenza
    vector<vector<unsigned int>> listaAdiacenza;
    listaAdiacenza.reserve(mesh.NumCell0Ds);

    // ora la riempio
    // costa O(b^4)
    for(unsigned int idVertice = 0; idVertice < mesh.NumCell0Ds; idVertice++){
        vector<unsigned int> vettoreAdiacenza;

        for(unsigned int idAdiacente = 0; idAdiacente < mesh.Cell1DsExtrema.cols(); idAdiacente++){
            int idVerticeIntero = idVertice;
            if( idVerticeIntero == mesh.Cell1DsExtrema(0, idAdiacente)){
                vettoreAdiacenza.push_back(mesh.Cell1DsExtrema(1, idAdiacente));
            } 
            
            if(idVerticeIntero == mesh.Cell1DsExtrema(1, idAdiacente)){
                vettoreAdiacenza.push_back(mesh.Cell1DsExtrema(0, idAdiacente));
            }
        }
        listaAdiacenza.push_back(vettoreAdiacenza);
    }
   
    //calcolo la matrice di adiacenza
    MatrixXd matricePesi = MatrixXd::Zero(mesh.NumCell0Ds, mesh.NumCell0Ds);
    
    // costa O(b^2)*costante
    for(unsigned int v = 0; v < listaAdiacenza.size(); v++) {
        for(unsigned int v1 :listaAdiacenza[v]) {
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

    //stampa del percorso minimo
    cout << "Vengono toccati " << path.size() << " punti nel percorso" << endl;
    cout << "ID dei punti toccati dal percorso minimo:" << endl;
    for (unsigned int i = 0;i < path.size(); i++) {
        cout << path[path.size() - 1 - i] << " ";
    }
    cout<<endl;
    

    // coloriamo i punti relativi al percorso
    vector<double> ProprietaPuntiPercorso(mesh.NumCell0Ds, 0.0);
	for(unsigned int punto : path){
		ProprietaPuntiPercorso[punto] = 1.0;
    }

    
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


    //cout << path.size() << endl;
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
    cout << "il costo di tale percorso è " << lunghezzaPercorso << endl;

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

bool CreaBaricentro(PolygonalMesh& meshTri ,const vector<unsigned int>& vecpunti, const unsigned int& IdBar, map<unsigned int, vector<unsigned int>>& map){
    unsigned int id0 = vecpunti[0];
    unsigned int id1 = vecpunti[1];
    unsigned int id2 = vecpunti[2];

    Vector3d punto0(meshTri.Cell0DsCoordinates(0,id0),meshTri.Cell0DsCoordinates(1,id0),meshTri.Cell0DsCoordinates(2,id0));
    Vector3d punto1(meshTri.Cell0DsCoordinates(0,id1),meshTri.Cell0DsCoordinates(1,id1),meshTri.Cell0DsCoordinates(2,id1));
    Vector3d punto2(meshTri.Cell0DsCoordinates(0,id2),meshTri.Cell0DsCoordinates(1,id2),meshTri.Cell0DsCoordinates(2,id2));
    
    Vector3d baricentro;
    baricentro = (punto0 + punto1 + punto2)/3;

    meshTri.Cell0DsCoordinates(0, IdBar) = baricentro(0);
    meshTri.Cell0DsCoordinates(1, IdBar) = baricentro(1);
    meshTri.Cell0DsCoordinates(2, IdBar) = baricentro(2);
    meshTri.Cell0DsId.push_back(IdBar);

    // costo di questa operazione O(log(b^2)), il dizionario è ordinato come un heap tree
    map[IdBar] = vecpunti;
    return true;
}

/**********************************/

bool ControllaBordi(const vector<vector<unsigned int>>& latiCompleti, const unsigned int& id1, const unsigned int& id2, Vector3d& Medio, PolygonalMesh& meshTri, unsigned int& contapunti, bool& Duplicato, unsigned int& IdTrovato){
    // costo di questo for O(n) con n numero di lati, al massimo è uguale a 20
    for(vector<unsigned int> lato : latiCompleti){
        // questo if costa O(2m) con m numero di id dentro al lato 
        if( (find(lato.begin(), lato.end(), id1) != lato.end()) & (find(lato.begin(), lato.end(), id2) != lato.end()) ){
            Vector3d punto1(meshTri.Cell0DsCoordinates(0,id1),meshTri.Cell0DsCoordinates(1,id1),meshTri.Cell0DsCoordinates(2,id1));
            Vector3d punto2(meshTri.Cell0DsCoordinates(0,id2),meshTri.Cell0DsCoordinates(1,id2),meshTri.Cell0DsCoordinates(2,id2));
            Medio = (punto1 + punto2)/2;
            // questo if costa O(s) con s numero di punti totali
            if(TestDuplicatiPunti(meshTri.Cell0DsCoordinates, Medio, IdTrovato)){
                Duplicato = true;
                return true;
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
    // dentro a vec punti ci sono tre punti
    for(unsigned int idPunto : vecpunti){
        // costo O(n) con n numero di punti del poligono triangolato
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

bool CollegaBaricentri(const map<unsigned int, vector<unsigned int>>& MapBaricentri,const unsigned int& IdBaricentro, PolygonalMesh& meshTri, unsigned int& contaLati, const vector<unsigned int>& baricentri, unsigned int& contaIdFacce){
    // il costo di questa operazione è O(log(n)+3) con n numero di baricentri
    vector<unsigned int> vettore1 = MapBaricentri.at(IdBaricentro);
    unsigned int quanti = 0;
    vector<unsigned int> VerticiAdiacenti;
    VerticiAdiacenti.reserve(3);
    
    // per ogni baricentro dentro al dizionario
    // costo totale del for O(n*m)
    for(const auto& chiave : MapBaricentri){
        // costo del for O(n) n numero di baricentri
        // se trovo la chiave dentro al vettore dei baricentri per quella faccia
        if(find(baricentri.begin(), baricentri.end(), chiave.first) != baricentri.end() ){
            // costo dell'if O(m) m numero di baricentri per faccia
            vector<unsigned int> vettore2 = chiave.second;
            // questi due for sono 9 iterazioni sempre
            for(unsigned int elemento1 : vettore1){
                for(unsigned int elemento2 : vettore2){
                    if(elemento1 == elemento2){
                        quanti += 1;
                        VerticiAdiacenti.push_back(elemento1);
                    }
                }
            }

            if(quanti == 2){
                // aggiungiamo il lato che collega i baricenti
                meshTri.Cell1DsExtrema(0, contaLati) = IdBaricentro;
                meshTri.Cell1DsExtrema(1, contaLati) = chiave.first;
                meshTri.Cell1DsId.push_back(contaLati);
                contaLati += 1;

                // aggiungiamo le due facce
                vector<unsigned int> vecVertici = {IdBaricentro, chiave.first, VerticiAdiacenti[0]};
                meshTri.VettoreVertici.push_back(vecVertici);
                meshTri.Cell2DsId.push_back(contaIdFacce);
                contaIdFacce += 1;

                vecVertici = {IdBaricentro, chiave.first, VerticiAdiacenti[1]};
                meshTri.VettoreVertici.push_back(vecVertici);
                meshTri.Cell2DsId.push_back(contaIdFacce);
                contaIdFacce += 1;
            }

        quanti = 0;
        VerticiAdiacenti = {};
        }
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

    // salvo il numero di vertici, lati e facce usando le formule per la triangolazione di tipo due
    // queste quantità sono stimabili con b^2 * opportuna costante
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
    // sono un numero piccolo di operazioni
    for(unsigned int id : mesh1.Cell0DsId){
        mesh2.Cell0DsId.push_back(id);
        mesh2.Cell0DsCoordinates(0, id) = mesh1.Cell0DsCoordinates(0, id);
        mesh2.Cell0DsCoordinates(1, id) = mesh1.Cell0DsCoordinates(1, id);
        mesh2.Cell0DsCoordinates(2, id) = mesh1.Cell0DsCoordinates(2, id);
        // faccio lo stesso per la mesh tri
        meshTri.Cell0DsId.push_back(id);
        meshTri.Cell0DsCoordinates(0, id) = mesh1.Cell0DsCoordinates(0, id);
        meshTri.Cell0DsCoordinates(1, id) = mesh1.Cell0DsCoordinates(1, id);
        meshTri.Cell0DsCoordinates(2, id) = mesh1.Cell0DsCoordinates(2, id);
    }


    // contiene dei vettori fatti così [idEstremoTetraedro, ...idPuntiInterni..., idEstremoTetraedro2]
    vector<vector<unsigned int>> latiCompleti; 
    // aggiungo le coordinate nuove (triangolazione) dei punti che stanno sui lati
    // costo totale di questo for O(b)
    for(unsigned int id : mesh1.Cell1DsId){
        // questo for costa al massimo 20

        // vettore così [idEstremoTetraedro, ...idPuntiInterni..., idEstremoTetraedro2]
        vector<unsigned int> latoCompleto;
        
        // memorizzo gli estremi del lato e li metto dentro dei vettori
        unsigned int idEstremo1 = mesh1.Cell1DsExtrema(0,id);
        unsigned int idEstremo2 = mesh1.Cell1DsExtrema(1,id);

        latoCompleto.push_back(idEstremo1);

        Vector3d Estremo1(mesh1.Cell0DsCoordinates(0,idEstremo1),mesh1.Cell0DsCoordinates(1,idEstremo1),mesh1.Cell0DsCoordinates(2,idEstremo1));
        Vector3d Estremo2(mesh1.Cell0DsCoordinates(0,idEstremo2),mesh1.Cell0DsCoordinates(1,idEstremo2),mesh1.Cell0DsCoordinates(2,idEstremo2));
        
        // vettore con la direzione del lato
        Vector3d VettoreDirezione = Estremo2 - Estremo1;
        
        // trovo i punti in mezzo ai lati e li memorizzo
        for(unsigned int i = 0; i < b-1; i++){
            // questo for è un O(b)
            Vector3d punto = Estremo1 + VettoreDirezione * (i+1)/(double)(b); 

            contIdPunti = contIdPunti + 1;

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


    // creo il vettore che ha come chiave l'id della faccia del poligono iniziale e come valore i lati copleti di quella faccia (vettori di vettori)
    vector<vector<vector<unsigned int>>> vecFacce;
    // costo del for O(b^2)
    for(unsigned int i = 0; i < mesh1.NumCell2Ds; i++){
        // massimo 20 operazioni
        unsigned int id1 = mesh1.VettoreVertici[i][0];
        unsigned int id2 = mesh1.VettoreVertici[i][1];
        unsigned int id3 = mesh1.VettoreVertici[i][2];

        vector<vector<unsigned int>> lati;
        for(unsigned int k = 0; k<latiCompleti.size(); k++){
            // costo O(b+1)
            if( (find(latiCompleti[k].begin(), latiCompleti[k].end(), id1) != latiCompleti[k].end()) & (find(latiCompleti[k].begin(), latiCompleti[k].end(), id2) != latiCompleti[k].end()) ){
                // costo dell'if O(b)
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
    meshTri.Cell2DsId.push_back(meshTri.NumCell2Ds);

    // creo il contatore per contare l'id dei lati di mesh2 temporanea
    unsigned int ContaIdPuntiMesh2 = contIdPunti + 1;
    unsigned int contaIdLatiMesh2 = 0;

    // creo il contatore per contare l'id dei lati di meshTri
    unsigned int IdPuntiMeshTri = contIdPunti + 1;
    unsigned int IdLatiMeshTri = 0;
    unsigned int IdFacceMeshtri = 0;
    // creo una map che contiene come chiavi gli id dei baricentri e come valore un vettore che contiene gli id dei punti che l'hanno generato
    map<unsigned int, vector<unsigned int>> MapBaricentri;

    // creo un vettore che per ogni faccia mi ricorda chi sono i baricentri
    vector<unsigned int> BaricentriXFaccia;
    // per ogni faccia ci sono b^2 baricentri
    BaricentriXFaccia.reserve(T);
    
    // itero su ogni faccia del poligono platonico
    // costa in totale O(b^6)
    for(unsigned int idFaccia = 0; idFaccia < mesh1.NumCell2Ds; idFaccia++) {
        // costo massimo 20 iterazioni

        // trovo la base e le altezze su cui salire
        // costa O(b+1)
        vector<unsigned int> base = vecFacce[idFaccia][0];
        vector<unsigned int> lato1 = vecFacce[idFaccia][1];  
        vector<unsigned int> lato2 = vecFacce[idFaccia][2];

        // all'inizio la base della triangolazione due sarà uguale
        vector<unsigned int> base2 = vecFacce[idFaccia][0];

        // inverto i vettori per avere compatibilità
        // costa O(b+1)
        if ( (lato1[0] != base[0]) & (lato1[0] != base[base.size()-1])){
              reverse( lato1.begin(),  lato1.end());
        }

        // costa O(b+1)
        if ( (lato2[0] != base[0]) & (lato2[0] != base[base.size()-1])){
              reverse( lato2.begin(),  lato2.end());
        }

        // aggiusto i vettori per fare in modo che altezza 1 inizi come base e altezza 2 inizi come finisce base
        vector<unsigned int> altezza1;
        vector<unsigned int> altezza2;
        // costa O(b)
        if(lato1[0] == base[0]){
            altezza1 = lato1;
            altezza2 = lato2;
        }else{
            altezza1 = lato2;
            altezza2 = lato1;
        }


        for(unsigned int h = 1; h < altezza1.size()-1; h++){
            // costo del for O(b)
            // trovo il vettore direzione del tetto
            
            unsigned int idEstremo1 = altezza1[h];
            unsigned int idEstremo2 = altezza2[h];

            // prendiamo le coordinate degli estremi
            Vector3d Estremo1(mesh2.Cell0DsCoordinates(0,idEstremo1),mesh2.Cell0DsCoordinates(1,idEstremo1),mesh2.Cell0DsCoordinates(2,idEstremo1));
            Vector3d Estremo2(mesh2.Cell0DsCoordinates(0,idEstremo2),mesh2.Cell0DsCoordinates(1,idEstremo2),mesh2.Cell0DsCoordinates(2,idEstremo2));
        
            // vettore con la direzione del tetto
            Vector3d VettoreDirezione = Estremo2 - Estremo1;

            vector<unsigned int> tetto = {altezza1[h]};
            vector<unsigned int> tetto2 = {altezza1[h]};
            
            int lunghezzaBase = base.size();
            for(int scorri = 0; scorri < 2*lunghezzaBase-2; scorri ++){
                // costo O(C*b^2) 
                if(scorri % 2 == 0){
                    // MESH TEMPORANEA
                    //stiamo scorrendo sulla base 
                    unsigned int id1 = tetto[tetto.size()-1];
                    unsigned int id2 = base[scorri/2];
                    // costa O(b^2)
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);

                    // inseriamo il secondo lato
                    id1 = base[scorri/2];
                    id2 = base[scorri/2+1];
                    // costa O(b^2)
                    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);

                    // inserisco i vertici della faccia dentro a vettore vertici
                    vector<unsigned int> vecpunti = {base[scorri/2], tetto[tetto.size()-1], base[scorri/2+1]};
                    mesh2.VettoreVertici.push_back(vecpunti);

                    // TRIANGOLAZIONE DI TIPO DUE
                    // creo il baricentro relativo alla faccia triangolare piccolina e lo salvo
                    vector<unsigned int> vecpunti1 = {base2[scorri/2], tetto2[tetto2.size()-1], base2[scorri/2+1]};
                    
                    BaricentriXFaccia.push_back(IdPuntiMeshTri);
                    // costa O(2*log(b))
                    CreaBaricentro(meshTri,vecpunti1, IdPuntiMeshTri, MapBaricentri);
                    // creo i lati che collegano il baricentro con i tre vertici che lo creano
                    // costa O(b^2)
                    AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);
                    unsigned int IdBaricentro = IdPuntiMeshTri;
                    IdPuntiMeshTri += 1;
                    
                    // aggiungo i possibili punti sui bordi del poligono platonico
                    id1 = tetto2[tetto2.size()-1];
                    id2 = base2[scorri/2];
                    Vector3d Medio;
                    bool Duplicato = false;
                    unsigned int IdTrovato = 0;
                    
                    if(ControllaBordi(latiCompleti, id1, id2, Medio, meshTri, IdPuntiMeshTri, Duplicato, IdTrovato)){
                        // costa O(b^3)
                        // aggiungo i lati che si creano con il bordo e il baricentro
                        if(!Duplicato){
                            vecpunti1 = {id1, id2, IdBaricentro};
                            // costa O(b^2)
                            AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);
                            
                            // aggiungo una faccia
                            vector<unsigned int> vecVertici = {IdPuntiMeshTri, IdBaricentro, id1};
                            meshTri.VettoreVertici.push_back(vecVertici);
                            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                            IdFacceMeshtri += 1;
                            // aggiungo l'altra
                            vecVertici = {IdPuntiMeshTri, IdBaricentro, id2};
                            meshTri.VettoreVertici.push_back(vecVertici);
                            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                            IdFacceMeshtri += 1;
                            IdPuntiMeshTri += 1;
                        }else{
                            int idBarIntero = IdBaricentro;
                            meshTri.Cell1DsExtrema(0, IdLatiMeshTri) = idBarIntero;
                            meshTri.Cell1DsExtrema(1, IdLatiMeshTri) = IdTrovato;
                            meshTri.Cell1DsId.push_back(IdLatiMeshTri);
                            // aggiungo una faccia
                            vector<unsigned int> vecVertici = {IdTrovato, IdBaricentro, id1};
                            meshTri.VettoreVertici.push_back(vecVertici);
                            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                            IdFacceMeshtri += 1;
                            // aggiungo l'altra
                            vecVertici = {IdTrovato, IdBaricentro, id2};
                            meshTri.VettoreVertici.push_back(vecVertici);
                            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                            IdFacceMeshtri += 1;
                            
                            IdLatiMeshTri += 1;
                        }
                    }
                        
                    

                    id1 = base2[scorri/2];
                    id2 = base2[scorri/2+1];
                    Duplicato = false;
                    IdTrovato = 0;
                    if(ControllaBordi(latiCompleti, id1, id2, Medio, meshTri, IdPuntiMeshTri, Duplicato, IdTrovato)){
                        // costa O(b^3)
                        if(!Duplicato){
                            vecpunti1 = {id1, id2, IdBaricentro};
                            // costa O(b^2)
                            AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);
                            
                            // aggiungo una faccia
                            vector<unsigned int> vecVertici = {IdPuntiMeshTri, IdBaricentro, id1};
                            meshTri.VettoreVertici.push_back(vecVertici);
                            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                            IdFacceMeshtri += 1;
                            // aggiungo l'altra
                            vecVertici = {IdPuntiMeshTri, IdBaricentro, id2};
                            meshTri.VettoreVertici.push_back(vecVertici);
                            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                            IdFacceMeshtri += 1;
                            IdPuntiMeshTri += 1;
                        }else{
                            int idBarIntero = IdBaricentro;
                            meshTri.Cell1DsExtrema(0, IdLatiMeshTri) = idBarIntero;
                            meshTri.Cell1DsExtrema(1, IdLatiMeshTri) = IdTrovato;
                            meshTri.Cell1DsId.push_back(IdLatiMeshTri);
                            // aggiungo una faccia
                            vector<unsigned int> vecVertici = {IdTrovato, IdBaricentro, id1};
                            meshTri.VettoreVertici.push_back(vecVertici);
                            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                            IdFacceMeshtri += 1;
                            // aggiungo l'altra
                            vecVertici = {IdTrovato, IdBaricentro, id2};
                            meshTri.VettoreVertici.push_back(vecVertici);
                            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                            IdFacceMeshtri += 1;
                            
                            IdLatiMeshTri += 1;
                        }
                    }
            
                    


                    // tratto a parte l'ultimo lato obliquo
                    if(scorri == 2*lunghezzaBase-4){
                        id1 = tetto2[tetto2.size()-1];
                        id2 = base2[base2.size()-1];
                        Vector3d Medio;
                        Duplicato = false;
                        IdTrovato = 0;
                        if(ControllaBordi(latiCompleti, id1, id2, Medio, meshTri, IdPuntiMeshTri, Duplicato, IdTrovato)){
                            // costa O(b^3)
                            if(!Duplicato){
                                vecpunti1 = {id1, id2, IdBaricentro};
                                AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);
                    
                                // aggiungo una faccia
                                vector<unsigned int> vecVertici = {IdPuntiMeshTri, IdBaricentro, id1};
                                meshTri.VettoreVertici.push_back(vecVertici);
                                meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                                IdFacceMeshtri += 1;
                                // aggiungo l'altra
                                vecVertici = {IdPuntiMeshTri, IdBaricentro, id2};
                                meshTri.VettoreVertici.push_back(vecVertici);
                                meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                                IdFacceMeshtri += 1;
                                
                                IdPuntiMeshTri += 1;
                            }else{
                                int idBarIntero = IdBaricentro;
                                meshTri.Cell1DsExtrema(0, IdLatiMeshTri) = idBarIntero;
                                meshTri.Cell1DsExtrema(1, IdLatiMeshTri) = IdTrovato;
                                meshTri.Cell1DsId.push_back(IdLatiMeshTri);
                                // aggiungo una faccia
                                vector<unsigned int> vecVertici = {IdTrovato, IdBaricentro, id1};
                                meshTri.VettoreVertici.push_back(vecVertici);
                                meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                                IdFacceMeshtri += 1;
                                // aggiungo l'altra
                                vecVertici = {IdTrovato, IdBaricentro, id2};
                                meshTri.VettoreVertici.push_back(vecVertici);
                                meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                                IdFacceMeshtri += 1;
                                
                                IdLatiMeshTri += 1;
                            }
                        }
                        
                    }

                    // aggiungo i lati che uniscono i due baricentri
                    // costa O(b^4)
                    CollegaBaricentri(MapBaricentri, IdBaricentro, meshTri, IdLatiMeshTri, BaricentriXFaccia, IdFacceMeshtri);


                }else{
                    // stiamo scorrendo sul tetto

                    // TRIANGOLAZIONE 1
                    // troviamo il nuovo punto di tetto, divido per b-h perchè il tetto è sempre diviso in meno parti, e lo inserisco 
                    if(scorri < 2*lunghezzaBase-5){
                        
                        Vector3d NuovoPuntoTetto = Estremo1 + VettoreDirezione * (scorri+1)/(2*(double)(b-h));

                        // inserisco il nuovo punto nelle coordinate della mesh temporanea
                        mesh2.Cell0DsCoordinates(0,ContaIdPuntiMesh2) = NuovoPuntoTetto(0);
                        mesh2.Cell0DsCoordinates(1,ContaIdPuntiMesh2) = NuovoPuntoTetto(1);
                        mesh2.Cell0DsCoordinates(2,ContaIdPuntiMesh2) = NuovoPuntoTetto(2);
                        mesh2.Cell0DsId.push_back(ContaIdPuntiMesh2);

                        // inserisco il nuovo punto nelle coordinate della mesh di secondo tipo
                        meshTri.Cell0DsCoordinates(0,IdPuntiMeshTri) = NuovoPuntoTetto(0);
                        meshTri.Cell0DsCoordinates(1,IdPuntiMeshTri) = NuovoPuntoTetto(1);
                        meshTri.Cell0DsCoordinates(2,IdPuntiMeshTri) = NuovoPuntoTetto(2);
                        meshTri.Cell0DsId.push_back(IdPuntiMeshTri);
                        //cout << "idNuovoPunto " << IdPuntiMeshTri << endl;
                        tetto2.push_back(IdPuntiMeshTri);
                        IdPuntiMeshTri += 1;

                        
                        tetto.push_back(ContaIdPuntiMesh2);
                        ContaIdPuntiMesh2 += 1;
                        // inserisco l'id di questo punto in tetto
                        

                    }else if(scorri == 2*lunghezzaBase-5){ 
                        tetto.push_back(altezza2[h]);
                        tetto2.push_back(altezza2[h]);
                        
                    }

                    if(scorri < 2*lunghezzaBase-3){
                        // inseriamo il primo lato (tetto con la base)
                        unsigned int id1 = base[scorri+2-tetto.size()];
                        unsigned int id2 = tetto[tetto.size()-2];
                        // costa O(b^2)
                        inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);

                        // inseriamo il secondo lato (tetto con il tetto) controllando che nell'ultima iterazione non metta tetto-tetto
                        id1 = tetto[tetto.size()-1];
                        id2 = tetto[tetto.size()-2];
                        // costa O(b^2)
                        inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);

                        vector<unsigned int> vecpunti = {tetto[tetto.size()-2],base[scorri+2-tetto.size()],tetto[tetto.size()-1]};
                        mesh2.VettoreVertici.push_back(vecpunti);
                    
                    }else if(scorri == 2*lunghezzaBase-3){ // = 5
                        // inseriamo il primo lato (tetto con la base)
                        unsigned int id1 = base[scorri+1-tetto.size()];
                        unsigned int id2 = tetto[tetto.size()-1];
                        // costa O(b^2)
                        inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
                    }


                    // TRIANGOLAZIONE  DI TIPO 2
                    // creo solo il baricentro perchè non ci saranno punti a contatto con i lati del poliedro
                    // evito l'ultimo lato (quando scorri assume l'ultimo valore del tetto mi darebbe errore)
                    if(scorri < 2*lunghezzaBase-3){
                        // trovo il baricentro della faccia e lo aggiungo 
                        vector<unsigned int> vecpunti2 = {tetto2[tetto2.size()-2],base2[scorri+2-tetto2.size()],tetto2[tetto2.size()-1]};
                        
                        BaricentriXFaccia.push_back(IdPuntiMeshTri);
                        // costa O(log(b^2))
                        CreaBaricentro(meshTri ,vecpunti2, IdPuntiMeshTri, MapBaricentri);
                        
                        // aggiungo i lati che dal baricentro vanno agli estremi del tiangolo
                        // costa O(b^2)
                        AggiungiLati(meshTri, vecpunti2, IdPuntiMeshTri, IdLatiMeshTri);
                        
                        unsigned int IdBaricentro = IdPuntiMeshTri;
                        IdPuntiMeshTri += 1;

                        // costa O(b^4)
                        CollegaBaricentri(MapBaricentri, IdBaricentro, meshTri, IdLatiMeshTri, BaricentriXFaccia, IdFacceMeshtri);
                    }

                }
            }       
            // costa O(b)
            base = tetto;
            base2 = tetto2;
            
        }        

    // TRIANGOLAZIONE TEMPORANEA
    // inserisco a mano gli ultimi due lati
    unsigned int id1 = altezza1[altezza1.size()-1];
    unsigned int id2 = base[0];
    // O(b^2)
    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
    
    id1 = altezza1[altezza1.size()-1];
    id2 = base[1];
    // O(b^2)
    inserisciLati(mesh2.Cell1DsExtrema, mesh2.Cell1DsId , contaIdLatiMesh2, id1, id2);
    
    // inserisco l'ultima faccia
    vector<unsigned int> vecpunti = {base[0],altezza1[altezza1.size()-1],base[1]};
    mesh2.VettoreVertici.push_back(vecpunti);


    // TRIANGOLAZIONE DI TIPO DUE
    // creo il baricentro relativo alla faccia triangolare piccolina e lo salvo
    // O(2*log(b))
    CreaBaricentro(meshTri ,vecpunti, IdPuntiMeshTri, MapBaricentri);
    // creo i lati che collegano il baricentro con i tre vertici che lo creano
    // O(b^2)
    AggiungiLati(meshTri, vecpunti, IdPuntiMeshTri, IdLatiMeshTri);
    unsigned int IdBaricentro = IdPuntiMeshTri;
    IdPuntiMeshTri += 1;
    
    // aggiungo i possibili punti sui bordi del poligono platonico
    id1 = altezza1[altezza1.size()-1];
    id2 = base[0];
    Vector3d Medio;
    bool Duplicato = false;
    unsigned int IdTrovato = 0;
    if(ControllaBordi(latiCompleti, id1, id2, Medio, meshTri, IdPuntiMeshTri, Duplicato, IdTrovato)){
        // O(b^3)
        if(!Duplicato){
            vector<unsigned int> vecpunti1 = {id1, id2, IdBaricentro};
            AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);
            
            // aggiungo una faccia
            vector<unsigned int> vecVertici = {IdPuntiMeshTri, IdBaricentro, id1};
            meshTri.VettoreVertici.push_back(vecVertici);
            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
            IdFacceMeshtri += 1;
            // aggiungo l'altra
            vecVertici = {IdPuntiMeshTri, IdBaricentro, id2};
            meshTri.VettoreVertici.push_back(vecVertici);
            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
            IdFacceMeshtri += 1;
            
            IdPuntiMeshTri += 1;
        }else{
            int idBarIntero = IdBaricentro;
            meshTri.Cell1DsExtrema(0, IdLatiMeshTri) = idBarIntero;
            meshTri.Cell1DsExtrema(1, IdLatiMeshTri) = IdTrovato;
            meshTri.Cell1DsId.push_back(IdLatiMeshTri);
            // aggiungo una faccia
            vector<unsigned int> vecVertici = {IdTrovato, IdBaricentro, id1};
            meshTri.VettoreVertici.push_back(vecVertici);
            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
            IdFacceMeshtri += 1;
            // aggiungo l'altra
            vecVertici = {IdTrovato, IdBaricentro, id2};
            meshTri.VettoreVertici.push_back(vecVertici);
            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
            IdFacceMeshtri += 1;
            
            IdLatiMeshTri += 1;
        } 
    }

    id1 = altezza1[altezza1.size()-1];
    id2 = base[1];
    
    Duplicato = false;
    IdTrovato = 0;
    if(ControllaBordi(latiCompleti, id1, id2, Medio, meshTri, IdPuntiMeshTri, Duplicato, IdTrovato)){
        // O(b^3)
        if(!Duplicato){
            vector<unsigned int> vecpunti1 = {id1, id2, IdBaricentro};
            AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);
            
            // aggiungo una faccia
            vector<unsigned int> vecVertici = {IdPuntiMeshTri, IdBaricentro, id1};
            meshTri.VettoreVertici.push_back(vecVertici);
            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
            IdFacceMeshtri += 1;
            // aggiungo l'altra
            vecVertici = {IdPuntiMeshTri, IdBaricentro, id2};
            meshTri.VettoreVertici.push_back(vecVertici);
            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
            IdFacceMeshtri += 1;
             
            IdPuntiMeshTri += 1;
        }else{
            int idBarIntero = IdBaricentro;
            meshTri.Cell1DsExtrema(0, IdLatiMeshTri) = idBarIntero;
            meshTri.Cell1DsExtrema(1, IdLatiMeshTri) = IdTrovato;
            meshTri.Cell1DsId.push_back(IdLatiMeshTri);
            // aggiungo una faccia
            vector<unsigned int> vecVertici = {IdTrovato, IdBaricentro, id1};
            meshTri.VettoreVertici.push_back(vecVertici);
            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
            IdFacceMeshtri += 1;
            // aggiungo l'altra
            vecVertici = {IdTrovato, IdBaricentro, id2};
            meshTri.VettoreVertici.push_back(vecVertici);
            meshTri.Cell2DsId.push_back(IdFacceMeshtri);
            IdFacceMeshtri += 1;
            
            IdLatiMeshTri += 1;
        } 
    }
    // O(b^4)
    CollegaBaricentri(MapBaricentri, IdBaricentro, meshTri, IdLatiMeshTri, BaricentriXFaccia, IdFacceMeshtri);

    BaricentriXFaccia = {};
    
    // tratto a parte il caso b = 1
    // qua b = 1 quindi i costi sono tutti O(1)
    if(b == 1){
        id1 = base[0];
        id2 = base[1];
        
        Duplicato = false;
        IdTrovato = 0;
        if(ControllaBordi(latiCompleti, id1, id2, Medio, meshTri, IdPuntiMeshTri, Duplicato, IdTrovato)){
            if(!Duplicato){
                vector<unsigned int> vecpunti1 = {id1, id2, IdBaricentro};
                AggiungiLati(meshTri, vecpunti1, IdPuntiMeshTri, IdLatiMeshTri);
                //cout << "passo come baricentro " << IdBaricentro;
                //cout << "id del punto sul bordo " << IdPuntiMeshTri << endl;
                // aggiungo una faccia
                vector<unsigned int> vecVertici = {IdPuntiMeshTri, IdBaricentro, id1};
                meshTri.VettoreVertici.push_back(vecVertici);
                meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                IdFacceMeshtri += 1;
                // aggiungo l'altra
                vecVertici = {IdPuntiMeshTri, IdBaricentro, id2};
                meshTri.VettoreVertici.push_back(vecVertici);
                meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                IdFacceMeshtri += 1;
                
                IdPuntiMeshTri += 1;
            }else{
                int idBarIntero = IdBaricentro;
                meshTri.Cell1DsExtrema(0, IdLatiMeshTri) = idBarIntero;
                meshTri.Cell1DsExtrema(1, IdLatiMeshTri) = IdTrovato;
                meshTri.Cell1DsId.push_back(IdLatiMeshTri);
                // aggiungo una faccia
                vector<unsigned int> vecVertici = {IdTrovato, IdBaricentro, id1};
                meshTri.VettoreVertici.push_back(vecVertici);
                meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                IdFacceMeshtri += 1;
                // aggiungo l'altra
                vecVertici = {IdTrovato, IdBaricentro, id2};
                meshTri.VettoreVertici.push_back(vecVertici);
                meshTri.Cell2DsId.push_back(IdFacceMeshtri);
                IdFacceMeshtri += 1;
                
                IdLatiMeshTri += 1;
        } 
        }
    }
    
    }


    // riempio il VettoreLati per ogni faccia in ordine
    // O(b^4)
    for(vector<unsigned int> vertici : meshTri.VettoreVertici){
        // O(b^2)
        int id1 = vertici[0];
        int id2 = vertici[1];
        int id3 = vertici[2];

        vector<unsigned int> vettoreAggiuntivo;
        vettoreAggiuntivo = {0,0,0};
        
        // con questi if faccio in modo da mettere i lati in posizione corretta
        for(unsigned int idl = 0; idl < meshTri.Cell1DsExtrema.cols(); idl++){
            // O(b^2)
            if( ( meshTri.Cell1DsExtrema(0,idl) == id1 ||  meshTri.Cell1DsExtrema(1,idl) == id1) & ( meshTri.Cell1DsExtrema(0,idl) == id2 ||  meshTri.Cell1DsExtrema(1,idl) == id2) ){
                vettoreAggiuntivo[0] = idl;
            }else if( ( meshTri.Cell1DsExtrema(0,idl) == id2 ||  meshTri.Cell1DsExtrema(1,idl) == id2) & ( meshTri.Cell1DsExtrema(0,idl) == id3 ||  meshTri.Cell1DsExtrema(1,idl) == id3) ){
                vettoreAggiuntivo[1] = idl;
            }else if(( meshTri.Cell1DsExtrema(0,idl) == id1 ||  meshTri.Cell1DsExtrema(1,idl) == id1) & ( meshTri.Cell1DsExtrema(0,idl) == id3 ||  meshTri.Cell1DsExtrema(1,idl) == id3)){
                vettoreAggiuntivo[2] = idl; 
            }
                
                
        }
        meshTri.VettoreLati.push_back(vettoreAggiuntivo);
    }

    // O(b^2)
    ProiettaPunti(meshTri.Cell0DsCoordinates);

    return true;
}

}