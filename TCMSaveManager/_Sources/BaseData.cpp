#include "TCMSaveManager/BaseData.h"

#include "EngineClasses/TCMBase.h"

namespace TCM
{
	namespace Editor
	{
		namespace Data
		{
			BaseData::BaseData()
				: m_classMetaName( new std::string() )
				, m_attrs( new std::unordered_map<std::string, SerializedInterface *>() )
			{
			}

			BaseData::BaseData( Engine::TCMBase* oneBase )
				: m_classMetaName( new std::string() )
				, m_attrs( new std::unordered_map<std::string, SerializedInterface *>() )
			{
				if ( oneBase != nullptr )
					m_classMetaName->assign( oneBase->_GetMetaFactoryName() );

				oneBase->_Serialize();
				const std::unordered_map<std::string, SerializedInterface *>* baseMap = oneBase->_GetSerializedAttributes();

				for ( auto iter = baseMap->cbegin(); iter != baseMap->cend(); ++iter )
				{
					( *m_attrs )[iter->first] = iter->second->Duplicate();
				}
			}

			BaseData::~BaseData()
			{
				if ( m_attrs )
				{
					for ( auto iter = m_attrs->begin(); iter != m_attrs->end(); ++iter )
					{
						if ( iter->second )
						{
							delete iter->second;
							iter->second = nullptr;
						}
					}
					delete m_attrs;
					m_attrs = nullptr;
				}

				if ( m_classMetaName != nullptr )
				{
					delete m_classMetaName;
					m_classMetaName = nullptr;
				}
			}

			Engine::TCMBase* BaseData::ExtractSerialized() const
			{
				Engine::TCMBase* instance = Engine::MetaFactory::Create( *m_classMetaName );
				instance->_SetSerializedAttributes( m_attrs );
				return instance;
			}

			void BaseData::Fill( Engine::TCMBase* object )
			{
				if ( object == nullptr )
					return;
				object->_SetSerializedAttributes( m_attrs );
			}

			std::string BaseData::GetClassMetaName() const
			{
				if ( m_classMetaName != nullptr )
					return std::string( *m_classMetaName );
				return std::string( "" );
			}

			void BaseData::_SetClassMetaName( const std::string& metaName )
			{
				if ( !m_classMetaName )
					m_classMetaName = new std::string( metaName );
				else
					m_classMetaName->assign( metaName );
			}

			void BaseData::_AddSerializedVar( SerializedInterface* serializedInterface ) const
			{
				( *m_attrs )[serializedInterface->m_varName] = serializedInterface;
			}

			const std::unordered_map<std::string, SerializedInterface*>* BaseData::_GetSerializedAttributes() const
			{
				return m_attrs;
			}
		}
	}
}
