#include "TCMSaveManager/BinarySerialization.hpp"

#include "TCMSaveManager/SaveData.h"
#include <sstream>

namespace TCM
{
	namespace Editor
	{
		namespace BinarySerialization
		{
#pragma region Writer
			bool Writer::SaveProjectData( const Data::ProjectData* projectData )
			{
				std::string projectFilename = projectData->GetFilename();

				if ( projectFilename.empty() == true )
				{
					TCMWARNING( "BinarySerialization::SaveSceneData() - ProjectData's filename not initialized or empty" );
					return false;
				}

				std::ofstream projectIOFile( projectFilename, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc );
				if ( !projectIOFile.is_open() )
				{
					TCMWARNING( "BinarySerialization::SaveProjectData() - unable to open file [" + projectFilename + "]" );
					return false;
				}

				_WritePrefixToFile( EPrefix::PROJECT, projectIOFile );

#pragma region Save Scenes
				const std::vector<Data::SceneData *>* vecScene = projectData->_GetScenes();
				if ( vecScene == nullptr || vecScene->empty() == true )
				TCMINFO( "BinarySerialization::SaveProjectData() - project does not contain any SceneData" );
				else
				{
					// Save SceneData for every scenes contained in the project
					for ( std::vector<Data::SceneData *>::const_iterator iter = vecScene->begin(); iter != vecScene->end(); ++iter )
					{
						if ( *iter )
							_SaveSceneDataToStream( *iter, projectIOFile );
					}
				}
#pragma endregion

				_WritePrefixToFile( EPrefix::END_PROJECT, projectIOFile );

				projectIOFile.close();

				return true;
			}

			bool Writer::SaveSceneData( const Data::SceneData* sceneData )
			{
				std::string sceneFilename = sceneData->GetFilename();

				if ( sceneFilename.empty() == true )
				{
					TCMWARNING( "BinarySerialization::SaveSceneData() - SceneData's filename not initialized or empty" );
					return false;
				}

				// Format: PREFIX -> call function ; PREFIX -> call function
				std::ofstream sceneIOFile( sceneFilename, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc );
				if ( !sceneIOFile.is_open() )
				{
					TCMWARNING( "BinarySerialization::SaveSceneData() - unable to open file [" + sceneFilename + "]" );
					return false;
				}

				_WritePrefixToFile( EPrefix::SCENE, sceneIOFile );

#pragma region Save Skybox
				_WritePrefixToFile( EPrefix::SKYBOX, sceneIOFile );

				const std::vector<std::string>* skyfaces = sceneData->_GetSkyfaces();

				for ( size_t i = 0; i < 6; ++i )
				{
					const std::string& oneFace = skyfaces->at( i );

					// Get the length of the path and write it first
					size_t len = oneFace.length();
					_WriteToFile( &len, sizeof( size_t), sceneIOFile );

					// Write the path of size specified before
					_WriteToFile( oneFace.c_str(), len, sceneIOFile );
				}

				_WritePrefixToFile( EPrefix::END_SKYBOX, sceneIOFile );
#pragma endregion

#pragma region Save SceneObject
				const std::vector<Data::SceneObjectData *>* vecSceneObject = sceneData->_GetRootSceneObjects();
				if ( vecSceneObject == nullptr || vecSceneObject->empty() == true )
				TCMINFO( "BinarySerialization::SaveSceneData() - scene does not contain any SceneObjectData" );
				else
				{
					// For every SceneObjectData in the SceneData, save the SceneObjectData in the ACTUAL file
					for ( std::vector<Data::SceneObjectData *>::const_iterator iter = vecSceneObject->begin(); iter != vecSceneObject->end(); ++iter )
					{
						if ( *iter )
							_SaveSceneObjectToStream( *iter, sceneIOFile );
					}
				}
#pragma endregion

				// Save attributes being serialized
				_SaveAttributesToStream( sceneData, sceneIOFile );

				_WritePrefixToFile( EPrefix::END_SCENE, sceneIOFile );

				sceneIOFile.close();

				return true;
			}

			bool Writer::SaveSceneObjectData( const Data::SceneObjectData* sceneObjectData, const std::string& sceneObjectFilename )
			{
				std::ofstream sceneObjectIOFile( sceneObjectFilename, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc );
				if ( !sceneObjectIOFile.is_open() )
				{
					TCMWARNING( "BinarySerialization::SaveSceneData() - unable to open file [" + sceneObjectFilename + "]" );
					return false;
				}

				_SaveSceneObjectToStream( sceneObjectData, sceneObjectIOFile );

				sceneObjectIOFile.close();

				return true;
			}

