#include "IntLineEdit.h"
#include <QValidator>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			IntLineEdit::IntLineEdit( QWidget* parent )
				: QLineEdit( parent )
			{
				connect( this, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnTextChanged( const QString & ) ) );
				setPlaceholderText( tr( "Integer" ) );
				QIntValidator* validator = new QIntValidator( this );
				setValidator( validator );
				m_validator = validator;
			}

			void IntLineEdit::SetValue( const int value )
			{
				QString char_ = QString::number( value );
				m_validator->fixup( char_ );
				setText( char_ );
			}

			void IntLineEdit::OnTextChanged( const QString& text )
			{
				bool result;
				int value = text.toInt( &result );
				if ( result )
				emit OnValueChanged( value );
			}
		}
	}
}
