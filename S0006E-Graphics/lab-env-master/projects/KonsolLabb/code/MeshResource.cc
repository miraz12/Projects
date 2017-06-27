#pragma once
#include "MeshResource.h"
#include "config.h"
#include <cstring>
#include <map>
#include <ctime>

const int BLOCKSIZE = 4096;

//#define  WIREFRAME

	MeshResource::MeshResource()
	{
        onethrid = 1.0f/3.0f;
        onefourth = 1.0f/4.0f;
	}
	MeshResource::~MeshResource()
	{
/*
        for (int i = 0; i < HEV.size(); ++i)
        {
            free(HEV[i]);
        }
        for (int i = 0; i < HEV.size(); ++i)
        {
            free(HEV[i]);
        }
        for (int i = 0; i < HEV.size(); ++i)
        {
            free(HEV[i]);
        }
*/
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
#ifdef WIREFRAME
		glBegin(GL_LINES);
        for (int i = 0; i < vertices.size() -4; i+=4)
        {
            glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
            glVertex3f(vertices[i+1].x(), vertices[i+1].y(), vertices[i+1].z());
            glVertex3f(vertices[i+1].x(), vertices[i+1].y(), vertices[i+1].z());
            glVertex3f(vertices[i+2].x(), vertices[i+2].y(), vertices[i+2].z());
            glVertex3f(vertices[i+2].x(), vertices[i+2].y(), vertices[i+2].z());
            glVertex3f(vertices[i+3].x(), vertices[i+3].y(), vertices[i+3].z());
            glVertex3f(vertices[i+3].x(), vertices[i+3].y(), vertices[i+3].z());
            glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());

        }
        glEnd();
#else
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