			std::string Writer::GetBinarySceneObjectData( const Data::SceneObjectData* sceneObjectData )
			{
				std::ostringstream stream( std::ostringstream::out | std::ostringstream::binary );

				_SaveSceneObjectToStream( sceneObjectData, stream );

				return stream.str();
			}

			bool Writer::_SaveSceneDataToStream( const Data::SceneData* sceneData, std::ostream& projectStream )
			{
				std::string sceneFilename = sceneData->GetFilename();

				if ( sceneFilename.empty() == false )
				{
					_WritePrefixToFile( EPrefix::SCENE, projectStream );

					// Get the length of the path and write it first
					size_t len = sceneFilename.length();
					_WriteToFile( &len, sizeof( size_t), projectStream );
					// Write the path of size specified before
					_WriteToFile( sceneFilename.c_str(), len, projectStream );
					// Save the SceneData in another file
					SaveSceneData( sceneData );

					_WritePrefixToFile( EPrefix::END_SCENE, projectStream );
				}
				else
				{
					TCMWARNING( "BinarySerialization::_SaveSceneDataToFile() - SceneData's filename not initialized or empty" );
					return false;
				}
				return true;
			}

			bool Writer::_SaveSceneObjectToStream( const Data::SceneObjectData* sceneObjectData, std::ostream& sceneStream )
			{
				_WritePrefixToFile( EPrefix::SCENEOBJECT, sceneStream );

#pragma region Save Children SceneObject
				const std::vector<Data::SceneObjectData *>* vecChildren = sceneObjectData->_GetChildren();
				if ( vecChildren == nullptr || vecChildren->empty() == true )
				TCMINFO( "BinarySerialization::_SaveSceneObjectToFile() - scene does not contain any SceneObjectData" );
				else
				{
					// For every SceneObjectData child of this SceneObjectData, save the SceneObjectData in the ACTUAL file
					for ( std::vector<Data::SceneObjectData *>::const_iterator iter = vecChildren->begin(); iter != vecChildren->end(); ++iter )
					{
						if ( *iter )
							_SaveSceneObjectToStream( *iter, sceneStream );
					}
				}
#pragma endregion

#pragma region Save Components
				const std::vector<Data::ComponentData *>* vecComponent = sceneObjectData->_GetComponents();
				if ( vecComponent == nullptr || vecComponent->empty() == true )
				TCMINFO( "BinarySerialization::_SaveSceneObjectToFile() - scene does not contain any ComponentData" );
				else
				{
					// For every ComponentData of this SceneObjectData, save the ComponentData in the ACTUAL file
					for ( std::vector<Data::ComponentData *>::const_iterator iter = vecComponent->begin(); iter != vecComponent->end(); ++iter )
					{
						if ( *iter )
							_SaveComponentToStream( *iter, sceneStream );
					}
				}
#pragma endregion

				// Save attributes being serialized
				_SaveAttributesToStream( sceneObjectData, sceneStream );

				_WritePrefixToFile( EPrefix::END_SCENEOBJECT, sceneStream );

				return true;
			}

			bool Writer::_SaveComponentToStream( const Data::ComponentData* componentData, std::ostream& sceneStream )
			{
				_WritePrefixToFile( EPrefix::COMPONENT, sceneStream );

				// Save attributes being serialized
				_SaveAttributesToStream( componentData, sceneStream );

				_WritePrefixToFile( EPrefix::END_COMPONENT, sceneStream );

				return true;
			}

