#include "TCMRenderer/Gizmo/GizmoLine.h"

TCM::Debug::GizmoLine::GizmoLine( Engine::Math::Vec3 p1, Engine::Math::Vec3 p2, TCM::Engine::Math::ColorRGBA color ) :
                                                                                                                     m_p1( p1, color )
                                                                                                                     , m_p2( p2, color )
{
}
