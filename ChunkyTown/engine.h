#ifndef ENGINE_H__
#define ENGINE_H__

#include "blockinfo.h"
#include "chunk.h"
#include "define.h"
#include "openglcontext.h"
#include "player.h"
#include "shader.h"
#include "texture.h"
#include "textureatlas.h"
#include<map>

class Engine : public OpenglContext
{
    public:
        Engine();
        virtual ~Engine();
        virtual void Init();
        virtual void DeInit();
        virtual void LoadResource();
        virtual void UnloadResource();
        virtual void Render(float elapsedTime);
        virtual void KeyPressEvent(unsigned char key);
        virtual void KeyReleaseEvent(unsigned char key);
        virtual void MouseMoveEvent(int x, int y);
        virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
        virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);
        virtual void UpdateChunkVisible(bool render);
        virtual void DrawHud(float time);
        virtual void PrintText(unsigned int x, unsigned int y, const std::string& t);
        virtual void GetBlockAtCursor();
        virtual Chunk* ChunkAt(float x, float y, float z) const;
        virtual Chunk* ChunkAt(const Vector3f& pos) const;
        virtual BlockType BlockAt(float x, float y, float z, BlockType defaultBlockType) const;
        
        bool EqualWithEpsilon(const float& v1, const float& v2, float epsilon = float(0.0001));
        bool InRangeWithEpsilon(const float& v, const float& vinf, const float& vsup, float epsilon = float(0.0001));

    private:
        bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);
        
    private:
        struct Vector2f
        {
            int x, z;
            Vector2f(int x, int z) : x(x), z(z) {}

            bool operator < (const Vector2f& autre) const
            {
                return std::make_pair(x, z) < std::make_pair(autre.x, autre.z);
            }
        };

        BlockInfo* m_blockInfo[BTYPE_LAST];
        BlockType m_bt1, m_bt2, m_bt3;
        Player m_player;
        Shader m_shader;
        Texture m_textureCrosshair, m_textureFont, m_face1, m_face2, m_face3, m_face4, m_face5, m_face6;
        TextureAtlas m_textureAtlas;
        Vector2f m_playerOldPos;
        Vector3f m_currentBlock, m_currentFaceNormal;
        std::map <Vector2f, Chunk*> m_map;
        int m_chunkVisibleParJoueur = 0;
        bool m_wireframe = false;
        bool m_keyW = false;
        bool m_keyA = false;
        bool m_keyS = false;
        bool m_keyD = false;
        bool m_jump = false;
        bool m_fly = false;
};
#endif // ENGINE_H__
