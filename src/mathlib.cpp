/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "mathlib.h"

void VectorMA(const glm::vec3 &start, const float scale, const glm::vec3 &direction, glm::vec3 &dest)
{
    dest = start + direction * scale;
}

void VectorNormalize(glm::vec3 &vec)
{
    vec = glm::normalize(vec);
}

bool PlanesGetIntersectionPoint(const plane_t *plane1, const plane_t *plane2, const plane_t *plane3, glm::vec3 &out)
{
    glm::vec3 n1 = glm::normalize(plane1->normal);
    glm::vec3 n2 = glm::normalize(plane2->normal);
    glm::vec3 n3 = glm::normalize(plane3->normal);

    glm::vec3 n1n2 = glm::cross(n1, n2);
    glm::vec3 n2n3 = glm::cross(n2, n3);
    glm::vec3 n3n1 = glm::cross(n3, n1);

    float denom = glm::dot(n1, n2n3);

    out = {0, 0, 0};

    // check if the denominator is zero (which would mean that no intersection is to be found
    if (denom == 0.0f)
    {
        // no intersection could be found, return <0,0,0>
        return false;
    }

    // compute intersection point
    out += n2n3 * plane1->dist;
    out += n3n1 * plane2->dist;
    out += n1n2 * plane3->dist;
    out *= (1.0f / denom);

    return true;
}

float AngleMod(float val)
{
    if (val > 360)
        return val - 360;
    else if (val < 0)
        return val + 360;

    return val;
}

glm::mat4 R_RotateForEntity(glm::vec3 pos, glm::vec3 angles)
{
    glm::mat4 transform(1);
    transform = glm::translate(transform, pos);

    auto concatRotate = [&](float deg, float x, float y, float z) {
        glm::mat4 ident = glm::mat4(1);
        ident           = glm::rotate(ident, glm::radians(deg), glm::vec3(x, y, z));
        transform *= ident;
    };

    concatRotate(angles[1], 0, 0, 1);
    concatRotate(-angles[0], 0, 1, 0);
    concatRotate(angles[2], 1, 0, 0);

    return transform;
}

void plane_s::CalcSignBits()
{
    signbits = 0;

    for (int j = 0; j < 3; j++)
    {
        if (normal[j] < 0)
            signbits |= 1 << j;
    }
}

int plane_s::BoxOnPlaneSide(const glm::vec3 &emins, const glm::vec3 &emaxs)
{
    float dist1, dist2;
    int   sides;

    // fast axial cases
    if (type < 3)
    {
        if (dist <= emins[type])
            return 1;
        if (dist >= emaxs[type])
            return 2;
        return 3;
    }

    // general case
    switch (signbits)
    {
    case 0:
        dist1 = normal[0] * emaxs[0] + normal[1] * emaxs[1] + normal[2] * emaxs[2];
        dist2 = normal[0] * emins[0] + normal[1] * emins[1] + normal[2] * emins[2];
        break;
    case 1:
        dist1 = normal[0] * emins[0] + normal[1] * emaxs[1] + normal[2] * emaxs[2];
        dist2 = normal[0] * emaxs[0] + normal[1] * emins[1] + normal[2] * emins[2];
        break;
    case 2:
        dist1 = normal[0] * emaxs[0] + normal[1] * emins[1] + normal[2] * emaxs[2];
        dist2 = normal[0] * emins[0] + normal[1] * emaxs[1] + normal[2] * emins[2];
        break;
    case 3:
        dist1 = normal[0] * emins[0] + normal[1] * emins[1] + normal[2] * emaxs[2];
        dist2 = normal[0] * emaxs[0] + normal[1] * emaxs[1] + normal[2] * emins[2];
        break;
    case 4:
        dist1 = normal[0] * emaxs[0] + normal[1] * emaxs[1] + normal[2] * emins[2];
        dist2 = normal[0] * emins[0] + normal[1] * emins[1] + normal[2] * emaxs[2];
        break;
    case 5:
        dist1 = normal[0] * emins[0] + normal[1] * emaxs[1] + normal[2] * emins[2];
        dist2 = normal[0] * emaxs[0] + normal[1] * emins[1] + normal[2] * emaxs[2];
        break;
    case 6:
        dist1 = normal[0] * emaxs[0] + normal[1] * emins[1] + normal[2] * emins[2];
        dist2 = normal[0] * emins[0] + normal[1] * emaxs[1] + normal[2] * emaxs[2];
        break;
    case 7:
        dist1 = normal[0] * emins[0] + normal[1] * emins[1] + normal[2] * emins[2];
        dist2 = normal[0] * emaxs[0] + normal[1] * emaxs[1] + normal[2] * emaxs[2];
        break;
    default:
        dist1 = dist2 = 0; // shut up compiler
        assert(0);
        break;
    }

    if (isnan(dist2) || isnan(dist1))
        return 2;

    sides = 0;
    if (dist1 >= dist)
        sides = 1;
    if (dist2 < dist)
        sides |= 2;

    assert(sides != 0);

    return sides;
}

