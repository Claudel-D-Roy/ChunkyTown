#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include "define.h"

#include <string>

class BlockInfo
{
    public:
        BlockInfo(BlockType type, const std::string& name, int durability);
        ~BlockInfo();

        BlockType GetType() const;
        std::string GetName() const;

        float GetU();
        void SetU(float u);

        float GetV();
        void SetV(float v);

        float GetHeight();
        void SetHeight(float height);

        float GetWidth();
        void SetWidth(float width);

        int GetDurability() const;
        void SetDurability(int durability);

        void SetUVWH(float u, float v, float w, float h);
        void Show() const;

    private:
        BlockType m_type;
        std::string m_name;
        int m_durability;
        float m_u;
        float m_v;
        float m_width;
        float m_height;
};
#endif // BLOCKINFO_H__
