#pragma once

#include "TCMComponentDrawable.h"

namespace TCM
{
	namespace Engine
	{
		class ENGINECLASSES_DLL_EXPORT TCMComponentCubeMesh : public TCMComponentDrawable
		{
			TCM_CLASS(TCMComponentCubeMesh, TCMComponentDrawable);

		public:
			TCMComponentCubeMesh();
			virtual ~TCMComponentCubeMesh();
		};
	}
}
