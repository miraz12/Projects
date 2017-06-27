//
// Created by antchr-4 on 9/5/16.
//

#include "JointsStructure.h"

JointsStructure::JointsStructure()
{

    a.LoadXML("Unit_Footman.constants");
    joints = a.getJoints();
	nLoad.LoadNax("Unit_Footman.nax3");
    an = nLoad.animRe;
    //nvxLoad.LoadNvx("Unit_Footman.nvx2");
}

JointsStructure::~JointsStructure(){}
//Loops through joint list and draws out the joints and lines.
void JointsStructure::drawSkeleton()
{
    std::vector<joint> tempJoint = joints;
    for (int i = 0; i < joints.size(); ++i)
    {
        matrix4D matr;
        if(joints[i].parent != -1)
        {
            matr = tempJoint[joints[i].parent].mat * joints[i].mat;
            tempJoint[i].mat = matr;
        }
        else
        {
            matr = joints[i].mat;
            tempJoint[i].mat = matr;
        }
    }
    matrix4D mattt;
    for (int j = 0; j < 21; ++j)
    {
        bones[j] =  tempJoint[j].mat * inverseOrg[j];
    }
    sortBones();

}
//Fill our graph vector with as many graphs as we have joints and set them up.
void JointsStructure::createSkeleton()
{
    std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();
    std::shared_ptr<TextureResource> texture = std::make_shared<TextureResource>("cat_diff.tga");
    std::shared_ptr<ShaderObject> shader = std::make_shared<ShaderObject>("VertexShader.vs", "FragmentShader.fs");

    mesh->loadOBJ("sphere.obj");
    mesh->Setup();

    for (int i = 0; i < joints.size(); ++i)
    {
        GraphicsNode tempG(mesh, texture, shader);
        gNodes.push_back(tempG);
    }
}
//Rotate joint on index i with matrix mat
void JointsStructure::rotateSkeleton(int i, matrix4D mat)
{
    joints[i].mat = mat * joints[i].mat ;
}
//Play animation "clipIndex" from loaded animation resource.
void JointsStructure::PlayAnimation(int index, int clipIndex)
{
    const AnimResource* animRes = this->an;
    const AnimClip& clip = animRes->animClips[clipIndex];

    vector4D* keys = an->animKeyBuffer->keyBuffer;

    int q =0;
    for (uint i = 0 ; i < this->joints.size(); i++)
    {
        vector4D trans, rot;
        trans = keys[clip.curves[q].firstKeyIndex + clip.keyStride * index];
        rot = keys[clip.curves[q+1].firstKeyIndex + clip.keyStride * index];

        matrix4D Trans;
        matrix4D Rot;
        Trans = Trans.transform(trans[0],trans[1], trans[2]);
        Rot = Rot.quatRotation(rot);

        this->joints[i].mat = Trans*Rot;
        q+=4;
    }
}

void JointsStructure::createInverseOriginal()
{
    std::vector<joint> tempJoint = joints;
    for (int i = 0; i < joints.size(); ++i)
    {
        matrix4D matr;

        if(joints[i].parent != -1)
        {
            matr = tempJoint[joints[i].parent].mat * joints[i].mat;
            tempJoint[i].mat = matr;
        }
        else
        {
            matr = joints[i].mat;
            tempJoint[i].mat = matr;
        }
        inverseOrg[i] = tempJoint[i].mat.inv();
    }
}

void JointsStructure::sortBones()
{
    matrix4D jointArrSorted[21];
    jointArrSorted[0] = bones[19];
    jointArrSorted[1] = bones[18];
    jointArrSorted[2] = bones[17];
    jointArrSorted[3] = bones[0];
    jointArrSorted[4] = bones[20];
    jointArrSorted[5] = bones[15];
    jointArrSorted[6] = bones[13];
    jointArrSorted[7] = bones[1];
    jointArrSorted[8] = bones[12];
    jointArrSorted[9] = bones[9];
    jointArrSorted[10] = bones[2];
    jointArrSorted[11] = bones[10];
    jointArrSorted[12] = bones[3];
    jointArrSorted[13] = bones[11];
    jointArrSorted[14] = bones[14];
    jointArrSorted[15] = bones[16];
    jointArrSorted[16] = bones[4];
    jointArrSorted[17] = bones[5];
    jointArrSorted[18] = bones[8];
    jointArrSorted[19] = bones[6];
    jointArrSorted[20] = bones[7];
    for (int j = 0; j < 21; ++j)
    {
        bones[j] = jointArrSorted[j];
    }
}