			bool Writer::_SaveAttributesToStream( const Data::BaseData* baseData, std::ostream& sceneStream )
			{
				_WritePrefixToFile( EPrefix::ATTRIBUTES, sceneStream );

				// Write MetaFactory len then name
				std::string classMetaName( baseData->GetClassMetaName() );
				size_t metaClasslen = classMetaName.length();
				_WriteToFile( &metaClasslen, sizeof( size_t), sceneStream );
				_WriteToFile( classMetaName.c_str(), metaClasslen, sceneStream );

				// Set serialized attributes
				const std::unordered_map<std::string, SerializedInterface *>* mapAttributes = baseData->_GetSerializedAttributes();
				for ( std::unordered_map<std::string, SerializedInterface *>::const_iterator iter = mapAttributes->cbegin(); iter != mapAttributes->cend(); ++iter )
				{
					const SerializedInterface* serializedInterface = iter->second;
					if ( serializedInterface == nullptr )
					TCMWARNING( "BinarySerialization::_SaveAttributesToFile() - serialized variable [" + iter->first + "] is nullptr" );
					else
					{
						_WritePrefixToFile( EPrefix::ONEATTRIB, sceneStream );

						std::string varName = serializedInterface->m_varName;

						size_t varLen = varName.length();
						// Write the length of the name
						_WriteToFile( &varLen, sizeof( size_t), sceneStream );
						// Write the name of length -> varLen
						_WriteToFile( varName.c_str(), varLen, sceneStream );
						// Write data
						serializedInterface->WriteToFile( sceneStream );

						_WritePrefixToFile( EPrefix::END_ONEATTRIB, sceneStream );
					}
				}

				_WritePrefixToFile( EPrefix::END_ATTRIBUTES, sceneStream );
				return true;
			}

			void Writer::_WritePrefixToFile( const EPrefix prefix, std::ostream& fileStream )
			{
				fileStream.write( reinterpret_cast<const char *>(&prefix), sizeof( EPrefix) );
			}

			void Writer::_WriteToFile( const void* data, const size_t& size, std::ostream& fileStream )
			{
				fileStream.write( static_cast<const char *>(data), size );
			}
#pragma endregion

#pragma region Reader
			Data::ProjectData* Reader::LoadProjectData( const std::string& path )
			{
				if ( path.empty() == true )
				{
					TCMWARNING( "BinarySerialization::LoadProjectData() - path is empty" );
					return nullptr;
				}

				std::ifstream projectIOFile( path, std::ifstream::in | std::ifstream::binary );
				if ( !projectIOFile.is_open() )
				{
					TCMWARNING( "BinarySerialization::LoadProjectData() - unable to open file [" + path + "]" );
					return nullptr;
				}

				Data::ProjectData* projectData = new Data::ProjectData();

				try
				{
					EPrefix prefix = EPrefix::BADPREFIX;
					EPrefix lastPrefix = EPrefix::BADPREFIX;

					_ReadPrefixFromFile( prefix, projectIOFile );

					if ( prefix != EPrefix::PROJECT )
						throw std::runtime_error( "BinarySerialization::LoadProjectData() - file is not a project file" );

					while ( _ReadPrefixFromFile( prefix, projectIOFile ) != false && prefix != EPrefix::END_PROJECT )
					{
						switch ( prefix )
						{
							case EPrefix::SCENE:
							{
								projectData->_AddScene( _LoadSceneDataFromFile( projectIOFile ) );
								break;
							}

								// Expected prefix after
							case EPrefix::END_SCENE:
							{
								if ( static_cast<uint8_t>(lastPrefix) != static_cast<uint8_t>(prefix) - static_cast<uint8_t>(1) )
									throw std::runtime_error( "BinarySerialization::LoadProjectData() - expected closing prefix" );
								break;
							}

								// Impossible prefix
							case EPrefix::PROJECT:
							case EPrefix::END_PROJECT: // Because should already be out if it's the case
							case EPrefix::SCENEOBJECT:
							case EPrefix::END_SCENEOBJECT:
							case EPrefix::COMPONENT:
							case EPrefix::END_COMPONENT:
							case EPrefix::ATTRIBUTES:
							case EPrefix::END_ATTRIBUTES:

							case EPrefix::SKYBOX:
							case EPrefix::END_SKYBOX:
							case EPrefix::ONEATTRIB:
							case EPrefix::END_ONEATTRIB:

							case EPrefix::BADPREFIX:
							default:
								throw std::runtime_error( "BinarySerialization::LoadProjectData() - unexpected prefix received [" + std::to_string( static_cast<uint8_t>(prefix) ) + "]" );
						}

						lastPrefix = prefix;
					}
				}
				catch ( std::exception& e )
				{
					delete projectData;

					TCMFAILURE( std::string( e.what() ) );
				}

				projectIOFile.close();

				return projectData;
			}

