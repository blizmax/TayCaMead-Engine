#pragma once

#include "TCMSaveManager/BaseData.h"

#include "EngineClasses/TCMSceneObject.h"

namespace TCM
{
	namespace Editor
	{
		namespace Data
		{
			class ComponentData;
			class SceneData;

			/**
			 * \brief Base class for serializing scene object data
			 */
			class TCMSAVEMANAGER_DLL SceneObjectData : public BaseData
			{
			public:
				SceneObjectData();

				explicit SceneObjectData( Engine::TCMSceneObject* sceneObject, SceneData* scene, SceneObjectData* parent = nullptr );

				virtual ~SceneObjectData();;

				/**
				 * \brief Extracts serialized data to an instance of \c TCMSceneObject
				 * \return A new \c TCMSceneObject that is of type stored by \c BaseData
				 */
				Engine::TCMSceneObject* Extract() const;

				/**
				* \brief Fill \c TCMSceneObject with serialized data
				*/
				void Fill( Engine::TCMSceneObject* object );

				/**
				 * \brief Add a \c SceneObjectData to children list
				 * \param sceneObjectData _IN_ The \c SceneObjectData to be set as child
				 */
				void _AddChildSceneObject( SceneObjectData* sceneObjectData );

				/**
				 * \brief Add a \c ComponentData to component list
				 * \param componentData _IN_ The \c ComponentData to be set as component
				 */
				void _AddComponent( ComponentData* componentData );

				/**
				 * \brief Set the \c SceneData containing the \c SceneObjectData
				 * \param sceneData _IN_ The \c SceneData to be set
				 */
				void _SetScene( SceneData* sceneData );

				/**
				 * \brief Get the \c SceneData containing the \c SceneObjectData
				 * \return The \c SceneData containing the \c SceneObjectData
				 */
				SceneData* _GetScene() const;

				/**
				 * \brief Set the parent \c SceneObjectData
				 * \param sceneObjectData _IN_ The \c SceneObjectData to be set
				 */
				void _SetParentSceneObject( SceneObjectData* sceneObjectData );

				/**
				 * \brief Get the parent \c SceneObjectData
				 * \return The parent \c SceneObjectData
				 */
				SceneObjectData* _GetParentSceneObject() const;

				/**
				 * \brief Get every \c SceneObjectData child of this \c SceneObjectData
				 * \return Const pointer to the vector containing every \c SceneObjectData child of this \c SceneObjectData
				 */
				const std::vector<SceneObjectData *>* _GetChildren() const;

				/**
				 * \brief Get every \c ComponentData of this \c SceneObjectData
				 * \return Const pointer to the vector containing every \c ComponentData of this \c SceneObjectData
				 */
				const std::vector<ComponentData *>* _GetComponents() const;

			protected:
				/**
				 * \brief The \c SceneData containing this \c SceneObjectData
				 */
				SceneData* m_scene = nullptr;

				/**
				 * \brief The parent \c SceneObjectData
				 */
				SceneObjectData* m_parent = nullptr;

				/**
				 * \brief Vector of every child \c SceneObjectData
				 */
				std::vector<SceneObjectData *>* m_children;

				/**
				 * \brief Vector of every \c ComponentData of this \c SceneObjectData
				 */
				std::vector<ComponentData *>* m_components;
			};
		}
	}
}
