#pragma once
#include "TCMNativeWindow/NativeWindow.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Windows
			{
				class WinNativeWindow :
						public NativeWindow
				{
				protected:
					void CoreInit() override;
					bool CoreIsActive() override;
				};
			}
		}
	}
}
