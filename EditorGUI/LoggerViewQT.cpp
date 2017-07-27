#include "LoggerViewQT.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			LoggerViewQT::LoggerViewQT( QWidget* parent )
				: QWidget( parent )
			{
				QVBoxLayout* layout = new QVBoxLayout( this );
				QHBoxLayout* buttonLayout = new QHBoxLayout();
				m_table = new QTableWidget( 0, 5 );
				m_table->setShowGrid( false );

				QStringList list;
				list << tr( "Type" ) << tr( "File" ) << tr( "Function" ) << tr( "Line" ) << tr( "Message" );

				m_table->setHorizontalHeaderLabels( list );
				m_table->horizontalHeader()->setStretchLastSection( true );
				m_table->verticalHeader()->hide();
				m_table->setSortingEnabled( true );

				std::vector<Debug::LogData> logs = Debug::Logger::GetAllLogs();
				for ( int i = 0; i < logs.size(); ++i )
					Push( logs[i] );

				QPushButton* clearButton = new QPushButton( tr( "Clear" ) );
				connect( clearButton, SIGNAL( clicked() ), this, SLOT( ClearQT() ) );

				buttonLayout->addWidget( clearButton );
				layout->addItem( buttonLayout );
				layout->addWidget( m_table );

				//Implicitly setted with QVBoxLayout* layout = new QVBoxLayout( this );
				//setLayout( layout );
			}

			void LoggerViewQT::OnClear()
			{
				if ( m_table == nullptr )
					return;

				m_table->clear();
				m_table->setRowCount( 0 );

				QStringList list;
				list << tr( "Type" ) << tr( "File" ) << tr( "Function" ) << tr( "Line" ) << tr( "Message" );

				m_table->setHorizontalHeaderLabels( list );
				m_table->horizontalHeader()->setStretchLastSection( true );
				m_table->verticalHeader()->hide();
				m_table->setSortingEnabled( true );
			}

			void LoggerViewQT::OnPush( Debug::LogData log )
			{
				if ( m_table == nullptr )
					return;

				m_table->setRowCount( m_listLog.size() );

				int index = m_listLog.size() - 1;

				QTableWidgetItem* fileItem = new QTableWidgetItem( QString::fromStdString( log.m_file ) );
				QTableWidgetItem* funcItem = new QTableWidgetItem( QString::fromStdWString( log.m_function ) );
				QTableWidgetItem* lineItem = new QTableWidgetItem( QString::number( log.m_line ) );
				QTableWidgetItem* messageItem = new QTableWidgetItem( QString::fromStdString( log.m_message ) );

				m_table->setItem( index, 1, fileItem );
				m_table->setItem( index, 2, funcItem );
				m_table->setItem( index, 3, lineItem );
				m_table->setItem( index, 4, messageItem );

				m_table->resizeColumnToContents( 2 );
				m_table->resizeColumnToContents( 3 );
			}

			void LoggerViewQT::ClearQT()
			{
				Clear();
			}
		}
	}
}
