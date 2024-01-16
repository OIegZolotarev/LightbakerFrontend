/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "winding.h"
#include "brush_face.h"

Winding::Winding(const Winding *pOther)
{
    m_Points.resize(pOther->m_Points.size());
    memcpy(m_Points.data(), pOther->m_Points.data(), sizeof(glm::vec3) * m_Points.size());
}

Winding::Winding(const uint32_t numPoints)
{
    if (numPoints > MAX_POINTS_ON_WINDING)
        Application::EPICFAIL(
            "Winding::Winding(): exceeded points limit (requested %d points, soft-limit is set to %d)", numPoints,
            MAX_POINTS_ON_WINDING);

    m_Points.reserve(numPoints);
}

Winding::Winding(const plane_t *planeFrom)
{
    int       i, x;
    float     max, v;
    glm::vec3 org, vright, vup;

    // find the major axis
    max = -BOGUS_RANGE;
    x   = -1;
    for (i = 0; i < 3; i++)
    {
        v = fabs(planeFrom->normal[i]);
        if (v > max)
        {
            x   = i;
            max = v;
        }
    }

    if (x == -1)
        Application::EPICFAIL("Winding::Winding(const plane_t&): no axis found");

    vup = {0, 0, 0};

    switch (x)
    {
    case 0:
    case 1:
        vup[2] = 1;
        break;
    case 2:
        vup[0] = 1;
        break;
    }

    v = glm::dot(vup, planeFrom->normal);
    VectorMA(vup, -v, planeFrom->normal, vup);
    vup = glm::normalize(vup);

    org = planeFrom->normal * planeFrom->dist;

    vright = glm::cross(vup, planeFrom->normal);

    vup    = vup * MAX_TRACE_LENGTH;
    vright = vright * MAX_TRACE_LENGTH;

    // project a really big	axis aligned box onto the plane

    glm::vec3 p;

    p = org;
    p -= vright;
    p += vup;

    m_Points.push_back(p);

    p = org;
    p += vright;
    p += vup;

    m_Points.push_back(p);

    p = org;
    p += vright;
    p -= vup;

    m_Points.push_back(p);

    p = org;
    p -= vright;
    p -= vup;

    m_Points.push_back(p);
}

Winding::~Winding()
{
    m_Points.clear();
}

#define SPLIT_EPSILON 0.01

void Winding::Clip(plane_t &split)
{
    float dists[MAX_POINTS_ON_WINDING];
    int   sides[MAX_POINTS_ON_WINDING];
    int   counts[3];
    float dot;

    glm::vec3 *p1, *p2, *mid;

    int maxpts;

    counts[0] = counts[1] = counts[2] = 0;

    // determine sides for each point
    for (size_t i = 0; i < m_Points.size(); i++)
    {
        dot = glm::dot(m_Points[i], split.normal);
        dot -= split.dist;

        dists[i] = dot;
        if (dot > SPLIT_EPSILON)
            sides[i] = SIDE_FRONT;
        else if (dot < -SPLIT_EPSILON)
            sides[i] = SIDE_BACK;
        else
        {
            sides[i] = SIDE_ON;
        }
        counts[sides[i]]++;
    }

    assert(m_Points.size() < MAX_POINTS_ON_WINDING);

    sides[m_Points.size()] = sides[0];
    dists[m_Points.size()] = dists[0];

    if (!counts[0] && !counts[1])
        return;

    if (!counts[0])
    {
        m_Points.clear();
        return;
    }

    if (!counts[1])
        return;

    maxpts = (uint32_t) m_Points.size() + 4; // can't use counts[0]+2 because
                                  // of fp grouping errors

    glm::vec3 *new_pts = new glm::vec3[maxpts];
    size_t     np      = 0;

    for (size_t i = 0; i < m_Points.size(); i++)
    {
        p1 = &m_Points[i];

        glm::vec3 *mid = &new_pts[np];

        if (sides[i] == SIDE_FRONT || sides[i] == SIDE_ON)
        {
            *mid = *p1;
            np++;

            if (sides[i] == SIDE_ON)
                continue;

            mid = &new_pts[np];
        }

        if (sides[i + 1] == SIDE_ON || sides[i + 1] == sides[i])
            continue;

        // generate a split point
        if (i == m_Points.size() - 1)
            p2 = &m_Points[0];
        else
            p2 = p1 + 1;

        np++;

        dot = dists[i] / (dists[i] - dists[i + 1]);
        for (int j = 0; j < 3; j++)
        { // avoid round off error when possible
            if (split.normal[j] == 1)
                mid[0][j] = split.dist;
            else if (split.normal[j] == -1)
                mid[0][j] = -split.dist;
            mid[0][j] = p1[0][j] + dot * (p2[0][j] - p1[0][j]);
        }
        //		mid[3] = p1[3] + (p2[3]-p1[3]) * dot;
        //		mid[4] = p1[4] + (p2[4]-p1[4]) * dot;
    }

    if (np > maxpts)
        Application::EPICFAIL("Winding::Clip(): points exceeded estimate");

    m_Points.resize(np);
    memcpy(m_Points.data(), new_pts, sizeof(glm::vec3) * np);

    delete[] new_pts;
}

void Winding::Clip(BrushFace *pFace)
{
    const plane_t *facePlane = pFace->GetPlane();
    plane_t        clipPlane;

    clipPlane.normal  = -facePlane->normal;
    clipPlane.dist    = -facePlane->dist;

    Clip(pFace);
}

void Winding::RemoveDuplicatePoints(float flMinDist)
{
    for (size_t i = 0; i < m_Points.size(); i++)
    {
        for (size_t j = 0; j < m_Points.size(); j++)
        {
            glm::vec3 edge;
            edge = m_Points[i] - m_Points[j];

            if (glm::length(edge) < flMinDist)
            {
                m_Points.erase(m_Points.begin() + j);
            }
        }
    }
}

bool Winding::IsEmpty()
{
    m_Points.empty();
}

std::vector<glm::vec3> &Winding::GetPoints()
{
    throw std::logic_error("The method or operation is not implemented.");
}
