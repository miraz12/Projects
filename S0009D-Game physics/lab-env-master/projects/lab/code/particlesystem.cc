//
// Created by antchr-4 on 1/30/17.
//

#include "particlesystem.h"

struct pos
{
    float x, y, z, w; // positions
};
struct vel
{
    float vx, vy, vz, vw; // velocities
};
struct color
{
    float r, g, b, a; // colors
};
// need to do the following for both position, velocity, and colors of the particles:
GLuint posSSbo;
GLuint velSSbo;
GLuint colSSbo;



void ParticleSystem::initGraphics()
{

    glGenBuffers( 1, &posSSbo);
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, posSSbo);
    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(pos), NULL, GL_STATIC_DRAW );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

    glGenBuffers( 1, &velSSbo);
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, velSSbo);
    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(vel), NULL, GL_STATIC_DRAW );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

    glGenBuffers( 1, &colSSbo);
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, colSSbo);
    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(color), NULL, GL_STATIC_DRAW );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

    // print some compute shader statistics:

    int cx, cy, cz;
    glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cx );
    glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cy );
    glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cz );
    fprintf( stderr, "Max Compute Work Group Count = %5d, %5d, %5d\n", cx, cy, cz );

    int sx, sy, sz;
    glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &sx );
    glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &sy );
    glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &sz );
    fprintf( stderr, "Max Compute Work Group Size  = %5d, %5d, %5d\n", sx, sy, sz );

    fprintf( stderr, "My Compute Work Group Count  = %5d, %5d, %5d\n", NUM_PARTICLES/WORK_GROUP_SIZE, 1, 1 );
    fprintf( stderr, "My Compute Work Group Size   = %5d, %5d, %5d\n", WORK_GROUP_SIZE, 1, 1 );

}

float ParticleSystem::fRandom(float low, float high)
{
    long random();

    float r = (float)rand();
    return (low + r * (high-low) / (float)RAND_MAX);
}

void ParticleSystem::animate()
{
    shaderCompute->bind();

    glDispatchCompute(NUM_PARTICLES / WORK_GROUP_SIZE, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

}

ParticleSystem::ParticleSystem(const std::string& cShader)
{
    shaderCompute = std::make_shared<ShaderObject>(cShader);
    shaderRender = std::make_shared<ShaderObject>("VertexShaderCompute.vs", "FragmentShaderCompute.fs");
}

void ParticleSystem::display(matrix4D mvp)
{

    shaderCompute->bind();

    glBindBuffer( GL_ARRAY_BUFFER, posSSbo  );
    glVertexPointer( 4, GL_FLOAT, 0, (void *)0 );
    glEnableClientState( GL_VERTEX_ARRAY );


    glBindBuffer( GL_ARRAY_BUFFER, colSSbo );
    glColorPointer( 4, GL_FLOAT, 0, (void *)0 );
    glEnableClientState( GL_COLOR_ARRAY );
    glColor3f(1,0,0);


    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    glUseProgram(0);

}

void ParticleSystem::updateParticles()
{

    glBindBuffer( GL_SHADER_STORAGE_BUFFER, posSSbo );
    struct pos *points = (pos *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(pos), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT );
    for( int i = 0; i < NUM_PARTICLES; i++ )
    {
        float r = fRandom( XMIN, XMAX );
        points[ i ].x = fRandom( XMIN, XMAX );
        points[ i ].y = fRandom( YMIN, YMAX );
        points[ i ].z = fRandom( ZMIN, ZMAX );
        points[ i ].w = 1.f;
    }
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

    glBindBuffer( GL_SHADER_STORAGE_BUFFER, velSSbo );
    struct vel *vels = (vel *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(vel), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT );
    for( int i = 0; i < NUM_PARTICLES; i++ )
    {
        vels[ i ].vx = fRandom( VXMIN, VXMAX );
        vels[ i ].vy = fRandom( VYMIN, VYMAX );
        vels[ i ].vz = fRandom( VZMIN, VZMAX );
        vels[ i ].vw = 0.f;
    }
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

    glBindBuffer( GL_SHADER_STORAGE_BUFFER, colSSbo );
    struct color *colors = (color *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(color), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT );
    for( int i = 0; i < NUM_PARTICLES; i++ )
    {
        colors[ i ].r = fRandom( .3f, 1.f );
        colors[ i ].g = fRandom( .3f, 1.f );
        colors[ i ].b = fRandom( .3f, 1.f );
        colors[ i ].a = 1.f;
    }
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

    glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  4,  posSSbo );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  5,  velSSbo  );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  6,  colSSbo  );

    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
}

void ParticleSystem::render(matrix4D mvp)
{
    shaderRender->bind();
    shaderRender->modMatrix4fv("projectionView", mvp);

    glPointSize(2.f);

    glEnableClientState( GL_VERTEX_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, posSSbo );
    glVertexPointer( 4, GL_FLOAT, 0, (void *)0 );
    glBindBuffer( GL_ARRAY_BUFFER, colSSbo );
    glVertexPointer( 4, GL_FLOAT, 0, (void *)0 );

    glDrawArraysInstanced(GL_POINTS, 0, 1, NUM_PARTICLES);

}

