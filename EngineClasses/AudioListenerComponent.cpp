#include "AudioListenerComponent.h"
#include <TCMAudioEngine/AudioEngine/AudioListener.h>

void AudioListenerComponent::OnPositionChanged( const TCM::Math::Vec3& position )
{
	SetPosition(position);
}

void AudioListenerComponent::OnRotationChanged( const TCM::Math::Vec3& rotation )
{
	SetDirection(parent->forward);
}

void AudioListenerComponent::SetPosition( const TCM::Math::Vec3& position )
{
	TCM::Audio::AudioListener::SetPosition(position);
}

TCM::Math::Vec3 AudioListenerComponent::GetPosition() const
{
	return TCM::Audio::AudioListener::GetPosition();
}

void AudioListenerComponent::SetDirection( const TCM::Math::Vec3& direction )
{
	TCM::Audio::AudioListener::SetDirection(direction);
}

TCM::Math::Vec3 AudioListenerComponent::GetDirection() const
{
	return TCM::Audio::AudioListener::GetDirection();
}

void AudioListenerComponent::SetGlobalVolume( float volume )
{
	TCM::Audio::AudioListener::SetGlobalVolume(volume);
}

float AudioListenerComponent::GetGlobalVolume() const
{
	return TCM::Audio::AudioListener::GetGlobalVolume();
}
