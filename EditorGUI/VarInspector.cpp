#include "VarInspector.h"

namespace TCM
{
	namespace Editor
	{
		VarInspector::VarInspector( SerializedInterface* var ) :
		                                                       m_var( var )
		{
		}

		void VarInspector::Init()
		{
			UpdateFromValue();
		}

		void VarInspector::SetVar( SerializedInterface* var )
		{
			m_var = var;
			UpdateFromValue();
		}

		bool VarInspector::UseAllColumns()
		{
			return false;
		}
	}
}
