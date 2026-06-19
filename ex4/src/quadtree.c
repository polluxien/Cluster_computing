#include "quadtree.h"

#include <stdlib.h>
#include <string.h>
#include "rect.h"

QuadIndex quadIndexFromV2(V2 position, V2 pivot) {
    QuadIndex index = 0;
    
    if (position.x < pivot.x && position.y < pivot.y) {
        index = QuadIndex_BottomLeft;
    } else if (position.x >= pivot.x && position.y < pivot.y) {
        index = QuadIndex_BottomRight;
    } else if (position.x < pivot.x && position.y >= pivot.y) {
        index = QuadIndex_TopLeft;
    } else if (position.x >= pivot.x && position.y >= pivot.y) {
        index = QuadIndex_TopRight;
    }

    return index;
}

Rect rectFromQuadIndex(Rect rect, QuadIndex index) {
    Rect result = rect;
    V2 center = rectCenter(rect);

    switch (index) {
        case QuadIndex_TopLeft:
            result.high.x = center.x;
            result.low.y  = center.y;
            break;

        case QuadIndex_TopRight:
            result.low.x = center.x;
            result.low.y = center.y;
            break;

        case QuadIndex_BottomLeft:
            result.high.x = center.x;
            result.high.y = center.y;
            break;

        case QuadIndex_BottomRight:
            result.low.x  = center.x;
            result.high.y = center.y;
            break;
    }

    return result;
}

QuadIndex quadIndexFromRect(V2 position, Rect rect) {
    V2 center = rectCenter(rect);
    return quadIndexFromV2(position, center);
}

char *stringFromQuadIndex(QuadIndex index) {
    if (index == QuadIndex_TopLeft) {
        return "TopLeft";
    }
    if (index == QuadIndex_TopRight) {
        return "TopRight";
    }
    if (index == QuadIndex_BottomLeft) {
        return "BottomLeft";
    }
    if (index == QuadIndex_BottomRight) {
        return "BottomRight";
    }
    return "<UNDEFINED>";
}

Quadtree quadtreeAlloc(int nodeCapacity) {
    Quadtree tree = {0};
    tree.nodeCount = nodeCapacity;
    tree.nodes = (QuadtreeNode *)malloc(tree.nodeCount * sizeof(QuadtreeNode));
    memset(tree.nodes, 0, tree.nodeCount * sizeof(QuadtreeNode));
    return tree;
}

void quadtreeFree(Quadtree *tree) {
    free(tree->nodes);
    memset(tree, 0, sizeof(Quadtree));
}

QuadtreeNode *quadtreeGetNodeByIndex(Quadtree *tree, QuadtreeNodeIndex index) {
    QuadtreeNode *node = 0;

    if (index != QuadtreeNodeIndex_Null) {
        node = &tree->nodes[index-1];
    }

    return node;
}

void quadtreeGetNextNode(Quadtree *tree, QuadtreeNode **node, QuadtreeNodeIndex *nodeIndex) {
    int index = tree->nextNodeIndex;
    tree->nextNodeIndex++;

    *nodeIndex = index+1;
    *node = &tree->nodes[index];
}

