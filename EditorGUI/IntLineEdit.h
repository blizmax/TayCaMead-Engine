#pragma once

#include <QLineEdit>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class IntLineEdit : public QLineEdit
			{
				Q_OBJECT

			public:
				IntLineEdit( QWidget* parent = 0 );
				void SetValue( const int value );

				signals :
				void OnValueChanged( const int );

			protected slots:

				void OnTextChanged( const QString& text );
			protected:
				QValidator* m_validator;
			};
		}
	}
}
