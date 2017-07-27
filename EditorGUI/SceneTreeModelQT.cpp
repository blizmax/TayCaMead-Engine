#include "SceneTreeModelQT.h"
#include "EngineClasses/TCMSceneObject.h"
#include <qiodevice.h>
#include <QMimeData>
#include <qdatastream.h>
#include "TCMSaveManager/SceneObjectData.h"
#include "TCMSaveManager/BinarySerialization.hpp"
#include "MainWindow.h"
#include "ObjectInsertCommand.h"
#include "EditorCommandManager.h"
#include "ObjectDeleteCommand.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			SceneTreeModelQT::SceneTreeModelQT( Engine::TCMSceneObject* object, QObject* parent )
				: QAbstractItemModel( parent )
			{
				rootItem = object;
			}

			SceneTreeModelQT::SceneTreeModelQT( QObject* parent )
				: QAbstractItemModel( parent )
			{
				rootItem = Engine::TCMSceneObject::Create();
			}

			int SceneTreeModelQT::columnCount( const QModelIndex& /* parent */ ) const
			{
				return 1;
			}

			QVariant SceneTreeModelQT::data( const QModelIndex& index, int role ) const
			{
				if ( !index.isValid() )
					return QVariant();

				if ( role != Qt::DisplayRole && role != Qt::EditRole )
					return QVariant();

				int column = index.column();
				if ( column )
					return QVariant();

				Engine::TCMSceneObject* item = getItem( index );
				return QVariant( QString::fromStdString( item->GetName() ) );
			}

			Qt::ItemFlags SceneTreeModelQT::flags( const QModelIndex& index ) const
			{
				Qt::ItemFlags defaultFlags = Qt::ItemIsEditable | QAbstractItemModel::flags( index );

				if ( index.isValid() )
					return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
				else
					return Qt::ItemIsDropEnabled | defaultFlags;
			}

			QStringList SceneTreeModelQT::mimeTypes() const
			{
				QStringList types;
				types << TCM_SCOBJ_MIME_DATA;
				return types;
			}

			QMimeData* SceneTreeModelQT::mimeData( const QModelIndexList& indexes ) const
			{
				QMimeData* mimeData = new QMimeData();
				QByteArray encodedData;

				std::string filepath = ".tcmtmp";
				QDataStream stream( &encodedData, QIODevice::WriteOnly );

				if ( indexes.size() > 0 )
				{
					QModelIndex index = indexes.at( 0 );
					if ( index.isValid() )
					{
						Data::SceneObjectData data( getItem( index ), nullptr );
						std::string str = BinarySerialization::Writer::GetBinarySceneObjectData( &data );
						stream.writeBytes( str.c_str(), str.size() );
					}
				}

				mimeData->setData( TCM_SCOBJ_MIME_DATA, encodedData );
				return mimeData;
			}

			bool SceneTreeModelQT::canDropMimeData( const QMimeData* data,
			                                        Qt::DropAction action, int row, int column, const QModelIndex& parent ) const
			{
				Q_UNUSED( action );
				Q_UNUSED( row );
				Q_UNUSED( parent );

				if ( !data->hasFormat( TCM_SCOBJ_MIME_DATA ) )
					return false;

				if ( column > 0 )
					return false;

				return true;
			}

			bool SceneTreeModelQT::dropMimeData( const QMimeData* data,
			                                     Qt::DropAction action, int row, int column, const QModelIndex& parent )
			{
				if ( !canDropMimeData( data, action, row, column, parent ) )
					return false;

				if ( action == Qt::IgnoreAction )
					return true;

				QByteArray encodedData = data->data( TCM_SCOBJ_MIME_DATA );
				QDataStream stream( &encodedData, QIODevice::ReadOnly );

				uint32_t len;
				char* text;

				stream.readBytes( text, len );
				int beginRow = row > -1 ? row : 0;

				insertRows( beginRow, 1, parent );

				QModelIndex idx = index( beginRow, 0, parent );

				setData( idx, QByteArray( text, len ) );

				return true;
			}

			Engine::TCMSceneObject* SceneTreeModelQT::getItem( const QModelIndex& index ) const
			{
				if ( index.isValid() )
				{
					Engine::TCMSceneObject* item = static_cast<Engine::TCMSceneObject*>(index.internalPointer());
					if ( item )
						return item;
				}
				return rootItem;
			}

			QVariant SceneTreeModelQT::headerData( int section, Qt::Orientation orientation,
			                                       int role ) const
			{
				if ( orientation == Qt::Horizontal && role == Qt::DisplayRole && !section )
					return QVariant( QString::fromStdString( rootItem->GetName() ) );

				return QVariant();
			}

			QModelIndex SceneTreeModelQT::index( int row, int column, const QModelIndex& parent ) const
			{
				if ( parent.isValid() && parent.column() != 0 )
					return QModelIndex();

				Engine::TCMSceneObject* parentItem = getItem( parent );

				Engine::TCMSceneObject* childItem = parentItem->GetChild( row );
				if ( childItem )
					return createIndex( row, column, childItem );
				else
					return QModelIndex();
			}

			bool SceneTreeModelQT::insertRows( int position, int rows, const QModelIndex& parent )
			{
				Engine::TCMSceneObject* parentItem = getItem( parent );

				beginInsertRows( parent, position, position + rows - 1 );
				for ( int i = 0; i < rows; ++i )
				{
					Engine::TCMSceneObject* newChild = Engine::TCMSceneObject::Create();
					EditorCommandManager::AddCommand( new ObjectInsertCommand( parentItem, newChild, position ) );
				}
				endInsertRows();

				return true;
			}

			QModelIndex SceneTreeModelQT::parent( const QModelIndex& index ) const
			{
				if ( !index.isValid() )
					return QModelIndex();

				Engine::TCMSceneObject* childItem = getItem( index );
				Engine::TCMSceneObject* parentItem = childItem->GetParent();

				if ( parentItem == rootItem || parentItem == nullptr )
					return QModelIndex();

				return createIndex( parentItem->GetChildCount(), 0, parentItem );
			}

			bool SceneTreeModelQT::removeRows( int position, int rows, const QModelIndex& parent )
			{
				Engine::TCMSceneObject* parentItem = getItem( parent );
				bool success = true;

				beginRemoveRows( parent, position, position + rows - 1 );
				for ( int i = 0; i < rows; ++i )
				{
					Engine::TCMSceneObject* item = parentItem->GetChild( position );
					if ( item == nullptr )
						success = false;
					else
						EditorCommandManager::AddCommand( new ObjectDeleteCommand( item ) );
				}
				endRemoveRows();

				return success;
			}

			bool SceneTreeModelQT::moveRows( const QModelIndex& sourceParent, int sourceRow, int count,
			                                 const QModelIndex& destinationParent, int destinationChild )
			{
				Engine::TCMSceneObject* source = getItem( sourceParent );
				if ( sourceRow < 0 || sourceRow + count > source->GetChildCount() )
					return false;

				Engine::TCMSceneObject* destination = getItem( destinationParent );
				if ( destinationChild < 0 || destinationChild >= destination->GetChildCount() )
					return false;

				beginMoveRows( sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild );
				for ( int i = 0; i < count; ++i )
				{
					Engine::TCMSceneObject* movingObj = source->GetChild( sourceRow );

					if ( movingObj == nullptr )
						continue;
					EditorCommandManager::AddCommand( new ObjectDeleteCommand( movingObj ) );
					EditorCommandManager::AddCommand( new ObjectInsertCommand( destination, movingObj ) );
				}
				endMoveRows();

				return true;
			}

			void SceneTreeModelQT::SetRootObject( Engine::TCMSceneObject* object )
			{
				if ( !object )
					return;
				beginResetModel();
				rootItem = object;
				endResetModel();
			}

			int SceneTreeModelQT::rowCount( const QModelIndex& parent ) const
			{
				Engine::TCMSceneObject* parentItem = getItem( parent );

				return parentItem->GetChildCount();
			}

			bool SceneTreeModelQT::setData( const QModelIndex& index, const QVariant& value, int role )
			{
				if ( role != Qt::EditRole )
					return false;
				int column = index.column();
				if ( column > 0 )
					return false;

				std::string string = value.toByteArray().toStdString();
				Data::SceneObjectData* data = BinarySerialization::Reader::ReadSceneObjectData( string );

				Engine::TCMSceneObject* item = getItem( index );

				if ( data == nullptr && !string.empty() )
				{
					item->SetName( string );
				} 
				else if ( data )
				{
					data->Fill( item );
					delete data;
				} else
				{
					return false;
				}

				emit dataChanged( index, index );

				return true;
			}

			Qt::DropActions SceneTreeModelQT::supportedDropActions() const
			{
				return Qt::CopyAction | Qt::MoveAction;
			}
		}
	}
}
