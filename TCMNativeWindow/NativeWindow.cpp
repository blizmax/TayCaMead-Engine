#include "TCMNativeWindow\NativeWindow.h"
#include <memory>
#include <mutex>

#ifdef _WIN32
#include "WinNativeWindow.h"
typedef TCM::Engine::Graphics::Windows::WinNativeWindow CustomNativeWindow;
#endif

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			NativeWindow& NativeWindow::GetInstance()
			{
				static std::unique_ptr<NativeWindow> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new CustomNativeWindow() );
				                }
				);

				return *( m_instance.get() );
			}

			void NativeWindow::Init()
			{
				GetInstance().CoreInit();
			}

			void* NativeWindow::GetWindow()
			{
				return GetInstance().m_whd;
			}

			bool NativeWindow::IsActive()
			{
				return GetInstance().CoreIsActive();
			}
		}
	}
}
