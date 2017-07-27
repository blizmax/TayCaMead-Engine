#pragma once

#include <functional>
#include <vector>
#include "TCMInputManager/DLLDefine.h"
#include "TCMInputManager/BaseInputManagerKeys.h"

#include "MathLib/Vec2.h"
#include <unordered_map>
#include "TCMMemory/TCMEventManager.hpp"

#if _WIN32
namespace TCM
{
	namespace Engine
	{
		namespace UserIO
		{
			namespace Windows
			{
				class TCMCoreInputManagerWin;
			}
		}
	}
}

typedef TCM::Engine::UserIO::Windows::TCMCoreInputManagerWin TCMCustomInputManager;
#else
#error "No Input Manager defined
#endif

#define BIND(func)	std::bind(func, this)
#define BIND_MOUSE(func)	std::bind(func, this, std::placeholders::_1)

namespace TCM
{
	namespace Engine
	{
		namespace UserIO
		{
			/**
			* \brief Combo of key used for binding
			*/
			typedef struct s_ComboKey
			{
				SMouseInput m_mouseInput;
				SSpecialKey m_specialKey;
				SKey m_key;

				inline bool operator==( const s_ComboKey& data ) const
				{
					return ( m_mouseInput == data.m_mouseInput ) && ( m_specialKey == data.m_specialKey ) && ( m_key == data.m_key );
				}
			} ComboKey;
		}
	}
}

namespace std
{
	template<>
	struct hash<TCM::Engine::UserIO::ComboKey>
	{
		size_t operator()( const TCM::Engine::UserIO::ComboKey& combo ) const
		{
			using std::size_t;
			using std::hash;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ( ( hash<uint8_t>()( static_cast<uint8_t>(combo.m_mouseInput) )
						^ ( hash<uint8_t>()( static_cast<uint8_t>(combo.m_specialKey) ) << 1 ) ) >> 1 )
					^ ( hash<uint8_t>()( static_cast<uint8_t>(combo.m_key) ) << 1 );
		}
	};
}

namespace TCM
{
	namespace Engine
	{
		namespace UserIO
		{
			typedef std::function<void( Math::Vec2 )> MouseMove;

			/**
			 * \brief Generic input manager
			 */
			class InputManager
			{
			public:
				virtual ~InputManager();

				/**
				 * \brief Get event connected to special key input, and char input, pressed together
				 * \param specialKey _IN_ SSpecialKey targeted
				 * \param key _IN_ SKey targeted
				 */
				static TCMINPUTMANAGER_DLL Event<>& GetKeyEvent( SSpecialKey specialKey, SKey key );

				/**
				 * \brief Get event connected to specific mouseInput
				 * \param mouseInput _IN_ SMouseInput targeted
				 */
				static TCMINPUTMANAGER_DLL Event<>& GetMouseEvent( SMouseInput mouseInput );

				/**
				 * \brief Get event connected to specific mouseInput, special key input, and char input, pressed together
				 * \param mouseInput _IN_ SMouseInput targeted
				 * \param specialKey _IN_ SSpecialKey targeted
				 * \param key _IN_ SKey targeted
				 */
				static TCMINPUTMANAGER_DLL Event<>& GetEvent( SMouseInput mouseInput, SSpecialKey specialKey, SKey key );

				/**
				 * \brief Clear event binded
				 * \param mouseInput _IN_ SMouseInput targeted
				 * \param specialKey _IN_ SSpecialKey targeted
				 * \param key _IN_ SKey targeted
				 */
				static TCMINPUTMANAGER_DLL void ClearEvent( SMouseInput mouseInput, SSpecialKey specialKey, SKey key );

				/**
				 * \brief Return mouse position in screen
				 * \return Position of the mouse
				 */
				static TCMINPUTMANAGER_DLL Math::Vec2 GetMousePosition();

				/**
				 * \brief Return mouse position in viewport
				 * \return Position of the mouse, X:0 Y:0 is the bottom left, X:1 Y:1 is the top right
				 */
				static TCMINPUTMANAGER_DLL Math::Vec2 GetMousePositionInViewport();

				/**
				 * \brief Check if mouse is in the screen
				 * \return If window is focused
				 */
				static TCMINPUTMANAGER_DLL bool IsFocused();

				/**
				 * \brief Update called on each frame
				 */
				static TCMINPUTMANAGER_DLL void Update();

				/**
				 * \brief Connect function to specific mouseInput
				 * \param mouseInput _IN_ SMouseInput targeted
				 * \param mouseMove _IN_ Action using translation of the mouse
				 */
				static TCMINPUTMANAGER_DLL void AddFunctionToMouseMove( SMouseInput mouseInput, MouseMove mouseMove );
				/**
				 * \brief Connect function to specific special key
				 * \param specialKey _IN_ SSpecialKey targeted
				 * \param mouseMove _IN_ Action using translation of the mouse
				 */
				static TCMINPUTMANAGER_DLL void AddFunctionToMouseMove( SSpecialKey specialKey, MouseMove mouseMove );

