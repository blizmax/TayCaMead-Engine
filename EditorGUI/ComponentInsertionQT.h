#pragma once

#include <QMenu>
#include "ComponentInsertion.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class InspectorQT;

			class ComponentInsertionQT : public QMenu, public ComponentInsertion
			{
				Q_OBJECT

			public:
				ComponentInsertionQT( InspectorQT* inspector );

			private slots:
				void OnAddComponent() const;
			};
		}
	}
}
