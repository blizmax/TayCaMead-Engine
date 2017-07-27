#include "ComponentInspector.h"
#include "Inspector.h"
#include "EngineClasses/TCMComponent.h"
#include "BaseInspector.h"

namespace TCM
{
	namespace Editor
	{
		ComponentInspector::ComponentInspector( Inspector* inspector, Engine::TCMComponent* component ): m_component( component )
		                                                                                               , m_inspector( inspector )
		{
		}

		ComponentInspector::~ComponentInspector()
		{
			/*if ( m_baseInspector )
				delete m_baseInspector;*/
		}

		Engine::TCMComponent* ComponentInspector::GetComponent() const
		{
			return m_component;
		}

		void ComponentInspector::Delete()
		{
			m_inspector->Remove( this );
		}

		void ComponentInspector::Enable( const bool value ) const
		{
			m_component->SetEnabled( value );
		}

		void ComponentInspector::UpdateFromValues() const
		{
			m_baseInspector->UpdateFromValues();
		}
	}
}
