#pragma once
#include "TCMMemory/TCMSerialization.h"

namespace TCM
{
	namespace Editor
	{
		class VarInspector abstract
		{
		public:
			explicit VarInspector( SerializedInterface* var );
			virtual ~VarInspector() = default;

			void Init();
			virtual void UpdateFromValue() = 0;
			void SetVar( SerializedInterface* var );

			virtual bool UseAllColumns();

		protected:
			SerializedInterface* m_var;
		};
	}
}
