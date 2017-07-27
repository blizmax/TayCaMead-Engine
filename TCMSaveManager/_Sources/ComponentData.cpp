#include "TCMSaveManager/ComponentData.h"

#include "TCMSaveManager/SceneObjectData.h"

namespace TCM
{
	namespace Editor
	{
		namespace Data
		{
			ComponentData::ComponentData()
				: m_parentSceneObject( nullptr )
			{
			}

			ComponentData::ComponentData( Engine::TCMComponent* component, SceneObjectData* parent )
				: BaseData( component )
				, m_parentSceneObject( parent )
			{
			}

			ComponentData::~ComponentData()
			{
			}

			Engine::TCMComponent* ComponentData::Extract() const
			{
				Engine::TCMComponent* extractedClass = dynamic_cast<Engine::TCMComponent *>(ExtractSerialized());

#if _DEBUG
				if ( !extractedClass )
				TCMFAILURE( "ComponentData::Extract() - extractedClass is not a TCM::Engine::TCMComponent" );
#endif

				return extractedClass;
			}

			SceneObjectData* ComponentData::_GetParentSceneObject() const
			{
				return m_parentSceneObject;
			}

			void ComponentData::_SetParentSceneObject( SceneObjectData* sceneObject )
			{
				m_parentSceneObject = sceneObject;
			}
		}
	}
}
