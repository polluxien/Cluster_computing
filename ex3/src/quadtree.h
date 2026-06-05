#ifndef QUADTREE_H
#define QUADTREE_H


#include "rect.h"
#include "point.h"


// Z-order index.
typedef int QuadIndex;
enum {
    QuadIndex_TopLeft     = 0,
    QuadIndex_TopRight    = 1,
    QuadIndex_BottomLeft  = 2,
    QuadIndex_BottomRight = 3,
    QuadIndex_COUNT       = 4,
};

QuadIndex quadIndexFromV2(V2 position, V2 pivot);
Rect      rectFromQuadIndex(Rect rect, QuadIndex index);
QuadIndex quadIndexFromRect(V2 position, Rect rect);
char     *stringFromQuadIndex(QuadIndex index);


typedef int QuadtreeNodeIndex;
enum {
    QuadtreeNodeIndex_Null = 0,
};

typedef struct QuadtreeNode QuadtreeNode;
struct QuadtreeNode {
    QuadtreeNodeIndex quads[QuadIndex_COUNT];
    int   pointCount;
    Point point;
};


typedef struct Quadtree Quadtree;
struct Quadtree {
    QuadtreeNode     *nodes;
    int               nodeCount;
    QuadtreeNodeIndex nextNodeIndex;

    Rect              bounds;
    QuadtreeNodeIndex root;
};

Quadtree      quadtreeAlloc(int nodeCapacity);
void          quadtreeFree(Quadtree *tree);
QuadtreeNode *quadtreeGetNodeByIndex(Quadtree *tree, QuadtreeNodeIndex index);
void          quadtreeGetNextNode(Quadtree *tree, QuadtreeNode **node, QuadtreeNodeIndex *index);
void          quadtreeBuild(Quadtree *tree, Rect bounds, Point *points, int pointCount);
V2            quadtreeGetForce(Quadtree *tree, Point target, double theta);


#endif // QUADTREE_H
