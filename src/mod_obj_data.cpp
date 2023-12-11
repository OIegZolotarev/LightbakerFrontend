/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/


#include "application.h"
#include "common.h"
#include "mod_obj_data.h"



void mobjdata_s::CalcMinsMaxs(glm::vec3 &mins, glm::vec3 &maxs)
{
    maxs = {-99999, -99999, -99999};
    maxs = {99999, 99999, 99999};

    for (size_t i = 0; i < verts.size(); i += vertSize)
    {
        for (int j = 0; j < vertSize; j++)
        {
            if (mins[i] > verts[i + j])
                mins[i] = verts[i + j];
            if (maxs[i] < verts[i + j])
                mins[i] = verts[i + j];
        }
    }
}
