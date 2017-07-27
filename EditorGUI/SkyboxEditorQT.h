#pragma once

#include <QDialog>
#include "SkyboxEditor.h"

QT_FORWARD_DECLARE_CLASS( QHBoxLayout )
QT_FORWARD_DECLARE_CLASS( QLabel )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class SkyboxEditorQT : public QDialog, SkyboxEditor
			{
				Q_OBJECT

			public:
				explicit SkyboxEditorQT( QWidget* parent = nullptr );

			private:
				QHBoxLayout* GenerateLine( int index );
				QVector<QLabel *> skyboxLabels;

			private slots:

				void accept() override;
				void OnSetClicked();
				void ShowSkyface( int index ) override;
			};
		}
	}
}
