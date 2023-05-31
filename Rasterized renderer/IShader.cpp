#include "IShader.h"

VertexOut* PhongShader::VS(vertex v)
{
    Eigen::Vector4f mPos(v.pos.x(), v.pos.y(), v.pos.x(), 1);
    Eigen::Vector4f normal(v.normal.x(), v.normal.y(), v.normal.x(), 0);

    PhongVertexOut* vout = new PhongVertexOut();
    vout->ScreenPos = cbuffer.ProjectMatrix * cbuffer.ViewMatrix * obuffer.WorldMatrix * mPos;
    vout->worldPos = (obuffer.WorldMatrix * mPos).head<3>();
    vout->uv = v.uv;
    vout->normal = (obuffer.InvTransWorldMatrix * normal).head<3>();
    return vout;
}

Color PhongShader::PS(VertexOut in)
{
    Color col;
    auto normal = in.normal.normalized() * 0.5 + Eigen::Vector3f(1, 1, 1);
    col = { normal.x() * 255, normal.y() * 255, normal.z() * 255, 1};
    return col;
}


//void PhongShader::UpdateCbuffer(const Cbuffer cbuffer)
//{
//    this->cbuffer.CameraPos = cbuffer.CameraPos;
//    this->cbuffer.ProjectMatrix = cbuffer.ProjectMatrix;
//    this->cbuffer.ScreenMatrix = cbuffer.ScreenMatrix;
//    this->cbuffer.ViewMatrix = cbuffer.ViewMatrix;
//    this->cbuffer.light = cbuffer.light;
//}
//
//void PhongShader::UpdateObjBuffer(const ObjBuffer objbuffer)
//{
//    obuffer.WorldMatrix = objbuffer.WorldMatrix;
//    obuffer.InvTransWorldMatrix = obuffer.WorldMatrix.inverse().transpose();
//}


