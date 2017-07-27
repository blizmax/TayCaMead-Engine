#include "FloatLineEdit.h"
#include <QValidator>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			FloatLineEdit::FloatLineEdit( QWidget* parent )
				: QLineEdit( parent )
			{
				connect( this, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnTextChanged( const QString & ) ) );
				setPlaceholderText( tr( "Float" ) );
				QDoubleValidator* validator = new QDoubleValidator( this );
				validator->setDecimals( 3 );
				setValidator( validator );
				m_validator = validator;
			}

			void FloatLineEdit::SetValue( const float& value )
			{
				QString char_ = QString::number( value );
				m_validator->fixup( char_ );
				setText( char_ );
			}

			void FloatLineEdit::OnTextChanged( const QString& text )
			{
				bool result;
				float value = text.toFloat( &result );
				if ( result )
				emit OnValueChanged( value );
			}
		}
	}
}
