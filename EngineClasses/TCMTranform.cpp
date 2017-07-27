#include "EngineClasses\TCMTranform.h"

TCM::Engine::s_TCMTransform::s_TCMTransform( const s_TCMTransform& other )
	: position( other.position )
	, eulerAngles( other.eulerAngles )
	, scale( other.scale )
{
}

TCM::Engine::s_TCMTransform& TCM::Engine::s_TCMTransform::operator=( const s_TCMTransform& other )
{
	if ( this == &other )
		return *this;

	position = other.position;
	eulerAngles = other.eulerAngles;
	scale = other.scale;

	return *this;
}

void TCM::Engine::s_TCMTransform::Reset()
{
	position = Math::Vec3( 0.0f, 0.0f, 0.0f );
	eulerAngles = Math::Vec3( 0.0f, 0.0f, 0.0f );
	scale = Math::Vec3( 1.0f, 1.0f, 1.0f );
}
