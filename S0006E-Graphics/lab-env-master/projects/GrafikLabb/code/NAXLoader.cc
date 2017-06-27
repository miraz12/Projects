//
// Created by antchr-4 on 9/16/16.
//

#include "NAXLoader.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <zconf.h>
#include <sys/stat.h>
#include <cstring>


NAXLoader::NAXLoader()
{

}

void NAXLoader::LoadNax(char* file)
{
    animRe = new AnimResource();
    int myfile = open(file, O_RDONLY);

    struct stat fileStat;
    if(fstat(myfile,&fileStat) < 0)
        return;

    if (myfile != -1)
    {

        uchar* ptr = (uchar*)mmap(0, fileStat.st_size, PROT_READ, MAP_SHARED, myfile, 0);

        if (ptr != MAP_FAILED )
        {
            printf("\n%s\n\n", "Loaded binary file into memory correctly!");
            Nax3Header* naxHeader = (Nax3Header*) ptr;
            ptr += sizeof(Nax3Header);

            if (naxHeader->magic == 'NA01')
            {
                if (naxHeader->numClips > 0)
                {
                    animRe->setupClips(naxHeader->numClips);
                    //Load clips
                    //int numClips =  ;
                    for (int i = 0; i < naxHeader->numClips; ++i)
                    {
                        Nax3Clip* naxClip = (Nax3Clip*)ptr;
                        ptr += sizeof(Nax3Clip);

                        AnimClip& clip = animRe->animClips[i];
                        clip.size = naxHeader->numClips;
                        clip.setupCurves(naxClip->numCurves);
                        clip.startKeyIndex = naxClip->startKeyIndex;
                        clip.numKeys = naxClip->numKeys;
                        clip.keyStride = naxClip->keyStride;
                        clip.keyDuration = naxClip->keyDuration;
                        clip.preInfinityType = (CodeClip)naxClip->preInfinityType;
                        clip.postInfinityType = (CodeClip)naxClip->postInfinityType;
                        clip.name = naxClip->name;

                        //Load events
                        for (int j = 0; j < naxClip->numEvents; j++)
                        {
                            Nax3AnimEvent* naxEvent = (Nax3AnimEvent*)ptr;
                            ptr += sizeof(Nax3AnimEvent);
                        }
                        //Load curves
                        for (int k = 0; k < naxClip->numCurves; k++)
                        {
                            Nax3Curve *naxCurve = (Nax3Curve *) ptr;
                            ptr += sizeof(Nax3Curve);

                            AnimCurve& animCur = clip.curves[k];
                            animCur.firstKeyIndex = naxCurve->firstKeyIndex;
                            animCur.isActive = (naxCurve->isActive != 0);
                            animCur.isStatic = (naxCurve->isStatic != 0);
                            animCur.curveType = (CodeCurve)naxCurve->curveType;
                            vector4D vec4(naxCurve->staticKeyX, naxCurve->staticKeyY, naxCurve->staticKeyZ, naxCurve->staticKeyW);
                            animCur.staticKey = vec4;
                        }
                    }
                }

                //load keys
                AnimKeyBuffer& animKeyBuffer = animRe->SetupKeyBuffer(naxHeader->numKeys);
                //void* keyPtr = animKeyBuffer.Map();
                //std::memcpy(keyPtr ,ptr , (naxHeader->numKeys * sizeof(vector4D)));
                //animKeyBuffer.UnMap();


                int index = naxHeader->numKeys * 4;
                int m = 0;
                for (int l = 0; l < index; l+=4)
                {
                    uchar* x = ptr;
                    ptr += sizeof(float);
                    uchar* y = ptr;
                    ptr += sizeof(float);
                    uchar* z = ptr;
                    ptr += sizeof(float);
                    uchar* w = ptr;
                    ptr += sizeof(float);

                    vector4D temp(*(float*)x,*(float*)y,*(float*)z,*(float*)w);



                    animRe->animKeyBuffer->keyBuffer[m] = temp;
                    m++;
                }




            } else
            {
                printf("%s\n\n", "Magic nr mismatch");
            }
        }

        munmap(ptr, fileStat.st_size);
        close(myfile);
    }
}

NAXLoader::~NAXLoader()
{
    delete animRe;
}
