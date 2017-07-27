#pragma once
#include "TCMRenderer/RendererDefines.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			/**
			* \brief Abstract class used as a base for renderable objects
			*/
			class TCMRENDERER_DLL RenderableObject abstract
			{
			public:
				RenderableObject() = default;
				virtual ~RenderableObject() = default;

				/**
				* \brief TCM function to manage rendering
				* \return True if can render, false otherwise
				* \warning Should not be manually called nor overriden
				*/
				virtual bool TCMRender() = 0;

			protected:

				/**
				* \brief Renders an object
				*/
				virtual void RenderGizmos() = 0;
			};
		}
	}
}
