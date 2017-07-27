#include "WinNativeWindow.h"
#include <windows.h>

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Windows
			{
				void WinNativeWindow::CoreInit()
				{
					m_whd = GetActiveWindow();
				}

				bool WinNativeWindow::CoreIsActive()
				{
					return GetActiveWindow() == m_whd;
				}
			}
		}
	}
}
