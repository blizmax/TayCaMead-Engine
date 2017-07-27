#include "TemporaryRotatingComponent.h"
#include "EngineClasses/TCMSceneObject.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TemporaryRotatingComponent );

		void TemporaryRotatingComponent::Update( const float deltaTime )
		{
			Math::Vec3 rotation = m_sceneObject->GetLocalRotation();
			rotation.y += deltaTime * 45.0f;
			rotation.y += deltaTime * 20.0f;
			rotation.z += deltaTime * 10.0f;
			m_sceneObject->SetRotation( rotation );
		}
	}
}