#endif
	}

	bool MeshResource::loadOBJ(const char * path)
	{

        int matches;
		printf("Loading OBJ file %s...\n", path);

		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<vector3D> temp_vertices;
		std::vector<vector3D> temp_uvs;
		std::vector<vector3D> temp_normals;


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
				unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
				matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
				if (matches != 9 && matches != 12){
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

                if(matches == 9)
                {
                    vector3D tempFace(vertexIndex[0], vertexIndex[1], vertexIndex[2]);
                    faces.push_back(tempFace);

                }

                if(matches == 12)
                {
                    vector4D tempFace(vertexIndex[0], vertexIndex[1], vertexIndex[2], vertexIndex[3]);
                    vector4D tempFace2(uvIndex[0], uvIndex[1], uvIndex[2], uvIndex[3]);
                    vector4D tempFace3(normalIndex[0], normalIndex[1], normalIndex[2], normalIndex[3]);
                    facesQuad.push_back(tempFace);
                    facesQuadUv.push_back(tempFace2);
                    facesQuadNormal.push_back(tempFace3);

                    vertexIndices.push_back(vertexIndex[3]);
                    uvIndices.push_back(uvIndex[3]);
                    normalIndices.push_back(normalIndex[3]);
                }

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

        ///THIS IS ONLY FOR SUBDIVISION AND QUADS. REMOVE OTHERWISE.
        this->vertices = temp_vertices;
        this->uvs = temp_uvs;
        this->normals = temp_normals;


		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vector3D), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvb);
		glBindBuffer(GL_ARRAY_BUFFER, uvb);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vector3D), &uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &nb);
		glBindBuffer(GL_ARRAY_BUFFER, nb);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vector3D), &normals[0], GL_STATIC_DRAW);


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

    void MeshResource::convertToHalfEdges(int times)
    {
        subTimes = times;


        numVerts = (int)vertices.size();
        numFaces = (int)(vertices.size() * 0.75f);
        numEdges = numFaces * 4;

        int fTot = numFaces * (int)pow(4, times +1);
        int vTot = fTot + 2;
        int eTot = numEdges * (int)pow(4, times +1);;


        HEVA = (HE_vert*)malloc( vTot  * sizeof(HE_vert));
        HEFA = (HE_face*)malloc( eTot* sizeof(HE_face));
        HEEA = (HE_edge*)malloc( fTot* sizeof(HE_edge));

        vertsIt = numVerts;
        faceIt  = numFaces;
        edgesIt = numEdges;
        

        for (int j = 0; j < numVerts; ++j)
        {

            HEVA[j].verts = vertices[j];
        }

        std::map<std::pair<int, int>, HE_edge*> edgeMapA;
        std::map<std::pair<int, int>, HE_edge*>::iterator edgeMapItA;
        int edgeIttemp = 0;
        for (int i = 0; i < facesQuad.size(); ++i)
        {
            //Index - 1
            int va = (int)facesQuad[i].x() -1, vb = (int)facesQuad[i].y() -1, vc = (int)facesQuad[i].z() -1, vd = (int)facesQuad[i].w() -1;
            int uva = (int)facesQuadUv[i].x() - 1, uvb = (int)facesQuadUv[i].y() - 1, uvc = (int)facesQuadUv[i].z() - 1, uvd = (int)facesQuadUv[i].w() - 1;
            int nora = (int)facesQuadNormal[i].x() - 1, norb = (int)facesQuadNormal[i].y() - 1, norc = (int)facesQuadNormal[i].z() - 1, nord = (int)facesQuadNormal[i].w() - 1;

            HEVA[va].uv = this->uvs[uva];
            HEVA[vb].uv = this->uvs[uvb];
            HEVA[vc].uv = this->uvs[uvc];
            HEVA[vd].uv = this->uvs[uvd];

            HEVA[va].normal = this->normals[nora];
            HEVA[vb].normal = this->normals[norb];
            HEVA[vc].normal = this->normals[norc];
            HEVA[vd].normal = this->normals[nord];

            HEEA[edgeIttemp].vert = &HEVA[va];
            HEEA[edgeIttemp].face = &HEFA[i];
            edgeMapA[std::pair<int,int>(vb, va)] = &HEEA[edgeIttemp];
            edgeIttemp++;

            HEEA[edgeIttemp].vert = &HEVA[vb];
            HEEA[edgeIttemp].face = &HEFA[i];
            edgeMapA[std::pair<int,int>(vc, vb)] = &HEEA[edgeIttemp];
            edgeIttemp++;

            HEEA[edgeIttemp].vert = &HEVA[vc];
            HEEA[edgeIttemp].face = &HEFA[i];
            edgeMapA[std::pair<int,int>(vd, vc)] = &HEEA[edgeIttemp];
            edgeIttemp++;

            HEEA[edgeIttemp].vert = &HEVA[vd];
            HEEA[edgeIttemp].face = &HEFA[i];
            edgeMapA[std::pair<int,int>(va, vd)] = &HEEA[edgeIttemp];
            edgeIttemp++;

            //HeVertex edges
            HEVA[va].edge = &HEEA[edgeIttemp - 4];
            HEVA[vb].edge = &HEEA[edgeIttemp - 3];
            HEVA[vc].edge = &HEEA[edgeIttemp - 2];
            HEVA[vd].edge = &HEEA[edgeIttemp - 1];

            //Halfedge next
            HEEA[edgeIttemp - 4].next = &HEEA[edgeIttemp - 3];
            HEEA[edgeIttemp - 3].next = &HEEA[edgeIttemp - 2];
            HEEA[edgeIttemp - 2].next = &HEEA[edgeIttemp - 1];
            HEEA[edgeIttemp - 1].next = &HEEA[edgeIttemp - 4];

            edgeMapItA = edgeMapA.find(std::pair<int, int>(va, vb));
            if (edgeMapItA != edgeMapA.end())
            {
                //If edge in opposite direction exists, set pairs
                edgeMapA[std::pair<int,int>(va, vb)]->pair = edgeMapA[std::pair<int,int>(vb, va)];
                edgeMapA[std::pair<int,int>(vb, va)]->pair = edgeMapA[std::pair<int,int>(va, vb)];
                edgeMapA.erase(std::pair<int,int>(va, vb));
                edgeMapA.erase(std::pair<int,int>(vb, va));
            }
            edgeMapItA = edgeMapA.find(std::pair<int, int>(vb, vc));
            if (edgeMapItA != edgeMapA.end())
            {
                edgeMapA[std::pair<int,int>(vb, vc)]->pair = edgeMapA[std::pair<int,int>(vc, vb)];
                edgeMapA[std::pair<int,int>(vc, vb)]->pair = edgeMapA[std::pair<int,int>(vb, vc)];
                edgeMapA.erase(std::pair<int,int>(vb, vc));
                edgeMapA.erase(std::pair<int,int>(vc, vb));
            }
            edgeMapItA = edgeMapA.find(std::pair<int, int>(vc, vd));
            if (edgeMapItA != edgeMapA.end())
            {
                edgeMapA[std::pair<int,int>(vc, vd)]->pair = edgeMapA[std::pair<int,int>(vd, vc)];
                edgeMapA[std::pair<int,int>(vd, vc)]->pair = edgeMapA[std::pair<int,int>(vc, vd)];
                edgeMapA.erase(std::pair<int,int>(vc, vd));
                edgeMapA.erase(std::pair<int,int>(vd, vc));
            }
            edgeMapItA = edgeMapA.find(std::pair<int, int>(vd, va));
            if (edgeMapItA != edgeMapA.end())
            {
                edgeMapA[std::pair<int,int>(vd, va)]->pair = edgeMapA[std::pair<int,int>(va, vd)];
                edgeMapA[std::pair<int,int>(va, vd)]->pair = edgeMapA[std::pair<int,int>(vd, va)];
                edgeMapA.erase(std::pair<int,int>(vd, va));
                edgeMapA.erase(std::pair<int,int>(va, vd));
            }

            HEFA[i].edge = &HEEA[edgeIttemp - 4];
        }

        if (!edgeMapA.empty())
        {
            printf("%s\n", "Edges not paired in map.");
        }

        DrawHalfEdge();
 /*       return;;

        std::map<std::pair<int, int>, HE_edge*> edgeMap;
        std::map<std::pair<int, int>, HE_edge*>::iterator edgeMapIt;
        int faceIndex = 0;
        for (vector4D vec : facesQuad)
        {
            HE_face* heFace = new HE_face;

            ///*
                                 a
                                / |\ halfEdge (point of reference)
                halfEdgeNext  |/    \
                              b------>c
                                halfEdgePrev

                           a<---------d
                           |          ^
                           |          |
                          \|/         |
                           b--------->c


            //Index - 1
            int va = (int)vec.x() -1, vb = (int)vec.y() -1, vc = (int)vec.z() -1, vd = (int)vec.w() -1;
            int uva = (int)facesQuadUv[faceIndex].x() - 1, uvb = (int)facesQuadUv[faceIndex].y() - 1, uvc = (int)facesQuadUv[faceIndex].z() - 1, uvd = (int)facesQuadUv[faceIndex].w() - 1;
            int nora = (int)facesQuadNormal[faceIndex].x() - 1, norb = (int)facesQuadNormal[faceIndex].y() - 1, norc = (int)facesQuadNormal[faceIndex].z() - 1, nord = (int)facesQuadNormal[faceIndex].w() - 1;


            HEV[va]->uv = this->uvs[uva];
            HEV[vb]->uv = this->uvs[uvb];
            HEV[vc]->uv = this->uvs[uvc];
            HEV[vd]->uv = this->uvs[uvd];

            HEV[va]->normal = this->normals[nora];
            HEV[vb]->normal = this->normals[norb];
            HEV[vc]->normal = this->normals[norc];
            HEV[vd]->normal = this->normals[nord];


            HE_edge* halfEdgeA = new HE_edge;
            halfEdgeA->vert = this->HEV[va];
            halfEdgeA->face = heFace;
            HEE.push_back(halfEdgeA);
            edgeMap[std::pair<int,int>(vb, va)] = halfEdgeA;

            HE_edge* halfEdgeB = new HE_edge;
            halfEdgeB->vert = this->HEV[vb];
            halfEdgeB->face = heFace;
            HEE.push_back(halfEdgeB);
            edgeMap[std::pair<int,int>(vc, vb)] = halfEdgeB;

            HE_edge* halfEdgeC = new HE_edge;
            halfEdgeC->vert = this->HEV[vc];
            halfEdgeC->face = heFace;
            HEE.push_back(halfEdgeC);
            edgeMap[std::pair<int,int>(vd, vc)] = halfEdgeC;

            HE_edge* halfEdgeD = new HE_edge;
            halfEdgeD->vert = this->HEV[vd];
            halfEdgeD->face = heFace;
            HEE.push_back(halfEdgeD);
            edgeMap[std::pair<int,int>(va, vd)] = halfEdgeD;

            //HeVertex edges
            HEV[va]->edge = halfEdgeA;
            HEV[vb]->edge = halfEdgeB;
            HEV[vc]->edge = halfEdgeC;
            HEV[vd]->edge = halfEdgeD;

            //Halfedge next
            halfEdgeA->next = halfEdgeB;
            halfEdgeB->next = halfEdgeC;
            halfEdgeC->next = halfEdgeD;
            halfEdgeD->next = halfEdgeA;

            edgeMapIt = edgeMap.find(std::pair<int, int>(va, vb));
            if (edgeMapIt != edgeMap.end())
            {
                //If edge in opposite direction exists, set pairs
                edgeMap[std::pair<int,int>(va, vb)]->pair = edgeMap[std::pair<int,int>(vb, va)];
                edgeMap[std::pair<int,int>(vb, va)]->pair = edgeMap[std::pair<int,int>(va, vb)];
                edgeMap.erase(std::pair<int,int>(va, vb));
                edgeMap.erase(std::pair<int,int>(vb, va));
            }
            edgeMapIt = edgeMap.find(std::pair<int, int>(vb, vc));
            if (edgeMapIt != edgeMap.end())
            {
                edgeMap[std::pair<int,int>(vb, vc)]->pair = edgeMap[std::pair<int,int>(vc, vb)];
                edgeMap[std::pair<int,int>(vc, vb)]->pair = edgeMap[std::pair<int,int>(vb, vc)];
                edgeMap.erase(std::pair<int,int>(vb, vc));
                edgeMap.erase(std::pair<int,int>(vc, vb));
            }
            edgeMapIt = edgeMap.find(std::pair<int, int>(vc, vd));
            if (edgeMapIt != edgeMap.end())
            {
                edgeMap[std::pair<int,int>(vc, vd)]->pair = edgeMap[std::pair<int,int>(vd, vc)];
                edgeMap[std::pair<int,int>(vd, vc)]->pair = edgeMap[std::pair<int,int>(vc, vd)];
                edgeMap.erase(std::pair<int,int>(vc, vd));
                edgeMap.erase(std::pair<int,int>(vd, vc));
            }
            edgeMapIt = edgeMap.find(std::pair<int, int>(vd, va));
            if (edgeMapIt != edgeMap.end())
            {
                edgeMap[std::pair<int,int>(vd, va)]->pair = edgeMap[std::pair<int,int>(va, vd)];
                edgeMap[std::pair<int,int>(va, vd)]->pair = edgeMap[std::pair<int,int>(vd, va)];
                edgeMap.erase(std::pair<int,int>(vd, va));
                edgeMap.erase(std::pair<int,int>(va, vd));
            }

            heFace->edge = halfEdgeA;
            HEF.push_back(heFace);

            //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            faceIndex++;
        }

        if (!edgeMap.empty())
        {
            printf("%s\n", "Edges not paired in map.");
        }

        //return;
        DrawHalfEdge();
        */

    }

    void MeshResource::DrawHalfEdge()
    {
        //Clear vertice list and make new from HEF
        vertices.clear();
		uvs.clear();
		normals.clear();

#ifdef WIREFRAME
		for (int i = 0; i < numFaces; ++i)
        {
			vertices.push_back(HEFA[i].edge->vert->verts);
			vertices.push_back(HEFA[i].edge->next->vert->verts);
			vertices.push_back(HEFA[i].edge->next->next->vert->verts);
			vertices.push_back(HEFA[i].edge->next->next->next->vert->verts);

			uvs.push_back(HEFA[i].edge->vert->uv);
			uvs.push_back(HEFA[i].edge->next->vert->uv);
			uvs.push_back(HEFA[i].edge->next->next->vert->uv);
			uvs.push_back(HEFA[i].edge->next->next->next->vert->uv);

			normals.push_back(HEFA[i].edge->vert->normal);
			normals.push_back(HEFA[i].edge->next->vert->normal);
			normals.push_back(HEFA[i].edge->next->next->vert->normal);
			normals.push_back(HEFA[i].edge->next->next->next->vert->normal);
		}
        int i = 0;
#else

        for (int i = 0; i < numFaces; ++i)
        {
            vertices.push_back(HEFA[i].edge->vert->verts);
            vertices.push_back(HEFA[i].edge->next->vert->verts);
            vertices.push_back(HEFA[i].edge->next->next->vert->verts);

            vertices.push_back(HEFA[i].edge->vert->verts);
            vertices.push_back(HEFA[i].edge->next->next->vert->verts);
            vertices.push_back(HEFA[i].edge->next->next->next->vert->verts);

            uvs.push_back(HEFA[i].edge->vert->uv);
            uvs.push_back(HEFA[i].edge->next->vert->uv);
            uvs.push_back(HEFA[i].edge->next->next->vert->uv);

            uvs.push_back(HEFA[i].edge->vert->uv);
            uvs.push_back(HEFA[i].edge->next->next->vert->uv);
            uvs.push_back(HEFA[i].edge->next->next->next->vert->uv);

            normals.push_back(HEFA[i].edge->vert->normal);
            normals.push_back(HEFA[i].edge->next->vert->normal);
            normals.push_back(HEFA[i].edge->next->next->vert->normal);

            normals.push_back(HEFA[i].edge->vert->normal);
            normals.push_back(HEFA[i].edge->next->next->vert->normal);
            normals.push_back(HEFA[i].edge->next->next->next->vert->normal);
        }

/*		for(HE_face* face : HEF)
        {
            vertices.push_back(face->edge->vert->verts);
            vertices.push_back(face->edge->next->vert->verts);
            vertices.push_back(face->edge->next->next->vert->verts);

            vertices.push_back(face->edge->vert->verts);
            vertices.push_back(face->edge->next->next->vert->verts);
            vertices.push_back(face->edge->next->next->next->vert->verts);

			uvs.push_back(face->edge->vert->uv);
			uvs.push_back(face->edge->next->vert->uv);
			uvs.push_back(face->edge->next->next->vert->uv);

			uvs.push_back(face->edge->vert->uv);
			uvs.push_back(face->edge->next->next->vert->uv);
			uvs.push_back(face->edge->next->next->next->vert->uv);

			normals.push_back(face->edge->vert->normal);
			normals.push_back(face->edge->next->vert->normal);
			normals.push_back(face->edge->next->next->vert->normal);

			normals.push_back(face->edge->vert->normal);
			normals.push_back(face->edge->next->next->vert->normal);
			normals.push_back(face->edge->next->next->next->vert->normal);
        }*/
#endif


		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vector3D), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvb);
		glBindBuffer(GL_ARRAY_BUFFER, uvb);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vector3D), &uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &nb);
		glBindBuffer(GL_ARRAY_BUFFER, nb);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vector3D), &normals[0], GL_STATIC_DRAW);


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
    }

    void MeshResource::catmullClark()
    {
        for (int l = 0; l < this->subTimes; ++l)
        {
            int oldFaces = numFaces;
            int oldEdges = numEdges;
            int oldVerts = numVerts;
            int newFaces = oldFaces * 4;
            int newEdges = oldEdges * 4;
            int newVerts = newFaces + 2;

            for (int k = 0; k < oldFaces; ++k)
            {
                //The new edge on side A->B. add to list
                HE_edge* abOld = HEFA[k].edge;
                HE_edge* ab = &HEEA[edgesIt++];
                HE_edge* aout = &HEEA[edgesIt++];
                HE_edge* ain = &HEEA[edgesIt++];

                //The new edge on side B->C. add to list
                HE_edge* bcOld = HEFA[k].edge->next;
                HE_edge* bc = &HEEA[edgesIt++];
                HE_edge* bout = &HEEA[edgesIt++];
                HE_edge* bin = &HEEA[edgesIt++];
                HE_face* bcFace = &HEFA[faceIt++];

                //The new edge on side C->D. add to list
                HE_edge* cdOld = HEFA[k].edge->next->next;
                HE_edge* cd = &HEEA[edgesIt++];
                HE_edge* cout = &HEEA[edgesIt++];
                HE_edge* cin = &HEEA[edgesIt++];
                HE_face* cdFace = &HEFA[faceIt++];
                //The new edge on side D->A. add to list
                HE_edge* daOld = HEFA[k].edge->next->next->next;
                HE_edge* da = &HEEA[edgesIt++];
                HE_edge* dout = &HEEA[edgesIt++];
                HE_edge* din = &HEEA[edgesIt++];
                HE_face* daFace = &HEFA[faceIt++];

                HE_vert* ve1 = abOld->vert;
                HE_vert* ve2 = bcOld->vert;
                HE_vert* ve3 = cdOld->vert;
                HE_vert* ve4 = daOld->vert;

                //Create the face point
                HE_vert* facePoint = &HEVA[vertsIt++];
                //Set face point
                facePoint->uv = (ve1->uv + ve2->uv + ve3->uv + ve4->uv) * 0.25f;
                facePoint->verts = (ve1->verts + ve2->verts + ve3->verts + ve4->verts) * 0.25f;
                facePoint->normal = (ve1->normal + ve2->normal + ve3->normal + ve4->normal) * 0.25f;
                facePoint->normal.normalize();

                //Set "known" things. ;)
                //abOld->next = ain;
                ab->next = bcOld;
                ab->pair = abOld->pair;
                ab->face = abOld->face;
                aout->next = ab;
                aout->pair = ain;
                aout->vert = facePoint;
                facePoint->edge = aout; // outgoing edge from the new face
                aout->face = bcFace;
                ain->pair = aout;
                ain->next = dout;
                ain->face = abOld->face;

                //bcOld->next = bin;
                bcOld->face = bcFace;
                bc->next = cdOld;
                bc->pair = bcOld->pair;
                bc->face = bcFace;
                bout->next = bc;
                bout->pair = bin;
                bout->vert = facePoint;
                bout->face = cdFace;
                bin->pair = bout;
                bin->next = aout;
                bin->face = bcFace;

                //cdOld->next = cin;
                cdOld->face = cdFace;
                cd->next = daOld;
                cd->pair = cdOld->pair;
                cd->face = cdFace;
                cout->next = cd;
                cout->pair = cin;
                cout->vert = facePoint;
                cout->face = daFace;
                cin->pair = cout;
                cin->next = bout;
                cin->face = cdFace;

                //daOld->next = din;
                daOld->face = daFace;
                da->next = abOld;
                da->pair = daOld->pair;
                da->face = daFace;
                dout->next = da;
                dout->pair = din;
                dout->vert = facePoint;
                dout->face = abOld->face;
                din->pair = dout;
                din->next = cout;
                din->face = daFace;

                //point new faces to the right edge
                bcFace->edge = ab;
                cdFace->edge = bout;
                daFace->edge = din;

                HE_edge* e1Splitt = abOld->pair->next->pair->next->pair;
                //Check if neighboring face is already divided.
                if ((abOld != e1Splitt))
                {
                    //If not divided
                    midPointInd.push_back(vertsIt);
                    HE_vert* edgePoint1 = &HEVA[vertsIt++];
                    edgePoint1->uv = (abOld->vert->uv + abOld->next->vert->uv) * 0.5f;
                    edgePoint1->verts = (abOld->vert->verts + abOld->next->vert->verts) * 0.5f;
                    edgePoint1->normal = (abOld->vert->normal + abOld->next->vert->normal) * 0.5f;
                    edgePoint1->normal.normalize();

                    ab->vert = edgePoint1;
                    ain->vert = edgePoint1;
                    edgePoint1->edge = ab;
                    abOld->next = ain;
                }
                else
                {
                    ab->pair = abOld->pair;
                    abOld->pair->pair = ab;
                    abOld->pair = abOld->pair->next->pair->next;
                    ab->vert = abOld->pair->vert;
                    ain->vert = abOld->pair->vert;
                    abOld->next = ain;
                }
                HE_edge* e2Splitt = bcOld->pair->next->pair->next->pair;
                //Check if neighboring face is already divided.
                if ((bcOld != e2Splitt))
                {
                    //If not divided
                    midPointInd.push_back(vertsIt);
                    HE_vert* edgePoint2 = &HEVA[vertsIt++];
                    edgePoint2->uv = (bcOld->vert->uv + bcOld->next->vert->uv) * 0.5f;
                    edgePoint2->verts = (bcOld->vert->verts + bcOld->next->vert->verts) * 0.5f;
                    edgePoint2->normal = (bcOld->vert->normal + bcOld->next->vert->normal) * 0.5f;
                    edgePoint2->normal.normalize();

                    bc->vert = edgePoint2;
                    bin->vert = edgePoint2;
                    edgePoint2->edge = bc;
                    bcOld->next = bin;
                }
                else
                {
                    bc->pair = bcOld->pair;
                    bcOld->pair->pair = bc;
                    bcOld->pair = bcOld->pair->next->pair->next;
                    bc->vert = bcOld->pair->vert;
                    bin->vert = bcOld->pair->vert;
                    bcOld->next = bin;
                }
                HE_edge* e3Splitt = cdOld->pair->next->pair->next->pair;
                //Check if neighboring face is already divided.
                if ((cdOld != e3Splitt))
                {
                    //If not divided
                    midPointInd.push_back(vertsIt);
                    HE_vert* edgePoint3 = &HEVA[vertsIt++];
                    edgePoint3->uv = (cdOld->vert->uv + cdOld->next->vert->uv) * 0.5f;
                    edgePoint3->verts = (cdOld->vert->verts + cdOld->next->vert->verts) * 0.5f;
                    edgePoint3->normal = (cdOld->vert->normal + cdOld->next->vert->normal) * 0.5f;
                    edgePoint3->normal.normalize();

                    cd->vert = edgePoint3;
                    cin->vert = edgePoint3;
                    edgePoint3->edge = cd;
                    cdOld->next = cin;
                }
                else
                {
                    cd->pair = cdOld->pair;
                    cdOld->pair->pair = cd;
                    cdOld->pair = cdOld->pair->next->pair->next;
                    cd->vert = cdOld->pair->vert;
                    cin->vert = cdOld->pair->vert;
                    cdOld->next = cin;
                }
                HE_edge* e4Splitt = daOld->pair->next->pair->next->pair;
                //Check if neighboring face is already divided.
                if ((daOld != e4Splitt))
                {
                    //If not divided
                    midPointInd.push_back(vertsIt);
                    HE_vert* edgePoint4 = &HEVA[vertsIt++];
                    edgePoint4->uv = (daOld->vert->uv + daOld->next->vert->uv) * 0.5f;
                    edgePoint4->verts = (daOld->vert->verts + daOld->next->vert->verts) * 0.5f;
                    edgePoint4->normal = (daOld->vert->normal + daOld->next->vert->normal) * 0.5f;
                    edgePoint4->normal.normalize();

                    da->vert = edgePoint4;
                    din->vert = edgePoint4;
                    edgePoint4->edge = da;
                    daOld->next = din;
                }
                else
                {
                    da->pair = daOld->pair;
                    daOld->pair->pair = da;
                    daOld->pair = daOld->pair->next->pair->next;
                    da->vert = daOld->pair->vert;
                    din->vert = daOld->pair->vert;
                    daOld->next = din;
                }
            }
            //Calculate the new original positions.
            for (int j = 0; j < oldVerts; ++j)
            {
                HE_edge* outEdge = HEVA[j].edge;

                vector3D faceAvg;   //Average of faces
                vector3D edgeAvg;   //Average of edges
                vector3D normAvg1;   //Normal average
                vector3D normAvg2;   //Normal average

                HE_edge* temp = outEdge->pair->next;
                int same = 1;
                while (temp != outEdge) //Check how many steps it take to get back to the same edge so we can determine if we are at a corner.
                {
                    same++;
                    temp = temp->pair->next;
                }
                if (same == 3)  //if there are 3 edges from the original point (corner)
                {
                    vector3D avg1;
                    vector3D avg2;
                    vector3D avg3;

                    vector3D nor1;
                    vector3D nor2;
                    vector3D nor3;

                    avg1 = outEdge->next->vert->verts;
                    nor1 = outEdge->next->vert->normal;
                    avg2 = outEdge->pair->next->pair->vert->verts;
                    nor2 = outEdge->pair->next->pair->vert->normal;
                    avg3 = outEdge->next->next->next->vert->verts;
                    nor3 = outEdge->next->next->next->vert->normal;

                    edgeAvg = (avg1 + avg2 + avg3) * onethrid;
                    normAvg1 = (nor1 + nor2 + nor3) * onethrid;

                    avg1 = outEdge->next->next->vert->verts;
                    nor1 = outEdge->next->next->vert->verts;
                    avg2 = outEdge->pair->next->next->next->vert->verts;
                    nor2 = outEdge->pair->next->next->next->vert->verts;
                    avg3 = outEdge->pair->next->pair->next->next->next->vert->verts;
                    nor3 = outEdge->pair->next->pair->next->next->next->vert->verts;

                    faceAvg = (avg1 + avg2 + avg3) * onethrid;
                    normAvg2 = (nor1 + nor2 + nor3) * onethrid;

                    vector3D calc = faceAvg + (edgeAvg * 2.0f);
                    vector3D calc2 = normAvg2 + (normAvg1 * 2.0f);

                    //Set vert to value
                    HEVA[j].edge->vert->verts = calc * onethrid;
                    HEVA[j].edge->vert->normal = calc2 * onethrid;
                    HEVA[j].edge->vert->normal.normalize();
                    int i = 0;

                }
                else if (same == 4) //if there are 4 edges from the original point
                {
                    vector3D avg1;
                    vector3D avg2;
                    vector3D avg3;
                    vector3D avg4;

                    vector3D nor1;
                    vector3D nor2;
                    vector3D nor3;
                    vector3D nor4;

                    avg1 = outEdge->next->vert->verts;
                    nor1 = outEdge->next->vert->normal;
                    avg2 = outEdge->pair->next->pair->vert->verts;
                    nor2 = outEdge->pair->next->pair->vert->normal;
                    avg3 = outEdge->next->next->next->vert->verts;
                    nor3 = outEdge->next->next->next->vert->normal;
                    avg4 = outEdge->pair->next->pair->next->next->vert->verts;
                    nor4 = outEdge->pair->next->pair->next->next->vert->normal;

                    edgeAvg = (avg1 + avg2 + avg3 + avg4) * onefourth;
                    normAvg1 = (nor1 + nor2 + nor3 + nor4) * onefourth;

                    avg1 = outEdge->next->next->vert->verts;
                    nor1 = outEdge->next->next->vert->normal;
                    avg2 = outEdge->pair->next->next->next->vert->verts;
                    nor2 = outEdge->pair->next->next->next->vert->normal;
                    avg3 = outEdge->pair->next->pair->next->next->next->vert->verts;
                    nor3 = outEdge->pair->next->pair->next->next->next->vert->normal;
                    avg4 = outEdge->next->next->next->pair->next->next->next->vert->verts;
                    nor4 = outEdge->next->next->next->pair->next->next->next->vert->normal;

                    faceAvg = (avg1 + avg2 + avg3 + avg4) * onefourth;
                    normAvg2 = (nor1 + nor2 + nor3 + nor4) * onefourth;

                    vector3D calc = (faceAvg + (edgeAvg * 2.0f) + HEVA[j].verts);
                    vector3D calc2 = (normAvg1 + (normAvg2 * 2.0f) + HEVA[j].normal);

                    //Set vert to value
                    HEVA[j].edge->vert->verts = calc * onefourth;
                    HEVA[j].edge->vert->normal = calc2 * onefourth;
                    HEVA[j].edge->vert->normal.normalize();

                } else
                {
                    printf("%s\n", "Somethings wrong!");
                }
            }
            //Start making midpoints to edgepoints. right now they are 1/2 but here we make them 1/4
            for (int i = 0; i < this->midPointInd.size(); ++i)
            {
                HEVA[midPointInd[i]].verts = HEVA[midPointInd[i]].verts * 0.5f;
                HEVA[midPointInd[i]].uv = HEVA[midPointInd[i]].uv * 0.5f;
                HEVA[midPointInd[i]].normal = HEVA[midPointInd[i]].normal * 0.5f;
            }

            this->midPointInd.clear();

            //Add facepoints to edge points
            for (int i = 0; i < oldFaces; ++i)
            {
                HE_vert* west = HEFA[i].edge->next->vert;
                HE_vert* north = HEFA[i].edge->next->next->pair->vert;
                HE_vert* east = HEFA[i].edge->next->next->pair->next->next->vert;
                HE_vert* south = HEFA[i].edge->next->pair->next->next->next->vert;

                HE_vert* facePoint = HEFA[i].edge->next->pair->vert;

                west->uv = (west->uv) + (facePoint->uv * 0.25f);
                west->verts = (west->verts) + (facePoint->verts * 0.25f);
                west->normal = (west->normal) + (facePoint->normal * 0.25f);

                north->uv = (north->uv) + (facePoint->uv * 0.25f);
                north->verts = (north->verts) + (facePoint->verts * 0.25f);
                north->normal = (north->normal) + (facePoint->normal * 0.25f);

                east->uv = (east->uv) + (facePoint->uv * 0.25f);
                east->verts = (east->verts) + (facePoint->verts * 0.25f);
                east->normal = (east->normal) + (facePoint->normal * 0.25f);

                south->uv = (south->uv) + (facePoint->uv * 0.25f);
                south->verts = (south->verts) + (facePoint->verts * 0.25f);
                south->normal = (south->normal) + (facePoint->normal * 0.25f);
            }
            numFaces = newFaces;
            numEdges = newEdges;
            numVerts = newVerts;
        }

    }

