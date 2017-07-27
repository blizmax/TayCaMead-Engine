#include "TCMInputManager/InputManager.h"
#include "TCMLogger/Logger.h"
#include "TCMWindow/AbstractWindow.h"

#include <memory>
#include <mutex>

#if _WIN32
#include "TCMCoreInputManagerWin.h"
typedef TCM::Engine::UserIO::Windows::TCMCoreInputManagerWin TCMCustomInputManager;
#else
#error "No Input Manager defined
#endif

namespace TCM
{
	namespace Engine
	{
		namespace UserIO
		{
			InputManager& InputManager::GetInstance()
			{
				static std::unique_ptr<InputManager> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new InputManager() );
				                }
				);

				return *( m_instance.get() );
			}

			InputManager::InputManager() : coreInstance( new TCMCustomInputManager )
			{
				TCMDEBUG("");
			}

			InputManager::~InputManager()
			{
				TCMDEBUG("");
				delete coreInstance;
			}

			void InputManager::CoreUpdate()
			{
				coreInstance->UpdateMousePosition( m_mousePosition );
				coreInstance->UpdateKeysState();

				CheckIfInsideTheContext();
				ClampCursorPositionToContextSize();

				CoreCheckEvents();
				CoreHandleMouseMovements();
			}

			Event<>& InputManager::GetKeyEvent( SSpecialKey specialKey, SKey key )
			{
				return GetInstance().CoreGetEvent( SMouseInput::NO_MOUSE_INPUT, specialKey, key );
			}

			Event<>& InputManager::GetMouseEvent( SMouseInput mouseInput )
			{
				return GetInstance().CoreGetEvent( mouseInput, SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::NO_KEY_INPUT );
			}

			Event<>& InputManager::GetEvent( SMouseInput mouseInput, SSpecialKey specialKey, SKey key )
			{
				return GetInstance().CoreGetEvent( mouseInput, specialKey, key );
			}

			void InputManager::ClearEvent( SMouseInput mouseInput, SSpecialKey specialKey, SKey key )
			{
				GetInstance().CoreClearEvent( mouseInput, specialKey, key );
			}

			Math::Vec2 InputManager::GetMousePosition()
			{
				return GetInstance().CoreGetMousePosition();
			}

			Math::Vec2 InputManager::GetMousePositionInViewport()
			{
				return GetInstance().CoreGetMousePositionInViewport();
			}

			bool InputManager::IsFocused()
			{
				return GetInstance().CoreIsFocused();
			}

			bool InputManager::CoreIsFocused() const
			{
				if ( m_manualFocus )
					return m_focused;
				return coreInstance->BaseIsFocused();
			}

			void InputManager::Update()
			{
				GetInstance().CoreUpdate();
			}

			void InputManager::AddFunctionToMouseMove( SMouseInput mouseInput, MouseMove mouseMove )
			{
				GetInstance().CoreAddFunctionToMouseMove( mouseInput, SSpecialKey::NO_SPECIAL_KEY_INPUT, mouseMove );
			}

			void InputManager::AddFunctionToMouseMove( SSpecialKey specialKey, MouseMove mouseMove )
			{
				GetInstance().CoreAddFunctionToMouseMove( SMouseInput::NO_MOUSE_INPUT, specialKey, mouseMove );
			}

			void InputManager::AddFunctionToMouseMove( MouseMove mouseMove )
			{
				GetInstance().CoreAddFunctionToMouseMove( SMouseInput::NO_MOUSE_INPUT, SSpecialKey::NO_SPECIAL_KEY_INPUT, mouseMove );
			}

			bool InputManager::KeyDown( SMouseInput key )
			{
				return GetInstance().CoreKeyDown( key );
			}

			bool InputManager::KeyDown( SKey key )
			{
				return GetInstance().CoreKeyDown( key );
			}

			bool InputManager::KeyDown( SSpecialKey key )
			{
				return GetInstance().CoreKeyDown( key );
			}

			Math::Vec2 InputManager::GetMouseTranslation()
			{
				return GetInstance().CoreGetMouseTranslation();
			}

			void InputManager::SetFocus( bool value )
			{
				GetInstance().m_manualFocus = true;
				GetInstance().m_focused = value;
			}

			void InputManager::DisableManualFocus()
			{
				GetInstance().m_manualFocus = false;
			}

			void InputManager::CoreCheckEvents()
			{
				//if ( isInTheContext && coreInstance->BaseIsFocused() )
				//if ( coreInstance->BaseIsWindowFocused() )
				if ( CoreIsFocused() )
					for ( auto iter = m_eventMap.begin(); iter != m_eventMap.end(); ++iter )
					{
						ComboKey comboKey = iter->first;
						Event<>& keyEvent = iter->second;

						if ( coreInstance->KeyDown( comboKey.m_mouseInput ) )
						{
							if ( coreInstance->KeyDown( comboKey.m_specialKey ) )
							{
								if ( coreInstance->KeyDown( comboKey.m_key ) )
									keyEvent.Invoke();
							}
						}
					}
			}

			Event<>& InputManager::CoreGetEvent( SMouseInput mouseInput, SSpecialKey specialKey, SKey key )
			{
				ComboKey comboKey = { mouseInput, specialKey, key };
				return m_eventMap[comboKey];
			}

			void InputManager::CoreClearEvent( SMouseInput mouseInput, SSpecialKey specialKey, SKey key )
			{
				ComboKey comboKey = { mouseInput, specialKey, key };
				auto result = m_eventMap.find( comboKey );
				if ( result != m_eventMap.end() )
					m_eventMap.erase( result );
			}

			Math::Vec2 InputManager::CoreGetMousePosition() const
			{
				return m_mousePosition;
			}

			Math::Vec2 InputManager::CoreGetMousePositionInViewport() const
			{
				int width, height;
				Graphics::AbstractWindow::GetSize( width, height );
				Math::Vec2 vec2 = Math::Vec2( m_mousePosition.x / width, m_mousePosition.y / height );
				if ( vec2.x < 0.0f )
					vec2.x = 0.0f;
				if ( vec2.y < 0.0f )
					vec2.y = 0.0f;

				if ( vec2.x > 1.0f )
					vec2.x = 1.0f;
				if ( vec2.y > 1.0f )
					vec2.y = 1.0f;
				return vec2;
			}

			inline void InputManager::ClampCursorPositionToContextSize()
			{
				int width, height;
				Graphics::AbstractWindow::GetSize( width, height );
				m_mousePosition.x = ( m_mousePosition.x >= width ) ? width : ( m_mousePosition.x <= 0 ) ? 0 : m_mousePosition.x;
				m_mousePosition.y = ( m_mousePosition.y >= height ) ? height : ( m_mousePosition.y <= 0 ) ? 0 : m_mousePosition.y;
			}

			inline void InputManager::CheckIfInsideTheContext()
			{
				int width, height;
				Graphics::AbstractWindow::GetSize( width, height );
				m_isInTheContext = ( m_mousePosition.x <= width && m_mousePosition.x >= 0 ) && ( m_mousePosition.y <= height && m_mousePosition.y >= 0 );
			}

			void InputManager::CoreAddFunctionToMouseMove( SMouseInput mouseInput, SSpecialKey specialKey, MouseMove mouseMove )
			{
				mouseMoveAction = mouseMove;
			}

			void InputManager::CoreHandleMouseMovements()
			{
				Math::Vec2 mousePosInViewport = CoreGetMousePositionInViewport();

				if ( !CoreIsFocused() )
					m_mouseTranslation = Math::Vec2();
				else
					m_mouseTranslation = lastMousePosition - mousePosInViewport;

				if ( mouseMoveAction )
					mouseMoveAction( m_mouseTranslation );

				lastMousePosition = mousePosInViewport;
			}

			bool InputManager::CoreKeyDown( SMouseInput key ) const
			{
				return coreInstance->KeyDown( key );
			}

			bool InputManager::CoreKeyDown( SKey key ) const
			{
				return coreInstance->KeyDown( key );
			}

			bool InputManager::CoreKeyDown( SSpecialKey key ) const
			{
				return coreInstance->KeyDown( key );
			}

			Math::Vec2 InputManager::CoreGetMouseTranslation() const
			{
				return m_mouseTranslation;
			}
		}
	}
}
