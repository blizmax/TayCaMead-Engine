#pragma once
#include "TCMRenderer/RHI/Generic/RenderData.h"
#include "ObjectLoader/ObjectLoaderDefines.h"

namespace TCM
{
	namespace Engine
	{
		namespace Resources
		{
			class TCMPrimitiveLoader final
			{
			public:

				/**
				* \brief Generates a Sphere
				* \param radius _IN_OPT_ Radius of the Sphere
				* \param nbLong _IN_OPT_ Number of longitude of the Sphere
				* \param nbLat _IN_OPT_ Number of latitude of the Sphere
				* \return The drawable Sphere as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				static OBJECTLOADER_DLL_EXPORT Graphics::AbstractMesh* GenSphere( float radius = 0.5f, int nbLong = 24, int nbLat = 16 );

				/**
				* \brief Generates a Tube
				* \param height _IN_OPT_ Height of the Tube
				* \param nbSides _IN_OPT_ Number of size of the Tube
				* \param bottomRadius1 _IN_OPT_ Intermediate bottom radius of the Tube
				* \param bottomRadius2 _IN_OPT_ End bottom radius of the Tube
				* \param topRadius1 _IN_OPT_ Intermediate top radius of the Tube
				* \param topRadius2 _IN_OPT_ End top radius of the Tube
				* \return The drawable Tube as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				static OBJECTLOADER_DLL_EXPORT Graphics::AbstractMesh* GenTube( float height = 1.0f, int nbSides = 24,
				                                                                float bottomRadius1 = 0.5f, float bottomRadius2 = .15f,
				                                                                float topRadius1 = .5f, float topRadius2 = .15f );

				/**
				* \brief Generates a Cone
				* \param height _IN_OPT_ Height of the Cone
				* \param nbSides _IN_OPT_ Number of size of the Cone
				* \param bottomRadius _IN_OPT_ Bottom radius of the Cone
				* \param topRadius _IN_OPT_ Top radius of the Cone
				* \return The drawable Cone as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				static OBJECTLOADER_DLL_EXPORT Graphics::AbstractMesh* GenCone( float height = 1.0f, int nbSides = 18, float bottomRadius = .25f, float topRadius = .05f );

				/**
				* \brief Generates a Torus
				* \param radius1 _IN_OPT_ Outer Radius of the Torus
				* \param radius2 _IN_OPT_ Inner Radius of the Torus
				* \param nbRadSeg _IN_OPT_ Number of segment of the Torus
				* \param nbSides _IN_OPT_ Number of sides for each segment of the Torus
				* \return The drawable Torus as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				static OBJECTLOADER_DLL_EXPORT Graphics::AbstractMesh* GenTorus( float radius1 = 1.0f, float radius2 = 0.3f, int nbRadSeg = 24, int nbSides = 18 );

				/**
				* \brief Generates a Capsule
				* \param height _IN_OPT_ Height of the Capsule
				* \param nbSides _IN_OPT_ Number of sides of the Capsule
				* \param radius _IN_OPT_ Radius of the Capsule
				* \return The drawable Capsule as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				static OBJECTLOADER_DLL_EXPORT Graphics::AbstractMesh* GenCapsule( float height = 1.0f, float radius = 0.5f, int nbSides = 24 );

				/**
				* \brief Generates a Plane
				* \param length _IN_OPT_ Length of the Plane
				* \param width _IN_OPT_ Width of the Plane
				* \param resX _IN_OPT_ X resolution, must be 2 or greater
				* \param resZ _IN_OPT_ Z resolution, must be 2 or greater
				* \return The drawable Plane as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				static OBJECTLOADER_DLL_EXPORT Graphics::AbstractMesh* GenPlane( float length = 1.0f, float width = 1.0f, int resX = 2, int resZ = 2 );

				/**
				* \brief Generates a Box
				* \param width _IN_OPT_ Width of the Box
				* \param height _IN_OPT_ Height of the Box
				* \param length _IN_OPT_ Length of the Box
				* \return The drawable Box as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				static OBJECTLOADER_DLL_EXPORT Graphics::AbstractMesh* GenBox( float width = 1.0f, float height = 1.0f, float length = 1.0f );
			private:
				/**
				* \brief Get the unique instance of the TCMPrimitiveLoader
				* \return A reference of the TCMPrimitiveLoader
				*/
				static TCMPrimitiveLoader& GetInstance();
				/**
				* \brief Generates a Sphere
				* \param radius _IN_ Radius of the Sphere
				* \param rings _IN_ Number of longitude of the Sphere
				* \param slices _IN_ Number of latitude of the Sphere
				* \return The drawable Sphere as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				Graphics::AbstractMesh* CoreGenSphere( const float& radius, const int rings, const int slices ) const;

				/**
				* \brief Generates a Tube
				* \param height _IN_ Height of the Tube
				* \param nbSides _IN_ Number of size of the Tube
				* \param bottomRadius1 _IN_ Intermediate bottom radius of the Tube
				* \param bottomRadius2 _IN_ End bottom radius of the Tube
				* \param topRadius1 _IN_ Intermediate top radius of the Tube
				* \param topRadius2 _IN_ End top radius of the Tube
				* \return The drawable Tube as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				Graphics::AbstractMesh* CoreGenTube( float height, int nbSides, float bottomRadius1, float bottomRadius2,
				                                     float topRadius1, float topRadius2 ) const;

				/**
				* \brief Generates a Cone
				* \param height _IN_ Height of the Cone
				* \param nbSides _IN_ Number of size of the Cone
				* \param bottomRadius _IN_ Bottom radius of the Cone
				* \param topRadius _IN_ Top radius of the Cone
				* \return The drawable Cone as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				Graphics::AbstractMesh* CoreGenCone( float height, int nbSides, float bottomRadius, float topRadius ) const;

				/**
				* \brief Generates a Torus
				* \param radius1 _IN_ Outer Radius of the Torus
				* \param radius2 _IN_ Inner Radius of the Torus
				* \param nbRadSeg _IN_ Number of segment of the Torus
				* \param nbSides _IN_ Number of sides for each segment of the Torus
				* \return The drawable Torus as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				Graphics::AbstractMesh* CoreGenTorus( float radius1, float radius2, int nbRadSeg, int nbSides ) const;

				/**
				* \brief Generates a Capsule
				* \param height _IN_ Height of the Capsule
				* \param nbSides _IN_ Number of sides of the Capsule
				* \param radius _IN_ Radius of the Capsule
				* \return The drawable Capsule as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				Graphics::AbstractMesh* CoreGenCapsule( float height, float radius, int nbSides ) const;

				/**
				* \brief Generates a Plane
				* \param length _IN_ Length of the Plane
				* \param width _IN_ Width of the Plane
				* \param resX _IN_ X resolution, must be 2 or greater
				* \param resZ _IN_ Z resolution, must be 2 or greater
				* \return The drawable Plane as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				Graphics::AbstractMesh* CoreGenPlane( float length, float width, int resX, int resZ ) const;

				/**
				* \brief Generates a Box
				* \param width _IN_OPT_ Width of the Box
				* \param height _IN_OPT_ Height of the Box
				* \param length _IN_OPT_ Length of the Box
				* \return The drawable Box as a TCMComponent\n
				*		   Returns nullptr if unable to create valid mesh
				*/
				Graphics::AbstractMesh* CoreGenBox( float width = 1.0f, float height = 1.0f, float length = 1.0f ) const;

				void ComputeTangents( const std::vector<Graphics::VertexData*>& vecFragmentVertexObject, const std::vector<Graphics::Fragment*>& vecFragmentObject ) const;
			};
		}
	}
}