float *BoundingBox::Data()
{
    return &m_Mins.x;
}

BoundingBox::BoundingBox(const glm::vec3 pos, const BoundingBox &other)
{
    m_Mins = pos + other.m_Mins;
    m_Maxs = pos + other.m_Maxs;
}

BoundingBox::BoundingBox(const glm::vec3 &mins, const glm::vec3 &maxs)
{
    m_Mins = mins;
    m_Maxs = maxs;
}

BoundingBox::BoundingBox(const glm::vec3 size)
{
    m_Mins = {-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f};
    m_Maxs = {size.x * 0.5f, size.y * 0.5f, size.z * 0.5f};
}

BoundingBox::BoundingBox(const float scale)
{
    const float h = scale * 0.5f;

    m_Mins = {-h, -h, -h};
    m_Maxs = {h, h, h};
}

BoundingBox::BoundingBox(const short *minsmaxs)
{
    m_Mins = {(float)minsmaxs[0], (float)minsmaxs[1], (float)minsmaxs[2]};
    m_Maxs = {(float)minsmaxs[3], (float)minsmaxs[4], (float)minsmaxs[5]};
}

BoundingBox::BoundingBox(const short *mins, const short *maxs)
{
    m_Mins = {(float)mins[0], (float)mins[1], (float)mins[2]};
    m_Maxs = {(float)maxs[0], (float)maxs[1], (float)maxs[2]};
}

BoundingBox::BoundingBox(const float *mins, const float *maxs)
{
    m_Mins = {(float)mins[0], (float)mins[1], (float)mins[2]};
    m_Maxs = {(float)maxs[0], (float)maxs[1], (float)maxs[2]};
}

const glm::vec3 &BoundingBox::Mins() const
{
    return m_Mins;
}

const glm::vec3 &BoundingBox::Maxs() const
{
    return m_Maxs;
}

void BoundingBox::AddPoint(const glm::vec3 pt)
{
    for (int i = 0; i < 3; i++)
    {
        if (m_Mins[i] > pt[i])
            m_Mins[i] = pt[i];

        if (m_Maxs[i] < pt[i])
            m_Maxs[i] = pt[i];
    }
}

void BoundingBox::AddBoundingBox(const BoundingBox &other)
{
    AddPoint(other.Mins());
    AddPoint(other.Maxs());
}

glm::vec3 BoundingBox::Center()
{
    return m_Mins + (m_Maxs - m_Mins) * 0.5f;
}

BoundingBox BoundingBox::ConvertToRelative()
{
    glm::vec3 center = Center();
    return BoundingBox(m_Mins - center, m_Maxs - center);
}

void BoundingBox::Translate(glm::vec3 delta)
{
    m_Mins += delta;
    m_Maxs += delta;
}

void BoundingBox::ApplyScale(glm::vec3 matrixScale)
{
    m_Mins *= matrixScale;
    m_Maxs *= matrixScale;
}

