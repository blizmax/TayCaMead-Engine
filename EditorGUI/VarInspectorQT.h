#pragma once

#pragma once
#include "VarInspector.h"
#include <QObject>

QT_FORWARD_DECLARE_CLASS( QWidget )
QT_FORWARD_DECLARE_CLASS( QLineEdit )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class VarInspectorQT :
					public QObject, public VarInspector
			{
				Q_OBJECT

			public:
				explicit VarInspectorQT( SerializedInterface* var )
					: QObject( nullptr )
					, VarInspector( var )
					, m_widget( nullptr )
				{
				}

				~VarInspectorQT()
				{
				}

				QWidget* GetQWidget() const;

				void SetReadOnly( QWidget* widget, bool readOnly ) const;

				void SetReadOnly( QLineEdit* lineEdit, bool readOnly ) const;

			protected:
				QWidget* m_widget;
			};
		}
	}
}
