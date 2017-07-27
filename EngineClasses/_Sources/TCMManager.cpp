#include "TCMManager.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMManager )

		TCMManager::TCMManager()
		{
			SetName( "TCMManager" );
		}

		TCMManager::~TCMManager()
		{
		}

		bool TCMManager::TCMUpdate( const float deltaTime )
		{
			return TCMTicking::TCMUpdate( deltaTime );
		}

		void TCMManager::OnDestroy()
		{
			// Do nothing
		}
	}
}
