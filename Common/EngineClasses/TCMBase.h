#pragma once

#include <string>
#include <unordered_map>

#include "EngineClassesDefines.h"

#include "TCMMemory/TCMSerialization.h"
#include "TCMMemory/TCMMetaFactory.hpp"
#include "TCMMemory/TCMSerializedFactory.hpp"

#include "TCMMemory/TCMPoolObject.h"

namespace TCM
{
	namespace Engine
	{
		typedef Memory::TCMSerializedFactory<std::string> MetaSerializedFactory;

		/**
		 * \brief Base class for every game TCM classes
		 */
		class ENGINECLASSES_DLL_EXPORT TCMBase
		{
		public:
			TCMBase();
			virtual ~TCMBase();

			/**
			 * \brief Get a user readable version of the class
			 * \return A string formated version of the class
			 */
			virtual std::string ToString() const;

			/**
			 * \brief Set the name of the instance
			 * \param name _IN_ New name given to the instance
			 */
			void SetName( const std::string& name ) const;

			/**
			 * \brief Get the name of the instance
			 * \return The name of the instance
			 */
			std::string GetName() const;

			/**
			 * \brief Set if the instance should be enabled or not
			 * \param bEnabled _IN_ The new state of the instance
			 * \param callback _IN_OPT_ Launch the callback or not
			 */
			virtual void SetEnabled( const bool bEnabled, const bool callback = true );

			/**
			 * \brief Get if the instance is enabled or not
			 * \return True if the object is enabled, false otherwise
			 */
			bool IsEnabled() const;

			/**
			 * \brief Size of the object
			 * \return The size in bytes of the instance
			 * \note Acts like sizeof() using polymorphism
			 */
			inline size_t SizeOf() const;

			/**
			 * \brief Callback function
			 */
			virtual void OnEnable();

			/**
			 * \brief Callback function
			 */
			virtual void OnDisable();

		private:
			/**
			 * \brief Name of the object
			 */
			std::string* m_name;

			/**
			 * \brief Is the instance actually enabled or not
			 * \note Default: true
			 */
			bool m_bEnabled;

#pragma region Serialization
		public:
#pragma region _Serialize() functions
			/**
			 * \brief Function used for serializing variables
			 * \note Overriden if serialized variables
			 */
			virtual void _Serialize()
			{
				m_sizeInBytes = sizeof( *this );
				_Serialize_Editable();
				_Serialize_Readonly();
				_Serialize_Hidden();
			};

			virtual void _Serialize_Editable()
			{
				_SerializeForEach( make_tuple( TCM_SERIALIZED_VAR_EDITABLE( m_name ) ) );
			}

			virtual void _Serialize_Readonly()
			{
				// Do nothing
			}

			virtual void _Serialize_Hidden()
			{
				// Do nothing
			}
#pragma endregion

#pragma region _CreateMapClassMembers() functions
			/**
			 * \brief Create the map containing every class member being serialized
			 * \return The map of every serialized class member
			 */
			static std::map<const std::string, std::string>* _CreateMapClassMembers()
			{
				std::map<const std::string, std::string>* mapClassMember = new std::map<const std::string, std::string>();
				std::map<const std::string, std::string>* mapClassMemberEditable = _CreateMapClassMembers_Editable();
				mapClassMember->insert( mapClassMemberEditable->begin(), mapClassMemberEditable->end() );
				delete mapClassMemberEditable;
				std::map<const std::string, std::string>* mapClassMemberReadonly = _CreateMapClassMembers_Readonly();
				mapClassMember->insert( mapClassMemberReadonly->begin(), mapClassMemberReadonly->end() );
				delete mapClassMemberReadonly;
				std::map<const std::string, std::string>* mapClassMemberHidden = _CreateMapClassMembers_Hidden();
				mapClassMember->insert( mapClassMemberHidden->begin(), mapClassMemberHidden->end() );
				delete mapClassMemberHidden;
				return mapClassMember;
			};

			static std::map<const std::string, std::string>* _CreateMapClassMembers_Editable()
			{
				std::map<const std::string, std::string>* mapClassMember = new std::map<const std::string, std::string>();
				std::initializer_list<std::pair<const std::string, std::string>> initList = { TCM_GET_PAIR_NAME_TYPE( m_name ) };
				mapClassMember->insert( initList );
				return mapClassMember;
			}

			static std::map<const std::string, std::string>* _CreateMapClassMembers_Readonly()
			{
				std::map<const std::string, std::string>* mapClassMember = new std::map<const std::string, std::string>();
				return mapClassMember;
			}

			static std::map<const std::string, std::string>* _CreateMapClassMembers_Hidden()
			{
				std::map<const std::string, std::string>* mapClassMember = new std::map<const std::string, std::string>();
				return mapClassMember;
			}

#pragma endregion

