#include "ComponentInsertionQT.h"
#include "InspectorQT.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			ComponentInsertionQT::ComponentInsertionQT( InspectorQT* inspector ) :
			                                                                     QMenu( inspector )
			                                                                     , ComponentInsertion( inspector )
			{
				for ( int i = 0; i < m_components.size(); ++i )
				{
					QString name = QString::fromStdString( m_components[i] );
					QAction* action = new QAction( name );
					action->setObjectName( name );
					connect( action, SIGNAL( triggered() ), this, SLOT( OnAddComponent() ) );
					addAction( action );
				}
			}

			void ComponentInsertionQT::OnAddComponent() const
			{
				CreateComponent( sender()->objectName().toStdString() );
			}
		}
	}
}
