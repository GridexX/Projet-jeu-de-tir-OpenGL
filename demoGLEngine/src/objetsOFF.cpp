//R. RAFFIN, IUT Aix-Marseille, d?partement Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//une classe pour lire les fichiers OFF
#include <errno.h>
#include <cstring>

#include <cstdlib>
#include <cstdio>
//#include <unistd.h>

#include "demoGLEngine/objetsOFF.h"

Objet::Objet() {
	strcpy(nom, "nil");
	nbsommets = 0;
	nbfaces = 0;
	id = 0;
}

Objet::~Objet() {
	delete[] lpoints;
	delete[] lfaces;
}

void Objet::charge_OFF(const char nom_fichier[40])
{
	unsigned int i;
	FILE* fichier;
	unsigned int n;

	char magic[10];

	if ((fichier = fopen(nom_fichier, "r")) == NULL)
	{
		perror("Erreur a l'ouverture du fichier OFF");
		exit(1);
	}

	fscanf(fichier, "%s\n", magic);

	if (strcmp(magic, "OFF") != 0) {
		perror("Erreur dans l'en t?te du fichier OFF");
		exit(0);
	}

	fscanf(fichier, "%u %u %u\n", &nbsommets, &nbfaces, &i);

	//Allocation des listes de donn?es
	lfaces = new struct indexedface[nbfaces];
	lpoints = new struct point3D[nbsommets];

	//Remplissage de la liste de points
	for (i = 0; i < nbsommets; i++)
	{
		//Adaptation pour lire les coordonn?es UV pour chaque point
		fscanf(fichier, "%lf %lf %lf %lf %lf\n", &lpoints[i].x, &lpoints[i].y, &lpoints[i].z, &lpoints[i].u, &lpoints[i].v);
		if (min.x > lpoints[i].x)
			min.x = lpoints[i].x;
		if (min.y > lpoints[i].y)
			min.y = lpoints[i].y;
		if (min.z > lpoints[i].z)
			min.z = lpoints[i].z;

		if (max.x < lpoints[i].x)
			max.x = lpoints[i].x;
		if (max.y < lpoints[i].y)
			max.y = lpoints[i].y;
		if (max.z < lpoints[i].z)
			max.z = lpoints[i].z;
	}

	//D?finition du centroid de l'objet
	centroid.x = (max.x + min.x) / 2.0f;
	centroid.y = (max.y + min.y) / 2.0f;
	centroid.z = (max.z + min.z) / 2.0f;


	//Remplissage de la liste de faces
	for (i = 0; i < nbfaces; i++)
	{
		fscanf(fichier, "%u %u %u %u\n", &n, &lfaces[i].S1, &lfaces[i].S2, &lfaces[i].S3);
		if (n != 3) {
			perror("Erreur au chargement des faces, non triangulaires");
			exit(1);
		}
	}

	fclose(fichier);
}

void Objet::affiche()
{
	unsigned int i;
	fprintf(stderr, "Objet %s nbsommets %u nbfaces %u\n", nom, nbsommets, nbfaces);

	fprintf(stderr, "\n SOMMETS\n");
	for (i = 0; i < nbsommets; i++)
		fprintf(stderr, "Sommets %u [%.2f %.2f %.2f %.2f %.2f]\n", i, lpoints[i].x, lpoints[i].y, lpoints[i].z, lpoints[i].u, lpoints [i].v);

	fprintf(stderr, "\n FACES\n");
	for (i = 0; i < nbfaces; i++)
		fprintf(stderr, "Face %u [%u %u %u]\n", i, lfaces[i].S1, lfaces[i].S2, lfaces[i].S3);
}


