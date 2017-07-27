#pragma once
#include "VarInspector.h"
#include <unordered_map>

namespace TCM
{
	namespace Engine
	{
		class TCMBase;
	}

	namespace Editor
	{
		class Inspector;

		class BaseInspector abstract
		{
		public:
			BaseInspector();
			BaseInspector( Engine::TCMBase* base );
			~BaseInspector();

			void UpdateFromValues();
			void SetBase( Engine::TCMBase* base );

		protected:

			void Init( Engine::TCMBase* base );

			std::vector<std::string> ParseVarPath( const std::string& name ) const;
			std::string ParseVarName( const std::string& name ) const;
			std::unordered_map<std::string, VarInspector*> m_inspectors;

			bool m_isSceneInspector = false;
		};
	}
}
