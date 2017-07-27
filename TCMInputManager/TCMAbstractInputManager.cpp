#include "TCMAbstractInputManager\TCMAbstractInputManager.h"

#include <memory>
#include <mutex>

#if defined(TCM_WINDOWS_API)
#include "TCMInputManagerWinAPI\TCMInputManagerWindowsAPI.h"
typedef TCMInputManagerWindowsAPI TCMInputManager;
#else
	#error "No Input Manager defined
#endif

TCMAbstractInputManager& TCMAbstractInputManager::GetInstance()
{
	static std::unique_ptr<TCMAbstractInputManager> m_instance;
	static std::once_flag m_onceFlag;

	std::call_once( m_onceFlag,
	                [](void)
	                {
		                m_instance.reset( new TCMInputManager() );
	                }
	);

	return *( m_instance.get() );
}

TCMAbstractInputManager::~TCMAbstractInputManager()
{
}

void TCMAbstractInputManager::Initialize()
{
	GetInstance().BaseInitialize();
}

void TCMAbstractInputManager::CheckInputEvent()
{
	GetInstance().BaseCheckInputEvent();
}