			/**
			 * \brief Get the map of every class members
			 * \return The map of class members
			 */
			virtual const std::map<const std::string, std::string>*& _GetMapClassMembers() const
			{
				return _mapClassMembers;
			}

			/**
			 * \brief Get the map of every class members
			 * \return The map of class members
			 */
			static const std::map<const std::string, std::string>*& _GetStaticMapClassMembers()
			{
				return _mapClassMembers;
			}

			/**
			 * \brief Creates a class of the same type as the caller
			 * \return The well-typed new TCMBase
			 */
			virtual TCMBase* _ClassConstructor() const = 0;

			/**
			 * \brief Destructs a class of the same type as the caller
			 * \
			 */
			virtual void _ClassDestructor( TCMBase* base ) const = 0;

			/**
			 * \brief Get the name which has been used when setting the concrete class in the MetaFactory
			 * \return The meta factory name of this class
			 */
			virtual std::string _GetMetaFactoryName() const = 0;

		public:
			/**
			 * \brief Get the Serialized Attributes of the instance
			 * \return A const pointer to the instance's serialized variables
			 */
			const std::unordered_map<std::string, SerializedInterface *>* _GetSerializedAttributes() const
			{
				return m_serializedVars;
			}

#pragma region _SetSerializedAttributes() functions
			/**
			 * \brief Unserialize the attributes from a similar map
			 * \param serializedMap _IN_ The map used to replace serialized variables
			 */
			void _SetSerializedAttributes( const std::unordered_map<std::string, SerializedInterface *>* serializedMap )
			{
				_SetSerializedAttributes_Editable( serializedMap );
				_SetSerializedAttributes_Readonly( serializedMap );
				_SetSerializedAttributes_Hidden( serializedMap );
			}

			virtual void _SetSerializedAttributes_Editable( const std::unordered_map<std::string, SerializedInterface *>* serializedMap )
			{
				_UnSerializeForEach( std::make_tuple( TCM_SERIALIZED_VAR_EDITABLE( m_name ) ), serializedMap );
			}

			virtual void _SetSerializedAttributes_Readonly( const std::unordered_map<std::string, SerializedInterface *>* serializedMap )
			{
				// Do nothing
			}

			virtual void _SetSerializedAttributes_Hidden( const std::unordered_map<std::string, SerializedInterface *>* serializedMap )
			{
				// Do nothing
			}
#pragma endregion

			/**
			 * \brief Tells wether the class has been metafactoried or not
			 */
			static bool _bHasBeenMetaFactoried;

			/**
			 * \brief Map containing every type for every variable contained 
			 */
			static const std::map<const std::string, std::string>* _mapClassMembers;

		protected:
#pragma region SerializeForEach
			/**
			 * \brief Serializes variables using their indexes as template parameter
			 * \tparam I Index to the actual item being serialized
			 * \tparam TArgs Types of all the items
			 * \return Returns void
			 * \note Behaves as a recursive
			 */
			template<size_t I = 0, typename... TArgs>
			typename std::enable_if<I == sizeof...(TArgs), void>::type _SerializeForEach( std::tuple<TArgs...>& )
			{
				// Do nothing
			};

			/**
			 * \brief Serializes variables using their indexes as template parameter
			 * \tparam I Index to the actual item being serialized
			 * \tparam TArgs Types of all the items
			 * \param serializedPairs _IN_ The pairs containing the serializable variables
			 * \return Returns void
			 * \note Behaves as a recursive
			 */
			template<size_t I = 0, typename... TArgs>
			typename std::enable_if<I < sizeof...(TArgs), void>::type _SerializeForEach( std::tuple<TArgs...>& serializedPairs )
			{
				_SerializeVariable( std::get<I>( serializedPairs ) );
				_SerializeForEach<I + 1, TArgs...>( serializedPairs );
			};
#pragma endregion

#pragma region UnSerializeForEach
			/**
			 * \brief Unserializes variables using their indexes as template parameter
			 * \tparam I Index to the actual item being unserialized
			 * \tparam TArgs Types of all the items
			 * \return Returns void
			 * \note Behaves as a recursive
			 */
			template<size_t I = 0, typename... TArgs>
			typename std::enable_if<I == sizeof...(TArgs), void>::type _UnSerializeForEach( std::tuple<TArgs...>&, const std::unordered_map<std::string, SerializedInterface *>* )
			{
				// Do nothing
			};

