#pragma once

#include "TCMSaveManager/SaveManagerDefines.h"

#include <string>
#include <unordered_map>

#include "TCMMemory/TCMSerializationContainer.hpp"

namespace TCM
{
	namespace Engine
	{
		class TCMBase;
	}

	namespace Editor
	{
		namespace Data
		{
			/**
			 * \brief Base class for serializing data
			 */
			class TCMSAVEMANAGER_DLL BaseData
			{
			public:
				BaseData();

				explicit BaseData( Engine::TCMBase* oneBase );

				virtual ~BaseData();

				/**
				 * \brief Extracts serialized data to an instance of \c TCMBase
				 * \return A new \c TCMBase that is of type stored by \c BaseData
				 */
				Engine::TCMBase* ExtractSerialized() const;

				/**
				* \brief Fill \c TCMBase with serialized 
				*/
				void Fill( Engine::TCMBase* object );

				/**
				 * \brief Get the contained class MetaName
				 * \return The MetaName of the class being stored
				 *		   Empty string is return if a problem occurs
				 */
				std::string GetClassMetaName() const;

				/**
				 * \brief Set the MetaName of the stored class
				 * \param metaName _IN_ The MetaName of the class
				 */
				void _SetClassMetaName( const std::string& metaName );

				/**
				 * \brief Add a SerializedVar to the attributes
				 * \param serializedInterface _IN_ The SerializedInterface to be added
				 */
				void _AddSerializedVar( SerializedInterface* serializedInterface ) const;

				/**
				 * \brief Get the SerializedAttributes of this \c BaseData
				 * \return The map of the \c BaseData attributes
				 */
				const std::unordered_map<std::string, SerializedInterface *>* _GetSerializedAttributes() const;

			protected:
				/**
				 * \brief The MetaName of the class
				 */
				std::string* m_classMetaName;

			private:
				/**
				 * \brief Map containing every serialized attributes of the stored class
				 */
				std::unordered_map<std::string, SerializedInterface *>* m_attrs;
			};
		}
	}
}