void quadtreeBuild(Quadtree *tree, Rect bounds, Point *points, int pointCount) {
    // Reset.
    memset(tree->nodes, 0, tree->nodeCount * sizeof(QuadtreeNode));
    tree->nextNodeIndex = 0;
    
    // Setup.
    tree->bounds = bounds;
    QuadtreeNode *root = 0;
    quadtreeGetNextNode(tree, &root, &tree->root);

    // Insert points.
    for (int index = 0; index < pointCount; index++) {

        // Setup.
        Point         newPoint = points[index];
        QuadtreeNode *nextNode = root;
        Rect          bounds   = tree->bounds;
        
        // Iterate until inserted.
        int wasPointInserted = 0;
        while (!wasPointInserted) {
            QuadIndex newPointQuadIndex = quadIndexFromRect(newPoint.position, bounds);

            // Empty => Set point.
            if (nextNode->pointCount == 0) {
                nextNode->point      = newPoint;
                nextNode->pointCount++;
                wasPointInserted = 1;
            }
            // Leaf => Split || Combine.
            else if (nextNode->pointCount == 1) {
                Point oldPoint = nextNode->point;
                QuadIndex oldPointQuadIndex = quadIndexFromRect(oldPoint.position, bounds);
                
                // Collision => Add to mass.
                if (v2Equals(newPoint.position, oldPoint.position)) {
                    nextNode->point = pointsCombine((Point[]){newPoint, oldPoint}, 2);
                }
                // Otherwise => Push down && Combine points && Recurse.
                else {
                    // Push down.
                    if (nextNode->quads[oldPointQuadIndex] == QuadtreeNodeIndex_Null) {
                        QuadtreeNode *quadNode = 0;
                        quadtreeGetNextNode(tree, &quadNode, &nextNode->quads[oldPointQuadIndex]);
                        
                        quadNode->point      = oldPoint;
                        quadNode->pointCount = 1;
                    }

                    // Combine points.
                    nextNode->pointCount++;
                    nextNode->point = pointsCombine((Point[]){newPoint, oldPoint}, 2);

                    // Recurse.
                    if (nextNode->quads[newPointQuadIndex] == QuadtreeNodeIndex_Null) {
                        QuadtreeNode *quadNode = 0;
                        quadtreeGetNextNode(tree, &quadNode, &nextNode->quads[newPointQuadIndex]);
                        quadNode->point      = newPoint;
                        quadNode->pointCount = 1;
                        wasPointInserted = 1;
                    } else {
                        bounds   = rectFromQuadIndex(bounds, newPointQuadIndex);
                        nextNode = quadtreeGetNodeByIndex(tree, nextNode->quads[newPointQuadIndex]);
                    }
                }
            }
            // Intermediate => Combine points && Recurse.
            else {
                // Combine points.
                nextNode->point = pointsCombine((Point[]){newPoint, nextNode->point}, 2);
                nextNode->pointCount++;

                // Recurse.
                if (nextNode->quads[newPointQuadIndex] == QuadtreeNodeIndex_Null) {
                    QuadtreeNode *quadNode = 0;
                    quadtreeGetNextNode(tree, &quadNode, &nextNode->quads[newPointQuadIndex]);
                    quadNode->point      = newPoint;
                    quadNode->pointCount = 1;
                    wasPointInserted = 1;
                } else {
                    bounds   = rectFromQuadIndex(bounds, newPointQuadIndex);
                    nextNode = quadtreeGetNodeByIndex(tree, nextNode->quads[newPointQuadIndex]);
                }
            }
        }
    }
}

/*
 *  theta = mass/distance ratio.
 */
V2 quadtreeGetForce(Quadtree *tree, Point target, double theta) {
    V2 forceTotal = v2(0, 0);
    
    // Setup stack.
    QuadtreeNodeIndex stack[64] = {0};
    int               stackSize = 0;

    // Push root.
    stack[0] = tree->root;
    stackSize++;

    while (stackSize) {
        // Pop next node.
        QuadtreeNode *node = quadtreeGetNodeByIndex(tree, stack[stackSize-1]);
        stackSize--;
    
        // Check non null.
        if (node == QuadtreeNodeIndex_Null) {
            break;
        }

        Point nodePoint = node->point;

        // Collision => Subtract own mass.
        if (node->pointCount == 1 && v2Equals(nodePoint.position, target.position)) {
            // Mass equal => Skip.
            if (nodePoint.mass == target.mass) {
                continue;
            }
            
            // Subtract own mass.
            nodePoint.mass -= target.mass;
        }

        double thetaSquared    = theta * theta;
        double massSquared     = nodePoint.mass * nodePoint.mass;
        V2 r = v2Subtract(node->point.position, target.position);
        double distanceSquared = v2DotProduct(r, r);

        // Leaf || Theta => Use point.
        if (node->pointCount == 1 || (thetaSquared > (massSquared / distanceSquared))) {
            // Newton's Law of Universal Gravitation.
            double epsilon = 1e-5;
            double G = 1.0;

            double factor = (G * nodePoint.mass * target.mass) / (distanceSquared + epsilon);
            V2 force   = v2Scale(r, factor);
            forceTotal = v2Add(forceTotal, force);
        }
        // Otherwise => Recurse
        else {
            if (node->quads[0] != QuadtreeNodeIndex_Null) {
                stack[stackSize] = node->quads[0];
                stackSize++;
            }
            if (node->quads[1] != QuadtreeNodeIndex_Null) {
                stack[stackSize] = node->quads[1];
                stackSize++;
            }
            if (node->quads[2] != QuadtreeNodeIndex_Null) {
                stack[stackSize] = node->quads[2];
                stackSize++;
            }
            if (node->quads[3] != QuadtreeNodeIndex_Null) {
                stack[stackSize] = node->quads[3];
                stackSize++;
            }
        }
    }

    return forceTotal;
}

