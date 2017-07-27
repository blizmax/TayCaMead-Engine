#include "../Common/TCMAudioEngine/AudioEngine/AudioListener.h"
#include <mutex>

#if defined(TCM_SFML_API)
#include "TCMAudioEngine/SFMLAudio/AudioListenerSFMLAPI.h"
typedef TCM::Engine::Audio::SFML::AudioListenerSFMLAPI OverridenAudioListener;
#else
	static_assert(false && "NO AUDIO LISTENER SET !");
#endif

namespace TCM
{
	namespace Engine
	{
		namespace Audio
		{
			AudioListener& AudioListener::GetInstance()
			{
				static std::unique_ptr<AudioListener> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new OverridenAudioListener() );
				                }
				);

				return *m_instance.get();
			}
		}
	}
}
