#include "ObjectInsertCommand.h"
#include "EngineClasses/TCMSceneObject.h"

namespace TCM
{
	namespace Editor
	{
		ObjectInsertCommand::ObjectInsertCommand( Engine::TCMSceneObject* parent, Engine::TCMSceneObject* child, int position )
			: m_parent( parent )
			, m_child( child )
			, m_position( position )
		{
		}

		ObjectInsertCommand::~ObjectInsertCommand()
		{
			if ( m_child->GetParent() == nullptr )
				Engine::TCMSceneObject::Release( m_child );
		}

		void ObjectInsertCommand::Execute()
		{
			m_parent->AddChild( m_child, m_position );
		}

		void ObjectInsertCommand::Undo()
		{
			m_parent->RemoveChild( m_child );
		}
	}
}
