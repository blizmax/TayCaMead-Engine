#include "Vec2InspectorQT.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QValidator>
#include "TCMMemory/TCMSerialization.h"
#include "MathLib/Vec2.h"
#include "Vec2Helper.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			Vec2InspectorQT::Vec2InspectorQT( SerializedInterface* var ) :
			                                                             VarInspectorQT( var )
			{
				QDoubleValidator* validator = new QDoubleValidator();
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

				layout->addWidget( m_x );
				layout->addWidget( m_y );

				m_widget = new QWidget;
				m_widget->setLayout( layout );

				m_validator = validator;
			}

			void Vec2InspectorQT::UpdateFromValue()
			{
				Engine::Math::Vec2 vec2 = Vec2Helper::Get( m_var );
				QString char_x = QString::number( vec2.x );
				m_validator->fixup( char_x );
				m_x->setText( char_x );
				QString char_y = QString::number( vec2.y );
				m_validator->fixup( char_y );
				m_y->setText( char_y );
			}

			void Vec2InspectorQT::OnXChanged( const QString& text ) const
			{
				bool result;
				float value = text.toFloat( &result );
				if ( result )
					Vec2Helper::SetX( m_var, value );
			}

			void Vec2InspectorQT::OnYChanged( const QString& text ) const
			{
				bool result;
				float value = text.toFloat( &result );
				if ( result )
					Vec2Helper::SetY( m_var, value );
			}

			void Vec2InspectorQT::SetThirdMinSize( QWidget* widget ) const
			{
				widget->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Preferred );
				widget->setMinimumWidth( widget->minimumWidth() / 3 );
			}
		}
	}
}
