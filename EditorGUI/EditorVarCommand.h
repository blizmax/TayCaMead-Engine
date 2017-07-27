#pragma once
#include "TCMMemory/TCMSerialization.h"
#include "EditorCommand.h"

namespace TCM
{
	namespace Editor
	{
		template<typename T>
		class EditorVarCommand : public EditorCommand
		{
		public:
			EditorVarCommand( SerializedInterface* var, const T& value ) : m_var( var )
			                                                             , m_value( value )
			                                                             , bIsPointer( var->IsPointer() )
			{
			}

			void Execute() override
			{
				m_before = m_var->GetPointerless<T>();

				if ( bIsPointer )
					m_var->ModifyVarEditorPointer( m_value );
				else
					m_var->ModifyVarEditorNoPointer( m_value );
			}

			void Undo() override
			{
				if ( bIsPointer )
					m_var->ModifyVarEditorPointer( m_before );
				else
					m_var->ModifyVarEditorNoPointer( m_before );
			}

		private:
			SerializedInterface* m_var;
			T m_value;
			T m_before;

			bool bIsPointer;
		};
	}
}
