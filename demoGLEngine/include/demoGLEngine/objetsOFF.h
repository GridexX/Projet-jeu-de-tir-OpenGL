//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//une classe basique pour lire les fichiers OFF

#ifndef _INC_OBJETSOFF_H
#define _INC_OBJETSOFF_H

struct indexedface { unsigned int S1, S2, S3; };

struct UV { 
	double x; 
	double y;
};

struct point3D {
	double x;
	double y;
	double z;
	double u;
	double v;
};

class Objet {

public:
	char nom[40];

	unsigned int nbsommets;
	unsigned int nbfaces;

	unsigned int id;

	struct point3D* lpoints;
	struct indexedface* lfaces;
	struct point3D min;
	struct point3D max;
	struct point3D centroid;

	Objet();
	Objet(const Objet&);
	~Objet();
	void charge_OFF(const char*);
	void affiche();

};

#endif
