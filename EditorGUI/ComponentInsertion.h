#pragma once
#include <vector>

namespace TCM
{
	namespace Engine
	{
		class TCMComponent;
	}

	namespace Editor
	{
		class Inspector;

		class ComponentInsertion
		{
		public:
			ComponentInsertion( Inspector* m_inspector );

			void CreateComponent( const std::string& name ) const;
		protected:
			std::vector<std::string> m_components;
			Inspector* m_inspector;
		};
	}
}
