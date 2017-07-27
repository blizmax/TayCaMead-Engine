#include "TCMSaveManager/ProjectData.h"

#include "TCMSaveManager/SceneData.h"

namespace TCM
{
	namespace Editor
	{
		namespace Data
		{
			ProjectData::ProjectData()
				: m_filename( new std::string() )
				, m_scenes( new std::vector<SceneData*>() )
			{
			}

			ProjectData::ProjectData( const Engine::Project* project )
				: m_filename( new std::string() )
				, m_scenes( new std::vector<SceneData*>() )
			{
				if ( project == nullptr )
				TCMFAILURE( "ProjectData::ProjectData() - project is nullptr" );

				const std::string* projectPath = project->GetPath();

				if ( projectPath )
					m_filename->assign( projectPath->c_str() );

				const std::vector<Engine::TCMScene *>* vecScene = project->GetScenes();
				for ( std::vector<Engine::TCMScene *>::const_iterator iter = vecScene->cbegin(); iter != vecScene->cend(); ++iter )
				{
					Engine::TCMScene* scene = *iter;
					m_scenes->push_back( new SceneData( scene ) );
				}
			}

			ProjectData::~ProjectData()
			{
				delete m_filename;
				// Maybe delete what's in it
				delete m_scenes;
			}

			Engine::Project* ProjectData::Extract() const
			{
				Engine::Project* project = dynamic_cast<Engine::Project *>(ExtractSerialized());

				project->SetPath( m_filename->c_str() );

				for ( auto iter = m_scenes->begin(); iter != m_scenes->end(); ++iter )
				{
					project->AddScene( dynamic_cast<Engine::TCMScene *>(( *iter )->Extract()) );
				}

				return project;
			}

			std::string ProjectData::GetFilename() const
			{
				if ( m_filename )
					return std::string( *m_filename );
				return std::string();
			}

			void ProjectData::SetFilename( const std::string& filename )
			{
				m_filename->assign( filename );
			}

			const std::vector<SceneData*>* ProjectData::_GetScenes() const
			{
				return m_scenes;
			}

			void ProjectData::_AddScene( SceneData* sceneData ) const
			{
#if _DEBUG
				if ( !sceneData )
				TCMFAILURE( "ProjectData::_AddScene() - sceneData is nullptr" );
				if ( !m_scenes )
				TCMFAILURE( "ProjectData::_AddScene() - m_scenes is nullptr" );
#endif
				m_scenes->push_back( sceneData );
			}
		}
	}
}
