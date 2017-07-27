#include "TCMCameraComponent.h"
#include "TCMInputManager/BaseInputManagerKeys.h"
#include "TCMTime/Time.h"
#include "TCMInputManager/InputManager.h"
#include "TCMRenderer/Camera/CameraManager.h"
#include "TCMMemory/TCMEventManager.hpp"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( TCMCameraComponent )

		TCMCameraComponent::TCMCameraComponent()
		{
			SetName( "TCMCameraComponent" );
		}

		TCMCameraComponent::~TCMCameraComponent()
		{
			if ( cameraData )
				delete cameraData;
		}

		void TCMCameraComponent::TCMPoolCreate()
		{
			Super::TCMPoolCreate();
			cameraData = new Graphics::Camera::CameraData();
		}

		void TCMCameraComponent::TCMPoolRelease()
		{
			delete cameraData;
			cameraData = nullptr;

			if ( m_inputManagerConnected )
				DisconnectInputManager();

			Super::TCMPoolRelease();
		}

		void TCMCameraComponent::MoveForward() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			Math::Vec3 position = m_sceneObject->GetLocalPosition() + m_sceneObject->Forward();
			m_sceneObject->SetPosition( position );
		}

		void TCMCameraComponent::MoveBackward() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			Math::Vec3 position = m_sceneObject->GetLocalPosition() - m_sceneObject->Forward();
			m_sceneObject->SetPosition( position );
		}

		void TCMCameraComponent::MoveLeft() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			Math::Vec3 position = m_sceneObject->GetLocalPosition() - m_sceneObject->Right();
			m_sceneObject->SetPosition( position );
		}

		void TCMCameraComponent::MoveRight() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			Math::Vec3 position = m_sceneObject->GetLocalPosition() + m_sceneObject->Right();
			m_sceneObject->SetPosition( position );
		}

		void TCMCameraComponent::MoveUp() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			Math::Vec3 position = m_sceneObject->GetLocalPosition() + m_sceneObject->Up();
			m_sceneObject->SetPosition( position );
		}

		void TCMCameraComponent::MoveDown() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			Math::Vec3 position = m_sceneObject->GetLocalPosition() - m_sceneObject->Up();
			m_sceneObject->SetPosition( position );
		}

		void TCMCameraComponent::RotateRight() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			Math::Vec3 rotation = m_sceneObject->GetLocalRotation();
			rotation.y += ( 90.0f * Time::GetDeltaTime() );
			m_sceneObject->SetRotation( rotation );
		}

		void TCMCameraComponent::RotateLeft() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			Math::Vec3 rotation = m_sceneObject->GetLocalRotation();
			rotation.y += ( -90.0f * Time::GetDeltaTime() );
			m_sceneObject->SetRotation( rotation );
		}

		void TCMCameraComponent::Rotate() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			Math::Vec3 rotation = m_sceneObject->GetLocalRotation();
			Math::Vec2 translation = UserIO::InputManager::GetMouseTranslation();
			rotation.y += translation.x * 90 * Time::GetDeltaTime() * 15;
			rotation.x -= translation.y * 90 * Time::GetDeltaTime() * 15;
			m_sceneObject->SetRotation( rotation );
		}

		void TCMCameraComponent::PickObject() const
		{
			if ( !IsEnabled() || !m_IsFocused )
				return;
			const Graphics::RenderData* pick = cameraData->ViewportPointToRay( UserIO::InputManager::GetMousePositionInViewport() );
			if ( pick == nullptr )
				return;
			TCMDEBUG( "Object picked!" );
		}

		void TCMCameraComponent::Init()
		{
			Super::Init();
			//InputManager::AddFunctionToMouseMove( BIND_MOUSE( &TCMCameraComponent::Rotate ) );

			cameraData->Init();
			cameraData->OnPositionChanged( m_sceneObject->GetWorldPosition() );
			cameraData->OnRotationChanged( m_sceneObject->GetWorldRotation() );
		}

		void TCMCameraComponent::TCMEditorUpdate( const float deltaTime )
		{
			Super::TCMEditorUpdate( deltaTime );

			CheckCameraFocus();
		}

		bool TCMCameraComponent::TCMUpdate( const float deltaTime )
		{
			if ( !Super::TCMUpdate( deltaTime ) )
				return false;

			CheckCameraFocus();

			return true;
		}

		Graphics::Camera::CameraData* TCMCameraComponent::GetCameraData() const
		{
			return cameraData;
		}

		void TCMCameraComponent::CheckCameraFocus()
		{
			m_IsFocused = cameraData->IsFocused( UserIO::InputManager::GetMousePositionInViewport() );
		}

		void TCMCameraComponent::ConnectInputManager()
		{
			m_inputManagerConnected = true;
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_W )
					.Push( &TCMCameraComponent::MoveForward, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_S )
					.Push( &TCMCameraComponent::MoveBackward, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_A )
					.Push( &TCMCameraComponent::MoveLeft, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_D )
					.Push( &TCMCameraComponent::MoveRight, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_R )
					.Push( &TCMCameraComponent::MoveUp, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_F )
					.Push( &TCMCameraComponent::MoveDown, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_Q )
					.Push( &TCMCameraComponent::RotateLeft, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_E )
					.Push( &TCMCameraComponent::RotateRight, this );
			UserIO::InputManager::GetMouseEvent( SMouseInput::LMB )
					.Push( &TCMCameraComponent::Rotate, this );
			UserIO::InputManager::GetMouseEvent( SMouseInput::RMB )
					.Push( &TCMCameraComponent::PickObject, this );
		}

		void TCMCameraComponent::DisconnectInputManager()
		{
			m_inputManagerConnected = false;
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_W )
					.Pop( &TCMCameraComponent::MoveForward, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_S )
					.Pop( &TCMCameraComponent::MoveBackward, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_A )
					.Pop( &TCMCameraComponent::MoveLeft, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_D )
					.Pop( &TCMCameraComponent::MoveRight, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_R )
					.Pop( &TCMCameraComponent::MoveUp, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_F )
					.Pop( &TCMCameraComponent::MoveDown, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_Q )
					.Pop( &TCMCameraComponent::RotateLeft, this );
			UserIO::InputManager::GetKeyEvent( SSpecialKey::NO_SPECIAL_KEY_INPUT, SKey::KEY_E )
					.Pop( &TCMCameraComponent::RotateRight, this );
			UserIO::InputManager::GetMouseEvent( SMouseInput::LMB )
					.Pop( &TCMCameraComponent::Rotate, this );
		}

		void TCMCameraComponent::OnPositionChanged( const Math::Vec3& position )
		{
			if ( cameraData )
				cameraData->OnPositionChanged( position );
		}

		void TCMCameraComponent::OnRotationChanged( const Math::Vec3& rotation )
		{
			if ( cameraData )
				cameraData->OnRotationChanged( rotation );
		}

		void TCMCameraComponent::OnEnable()
		{
			if ( cameraData )
				cameraData->SetEnable( true );
		}

		void TCMCameraComponent::OnDisable()
		{
			if ( cameraData )
				cameraData->SetEnable( false );
		}
	}
}
