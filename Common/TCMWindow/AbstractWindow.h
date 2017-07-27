#pragma once
#include "DLLDefines.h"
#include "TCMMemory/TCMEventManager.hpp"

#define BASE_WINDOW_WIDTH	1280
#define BASE_WINDOW_HEIGHT	 720

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			/**
			* \brief Abstract class auto-implemented to manage Window
			*/
			class AbstractWindow abstract
			{
			public:
				virtual ~AbstractWindow() = default;
				/**
				* \brief Initialize new Window
				* \note Uses static members for Window size
				*/
				static TCMWINDOW_EXPORT void Init();
				/**
				* \brief Initialize new Window
				* \param width _IN_ Size X of the new Window
				* \param height _IN_ Size Y of the new Window
				*/
				static TCMWINDOW_EXPORT void Init( const int width, const int height );
				/**
				* \brief Stuff to be done before rendering RHI
				*/
				static TCMWINDOW_EXPORT void RenderFinish();
				/**
				* \brief Stuff to be done after rendering RHI
				*/
				static void RenderCallback();
				/**
				* \brief Stuff to be done after closing the window
				*/
				static TCMWINDOW_EXPORT void CloseCallback();
				/**
				* \brief Sets the width (size on X) of the Window
				* \param width _IN_ Size X of the Window
				*/
				static TCMWINDOW_EXPORT void SetWidth( const int width );
				/**
				* \brief Sets the height (size on Y) of the Window
				* \param height _IN_ Size Y of the Window
				*/
				static TCMWINDOW_EXPORT void SetHeight( const int height );
				/**
				* \brief Resize callback
				* \param width The size X of the new Window size
				* \param height The size Y of the new Window size
				*/
				static TCMWINDOW_EXPORT void ResizeCallback( const int width, const int height );

				/**
				 * \brief Get size of the window
				 * \param width _OUT_ Width of the window
				 * \param height _OUT_ Height of the window
				 */
				static TCMWINDOW_EXPORT void GetSize( int& width, int& height );

				/**
				* \brief Set current window context, specific
				*/
				static TCMWINDOW_EXPORT void MakeCurrent();

				/**
				 * \brief Get event called at each rendering
				 */
				static TCMWINDOW_EXPORT Event<>& GetRenderEvent();

				/**
				 * \brief Get event called at window close
				 */
				static TCMWINDOW_EXPORT Event<>& GetCloseEvent();

			protected:
				AbstractWindow() = default;
				/**
				* \brief Initialize new Window
				* \note Uses members for Window size
				*/
				void CoreInit();
				/**
				* \brief Initialize new Window
				* \param width _IN_ Size X of the new Window
				* \param height _IN_ Size Y of the new Window
				*/
				void CoreInit( const int width, const int height );
				/**
				* \brief Creates new Window
				*/
				virtual void CoreCreateWindow() = 0;
				/**
				* \brief Stuff to be done before rendering RHI
				*/
				virtual void CoreRenderFinish() = 0;
				/**
				* \brief Stuff to be done after rendering RHI
				*/
				virtual void CoreRenderCallback() = 0;

				/**
				* \brief Set current window context, specific
				*/
				virtual void CoreMakeCurrent() = 0;
				/**
				* \brief Resize callback
				* \param width The size X of the new Window size
				* \param height The size Y of the new Window size
				*/
				void CoreResizeCallback( const int width, const int height );
				/**
				* \brief Window's width (size X)
				*/
				int m_width = BASE_WINDOW_WIDTH;
				/**
				* \brief Window's height (size Y)
				*/
				int m_height = BASE_WINDOW_HEIGHT;

			private:
				/**
				* \brief Get the unique instance of the AbstractWindow
				* \return A reference to the AbstractWindow
				*/
				static AbstractWindow& GetInstance();

				Event<> m_renderCallbackEvent;
				Event<> m_closeCallbackEvent;
			};
		}
	}
}
