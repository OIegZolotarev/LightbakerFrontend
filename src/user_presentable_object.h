/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

// Interface for objects presentable to user
class IUserPresentableObject
{
public:    
    virtual const char *Description() const = 0;
};
