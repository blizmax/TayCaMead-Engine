#pragma once

#include <string>
#include <map>
#include <memory>
#include <mutex>

#include "TCMMemory/MemoryDefines.h"

#include "TCMMemory/TCMSerializationContainer.hpp"

namespace TCM
{
	namespace Engine
	{
		namespace Memory
		{
			/**
			 * \brief Class Factory used to instanciate any SerializedVar from using a Key
			 * \tparam TKey The type of the Key associated to the type
			 */
			template<typename TKey = std::string>
			class MEMORY_DLL TCMSerializedFactory
			{
			public:
				typedef TKey Key;
				typedef SerializedInterface* pType;
				typedef pType ( *Instanciator )();

				~TCMSerializedFactory()
				{
					if ( m_registry )
					{
						delete m_registry;
						m_registry = nullptr;
					}
				}

				/**
				 * \brief Defines a pair for future instanciation of template using a Key
				 * \tparam T Type to be defined as SerializedVar
				 * \param key _IN_ The Key which will be used to instanciate of type T
				 * \return True if new pair was inserted, false otherwise
				 */
				template<typename T>
				static Key Define( const Key& key )
				{
					return GetInstance().CoreDefine<T>( key );
				}

				/**
				 * \brief Creates a instance of a SerializedVar using the associated Key
				 * \param key _IN_ The key used to instanciate of the associated type
				 * \return The new instance of the SerializedVar of associated type
				 * \warning Instanciate well-typed SerializedVar but does *NOT* initialize it
				 */
				static pType Create( const Key& key )
				{
					return GetInstance().CoreCreate( key );
				}

				/**
				 * \brief Creates a instance of a SerializedVar using the associated Key
				 * \tparam T The type used to instanciate using the typeid().name
				 * \return The new instance of the SerializedVar of associated type
				 * \warning Instanciate well-typed SerializedVar but does *NOT* initialize it
				 */
				template<typename T>
				static pType Create()
				{
					return GetInstance().CoreCreate( typeid( T).name() );
				}

			private:
				TCMSerializedFactory()
					: m_registry( new Registry() )
				{
				}

				/**
				 * \brief Gets the instance templated of the class
				 * \return The unique instance of the TCMSerializedFactory
				 * \note Should not be called manually
				 */
				static TCMSerializedFactory<TKey>& GetInstance()
				{
					static std::unique_ptr<TCMSerializedFactory<TKey>> m_instance;
					static std::once_flag m_onceFlag;

					std::call_once( m_onceFlag,
					                []()
					                {
						                m_instance.reset( new TCMSerializedFactory<TKey>() );
					                }
					);
					return *( m_instance.get() );
				}

				/**
				 * \brief Defines a pair for future instanciation of template using a Key
				 * \tparam T Type to be defined as SerializedVar
				 * \param key _IN_ The Key which will be used to instanciate of type T
				 * \return True if new pair was inserted, false otherwise
				 * \note Instanciate well-typed SerializedVar but does *NOT* initialize it
				 */
				template<typename T>
				Key CoreDefine( const Key& key )
				{
					m_registry->insert(
						std::make_pair(
							key,
							[]() -> pType
							{
								return new SerializedVar<T>( "" );
							}
						)
					);

					return key;
				}

				/**
				 * \brief Creates a instance of a SerializedVar using the associated Key
				 * \param key _IN_ The key used to instanciate of the associated type
				 * \return The new instance of the SerializedVar of associated type
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

				typedef std::map<Key, Instanciator> Registry;

				/**
				 * \brief The Registry containing the association Key and Instanciator
				 */
				Registry* m_registry = nullptr;
			};

			//// Examples
			/*
			SerializedInterface* boolean = MetaSerializedFactory::Create( "bool" );
			SerializedInterface* integer = MetaSerializedFactory::Create( "int" );
			SerializedInterface* floating = MetaSerializedFactory::Create( "float" );

			SerializedInterface* boolean2 = MetaSerializedFactory::Create<bool>();
			SerializedInterface* integer2 = MetaSerializedFactory::Create<int>();
			SerializedInterface* str = MetaSerializedFactory::Create<std::string *>();
			 */

			template MEMORY_ALWAYS_EXPORT class TCMSerializedFactory<std::string>;
		}
	}
}
