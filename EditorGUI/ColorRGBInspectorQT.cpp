#include "ColorRGBInspectorQT.h"
#include <QPushButton>
#include <QColorDialog>
#include "MathLib/ColorRGB.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			ColorRGBInspectorQT::ColorRGBInspectorQT( SerializedInterface* var ) :
			                                                                     VarInspectorQT( var )
			{
				QPushButton* button = new QPushButton;
				connect( button, SIGNAL( clicked() ), this, SLOT( OnClick() ) );
				m_widget = button;
				m_widget->setAutoFillBackground( true );

				SetReadOnly( m_widget, var->GetEditorType() == SerializedInterface::READONLY );
			}

			void ColorRGBInspectorQT::UpdateFromValue()
			{
				QPalette pal = m_widget->palette();
				pal.setColor( QPalette::Button, GetQT() );

				m_widget->setPalette( pal );
				m_widget->update();
			}

			QColor ColorRGBInspectorQT::GetQT() const
			{
				Engine::Math::ColorRGB color = VarHelper<Engine::Math::ColorRGB>::Get( m_var );
				return QColor::fromRgbF( color.r, color.g, color.b );
			}

			void ColorRGBInspectorQT::OnClick()
			{
				QColor color = QColorDialog::getColor( GetQT(), m_widget );

				if ( color.isValid() )
				{
					Engine::Math::ColorRGB tcmColor( color.redF(), color.greenF(), color.blueF() );
					VarHelper<Engine::Math::ColorRGB>::Set( m_var, tcmColor );
					UpdateFromValue();
				}
			}
		}
	}
}