			Data::SceneData* Reader::LoadSceneData( const std::string& path )
			{
				if ( path.empty() == true )
				{
					TCMWARNING( "BinarySerialization::LoadSceneData() - path is empty" );
					return nullptr;
				}

				std::ifstream sceneIOFile( path, std::ifstream::in | std::ifstream::binary );
				if ( !sceneIOFile.is_open() )
				{
					TCMWARNING( "BinarySerialization::LoadSceneData() - unable to open file [" + path + "]" );
					return nullptr;
				}

				Data::SceneData* sceneData = new Data::SceneData();

				EPrefix prefix = EPrefix::BADPREFIX;
				EPrefix lastPrefix = EPrefix::BADPREFIX;

				_ReadPrefixFromFile( prefix, sceneIOFile );

				if ( prefix != EPrefix::SCENE )
					throw std::runtime_error( "BinarySerialization::LoadSceneData() - file is not a Scene file" );

				while ( _ReadPrefixFromFile( prefix, sceneIOFile ) != false && prefix != EPrefix::END_SCENE )
				{
					switch ( prefix )
					{
						case EPrefix::SKYBOX:
						{
							for ( size_t i = 0; i < 6; ++i )
							{
								std::string& oneFace = sceneData->_GetSkyfaceAt( i );

								// Get the length of the path
								size_t len = 0;
								_ReadFromFile( &len, sizeof( size_t), sceneIOFile );

								// Read the path of size specified before
								char* buff = new char[len + 1];
								memset( buff, 0, len + 1 );
								_ReadFromFile( buff, len, sceneIOFile );
								oneFace.assign( buff );
								delete[] buff;
							}
							break;
						}

						case EPrefix::SCENEOBJECT:
						{
							sceneData->_AddRootSceneObject( _LoadSceneObjectFromFile( sceneIOFile ) );
							break;
						}

						case EPrefix::ATTRIBUTES:
						{
							_LoadAttributesFromFile( sceneData, sceneIOFile );
							break;
						}

							// Expected prefix after
						case EPrefix::END_SKYBOX:
						{
							if ( static_cast<uint8_t>(lastPrefix) != static_cast<uint8_t>(prefix) - static_cast<uint8_t>(1) )
								throw std::runtime_error( "BinarySerialization::LoadSceneData() - expected closing prefix" );
							break;
						}

							// Impossible prefix
						case EPrefix::PROJECT:
						case EPrefix::END_PROJECT:
						case EPrefix::SCENE:
						case EPrefix::END_SCENE: // Because should already be out if it's the case
						case EPrefix::END_SCENEOBJECT:
						case EPrefix::COMPONENT:
						case EPrefix::END_COMPONENT:
						case EPrefix::END_ATTRIBUTES:

						case EPrefix::ONEATTRIB:
						case EPrefix::END_ONEATTRIB:

						case EPrefix::BADPREFIX:
						default:
							throw std::runtime_error( "BinarySerialization::LoadSceneData() - unexpected prefix received [" + std::to_string( static_cast<uint8_t>(prefix) ) + "]" );
					}

					lastPrefix = prefix;
				}

				sceneIOFile.close();

				return sceneData;
			}

			Data::SceneObjectData* Reader::LoadSceneObjectData( const std::string& path )
			{
				if ( path.empty() == true )
				{
					TCMWARNING( "BinarySerialization::LoadSceneObjectData() - path is empty" );
					return nullptr;
				}

				std::ifstream sceneObjectIOFile( path, std::ifstream::in | std::ifstream::binary );
				if ( !sceneObjectIOFile.is_open() )
				{
					TCMWARNING( "BinarySerialization::LoadSceneObjectData() - unable to open file [" + path + "]" );
					return nullptr;
				}

				EPrefix prefix = EPrefix::BADPREFIX;

				Data::SceneObjectData* sceneObjectData = nullptr;
				if ( _ReadPrefixFromFile( prefix, sceneObjectIOFile ) != false && prefix == EPrefix::SCENEOBJECT )
				{
					sceneObjectData = _LoadSceneObjectFromFile( sceneObjectIOFile );
				}

				sceneObjectIOFile.close();

				return sceneObjectData;
			}

			Data::SceneObjectData* Reader::ReadSceneObjectData( const std::string& string )
			{
				std::istringstream stream( string, std::istringstream::in | std::istringstream::binary );

				EPrefix prefix = EPrefix::BADPREFIX;

				Data::SceneObjectData* sceneObjectData = nullptr;
				if ( _ReadPrefixFromFile( prefix, stream ) != false && prefix == EPrefix::SCENEOBJECT )
				{
					sceneObjectData = _LoadSceneObjectFromFile( stream );
				}

				return sceneObjectData;
			}

