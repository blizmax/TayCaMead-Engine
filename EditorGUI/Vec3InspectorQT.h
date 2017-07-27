#pragma once

#include "VarInspectorQT.h"
#include <qvalidator.h>

QT_FORWARD_DECLARE_CLASS( QLineEdit )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class Vec3InspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				Vec3InspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;

			private slots:
				void OnXChanged( const QString& text ) const;
				void OnYChanged( const QString& text ) const;
				void OnZChanged( const QString& text ) const;
				void SetThirdMinSize( QWidget* widget ) const;
			private:

				QLineEdit* m_x;
				QLineEdit* m_y;
				QLineEdit* m_z;
				QValidator* m_validator;
			};
		}
	}
}
