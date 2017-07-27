#include "TCMScene.h"
#include "TCMSceneObject.h"
#include "EngineClasses/TCMCameraComponent.h"
#include "TCMRenderer/Rendering/RenderGraph.h"
#include "TCMRenderer/Rendering/RendererFactory.h"
#include "TCMInputManager/InputManager.h"
#include "TCMRenderer/Gizmo/Gizmo.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMScene )

		TCMScene::TCMScene()
			: m_skyboxData( nullptr )
			, m_rootObject( nullptr )
			, m_bOvniMode( false )
		{
			SetName( "TCMScene" );

			m_rootObject = TCMSceneObject::Create();
			m_skyfaces = new std::vector<std::string>();
			m_skyfaces->resize( 6 );

			m_ovni = TCMSceneObject::Create();
			TCMCameraComponent* cam = TCMComponent::Create<TCMCameraComponent>();
			cam->GetCameraData()->m_debugCamera = true;
			cam->GetCameraData()->SetDepth( -1 );
			m_ovni->AddComponent( cam );
		}

		TCMScene::~TCMScene()
		{
			delete m_skyfaces;

			if ( m_skyboxData != nullptr )
				delete m_skyboxData;

			TCMSceneObject::Release( m_rootObject );
			TCMSceneObject::Release( m_ovni );
		}

		void TCMScene::TCMEditorUpdate( const float deltaTime )
		{
			m_rootObject->TCMEditorUpdate( deltaTime );
			m_ovni->TCMEditorUpdate( deltaTime );
		}

		void TCMScene::Init()
		{
			Super::Init();

			if ( m_rootObject->HasToBeInitialized() )
				m_rootObject->Init();

			if ( m_ovni->HasToBeInitialized() )
			{
				m_ovni->Init();
				m_ovni->GetComponent<TCMCameraComponent>()->ConnectInputManager();
				m_ovni->SetEnabled( m_bOvniMode );
			}

			UserIO::InputManager::GetKeyEvent( SSpecialKey::KEY_CONTROL, SKey::KEY_O ).Push( &TCMScene::SwitchOvniMode, this );

			if ( !m_skyboxData )
				m_skyboxData = Graphics::RendererFactory::CreateSkybox( m_skyfaces->data() );
		}

		bool TCMScene::TCMUpdate( const float deltaTime )
		{
			if ( !Super::TCMUpdate( deltaTime ) )
				return false;

			m_rootObject->TCMUpdate( deltaTime );
			m_ovni->TCMUpdate( deltaTime );

			return true;
		}

		bool TCMScene::TCMFixedUpdate( const float fixedDeltaTime )
		{
			if ( !Super::TCMFixedUpdate( fixedDeltaTime ) )
				return false;

			m_rootObject->TCMFixedUpdate( fixedDeltaTime );
			m_ovni->TCMFixedUpdate( fixedDeltaTime );

			return true;
		}

		bool TCMScene::TCMRender()
		{
			if ( !Super::TCMRender() )
				return false;

			m_rootObject->TCMRender();

			m_ovni->TCMRender();

			if ( m_skyboxData )
				Graphics::RenderGraph::RegisterSkybox( m_skyboxData );

			return true;
		}

		void TCMScene::SetEnabled( const bool bEnabled, const bool callback )
		{
			Super::SetEnabled( bEnabled, callback );

			m_rootObject->SetEnabled( bEnabled, callback );

			m_ovni->SetEnabled( bEnabled, callback );
		}

		void TCMScene::AddSceneObject( TCMSceneObject* sceneObject ) const
		{
			if ( sceneObject != nullptr )
				m_rootObject->AddChild( sceneObject );
			else
			TCMWARNING("Parameter sceneObject is null");
		}

		bool TCMScene::RemoveSceneObject( TCMSceneObject* sceneObject ) const
		{
			return m_rootObject->RemoveChild( sceneObject );
		}

		void TCMScene::SetSkyfaces( std::string skyfaces[6] )
		{
			for ( int i = 0; i < 6; ++i )
			{
				( *m_skyfaces )[i] = skyfaces[i];
			}
			if ( m_skyboxData )
			{
				delete m_skyboxData;
				m_skyboxData = nullptr;
			}
			m_skyboxData = Graphics::RendererFactory::CreateSkybox( m_skyfaces->data() );
		}

		const std::vector<std::string>* TCMScene::GetSkyfaces() const
		{
			return m_skyfaces;
		}

		std::vector<TCMSceneObject*> TCMScene::GetAllSceneObjects() const
		{
			return m_rootObject->DeepGetAllChildren();
		}

		std::vector<TCMSceneObject*> TCMScene::GetRootSceneObjects() const
		{
			return m_rootObject->GetAllChildren();
		}

		TCMSceneObject* TCMScene::GetRootSceneObject() const
		{
			return m_rootObject;
		}

		void TCMScene::SwitchOvniMode()
		{
			if ( !IsEnabled() )
				return;
			m_bOvniMode = !m_bOvniMode;
			m_ovni->SetEnabled( m_bOvniMode );
		}

		void TCMScene::SwitchOvniMode( bool value )
		{
			if ( !IsEnabled() )
				return;
			m_bOvniMode = value;
			m_ovni->SetEnabled( value );
		}

		void TCMScene::OvniLookAt( TCMSceneObject* object ) const
		{
			if ( !IsEnabled() )
				return;
			Math::Mat4::LookAt( m_ovni->GetWorldPosition(), object->GetWorldPosition(), Math::Vec3::Up() );
		}

		void TCMScene::OnDestroy()
		{
			UserIO::InputManager::GetKeyEvent( SSpecialKey::KEY_CONTROL, SKey::KEY_O ).Pop( &TCMScene::SwitchOvniMode, this );

			m_rootObject->TCMDestroy();

			m_ovni->TCMDestroy();
		}

		void TCMScene::RenderGizmos()
		{
			Debug::Gizmo::Color( Math::ColorRGBA::White() );
			Debug::Gizmo::DrawGrid( -200, 200, -200, 200, 5 );
		}
	}
}
