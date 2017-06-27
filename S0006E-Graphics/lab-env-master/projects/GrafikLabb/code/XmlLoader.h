//
// Created by antchr-4 on 9/2/16.
//

#ifndef GSCEPT_LAB_ENV_XMLLOADER_H
#define GSCEPT_LAB_ENV_XMLLOADER_H

#include "../tinyxml2/tinyxml2.h"
#include "matrix4D.h"
#include <vector>

struct joint
{
    int index;
    int parent;
    std::vector<int> children;
    const char* name;
    matrix4D mat;
};

class XmlLoader {
public:
    XmlLoader();

    virtual ~XmlLoader();
    void LoadXML(char* file);
    const std::vector<joint> &getJoints() const {return joints;}

private:
    int i =0;
    std::vector<joint> joints;
    void addJoint(tinyxml2::XMLElement* e);
    matrix4D parseToMatrix4D(std::string a, std::string b, std::string c);
    std::vector<std::string> split(const std::string &s);


};



#endif //GSCEPT_LAB_ENV_XMLLOADER_H
