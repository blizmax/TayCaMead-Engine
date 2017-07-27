#pragma once

#include "MathLib/Vec3.h"
#include "MathLib/Quat.h"
#include "TCMPhysicEngine/PhysicEngineBulletAPI/BulletAPI/btBulletDynamicsCommon.h"

#define TobtVector3(vector) btVector3(vector.x, vector.y, vector.z)
#define TobtQuaternion(quat) btQuaternion(quat.x, quat.y, quat.z, quat.w)

#define ToEngineVector3(vector) TCM::Engine::Math::Vec3(vector.x(), vector.y(), vector.z())
#define ToEngineQuaternion(quat) TCM::Engine::Math::Quat(quat.x(), quat.y(), quat.z(), quat.w())
