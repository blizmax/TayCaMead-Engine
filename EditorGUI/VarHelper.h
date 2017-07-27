#pragma once
#include "TCMMemory/TCMSerialization.h"
#include "EditorCommandManager.h"
#include "EditorVarCommand.h"

namespace TCM
{
	namespace Editor
	{
		template<typename T>
		class VarHelper
		{
		public:

			static T Get( SerializedInterface* var )
			{
				return var->GetPointerless<T>();
			}

			static void Set( SerializedInterface* var, const T& value )
			{
				EditorCommandManager::AddCommand( new EditorVarCommand<T>( var, value ) );
			}
		};
	}
}
