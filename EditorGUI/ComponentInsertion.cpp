#include "ComponentInsertion.h"
#include "EngineClasses/TCMComponent.h"
#include "EngineClasses/TCMSceneObject.h"
#include "Inspector.h"

namespace TCM
{
	namespace Editor
	{
		ComponentInsertion::ComponentInsertion( Inspector* inspector ) :
		                                                               m_components( Engine::MetaFactory::GetDerivedTypes<Engine::TCMComponent>() )
		                                                               , m_inspector( inspector )
		{
		}

		void ComponentInsertion::CreateComponent( const std::string& name ) const
		{
			m_inspector->AddComponent( name );
		}
	}
}