			Data::SceneData* Reader::_LoadSceneDataFromFile( std::istream& projectIOStream )
			{
				size_t len = 0;
				if ( _ReadFromFile( &len, sizeof( size_t), projectIOStream ) == false )
					throw std::runtime_error( "BinarySerialization::_LoadSceneDataFromFile() - unexpected eof - scene len" );

				char* fileName = new char[len + 1];
				memset( fileName, 0, len + 1 );
				if ( _ReadFromFile( fileName, len, projectIOStream ) == false )
					throw std::runtime_error( "BinarySerialization::_LoadSceneDataFromFile() - unexpected eof - scene fileName" );

				// TODO: check if it's the filename or path we send
				Data::SceneData* data = LoadSceneData( fileName );

				delete[] fileName;

				return data;
			}

			Data::SceneObjectData* Reader::_LoadSceneObjectFromFile( std::istream& sceneIOStream )
			{
				Data::SceneObjectData* sceneObjectData = new Data::SceneObjectData();

				EPrefix prefix = EPrefix::BADPREFIX;
				EPrefix lastPrefix = EPrefix::BADPREFIX;

				while ( _ReadPrefixFromFile( prefix, sceneIOStream ) != false && prefix != EPrefix::END_SCENEOBJECT )
				{
					switch ( prefix )
					{
						case EPrefix::SCENEOBJECT:
						{
							sceneObjectData->_AddChildSceneObject( _LoadSceneObjectFromFile( sceneIOStream ) );
							break;
						}

						case EPrefix::COMPONENT:
						{
							sceneObjectData->_AddComponent( _LoadComponentFromFile( sceneIOStream ) );
							break;
						}

						case EPrefix::ATTRIBUTES:
						{
							_LoadAttributesFromFile( sceneObjectData, sceneIOStream );
							break;
						}

							// Impossible prefix
						case EPrefix::PROJECT:
						case EPrefix::END_PROJECT:
						case EPrefix::SCENE:
						case EPrefix::END_SCENE:
						case EPrefix::END_SCENEOBJECT: // Because should already be out if it's the case
						case EPrefix::END_COMPONENT: // Is already parsed in the function
						case EPrefix::END_ATTRIBUTES: // Is already parsed in the function

						case EPrefix::SKYBOX:
						case EPrefix::END_SKYBOX:
						case EPrefix::ONEATTRIB:
						case EPrefix::END_ONEATTRIB:

						case EPrefix::BADPREFIX:
						default:
							throw std::runtime_error( "BinarySerialization::_LoadSceneObject() - unexpected prefix received [" + std::to_string( static_cast<uint8_t>(prefix) ) + "]" );
					}

					lastPrefix = prefix;
				}

				return sceneObjectData;
			}

			Data::ComponentData* Reader::_LoadComponentFromFile( std::istream& sceneIOStream )
			{
				Data::ComponentData* componentData = new Data::ComponentData();

				EPrefix prefix = EPrefix::BADPREFIX;

				while ( _ReadPrefixFromFile( prefix, sceneIOStream ) != false && prefix != EPrefix::END_COMPONENT )
				{
					switch ( prefix )
					{
						case EPrefix::ATTRIBUTES:
						{
							_LoadAttributesFromFile( componentData, sceneIOStream );
							break;
						}

							// Impossible prefix
						case EPrefix::PROJECT:
						case EPrefix::END_PROJECT:
						case EPrefix::SCENE:
						case EPrefix::END_SCENE:
						case EPrefix::SCENEOBJECT:
						case EPrefix::END_SCENEOBJECT:
						case EPrefix::COMPONENT:
						case EPrefix::END_COMPONENT: // Because should already be out if it's the case
						case EPrefix::END_ATTRIBUTES: // Is already parsed in the function

						case EPrefix::SKYBOX:
						case EPrefix::END_SKYBOX:
						case EPrefix::ONEATTRIB:
						case EPrefix::END_ONEATTRIB:

						case EPrefix::BADPREFIX:
						default:
							throw std::runtime_error( "BinarySerialization::_LoadComponentFromFile() - unexpected prefix received [" + std::to_string( static_cast<uint8_t>(prefix) ) + "]" );
					}
				}

				return componentData;
			}

