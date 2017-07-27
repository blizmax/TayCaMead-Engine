#pragma once

#include <QWidget>
#include "FileManager.h"

QT_FORWARD_DECLARE_CLASS( QLineEdit )
QT_FORWARD_DECLARE_CLASS( QTreeView )
QT_FORWARD_DECLARE_CLASS( QListView )
QT_FORWARD_DECLARE_CLASS( QFileSystemModel )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class FileExplorerQT : public QWidget, public FileManager
			{
				Q_OBJECT

			public:
				FileExplorerQT( QWidget* parent = nullptr );
				~FileExplorerQT();

				void AddTopItem( QString str ) const;
				void AddItem( int index, QString str ) const;
				void SetDirectory( QString path ) const;

			private slots:
				void CreateClicked() const;
				void OnTreeViewClicked( const QModelIndex& index ) const;

			private:
				void Refresh( Engine::Project* project, Engine::TCMScene* scene ) override;
				void ShowFiles() override;
				void ShowFolders() override;
				QLineEdit* m_SearchField;
				QTreeView* m_Tree;
				QListView* m_List;

				// Make two models instead of one
				// to filter them separately
				QFileSystemModel* dirModel;
				QFileSystemModel* fileModel;
			};
		}
	}
}
