#pragma once

#include "TCMSaveManager/BaseData.h"

#include "EngineClasses/TCMComponent.h"

namespace TCM
{
	namespace Editor
	{
		namespace Data
		{
			class SceneObjectData;

			/**
			 * \brief Base class for serializing component data
			 */
			class TCMSAVEMANAGER_DLL ComponentData : public BaseData
			{
			public:
				ComponentData();

				explicit ComponentData( Engine::TCMComponent* component, SceneObjectData* parent );

				virtual ~ComponentData();;

				/**
				 * \brief Extracts serialized data to an instance of \c TCMComponent
				 * \return A new \c TCMComponent that is of type stored by \c BaseData
				 */
				Engine::TCMComponent* Extract() const;

				/**
				 * \brief Get the parent \c SceneObjectData
				 * \return The parent \c SceneObjectData
				 */
				SceneObjectData* _GetParentSceneObject() const;

				/**
				 * \brief Set the parent \c SceneObjectData
				 * \param sceneObject _IN_ The \c SceneObjectData to be set as parent
				 */
				void _SetParentSceneObject( SceneObjectData* sceneObject );

			protected:
				/**
				 * \brief The parent SceneObjectData of this object
				 */
				SceneObjectData* m_parentSceneObject = nullptr;
			};
		}
	}
}