			/**
			 * \brief Unserializes variables using their indexes as template parameter
			 * \tparam I Index to the actual item being unserialized
			 * \tparam TArgs Types of all the items
			 * \param serializedPairs _IN_ The pairs containing the serializable variables
			 * \param serializedMap _IN_ The map containing the new serializable variables
			 * \return Returns void
			 * \note Behaves as a recursive
			 */
			template<size_t I = 0, typename... TArgs>
			typename std::enable_if<I < sizeof...(TArgs), void>::type _UnSerializeForEach( std::tuple<TArgs...>& serializedPairs, const std::unordered_map<std::string, SerializedInterface *>* serializedMap )
			{
				_UnSerializeVariable( std::get<I>( serializedPairs ), serializedMap );
				_UnSerializeForEach<I + 1, TArgs...>( serializedPairs, serializedMap );
			};
#pragma endregion

#pragma region GetSerializedConstructor
			/**
			 * \brief Get the basic constructor used when serializing
			 * \tparam T The type of the class being constructed
			 * \return A pointer to the template type
			 * \note Calls TCMDynamicPoolManager::Create<>() if T inherits from TCMPoolObject
			 */
			template<class T>
			static typename std::enable_if<std::is_base_of<Memory::TCMPoolObject, T>::value, TCMBase*>::type _GetSerializedConstructor()
			{
				return T::template Create<T>();
			}

			/**
			 * \brief Get the basic constructor used when serializing
			 * \tparam T The type of the class being constructed
			 * \return A pointer to the template type
			 * \note Calls TCMDynamicPoolManager::Create<>() if T inherits from TCMPoolObject
			 */
			template<class T>
			static typename std::enable_if<!std::is_base_of<Memory::TCMPoolObject, T>::value, TCMBase*>::type _GetSerializedConstructor()
			{
				return new T();
			}
#pragma endregion

#pragma region GetSerializedDestructor
			/**
			 * \brief Get the basic destructor used when serializing
			 * \tparam T The type of the class being destroyed
			 * \return A pointer to the template type
			 * \note Calls TCMDynamicPoolManager::Release<>() if T inherits from TCMPoolObject
			 */
			template<class T>
			static typename std::enable_if<std::is_base_of<Memory::TCMPoolObject, T>::value, void>::type _GetSerializedDestructor( T* object )
			{
				T::Release( object );
			}

			/**
			 * \brief Get the basic destructor used when serializing
			 * \tparam T The type of the class being destroyed
			 * \return A pointer to the template type
			 * \note Calls TCMDynamicPoolManager::Release<>() if T inherits from TCMPoolObject
			 */
			template<class T>
			static typename std::enable_if<!std::is_base_of<Memory::TCMPoolObject, T>::value, void>::type _GetSerializedDestructor( T* object )
			{
				delete object;
			}
#pragma endregion

			/**
			 * \brief Size of this object
			 * \notice Used for polymorphism, to replace sizeof()
			 */
			size_t m_sizeInBytes;

			/**
			 * \brief Unorderded map containing every serialized field of the object
			 */
			std::unordered_map<std::string, SerializedInterface *>* m_serializedVars;

		private:
			/**
			 * \brief Serialize one variable
			 * \tparam T The type of the variable being serialized
			 * \param serializedVar _IN_ The SerializedVar being serialized
			 * \note Variables are sent using TCM_SERIALIZED_VAR()
			 */
			template<typename T>
			void _SerializeVariable( SerializedVarClass<T>* serializedVar )
			{
				std::string key = serializedVar->m_varName;

				auto iter = m_serializedVars->find( key );
				if ( iter != m_serializedVars->end() )
				{
					iter->second->ModifyVar( serializedVar->Get() );
					delete serializedVar;
				}
				else
					( *m_serializedVars )[key] = serializedVar;
			}

			/**
			 * \brief Unserialize one variable
			 * \tparam T The type of the variable being unserialized
			 * \param serializedVar _IN_ The SerializedVar being unserialized
			 * \param serializedMap _IN_ The map containing every SerializedVar to be loaded
			 * \note Variables are sent using TCM_SERIALIZED_VAR()
			 */
			template<typename T>
			void _UnSerializeVariable( SerializedVarClass<T>* serializedVar, const std::unordered_map<std::string, SerializedInterface *>* serializedMap )
			{
				std::string key = serializedVar->m_varName;

				auto iter = m_serializedVars->find( key );
				if ( iter == m_serializedVars->end() )
					( *m_serializedVars )[key] = serializedVar;
				else
					delete serializedVar;

				//#if _DEBUG
				//				auto checkIter = serializedMap->find( key );
				//				if ( checkIter == serializedMap->end() )
				//				{
				//					// TODO: this is currently normal if loading from files
				//					TCMFAILURE( "TCMBase::_UnSerializeVariable() - sent serializedMap does not contain the serialized key" );
				//					return;
				//				}
				//#endif

				( *m_serializedVars )[key]->ModifyVar( serializedMap->at( key )->Get<T>() );
			}
#pragma endregion
		};

		typedef Memory::TCMMetaFactory<TCMBase> MetaFactory;
	}
}
