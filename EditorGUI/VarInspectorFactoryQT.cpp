#include "VarInspectorFactoryQT.h"
#include "IntInspectorQT.h"
#include "FloatInspectorQT.h"
#include "BoolInspectorQT.h"
#include "StringInspectorQT.h"
#include "Vec2InspectorQT.h"
#include "Vec3InspectorQT.h"
#include "Vec4InspectorQT.h"
#include "ColorRGBInspectorQT.h"
#include "ColorRGBAInspectorQT.h"
#include "RigidDataInspectorQT.h"
#include "StringVecInspectorQT.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			VarInspector* VarInspectorFactoryQT::CoreCreateInspector( VarType type, SerializedInterface* var )
			{
				switch ( type )
				{
					case VarType::BOOL: return new BoolInspectorQT( var );
					case VarType::INT: return new IntInspectorQT( var );
					case VarType::FLOAT: return new FloatInspectorQT( var );
					case VarType::STRING: return new StringInspectorQT( var );
					case VarType::VEC2: return new Vec2InspectorQT( var );
					case VarType::VEC3: return new Vec3InspectorQT( var );
					case VarType::VEC4: return new Vec4InspectorQT( var );
					case VarType::COLOR_RGB: return new ColorRGBInspectorQT( var );
					case VarType::COLOR_RGBA: return new ColorRGBAInspectorQT( var );
					case VarType::RIGIDBODY: return new RigidDataInspectorQT( var );
					case VarType::STRING_VEC: return new StringVecInspectorQT( var );
						//case VarType::COLLISIONSHAPE : return new ColorRGBAInspectorQT( var );
					default: return nullptr;
				}
			}
		}
	}
}
