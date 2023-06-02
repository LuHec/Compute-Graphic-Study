#include "IShader.h"

VertexOut PhongShader::VS(vertex v)
{
    // 以史为鉴！牢记教训！警惕前行！orz
    // 
    // Eigen::Vector4f mPos(v.pos.x(), v.pos.y(), v.pos.x(), 1);
    // 
    // 以史为鉴！牢记教训！警惕前行！orz
    Eigen::Vector4f mPos(v.pos.x(), v.pos.y(), v.pos.z(), 1);
    Eigen::Vector4f normal(v.normal.x(), v.normal.y(), v.normal.z(), 0);

    PhongVertexOut vout;
    vout.ScreenPos = cbuffer.ProjectMatrix * cbuffer.ViewMatrix * obuffer.WorldMatrix * mPos;
    vout.worldPos = (obuffer.WorldMatrix * mPos).head<3>();
    vout.uv = v.uv;
    vout.normal = (obuffer.InvTransWorldMatrix * normal).head<3>();
    return vout;
}

Color PhongShader::PS(VertexOut in)
{
    Color col;
    auto normal = (in.normal.normalized() + Eigen::Vector3f(1, 1, 1)) / 2;

    col = { normal.x() * 255, normal.y() * 255, normal.z() * 255, 1};
    //col = { 255, 255, 255, 1 };
    return col;
}




