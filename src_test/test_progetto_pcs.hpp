#include <iostream>
#include <gtest/gtest.h>
#include <algorithm>
#include "utils.hpp"

using namespace PolygonalLibrary;

const unsigned int b = 5;

int GradoV(int GradoVAtteso, const std::vector<unsigned int>& Vertici, const std::vector<std::vector<unsigned int>>& Facce) {
	int numVGradoAtteso = 0;
	for (const auto& idV : Vertici) {
		int corrente = 0;
		for (const auto& listaFacce : Facce )
			if (std::find(listaFacce.begin(), listaFacce.end(), idV) != listaFacce.end()){
				corrente++;
			}
		if (corrente == GradoVAtteso)
			numVGradoAtteso++;
	}
	
	return numVGradoAtteso;
}		

array<unsigned int, 3> ProprietaSolido(const unsigned int& V, const unsigned int& E, const unsigned int& F, const unsigned int& b) {
	unsigned int Vatteso = V + E*(2*b-1) + F * ( (3.0*b*b)/2.0 - (3.0*b/2.0) + 1);
	unsigned int Eatteso = E * 2 * b + F * ( (9.0*b*b)/2.0 + (3.0*b/2.0) );
	unsigned int Fatteso = F * ( 3 * b * b + 3 * b);
	array<unsigned int, 3> proprieta_solido = {Vatteso, Eatteso, Fatteso};
	return proprieta_solido;
}

TEST(TestTriangolazioneUno, TestTetraedro1)
{
	unsigned int q = 3;
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Tetraedro")){
		FAIL() << "errore nella creazione della mesh del tetraedro triangolato con triangolazione uno";
	}

	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneUno(poliPlatonico, poliTRI, b, q);
	
	int T = b*b;
	int verticiAttesi = 2*T + 2;
	int latiAttesi = 6*T;
	int facceAttese = 4*T;
	
	EXPECT_EQ(poliTRI.NumCell0Ds, verticiAttesi);
	EXPECT_EQ(poliTRI.NumCell1Ds, latiAttesi);
	EXPECT_EQ(poliTRI.NumCell2Ds, facceAttese);
	
	int grado1 = 3;
	int grado2 = 6;
	int numgradoAtteso1 = 4;
	int numgradoAtteso2 = 2*(T-1);
	
	int numVGrado1 = GradoV(grado1, poliTRI.Cell0DsId, poliTRI.VettoreVertici);
	int numVGrado2 = GradoV(grado2, poliTRI.Cell0DsId, poliTRI.VettoreVertici);
	
	EXPECT_EQ(numgradoAtteso1, numVGrado1);
	EXPECT_EQ(numgradoAtteso2, numVGrado2);
}

TEST(TestTriangolazioneUno, TestOttaedro1)
{
	unsigned int q = 4;
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Ottaedro")){
		FAIL() << "errore nella creazione della mesh del ottaedro triangolato con triangolazione uno";
	}

	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneUno(poliPlatonico, poliTRI, b, q);
	
	int T = pow(b,2);
	int verticiAttesi = 4*T + 2;
	int latiAttesi = 12*T;
	int facceAttese = 8*T;
	
	EXPECT_EQ(poliTRI.NumCell0Ds, verticiAttesi);
	EXPECT_EQ(poliTRI.NumCell1Ds, latiAttesi);
	EXPECT_EQ(poliTRI.NumCell2Ds, facceAttese);
	
	int grado1 = 4;
	int grado2 = 6;
	int numgradoAtteso1 = 6;
	int numgradoAtteso2 = 4*(T-1);
	
	int numVGrado1 = GradoV(grado1, poliTRI.Cell0DsId, poliTRI.VettoreVertici);
	int numVGrado2 = GradoV(grado2, poliTRI.Cell0DsId, poliTRI.VettoreVertici);
	
	EXPECT_EQ(numgradoAtteso1, numVGrado1);
	EXPECT_EQ(numgradoAtteso2, numVGrado2);
}

TEST(TestTriangolazioneUno, TestIcosaedro1)
{
	unsigned int q = 5;
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Icosaedro")){
		FAIL() << "errore nella creazione della mesh del icosaedro triangolato con triangolazione uno";
	}

	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneUno(poliPlatonico, poliTRI, b, q);
	
	int T = pow(b,2);
	int verticiAttesi = 10*T + 2;
	int latiAttesi = 30*T;
	int facceAttese = 20*T;
	
	EXPECT_EQ(poliTRI.NumCell0Ds, verticiAttesi);
	EXPECT_EQ(poliTRI.NumCell1Ds, latiAttesi);
	EXPECT_EQ(poliTRI.NumCell2Ds, facceAttese);
	
	int grado1 = 5;
	int grado2 = 6;
	int numgradoAtteso1 = 12;
	int numgradoAtteso2 = 10*(T-1);
	
	int numVGrado1 = GradoV(grado1, poliTRI.Cell0DsId, poliTRI.VettoreVertici);
	int numVGrado2 = GradoV(grado2, poliTRI.Cell0DsId, poliTRI.VettoreVertici);
	
	EXPECT_EQ(numgradoAtteso1, numVGrado1);
	EXPECT_EQ(numgradoAtteso2, numVGrado2);
}

