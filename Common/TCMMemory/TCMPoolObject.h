#pragma once

#include "TCMMemory/MemoryDefines.h"

namespace TCM
{
	namespace Engine
	{
		namespace Memory
		{
			/**
			 * \brief Base class for every "poolable" object
			 */
			class MEMORY_ALWAYS_EXPORT TCMPoolObject
			{
			public:
				TCMPoolObject() = default;
				virtual ~TCMPoolObject() = default;

				/**
				 * \brief Pseudo-constructor used instanciated by pool
				 */
				virtual void TCMPoolCreate() = 0;

				/**
				 * \brief Pseudo-destructor used when released by pool
				 */
				virtual void TCMPoolRelease() = 0;

				/**
				 * \brief Set the ID of this object
				 * \param id The new ID to be set
				 */
				void TCMSetID( const uint32_t& id )
				{
					m_id = id;
				};

				/**
				 * \brief Get the curent ID of this object
				 * \return The object's ID
				 */
				uint32_t GetID() const
				{
					return m_id;
				};

			private:
				/**
				 * \brief Unique id for each object of same type
				 */
				uint32_t m_id = 0;
			};
		}
	}
}
