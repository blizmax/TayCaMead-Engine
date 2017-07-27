#pragma once
#include "EditorDefines.h"

namespace TCM
{
	namespace Engine
	{
		class TCMSceneObject;
		class Project;
		class TCMScene;
	}

	namespace Editor
	{
		class EditorComponent abstract
		{
		public:
			EditorComponent() = default;
			virtual ~EditorComponent();

			void Register();
			virtual void Refresh( Engine::Project* project, Engine::TCMScene* scene ) = 0;

			virtual void SetSceneObject( Engine::TCMSceneObject* object )
			{
			};

			Engine::TCMScene* GetScene() const;
			Engine::TCMSceneObject* GetSceneObject() const;
			Engine::Project* GetProject() const;
		};
	}
}
