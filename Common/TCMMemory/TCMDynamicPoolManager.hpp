#pragma once

#include <unordered_map>
#include <typeindex>
#include <queue>

#include "TCMPoolObject.h"

namespace TCM
{
	namespace Engine
	{
		namespace Memory
		{
			/**
			 * \brief Class used as a Dynamic Pool Manager
			 *		  Will manage objects using custom New() and Delete() instead of new and delete operators
			 *		  Supports derived class
			 * \tparam TObject The base class being contained by the pool
			 */
			template<class TObject>
			class TCMDynamicPoolManager
			{
				static_assert( std::is_base_of<TCMPoolObject, TObject>::value, "TObject must inherit from TCMPoolObject" );

			public:
				TCMDynamicPoolManager();
				virtual ~TCMDynamicPoolManager();

				/**
				 * \brief Creates an object
				 * \tparam TDerived The type of the object to be instanciated
				 * \return The new object
				 * \note Don't always call type constructor. Calls TCMPoolCreate(), but no Construct()
				 */
				template<class TDerived>
				TDerived* New();

				/**
				 * \brief Creates an object
				 * \tparam TDerived The type of the object to be instanciated
				 * \tparam Args The types of the arguments taken by the Construct()
				 * \param args _IN_ The arguments sent to Construct()
				 * \return The new object
				 * \note Don't always call type constructor. Calls TCMPoolCreate(), and Construct(args)
				 */
				template<class TDerived, typename... Args>
				TDerived* New( Args&&... args );

				/**
				 * \brief Releases object
				 * \param object _IN_ The object to be deleted
				 * \note Will never call type destructor. Calls TCMPoolRelease() only
				 */
				void Delete( TObject* object );

			private:
				/**
				 * \brief ID of the last object instanciated
				 */
				uint32_t m_idLastObject = 0;

				/**
				 * \brief Map of every re-usable objects
				 */
				std::unordered_map<std::type_index, std::queue<TObject *>> m_mapAvailable;

				/**
				 * \brief Get an available object
				 *		  Gets a valid one in the list, if not possible, calls operator new
				 * \tparam TDerived The type of the object to be returned
				 * \return An available object
				 */
				template<class TDerived>
				TDerived* GetAvailable();
			};

			template<class TObject>
			TCMDynamicPoolManager<TObject>::TCMDynamicPoolManager()
				: m_idLastObject( 0 )
			{
			}

			template<class TObject>
			TCMDynamicPoolManager<TObject>::~TCMDynamicPoolManager()
			{
				while ( !m_mapAvailable.empty() )
				{
					for ( auto iter = m_mapAvailable.begin(); iter != m_mapAvailable.end(); )
					//for ( std::pair<std::type_index, std::queue<TObject *>> onePair : m_mapAvailable )
					{
						std::queue<TObject *>& oneQueue = iter->second;

						while ( !oneQueue.empty() )
						{
							delete oneQueue.front();
							oneQueue.pop();
						}

						iter = m_mapAvailable.erase( iter );
					}
				}
			}

			template<class TObject>
			template<class TDerived>
			TDerived* TCMDynamicPoolManager<TObject>::New()
			{
				TDerived* object = GetAvailable<TDerived>();

				object->TCMSetID( ++m_idLastObject );
				object->TCMPoolCreate();
				return object;
			}

			template<class TObject>
			template<class TDerived, typename... Args>
			TDerived* TCMDynamicPoolManager<TObject>::New( Args&&... args )
			{
				TDerived* object = GetAvailable<TDerived>();

				object->TCMSetID( ++m_idLastObject );
				object->TCMPoolCreate();
				object->Construct( std::forward<Args>( args )... );
				return object;
			}

			template<class TObject>
			void TCMDynamicPoolManager<TObject>::Delete( TObject* object )
			{
				object->TCMPoolRelease();
				std::queue<TObject *>& queueTDerived = m_mapAvailable[std::type_index( typeid( *object ) )];
				queueTDerived.push( object );
			}

			template<class TObject>
			template<class TDerived>
			TDerived* TCMDynamicPoolManager<TObject>::GetAvailable()
			{
				std::queue<TObject *>& queueTDerived = m_mapAvailable[std::type_index( typeid( typename std::remove_pointer<TDerived>::type) )];

				if ( queueTDerived.size() > 0 )
				{
					TDerived* object = static_cast<TDerived *>(queueTDerived.front());
					queueTDerived.pop();
					return object;
				}

				return new TDerived();
			}
		}
	}
}
