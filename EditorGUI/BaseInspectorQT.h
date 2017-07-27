#pragma once

#include <QObject>
#include "BaseInspector.h"

QT_FORWARD_DECLARE_CLASS( QFormLayout )

namespace TCM
{
	namespace Engine
	{
		class TCMBase;
	}

	namespace Editor
	{
		class Inspector;

		namespace QT
		{
			class BaseInspectorQT : public QObject, public BaseInspector
			{
				Q_OBJECT

			public:
				BaseInspectorQT();
				BaseInspectorQT( TCM::Engine::TCMBase* component );

				QFormLayout* GetLayout() const;
			private:
				void InitLayout();
				QFormLayout* m_form = nullptr;
			};
		}
	}
}
