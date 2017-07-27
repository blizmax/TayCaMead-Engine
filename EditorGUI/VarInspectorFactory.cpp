#include "VarInspectorFactory.h"
#include <mutex>
#include "TCMMemory/TCMSerialization.h"
#include "MathLib/Vec2.h"
#include "MathLib/Vec3.h"
#include "VarInspector.h"
#include "MathLib/ColorRGB.h"
#include "MathLib/ColorRGBA.h"
#include "EngineClasses/TCMComponentRigidBody.h"

#if defined(TCM_QT_API)
#include "VarInspectorFactoryQT.h"
typedef TCM::Editor::QT::VarInspectorFactoryQT CustomVarFactory;
#endif

namespace TCM
{
	namespace Editor
	{
		VarInspector* VarInspectorFactory::Create( SerializedInterface* var )
		{
			return GetInstance().CoreCreate( var );
		}

		VarInspectorFactory& VarInspectorFactory::GetInstance()
		{
			static std::unique_ptr<VarInspectorFactory> m_instance;
			static std::once_flag m_onceFlag;

			std::call_once( m_onceFlag,
			                []()
			                {
				                m_instance.reset( new CustomVarFactory() );
			                }
			);
			return *( m_instance.get() );
		}

		VarType VarInspectorFactory::GetType( SerializedInterface* var ) const
		{
			size_t hashCode = std::hash<std::string>()( var->GetPointerlessTypeName() );
			if ( hashCode == std::hash<std::string>()( typeid( int).name() ) )
			{
				return VarType::INT;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( float).name() ) )
			{
				return VarType::FLOAT;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( bool).name() ) )
			{
				return VarType::BOOL;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( std::string).name() ) )
			{
				return VarType::STRING;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( Engine::Math::Vec2).name() ) )
			{
				return VarType::VEC2;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( Engine::Math::Vec3).name() ) )
			{
				return VarType::VEC3;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( Engine::Math::Vec4).name() ) )
			{
				return VarType::VEC4;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( Engine::Math::ColorRGB).name() ) )
			{
				return VarType::COLOR_RGB;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( Engine::Math::ColorRGBA).name() ) )
			{
				return VarType::COLOR_RGBA;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( Engine::Physics::CollisionShapeData).name() ) )
			{
				return VarType::COLLISIONSHAPE;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( Engine::Physics::RigidBodyData).name() ) )
			{
				return VarType::RIGIDBODY;
			}
			else if ( hashCode == std::hash<std::string>()( typeid( std::vector<std::string>).name() ) )
			{
				return VarType::STRING_VEC;
			}
			return VarType::NONE;
		}

		VarInspector* VarInspectorFactory::CoreCreate( SerializedInterface* var )
		{
			if ( var->GetEditorType() == SerializedInterface::HIDDEN )
				return nullptr;
			VarType type = GetType( var );
			if ( type == VarType::NONE )
				return nullptr;
			VarInspector* result = CoreCreateInspector( type, var );
			return result;
		}
	}
}
