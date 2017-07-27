#include "StringVecInspectorQT.h"

#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include "VarHelper.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			StringVecInspectorQT::StringVecInspectorQT( SerializedInterface* var )
				: VarInspectorQT( var )
			{
				m_widget = new QGroupBox( tr( "Vector" ) );
				QVBoxLayout* layout = new QVBoxLayout;

				m_formLayout = new QFormLayout;

				QPushButton* addButton = new QPushButton( tr( "Add field" ) );
				connect( addButton, SIGNAL( clicked() ), this, SLOT( AddField() ) );

				layout->addLayout( m_formLayout );
				layout->addWidget( addButton );

				m_widget->setLayout( layout );
			}

			void StringVecInspectorQT::UpdateFromValue()
			{
				std::vector<std::string> vector = VarHelper<std::vector<std::string>>::Get( m_var );

				for ( int i = 0; i < vector.size(); ++i )
				{
					if ( i == m_fields.size() )
						AddField();
					m_fields[i]->setText( QString::fromStdString( vector[i] ) );
				}
			}

			bool StringVecInspectorQT::UseAllColumns()
			{
				return true;
			}

			void StringVecInspectorQT::AddField()
			{
				QHBoxLayout* layout = new QHBoxLayout;
				QLineEdit* line = new QLineEdit;

				int length = m_widgetList.length();

				line->setObjectName( QString::number( length ) );
				connect( line, SIGNAL( editingFinished() ), this, SLOT( OnTextChanged() ) );

				QPushButton* removeButton = new QPushButton( "x" );
				removeButton->setObjectName( QString::number( length ) );
				connect( removeButton, SIGNAL( clicked() ), this, SLOT( OnDelete() ) );

				layout->addWidget( line );
				layout->addWidget( removeButton );

				m_formLayout->addRow( tr( "Item " ) + QString::number( length ), layout );
				m_widgetList.append( layout );
				m_fields.push_back( line );
			}

			void StringVecInspectorQT::OnTextChanged()
			{
				QLineEdit* line = qobject_cast<QLineEdit*>( sender() );
				int index = line->objectName().toInt();
				std::vector<std::string> vector = VarHelper<std::vector<std::string>>::Get( m_var );
				vector[index] = line->text().toStdString();
				VarHelper<std::vector<std::string>>::Set( m_var, vector );
			}

			void StringVecInspectorQT::OnDelete()
			{
				int i = sender()->objectName().toInt();
				delete m_widgetList.at( i );
				m_widgetList.removeAt( i );
				m_fields.erase( m_fields.begin() + i );

				std::vector<std::string> vector = VarHelper<std::vector<std::string>>::Get( m_var );
				vector.erase( vector.begin() + i );
				VarHelper<std::vector<std::string>>::Set( m_var, vector );
			}
		}
	}
}