				/**
				* \brief Connect function to Mouse move
				* \param mouseMove _IN_ Action using translation of the mouse
				*/
				static TCMINPUTMANAGER_DLL void AddFunctionToMouseMove( MouseMove mouseMove );

				/**
				 * \brief Check if mouse input is pressed
				 * \param key _IN_ SMouseInput targeted
				 * \return Button pressed
				 */
				static TCMINPUTMANAGER_DLL bool KeyDown( SMouseInput key );

				/**
				* \brief Check if key is pressed
				* \param key _IN_ SKey targeted
				* \return Button pressed
				*/
				static TCMINPUTMANAGER_DLL bool KeyDown( SKey key );
				/**
				* \brief Check if special key is pressed
				* \param key _IN_ SSpecialKey targeted
				* \return Button pressed
				*/
				static TCMINPUTMANAGER_DLL bool KeyDown( SSpecialKey key );

				/**
				 * \brief Get mouse translation on the frame
				 * \return Mouse translation
				 */
				static TCMINPUTMANAGER_DLL Math::Vec2 GetMouseTranslation();

				/**
				 * \brief Manually set keyboard focus
				 * \param value _IN_ State of the focus
				 */
				static TCMINPUTMANAGER_DLL void SetFocus( bool value );

				/**
				 * \brief Disable manula focus if setted 
				 */
				static TCMINPUTMANAGER_DLL void DisableManualFocus();

			protected:
				InputManager();

				std::unordered_map<ComboKey, Event<>> m_eventMap;
				bool m_isInTheContext = false;
				Math::Vec2 m_mousePosition;
				Math::Vec2 lastMousePosition;
				Math::Vec2 m_mouseTranslation;

				MouseMove mouseMoveAction;

				bool m_focused = true;
				bool m_manualFocus = false;

			private:
				static InputManager& GetInstance();

				TCMCustomInputManager* coreInstance;

				/**
				 * \brief Update called on each frame
				 */
				void CoreUpdate();

				/**
				 * \brief Check all events, if they can be call
				 */
				void CoreCheckEvents();

				/**
				 * \brief Get event connected to specific mouseInput, special key input, and char input, pressed together
				 * \param mouseInput _IN_ SMouseInput targeted
				 * \param specialKey _IN_ SSpecialKey targeted
				 * \param key _IN_ SKey targeted
				 */
				inline Event<>& CoreGetEvent( SMouseInput mouseInput, SSpecialKey specialKey, SKey key );
				/**
				 * \brief Remove function binded
				 * \param mouseInput _IN_ SMouseInput targeted
				 * \param specialKey _IN_ SSpecialKey targeted
				 * \param key _IN_ SKey targeted
				 */
				inline void CoreClearEvent( SMouseInput mouseInput, SSpecialKey specialKey, SKey key );
				/**
				 * \brief Check if mouse is in the screen
				 * \return If window is focused
				 */
				inline bool CoreIsFocused() const;
				/**
				 * \brief Return mouse position in screen
				 * \return Position of the mouse
				 */
				inline Math::Vec2 CoreGetMousePosition() const;
				/**
				 * \brief Return mouse position in viewport
				 * \return Position of the mouse, X:0 Y:0 is the bottom left, X:1 Y:1 is the top right
				 */
				inline Math::Vec2 CoreGetMousePositionInViewport() const;
				inline void ClampCursorPositionToContextSize();
				inline void CheckIfInsideTheContext();
				/**
				 * \brief Connect function to specific mouseInput
				 * \param mouseInput _IN_ SMouseInput targeted
				 * \param specialKey _IN_ SSpecialKey targeted
				 * \param mouseMove _IN_ Action using translation of the mouse
				 */
				inline void CoreAddFunctionToMouseMove( SMouseInput mouseInput, SSpecialKey specialKey, MouseMove mouseMove );
				inline void CoreHandleMouseMovements();

				/**
				 * \brief Check if mouse input is pressed
				 * \param key _IN_ SMouseInput targeted
				 * \return Button pressed
				 */
				inline bool CoreKeyDown( SMouseInput key ) const;
				/**
				* \brief Check if key is pressed
				* \param key _IN_ SKey targeted
				* \return Button pressed
				*/
				inline bool CoreKeyDown( SKey key ) const;
				/**
				* \brief Check if special key is pressed
				* \param key _IN_ SSpecialKey targeted
				* \return Button pressed
				*/
				inline bool CoreKeyDown( SSpecialKey key ) const;

				/**
				 * \brief Get mouse translation on the frame
				 * \return Mouse translation
				 */
				inline Math::Vec2 CoreGetMouseTranslation() const;
			};
		}
	}
}
