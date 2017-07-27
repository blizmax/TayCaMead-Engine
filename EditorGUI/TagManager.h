#pragma once
#include "EditorComponent.h"
#include "EngineClasses/Project.h"

namespace TCM
{
	namespace Editor
	{
		class TagManager abstract : public EditorComponent
		{
		public:
			TagManager() = default;
			virtual ~TagManager() = default;

			/*void AddTag( const std::string& tag );
			void RemoveTag( const std::string& tag );*/

			virtual void PrintTags( std::vector<std::string> list ) = 0;
			virtual void ClearUI() = 0;

		protected:
			void Refresh( Engine::Project* project, Engine::TCMScene* scene ) override;
		};
	}
}
