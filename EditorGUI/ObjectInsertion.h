#pragma once
#include <string>

namespace TCM
{
	namespace Engine
	{
		class TCMScene;
		class TCMSceneObject;
	}

	namespace Editor
	{
		namespace Data
		{
			class SceneObjectData;
		}

		class ObjectInsertion
		{
		public:
			ObjectInsertion( Engine::TCMSceneObject* parent );
			virtual ~ObjectInsertion() = default;

			void Duplicate() const;
			void Paste( Data::SceneObjectData* data );
			void CreateEmptyObject() const;

			void CreateDirectionnalLight() const;
			void CreateSpotLight() const;
			void CreatePointLight() const;
			void CreateSphere() const;
			void CreateCube() const;
			void CreatePlane() const;
			void CreateCone() const;
			void CreateTube() const;
			void CreateCapsule() const;
			void CreateSoundSource() const;
			void CreateMusicSource() const;

			void CreateMesh( const std::string& filepath ) const;
			void CreateCamera() const;

			void CreatePrefab( const std::string& filepath ) const;
			void SavePrefab( const std::string& filepath ) const;

			void DeleteObject() const;

		protected:

			Engine::TCMSceneObject* m_parent = nullptr;
		};
	}
}
