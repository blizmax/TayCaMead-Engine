#include "BaseInspector.h"

#include "EngineClasses/TCMBase.h"
#include "VarInspectorFactory.h"
#include "EngineClasses/TCMSceneObject.h"

#define  min(a,b) ((a < b) ? a : b)

namespace TCM
{
	namespace Editor
	{
		BaseInspector::BaseInspector()
		{
			m_isSceneInspector = true;

			Engine::TCMSceneObject* obj = Engine::TCMSceneObject::Create();
			Init( obj );
			Engine::TCMSceneObject::Release( obj );
		}

		BaseInspector::BaseInspector( TCM::Engine::TCMBase* base )
		{
			Init( base );
		}

		BaseInspector::~BaseInspector()
		{
			for ( auto inspector : m_inspectors )
				delete inspector.second;
			m_inspectors.clear();
		}

		void BaseInspector::UpdateFromValues()
		{
			for ( auto inspector : m_inspectors )
				inspector.second->UpdateFromValue();
		}

		void BaseInspector::SetBase( Engine::TCMBase* base )
		{
			if ( !m_isSceneInspector )
				return;
			if ( !dynamic_cast<Engine::TCMSceneObject *>(base) )
				return;

			base->_Serialize();
			const std::unordered_map<std::string, SerializedInterface*>* pairs = base->_GetSerializedAttributes();

			for ( auto inspector : m_inspectors )
				inspector.second->SetVar( pairs->find( inspector.first )->second );
		}

		void BaseInspector::Init( Engine::TCMBase* base )
		{
			base->_Serialize();
			const std::unordered_map<std::string, SerializedInterface*>* pairs = base->_GetSerializedAttributes();

			for ( auto pair : *pairs )
			{
				VarInspector* varInspector = VarInspectorFactory::Create( pair.second );
				if ( !varInspector )
					continue;

				m_inspectors[pair.first] = varInspector;
				varInspector->Init();
			}
		}

		std::vector<std::string> BaseInspector::ParseVarPath( const std::string& name ) const
		{
			std::vector<std::string> result;

			std::string str( name );

			size_t position;

			while ( ( position = min( str.find_first_of( '.' ), str.find_first_of( "->" ) ) ) != std::string::npos )
			{
				std::string string = ParseVarName( str.substr( 0, position ) );
				if ( string.size() > 0 )
					result.push_back( string );
				str = str.substr( position + 1 );
			}
			result.push_back( ParseVarName( str ) );
			return result;
		}

		std::string BaseInspector::ParseVarName( const std::string& name ) const
		{
			std::string result( name );

			if ( result[0] == 'm' && result[1] == '_' )
				result.erase( 0, 2 );

			if ( result[0] == '_' )
				result.erase( 0, 1 );

			if ( result[0] == 'b' && isupper( result[1] ) )
				result.erase( 0, 1 );

			if ( result.size() == 0 )
				return "";

			result[0] = toupper( result[0] );

			int i = 0;
			while ( i < result.size() - 1 )
			{
				if ( islower( result[i] ) && isupper( result[i + 1] ) )
				{
					result.insert( result.begin() + i + 1, ' ' );
					++i;
				}
				++i;
			}

			return result;
		}
	}
}
