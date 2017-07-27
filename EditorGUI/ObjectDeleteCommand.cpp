#include "ObjectDeleteCommand.h"
#include "EngineClasses/TCMSceneObject.h"

namespace TCM
{
	namespace Editor
	{
		ObjectDeleteCommand::ObjectDeleteCommand( Engine::TCMSceneObject* object )
			: m_objectDeleted( object )
			, m_oldParent( object->GetParent() )
			, m_position( -1 )
		{
		}

		ObjectDeleteCommand::~ObjectDeleteCommand()
		{
			if ( m_objectDeleted->GetParent() == nullptr )
				Engine::TCMSceneObject::Release( m_objectDeleted );
		}

		void ObjectDeleteCommand::Execute()
		{
			m_position = m_oldParent->GetChildIndex( m_objectDeleted );
			m_oldParent->RemoveChild( m_objectDeleted );
		}

		void ObjectDeleteCommand::Undo()
		{
			m_oldParent->AddChild( m_objectDeleted, m_position );
		}
	}
}
