#pragma once
#include <QObject>
#include "VarInspectorQT.h"

QT_FORWARD_DECLARE_CLASS( QLineEdit )
QT_FORWARD_DECLARE_CLASS( QValidator )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class Vec2InspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				Vec2InspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;

			private slots:
				void OnXChanged( const QString& text ) const;
				void OnYChanged( const QString& text ) const;
				void SetThirdMinSize( QWidget* widget ) const;
			private:
				QLineEdit* m_x;
				QLineEdit* m_y;

				QValidator* m_validator;
			};
		}
	}
}
