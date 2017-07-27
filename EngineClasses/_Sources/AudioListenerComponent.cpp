#include "AudioListenerComponent.h"
#include "EngineClasses/TCMSceneObject.h"
#include <TCMAudioEngine/AudioEngine/AudioListener.h>

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( AudioListenerComponent )

		void AudioListenerComponent::OnPositionChanged( const Math::Vec3& position )
		{
			SetPosition( position );
		}

		void AudioListenerComponent::OnRotationChanged( const Math::Vec3& rotation )
		{
			Math::Vec3 forward = m_sceneObject->Forward();
			Math::Vec3 trueForward;
			trueForward.x = forward.x;
			trueForward.y = -forward.z;
			trueForward.z = -forward.y;
			SetDirection( trueForward );
		}

		AudioListenerComponent::AudioListenerComponent()
		{
			SetName( "AudioListenerComponent" );
		}

		void AudioListenerComponent::SetPosition( const Math::Vec3& position ) const
		{
			Audio::AudioListener::SetPosition( position );
		}

		Math::Vec3 AudioListenerComponent::GetPosition() const
		{
			return Audio::AudioListener::GetPosition();
		}

		void AudioListenerComponent::SetDirection( const Math::Vec3& direction ) const
		{
			Audio::AudioListener::SetDirection( direction );
		}

		Math::Vec3 AudioListenerComponent::GetDirection() const
		{
			return Audio::AudioListener::GetDirection();
		}

		void AudioListenerComponent::SetGlobalVolume( const float& volume ) const
		{
			Audio::AudioListener::SetGlobalVolume( volume );
		}

		float AudioListenerComponent::GetGlobalVolume() const
		{
			return Audio::AudioListener::GetGlobalVolume();
		}
	}
}
