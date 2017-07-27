#pragma once

#include <vector>
#include "TCMLogger/Logger.h"
#include "TCMMemory/MemoryDefines.h"

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief Class used to act as a event
		 * \tparam Args The arguments that will be taken by the functions triggered by this event
		 */
		template<typename... Args>
		class Event
		{
			/**
			 * \brief Base struct for manager one function of the event
			 */
			struct EventHandler
			{
				virtual ~EventHandler()
				{
				};
				virtual void operator()( Args ... args ) = 0;
			};

			/**
			 * \brief Concrete class for generic functions such as global or static functions
			 */
			struct EventHandlerGeneric : EventHandler
			{
				explicit EventHandlerGeneric( void ( *function )( Args ... ) )
					: pFunction( function )
				{
				};

				virtual ~EventHandlerGeneric()
				{
					// Nothing to do, we don't have the rights to delete pFunction.
				};

				bool operator==( const EventHandlerGeneric& other )
				{
					return pFunction == other.pFunction;
				}

				bool Compare( const EventHandlerGeneric& other )
				{
					return operator==( other );
				}

				void ( *pFunction )( Args ... );

				void operator()( Args ... args ) override
				{
					( *pFunction )( std::forward<Args>( args )... );
				};
			};

			/**
			 * \brief Concrete class for specific functions such as class methods
			 * \tparam TCaller Type of the instance that will call the function
			 */
			template<class TCaller>
			struct EventHandlerSpecific : EventHandler
			{
				explicit EventHandlerSpecific( TCaller* caller, void ( TCaller::*function )( Args ... ) )
					: pCaller( caller )
					, pFunction( function )
				{
				};

				virtual ~EventHandlerSpecific()
				{
					// Nothing to do, we don't have the rights to delete pCaller nor pFunction.
				};

				bool operator==( const EventHandlerSpecific<TCaller>& other )
				{
					return pCaller == other.pCaller && pFunction == other.pFunction;
				}

				bool Compare( const EventHandlerSpecific<TCaller>& other )
				{
					return operator==( other );
				}

				TCaller* pCaller;
				void ( TCaller::*pFunction )( Args ... );

				void operator()( Args ... args ) override
				{
					( pCaller ->* pFunction )( std::forward<Args>( args )... );
				};
			};

			/**
			 * \brief Concrete class for specific const functions such as class const methods
			 * \tparam TCaller Type of the instance that will call the function
			 */
			template<class TCaller>
			struct EventHandlerConstSpecific : EventHandler
			{
				explicit EventHandlerConstSpecific( const TCaller* caller, void ( TCaller::*function )( Args ... ) const )
					: pCaller( caller )
					, pFunction( function )
				{
				};

				virtual ~EventHandlerConstSpecific()
				{
					// Nothing to do, we don't have the rights to delete pCaller nor pFunction.
				};

				bool operator==( const EventHandlerConstSpecific<TCaller>& other )
				{
					return pCaller == other.pCaller && pFunction == other.pFunction;
				}

				bool Compare( const EventHandlerConstSpecific<TCaller>& other )
				{
					return operator==( other );
				}

				const TCaller* pCaller;
				void ( TCaller::*pFunction )( Args ... ) const;

				void operator()( Args ... args ) override
				{
					( pCaller ->* pFunction )( std::forward<Args>( args )... );
				};
			};

		public:
			Event() = default;

			~Event()
			{
				Clear();
			};

			/**
			 * \brief Directly calls Invoke()
			 * \param args The arguments sent to every functions of the event
			 */
			void MEMORY_ALWAYS_EXPORT operator()( Args ... args )
			{
				Invoke( std::forward<Args>( args )... );
			};

			/**
			 * \brief Calls every functions with the wanted arguments
			 * \param args The arguments sent to every functions of the event
			 */
			void MEMORY_ALWAYS_EXPORT Invoke( Args ... args )
			{
				for ( auto iter = m_vecFunctions.begin(); iter != m_vecFunctions.end(); ++iter )
				{
					( *iter )->operator()( std::forward<Args>( args )... );
				}
			};

			/**
			 * \brief Adds a class method to be called by this event
			 * \tparam TCaller The type of the instance that will call the function
			 * \param functionToAdd Method to be added
			 * \param caller Instance of the caller
			 */
			template<class TCaller>
			void MEMORY_ALWAYS_EXPORT Push( void ( TCaller::*functionToAdd )( Args ... ), TCaller* caller )
			{
#ifdef _DEBUG
				EventHandlerSpecific<TCaller> toBeCompared( caller, functionToAdd );

				auto iter = Find( toBeCompared );

				if ( iter != m_vecFunctions.end() )
				TCMWARNING( "Event::Push() - specific - adding twice" );
#endif
				m_vecFunctions.push_back( new EventHandlerSpecific<TCaller>( caller, functionToAdd ) );
			};

			/**
			 * \brief Adds a const class method to be called by this event
			 * \tparam TCaller The type of the instance that will call the function
			 * \param functionToAdd Const method to be added
			 * \param caller Const instance of the caller
			 */
			template<class TCaller>
			void MEMORY_ALWAYS_EXPORT Push( void ( TCaller::*functionToAdd )( Args ... ) const, const TCaller* caller )
			{
#ifdef _DEBUG
				EventHandlerConstSpecific<TCaller> toBeCompared( caller, functionToAdd );

				auto iter = Find( toBeCompared );

				if ( iter != m_vecFunctions.end() )
				TCMWARNING( "Event::Push() - specific - adding twice" );
#endif
				m_vecFunctions.push_back( new EventHandlerConstSpecific<TCaller>( caller, functionToAdd ) );
			};

			/**
			 * \brief Adds a function to be called by this event
			 * \param functionToAdd Function to be added
			 */
			void MEMORY_ALWAYS_EXPORT Push( void ( *functionToAdd )( Args ... ) )
			{
#ifdef _DEBUG
				EventHandlerGeneric toBeCompared( functionToAdd );

				auto iter = Find( toBeCompared );

				if ( iter != m_vecFunctions.end() )
				TCMWARNING( "Event::Push() - generic - adding twice" );
#endif
				m_vecFunctions.push_back( new EventHandlerGeneric( functionToAdd ) );
			};

			/**
			 * \brief Removes a class method from this event
			 * \tparam TCaller The type of the instance that was calling the function
			 * \param functionToRemove Method to be removed
			 * \param caller Instance of the caller
			 */
			template<class TCaller>
			void MEMORY_ALWAYS_EXPORT Pop( void ( TCaller::*functionToRemove )( Args ... ), TCaller* caller )
			{
				EventHandlerSpecific<TCaller> toBeCompared( caller, functionToRemove );

				auto iter = Find( toBeCompared );

				if ( iter != m_vecFunctions.end() )
				{
					delete *iter;
					m_vecFunctions.erase( iter );
				}
			};

			/**
			 * \brief Removes a const class method from this event
			 * \tparam TCaller The type of the instance that was calling the function
			 * \param functionToRemove Const method to be removed
			 * \param caller Const instance of the caller
			 */
			template<class TCaller>
			void MEMORY_ALWAYS_EXPORT Pop( void ( TCaller::*functionToRemove )( Args ... ) const, const TCaller* caller )
			{
				EventHandlerConstSpecific<TCaller> toBeCompared( caller, functionToRemove );

				auto iter = Find( toBeCompared );

				if ( iter != m_vecFunctions.end() )
				{
					delete *iter;
					m_vecFunctions.erase( iter );
				}
			};

			/**
			 * \brief Removes a function from this event
			 * \param functionToRemove Function to be removed
			 */
			void MEMORY_ALWAYS_EXPORT Pop( void ( *functionToRemove )( Args ... ) )
			{
				EventHandlerGeneric toBeCompared( functionToRemove );

				auto iter = Find( toBeCompared );

				if ( iter != m_vecFunctions.end() )
				{
					delete *iter;
					m_vecFunctions.erase( iter );
				}
			};

			/**
			 * \brief Remove all functions from this event
			 */
			void MEMORY_ALWAYS_EXPORT Clear()
			{
				for ( auto iter = m_vecFunctions.begin(); iter != m_vecFunctions.end(); ++iter )
				{
					delete *iter;
				}
				m_vecFunctions.clear();
			};

		private:
			/**
			 * \brief Finds a EventHandler in the function list
			 * \tparam TEventHandler The type of the EventHandler to be found
			 * \param eventHandler An equivalent of the EventHandler to be found
			 * \return Iterator on the vector of EventHandler's\n
			 *		   Returns end() if unable to find
			 */
			template<typename TEventHandler>
			typename std::vector<EventHandler *>::iterator Find( const TEventHandler& eventHandler )
			{
				for ( auto iter = m_vecFunctions.begin(); iter != m_vecFunctions.end(); ++iter )
				{
					TEventHandler* eventHandlerGeneric = dynamic_cast<TEventHandler *>(*iter);

					if ( eventHandlerGeneric && eventHandlerGeneric->Compare( eventHandler ) )
						return iter;
				}
				return m_vecFunctions.end();
			}

			/**
			 * \brief Finds a EventHandler in the function list
			 * \tparam TEventHandler The type of the EventHandler to be found
			 * \param eventHandler An equivalent of the EventHandler to be found
			 * \return Const_iterator on the vector of EventHandler's\n
			 *		   Returns cend() if unable to find
			 */
			template<typename TEventHandler>
			typename std::vector<EventHandler *>::const_iterator Find( const TEventHandler& eventHandler ) const
			{
				for ( auto iter = m_vecFunctions.cbegin(); iter != m_vecFunctions.cend(); ++iter )
				{
					const TEventHandler* eventHandlerGeneric = dynamic_cast<const TEventHandler *>(*iter);

					if ( eventHandlerGeneric && eventHandlerGeneric->Compare( eventHandler ) )
						return iter;
				}
				return m_vecFunctions.cend();
			}

			/**
			 * \brief Vector containing every functions to be called when invoking this event
			 */
			std::vector<EventHandler *> m_vecFunctions;
		};
	}
}
