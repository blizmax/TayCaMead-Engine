#include "ColorRGBAInspectorQT.h"

#include <QPushButton>
#include <QColorDialog>
#include "MathLib/ColorRGBA.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			ColorRGBAInspectorQT::ColorRGBAInspectorQT( SerializedInterface* var ) :
			                                                                       VarInspectorQT( var )
			{
				QPushButton* button = new QPushButton;
				connect( button, SIGNAL( clicked() ), this, SLOT( OnClick() ) );
				m_widget = button;
				m_widget->setAutoFillBackground( true );

				SetReadOnly( m_widget, var->GetEditorType() == SerializedInterface::READONLY );
			}

			void ColorRGBAInspectorQT::UpdateFromValue()
			{
				QPalette pal = m_widget->palette();
				pal.setColor( QPalette::Button, GetQT() );

				m_widget->setPalette( pal );
				m_widget->update();
			}

			QColor ColorRGBAInspectorQT::GetQT() const
			{
				Engine::Math::ColorRGBA color = VarHelper<Engine::Math::ColorRGBA>::Get( m_var );
				return QColor::fromRgbF( color.r, color.g, color.b, color.a );
			}

			void ColorRGBAInspectorQT::OnClick()
			{
				QColor color = QColorDialog::getColor( GetQT(), m_widget, QString(), QColorDialog::ColorDialogOption::ShowAlphaChannel );

				if ( color.isValid() )
				{
					Engine::Math::ColorRGBA tcmColor( color.redF(), color.greenF(), color.blueF(), color.alphaF() );
					VarHelper<Engine::Math::ColorRGBA>::Set( m_var, tcmColor );
					UpdateFromValue();
				}
			}
		}
	}
}
