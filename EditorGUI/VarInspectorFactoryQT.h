#pragma once
#include "VarInspectorFactory.h"

class QWidget;

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class VarInspectorFactoryQT : public VarInspectorFactory
			{
				VarInspector* CoreCreateInspector( VarType type, SerializedInterface* var ) override;
			};
		}
	}
}
