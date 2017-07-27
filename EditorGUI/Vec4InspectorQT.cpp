#include "Vec4InspectorQT.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <qvalidator.h>
#include "MathLib/Vec4.h"
#include "Vec4Helper.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			Vec4InspectorQT::Vec4InspectorQT( SerializedInterface* var ) :
			                                                             VarInspectorQT( var )
			{
				QDoubleValidator* validator = new QDoubleValidator( this );
				validator->setDecimals( 3 );

				int left, right;
				QHBoxLayout* layout = new QHBoxLayout();
				layout->getContentsMargins( &left, nullptr, &right, nullptr );
				layout->setContentsMargins( left / 2, 0, right / 2, 0 );

				m_x = new QLineEdit;
				m_x->setPlaceholderText( "X" );
				m_x->setValidator( validator );
				SetThirdMinSize( m_x );
				connect( m_x, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnXChanged( const QString & ) ) );
				SetReadOnly( m_x, var->GetEditorType() == SerializedInterface::READONLY );

				m_y = new QLineEdit;
				m_y->setPlaceholderText( "Y" );
				m_y->setValidator( validator );
				SetThirdMinSize( m_y );
				connect( m_y, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnYChanged( const QString & ) ) );
				SetReadOnly( m_y, var->GetEditorType() == SerializedInterface::READONLY );

				m_z = new QLineEdit;
				m_z->setPlaceholderText( "Z" );
				m_z->setValidator( validator );
				SetThirdMinSize( m_z );
				connect( m_z, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnZChanged( const QString & ) ) );
				SetReadOnly( m_z, var->GetEditorType() == SerializedInterface::READONLY );

				m_w = new QLineEdit;
				m_w->setPlaceholderText( "W" );
				m_w->setValidator( validator );
				SetThirdMinSize( m_w );
				connect( m_w, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnWChanged( const QString & ) ) );
				SetReadOnly( m_w, var->GetEditorType() == SerializedInterface::READONLY );

				layout->addWidget( m_x );
				layout->addWidget( m_y );
				layout->addWidget( m_z );
				layout->addWidget( m_w );

				m_widget = new QWidget;
				m_widget->setLayout( layout );

				m_validator = validator;
			}

			void Vec4InspectorQT::UpdateFromValue()
			{
				Engine::Math::Vec4 vec4 = Vec4Helper::Get( m_var );
				QString char_x = QString::number( vec4.x );
				m_validator->fixup( char_x );
				m_x->setText( char_x );
				QString char_y = QString::number( vec4.y );
				m_validator->fixup( char_y );
				m_y->setText( char_y );
				QString char_z = QString::number( vec4.z );
				m_validator->fixup( char_z );
				m_z->setText( char_z );
				QString char_w = QString::number( vec4.w );
				m_validator->fixup( char_w );
				m_w->setText( char_w );
			}

			void Vec4InspectorQT::OnXChanged( const QString& text ) const
			{
				bool result;
				float value = text.toFloat( &result );
				if ( result )
					Vec4Helper::SetX( m_var, value );
			}

			void Vec4InspectorQT::OnYChanged( const QString& text ) const
			{
				bool result;
				float value = text.toFloat( &result );
				if ( result )
					Vec4Helper::SetY( m_var, value );
			}

			void Vec4InspectorQT::OnZChanged( const QString& text ) const
			{
				bool result;
				float value = text.toFloat( &result );
				if ( result )
					Vec4Helper::SetZ( m_var, value );
			}

			void Vec4InspectorQT::OnWChanged( const QString& text ) const
			{
				bool result;
				float value = text.toFloat( &result );
				if ( result )
					Vec4Helper::SetW( m_var, value );
			}

			void Vec4InspectorQT::SetThirdMinSize( QWidget* widget ) const
			{
				widget->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Preferred );
				widget->setMinimumWidth( widget->minimumWidth() / 3 );
			}
		}
	}
}
