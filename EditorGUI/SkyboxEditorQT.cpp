#include "SkyboxEditorQT.h"
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <complex.h>
#include <QFileDialog>
#include <QFormLayout>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			SkyboxEditorQT::SkyboxEditorQT( QWidget* parent )
				: QDialog( parent )
			{
				QStringList list;
				list.append( tr( "Right" ) + "(+x)" );
				list.append( tr( "Left" ) + "(-x)" );
				list.append( tr( "Top" ) + "(+y)" );
				list.append( tr( "Bottom" ) + "(-y)" );
				list.append( tr( "Back" ) + "(+z)" );
				list.append( tr( "Front" ) + "(-z)" );

				QVBoxLayout* mainLayout = new QVBoxLayout( this );
				QFormLayout* formLayout = new QFormLayout;
				for ( int i = 0; i < 6; ++i )
				{
					formLayout->addRow( list[i], GenerateLine( i ) );
				}
				QDialogButtonBox* buttons = new QDialogButtonBox;

				buttons->addButton( QDialogButtonBox::Ok );
				buttons->addButton( QDialogButtonBox::Cancel );
				connect( buttons->button( QDialogButtonBox::Ok ), SIGNAL( clicked() ), this, SLOT( accept() ) );
				connect( buttons->button( QDialogButtonBox::Cancel ), SIGNAL( clicked() ), this, SLOT( reject() ) );

				mainLayout->addLayout( formLayout );
				mainLayout->addWidget( buttons );
				ShowSkyfaces();

				setLayout( mainLayout );
			}

			QHBoxLayout* SkyboxEditorQT::GenerateLine( int index )
			{
				QHBoxLayout* line = new QHBoxLayout();

				QPushButton* button = new QPushButton( tr( "Edit" ) );
				button->setObjectName( QString::number( index ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( OnSetClicked() ) );

				QLabel* label = new QLabel();
				label->setText( tr( "Empty" ) );

				line->addWidget( label );
				line->addWidget( button );

				skyboxLabels.append( label );

				return line;
			}

			void SkyboxEditorQT::accept()
			{
				Apply();
				QDialog::accept();
			}

			void SkyboxEditorQT::OnSetClicked()
			{
				int index = sender()->objectName().toInt();
				QString fileName = QFileDialog::getOpenFileName( this,
				                                                 tr( "Open Image" ), QString(), tr( "Image Files (*.png *.jpg *.bmp)" ) );
				if ( fileName.isEmpty() )
					return;
				SetSkyface( index, fileName.toStdString().c_str() );

				ShowSkyface( index );
			}

			void SkyboxEditorQT::ShowSkyface( int index )
			{
				QString skyface = QString::fromStdString( skyfaces[index] );
				QImage img;
				img.load( skyface );
				skyboxLabels[index]->setText( skyface );
			}
		}
	}
}
