#pragma once

#include <QObject>
#include "VarInspectorQT.h"
#include "VarHelper.h"

struct SerializedInterface;

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class ColorRGBAInspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				ColorRGBAInspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;

			private:
				QColor GetQT() const;
			private slots:
				void OnClick();
			};
		}
	}
}
