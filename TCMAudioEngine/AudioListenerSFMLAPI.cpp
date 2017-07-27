#include "../Common/TCMAudioEngine/SFMLAudio/AudioListenerSFMLAPI.h"
#include <SFML/Audio/Listener.hpp>

#define ToSFMLVector3(vector) sf::Vector3f(vector.x, -vector.z, -vector.y)
#define ToEngineVector3(vector) TCM::Engine::Math::Vec3(vector.x, -vector.z, -vector.y)

namespace TCM
{
	namespace Engine
	{
		namespace Audio
		{
			namespace SFML
			{
				void AudioListenerSFMLAPI::CoreSetPosition( const Math::Vec3& position )
				{
					sf::Listener::setPosition( ToSFMLVector3(position) );
				}

				Math::Vec3 AudioListenerSFMLAPI::CoreGetPosition()
				{
					return ToEngineVector3(sf::Listener::getPosition());
				}

				void AudioListenerSFMLAPI::CoreSetDirection( const Math::Vec3& direction )
				{
					sf::Listener::setDirection( ToSFMLVector3(direction) );
				}

				Math::Vec3 AudioListenerSFMLAPI::CoreGetDirection()
				{
					return ToEngineVector3(sf::Listener::getDirection());
				}

				void AudioListenerSFMLAPI::CoreSetGlobalVolume( const float& volume )
				{
					sf::Listener::setGlobalVolume( volume );
				}

				float AudioListenerSFMLAPI::CoreGetGlobalVolume()
				{
					return sf::Listener::getGlobalVolume();
				}
			}
		}
	}
}