TEST(TestTriangolazioneDue, TestTetraedro2)
{
	unsigned int q = 3;
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Tetraedro")){
		FAIL() << "errore nella creazione della mesh del tetraedro triangolato con triangolazione due";
	}

	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneDue(poliPlatonico, poliTRI, b, q);
	
	unsigned int V = 4;
	unsigned int E = 6;
	unsigned int F = 4;
	
	array<unsigned int, 3> ProprietaSolidoAttese = ProprietaSolido(V, E, F, b);
	array<unsigned int, 3> ProprietaSolido = {poliTRI.NumCell0Ds, poliTRI.NumCell1Ds, poliTRI.NumCell2Ds};

	EXPECT_EQ(ProprietaSolido, ProprietaSolidoAttese);
}

TEST(TestTriangolazioneDue, TestOttaedro2)
{
	unsigned int q = 4;
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Ottaedro")){
		FAIL() << "errore nella creazione della mesh del ottaedro triangolato con triangolazione due";
	}

	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneDue(poliPlatonico, poliTRI, b, q);
	
	unsigned int V = 6;
	unsigned int E = 12;
	unsigned int F = 8;
	
	array<unsigned int, 3> proprietasolidoattese = ProprietaSolido(V, E, F, b);
	array<unsigned int, 3> proprietasolido = {poliTRI.NumCell0Ds, poliTRI.NumCell1Ds, poliTRI.NumCell2Ds};

	EXPECT_EQ(proprietasolido, proprietasolidoattese);
}

TEST(TestTriangolazioneDue, TestIcosaedrodro2)
{
	unsigned int q = 5;
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Icosaedro")){
		FAIL() << "errore nella creazione della mesh del icosaedro triangolato con triangolazione due";
	}

	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneDue(poliPlatonico, poliTRI, b, q);
	
	unsigned int V = 12;
	unsigned int E = 30;
	unsigned int F = 20;
	
	array<unsigned int, 3> proprietasolidoattese = ProprietaSolido(V, E, F, b);
	array<unsigned int, 3> proprietasolido = {poliTRI.NumCell0Ds, poliTRI.NumCell1Ds, poliTRI.NumCell2Ds};

	EXPECT_EQ(proprietasolido, proprietasolidoattese);
}

TEST(TestDualeTriangolazioneUno, TestTetraedroD1)
{
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Tetraedro")){
		FAIL() << "errore nella creazione della mesh del tetraedro triangolato con triangolazione uno";
	}
	
	unsigned int q = 3;
	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneUno(poliPlatonico, poliTRI, b, q);

	PolygonalMesh poliDuale;
	PolygonalLibrary::CreaDuale(poliTRI, poliDuale);
	
	unsigned int T = pow(b, 2);
	unsigned int verticiattesi = 4*T;
	unsigned int facceattese = 2*T+2;
	
	EXPECT_EQ(poliDuale.NumCell0Ds, verticiattesi);
	EXPECT_EQ(poliDuale.NumCell2Ds, facceattese);
}


TEST(TestDualeTriangolazioneDue, TestTetraedroD2)
{
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Tetraedro")){
		FAIL() << "errore nella creazione della mesh del tetraedro triangolato con triangolazione uno";
	}
	
	unsigned int q = 3;
	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneDue(poliPlatonico, poliTRI, b, q);


	PolygonalMesh poliDuale;
	PolygonalLibrary::CreaDuale(poliTRI, poliDuale);
	
	unsigned int V = 4;
	unsigned int E = 6;
	unsigned int F = 4;
	
	unsigned int verticiattesi = F * ( 3 * b * b + 3 * b);
	unsigned int facceattese = V + E*(2*b-1) + F * ( (3.0*b*b)/2.0 - (3.0*b/2.0) + 1);
	
	EXPECT_EQ(poliDuale.NumCell0Ds, verticiattesi);
	EXPECT_EQ(poliDuale.NumCell2Ds, facceattese);
}


TEST(TestDijkstra, CamminoMinimoSemplice)
{
    unsigned int n = 4;
    unsigned int Start = 0;
    unsigned int End = 3;

    vector<vector<unsigned int>> LA = {
        {1, 2},    
        {0, 3},    
        {0, 3},    
        {1, 2}     
    };

    MatrixXd MatricePesi = MatrixXd::Zero(n, n);
    MatricePesi(0, 1) = 1;
    MatricePesi(1, 0) = 1;
    MatricePesi(0, 2) = 4;
    MatricePesi(2, 0) = 4;
    MatricePesi(1, 3) = 2;
    MatricePesi(3, 1) = 2;
    MatricePesi(2, 3) = 1;
    MatricePesi(3, 2) = 1;

    vector<unsigned int> path;
    bool esito = Dijkstra(n, LA, Start, End, MatricePesi, path);

    EXPECT_TRUE(esito);
    ASSERT_EQ(path.size(), 3); 
    EXPECT_EQ(path.back(), Start);
    EXPECT_EQ(path.front(), End);

    
    reverse(path.begin(), path.end());
    vector<unsigned int> expected = {0, 1, 3};
    EXPECT_EQ(path, expected);
}


