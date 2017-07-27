#pragma once

#include "EngineClasses/TCMComponentBehaviour.h"

namespace TCM
{
	namespace Engine
	{
		class TemporaryRotatingComponent : public TCMComponentBehaviour
		{
			TCM_CLASS(TemporaryRotatingComponent, TCMComponentBehaviour)

		protected:
			virtual void Update( const float deltaTime ) override;
		};
	}
}
