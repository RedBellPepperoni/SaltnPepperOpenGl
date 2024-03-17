#ifndef VERLETCONSTRAINT_H
#define VERLETCONSTRAINT_H
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine 
{
    class Constraint
    {
    public:
        Constraint() {}
        ~Constraint() {}
        virtual void Apply() const = 0;
    };

    // Constrain particle to specific point
    class PointConstraint : public Constraint 
    {
        Vector3* m_pA;
        Vector3 m_fixedPos;

    public:
        PointConstraint(Vector3* pa, const Vector3& fp) : m_pA(pa), m_fixedPos(fp) {}
        ~PointConstraint() {};
        void Apply() const;
        const Vector3& getPos() const { return m_fixedPos; }
        void setPos(const Vector3& np) { m_fixedPos = np; }
    };

    // Constrain two particles to a specific distance from each other
    class RodConstraint : public Constraint 
    {
        Vector3* m_pA, * m_pB;
        float m_restLen;
        float m_restLenSqr;

    public:
        RodConstraint(Vector3* pa, Vector3* pb, float rl) : m_pA(pa), m_pB(pb), m_restLen(rl), m_restLenSqr(rl* rl) {}
        ~RodConstraint() {};
        void Apply() const;
    };

    // Constrain particle in some axes but allow movement in others
    enum ConstrainAxis { CX_AXIS = 1, CY_AXIS = 2, CZ_AXIS = 4 };
    class SlideConstraint : public Constraint 
    {
        Vector3* m_pA;
        Vector3 m_fixedPos;
        ConstrainAxis m_constrainAxis;

    public:
        SlideConstraint(Vector3* pa, const Vector3& fp, ConstrainAxis axis) : m_pA(pa), m_fixedPos(fp), m_constrainAxis(axis) {}
        ~SlideConstraint() {};
        void Apply() const;
    };

    inline void PointConstraint::Apply() const { *m_pA = m_fixedPos; }

    inline void RodConstraint::Apply() const
    {
        Vector3 delta = *m_pB - *m_pA;
#if 0
        float deltaLen = delta.length();
        float halfDiff = 0.5f * (deltaLen - m_restLen) / deltaLen;
#else
        // Faster because no sqrt, but a bit less accurate
        float halfDiff = -(m_restLenSqr / (LengthSquared(delta) + m_restLenSqr) - 0.5f);
#endif
        delta *= halfDiff;
        *m_pA += delta;
        *m_pB -= delta;
    }

    inline void SlideConstraint::Apply() const
    {
        if (m_constrainAxis & CX_AXIS) { (*m_pA)[0] = m_fixedPos[0]; }
        if (m_constrainAxis & CY_AXIS) { (*m_pA)[1] = m_fixedPos[1]; }
        if (m_constrainAxis & CZ_AXIS) { (*m_pA)[2] = m_fixedPos[2]; }
    }
}

#endif // !VERLETCONSTRAINT_H
