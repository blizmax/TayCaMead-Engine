#include <stdexcept>
#include "GlutWindow.h"
#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"
#include "TCMRenderer/Rendering/RenderGraph.h"
#include "TCMGlut.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Glut
			{
				void GlutWindow::CoreCreateWindow()
				{
					int argc = 0;
					char* argv = new char;
					glutInit( &argc, &argv );
					delete argv;
					glutInitWindowSize( m_width, m_height );
					glutInitWindowPosition( 100, 100 );
					glutInitDisplayMode( HDR_ENABLED ? GLUT_SRGB : GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH
						                                | GLUT_BORDERLESS | GLUT_CAPTIONLESS
					);
					m_renderWindow = glutCreateWindow( "TayCaMead Engine" );
					if ( m_renderWindow <= 0 )
						throw std::runtime_error( "TCMRendererOpenGL::CreateRenderWindow() - glutCreateWindow() failed - could not create window" );
#if FREEGLUT
					glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT );
					glutCloseFunc( CloseCallback );
#endif

					glutDisplayFunc( RenderCallback );
					glutReshapeFunc( ResizeCallback );
				}

				void GlutWindow::CoreRenderFinish()
				{
					glutMainLoopEvent();
					glutPostRedisplay();
				}

				void GlutWindow::CoreMakeCurrent()
				{
					glutSetWindow( m_renderWindow );
				}

				void GlutWindow::CoreRenderCallback()
				{
					glutSwapBuffers();
				}
			}
		}
	}
}
