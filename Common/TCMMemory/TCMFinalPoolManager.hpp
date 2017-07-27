#pragma once

#include <queue>

#include "TCMPoolObject.h"

namespace TCM
{
	namespace Engine
	{
		namespace Memory
		{
			/**
			 * \brief Class used as a Final Pool Manager
			 *		  Will manage objects using custom New() and Delete() instead of new and delete operators
			 *		  Does not support derived class
			 * \tparam TObject The type being contained by the pool
			 */
			template<class TObject>
			class MEMORY_ALWAYS_EXPORT TCMFinalPoolManager
			{
				static_assert( std::is_base_of<TCMPoolObject, TObject>::value, "TObject must inherit from TCMPoolObject" );
				static_assert( std::is_final<TObject>::value, "TObject must be final. Try using TCMDynamicPoolManager instead." );

			public:
				TCMFinalPoolManager();
				virtual ~TCMFinalPoolManager();

				/**
				 * \brief Creates an object
				 * \return The new object
				 * \note DOn't always call type construct. Calls TCMPoolCreate(), but no Construct()
				 */
				TObject* New();

				/**
				 * \brief Creates an object
				 * \tparam Args The types of the arguments taken by the Construct()
				 * \param args The arguments sent to Construct()
				 * \return The new object
				 * \note Don't always call type constructor. Calls TCMPoolCreate(), and Construct(args)
				 */
				template<typename... Args>
				TObject* New( Args&&... args );

				/**
				 * \brief Releases an object
				 * \param object The object to be deleted
				 */
				void Delete( TObject* object );

			private:
				/**
				 * \brief Queue of every re-usable objects
				 */
				std::queue<TObject *> m_queueAvailable;

				/**
				 * \brief Get an available object
				 *		  Gets a valid one in the list, if not possible, calls operator new
				 * \return An available object
				 */
				TObject* GetAvailable();
			};

			template<class TObject>
			TCMFinalPoolManager<TObject>::TCMFinalPoolManager()
			{
			}

			template<class TObject>
			TCMFinalPoolManager<TObject>::~TCMFinalPoolManager()
			{
				size_t length = m_queueAvailable.size();

				for ( int i = 0; i < length; ++i )
				{
					delete m_queueAvailable.front();
					m_queueAvailable.pop();
				}
			}

			template<class TObject>
			TObject* TCMFinalPoolManager<TObject>::New()
			{
				TObject* object = GetAvailable();

				object->PoolCreate();
				return object;
			}

			template<class TObject>
			template<typename... Args>
			TObject* TCMFinalPoolManager<TObject>::New( Args&&... args )
			{
				TObject* object = GetAvailable();

				object->PoolCreate();
				object->Construct( std::forward<Args>( args )... );
				return object;
			}

			template<class TObject>
			void TCMFinalPoolManager<TObject>::Delete( TObject* object )
			{
				object->PoolRelease();
				m_queueAvailable.push( object );
			}

			template<class TObject>
			TObject* TCMFinalPoolManager<TObject>::GetAvailable()
			{
				TObject* object;

				if ( m_queueAvailable.size() > 0 )
				{
					object = m_queueAvailable.front();
					m_queueAvailable.pop();
				}
				else
				{
					object = new TObject();
				}

				return object;
			}
		}
	}
}
