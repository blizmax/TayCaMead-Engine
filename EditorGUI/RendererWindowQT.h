#pragma once

#include <QWindow>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class RendererWindowQT : public QWindow
			{
				Q_OBJECT

			public:
				RendererWindowQT();
			protected:
				void focusInEvent( QFocusEvent* event ) override;
				void focusOutEvent( QFocusEvent* event ) override;
				void resizeEvent( QResizeEvent* event ) override;
			};
		}
	}
}
