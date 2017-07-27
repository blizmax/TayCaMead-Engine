#include "DeselectableTreeView.h"
#include <QMouseEvent>
#include <qapplication.h>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			DeselectableTreeView::DeselectableTreeView( QWidget* parent )
				: QTreeView( parent )
			{
				qApp->installEventFilter( this );
			}

			DeselectableTreeView::~DeselectableTreeView()
			{
			}

			void DeselectableTreeView::mousePressEvent( QMouseEvent* event )
			{
				QModelIndex item = indexAt( event->pos() );
				bool selected = selectionModel()->isSelected( indexAt( event->pos() ) );
				QTreeView::mousePressEvent( event );
				if ( ( item.row() == -1 && item.column() == -1 ) || selected )
				{
					clearSelection();
					const QModelIndex index;
					selectionModel()->setCurrentIndex( index, QItemSelectionModel::Select );
				}
			}

			bool DeselectableTreeView::eventFilter( QObject* obj, QEvent* event )
			{
				if ( obj == this && event->type() == QEvent::KeyPress ) {
					QKeyEvent* key = static_cast<QKeyEvent*>( event );
					if ( key->key() == Qt::Key_Delete ) {
						//Delete was pressed
						QModelIndex index = selectionModel()->currentIndex();
						if ( index != QModelIndex() )
						{
							model()->removeRow( index.row(), index.parent() );
							return true;
						}
					}
				}
				return false;
			}
		}
	}
}
