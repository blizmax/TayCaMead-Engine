#include "TCMCoreInputManagerWin.h"
#include "TCMLogger/Logger.h"

#include <stdlib.h>
#include <mutex>
#include "MathLib/Vec2.h"

#include "TCMNativeWindow/NativeWindow.h"

#pragma region Init and co.
namespace TCM
{
	namespace Engine
	{
		namespace UserIO
		{
			namespace Windows
			{
				TCMCoreInputManagerWin::TCMCoreInputManagerWin()
				{
					m_winMousePoint = new MousePosition();
					m_currentWindow = static_cast<HWND>(Graphics::NativeWindow::GetWindow());

					RECT* rect = new RECT();

					GetWindowRect( m_currentWindow, rect );

					Math::Vec2 window( static_cast<float>(rect->right - rect->left), static_cast<float>(rect->bottom - rect->top) );

					m_currentWindowHandle = GetWindowDC( m_currentWindow );
					SetViewportOrgEx( m_currentWindowHandle, 0, static_cast<int>(window.y), nullptr );
					delete rect;

					uint8_t winMouseInput[INT(SMouseInput::COUNT)] =
					{
						0x01,
						0x02,
						0x04,
						0x00,
					};

					int length;

					length = INT(SMouseInput::COUNT);
					for ( int i = 0; i < length; i++ )
					{
						mouseInputs[i] = winMouseInput[i];
					}

					uint8_t winSpecialKey[INT(SSpecialKey::COUNT)] =
					{
						0x08,
						0x09,
						0x0D,
						0x10,
						0x11,
						0x12,
						0x1B,
						0x20,
						0x25,
						0x26,
						0x27,
						0x28,
						0x00,
					};

					length = INT(SSpecialKey::COUNT);
					for ( int i = 0; i < length; i++ )
					{
						specialKeys[i] = winSpecialKey[i];
					}

					uint8_t winKey[INT(SKey::COUNT)] =
					{
						0x30,
						0x31,
						0x32,
						0x33,
						0x34,
						0x35,
						0x36,
						0x37,
						0x38,
						0x39,

						0x41,
						0x42,
						0x43,
						0x44,
						0x45,
						0x46,
						0x47,
						0x48,
						0x49,
						0x4A,
						0x4B,
						0x4C,
						0x4D,
						0x4E,
						0x4F,
						0x50,
						0x51,
						0x52,
						0x53,
						0x54,
						0x55,
						0x56,
						0x57,
						0x58,
						0x59,
						0x5A,
						0x00,
					};

					length = INT(SKey::COUNT);
					for ( int i = 0; i < length; i++ )
					{
						keys[i] = winKey[i];
					}
				}

				TCMCoreInputManagerWin::~TCMCoreInputManagerWin()
				{
					delete m_winMousePoint;
				}

#pragma endregion

				bool TCMCoreInputManagerWin::BaseIsFocused() const
				{
					return GetFocus() == m_currentWindow;
				}

				void TCMCoreInputManagerWin::UpdateKeysState()
				{
				}

				void TCMCoreInputManagerWin::UpdateMousePosition( Math::Vec2& pos )
				{
					GetCursorPos( m_winMousePoint );
					ScreenToClient( m_currentWindow, m_winMousePoint );
					DPtoLP( m_currentWindowHandle, m_winMousePoint, 1 );

					// HACK: After redefying the new logical origin of the window, Y coordinates need to be sign-reversed or, they'll be lesser than 0 (Having a negative-size window is a bad idea)
					m_winMousePoint->y *= -1;

					pos.x = static_cast<float>(m_winMousePoint->x);
					pos.y = static_cast<float>(m_winMousePoint->y);
				}

				bool TCMCoreInputManagerWin::CoreKeyDown( SMouseInput key ) const
				{
					return ( GetAsyncKeyState( GetMouseInput( key ) ) << 16 ) != 0;
				}

				bool TCMCoreInputManagerWin::CoreKeyUp( SMouseInput key ) const
				{
					return !KeyDown( key );
				}

				bool TCMCoreInputManagerWin::CoreKeyToggled( SMouseInput key ) const
				{
					return ( GetAsyncKeyState( GetMouseInput( key ) ) << 16 ) != 0;
				}

				bool TCMCoreInputManagerWin::CoreKeyDown( SKey key ) const
				{
					return ( GetAsyncKeyState( GetKey( key ) ) << 16 ) != 0;
				}

				bool TCMCoreInputManagerWin::CoreKeyUp( SKey key ) const
				{
					return !KeyDown( key );
				}

				bool TCMCoreInputManagerWin::CoreKeyToggled( SKey key ) const
				{
					return ( GetAsyncKeyState( GetKey( key ) ) << 16 ) != 0;
				}

				bool TCMCoreInputManagerWin::CoreKeyDown( SSpecialKey key ) const
				{
					return ( GetAsyncKeyState( GetSpecialKey( key ) ) << 16 ) != 0;
				}

				bool TCMCoreInputManagerWin::CoreKeyUp( SSpecialKey key ) const
				{
					return !KeyDown( key );
				}

				bool TCMCoreInputManagerWin::CoreKeyToggled( SSpecialKey key ) const
				{
					return ( GetAsyncKeyState( GetSpecialKey( key ) ) << 16 ) != 0;
				}
			}
		}
	}
}
