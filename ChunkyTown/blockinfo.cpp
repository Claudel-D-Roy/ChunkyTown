#include "blockinfo.h"

#include <iostream>

BlockInfo::BlockInfo(BlockType type, const std::string& name, int durability) : m_type(type), m_name(name), m_durability(durability), m_u(), m_v(), m_height(), m_width()
{

}

BlockInfo::~BlockInfo()
{

}

BlockType BlockInfo::GetType() const
{
    return m_type;
}

std::string BlockInfo::GetName() const
{
    return m_name;
}

float BlockInfo::GetU()
{
    return m_u;
}

void BlockInfo::SetU(float u)
{
    m_u = u;
}

float BlockInfo::GetV()
{
    return m_v;
}

void BlockInfo::SetV(float v)
{
    m_v = v;
}

float BlockInfo::GetHeight()
{
    return m_height;
}

void BlockInfo::SetHeight(float height)
{
    m_height = height;
}

float BlockInfo::GetWidth()
{
    return m_width;
}

void BlockInfo::SetWidth(float width)
{
    m_width = width;
}

int BlockInfo::GetDurability() const
{
    return m_durability;
}

void BlockInfo::SetDurability(int durability)
{
    m_durability = durability;
}

void BlockInfo::SetUVWH(float u, float v, float w, float h)
{
    m_u = u;
    m_v = v;
    m_width = w;
    m_height = h;
}

void BlockInfo::Show() const
{
    std::cout << "Type: " << m_type << std::endl;
    std::cout << "Nom: " << m_name << std::endl;
    std::cout << "Durabilite: " << m_durability << std::endl;
}
