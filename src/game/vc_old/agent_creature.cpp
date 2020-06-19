/*--------------------------------------------------------------------------------------
 * SimGame
 * Copyright (C) 2016-2020, Mark Elsworth Sowden <markelswo@gmail.com>
 *------------------------------------------------------------------------------------*/

#include "../shared.h"
#include "ConfigLoader.h"
#include "agent.h"
#include "alife/brain.h"

namespace vc {
	class CreatureAgent : public Agent {
	public:
		IMPLEMENT_SUPER( Agent )
		IMPLEMENT_FACTORY( CreatureAgent )

		CreatureAgent();
		~CreatureAgent() override;

		void Tick() override;

	protected:
	private:
		Brain *brain_{ nullptr };

		void GenerateName();

		std::string name_{ "none" };
	};

	CreatureAgent::CreatureAgent() {
		brain_ = new Brain();

		GenerateName();
	}

	CreatureAgent::~CreatureAgent() {
		delete brain_;
	}

	void CreatureAgent::GenerateName() {
		name_.clear();

		ConfigLoader idx( "./names.idx" );
		unsigned int sets = rand() % 5 + 1;
		for ( unsigned int i = 0; i < sets; ++i ) {
			std::string app = idx.GetIndex( rand() % idx.GetNumIndices() );
			if ( app.empty() ) {
				continue;
			}

			name_.append( app );
		}

		name_[ 0 ] = std::toupper( name_[ 0 ] );
	}

	void CreatureAgent::Tick() {
		Agent::Tick();

		brain_->Tick();
	}
}// namespace vc
