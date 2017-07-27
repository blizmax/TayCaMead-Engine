#include "RendererWindowQT.h"
#include "TCMNativeWindow/NativeWindow.h"
#include "TCMInputManager/InputManager.h"
#include "TCMCore/Core.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			RendererWindowQT::RendererWindowQT()
				: QWindow()
			{
				fromWinId( WId( Engine::Graphics::NativeWindow::GetWindow() ) )->setParent( this );
			}

			void RendererWindowQT::focusInEvent( QFocusEvent* event )
			{
				QWindow::focusInEvent( event );
				Engine::UserIO::InputManager::SetFocus( true );
			}

			void RendererWindowQT::focusOutEvent( QFocusEvent* event )
			{
				QWindow::focusOutEvent( event );
				Engine::UserIO::InputManager::SetFocus( false );
			}

			void RendererWindowQT::resizeEvent( QResizeEvent* event )
			{
				QWindow::resizeEvent( event );
				TCM::Engine::Core::ResizeRenderer( width(), height() );
			}
		}
	}
}
