#include "MeshResource.h"
#include "config.h"
#include <cstring>
#include "ray.h"

MeshResource::MeshResource()
{

    vbo = -1;
    vao = -1;
    uvb = -1;
    ebo = -1;
    nb = -1;
}

MeshResource::~MeshResource()
{
    if(this->pla != nullptr)
        delete this->pla;
    if(this->ra != nullptr)
        delete this->ra;
    glDeleteBuffers(GL_ARRAY_BUFFER, &vbo);
    glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &vao);
}

void MeshResource::DrawQuad()
{

    // do stuff
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    //glBindVertexArray(this->vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32)* 5, NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32)* 5, (GLvoid*)(sizeof(float32)* 3));

    //Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Cube
void MeshResource::DrawCube()
{

    // do stuff
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    //glBindVertexArray(this->vao);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 5, NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 5, (GLvoid*)(sizeof(float32) * 3));

    //Draw
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshResource::DrawOBJ()
{


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvb);
    glVertexAttribPointer(
            1,                                // attribute
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0    // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, nb);
    glVertexAttribPointer(
            2,                                // attribute
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool MeshResource::loadOBJ(const char * path)
{
    printf("Loading OBJ file %s...\n", path);

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<vector3D> temp_vertices;
    std::vector<vector3D> temp_uvs;
    std::vector<vector3D> temp_normals;

    bool first = true;
    float maxX = 0;
    float minX = 0;
    float maxY = 0;
    float minY = 0;
    float maxZ = 0;
    float minZ = 0;


    FILE * file = fopen(path, "r");
    if (file == NULL){
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return false;
    }

    while (1){

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if (strcmp(lineHeader, "v") == 0){
            vector3D vertex;
            fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
            temp_vertices.push_back(vertex);

            if (first)
            {
                maxX = vertex[0];
                minX = vertex[0];
                maxY = vertex[1];
                minY = vertex[1];
                maxZ = vertex[2];
                minZ = vertex[2];
                first = false;
            }

            if (vertex[0] > maxX)
                maxX = vertex[0];
            if (vertex[0] < minX)
                minX = vertex[0];
            if (vertex[1] > maxY)
                maxY = vertex[1];
            if (vertex[1] < minY)
                minY = vertex[1];
            if (vertex[2] > maxZ)
                maxZ = vertex[2];
            if (vertex[2] < minZ)
                minZ = vertex[2];


        }
        else if (strcmp(lineHeader, "vt") == 0){
            vector3D uv;
            fscanf(file, "%f %f\n", &uv[0], &uv[1]);
            uv[1] = -uv[1]; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0){
            vector3D normal;
            fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9){
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else{
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++){

        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        vector3D vertex = temp_vertices[vertexIndex - 1];
        vector3D uv = temp_uvs[uvIndex - 1];
        vector3D normal = temp_normals[normalIndex - 1];

        // Put the attributes in buffers
        this->vertices.push_back(vertex);
        this->uvs.push_back(uv);
        this->normals.push_back(normal);
    }
    for (int j = 0; j < vertices.size()/3.0f; ++j)
    {
        vector3D x = vertices[j*3];
        vector3D y = vertices[(j*3)+1];
        vector3D z = vertices[(j*3)+2];
        Plane* newMeshPlane = new Plane(vertices[j*3], vertices[(j*3)+1], vertices[(j*3)+2]);
        meshPlanes.push_back(newMeshPlane);
    }
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vector3D), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvb);
    glBindBuffer(GL_ARRAY_BUFFER, uvb);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vector3D), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &nb);
    glBindBuffer(GL_ARRAY_BUFFER, nb);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vector3D), &normals[0], GL_STATIC_DRAW);

    box = new AABB(maxX, maxY, maxZ, minX, minY, minZ);

    return true;
}


