#include <iostream>
#include <gtest/gtest.h>
#include <algorithm>
#include "utils.hpp"

using namespace PolygonalLibrary;

const unsigned int b = 5;

int GradoV(int& GradoVAtteso, const std::vector<unsigned int>& Vertici, const std::vector<std::vector<unsigned int>>& Facce) {
	int numVGradoAtteso = 0;
	for (const auto& idV : Vertici) {
		int corrente = 0;
		for (const std::vector<unsigned int>& listaFacce : Facce )
			if (std::find(listaFacce.begin(), listaFacce.end(), idV) != listaFacce.end())
				corrente++;
		if (corrente == numVGradoAtteso)
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
	unsigned int q = 4;
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Tetraedro")){
		FAIL() << "errore nella creazione della mesh del tetraedro triangolato con triangolazione uno";
	}

	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneUno(poliPlatonico, poliTRI, b, q);
	
	int T = pow(b,2);
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


TEST(TestTriangolazioneUno, TestTetraedro2)
{
	unsigned int q = 4;
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Tetraedro")){
		FAIL() << "errore nella creazione della mesh del tetraedro triangolato con triangolazione uno";
	}

	PolygonalMesh poliTRI;
	PolygonalLibrary::TriangolazioneDue(poliPlatonico, poliTRI, b, q);
	
	unsigned int V = 4;
	unsigned int E = 6;
	unsigned int F = 4;
	
	array<unsigned int, 3> proprietasolidoattese = ProprietaSolido(V, E, F, b);
	array<unsigned int, 3> proprietasolido = {poliTRI.NumCell0Ds, poliTRI.NumCell1Ds, poliTRI.NumCell2Ds};

	EXPECT_EQ(proprietasolido, proprietasolidoattese);
}

TEST(TestDuale, Test2)
{
	PolygonalMesh poliPlatonico;
	if (!PolygonalLibrary::ImportMesh(poliPlatonico, "Tetraedro")){
		FAIL() << "errore nella creazione della mesh del tetraedro triangolato con triangolazione uno";
	}
	
	PolygonalMesh poliDuale;
	PolygonalLibrary::CreaDuale(poliPlatonico, poliDuale);
	
	unsigned int V = 4;
	unsigned int E = 6;
	unsigned int F = 4;
	
	unsigned int verticiattesi = F * ( 3 * b * b + 3 * b);
	unsigned int facceattese = V + E*(2*b-1) + F * ( (3.0*b*b)/2.0 - (3.0*b/2.0) + 1);
	
	EXPECT_EQ(poliDuale.NumCell0Ds, verticiattesi);
	EXPECT_EQ(poliDuale.NumCell2Ds, facceattese);
}