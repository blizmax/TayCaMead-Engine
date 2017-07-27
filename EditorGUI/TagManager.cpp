#include "TagManager.h"

namespace TCM
{
	namespace Editor
	{
		// TODO: fix errors
		/*void TagManager::AddTag( const std::string& tag )
		{
			GetProject()->AddTag( tag );
		}

		void TagManager::RemoveTag( const std::string& tag )
		{
			GetProject()->DeleteTag( tag );
		}*/

		void TagManager::Refresh( Engine::Project* project, Engine::TCMScene* )
		{
			ClearUI();
			//PrintTags( *project->GetTags() );
		}
	}
}
