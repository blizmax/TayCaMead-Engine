#pragma once

#include <QTreeView>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class DeselectableTreeView : public QTreeView
			{
				Q_OBJECT

			public:
				DeselectableTreeView( QWidget* parent = 0 );
				~DeselectableTreeView();
			protected:
				void mousePressEvent( QMouseEvent* event ) override;

				bool eventFilter( QObject* obj, QEvent* event );
			};
		}
	}
}
