#ifndef VERTEXBUFFER_H__
#define VERTEXBUFFER_H__

#include "define.h"

class VertexBuffer
{
    public:
        // Structure représentant toutes les informations de chacuns des vertex
        // S'assurer que le size de cette struct reste un multiple de 32
        // octet pour un maximum de performance
        // Au besoin, ajouter du padding
        struct VertexData
        {
            float x = 0, y = 0, z = 0;
            float r = 0, g = 0, b = 0;
            float u = 0, v = 0;

            VertexData() {}
            VertexData(float x, float y, float z, float r, float g, float b, float u, float v) : x(x), y(y), z(z), r(r), g(g), b(b), u(u), v(v) {}
        };

    public:
        VertexBuffer();
        ~VertexBuffer();
    
        bool IsValid() const;
        void SetMeshData(VertexData* vd, int vertexCount);
        void Render() const;
        int Count() const;

    private:
        bool m_isValid;
        int m_vertexCount;
        GLuint m_vertexVboId;
        GLuint m_indexVboId;
};
#endif // VERTEXBUFFER_H__
