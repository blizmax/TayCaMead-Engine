#pragma once

namespace TCM
{
	namespace Engine
	{
		class TCMComponent;
	}

	namespace Editor
	{
		class Inspector;
		class BaseInspector;

		class ComponentInspector abstract
		{
		public:
			ComponentInspector( Inspector* inspector, Engine::TCMComponent* component );
			virtual ~ComponentInspector();
			Engine::TCMComponent* GetComponent() const;

			void Delete();
			void Enable( const bool value ) const;
			void UpdateFromValues() const;

		protected:
			BaseInspector* m_baseInspector = nullptr;
			Engine::TCMComponent* m_component = nullptr;
			Inspector* m_inspector = nullptr;
		};
	}
}
