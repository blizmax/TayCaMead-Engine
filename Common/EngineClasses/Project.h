#pragma once
#include "EngineClassesDefines.h"

#include "MathLib/Vec3.h"
#include <vector>
#include "TCMBase.h"

namespace TCM
{
	namespace Engine
	{
		class TCMScene;

		struct ENGINECLASSES_DLL_EXPORT GraphicSettings
		{
			bool m_HDREnabled = false;
			bool m_MSAAEnabled = false;
			int m_MSAASamples = 2;
			int m_shadowMapSizeX = 1024;
			int m_shadowMapSizeY = 1024;

			float m_shadowNearPlane = 1.0f;
			float m_shadowFarPlane = 1.0f;

			int GetShadowMapSizeX() const;
			int GetShadowMapSizeY() const;
			float GetShadowNearPlane() const;
			float GetShadowFarPlane() const;
			bool GetHdrEnabled() const;
			bool GetMsaaEnabled() const;
			int GetMsaaSamples() const;
		};

		struct ENGINECLASSES_DLL_EXPORT PhysicSettings
		{
			PhysicSettings();
			~PhysicSettings();
			Math::Vec3 m_gravity = { 0.0f, -9.8f, 0.0f };
			Math::Vec3 m_inertie = { 0.0f, -1.0f, 0.0f };
			float m_friction = 1.0f;
			float m_restitution = 0.5f;
			std::vector<std::string*>* m_layerVector; // TODO: Layer tab[][]
			std::vector<std::vector<bool>*>* m_layerMatrix;

			Math::Vec3 GetInertie() const;
			Math::Vec3 GetGravity() const;
			float GetFriction() const;
			float GetRestitution() const;
			std::vector<std::string*>* GetLayerVector() const;
			std::vector<std::vector<bool>*>* GetLayerMatrix() const;
		};

		class ENGINECLASSES_DLL_EXPORT Project final : public TCMBase
		{
			TCM_CLASS( Project, TCMBase );

			TCM_SERIALIZE( m_sceneBuildOrder
				, m_scenePaths
				, m_tagVector
				, m_graphicSettings.m_MSAAEnabled
				, m_graphicSettings.m_MSAASamples
				, m_graphicSettings.m_HDREnabled
				, m_graphicSettings.m_shadowMapSizeX
				, m_graphicSettings.m_shadowMapSizeY
				, m_graphicSettings.m_shadowNearPlane
				, m_graphicSettings.m_shadowFarPlane
				, m_physicSettings.m_gravity
				, m_physicSettings.m_friction
				, m_physicSettings.m_restitution
				, m_physicSettings.m_layerVector
				, m_physicSettings.m_layerMatrix );

			Project();
			~Project();

			std::vector<int>* GetSceneBuildOrder() const;
			std::vector<std::string>* GetScenePaths() const;
			std::vector<TCMScene *>* GetScenes() const;
			std::vector<std::string>* GetTags() const;

			void SetPath( const std::string& path ) const;
			std::string* GetPath() const;
			void AddScene( TCMScene* scene ) const;
			void AddScenePath( const std::string& path ) const;

			GraphicSettings m_graphicSettings;
			PhysicSettings m_physicSettings;

		private:
			std::vector<int>* m_sceneBuildOrder;
			std::vector<std::string>* m_scenePaths;
			std::vector<TCMScene *>* m_scenes;
			std::vector<std::string>* m_tagVector;
			std::string* m_path;
		};
	}
}
