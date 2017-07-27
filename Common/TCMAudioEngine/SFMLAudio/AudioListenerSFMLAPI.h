#pragma once
#include "TCMAudioEngine/AudioEngine/AudioListener.h"

namespace TCM
{
	namespace Engine
	{
		namespace Audio
		{
			namespace SFML
			{
				class AudioListenerSFMLAPI :
						public AudioListener
				{
				public:
					AudioListenerSFMLAPI() = default;
					~AudioListenerSFMLAPI() = default;

				protected:
					void CoreSetPosition( const Math::Vec3& position ) override;
					Math::Vec3 CoreGetPosition() override;

					void CoreSetDirection( const Math::Vec3& direction ) override;
					Math::Vec3 CoreGetDirection() override;

					void CoreSetGlobalVolume( const float& volume ) override;
					float CoreGetGlobalVolume() override;
				};
			}
		}
	}
}