//Setting VBO and EBO
void MeshResource::Setup()
{
    GLfloat buf[] =
    {
        -0.3f, -0.3f, -0.3f,	// pos 0
        0, 0,			// color 0

        -0.3f, 0.3f, -0.3f,		// pos 1
        1.0, 0,				// color 1

        0.3f, -0.3f, -0.3f,		// pos 2
        0.0, 1.0,				// color 2

        0.3f, 0.3f, -0.3f,		// pos 3
        1.0, 1.0,				// color 3

        -0.3f, -0.3f, 0.3f,		// pos 4
        0.0, 0.0,				// color 4

        -0.3, 0.3f, 0.3f,		// pos 5
        0.0, 0.0,				// color 5

        0.3f, -0.3f, 0.3f,		// pos 6
        0.0, 1.0,				// color 6

        0.3f, 0.3f, 0.3f,		// pos 7
        0.0, 0.0,				// color 7


    };

    GLuint bUff[] = {
        0, 1, 2,
        1, 3, 2,

        4, 5, 6,
        5, 7, 6,

        0, 4, 2,
        2, 4, 6,

        2, 6, 3,
        6, 3, 7,

        1, 3, 5,
        3, 5, 7,

        0, 1, 4,
        1, 4, 5
    };

    // setup vertex buffer
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // setup index buffer
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bUff), bUff, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Plane* MeshResource::CreateQuadPlane(vector3D p1, vector3D p2, vector3D p3, vector3D p4)
{
	this->pla = new Plane(p1, p2, p3, p4);
    GLfloat buf[] =
            {
                    p1[0], p1[1], p1[2],	// pos 0
                    0, 0,			// color 0

					p2[0], p2[1], p2[2],		// pos 1
                    1.0, 0,				// color 1

					p3[0], p3[1], p3[2],		// pos 2
                    0.0, 1.0,				// color 2

					p4[0], p4[1], p4[2],		// pos 3
                    1.0, 1.0,				// color 3
            };

    GLuint bUff[] = {
            0, 1, 2,
            1, 3, 2,
    };

    // setup vertex buffer
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // setup index buffer
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bUff), bUff, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return pla;

}

