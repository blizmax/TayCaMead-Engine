#include "ComponentInspectorQT.h"
#include "EngineClasses/TCMComponent.h"

#include <QFormLayout>
#include "BaseInspectorQT.h"
#include <QPushButton>
#include "Inspector.h"
#include "MainWindow.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			ComponentInspectorQT::ComponentInspectorQT( Inspector* inspector, Engine::TCMComponent* component, QWidget* parent ) :
			                                                                                                                     QWidget( parent )
			                                                                                                                     , ComponentInspector( inspector, component )
			{
				QVBoxLayout* mainLayout = new QVBoxLayout( this );
				QHBoxLayout* buttonLayout = new QHBoxLayout;
				QPushButton* button = new QPushButton( QIcon::fromTheme( QString::fromStdString( MainWindow::GetIconPath( IconType::REMOVE ) ) ), tr( "Remove" ) );

				connect( button, SIGNAL( clicked( bool ) ), this, SLOT( DeleteComponent( bool ) ) );

				BaseInspectorQT* baseInspectorQt = new BaseInspectorQT( component );
				buttonLayout->addWidget( button );
				mainLayout->addLayout( buttonLayout );
				mainLayout->addLayout( baseInspectorQt->GetLayout() );
				setLayout( mainLayout );

				m_baseInspector = baseInspectorQt;
			}

			void ComponentInspectorQT::DeleteComponent( bool clicked )
			{
				m_inspector->Remove( this );
			}
		}
	}
}
