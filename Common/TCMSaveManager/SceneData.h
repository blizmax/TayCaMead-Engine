#pragma once

#include "TCMSaveManager/BaseData.h"

#include "EngineClasses/TCMScene.h"

namespace TCM
{
	namespace Editor
	{
		namespace Data
		{
			class SceneObjectData;
			class ProjectData;

			/**
			 * \brief Base class for serializing scene data
			 */
			class TCMSAVEMANAGER_DLL SceneData : public BaseData
			{
			public:
				SceneData();

				explicit SceneData( Engine::TCMScene* scene, ProjectData* project = nullptr );

				virtual ~SceneData();

				/**
				 * \brief Extracts serialized data to an instance of \c TCMScene
				 * \return A new \c TCMScene that is of type stored by \c BaseData
				 */
				Engine::TCMScene* Extract() const;

				/**
				 * \brief Add a \c SceneObjectData to root scene object list
				 * \param sceneObjectData _IN_ The \c SceneObjectData to be set as a root scene object
				 */
				void _AddRootSceneObject( SceneObjectData* sceneObjectData );

				/**
				 * \brief Get the filename of this \c SceneData
				 * \return The filename of this \c SceneData
				 */
				std::string GetFilename() const;

				/**
				 * \brief Set the filename of this \c SaveManager
				 * \param filename _IN_ The new filename to be set
				 */
				void SetFilename( const std::string& filename );

				/**
				 * \brief Get every skyface contained by this \c SceneData
				 * \return Const pointer to the vector containing every skyface of this \c SceneData
				 */
				const std::vector<std::string>* _GetSkyfaces() const;

				/**
				 * \brief Get skyface contained by this \c SceneData at position \p index
				 * \return Const pointer to the vector containing every skyface of this \c SceneData
				 * \note \p index must be between 0 and 6
				 */
				std::string& _GetSkyfaceAt( const size_t& index ) const;

				/**
				 * \brief Get every \c SceneObjectData contained by this \c SceneData
				 * \return Const pointer to the vector containing every \c SceneObjectData of this \c SceneData
				 */
				const std::vector<SceneObjectData *>* _GetRootSceneObjects() const;

			protected:
				/**
				 * \brief The filename to be used if saving in a file
				 */
				std::string* m_filename = nullptr;

				/**
				 * \brief The \c ProjectData containing the \c SceneData
				 */
				ProjectData* m_project = nullptr;

				/**
				 * \brief The skyfaces paths for this \c SceneData
				 * \note Size will always be equal to 6
				 */
				std::vector<std::string>* m_skyfaces = nullptr;

				/**
				 * \brief Vector of every root \c SceneObjectData of this \c SceneData
				 */
				std::vector<SceneObjectData *>* m_rootSceneObjects = nullptr;
			};
		}
	}
}
