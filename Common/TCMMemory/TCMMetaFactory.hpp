#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <mutex>

#include "TCMMemory/MemoryDefines.h"

namespace TCM
{
	namespace Engine
	{
		class TCMBase;

		namespace Memory
		{
			class TCMPoolObject;

			/**
			 * \brief Class Factory used to instanciate any class from using a Key
			 * \tparam TInterface The base type to be instanciated with Keys
			 * \tparam TKey The type of the Key associated to the type
			 */
			template<class TInterface, typename TKey = std::string, typename std::enable_if<std::is_base_of<TCMBase, TInterface>::value>::type* = nullptr>
			class MEMORY_DLL TCMMetaFactory
			{
			public:
				typedef const std::map<const std::string, std::string>*& MapClassMembersType;
				typedef TKey Key;
				typedef TInterface* pType;

				~TCMMetaFactory()
				{
					if ( m_registry )
					{
						delete m_registry;
						m_registry = nullptr;
					}

					if ( m_vecFuncMapClassMembers )
					{
						for ( auto iter = m_vecFuncMapClassMembers->begin(); iter != m_vecFuncMapClassMembers->end(); ++iter )
						{
							MapClassMembersType mapClassMembers = ( *iter )();

							if ( mapClassMembers != nullptr )
							{
								delete mapClassMembers;
								mapClassMembers = nullptr;
							}
						}
						delete m_vecFuncMapClassMembers;
						m_vecFuncMapClassMembers = nullptr;
					}
				}

			protected:
				/**
				 * \brief Gets the instance templated of the class
				 * \return The unique instance of the TCMMetaFactory
				 * \note Should not be called manually
				 */
				static TCMMetaFactory<TInterface, TKey>& GetInstance()
				{
					static std::unique_ptr<TCMMetaFactory<TInterface, TKey>> m_instance;
					static std::once_flag m_onceFlag;

					std::call_once( m_onceFlag,
					                []()
					                {
						                m_instance.reset( new TCMMetaFactory<TInterface, TKey>() );
					                }
					);
					return *( m_instance.get() );
				}

			public:
				/**
				 * \brief Defines a pair for future instanciation of template using a Key
				 * \tparam TDerived Type to be defined
				 * \param key _IN_ The Key which will be used to instanciate of type TDerived
				 * \return True if new pair was inserted, false otherwise
				 */
				template<typename TDerived>
				static typename std::enable_if<std::is_base_of<TInterface, TDerived>::value, bool>::type Define( const Key& key )
				{
					return GetInstance().template CoreDefine<TDerived>( key );
				}

				/**
				 * \brief Creates a instance of an object using the associated Key
				 * \param key _IN_ The key used to instanciate of the associated type
				 * \return The new instance of the object of associated type
				 */
				static TInterface* Create( const Key& key )
				{
					return GetInstance().CoreCreate( key );
				}

				/**
				 * \brief Get the list of every keys for types iheriting from \p TDerived
				 * \tparam TDerived Base type used to get derived type
				 * \return A vector of keys for types inheriting from \p TDerived
				 */
				template<typename TDerived, typename std::enable_if<std::is_base_of<TInterface, TDerived>::value>::type* = nullptr>
				static std::vector<Key> GetDerivedTypes()
				{
					return GetInstance().template CoreGetDerivedTypes<TDerived>();
				}

			private:
				typedef pType ( *Instanciator )();

				TCMMetaFactory()
					: m_registry( new Registry() )
					, m_vecFuncMapClassMembers( new std::vector<MapGetter>() )
				{
				}

				/**
				 * \brief Defines a pair for future instanciation of template using a Key
				 * \tparam TDerived Type to be defined
				 * \param key _IN_ The Key which will be used to instanciate of type TDerived
				 * \return True if new pair was inserted, false otherwise
				 */
				template<typename TDerived>
				typename std::enable_if<std::is_base_of<TInterface, TDerived>::value, bool>::type CoreDefine( const Key& key )
				{
					bool bOk = m_registry->insert(
						std::make_pair(
							key,
							[]() -> pType
							{
								return TDerived::_FactoryConstructor();
							}
						)
					).second;

					if ( bOk )
						m_vecFuncMapClassMembers->push_back( &TDerived::_GetStaticMapClassMembers );

					return bOk;
				}

				/**
				 * \brief Creates a instance of an object using the associated Key
				 * \param key _IN_ The key used to instanciate of the associated type
				 * \return The new instance of the object of associated type
				 */
				pType CoreCreate( const Key& key )
				{
					typename Registry::const_iterator iter = m_registry->find( key );

					if ( iter == m_registry->end() )
					{
						// TODO: TCMERROR
						throw std::invalid_argument( "Factory::Create() - Key not registered" );
					}

					return iter->second();
				}

				/**
				 * \brief Get the list of every keys for types iheriting from \p TDerived
				 * \tparam TDerived Base type used to get derived type
				 * \return A vector of keys for types inheriting from \p TDerived
				 */
				template<typename TDerived, typename std::enable_if<std::is_base_of<TInterface, TDerived>::value>::type* = nullptr>
				std::vector<Key> CoreGetDerivedTypes() const
				{
					std::vector<Key> vecDerivedTypes;

					for ( typename Registry::const_iterator iter = m_registry->cbegin(); iter != m_registry->cend(); ++iter )
					{
						TInterface* derived = iter->second();
						if ( dynamic_cast<TDerived*>(derived) )
							vecDerivedTypes.push_back( iter->first );
						derived->_ClassDestructor( derived );
					}

					return vecDerivedTypes;
				}

				typedef std::map<Key, Instanciator> Registry;

				/**
				 * \brief The Registry containing the association Key and Instanciator
				 */
				Registry* m_registry = nullptr;

				typedef MapClassMembersType (*MapGetter)();
				std::vector<MapGetter>* m_vecFuncMapClassMembers = nullptr;
			};

			template MEMORY_ALWAYS_EXPORT class TCMMetaFactory<TCMBase>;
		}
	}
}
