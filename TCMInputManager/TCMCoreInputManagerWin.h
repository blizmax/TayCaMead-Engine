#pragma once

#include <windows.h>

#include "TCMInputManager/TCMCoreInputManager.h"

typedef POINT MousePosition;

namespace TCM
{
	namespace Engine
	{
		namespace UserIO
		{
			namespace Windows
			{
				class TCMCoreInputManagerWin : public TCMCoreInputManager<uint8_t>
				{
				public:
					TCMCoreInputManagerWin();

					~TCMCoreInputManagerWin();

					// * Function that retrieve Mouse/Keyboard event
					virtual void UpdateMousePosition( Math::Vec2& pos ) override;
					virtual bool BaseIsFocused() const override;
					virtual void UpdateKeysState() override;

#pragma region Key States
					// TODO: Double / Triple mod key combo detection

					bool CoreKeyDown( SMouseInput key ) const override;
					bool CoreKeyUp( SMouseInput key ) const override;
					bool CoreKeyToggled( SMouseInput key ) const override;

					bool CoreKeyDown( SKey key ) const override;
					bool CoreKeyUp( SKey key ) const override;
					bool CoreKeyToggled( SKey key ) const override;

					bool CoreKeyDown( SSpecialKey key ) const override;
					bool CoreKeyUp( SSpecialKey key ) const override;
					bool CoreKeyToggled( SSpecialKey key ) const override;
#pragma  endregion

				private:
					MousePosition* m_winMousePoint;
					HWND m_currentWindow;
					HDC m_currentWindowHandle;
				};
			}
		}
	}
}
