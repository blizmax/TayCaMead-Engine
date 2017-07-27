#include "ObjectInsertionQT.h"
#include <QAbstractItemModel>
#include "EngineClasses/TCMScene.h"
#include "MainWindow.h"
#include <QFileDialog>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include "TCMSaveManager/SceneObjectData.h"
#include "TCMSaveManager/BinarySerialization.hpp"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			ObjectInsertionQT::ObjectInsertionQT( const QModelIndex& modelIndex, QAbstractItemModel* model, QWidget* parent )
				: QMenu( parent )
				, ObjectInsertion( static_cast<Engine::TCMSceneObject*>(modelIndex.internalPointer()) )
				, m_modelIndex( modelIndex )
				, m_model( model )
			{
				const QClipboard* clipboard = QApplication::clipboard();
				const QMimeData* mimeData = clipboard->mimeData();

				QAction* action;

				action = addAction( QIcon::fromTheme( QString::fromStdString( MainWindow::GetIconPath( IconType::CUT ) ) ), tr( "Cut" ), this, &ObjectInsertionQT::CutQT );
				if ( modelIndex.internalPointer() == nullptr )
					action->setEnabled( false );

				action = addAction( QIcon::fromTheme( QString::fromStdString( MainWindow::GetIconPath( IconType::COPY ) ) ), tr( "Copy" ), this, &ObjectInsertionQT::CopyQT );
				if ( modelIndex.internalPointer() == nullptr )
					action->setEnabled( false );

				action = addAction( QIcon::fromTheme( QString::fromStdString( MainWindow::GetIconPath( IconType::PASTE ) ) ), tr( "Paste" ), this, &ObjectInsertionQT::PasteQT );
				if ( !mimeData->hasFormat( TCM_SCOBJ_MIME_DATA ) )
					action->setEnabled( false );

				addSeparator();

				action = addAction( tr( "Duplicate" ), this, &ObjectInsertionQT::DupplicateQT );
				if ( modelIndex.internalPointer() == nullptr )
					action->setEnabled( false );
				action = addAction( tr( "Delete" ), this, &ObjectInsertionQT::DeleteObjectQT );
				if ( modelIndex.internalPointer() == nullptr )
					action->setEnabled( false );

				addSeparator();

				addAction( tr( "Create Empty" ), this, &ObjectInsertionQT::CreateEmptyObjectQT );

				QMenu* lightMenu = addMenu( tr( "Light" ) );
				lightMenu->addAction( tr( "Directionnal Light" ), this, &ObjectInsertionQT::CreateDirectionnalLightQT );
				lightMenu->addAction( tr( "Point Light" ), this, &ObjectInsertionQT::CreatePointLightQT );
				lightMenu->addAction( tr( "Spot Light" ), this, &ObjectInsertionQT::CreateSpotLightQT );

				QMenu* volumeMenu = addMenu( tr( "3D objects" ) );
				volumeMenu->addAction( tr( "Mesh" ), this, &ObjectInsertionQT::CreateMeshQT );
				volumeMenu->addAction( tr( "Sphere" ), this, &ObjectInsertionQT::CreateSphereQT );
				volumeMenu->addAction( tr( "Cube" ), this, &ObjectInsertionQT::CreateCubeQT );
				volumeMenu->addAction( tr( "Capsule" ), this, &ObjectInsertionQT::CreateCapsuleQT );
				volumeMenu->addAction( tr( "Plane" ), this, &ObjectInsertionQT::CreatePlaneQT );
				volumeMenu->addAction( tr( "Tube" ), this, &ObjectInsertionQT::CreateTubeQT );
				volumeMenu->addAction( tr( "Cone" ), this, &ObjectInsertionQT::CreateConeQT );

				QMenu* audioMenu = addMenu( tr( "Audio" ) );
				audioMenu->addAction( tr( "Music Source" ), this, &ObjectInsertionQT::CreateMusicSourceQT );
				audioMenu->addAction( tr( "Sound Source" ), this, &ObjectInsertionQT::CreateSoundSourceQT );

				addAction( tr( "Camera" ), this, &ObjectInsertionQT::CreateCameraQT );

				addSeparator();

				addAction( tr( "Load object" ), this, &ObjectInsertionQT::CreatePrefabQT );
				action = addAction( tr( "Save to file" ), this, &ObjectInsertionQT::SavePrefabQT );
				if ( modelIndex.internalPointer() == nullptr )
					action->setEnabled( false );
			}

			void ObjectInsertionQT::CutQT()
			{
				CopyToClipboard();
				DeleteObject();
			}

			void ObjectInsertionQT::CopyQT()
			{
				CopyToClipboard();
			}

			void ObjectInsertionQT::PasteQT()
			{
				const QClipboard* clipboard = QApplication::clipboard();
				const QMimeData* mimeData = clipboard->mimeData();

				QByteArray encodedData = mimeData->data( TCM_SCOBJ_MIME_DATA );
				QDataStream stream( &encodedData, QIODevice::ReadOnly );

				uint32_t len;
				char* text;
				stream.readBytes( text, len );
				std::string objectBytes( text, len );

				Data::SceneObjectData* data = BinarySerialization::Reader::ReadSceneObjectData( objectBytes );

				Paste( data );
				delete data;
			}

			void ObjectInsertionQT::DupplicateQT()
			{
				Duplicate();
			}

			void ObjectInsertionQT::CreateEmptyObjectQT()
			{
				CreateEmptyObject();
			}

			void ObjectInsertionQT::CreateDirectionnalLightQT()
			{
				CreateDirectionnalLight();
			}

			void ObjectInsertionQT::CreateSpotLightQT()
			{
				CreateSpotLight();
			}

			void ObjectInsertionQT::CreatePointLightQT()
			{
				CreatePointLight();
			}

			void ObjectInsertionQT::CreateMeshQT()
			{
				QString fileName
						= QFileDialog::getOpenFileName( this, tr( "Load mesh" ), QString(), tr( "Mesh (*.fbx *.obj)" ) );
				if ( fileName.isEmpty() )
					return;
				CreateMesh( fileName.toStdString() );
			}

			void ObjectInsertionQT::CreateSphereQT()
			{
				CreateSphere();
			}

			void ObjectInsertionQT::CreateCubeQT()
			{
				CreateCube();
			}

			void ObjectInsertionQT::CreatePlaneQT()
			{
				CreatePlane();
			}

			void ObjectInsertionQT::CreateConeQT()
			{
				CreateCone();
			}

			void ObjectInsertionQT::CreateTubeQT()
			{
				CreateTube();
			}

			void ObjectInsertionQT::CreateCapsuleQT()
			{
				CreateCapsule();
			}

			void ObjectInsertionQT::CreateSoundSourceQT()
			{
				CreateSoundSource();
			}

			void ObjectInsertionQT::CreateMusicSourceQT()
			{
				CreateMusicSource();
			}

			void ObjectInsertionQT::CreateCameraQT()
			{
				CreateCamera();
			}

			void ObjectInsertionQT::DeleteObjectQT()
			{
				DeleteObject();
			}

			void ObjectInsertionQT::CreatePrefabQT()
			{
				QString fileName
						= QFileDialog::getOpenFileName( this, tr( "Load scene object" ), QString(), tr( "TCM Scene Object (*.tcmscobj)" ) );
				if ( fileName.isEmpty() )
					return;

				CreatePrefab( fileName.toStdString() );
			}

			void ObjectInsertionQT::SavePrefabQT()
			{
				QString fileName
						= QFileDialog::getSaveFileName( this, tr( "Save scene object" ), QString(), tr( "TCM Scene Object (*.tcmscobj)" ) );
				if ( fileName.isEmpty() )
					return;

				SavePrefab( fileName.toStdString() );
			}

			void ObjectInsertionQT::Dummy()
			{
			}

			void ObjectInsertionQT::CopyToClipboard()
			{
				QMimeData* mimeData = new QMimeData();
				QByteArray encodedData;

				QDataStream stream( &encodedData, QIODevice::WriteOnly );

				Data::SceneObjectData data( m_parent, nullptr );
				std::string string = BinarySerialization::Writer::GetBinarySceneObjectData( &data );
				stream.writeBytes( string.c_str(), string.size() );

				mimeData->setData( TCM_SCOBJ_MIME_DATA, encodedData );

				QClipboard* clipboard = QApplication::clipboard();
				clipboard->setMimeData( mimeData );
			}
		}
	}
}
