#pragma once
#include "TCMPhysicEngine/PhysicEngineDLL.h"
#include "CollisionShapeData.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			/**
			* \brief \n A Collision Shape, allowing an object to be detected when colliding with another Collision Shape. \n
			* \remarks A Collision Shape has its shape never changed once created.
			*/
			typedef struct TCMPHYSICENGINE_DLL SCollisionShape
			{
				/**
				* \brief \n Default Constructor for Collision Shapes. \n
				* \note Do yourself a favor and use : \n
				* ->	\c SCollisionShape( \p const \c CollisionShapeData& \a info ) \n
				* \remarks This structure is supposed to be \b overriden, only providing basis for a Collision Shape.
				*/
				SCollisionShape() = default;

				/**
				* \brief \n Constructor used to create a Collision Shape with \c CollisionShapeData. \n
				* \param info _IN_ Info about the Collision Shape.
				*/
				explicit SCollisionShape( CollisionShapeData* info ) : m_collisionShapeInfo( info )
				{
				}

				/**
				* \brief \n Default destructor. \n
				* \remarks Don't forget to \b override it when derivating \c SCollisionShape !
				*/
				virtual ~SCollisionShape() = default;

				/**
				* \brief \n Gets the scale of the Collision Shape. \n
				* \return The value of the Collision Shape's scale in \c Vec3.
				*/
				virtual Math::Vec3 GetScale() const;

				/**
				* \brief  \n Sets the scale of the Collision Shape. \n
				* \param scale _IN_ The new scale of the Collision Shape.
				*/
				virtual void SetScale( Math::Vec3 scale );

				/**
				* \brief  \n Gets the offset of the Collision Shape.  \n
				* \return The value of the Collision Shape's offset in \c Vec3.
				*/
				virtual Math::Vec3 GetOffset() const;

				/**
				* \brief  \n Sets the offset of the Collision Shape. \n
				* \param offset _IN_ The new offset of the Collision Shape.
				*/
				virtual void SetOffset( Math::Vec3 offset );

				/**
				* \brief  \n Gets the Shape type of the Collision Shape. \n
				* \return The \c CollisionShapesType of the Collision Shape.
				* \remarks You \b CANNOT change the Collision Shape type, only getting it.
				*/
				virtual CollisionShapesType GetShapeType() const;

				/**
				* \brief  \n Gets all data from the Collision Shape. \n
				* \return A pointer to the \c CollisionShapeData data structure stated above.
				*/
				virtual CollisionShapeData* GetCollisionShapeInfo();

				/**
				* \brief  \n Sets \b all data from the Collision Shape. \n
				* \param bodyInfo _IN_ The new data structure.
				*/
				virtual void SetCollisionShapeInfo( CollisionShapeData* bodyInfo );

			protected:
				/**
				* \brief  \n The data structure for the Collision Shape. \n
				*/
				CollisionShapeData* m_collisionShapeInfo = nullptr;
			} CollisionShape;
		}
	}
}
