#pragma once

#include "TCMSaveManager/BaseData.h"

#include "EngineClasses/Project.h"

namespace TCM
{
	namespace Editor
	{
		namespace Data
		{
			class SceneData;

			/**
			 * \brief Base class for serializing project data
			 */
			class TCMSAVEMANAGER_DLL ProjectData : public BaseData
			{
			public:
				ProjectData();

				explicit ProjectData( const Engine::Project* project );

				virtual ~ProjectData();

				/**
				 * \brief Extracts serialized data to an instance of \c Project
				 * \return A new \c Project that is of type stored by \c BaseData
				 */
				Engine::Project* Extract() const;

				/**
				 * \brief Get the filename of this \c ProjectData
				 * \return The filename of this \c ProjectData
				 */
				std::string GetFilename() const;

				/**
				 * \brief Set the filename of this \c ProjectData
				 * \param filename _IN_ The new filename to be set
				 */
				void SetFilename( const std::string& filename );

				/**
				 * \brief Get every \c SceneData contained by this \c ProjectData
				 * \return Const pointer to the vector containing every \c SceneData of this \c ProjectData
				 */
				const std::vector<SceneData *>* _GetScenes() const;

				/**
				 * \brief Add a \c SceneData to this \c ProjectData's scenes
				 * \param sceneData _IN_ The \c SceneData to be added
				 */
				void _AddScene( SceneData* sceneData ) const;

			protected:
				/**
				 * \brief The filename to be used if saving in a file
				 */
				std::string* m_filename;

				/**
				 * \brief Vector containing every \c SceneData of this \c ProjectData
				 */
				std::vector<SceneData *>* m_scenes;
			};
		}
	}
}
