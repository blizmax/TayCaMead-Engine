#include "EngineClasses/Project.h"

#include "TCMScene.h"

namespace TCM
{
	namespace Engine
	{
		int GraphicSettings::GetShadowMapSizeX() const
		{
			return m_shadowMapSizeX;
		}

		int GraphicSettings::GetShadowMapSizeY() const
		{
			return m_shadowMapSizeY;
		}

		float GraphicSettings::GetShadowNearPlane() const
		{
			return m_shadowNearPlane;
		}

		float GraphicSettings::GetShadowFarPlane() const
		{
			return m_shadowFarPlane;
		}

		bool GraphicSettings::GetHdrEnabled() const
		{
			return m_HDREnabled;
		}

		bool GraphicSettings::GetMsaaEnabled() const
		{
			return m_MSAAEnabled;
		}

		int GraphicSettings::GetMsaaSamples() const
		{
			return m_MSAASamples;
		}

		PhysicSettings::PhysicSettings()
		{
			m_layerVector = new std::vector<std::string*>;
			m_layerMatrix = new std::vector<std::vector<bool>*>;
		}

		PhysicSettings::~PhysicSettings()
		{
			if ( m_layerVector )
			{
				std::vector<std::string*>::iterator current = m_layerVector->begin();
				std::vector<std::string*>::iterator end = m_layerVector->end();

				for ( ; current != end; ++current )
				{
					std::string* value = *current;
					if ( value )
					{
						delete value;
					}
					*current = nullptr;
				}
				delete m_layerVector;
			}
			m_layerVector = nullptr;

			if ( m_layerMatrix )
			{
				std::vector<std::vector<bool>*>::iterator upperCurrent = m_layerMatrix->begin();
				std::vector<std::vector<bool>*>::iterator upperEnd = m_layerMatrix->end();

				for ( ; upperCurrent != upperEnd; ++upperCurrent )
				{
					std::vector<bool>* value = *upperCurrent;
					value->clear();
					delete value;
				}
				delete m_layerMatrix;
			}
			m_layerMatrix = nullptr;
		}

		Math::Vec3 PhysicSettings::GetInertie() const
		{
			return m_inertie;
		}

		Math::Vec3 PhysicSettings::GetGravity() const
		{
			return m_gravity;
		}

		float PhysicSettings::GetFriction() const
		{
			return m_friction;
		}

		float PhysicSettings::GetRestitution() const
		{
			return m_restitution;
		}

		std::vector<std::string*>* PhysicSettings::GetLayerVector() const
		{
			return m_layerVector;
		}

		std::vector<std::vector<bool>*>* PhysicSettings::GetLayerMatrix() const
		{
			return m_layerMatrix;
		}

		TCM_DEFINE( Project );

		Project::Project(): m_sceneBuildOrder( new std::vector<int> )
		                  , m_scenePaths( new std::vector<std::string> )
		                  , m_scenes( new std::vector<TCMScene *> )
		                  , m_tagVector( new std::vector<std::string> )
		                  , m_path( new std::string )
		{
		}

		Project::~Project()
		{
			if ( m_sceneBuildOrder )
				delete m_sceneBuildOrder;
			m_sceneBuildOrder = nullptr;

			if ( m_scenePaths )
			{
				delete m_scenePaths;
			}
			m_scenePaths = nullptr;

			delete m_scenes;

			if ( m_tagVector )
			{
				delete m_tagVector;
			}
			m_tagVector = nullptr;

			if ( m_path )
				delete m_path;
			m_path = nullptr;
		}

		std::vector<int>* Project::GetSceneBuildOrder() const
		{
			return m_sceneBuildOrder;
		}

		std::vector<std::string>* Project::GetScenePaths() const
		{
			return m_scenePaths;
		}

		std::vector<TCMScene*>* Project::GetScenes() const
		{
			return m_scenes;
		}

		std::vector<std::string>* Project::GetTags() const
		{
			return m_tagVector;
		}

		void Project::SetPath( const std::string& path ) const
		{
			m_path->assign( path );
		}

		std::string* Project::GetPath() const
		{
			return m_path;
		}

		void Project::AddScene( TCMScene* scene ) const
		{
			m_scenes->push_back( scene );
		}

		void Project::AddScenePath( const std::string& path ) const
		{
			auto it = m_scenePaths->begin();
			for ( ; it != m_scenePaths->end(); ++it )
				if ( *it == path )
					break;
			if ( it != m_scenePaths->end() )
				m_scenePaths->push_back( path );
		}
	}
}
