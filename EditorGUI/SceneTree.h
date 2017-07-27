#pragma once
#include "EditorComponent.h"
#include <vector>

namespace TCM
{
	namespace Editor
	{
		class SceneTree abstract
				: public EditorComponent
		{
		public:
			SceneTree();
			virtual ~SceneTree();

			void Refresh( Engine::Project* project, Engine::TCMScene* scene ) override final;

		protected:
			std::vector<Engine::TCMSceneObject *> FindSceneObjects( const std::string& name ) const;
			void SetSelectedSceneObject( Engine::TCMSceneObject* object );
		private:
			virtual void SetScene( Engine::TCMScene* scene ) = 0;
			virtual void RefreshUI() = 0;
			virtual void EnableUI() = 0;
			virtual void DisableUI() = 0;
		};
	}
}
