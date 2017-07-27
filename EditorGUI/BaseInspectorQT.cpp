#include "BaseInspectorQT.h"
#include "EngineClasses/TCMBase.h"
#include "VarInspectorQT.h"
#include <QFormLayout>
#include <QGroupBox>

TCM::Editor::QT::BaseInspectorQT::BaseInspectorQT()
	: BaseInspector()
{
	InitLayout();
}

TCM::Editor::QT::BaseInspectorQT::BaseInspectorQT( TCM::Engine::TCMBase* component ) :
                                                                                     BaseInspector( component )
{
	InitLayout();
}

QFormLayout* TCM::Editor::QT::BaseInspectorQT::GetLayout() const
{
	return m_form;
}

void TCM::Editor::QT::BaseInspectorQT::InitLayout()
{
	m_form = new QFormLayout();
	std::unordered_map<std::string, QFormLayout *> categories;

	QFormLayout* secondaryLayout = nullptr;

	for ( auto inspector : m_inspectors )
	{
		QWidget* field = dynamic_cast<VarInspectorQT*>(inspector.second)->GetQWidget();
		if ( !field )
		TCMDEBUG( "Can't get the widget of the VarInspector" );
		else
		{
			std::vector<std::string> path = ParseVarPath( inspector.first );
			if ( path.size() > 1 )
			{
				std::string cs = path[path.size() - 2];
				if ( categories.find( cs ) == categories.end() )
				{
					QGroupBox* groupBox = new QGroupBox( QString::fromStdString( cs ) );
					QFormLayout* layout = new QFormLayout;

					groupBox->setLayout( layout );

					m_form->addRow( groupBox );

					categories[cs] = layout;
				}
				secondaryLayout = categories[cs];
			}
			else
			{
				secondaryLayout = nullptr;
			}

			QFormLayout* effectiveLayout = secondaryLayout == nullptr ? m_form : secondaryLayout;
			if ( !inspector.second->UseAllColumns() )
				effectiveLayout->addRow( QString::fromStdString( path.back() ), field );
			else
				effectiveLayout->addRow( field );
		}
	}
}
