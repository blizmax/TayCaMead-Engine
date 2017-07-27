#pragma once

#include <QLineEdit>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class FloatLineEdit : public QLineEdit
			{
				Q_OBJECT

			public:
				FloatLineEdit( QWidget* parent = 0 );
				void SetValue( const float& value );

				signals :
				void OnValueChanged( const float& );
			protected slots:
				void OnTextChanged( const QString& text );

			protected:
				QValidator* m_validator;
			};
		}
	}
}
