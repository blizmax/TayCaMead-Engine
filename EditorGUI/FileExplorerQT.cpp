#include "FileExplorerQT.h"
#include <QSplitter>
#include <QLineEdit>
#include <QListView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QDirModel>
#include <QFileSystemModel>
#include <QHeaderView>

#include "MainWindow.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			FileExplorerQT::FileExplorerQT( QWidget* parent )
				: QWidget( parent )
			{
				QVBoxLayout* layout = new QVBoxLayout( this );

				QHBoxLayout* actionsLayout = new QHBoxLayout();
				QPushButton* createButton = new QPushButton( tr( "Create" ) );
				connect( createButton, SIGNAL( clicked() ), this, SLOT( CreateClicked() ) );

				actionsLayout->addWidget( createButton );

				m_SearchField = new QLineEdit( this );
				m_SearchField->setPlaceholderText( tr( "Search" ) );

				dirModel = new QFileSystemModel;
				dirModel->setRootPath( QDir::currentPath() );
				dirModel->setFilter( QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks );

				fileModel = new QFileSystemModel;
				fileModel->setRootPath( QDir::currentPath() );
				fileModel->setFilter( QDir::NoDotAndDotDot | QDir::Files | QDir::NoSymLinks );
				QStringList filters;
				filters.append( "*.cc" ); // whatever filters you want
				filters.append( "*.h" );
				fileModel->setNameFilterDisables( false );
				fileModel->setNameFilters( filters );

				/*QString path = QDir::currentPath();
				QModelIndex model = dirModel->index(QDir::currentPath());
				std::string str = path.toStdString();*/

				m_Tree = new QTreeView();
				m_Tree->setModel( dirModel );
				m_Tree->setHeaderHidden( true );
				m_Tree->expandAll();
				m_Tree->setRootIndex( dirModel->index( QDir::currentPath() ) );
				m_Tree->setAnimated( true );
				//m_Tree->setRootIndex(model->index("C:/Users/"));

				//connect(m_Tree, SIGNAL(clicked(QModelIndex)), this, SLOT(OnTreeViewClicked(QModelIndex)));

				QHeaderView* header = m_Tree->header();
				header->setSectionResizeMode( QHeaderView::ResizeToContents );

				for ( int i = 1; i < header->count(); i++ )
					header->hideSection( i );

				m_List = new QListView();
				m_List->setModel( fileModel );
				m_List->setRootIndex( fileModel->index( QDir::currentPath() ) );
				m_List->setSelectionMode( QAbstractItemView::ExtendedSelection );
				m_List->setUniformItemSizes( true );
				m_List->setDragEnabled( true );
				m_List->setAcceptDrops( true );
				m_List->setDropIndicatorShown( true );

				QSplitter* splitter = new QSplitter();
				splitter->addWidget( m_Tree );
				splitter->addWidget( m_List );

				layout->addLayout( actionsLayout );
				layout->addWidget( m_SearchField );
				layout->addWidget( splitter );

				setLayout( layout );
			}

			FileExplorerQT::~FileExplorerQT()
			{
			}

			void FileExplorerQT::CreateClicked() const
			{
				/*QTreeWidgetItem* currentItem = m_Tree->currentItem();
				if (!currentItem)
				m_Tree->addTopLevelItem(new QTreeWidgetItem());
				else
				currentItem->addChild(new QTreeWidgetItem(currentItem));*/
			}

			void FileExplorerQT::SetDirectory( QString path ) const
			{
				dirModel->setRootPath( path );
				fileModel->setRootPath( path );

				m_Tree->setModel( dirModel );
				m_List->setModel( fileModel );
				/*QDirModel * model = new QDirModel(this);
				model->setReadOnly(true);
				model->index(QDir(path).path());
				model->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
				model->setFilter(QDir::Dirs);

				m_Tree->setModel(model);*/
			}

			void FileExplorerQT::AddTopItem( QString name ) const
			{
			}

			void FileExplorerQT::AddItem( int index, QString str ) const
			{
			}

			void FileExplorerQT::OnTreeViewClicked( const QModelIndex& index ) const
			{
				// TreeView clicked
				// 1. We need to extract path
				// 2. Set that path into our ListView

				// Get the full path of the item that's user clicked on
				QString mPath = dirModel->fileInfo( index ).absoluteFilePath();
				m_List->setRootIndex( fileModel->setRootPath( mPath ) );
			}

			void FileExplorerQT::Refresh( Engine::Project* project, Engine::TCMScene* scene )
			{
				SetDirectory( QString::fromStdString( MainWindow::GetProjectFilepath() ) );
			}

			void FileExplorerQT::ShowFiles()
			{
			}

			void FileExplorerQT::ShowFolders()
			{
			}
		}
	}
}
