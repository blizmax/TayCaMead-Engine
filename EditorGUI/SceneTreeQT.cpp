#include "SceneTreeQT.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>

#include "EngineClasses/TCMScene.h"
#include "EngineClasses/TCMSceneObject.h"
#include "SceneTreeModelQT.h"
#include "ObjectInsertionQT.h"
#include "DeselectableTreeView.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			SceneTreeQT::SceneTreeQT( QWidget* parent )
				: QWidget( parent )
			{
				QVBoxLayout* layout = new QVBoxLayout( this );

				m_searchField = new QLineEdit();
				m_searchField->setPlaceholderText( tr( "Search" ) );
				m_searchField->setClearButtonEnabled( true );
				connect( m_searchField, SIGNAL( textChanged( const QString & ) ), this, SLOT( OnSearch( const QString & ) ) );

				m_stack = new QStackedWidget();

				m_tree = new DeselectableTreeView( this );
				m_tree->setContextMenuPolicy( Qt::CustomContextMenu );
				connect( m_tree, SIGNAL( customContextMenuRequested( const QPoint & ) ), this, SLOT( OnCustomContextMenu( const QPoint & ) ) );
				m_tree->setHeaderHidden( true );
				m_model = new SceneTreeModelQT();
				m_tree->setModel( m_model );
				m_tree->setSelectionMode( QAbstractItemView::ExtendedSelection );

				m_tree->setDragEnabled( true );
				m_tree->setAcceptDrops( true );
				m_tree->setDropIndicatorShown( true );
				m_tree->setAnimated( true );
				m_tree->setAutoExpandDelay( 300 );
				m_tree->setDragDropMode( QAbstractItemView::InternalMove );
				m_tree->setUniformRowHeights( true );

				connect( m_tree->selectionModel(), SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), this,
				         SLOT( TreeSelectionChanged( const QModelIndex & , const QModelIndex & ) ) );

				m_list = new QListWidget();
				m_list->setUniformItemSizes( true );

				connect( m_list, SIGNAL( itemSelectionChanged() ), this,
				         SLOT( ListSelectionChanged() ) );

				layout->addWidget( m_searchField );
				m_stack->addWidget( m_tree );
				m_stack->addWidget( m_list );
				layout->addWidget( m_stack );

				setLayout( layout );
			}

			SceneTreeQT::~SceneTreeQT()
			{
			}

			void SceneTreeQT::TreeSelectionChanged( const QModelIndex& current, const QModelIndex& previous )
			{
				SetSelectedSceneObject( static_cast<Engine::TCMSceneObject*>(current.internalPointer()) );
			}

			void SceneTreeQT::ListSelectionChanged()
			{
				SetSelectedSceneObject( m_result[m_list->currentRow()] );
			}

			void SceneTreeQT::OnSearch( const QString& text )
			{
				if ( text.size() == 0 )
				{
					m_stack->setCurrentIndex( 0 );
					m_result.clear();
					m_list->clear();
				}
				else
				{
					if ( m_stack->currentIndex() == 0 )
						m_stack->setCurrentIndex( 1 );

					m_result = FindSceneObjects( text.toStdString() );
					m_list->clear();
					for ( Engine::TCMSceneObject* object : m_result )
					{
						m_list->addItem( new QListWidgetItem( QString::fromStdString( object->GetName() ) ) );
					}
				}
			}

			void SceneTreeQT::AddObject( QString name )
			{
				QModelIndex index = m_tree->selectionModel()->currentIndex();
				QAbstractItemModel* model = m_tree->model();
				model->insertRow( 0, index );
			}

			void SceneTreeQT::SetScene( Engine::TCMScene* scene )
			{
				m_model->SetRootObject( scene->GetRootSceneObject() );
				m_tree->clearSelection();
			}

			void SceneTreeQT::EnableUI()
			{
				setEnabled( true );
			}

			void SceneTreeQT::DisableUI()
			{
				setEnabled( false );
			}

			void SceneTreeQT::RefreshUI()
			{
				m_model->layoutChanged();
			}

			void SceneTreeQT::OnCustomContextMenu( const QPoint& )
			{
				ObjectInsertionQT* creaMenu = new ObjectInsertionQT( m_tree->currentIndex(), m_model, this );
				creaMenu->exec( QCursor::pos() );
				RefreshUI();
				//m_tree->dataChanged( m_tree->currentIndex(), m_tree->currentIndex() );
			}
		}
	}
}
