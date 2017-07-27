#include <mutex>
#include "TCMAudioEngine/AudioEngine/AudioEngine.h"

#if defined(TCM_SFML_API)
#include "TCMAudioEngine/SFMLAudio/AudioEngineSFMLAPI.h"
typedef TCM::Engine::Audio::SFML::AudioEngineSFMLAPI OverridenAudioEngine;
#else
	#error "NO PHYSIC ENGINE SET !"
#endif
namespace TCM
{
	namespace Engine
	{
		namespace Audio
		{
			AudioEngine::AudioEngine()
			{
			}

			AudioEngine& AudioEngine::GetInstance()
			{
				static std::unique_ptr<AudioEngine> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new OverridenAudioEngine() );
				                }
				);

				return *m_instance.get();
			}
		}
	}
}
