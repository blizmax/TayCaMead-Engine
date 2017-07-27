#include "TCMSaveManager/SceneData.h"

#include "TCMSaveManager/ProjectData.h"
#include "TCMSaveManager/SceneObjectData.h"

namespace TCM
{
	namespace Editor
	{
		namespace Data
		{
			SceneData::SceneData()
				: m_filename( new std::string() )
				, m_project( nullptr )
				, m_skyfaces( new std::vector<std::string> )
				, m_rootSceneObjects( new std::vector<SceneObjectData *>() )
			{
				m_skyfaces->resize( 6 );
			}

			SceneData::SceneData( Engine::TCMScene* scene, ProjectData* project ): BaseData( scene )
			                                                                     , m_filename( new std::string() )
			                                                                     , m_project( project )
			                                                                     , m_skyfaces( new std::vector<std::string> )
			                                                                     , m_rootSceneObjects( new std::vector<SceneObjectData *>() )
			{
				m_skyfaces->resize( 6 );
				const std::vector<std::string>* sceneSkyfaces = scene->GetSkyfaces();

				for ( size_t i = 0; i < 6; ++i )
				{
					std::string& oneFace = m_skyfaces->at( i );
					oneFace.assign( sceneSkyfaces->at( i ) );
				}

				std::vector<Engine::TCMSceneObject *> vecSceneObjects = scene->GetRootSceneObjects();
				m_rootSceneObjects->reserve( vecSceneObjects.size() );

				for ( auto iter = vecSceneObjects.begin(); iter != vecSceneObjects.end(); ++iter )
				{
					m_rootSceneObjects->push_back( new SceneObjectData( *iter, this, nullptr ) );
				}
			}

			SceneData::~SceneData()
			{
				if ( m_rootSceneObjects )
				{
					for ( auto iter = m_rootSceneObjects->begin(); iter != m_rootSceneObjects->end(); ++iter )
					{
						if ( *iter )
						{
							delete *iter;
							*iter = nullptr;
						}
					}
					delete m_rootSceneObjects;
					m_rootSceneObjects = nullptr;
				}

				if ( m_filename )
				{
					delete m_filename;
					m_filename = nullptr;
				}

				if ( m_skyfaces )
				{
					delete m_skyfaces;
					m_skyfaces = nullptr;
				}
			}

			Engine::TCMScene* SceneData::Extract() const
			{
				Engine::TCMScene* scene = dynamic_cast<Engine::TCMScene *>(ExtractSerialized());

				scene->SetSkyfaces( m_skyfaces->data() );

				for ( auto iter = m_rootSceneObjects->begin(); iter != m_rootSceneObjects->end(); ++iter )
				{
					scene->AddSceneObject( dynamic_cast<Engine::TCMSceneObject *>(( *iter )->Extract()) );
				}

				return scene;
			}

			void SceneData::_AddRootSceneObject( SceneObjectData* sceneObjectData )
			{
				m_rootSceneObjects->push_back( sceneObjectData );
				sceneObjectData->_SetScene( this );
			}

			std::string SceneData::GetFilename() const
			{
				if ( m_filename )
					return std::string( *m_filename );
				return std::string();
			}

			void SceneData::SetFilename( const std::string& filename )
			{
				m_filename->assign( filename );
			}

			const std::vector<std::string>* SceneData::_GetSkyfaces() const
			{
				return m_skyfaces;
			}

			std::string& SceneData::_GetSkyfaceAt( const size_t& index ) const
			{
				return m_skyfaces->at( index );
			}

			const std::vector<SceneObjectData*>* SceneData::_GetRootSceneObjects() const
			{
				return m_rootSceneObjects;
			}
		}
	}
}
