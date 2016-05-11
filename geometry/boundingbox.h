#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../includes.h"
#include "Triangle.h"

class BoundingBox
{
    public:
        BoundingBox();
        BoundingBox(Vector3f a, Vector3f b);
        BoundingBox(const BoundingBox &other);
        BoundingBox(const Scene *scene, const Triangle *other);
        void expand(BoundingBox &other);
        void expand(const Scene *scene, const Triangle *other);
        void expand(Vector3f point);
        virtual ~BoundingBox();

        Vector3f getCenter() const;
        Vector3f getHalfSize() const;

        Vector3f m;
        Vector3f M;
    protected:
    private:
};

#endif // BOUNDINGBOX_H
