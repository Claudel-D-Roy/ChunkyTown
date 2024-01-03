#include "chunk.h"

#include <iostream>
#include <cmath>
#include <climits>
#include <fstream>

Chunk::Chunk(float x, float z) : m_posX(x), m_posZ(z), m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z)
{
	m_posX = x * CHUNK_SIZE_X;
	m_posZ = z * CHUNK_SIZE_Z;

	

	std::string nomFichier = std::to_string((int)m_posX) + "" + std::to_string((int)m_posZ) + ".bin";
	std::ifstream entree(nomFichier.c_str(), std::fstream::binary);


	if (entree.is_open())
	{
		entree.read((char*)m_blocks.PointeurArray(), CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);


	}
	else 
	{
		Perlin perlin(16, 6, 1, 95);

		m_blocks.Reset(BTYPE_AIR);

		std::cout << "X : " << m_posX << std::endl;
		std::cout << "Z : " << m_posZ << std::endl;

		for (int x = 0; x < CHUNK_SIZE_X; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			{
				float val = perlin.Get((float)(m_posX + x) / 2000.f, (float)(m_posZ + z) / 2000.f);
				val = (val * 100) + 80;

				for (int y = 0; y < val; ++y)
				{
					if (y > 0 && y < 10)
						SetBlock(x, y, z, BTYPE_COBBLE);
					else if (y > 9 && y < 20)
						SetBlock(x, y, z, BTYPE_STONE);
					else if (y > 19 && y < 56)
						SetBlock(x, y, z, BTYPE_SAND);
					else if (y > 55 && y < 70)
						SetBlock(x, y, z, BTYPE_DIRT);
					else if (y > 69 && y < 80)
						SetBlock(x, y, z, BTYPE_GRASS);
					else if (y > 79 && y < 90)
						SetBlock(x, y, z, BTYPE_STONE);
					else if (y > 89)
						SetBlock(x, y, z, BTYPE_SNOW);
				}
			}
		}

	}
	m_modifier = false;
}

Chunk::~Chunk()
{

	if (m_modifier) {


		std::string nomFichier = std::to_string((int)m_posX) + "" + std::to_string((int)m_posZ) + ".bin";

		std::ofstream sortie(nomFichier.c_str(), std::fstream::binary);
		sortie.write((const char*)(m_blocks.PointeurArray()), CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
		sortie.close();
		
	}

}

void Chunk::RemoveBlock(int x, int y, int z)
{
	m_blocks.Set(x, y, z, BTYPE_AIR);
	m_isDirty = true;
	m_modifier = true;
}

void Chunk::SetBlock(int x, int y, int z, BlockType type)
{
	m_blocks.Set(x, y, z, type);
	m_isDirty = true;
	m_modifier = true;
}

BlockType Chunk::GetBlock(int x, int y, int z)
{
	return m_blocks.Get(x, y, z);
}

void Chunk::Update(BlockInfo* blockInfo[BTYPE_LAST])
{
	// Update mesh
	if (m_isDirty)
	{
		int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);
		VertexBuffer::VertexData* vd = new VertexBuffer::VertexData[maxVertexCount];
		int count = 0;
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			{
				for (int y = 0; y < CHUNK_SIZE_Y; ++y)
				{
					if (count > USHRT_MAX)
						break;
					BlockType bt = GetBlock(x, y, z);

					if (bt != BTYPE_AIR)
					{
						AddBlockToMesh(vd, count, bt, x, y, z, blockInfo[bt]->GetU(), blockInfo[bt]->GetV(), blockInfo[bt]->GetWidth(), blockInfo[bt]->GetWidth());
					}
				}
			}
		}

		if (count > USHRT_MAX)
		{
			count = USHRT_MAX;
			std::cout << "[Chunk::Update] Chunk data truncated, too many vertices to have a 16 bit index" << std::endl;
		}
		m_vertexBuffer.SetMeshData(vd, count);
		delete[] vd;
	}
	m_isDirty = false;
}

void Chunk::Render() const
{
	m_vertexBuffer.Render();
}

bool Chunk::IsDirty() const
{
	return m_isDirty;
}

bool Chunk::Modifier() const 
{
	return m_modifier;
}

void Chunk::AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType bt, int x, int y, int z, float u, float v, float w, float h)
{
	if (y == CHUNK_SIZE_Y - 1 || m_blocks.Get(x, y + 1, z) == BTYPE_AIR)
	{
		// DESSUS                                                                           
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y + .5f, m_posZ + z + .5f, 0.8f, 0.8f, 0.8f, u, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y + .5f, m_posZ + z + .5f, 0.8f, 0.8f, 0.8f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y + .5f, m_posZ + z - .5f, 0.8f, 0.8f, 0.8f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y + .5f, m_posZ + z - .5f, 0.8f, 0.8f, 0.8f, u, v + h);
	}
	if (z == CHUNK_SIZE_Z - 1 || m_blocks.Get(x, y, z + 1) == BTYPE_AIR)
	{
		// DEVANT
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y - .5f, m_posZ + z + .5f, 1.f, 1.f, 1.f, u, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y - .5f, m_posZ + z + .5f, 1.f, 1.f, 1.f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y + .5f, m_posZ + z + .5f, 1.f, 1.f, 1.f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y + .5f, m_posZ + z + .5f, 1.f, 1.f, 1.f, u, v + h);
	}
	if (z == (CHUNK_SIZE_Z - CHUNK_SIZE_Z) || m_blocks.Get(x, y, z - 1) == BTYPE_AIR)
	{
		// DERRI�RE
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y - .5f, m_posZ + z - .5f, 1.f, 1.f, 1.f, u, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y - .5f, m_posZ + z - .5f, 1.f, 1.f, 1.f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y + .5f, m_posZ + z - .5f, 1.f, 1.f, 1.f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y + .5f, m_posZ + z - .5f, 1.f, 1.f, 1.f, u, v + h);
	}
	if (x == (CHUNK_SIZE_X - CHUNK_SIZE_X) || m_blocks.Get(x - 1, y, z) == BTYPE_AIR)
	{
		// GAUCHE
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y - .5f, m_posZ + z - .5f, 0.9f, 0.9f, 0.9f, u, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y - .5f, m_posZ + z + .5f, 0.9f, 0.9f, 0.9f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y + .5f, m_posZ + z + .5f, 0.9f, 0.9f, 0.9f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y + .5f, m_posZ + z - .5f, 0.9f, 0.9f, 0.9f, u, v + h);
	}
	if (x == CHUNK_SIZE_X - 1 || m_blocks.Get(x + 1, y, z) == BTYPE_AIR)
	{
		// DROITE                                                                           
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y - .5f, m_posZ + z + .5f, 0.9f, 0.9f, 0.9f, u, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y - .5f, m_posZ + z - .5f, 0.9f, 0.9f, 0.9f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y + .5f, m_posZ + z - .5f, 0.9f, 0.9f, 0.9f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y + .5f, m_posZ + z + .5f, 0.9f, 0.9f, 0.9f, u, v + h);
	}
	if (y > 0 && m_blocks.Get(x, y - 1, z) == BTYPE_AIR)
	{
		// DESSOUS
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y - .5f, m_posZ + z - .5f, 0.8f, 0.8f, 0.8f, u, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y - .5f, m_posZ + z - .5f, 0.8f, 0.8f, 0.8f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(m_posX + x + .5f, y - .5f, m_posZ + z + .5f, 0.8f, 0.8f, 0.8f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(m_posX + x - .5f, y - .5f, m_posZ + z + .5f, 0.8f, 0.8f, 0.8f, u, v + h);
	}
}


