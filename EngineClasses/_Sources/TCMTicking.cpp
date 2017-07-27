#include <stdexcept>
#include "TCMTicking.h"

namespace TCM
{
	namespace Engine
	{
		TCMTicking::TCMTicking()
			: m_bInit( false )
			, m_bIsTicking( true )
			, m_bNeedDestroy( false )
		{
			SetName( "TCMTicking" );
		}

		TCMTicking::~TCMTicking()
		{
		}

		void TCMTicking::Init()
		{
			m_bInit = true;
		}

		void TCMTicking::TCMEditorUpdate( const float deltaTime )
		{
			// Do nothing
		}

		bool TCMTicking::TCMUpdate( const float deltaTime )
		{
			if ( IsEnabled() && m_bIsTicking )
				Update( deltaTime );
			else
				return false;
			return true;
		}

		bool TCMTicking::TCMFixedUpdate( const float fixedDeltaTime )
		{
			if ( IsEnabled() && m_bIsTicking )
				FixedUpdate( fixedDeltaTime );
			else
				return false;
			return true;
		}

		bool TCMTicking::TCMRender()
		{
			bool enabled = IsEnabled();
			if ( enabled )
				RenderGizmos();

			return enabled;
		}

		void TCMTicking::TCMDestroy()
		{
			m_bNeedDestroy = true;

			OnDestroy();

			m_bInit = false;
		}

		void TCMTicking::Destroy()
		{
			TCMDestroy();
		}

		bool TCMTicking::HasToBeDestroyed() const
		{
			return m_bNeedDestroy;
		}

		bool TCMTicking::HasToBeInitialized() const
		{
			return !m_bInit;
		}

		void TCMTicking::Update( const float deltaTime )
		{
			if ( m_bIsTicking == false )
			{
				throw new std::runtime_error( "TCMTicking::Update(): [" + ToString() + "] ticked when not supposed to!" );
			}
		}

		void TCMTicking::FixedUpdate( const float fixedDeltaTime )
		{
			if ( m_bIsTicking == false )
			{
				throw new std::runtime_error( "TCMTicking::FixedUpdate(): [" + ToString() + "] ticked when not supposed to!" );
			}
		}

		void TCMTicking::RenderGizmos()
		{
			if ( IsEnabled() == false )
			{
				throw new std::runtime_error( "TCMTicking::Render(): [" + ToString() + "] rendered when not supposed to!" );
			}
		}

		void TCMTicking::OnDestroy()
		{
		}
	}
}
