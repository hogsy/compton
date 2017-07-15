/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
*/

#include <PL/platform_model.h>

// todo, http://www.gamers.org/dEngine/quake/spec/quake-spec34/qkspec_4.htm

/* Quake II BSP Support */

typedef struct BSPVector {
    float x;
    float y;
    float z;
} BSPVector;

typedef struct BSPEntry {
    int32_t offset;
    int32_t size;
} BSPEntry;

typedef struct BSPHeader {
    int32_t version;

    BSPEntry entities;
    BSPEntry planes;
    BSPEntry miptex;
    BSPEntry vertices;
    BSPEntry visilist;
    BSPEntry nodes;
    BSPEntry texinfo;
    BSPEntry faces;
    BSPEntry lightmaps;
    BSPEntry clipnodes;
    BSPEntry leaves;
    BSPEntry lface;
    BSPEntry edges;
    BSPEntry ledges;
    BSPEntry models;
} BSPHeader;

typedef struct BSPBoundBox {
    BSPVector min;
    BSPVector max;
} BSPBoundBox;

typedef struct BSPBoundBoxShort {
    int16_t min;
    int16_t max;
} BSPBoundBoxShort;

typedef struct BSPModel {
    BSPBoundBox bound;
    BSPVector origin;

    int16_t node[4];

    int16_t num_leafs;

    int16_t face_id;
    int16_t num_faces;
} BSPModel;

typedef struct BSPEdge {
    uint16_t vertices[2];
} BSPEdge;

/* Source BSP Support */

// etc etc etc