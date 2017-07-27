#pragma once
#include <cstdint>

namespace TCM
{
	struct SerializedInterface;

	namespace Editor
	{
		class VarInspector;

		enum class VarType : uint8_t
		{
			BOOL = 0,
			INT,
			FLOAT,
			STRING,
			VEC2,
			VEC3,
			VEC4,
			COLOR_RGB,
			COLOR_RGBA,
			RIGIDBODY,
			COLLISIONSHAPE,
			STRING_VEC,
			NONE
		};

		class VarInspectorFactory abstract
		{
		public:
			static VarInspector* Create( SerializedInterface* var );
			virtual ~VarInspectorFactory() = default;
		protected:
			static VarInspectorFactory& GetInstance();
			VarType GetType( SerializedInterface* var ) const;
			VarInspector* CoreCreate( SerializedInterface* var );
			virtual VarInspector* CoreCreateInspector( VarType type, SerializedInterface* var ) = 0;
		};
	}
}
