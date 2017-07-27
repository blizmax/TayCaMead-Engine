#pragma once

#include "SceneTree.h"
#include <QWidget>

QT_FORWARD_DECLARE_CLASS( QLineEdit )
QT_FORWARD_DECLARE_CLASS( QTreeView )
QT_FORWARD_DECLARE_CLASS( QTreeWidget )
QT_FORWARD_DECLARE_CLASS( QListWidget )
QT_FORWARD_DECLARE_CLASS( QStackedWidget )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class SceneTreeModelQT;

			class SceneTreeQT : public QWidget, public SceneTree
			{
				Q_OBJECT
			public:
				explicit SceneTreeQT( QWidget* parent = nullptr );
				~SceneTreeQT();

				void AddObject( QString name );

				void SetScene( Engine::TCMScene* scene ) override;
				void EnableUI() override;
				void DisableUI() override;
				void RefreshUI() override;

			private
			slots:

				void OnCustomContextMenu( const QPoint& );
				void TreeSelectionChanged( const QModelIndex& current, const QModelIndex& previous );
				void ListSelectionChanged();
				void OnSearch( const QString& text );

			protected:
				QLineEdit* m_searchField;
				QTreeView* m_tree;
				QListWidget* m_list;
				QStackedWidget* m_stack;

				SceneTreeModelQT* m_model;
				std::vector<Engine::TCMSceneObject*> m_result;
			};
		}
	}
}
