#include "MathLib/BoundingObjects.h"

using namespace isart;

BoundingObjects::Sphere BoundingObjects::GetBoundingSphere( const std::vector<isart::Vec3>& pointSet, const int maxId )
{
	if ( maxId < 0 )
		return Sphere();

	isart::Vec3 p = pointSet[maxId];
	Sphere sphere = GetBoundingSphere( pointSet, maxId - 1 );

	if ( p.DistanceTo( sphere.position ) > sphere.radius )
	{
		std::vector<isart::Vec3> boundarySet;
		boundarySet.push_back( p );
		sphere = GetMiniBoundingSphere( pointSet, maxId - 1, boundarySet );
	}

	return sphere;
}

bool BoundingObjects::IsSphereInFrustum( const float frustum[24], const Sphere& sphere )
{
	float x = sphere.position.x;
	float y = sphere.position.y;
	float z = sphere.position.z;
	for ( int i = 0; i < 6; i++ )
	{
		if ( frustum[i * 6] * x + frustum[i * 6 + 1] * y + frustum[i * 6 + 2] * z + frustum[i * 6 + 3] <= sphere.radius )
			return false;
	}
	return true;
}

BoundingObjects::Sphere BoundingObjects::GetMiniBoundingSphere( const std::vector<isart::Vec3>& pointSet, const int maxId, std::vector<isart::Vec3>& boundarySet )
{
	Sphere s;
	if ( maxId < 0 || boundarySet.size() == 3 )
	{
		switch ( boundarySet.size() )
		{
			case 1:
				s.position = boundarySet[0];
				break;
			case 2:
				s.position = ( boundarySet[0] + boundarySet[1] ) / 2;
				s.radius = boundarySet[0].DistanceTo( s.position );
				break;
			case 3:
			{
				isart::Vec3 list[3] = { boundarySet[0], boundarySet[1], boundarySet[2] };
				s = GetSphere( list );
			}
				break;
			default:
				break;
		}
		return s;
	}
	isart::Vec3 p = boundarySet[maxId];
	s = GetMiniBoundingSphere( pointSet, maxId - 1, boundarySet );
	if ( p.DistanceTo( s.position ) > s.radius )
	{
		boundarySet.push_back( p );
		s = GetMiniBoundingSphere( pointSet, maxId - 1, boundarySet );
	}

	return s;
}

BoundingObjects::Sphere BoundingObjects::GetSphere( const isart::Vec3 p[3] )
{
	Sphere s;
	s.position = ( p[0] + p[1] + p[3] ) / 3;
	for ( int i = 0; i < 3; i++ )
		if ( s.position.DistanceTo( p[i] ) > s.radius )
			s.radius = s.position.DistanceTo( p[i] );
	return s;
}
