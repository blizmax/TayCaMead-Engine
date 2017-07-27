// TCMMemory.cpp : Defines the exported functions for the DLL application.
//

#include "TCMMemory/MemoryDefines.h"
#include "EngineClasses/TCMBase.h"

namespace TCM
{
	namespace Engine
	{
		namespace Memory
		{
			template MEMORY_ALWAYS_EXPORT class TCMMetaFactory<TCMBase>;
			template MEMORY_ALWAYS_EXPORT class TCMSerializedFactory<std::string>;
		}
	}
}
