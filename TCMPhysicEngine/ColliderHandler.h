#pragma once
namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			struct Vec3;
			struct Vec2;
		}

		namespace Physics
		{
			enum class EAxis : unsigned char;

			class ColliderHandler abstract
			{
			public:
				virtual ~ColliderHandler() = default;
				virtual void UpdateTrigger(const bool trigger) = 0;
				virtual void UpdateCenter(const Math::Vec3& center) = 0;
			};

			class BoxColliderHandler abstract : ColliderHandler
			{
			public:
				virtual ~BoxColliderHandler() = default;
				virtual void UpdateExtends(const Math::Vec3& extends) = 0;
			};

			class SphereColliderHandler abstract : ColliderHandler
			{
			public:
				virtual ~SphereColliderHandler() = default;
				virtual void UpdateRadius(const float radius) = 0;
			};

			class PlaneColliderHandler abstract : ColliderHandler
			{
			public:
				virtual ~PlaneColliderHandler() = default;
				virtual void UpdateExtends(const Math::Vec2& extends) = 0;
			};

			class CylinderColliderHandler abstract : ColliderHandler
			{
			public:
				virtual ~CylinderColliderHandler() = default;
				virtual void UpdateRadius(const float radius) = 0;
				virtual void UpdateHeight(const float height) = 0;
			};


			class CapsuleColliderHandler abstract : ColliderHandler
			{
			public:
				virtual ~CapsuleColliderHandler() = default;
				virtual void UpdateRadius(const float radius) = 0;
				virtual void UpdateHeight(const float height) = 0;
				virtual void UpdateAxis(const EAxis axis) = 0;
			};
		}
	}
}
