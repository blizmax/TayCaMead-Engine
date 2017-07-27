#pragma once

#include "VarInspectorQT.h"

QT_FORWARD_DECLARE_CLASS( QFormLayout )
QT_FORWARD_DECLARE_CLASS( QLineEdit )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class StringVecInspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				StringVecInspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;

				virtual bool UseAllColumns() override;

			protected slots:
				void AddField();
				void OnTextChanged();
				void OnDelete();

			protected:
				QFormLayout* m_formLayout;
				QObjectList m_widgetList;

				std::vector<QLineEdit *> m_fields;
			};
		}
	}
}
