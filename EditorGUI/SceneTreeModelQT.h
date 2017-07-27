#pragma once

#include <QAbstractItemModel>

namespace TCM
{
	namespace Engine
	{
		class TCMSceneObject;
	}

	namespace Editor
	{
		namespace QT
		{
			class SceneTreeModelQT : public QAbstractItemModel
			{
				Q_OBJECT

			public:
				explicit SceneTreeModelQT( Engine::TCMSceneObject* object, QObject* parent = nullptr );
				explicit SceneTreeModelQT( QObject* parent = nullptr );

				QVariant data( const QModelIndex& index, int role ) const override;
				QVariant headerData( int section, Qt::Orientation orientation,
				                     int role = Qt::DisplayRole ) const override;

				QModelIndex index( int row, int column,
				                   const QModelIndex& parent = QModelIndex() ) const override;
				QModelIndex parent( const QModelIndex& index ) const override;

				int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
				int columnCount( const QModelIndex& parent = QModelIndex() ) const override;

				Qt::ItemFlags flags( const QModelIndex& index ) const override;

				QStringList mimeTypes() const override;
				QMimeData* mimeData( const QModelIndexList& indexes ) const override;
				bool canDropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent ) const override;
				bool dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent ) override;

				bool setData( const QModelIndex& index, const QVariant& value,
				              int role = Qt::EditRole ) override;
				Qt::DropActions supportedDropActions() const override;
				bool insertRows( int position, int rows,
				                 const QModelIndex& parent = QModelIndex() ) override;
				bool removeRows( int position, int rows,
				                 const QModelIndex& parent = QModelIndex() ) override;

				bool moveRows( const QModelIndex& sourceParent,
				               int sourceRow, int count,
				               const QModelIndex& destinationParent, int destinationChild ) override;

				void SetRootObject( Engine::TCMSceneObject* object );
			private:
				Engine::TCMSceneObject* getItem( const QModelIndex& index ) const;

				Engine::TCMSceneObject* rootItem;
			};
		}
	}
}
