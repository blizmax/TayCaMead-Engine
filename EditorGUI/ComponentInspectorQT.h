#pragma once

#include <QWidget>
#include "ComponentInspector.h"

QT_FORWARD_DECLARE_CLASS( QFormLayout )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class ComponentInspectorQT : public QWidget, public ComponentInspector
			{
				Q_OBJECT

			public:
				ComponentInspectorQT( Inspector* inspector, Engine::TCMComponent* component, QWidget* parent = nullptr );

			protected slots:
				void DeleteComponent( bool clicked );
			};
		}
	}
}
