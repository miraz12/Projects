//
// Created by antchr-4 on 9/2/16.
//

#include "XmlLoader.h"
#include <sstream>


XmlLoader::XmlLoader() {}

XmlLoader::~XmlLoader() {}

//Load xml from file
void XmlLoader::LoadXML(char *file)
{
    tinyxml2::XMLDocument doc;

    if(doc.LoadFile(file) == tinyxml2::XML_SUCCESS)
    {
        tinyxml2::XMLElement* ele = doc.FirstChildElement();
        const char* name = ele->Name();
        while(1)
        {
            if (!strcmp(name, "Skinlist"))
            {
                if (ele->NextSiblingElement() != NULL)
                {
                    ele->NextSiblingElement()->Attribute("name");
                    ele = ele->NextSiblingElement();
                    addJoint(ele);
                }
                else
                {
                    break;
                }
            }
            else if (ele->NoChildren())
            {
                printf("%s", "Error finding nodes\n");
                return;
            }
            else
            {
                printf("%s \n", name);
                ele = ele->FirstChildElement();
                name = ele->Name();
            }
        }
    }
    else
    {
        printf("%s", "Error loading file!\n");
    }
}
//Loop through xml finding all joints, setting and adding them to vector of joints
void XmlLoader::addJoint(tinyxml2::XMLElement* e)
{
    joint temp;

    temp.name = e->Attribute("name");
    tinyxml2::XMLUtil::ToInt(e->Attribute("index"), &temp.index);
    tinyxml2::XMLUtil::ToInt(e->Attribute("parent"), &temp.parent);

    std::string a(e->Attribute("position"));
    std::string b(e->Attribute("rotation"));
    std::string c(e->Attribute("scale"));
    temp.mat = this->parseToMatrix4D(a, b ,c);

    int l = temp.parent;

    joints.push_back(temp);
    if (temp.parent != -1)
    {
        int p = temp.parent;
        joints[temp.parent].children.push_back(temp.index);
    }
}
//Create a matrix4 from thre strings of vector4d
matrix4D XmlLoader::parseToMatrix4D(std::string a, std::string b, std::string c)
{
    std::vector<std::string> vecA = split(a);
    std::vector<std::string> vecB = split(b);
    std::vector<std::string> vecC = split(c);
    std::string::size_type sz;

    vector4D v4A(std::stof(vecA[0], &sz), std::stof(vecA[1], &sz), std::stof(vecA[2], &sz), std::stof(vecA[3], &sz));
    vector4D v4B(std::stof(vecB[0], &sz), std::stof(vecB[1], &sz), std::stof(vecB[2], &sz), std::stof(vecB[3], &sz));
    vector4D v4C(std::stof(vecC[0], &sz), std::stof(vecC[1], &sz), std::stof(vecC[2], &sz), std::stof(vecC[3], &sz));

    matrix4D trans, scale, rot;
    trans = trans.transform(v4A[0], v4A[1],  v4A[2] );
    rot = rot.quatRotation(v4B);
    scale = scale.scale(v4C[0], v4C[1], v4C[2]);

    matrix4D mat;
    mat = trans * rot * scale;

    i++;
    return mat;
}
//Split string by delim.
std::vector<std::string> XmlLoader::split(const std::string &s)
{
    char delim = ',';
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;

    while (getline(ss, item, delim))
    {
        tokens.push_back(item);
    }
    return tokens;
}