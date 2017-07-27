#pragma once

#include <map>
#include <string>
#include <cstdint>

#include "TCMSaveManager/SaveManagerDefines.h"

namespace TCM
{
	struct SerializedInterface;

	namespace Editor
	{
#pragma region Forward Declaration
		namespace Data
		{
			class BaseData;
			class ComponentData;
			class SceneObjectData;
			class SceneData;
			class ProjectData;
		}
#pragma endregion

		namespace BinarySerialization
		{
			/**
			 * \brief Values of prefix used when serializing in a binary file
			 * \note Values should always be END_VALUE = VALUE + 1
			 */
			enum class EPrefix : uint8_t
			{
				// Values should ALWAYS be:
				// VALUE
				// END_VALUE
				// Whatever VALUE is, END_VALUE should be VALUE + 1

				// Main Data
				PROJECT = 0x01,
				END_PROJECT,

				SCENE,
				END_SCENE,

				SCENEOBJECT,
				END_SCENEOBJECT,

				COMPONENT,
				END_COMPONENT,

				ATTRIBUTES,
				END_ATTRIBUTES,

				// Particular Data
				SKYBOX,
				END_SKYBOX,

				ONEATTRIB,
				END_ONEATTRIB,

				// Error
				BADPREFIX = 0xFF
			};

			class Writer
			{
			public:
				/**
				 * \brief Save a \c ProjectData to a file
				 * \param projectData _IN_ The \c ProjectData to be saved
				 * \return True if everything went good, false otherwise
				 */
				static TCMSAVEMANAGER_DLL bool SaveProjectData( const Data::ProjectData* projectData );

				/**
				 * \brief Save a \c SceneData to a file
				 * \param sceneData _IN_ The \c SceneData to be saved
				 * \return True if everything went good, false otherwise
				 */
				static TCMSAVEMANAGER_DLL bool SaveSceneData( const Data::SceneData* sceneData );

				/**
				 * \brief Save a \c SceneObjectData to a file
				 * \param sceneObjectData _IN_ The \c SceneObjectData to be saved
				 * \param sceneObjectFilename _IN_ The file the data will be stored in
				 * \return True if everything went good, false otherwise
				 */
				static TCMSAVEMANAGER_DLL bool SaveSceneObjectData( const Data::SceneObjectData* sceneObjectData, const std::string& sceneObjectFilename );

				/**
				* \brief Save a \c SceneObjectData to a string
				* \param sceneObjectData _IN_ The \c SceneObjectData to be saved
				* \return The binary of the SceneObjectData
				*/
				static TCMSAVEMANAGER_DLL std::string GetBinarySceneObjectData( const Data::SceneObjectData* sceneObjectData );

			private:
				static bool _SaveSceneDataToStream( const Data::SceneData* sceneData, std::ostream& projectStream );
				static bool _SaveSceneObjectToStream( const Data::SceneObjectData* sceneObjectData, std::ostream& sceneStream );
				static bool _SaveComponentToStream( const Data::ComponentData* componentData, std::ostream& sceneStream );
				static bool _SaveAttributesToStream( const Data::BaseData* baseData, std::ostream& sceneStream );

				static void _WritePrefixToFile( const EPrefix prefix, std::ostream& fileStream );
				static void _WriteToFile( const void* data, const size_t& size, std::ostream& fileStream );
			};

			class Reader
			{
			public:
				/**
				 * \brief Loads a \c ProjectData from a \p path
				 * \param path _IN_ Path from which the \c ProjectData will be loaded
				 * \return The loaded \c ProjectData
				 */
				static TCMSAVEMANAGER_DLL Data::ProjectData* LoadProjectData( const std::string& path );

				/**
				 * \brief Loads a \c SceneData from a \p path
				 * \param path _IN_ Path from which the \c SceneData will be loaded
				 * \return The loaded \c SceneData
				 */
				static TCMSAVEMANAGER_DLL Data::SceneData* LoadSceneData( const std::string& path );

				/**
				 * \brief Loads a \c SceneData from a \p path
				 * \param path _IN_ Path from which the \c SceneObjectData will be loaded
				 * \return The loaded \c SceneObjectData
				 */
				static TCMSAVEMANAGER_DLL Data::SceneObjectData* LoadSceneObjectData( const std::string& path );

				/**
				* \brief Loads a \c SceneData from a \p binary string
				* \param string _IN_ String from which the \c SceneObjectData will be loaded
				* \return The loaded \c SceneObjectData
				*/
				static TCMSAVEMANAGER_DLL Data::SceneObjectData* ReadSceneObjectData( const std::string& string );

			private:
				static Data::SceneData* _LoadSceneDataFromFile( std::istream& projectIOStream );
				static Data::SceneObjectData* _LoadSceneObjectFromFile( std::istream& sceneIOStream );
				static Data::ComponentData* _LoadComponentFromFile( std::istream& sceneIOStream );
				static bool _LoadAttributesFromFile( Data::BaseData* baseData, std::istream& stream );
				static SerializedInterface* _LoadOneAttributeFromFile( const std::map<const std::string, std::string>* mapClassMembers, std::istream& stream );

				static bool _ReadPrefixFromFile( EPrefix& prefix, std::istream& fileIO );
				static bool _ReadFromFile( void* data, const size_t& size, std::istream& fileIO );
			};
		};
	}
}
