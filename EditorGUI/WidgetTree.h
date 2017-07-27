#pragma once

#include <QWidget>
#include <qtreewidgetitemiterator.h>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class MainWindowQT;
		}
	}
}

QT_FORWARD_DECLARE_CLASS( QTreeWidget )

class WidgetTree : public QWidget
{
	Q_OBJECT

public:
	explicit WidgetTree( TCM::Editor::QT::MainWindowQT* window, QWidget* parent = nullptr );
	~WidgetTree();

private slots:
	void dumpAllObjectTrees();

private:
	void dumpRecursive( const QObjectList list, QTreeWidgetItem* parent ) const;

	QTreeWidget* tree;
	TCM::Editor::QT::MainWindowQT* m_window;
};
