#pragma once
#include "VaisseauLogique.h"
#include <vector>

namespace tp3
{
	class Vaisseau : public VaisseauLogique
	{
	public:
		Vaisseau();
		virtual ~Vaisseau();
		virtual void mouvement(int thrust);
		virtual void mouvementJoueur(Vector2i& mouvement);

		void monter();
		void descendre();
		void centrer();
		void pivoter();

		virtual void initGraphiques();

	private:
		static const int OFFSET = 5;
		static const int CENTRE = 24;
		static const int LIMITE_BAS = 49;
		static const int LIMITE_HAUT = 0;
		static const int RHYTME_ANIM = 10;

		int image;
		IntRect rectangleAnimation;
	};
}
