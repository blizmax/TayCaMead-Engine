#include "Inspector.h"
#include "MainWindow.h"
#include "BaseInspector.h"
#include "EngineClasses/TCMSceneObject.h"
#include "EngineClasses/TCMComponent.h"
#include "EditorCommandManager.h"

namespace TCM
{
	namespace Editor
	{
		Inspector::Inspector() : m_object( nullptr )
		{
			MainWindow::GetPlayEvent().Push( &Inspector::DisableUI, this );
			MainWindow::GetStopEvent().Push( &Inspector::EnableUI, this );
			MainWindow::GetUpdateCallback().Push( &Inspector::OnCoreUpdate, this );

			EditorCommandManager::GetOnUndoRedoEvent().Push( &Inspector::OnCoreUpdate, this );
		}

		Inspector::~Inspector()
		{
			MainWindow::GetPlayEvent().Pop( &Inspector::DisableUI, this );
			MainWindow::GetStopEvent().Pop( &Inspector::EnableUI, this );
			MainWindow::GetUpdateCallback().Pop( &Inspector::OnCoreUpdate, this );

			EditorCommandManager::GetOnUndoRedoEvent().Pop( &Inspector::OnCoreUpdate, this );
		}

		void Inspector::SetSceneObject( Engine::TCMSceneObject* object )
		{
			m_object = object;
			Clear();

			if ( object == nullptr )
				return;

			ShowUI();

			if ( m_baseInspector )
				m_baseInspector->SetBase( object );

			for ( int i = 0; i < object->GetComponentsCount(); ++i )
				PrintComponent( object->GetComponent( i ) );
		}

		void Inspector::Refresh( Engine::Project* project, Engine::TCMScene* scene )
		{
			HideUI();
		}

		void Inspector::Remove( ComponentInspector* comp )
		{
			int i;
			for ( i = 0; i < m_componentInspectors.size(); ++i )
			{
				if ( m_componentInspectors[i] == comp )
					break;
			}
			if ( i < m_componentInspectors.size() )
				RemoveComponent( i );
		}

		void Inspector::OnCoreUpdate()
		{
			if ( m_baseInspector )
				m_baseInspector->UpdateFromValues();

			for ( int i = 0; i < m_componentInspectors.size(); ++i )
			{
				m_componentInspectors[i]->UpdateFromValues();
			}
		}

		Engine::TCMComponent* Inspector::GetComponent( int index ) const
		{
			return GetSceneObject()->GetComponent( index );
		}

		void Inspector::AddComponent( const std::string name )
		{
			if ( !m_object )
				return;
			Engine::TCMComponent* comp = dynamic_cast<Engine::TCMComponent*>(Engine::MetaFactory::Create( name ));

			AddComponent( comp );
			PrintComponent( comp );
		}

		void Inspector::AddComponent( Engine::TCMComponent* component ) const
		{
			if ( !m_object || !component )
				return;

			m_object->AddComponent( component );
		}

		void Inspector::PrintComponent( Engine::TCMComponent* component )
		{
			if ( !m_object || !component )
				return;
			m_componentInspectors.push_back( CreateComponentInspector( component ) );
		}

		void Inspector::RemoveComponent( int index )
		{
			UIRemoveComponent( index );
			m_componentInspectors.erase( m_componentInspectors.begin() + index );
			Engine::TCMComponent::Release( GetSceneObject()->GetComponent( index ) );
		}

		void Inspector::RemoveAllComponents()
		{
			UIRemoveAllComponents();
			m_componentInspectors.clear();
		}

		void Inspector::Clear()
		{
			RemoveAllComponents();
			HideUI();
		}
	}
}
