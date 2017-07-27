#pragma once
#include "TCMWindow/AbstractWindow.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Glut
			{
				/**
				* \brief Implemented AbstractWindow using the Glut API
				*/
				class GlutWindow : public AbstractWindow
				{
				public:
					virtual ~GlutWindow() = default;
				protected:
					/**
					* \brief Creates new Window
					*/
					virtual void CoreCreateWindow() override;
					/**
					* \brief Stuff to be done before rendering RHI
					*/
					virtual void CoreRenderFinish() override;

					virtual void CoreMakeCurrent() override;

				private:
					/**
					* \brief Custom Glut rendering callback
					*/
					virtual void CoreRenderCallback() override;

					int m_renderWindow = 0;
				};
			}
		}
	}
}
