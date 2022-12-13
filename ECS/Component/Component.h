#ifndef __COMPONENT__H__
#define __COMPONENT__H__

#include "../../Vector2D.h"

struct Transform2D
{
    int position;
    int rotation;
    int scale;

    static Transform2D CreateInvalidObject()
    {
        Transform2D invalidObject{};
        invalidObject.isValid = false;
        return invalidObject;
    }

    bool IsValid() const
    {
        return isValid;
    }

private:
    bool isValid = true;
};

struct RigidBody
{
    float velocity;
    float acceleration;

    static RigidBody CreateInvalidObject()
    {
        RigidBody invalidObject{};
        invalidObject.isValid = false;
        return invalidObject;
    }

    bool IsValid() const
    {
        return isValid;
    }

private:
    bool isValid = true;
};

#endif // __COMPONENT__H__