void MeshResource::DrawLine()
{
	glLineWidth(2.5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(ra->P0[0], ra->P0[1], ra->P0[2]);
	glVertex3f(ra->P1[0], ra->P1[1], ra->P1[2]);
	glEnd();
}

void MeshResource::DrawLine(vector3D v, vector3D p)
{
    glLineWidth(2.5);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(p.x(), p.y(), p.z());
    glVertex3f((p * v).x(), (p * v).y(), (p * v).z());
    glEnd();
}

void MeshResource::DrawPoint(vector3D p)
{
	glPointSize(10.0f);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
    	glVertex3f(p[0], p[1], p[2]);
	glEnd();
}

void MeshResource::DrawBox()
{
    glLineWidth(4);
    glColor3f(1.f,0.f,0.f);
    glBegin(GL_LINES);
        glVertex3f(box->p1[0], box->p1[1], box->p1[2]);
        glVertex3f(box->p2[0], box->p2[1], box->p2[2]);
        glVertex3f(box->p3[0], box->p3[1], box->p3[2]);
        glVertex3f(box->p4[0], box->p4[1], box->p4[2]);
        glVertex3f(box->p1[0], box->p1[1], box->p1[2]);
        glVertex3f(box->p3[0], box->p3[1], box->p3[2]);
        glVertex3f(box->p2[0], box->p2[1], box->p2[2]);
        glVertex3f(box->p4[0], box->p4[1], box->p4[2]);

        glVertex3f(box->p5[0], box->p5[1], box->p5[2]);
        glVertex3f(box->p6[0], box->p6[1], box->p6[2]);
        glVertex3f(box->p8[0], box->p7[1], box->p7[2]);
        glVertex3f(box->p7[0], box->p8[1], box->p8[2]);
        glVertex3f(box->p5[0], box->p5[1], box->p5[2]);
        glVertex3f(box->p7[0], box->p7[1], box->p7[2]);
        glVertex3f(box->p8[0], box->p8[1], box->p8[2]);
        glVertex3f(box->p6[0], box->p6[1], box->p6[2]);

        glVertex3f(box->p3[0], box->p3[1], box->p3[2]);
        glVertex3f(box->p7[0], box->p7[1], box->p7[2]);
        glVertex3f(box->p1[0], box->p1[1], box->p1[2]);
        glVertex3f(box->p5[0], box->p5[1], box->p5[2]);
        glVertex3f(box->p2[0], box->p2[1], box->p2[2]);
        glVertex3f(box->p6[0], box->p6[1], box->p6[2]);
        glVertex3f(box->p4[0], box->p4[1], box->p4[2]);
        glVertex3f(box->p8[0], box->p8[1], box->p8[2]);
    glEnd();
}

void MeshResource::CollisionBox(Ray* r)
{

    //Check intersections of ray on all current planes
    this->hitByRay = r->Intersect(box->planes);
    if (this->hitByRay)
        CollisionMeshPlanes(r);
}

void MeshResource::CollisionMeshPlanes(Ray* r)
{
    vector3D re(-13, -3, -7);
    r->pHit = re;
    this->hitByRay = false;
    for (int i = 0; i < this->meshPlanes.size(); ++i)
    {
        r->IntersectTriangle(this->meshPlanes[i]);
    }

    return;
}

void MeshResource::MoveMesh(matrix4D mod)
{
    this->model = mod;
    if(this->meshPlanes.size() > 0)
    {

        this->box->reconstructAABB(model);

        vertColl.clear();
        for (int i = 0; i < vertices.size(); ++i)
        {
            vertColl.push_back(model.transform(vertices[i]));
        }

        for (int i = 0; i < meshPlanes.size(); ++i)
        {
            vector3D a = model.transform(this->meshPlanes[i]->oA);
            vector3D b = model.transform(this->meshPlanes[i]->oB);
            vector3D c = model.transform(this->meshPlanes[i]->oC);

            this->meshPlanes[i]->reconstructPlane(a, b, c);
        }
    }
    else if(this->pla != nullptr)
    {
        vector3D a = model.transform(this->pla->oA);
        vector3D b = model.transform(this->pla->oB);
        vector3D c = model.transform(this->pla->oC);
        vector3D d = model.transform(this->pla->oD);

        this->pla->reconstructPlane(a, b, c, d);
    }


}

void MeshResource::PointOnMesh(Ray* r)
{

    for (int i = 0; i < this->meshPlanes.size(); ++i)
    {
        if(r->PointOnMesh(this->meshPlanes[i]) == true)
        {
            this->hitByRay = true;
            this->hitDir = r->v;
            this->hitPoint = r->pHit;
            hitDir.normalize();
        }
    }
}

void MeshResource::DrawBox(AABB box)
{
    //glLineWidth(.5f);
    glBegin(GL_LINES);
        glColor3f(1.f,0.f,0.f);
        glVertex3f((box.center + box.p1)[0], (box.center + box.p1)[1], (box.center + box.p1)[2]);
        glVertex3f((box.center + box.p2)[0], (box.center + box.p2)[1], (box.center + box.p2)[2]);
        glVertex3f((box.center + box.p3)[0], (box.center + box.p3)[1], (box.center + box.p3)[2]);
        glVertex3f((box.center + box.p4)[0], (box.center + box.p4)[1], (box.center + box.p4)[2]);
        glVertex3f((box.center + box.p1)[0], (box.center + box.p1)[1], (box.center + box.p1)[2]);
        glVertex3f((box.center + box.p3)[0], (box.center + box.p3)[1], (box.center + box.p3)[2]);
        glVertex3f((box.center + box.p2)[0], (box.center + box.p2)[1], (box.center + box.p2)[2]);
        glVertex3f((box.center + box.p4)[0], (box.center + box.p4)[1], (box.center + box.p4)[2]);

        glVertex3f((box.center + box.p5)[0], (box.center + box.p5)[1], (box.center + box.p5)[2]);
        glVertex3f((box.center + box.p6)[0], (box.center + box.p6)[1], (box.center + box.p6)[2]);
        glVertex3f((box.center + box.p8)[0], (box.center + box.p7)[1], (box.center + box.p7)[2]);
        glVertex3f((box.center + box.p7)[0], (box.center + box.p8)[1], (box.center + box.p8)[2]);
        glVertex3f((box.center + box.p5)[0], (box.center + box.p5)[1], (box.center + box.p5)[2]);
        glVertex3f((box.center + box.p7)[0], (box.center + box.p7)[1], (box.center + box.p7)[2]);
        glVertex3f((box.center + box.p8)[0], (box.center + box.p8)[1], (box.center + box.p8)[2]);
        glVertex3f((box.center + box.p6)[0], (box.center + box.p6)[1], (box.center + box.p6)[2]);

        glVertex3f((box.center + box.p3)[0], (box.center + box.p3)[1], (box.center + box.p3)[2]);
        glVertex3f((box.center + box.p7)[0], (box.center + box.p7)[1], (box.center + box.p7)[2]);
        glVertex3f((box.center + box.p1)[0], (box.center + box.p1)[1], (box.center + box.p1)[2]);
        glVertex3f((box.center + box.p5)[0], (box.center + box.p5)[1], (box.center + box.p5)[2]);
        glVertex3f((box.center + box.p2)[0], (box.center + box.p2)[1], (box.center + box.p2)[2]);
        glVertex3f((box.center + box.p6)[0], (box.center + box.p6)[1], (box.center + box.p6)[2]);
        glVertex3f((box.center + box.p4)[0], (box.center + box.p4)[1], (box.center + box.p4)[2]);
        glVertex3f((box.center + box.p8)[0], (box.center + box.p8)[1], (box.center + box.p8)[2]);
    glEnd();
}
