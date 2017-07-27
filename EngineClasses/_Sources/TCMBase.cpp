#include "TCMBase.h"

namespace TCM
{
	namespace Engine
	{
		bool TCMBase::_bHasBeenMetaFactoried = false;
		const std::map<const std::string, std::string>* TCMBase::_mapClassMembers = TCMBase::_CreateMapClassMembers();

		TCMBase::TCMBase()
			: m_name( nullptr )
			, m_bEnabled( true )
			, m_sizeInBytes( 0 )
			, m_serializedVars( nullptr )
		{
			m_name = new std::string( "TCMBase" );
			m_serializedVars = new std::unordered_map<std::string, SerializedInterface *>();
		}

		TCMBase::~TCMBase()
		{
			if ( m_name )
				delete m_name;

			if ( m_serializedVars )
			{
				for ( auto iter = m_serializedVars->begin(); iter != m_serializedVars->end(); ++iter )
				{
					if ( iter->second )
					{
						delete iter->second;
					}
				}
				delete m_serializedVars;
			}
		}

		std::string TCMBase::ToString() const
		{
			return GetName();
		}

		void TCMBase::SetName( const std::string& name ) const
		{
			m_name->assign( name );
		}

		std::string TCMBase::GetName() const
		{
			return std::string( *m_name );
		}

		void TCMBase::SetEnabled( const bool bEnabled, const bool callback )
		{
			if ( m_bEnabled == bEnabled )
				return;

			m_bEnabled = bEnabled;

			if ( !callback )
				return;

			if ( m_bEnabled )
				OnEnable();
			else
				OnDisable();
		}

		bool TCMBase::IsEnabled() const
		{
			return m_bEnabled;
		}

		size_t TCMBase::SizeOf() const
		{
			return m_sizeInBytes;
		}

		void TCMBase::OnEnable()
		{
		}

		void TCMBase::OnDisable()
		{
		}
	}
}
