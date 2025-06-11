#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <string>
#include <sstream>

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
using namespace Gedim;




int main(int argc, char* argv[]){
    
    // creo un oggetto mesh
    PolygonalMesh mesh;
    PolygonalMesh meshTriangolata1;
    PolygonalMesh meshTriangolata2;
    PolygonalMesh meshDuale1;
    PolygonalMesh meshDuale2;

    unsigned int p;
    unsigned int q; 
    unsigned int b; 
    unsigned int c;
    unsigned int id1;
    unsigned int id2;
    bool cammino = false;


    if (argc != 5 && argc != 7) {
        cerr << "Input non valido" << endl ;
        return 1;
	}
    //verifico che i valori dati in input siano tutti valori numerici e positivi
    stringstream convert;
    for (int i = 1; i < argc; i ++) {
        string arg = argv[i];
        for (char c : arg) {
            if (!isdigit(c)) {
                cerr << "Errore: '"<< arg<< "' non è un numero positivo"<<endl;
                return 1;
            }
        }
        convert<< arg << " ";
    }
    for (int i = 1; i < argc; i++){
	    convert << argv[i] << " ";
        }

    convert >> p >> q >> b >> c;
    if (argc == 7) {
      convert >> id1 >> id2;
      cammino = true;
    }

    if (p != 3){
        cerr << "la costruzione del solido geodetico richiede p = 3" << endl;
        return 1;
    }
    if (q < 3 || q > 5){
        cerr << "non esiste un poligono platonico con " << q << " come valore di q" << endl;
        return 1;
    }
    if (b != c && (c != 0 && b!= 0)){
        cerr << "valori di b e c non validi"<<endl;
        return 1;
    }
    
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

     string ParaviewPuntiTriangolati1 = "./Cell0DTriang1.inp";
     string ParaviewSegmentiTriangolati1 = "./Cell1DTriang1.inp";                         
    if(b == 0 || c == 0){
        cout << endl << "FACCIO LA TRIANGOLAZIONE UNO" << endl;
        cout << endl;

        unsigned int b_temp;
        b_temp = b;

        if(b_temp ==0){
            b_temp = c;
        }

        if(!TriangolazioneUno(mesh, meshTriangolata1, b_temp, q))
        {
            cerr << "error during triangolation" << endl;
            return 1;
        }

        StampasuFile(meshTriangolata1);
    
        utilities.ExportPoints(ParaviewPuntiTriangolati1, 
                                meshTriangolata1.Cell0DsCoordinates);
        utilities.ExportSegments(ParaviewSegmentiTriangolati1,
                                 meshTriangolata1.Cell0DsCoordinates,
                                 meshTriangolata1.Cell1DsExtrema);

        cout << "FACCIO IL DUALE UNO" << endl;
        cout << endl;

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


        if (cammino == true ) {
            cout << "FACCIO IL CAMMINO MINIMO 1" << endl;
            cout << endl;
            //controllo che gli id passati siano validi
            bool valido = true;
            if( (id1 > meshTriangolata1.NumCell0Ds) || (id2 > meshTriangolata1.NumCell0Ds) ){
                cerr << "Gli id non sono validi per il cammino minimo" << endl;
                valido = false;
            }

            if (valido == true){
               if(!CamminoMinimo(meshTriangolata1, id1, id2, ParaviewPuntiTriangolati1, ParaviewSegmentiTriangolati1)){
                                return false;
                            }
            }
            cout << endl;

            cout << "FACCIO IL CAMMINO MINIMO 1 SUL DUALE" << endl;
            cout << endl;
            //controllo che gli id passati siano validi
            bool validoD = true;
            if( (id1 > meshDuale1.NumCell0Ds) || (id2 > meshDuale1.NumCell0Ds) ){
                cerr << "Gli id non sono validi per il cammino minimo sul duale" << endl;
                validoD = false;
            }
            if(validoD == true){
                if(!CamminoMinimo(meshDuale1, id1, id2, ParaviewPuntiDuale1, ParaviewSegmentiDuale1)){
                    return false;
                }
            }
        }
    }

    string ParaviewPuntiTriangolati2 = "./Cell0DTriang2.inp";
    string ParaviewSegmentiTriangolati2 = "./Cell1DTriang2.inp";

    if(b == c){
        cout << endl << "FACCIO LA TRIANGOLAZIONE DUE" << endl;
        cout << endl;

        if(!TriangolazioneDue(mesh, meshTriangolata2, b, q))
        {
            cerr << "error during triangolation" << endl;
            return 1;
        }

        StampasuFile(meshTriangolata2);

        utilities.ExportPoints(ParaviewPuntiTriangolati2, 
                                meshTriangolata2.Cell0DsCoordinates);
        utilities.ExportSegments(ParaviewSegmentiTriangolati2,
                                meshTriangolata2.Cell0DsCoordinates,
                                meshTriangolata2.Cell1DsExtrema);

            cout << "FACCIO IL DUALE DUE" << endl;
            cout << endl;

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

        if (cammino == true ) {
            cout << "FACCIO IL CAMMINO MINIMO 2" << endl;
            cout << endl;
            //controllo che gli id passati siano validi
            bool valido = true;
            if( (id1 > meshTriangolata2.NumCell0Ds) || (id2 > meshTriangolata2.NumCell0Ds) ){
                cerr << "Gli id non sono validi per il cammino minimo" << endl;
                valido = false;
            }
            if (valido == true) {
                if(!CamminoMinimo(meshTriangolata2, id1, id2, ParaviewPuntiTriangolati2, ParaviewSegmentiTriangolati2)){
                return false;
                }
            }

            cout << endl;
            cout << "FACCIO IL CAMMINO MINIMO 2 SUL DUALE" << endl;
            cout << endl;
            bool validoD = true;
            if( (id1 > meshDuale2.NumCell0Ds) || (id2 > meshDuale2.NumCell0Ds) ){
                cerr << "Gli id non sono validi per il cammino minimo" << endl;
                validoD = false;
            }
            if (validoD == true){
                if(!CamminoMinimo(meshDuale2, id1, id2, ParaviewPuntiDuale2, ParaviewSegmentiDuale2)){
                return false;
                }
            }
        }
    }

    return 0;

    }
    