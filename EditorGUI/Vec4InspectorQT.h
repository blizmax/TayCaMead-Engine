#pragma once

#include <QWidget>
#include "VarInspectorQT.h"
#include <qvalidator.h>

QT_FORWARD_DECLARE_CLASS( QWidget )
QT_FORWARD_DECLARE_CLASS( QLineEdit )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class Vec4InspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				Vec4InspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;

			private slots:
				void OnXChanged( const QString& text ) const;
				void OnYChanged( const QString& text ) const;
				void OnZChanged( const QString& text ) const;
				void OnWChanged( const QString& text ) const;
				void SetThirdMinSize( QWidget* widget ) const;
			private:
				QLineEdit* m_x;
				QLineEdit* m_y;
				QLineEdit* m_z;
				QLineEdit* m_w;
				QValidator* m_validator;
			};
		}
	}
}
