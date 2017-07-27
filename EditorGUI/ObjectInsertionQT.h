#pragma once

#include <QMenu>
#include "ObjectInsertion.h"

class QAbstractItemModel;

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class ObjectInsertionQT : public QMenu, public ObjectInsertion
			{
				Q_OBJECT

			public:
				ObjectInsertionQT( const QModelIndex& modelIndex, QAbstractItemModel* model, QWidget* parent = nullptr );
				~ObjectInsertionQT() = default;

			protected slots:
				void CutQT();
				void CopyQT();
				void PasteQT();

				void DupplicateQT();

				void CreateEmptyObjectQT();

				void CreateDirectionnalLightQT();
				void CreateSpotLightQT();
				void CreatePointLightQT();

				void CreateMeshQT();
				void CreateSphereQT();
				void CreateCubeQT();
				void CreatePlaneQT();
				void CreateConeQT();
				void CreateTubeQT();
				void CreateCapsuleQT();
				void CreateSoundSourceQT();
				void CreateMusicSourceQT();

				void CreateCameraQT();
				void DeleteObjectQT();

				void CreatePrefabQT();
				void SavePrefabQT();

				void Dummy();

			protected:

				void CopyToClipboard();

				const QModelIndex& m_modelIndex;
				QAbstractItemModel* m_model;
			};
		}
	}
}
