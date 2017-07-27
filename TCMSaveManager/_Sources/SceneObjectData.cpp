#include "TCMSaveManager/SceneObjectData.h"

#include "TCMSaveManager/SceneData.h"
#include "TCMSaveManager/ComponentData.h"

namespace TCM
{
	namespace Editor
	{
		namespace Data
		{
			SceneObjectData::SceneObjectData()
				: m_scene( nullptr )
				, m_parent( nullptr )
				, m_children( new std::vector<SceneObjectData *>() )
				, m_components( new std::vector<ComponentData *>() )
			{
			}

			SceneObjectData::SceneObjectData( Engine::TCMSceneObject* sceneObject, SceneData* scene, SceneObjectData* parent )
				: BaseData( sceneObject )
				, m_scene( scene )
				, m_parent( parent )
				, m_children( new std::vector<SceneObjectData *>() )
				, m_components( new std::vector<ComponentData *>() )
			{
				std::vector<Engine::TCMSceneObject *> vecSceneObjects = sceneObject->GetAllChildren();
				m_children->reserve( vecSceneObjects.size() );

				for ( auto iter = vecSceneObjects.cbegin(); iter != vecSceneObjects.cend(); ++iter )
				{
					m_children->push_back( new SceneObjectData( *iter, scene, this ) );
				}

				std::vector<Engine::TCMComponent *> vecComponent = sceneObject->GetAllComponents();
				m_components->reserve( vecComponent.size() );
				for ( auto iter = vecComponent.cbegin(); iter != vecComponent.cend(); ++iter )
				{
					m_components->push_back( new ComponentData( *iter, this ) );
				}
			}

			SceneObjectData::~SceneObjectData()
			{
				if ( m_children )
				{
					for ( auto iter = m_children->begin(); iter != m_children->end(); ++iter )
					{
						if ( *iter )
						{
							delete *iter;
							*iter = nullptr;
						}
					}
					delete m_children;
					m_children = nullptr;
				}

				if ( m_components )
				{
					for ( auto iter = m_components->begin(); iter != m_components->end(); ++iter )
					{
						if ( *iter )
						{
							delete *iter;
							*iter = nullptr;
						}
					}
					delete m_components;
					m_components = nullptr;
				}
			}

			Engine::TCMSceneObject* SceneObjectData::Extract() const
			{
				Engine::TCMSceneObject* sceneObject = dynamic_cast<Engine::TCMSceneObject *>(ExtractSerialized());

				for ( auto iter = m_children->begin(); iter != m_children->end(); ++iter )
				{
					sceneObject->AddChild( dynamic_cast<Engine::TCMSceneObject *>(( *iter )->Extract()) );
				}

				for ( auto iter = m_components->begin(); iter != m_components->end(); ++iter )
				{
					sceneObject->AddComponent( dynamic_cast<Engine::TCMComponent *>(( *iter )->Extract()) );
				}

				return sceneObject;
			}

			void SceneObjectData::Fill( Engine::TCMSceneObject* object )
			{
				if ( object == nullptr )
					return;

				BaseData::Fill( object );

				for ( auto iter = m_children->begin(); iter != m_children->end(); ++iter )
				{
					object->AddChild( dynamic_cast<Engine::TCMSceneObject *>(( *iter )->Extract()) );
				}

				for ( auto iter = m_components->begin(); iter != m_components->end(); ++iter )
				{
					object->AddComponent( dynamic_cast<Engine::TCMComponent *>(( *iter )->Extract()) );
				}
			}

			void SceneObjectData::_AddChildSceneObject( SceneObjectData* sceneObjectData )
			{
				m_children->push_back( sceneObjectData );
				sceneObjectData->_SetScene( m_scene );
				sceneObjectData->_SetParentSceneObject( this );
			}

			void SceneObjectData::_AddComponent( ComponentData* componentData )
			{
				m_components->push_back( componentData );
				componentData->_SetParentSceneObject( this );
			}

			void SceneObjectData::_SetScene( SceneData* sceneData )
			{
				m_scene = sceneData;
			}

			SceneData* SceneObjectData::_GetScene() const
			{
				return m_scene;
			}

			void SceneObjectData::_SetParentSceneObject( SceneObjectData* sceneObjectData )
			{
				m_parent = sceneObjectData;
			}

			SceneObjectData* SceneObjectData::_GetParentSceneObject() const
			{
				return m_parent;
			}

			const std::vector<SceneObjectData*>* SceneObjectData::_GetChildren() const
			{
				return m_children;
			}

			const std::vector<ComponentData*>* SceneObjectData::_GetComponents() const
			{
				return m_components;
			}
		}
	}
}
