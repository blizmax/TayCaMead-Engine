#include "TCMWindow/AbstractWindow.h"

#include <mutex>

#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"
#include "TCMRenderer/Rendering/RenderGraph.h"
#include "TCMRenderer/Rendering/RendererFactory.h"

#if defined(TCMGLUT)
#include "GlutWindow.h"
typedef TCM::Engine::Graphics::Glut::GlutWindow TCMWindowManager;
#else
#error "Not valid TCMWindowManager"
#endif

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			void AbstractWindow::Init()
			{
				GetInstance().CoreInit();
			}

			void AbstractWindow::Init( const int width, const int height )
			{
				GetInstance().CoreInit( width, height );
			}

			AbstractWindow& AbstractWindow::GetInstance()
			{
				static std::unique_ptr<AbstractWindow> m_instance;
				static std::once_flag m_onceFlag;
				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new TCMWindowManager() );
				                }
				);
				return *( m_instance.get() );
			}

			void AbstractWindow::CoreInit()
			{
				CoreCreateWindow();
				RendererFactory::Init( m_width, m_height );
			}

			void AbstractWindow::CoreInit( const int width, const int height )
			{
				m_width = width;
				m_height = height;
				CoreInit();
			}

			void AbstractWindow::RenderFinish()
			{
				GetInstance().CoreRenderFinish();
				GetInstance().m_renderCallbackEvent.Invoke();
			}

			void AbstractWindow::RenderCallback()
			{
				GetInstance().CoreRenderCallback();
				RenderGraph::PostRender();
			}

			void AbstractWindow::CloseCallback()
			{
				GetInstance().m_closeCallbackEvent.Invoke();
			}

			void AbstractWindow::SetWidth( const int width )
			{
				GetInstance().m_width = width;
			}

			void AbstractWindow::SetHeight( const int height )
			{
				GetInstance().m_height = height;
			}

			void AbstractWindow::ResizeCallback( const int width, const int height )
			{
				GetInstance().CoreResizeCallback( width, height );
			}

			void AbstractWindow::GetSize( int& width, int& height )
			{
				width = GetInstance().m_width;
				height = GetInstance().m_height;
			}

			void AbstractWindow::MakeCurrent()
			{
				GetInstance().CoreMakeCurrent();
			}

			Event<>& AbstractWindow::GetRenderEvent()
			{
				return GetInstance().m_renderCallbackEvent;
			}

			Event<>& AbstractWindow::GetCloseEvent()
			{
				return GetInstance().m_closeCallbackEvent;
			}

			void AbstractWindow::CoreResizeCallback( const int width, const int height )
			{
				m_width = width;
				m_height = height;
				RendererFactory::Resize( width, height );
			}
		}
	}
}
