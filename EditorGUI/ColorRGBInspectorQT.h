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
			class ColorRGBInspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				ColorRGBInspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;
			private:
				QColor GetQT() const;
			private slots:
				void OnClick();
			};
		}
	}
}
