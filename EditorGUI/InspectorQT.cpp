#include "InspectorQT.h"

#include <QLineEdit>
#include <QFormLayout>
#include <QComboBox>
#include <QToolBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QShowEvent>

#include "EngineClasses/TCMComponent.h"
#include "ComponentInspectorQT.h"
#include "ComponentInsertionQT.h"
#include "BaseInspectorQT.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			InspectorQT::InspectorQT( QWidget* parent )
				: QWidget( parent )
				, Inspector()
			{
				QVBoxLayout* layout = new QVBoxLayout( this );

				BaseInspectorQT* baseInspector = new BaseInspectorQT();
				layout->addLayout( baseInspector->GetLayout() );
				m_baseInspector = baseInspector;

				m_inspectorToolbox = new QToolBox();
				layout->addWidget( m_inspectorToolbox );

				ComponentInsertionQT* creationMenu = new ComponentInsertionQT( this );
				m_creationMenu = creationMenu;
				QPushButton* addButton = new QPushButton( tr( "Add Component" ) );
				addButton->setMenu( creationMenu );

				layout->addWidget( addButton );

				//Implicitly setted with QVBoxLayout* layout = new QVBoxLayout( this );
				//setLayout( layout );

				QSizePolicy sp_retain = sizePolicy();
				sp_retain.setRetainSizeWhenHidden( true );
				setSizePolicy( sp_retain );
			}

			InspectorQT::~InspectorQT()
			{
			}

			void InspectorQT::HideUI()
			{
				hide();
			}

			void InspectorQT::ShowUI()
			{
				show();
			}

			void InspectorQT::EnableUI()
			{
				setEnabled( true );
			}

			void InspectorQT::DisableUI()
			{
				setEnabled( false );
			}

			void InspectorQT::UIRemoveComponent( int index )
			{
				m_inspectorToolbox->removeItem( index );
			}

			void InspectorQT::UIRemoveAllComponents()
			{
				while ( m_inspectorToolbox->count() > 0 )
				{
					m_inspectorToolbox->removeItem( 0 );
				}
			}

			ComponentInspector* InspectorQT::CreateComponentInspector( Engine::TCMComponent* component )
			{
				ComponentInspectorQT* result = new ComponentInspectorQT( this, component );
				m_inspectorToolbox->addItem( static_cast<QWidget *>(result), QString::fromStdString( component->_GetMetaFactoryName() ) );
				return static_cast<ComponentInspector *>(result);
			}
		}
	}
}
