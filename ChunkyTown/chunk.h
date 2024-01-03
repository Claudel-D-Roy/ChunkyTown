#ifndef CHUNK_H__
#define CHUNK_H__

#include "array2d.h"
#include "array3d.h"
#include "blockinfo.h"
#include "vertexbuffer.h"
#include "perlin.h"

class Chunk
{
public:
    Chunk(float x, float z);
    ~Chunk();

    BlockType GetBlock(int x, int y, int z);
    bool IsDirty() const;
    bool Modifier() const;
    void SetBlock(int x, int y, int z, BlockType type);
    void RemoveBlock(int x, int y, int z);
    void Update(BlockInfo* blockInfo[BTYPE_LAST]);
    void Render() const;

private:   
    Array3d<BlockType> m_blocks;
    VertexBuffer m_vertexBuffer;
    float m_posX = 0.f;
    float m_posZ = 0.f;
    bool m_isDirty = true;
    bool m_modifier = false;
   
    void AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType bt, int x, int y, int z, float u, float v, float w, float h);
};
#endif // CHUNK_H__
