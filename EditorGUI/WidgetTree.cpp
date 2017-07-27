#include "WidgetTree.h"
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QPushButton>
#include "MainWindowQT.h"

WidgetTree::WidgetTree( TCM::Editor::QT::MainWindowQT* mainWindow, QWidget* parent )
	: QWidget( parent )
	, m_window( mainWindow )
{
	QVBoxLayout* layout = new QVBoxLayout( this );

	tree = new QTreeWidget( nullptr );
	tree->setRootIsDecorated( true );
	QStringList listLabels;
	listLabels << "Class" << "Name" << "Geometry";
	tree->setHeaderLabels( listLabels );
	tree->setSortingEnabled( false );
	layout->addWidget( tree );

	QPushButton* button = new QPushButton( "Refresh" );
	connect( button, SIGNAL( clicked() ), this, SLOT( dumpAllObjectTrees() ) );
	layout->addWidget( button );

	dumpAllObjectTrees();
}

WidgetTree::~WidgetTree()
{
}

void WidgetTree::dumpRecursive( const QObjectList list,
                                QTreeWidgetItem* parent ) const
{
	if ( list.size() == 0 )
		return;
	auto it = list.begin();
	QObject* obj;
	while ( it != list.end() )
	{
		obj = *it;
		if ( obj == tree )
		{
			++it;
			continue;
		}
		QString flags;
		if ( obj->isWidgetType() )
		{
			QWidget* w = static_cast<QWidget *>(obj);
			if ( w->isVisible() )
			{
				flags.sprintf( "<%d,%d,%d,%d>", w->x(),
				               w->y(), w->width(),
				               w->height() );
			}
			else
			{
				flags = "invisible";
			}
		}
		QTreeWidgetItem* child = parent ? new QTreeWidgetItem( parent )
			                         : new QTreeWidgetItem( tree );
		child->setText( 0, obj->metaObject()->className() );
		child->setText( 1, obj->objectName() );
		child->setText( 2, flags );
		dumpRecursive( ( *it )->children(), child );
		++it;
	}
}

void WidgetTree::dumpAllObjectTrees()
{
	tree->clear();
	dumpRecursive( m_window->children(), nullptr );
}