TEST(TestCamminoMinimo, ListaAdiacenzaGenerataCorrettamente)
{


    PolygonalMesh mesh;
    mesh.NumCell0Ds = 4;
    mesh.NumCell1Ds = 4;
    mesh.Cell1DsExtrema.resize(2, 4);


    mesh.Cell1DsExtrema(0, 0) = 0; mesh.Cell1DsExtrema(1, 0) = 1;
    mesh.Cell1DsExtrema(0, 1) = 1; mesh.Cell1DsExtrema(1, 1) = 2;
    mesh.Cell1DsExtrema(0, 2) = 2; mesh.Cell1DsExtrema(1, 2) = 3;
    mesh.Cell1DsExtrema(0, 3) = 3; mesh.Cell1DsExtrema(1, 3) = 0;


    vector<vector<unsigned int>> listaAdiacenza;
    listaAdiacenza.reserve(mesh.NumCell0Ds);

    for (unsigned int idVertice = 0; idVertice < mesh.NumCell0Ds; idVertice++) {
        vector<unsigned int> vettoreAdiacenza;

        for (unsigned int idAdiacente = 0; idAdiacente < mesh.Cell1DsExtrema.cols(); idAdiacente++) {
            int idVerticeIntero = idVertice;
            if (idVerticeIntero == mesh.Cell1DsExtrema(0, idAdiacente)) {
                vettoreAdiacenza.push_back(mesh.Cell1DsExtrema(1, idAdiacente));
            }
            if (idVerticeIntero == mesh.Cell1DsExtrema(1, idAdiacente)) {
                vettoreAdiacenza.push_back(mesh.Cell1DsExtrema(0, idAdiacente));
            }
        }
        listaAdiacenza.push_back(vettoreAdiacenza);
    }

    vector<vector<unsigned int>> attesa = {
        {1, 3}, 
        {0, 2}, 
        {1, 3}, 
        {2, 0}  
    };

    EXPECT_EQ(listaAdiacenza, attesa);
}

TEST(TestDuplicati, LatoPresenteNonPresente)
{

    MatrixXi MatriceLati(2, 3);
    MatriceLati << 0, 1, 2,
                   1, 2, 3;

    unsigned int index;

 
    EXPECT_TRUE(TestDuplicati(MatriceLati, 0, 1, &index));
    EXPECT_EQ(index, 0);


    EXPECT_TRUE(TestDuplicati(MatriceLati, 1, 0, &index));
    EXPECT_EQ(index, 0);


    EXPECT_TRUE(TestDuplicati(MatriceLati, 2, 1, &index));
    EXPECT_EQ(index, 1);


    EXPECT_TRUE(TestDuplicati(MatriceLati, 3, 2, &index));
    EXPECT_EQ(index, 2);


    EXPECT_FALSE(TestDuplicati(MatriceLati, 0, 3, &index));


    EXPECT_TRUE(TestDuplicati(MatriceLati, 1, 2, nullptr));
}

TEST(TestCreaBaricentro, BaricentroTriangolo)
{

    PolygonalMesh mesh;
    mesh.NumCell0Ds = 4;  
    mesh.Cell0DsCoordinates.resize(3, 4); 
    mesh.Cell0DsCoordinates << 
        0.0, 1.0, 0.0, 0.0,   
        0.0, 0.0, 1.0, 0.0,   
        0.0, 0.0, 0.0, 0.0;   


    vector<unsigned int> vertici = {0, 1, 2};
    unsigned int idBar = 3;

    map<unsigned int, vector<unsigned int>> mapBaricentri;


    bool risultato = CreaBaricentro(mesh, vertici, idBar, mapBaricentri);

    EXPECT_TRUE(risultato);


    Vector3d atteso = (Vector3d(0,0,0) + Vector3d(1,0,0) + Vector3d(0,1,0)) / 3.0;
    Vector3d ottenuto(mesh.Cell0DsCoordinates(0, idBar),
                      mesh.Cell0DsCoordinates(1, idBar),
                      mesh.Cell0DsCoordinates(2, idBar));

    EXPECT_NEAR(ottenuto(0), atteso(0), 1e-6);
    EXPECT_NEAR(ottenuto(1), atteso(1), 1e-6);
    EXPECT_NEAR(ottenuto(2), atteso(2), 1e-6);


    EXPECT_TRUE(find(mesh.Cell0DsId.begin(), mesh.Cell0DsId.end(), idBar) != mesh.Cell0DsId.end());

 
    ASSERT_TRUE(mapBaricentri.count(idBar));
    EXPECT_EQ(mapBaricentri[idBar], vertici);
}