			bool Reader::_LoadAttributesFromFile( Data::BaseData* baseData, std::istream& stream )
			{
				EPrefix prefix = EPrefix::BADPREFIX;

				// Read MetaFactory len then name
				size_t len = 0;
				_ReadFromFile( &len, sizeof( size_t), stream );

				char* metaName = new char[len + 1];
				memset( metaName, 0, len + 1 );
				_ReadFromFile( metaName, len, stream );
				baseData->_SetClassMetaName( metaName );

				// Create tmporary class to get attributes
				Engine::TCMBase* tmp = Engine::MetaFactory::Create( metaName );

				const std::map<const std::string, std::string>* mapClassMembers = tmp->_GetMapClassMembers();

				// delete tmp, mapClassMembers is static
				tmp->_ClassDestructor( tmp );
				delete[] metaName;

				while ( _ReadPrefixFromFile( prefix, stream ) != false && prefix != EPrefix::END_ATTRIBUTES )
				{
					switch ( prefix )
					{
						case EPrefix::ONEATTRIB:
						{
							baseData->_AddSerializedVar( _LoadOneAttributeFromFile( mapClassMembers, stream ) );
							break;
						}

							// Impossible prefix
						case EPrefix::PROJECT:
						case EPrefix::END_PROJECT:
						case EPrefix::SCENE:
						case EPrefix::END_SCENE:
						case EPrefix::SCENEOBJECT:
						case EPrefix::END_SCENEOBJECT:
						case EPrefix::COMPONENT:
						case EPrefix::END_COMPONENT:
						case EPrefix::ATTRIBUTES:
						case EPrefix::END_ATTRIBUTES: // Because should already be out if it's the case

						case EPrefix::SKYBOX:
						case EPrefix::END_SKYBOX:
						case EPrefix::END_ONEATTRIB: // Is already parsed in the function

						case EPrefix::BADPREFIX:
						default:
							throw std::runtime_error( "BinarySerialization::_LoadAttributesFromFile() - unexpected prefix received [" + std::to_string( static_cast<uint8_t>(prefix) ) + "]" );
					}
				}

				return true;
			}

			SerializedInterface* Reader::_LoadOneAttributeFromFile( const std::map<const std::string, std::string>* mapClassMembers, std::istream& stream )
			{
				EPrefix prefix = EPrefix::BADPREFIX;

				// Read the length of the name
				size_t len = 0;
				_ReadFromFile( &len, sizeof( size_t), stream );

				// Read the name of length -> len
				char* nameVar = new char[len + 1];
				memset( nameVar, 0, len + 1 );
				_ReadFromFile( nameVar, len, stream );

				auto iter = mapClassMembers->find( nameVar );
				if ( iter == mapClassMembers->end() )
				{
					std::string sNameVar( nameVar );
					delete[] nameVar;
					throw std::runtime_error( "BinarySerialization::_LoadOneAttributeFromFile - couldn't find [" + sNameVar + "] in map class members" );
				}

				std::string currAttributeTypeName = iter->second;

				SerializedInterface* wellTypedInterface = Engine::MetaSerializedFactory::Create( currAttributeTypeName );
				wellTypedInterface->SetVarName( nameVar );
				delete[] nameVar;

				wellTypedInterface->ReadFromFile( stream );

				if ( _ReadPrefixFromFile( prefix, stream ) == false )
					throw std::runtime_error( "BinarySerialization::_LoadOneAttributeFromFile() - unexpected prefix received [" + std::to_string( static_cast<uint8_t>(prefix) ) + "], waiting for END_ONEATTRIB" );
				if ( prefix != EPrefix::END_ONEATTRIB )
					throw std::runtime_error( "BinarySerialization::_LoadOneAttributeFromFile() - unexpected prefix received [" + std::to_string( static_cast<uint8_t>(prefix) ) + "], waiting for END_ONEATTRIB" );

				return wellTypedInterface;
			}

			bool Reader::_ReadPrefixFromFile( EPrefix& prefix, std::istream& fileIO )
			{
				fileIO.read( reinterpret_cast<char *>(&prefix), sizeof( EPrefix) );

				return !fileIO.eof();
			}

			bool Reader::_ReadFromFile( void* data, const size_t& size, std::istream& fileIO )
			{
				if ( data == nullptr )
				{
					TCMFAILURE( "BinarySerialization::_ReadFromFile() - data should be initialized to size before reading" );
					return false;
				}

				fileIO.read( reinterpret_cast<char *>(data), size );

				return !fileIO.eof();
			}
#pragma endregion
		}
	}
}
