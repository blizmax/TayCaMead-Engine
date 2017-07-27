#pragma once
#include "EditorComponent.h"
#include "ComponentInspector.h"
#include <vector>

namespace TCM
{
	namespace Engine
	{
		class TCMComponent;
		class TCMSceneObject;
	}

	namespace Editor
	{
		class Inspector abstract : public EditorComponent
		{
		public:

			Inspector();
			virtual ~Inspector();

			void SetSceneObject( Engine::TCMSceneObject* object ) override;
			void Refresh( Engine::Project* project, Engine::TCMScene* scene ) override final;

			void AddComponent( const std::string name );
			void Remove( ComponentInspector* comp );

		protected:

			void OnCoreUpdate();

			void AddComponent( Engine::TCMComponent* component ) const;
			void PrintComponent( Engine::TCMComponent* component );
			Engine::TCMComponent* GetComponent( int index ) const;

			void RemoveComponent( int index );
			virtual void UIRemoveComponent( int index ) = 0;
			void RemoveAllComponents();
			virtual void UIRemoveAllComponents() = 0;

			virtual void Clear();
			virtual void ShowUI() = 0;
			virtual void HideUI() = 0;
			virtual void EnableUI() = 0;
			virtual void DisableUI() = 0;

			virtual ComponentInspector* CreateComponentInspector( Engine::TCMComponent* component ) = 0;

			std::vector<ComponentInspector *> m_componentInspectors;
			Engine::TCMSceneObject* m_object = nullptr;
			BaseInspector* m_baseInspector = nullptr;
		};
	}
}
