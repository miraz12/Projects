//
// Created by antchr-4 on 2/2/17.
//

#include "fluidsimulation.h"

FluidSimulation::FluidSimulation(float width, float height, float depth, vector3D pos) : width(width), height(height), depth(depth), grid(width, height, depth, pos)
{
    this->cellWidth = 10.f;
    //A = boost::numeric::ublas::mapped_matrix<int>(grid.colCount*grid.rowCount*grid.depthCount, grid.colCount*grid.rowCount*grid.depthCount);
}

void FluidSimulation::CalculateTimeStep()
{
    dt = kcfl * (cellWidth / maxVelocity);
}

void FluidSimulation::initParticles()
{
    this->markerPart.resize(NUM_PARTICLES);
    for( int i = 0; i < NUM_PARTICLES; i++ )
    {
        this->markerPart[ i ].pos[0] = fRandom( XMIN, XMAX );
        this->markerPart[ i ].pos[1] = fRandom( YMIN, YMAX );
        this->markerPart[ i ].pos[2] = fRandom( ZMIN, ZMAX );
        this->markerPart[ i ].pos[3] = 1.f;

        this->markerPart[ i ].vel[0] = fRandom( VXMIN, VXMAX );
        this->markerPart[ i ].vel[1] = fRandom( VYMIN, VYMAX );
        this->markerPart[ i ].vel[2] = fRandom( VZMIN, VZMAX );
        this->markerPart[ i ].vel[3] = 0.f;

        this->markerPart[ i ].col[0] = fRandom( .3f, 1.f );
        this->markerPart[ i ].col[1] = fRandom( .3f, 1.f );
        this->markerPart[ i ].col[2] = fRandom( .3f, 1.f );
        this->markerPart[ i ].col[3] = 1.f;
    }
}

float FluidSimulation::fRandom(float low, float high)
{
    long random();

    float r = (float)rand();
    return (low + r * (high-low) / (float)RAND_MAX);
}

void FluidSimulation::UpdateGrid()
{
    int todoOut = 0;
    for (auto it : this->grid.macCells)
    {
        it.second->layer = -1;
        todoOut++;
    }
    this->grid.macCells.begin();
    for (int i = 0; i < this->markerPart.size(); ++i)   //Set all cells with marker particles to fluid.
    {
        vector3D pos = grid.findClosestCell(vector3D(markerPart[i].pos.x(), markerPart[i].pos.y(), markerPart[i].pos.z()));

        std::tr1::unordered_map<float, Cell*>::iterator got = grid.findCell(pos.x(), pos.y(), pos.z());
        if (got == grid.macCells.end())
        {
            Cell* ce = new Cell(Cell::FLUID, 0);
            grid.addCell(pos.x(), pos.y(), pos.z(), ce);
        }
        else
        {
            Cell* c = got->second;
            if (c->type != Cell::SOLID)
            {
                c->type = Cell::FLUID;
                c ->layer = 0;
            }
        }
    }
    for (int j = 1; j <= std::max(2, (int)kcfl); ++j)
    {
        std::tr1::unordered_map<float, Cell*>::iterator it = grid.macCells.begin();

        for (it; it != grid.macCells.end(); ++it)
        {
            if ((it->second->type == Cell::FLUID || it->second->type == Cell::AIR) && (it->second->layer == j - 1)) //For each fluid or air cell with layer == j - 1;
            {
                vector3D pos = it->second->position;
                std::vector<vector3D> neigh = grid.getNeighbourPos(pos.x(), pos.y(), pos.z());
                for (int i = 0; i < neigh.size(); ++i)  //For cells neighbors
                {
                    if (grid.cellInGrid(neigh[i].x(), neigh[i].y(), neigh[i].z()))   //If in grid
                    {
                        Cell* c = grid.getCell(neigh[i].x(), neigh[i].y(), neigh[i].z());
                        if((c->layer == -1) && (c->type != Cell::SOLID))
                        {
                            c->type = Cell::AIR;
                            c->layer = j;
                        }
                    }
                    else    //Else add to grid
                    {
                        Cell* c = new Cell;
                        if (grid.cellInBounds(neigh[i].x(), neigh[i].y(), neigh[i].z()))
                        {
                            c->type = Cell::AIR;
                        } else
                        {
                            c->type = Cell::SOLID;
                        }
                        c->layer = j;
                        grid.addCell(neigh[i].x(), neigh[i].y(), neigh[i].z(), c);
                    }
                }
            }
        }
    }
    std::tr1::unordered_map<float, Cell*>::iterator it = grid.macCells.begin();
    for (it; it != grid.macCells.end() ; it++)  //Delete all cells with layer == -1
    {
        if (it->second->layer == -1 && it->second->type != Cell::SOLID)
        {
            grid.removeCell(it->second->position.x(), it->second->position.y(), it->second->position.z());
        }
    }
    for (auto it : this->grid.macCells)
    {
        grid.findNeighbours(it.second->position, it.second);
    }

}

void FluidSimulation::AdvanceVelocityField()
{
    int size = grid.macCells.size();

    TraceParticle();                    //Apply convection using backwards particles
    ApplyExternal();                    //Apply external forces. (Like gravity)
    ApplyViscosity();                   //Viscosity
    std::vector<vector3D> g;
    CalculatePressure();                //Solve pressure
    ApplyPressureToVelocityField();     //Apply pressure to grid
    ExtrapolateFluidVel();              //Extrapolate fluid velocity to neigbours
    SetSolidVelocity();                 //Set velocities of solid cells
}

void FluidSimulation::TraceParticle()
{
    for(auto it : grid.macCells)
    {
        if (it.second->type == Cell::FLUID)
        {
            float x = it.second->position.x();
            float y = it.second->position.y();
            float z = it.second->position.z();

            //RK2 Integration
            vector3D v = getVelocity(x, y, z);
            v = getVelocity(x + 0.5f * dt * v.x(), y + 0.5f * dt * v.y(), z + 0.5f * dt * v.z());
            it.second->stagedVel = vector3D(x, y, z) + dt * v;
        }
    }
}

vector3D FluidSimulation::getVelocity(float x, float y, float z)
{
    vector3D v;
    v[0] = grid.getInterpolatedValue(x/grid.cellWidth, y/grid.cellWidth - 0.5f ,z/grid.cellWidth-0.5f, 0);
    v[1] = grid.getInterpolatedValue(x/grid.cellWidth - 0.5f, y/grid.cellWidth ,z/grid.cellWidth-0.5f, 1);
    v[2] = grid.getInterpolatedValue(x/grid.cellWidth - 0.5f, y/grid.cellWidth - 0.5f ,z/grid.cellWidth, 2);
    return v;
}

void FluidSimulation::ApplyExternal()
{
    for(auto it : grid.macCells)
    {
        if (it.second->type == Cell::FLUID)
        {
            float x = it.second->position.x();
            float y = it.second->position.y();
            float z = it.second->position.z();

            grid.getCell(x, y, z)->stagedVel += gravity;
        }
    }
}

void FluidSimulation::ApplyViscosity()
{
    for(auto it : grid.macCells)
    {
        if (it.second->type == Cell::FLUID)
        {
            float x = it.second->position.x();
            float y = it.second->position.y();
            float z = it.second->position.z();

            vector3D v;
            v[0] = grid.getLaplacianTimes(x/grid.cellWidth, y/grid.cellWidth - 0.5f ,z/grid.cellWidth-0.5f, 0);
            v[1] = grid.getLaplacianTimes(x/grid.cellWidth - 0.5f, y/grid.cellWidth ,z/grid.cellWidth-0.5f, 1);
            v[2] = grid.getLaplacianTimes(x/grid.cellWidth - 0.5f, y/grid.cellWidth - 0.5f ,z/grid.cellWidth, 2);
            it.second->stagedVel = it.second->stagedVel +  v;
        }
    }
}

void FluidSimulation::CalculatePressure()
{
    PressureSolverParameters param;
    param.cellwidth = grid.cellWidth;
    param.deltaTime = this->dt;
    param.velocityField = &grid;

    int numFluid = 0;
    for (auto it : grid.macCells)
    {
        if (it.second->type == Cell::FLUID)
        {
            numFluid++;
        }
    }
    VectorXd p(numFluid);
    PressureSolver pSolver = PressureSolver();
    pSolver.solve(param, p);

    this->pressureVector.resize(numFluid);
    for (int i = 0; i < p.size(); ++i)
    {
        this->pressureVector[i] = (float)p[i];
    }


    std::map<int, int> temp(pSolver.fluidVIndex);
    fluidVIndex = temp;


}

void FluidSimulation::ApplyPressureToVelocityField()
{
    for (int k = 0; k < grid.depthCount; k++)
    {
        for (int j = 0; j < grid.rowCount; j++)
        {
            for (int i = 0; i < grid.colCount + 1; i++)
            {
                if (grid.cellInGrid(i,j,k))
                {
                    if (grid.getCellIndex(i,j,k)->type == Cell::SOLID)
                    {
                        grid.getCellIndex(i, j, k)->stagedVel[0] = 0.0f;
                    }

                    if (grid.getCellIndex(i,j,k)->type == Cell::FLUID && grid.getCellIndex(i,j,k)->type != Cell::SOLID)
                    {
                        ApplyPressureToFaceU(i,j,k);
                    }
                }
            }
        }
    }

    for (int k = 0; k < grid.depthCount; k++)
    {
        for (int j = 0; j < grid.rowCount + 1; j++)
        {
            for (int i = 0; i < grid.colCount; i++)
            {
                if (grid.cellInGrid(i,j,k))
                {
                    if (grid.getCellIndex(i, j, k)->type == Cell::SOLID)
                    {
                        grid.getCellIndex(i, j, k)->stagedVel[0] = 0.0f;
                    }

                    if (grid.getCellIndex(i, j, k)->type == Cell::FLUID &&
                        grid.getCellIndex(i, j, k)->type != Cell::SOLID)
                    {
                        ApplyPressureToFaceV(i, j, k);
                    }
                }
            }
        }
    }

    for (int k = 0; k < grid.depthCount+1; k++)
    {
        for (int j = 0; j < grid.rowCount; j++)
        {
            for (int i = 0; i < grid.colCount; i++)
            {
                if (grid.cellInGrid(i,j,k))
                {
                    if (grid.getCellIndex(i, j, k)->type == Cell::SOLID)
                    {
                        grid.getCellIndex(i, j, k)->stagedVel[0] = 0.0f;
                    }

                    if (grid.getCellIndex(i, j, k)->type == Cell::FLUID &&
                        grid.getCellIndex(i, j, k)->type != Cell::SOLID)
                    {
                        ApplyPressureToFaceW(i, j, k);
                    }
                }
            }
        }
    }
}

void FluidSimulation::ApplyPressureToFaceU(int i, int j, int k)
{
    double usolid = 0.0;   // solids are stationary
    double scale = dt / (1.f * grid.cellWidth);
    double invscale = 1.0 / scale;

    int ci = i - 1; int cj = j; int ck = k;

    double p0, p1;
    if (!grid.getCellIndex(ci , cj, ck)->type == Cell::SOLID && !grid.getCellIndex(ci + 1, cj, ck)->type == Cell::SOLID) {
        p0 = pressureVector[fluidVIndex[541 * ci + 79 * cj + 31 * ck]];
        p1 = pressureVector[fluidVIndex[(541 * (ci+1)) + 79 * cj + 31 * ck]];
    } else if (grid.getCellIndex(ci , cj, ck)->type == Cell::SOLID) {
        p0 = pressureVector[fluidVIndex[(541 * (ci+1)) + 79 * cj + 31 * ck]] - invscale*(grid.getCellIndex(i, j, k)->velocity.x() - usolid);
        p1 = pressureVector[fluidVIndex[(541 * (ci+1)) + 79 * cj + 31 * ck]];
    } else {
        p0 = pressureVector[fluidVIndex[541 * ci + 79 * cj + 31 * ck]];
        p1 = pressureVector[fluidVIndex[541 * ci + 79 * cj + 31 * ck]] + invscale*(grid.getCellIndex(i, j, k)->velocity.x() - usolid);
    }

    double unext = grid.getCellIndex(i, j, k)->stagedVel.x() - scale*(p1 - p0);
    grid.getCellIndex(i, j, k)->stagedVel[0] = unext;
}

void FluidSimulation::ApplyPressureToFaceV(int i, int j, int k)
{
    double usolid = 0.0;   // solids are stationary
    double scale = dt / (1.f * grid.cellWidth);
    double invscale = 1.0 / scale;

    int ci = i; int cj = j - 1; int ck = k;

    double p0, p1;
    if (!grid.getCellIndex(ci , cj, ck)->type == Cell::SOLID && !grid.getCellIndex(ci, cj + 1, ck)->type == Cell::SOLID) {
        p0 = pressureVector[fluidVIndex[541 * ci + 79 * cj + 31 * ck]];
        p1 = pressureVector[fluidVIndex[(541 * (ci)) + 79 * (cj+1) + 31 * ck]];
    } else if (grid.getCellIndex(ci , cj, ck)->type == Cell::SOLID) {
        p0 = pressureVector[fluidVIndex[(541 * (ci)) + 79 * (cj+1) + 31 * ck]] - invscale*(grid.getCellIndex(i, j, k)->velocity.y() - usolid);
        p1 = pressureVector[fluidVIndex[(541 * (ci)) + 79 * (cj+1) + 31 * ck]];
    } else {
        p0 = pressureVector[fluidVIndex[541 * ci + 79 * cj + 31 * ck]];
        p1 = pressureVector[fluidVIndex[541 * ci + 79 * cj + 31 * ck]] + invscale*(grid.getCellIndex(i, j, k)->velocity.y() - usolid);
    }

    double unext = grid.getCellIndex(i, j, k)->stagedVel.y() - scale*(p1 - p0);
    grid.getCellIndex(i, j, k)->stagedVel[1] = unext;
}

void FluidSimulation::ApplyPressureToFaceW(int i, int j, int k)
{
    double usolid = 0.0;   // solids are stationary
    double scale = dt / (1.f * grid.cellWidth);
    double invscale = 1.0 / scale;

    int ci = i; int cj = j; int ck = k-1;

    double p0, p1;
    if (!grid.getCellIndex(ci , cj, ck)->type == Cell::SOLID && !grid.getCellIndex(ci, cj, ck+1)->type == Cell::SOLID) {
        p0 = pressureVector[fluidVIndex[541 * ci + 79 * cj + 31 * ck]];
        p1 = pressureVector[fluidVIndex[(541 * (ci)) + 79 * cj + 31 * (ck+1)]];
    } else if (grid.getCellIndex(ci , cj, ck)->type == Cell::SOLID) {
        p0 = pressureVector[fluidVIndex[(541 * (ci+1)) + 79 * cj + 31 * (ck+1)]] - invscale*(grid.getCellIndex(i, j, k)->velocity.z() - usolid);
        p1 = pressureVector[fluidVIndex[(541 * (ci+1)) + 79 * cj + 31 * (ck+1)]];
    } else {
        p0 = pressureVector[fluidVIndex[541 * ci + 79 * cj + 31 * ck]];
        p1 = pressureVector[fluidVIndex[541 * ci + 79 * cj + 31 * ck]] + invscale*(grid.getCellIndex(i, j, k)->velocity.z() - usolid);
    }

    double unext = grid.getCellIndex(i, j, k)->stagedVel.z() - scale*(p1 - p0);
    grid.getCellIndex(i, j, k)->stagedVel[2] = unext;
}

void FluidSimulation::ExtrapolateFluidVel()
{
    for (auto it : this->grid.macCells)
    {
        if (it.second->type == Cell::FLUID)
        {
            it.second->layer = 0;
        } else
        {
            it.second->layer = -1;
        }
    }
    for (int i = 1; i <= std::max(2, (int)kcfl) ; ++i)
    {
        for (auto it : this->grid.macCells)
        {
            int size = grid.macCells.size();
            if(it.second->layer == -1 && it.second->type != Cell::SOLID)
            {
                vector3D ind = grid.findIndex(it.second->position);
                int j = ind.x();
                int i = ind.y();
                int k = ind.z();

                bool fNe = false;
                vector3D avgV;
                if(grid.cellInGrid(i-1,j,k))
                {
                    if (grid.getCellIndex(i-1, j, k)->layer == i - 1)
                        fNe = true;
                }
                if(grid.cellInGrid(i+1,j,k))
                {
                    if (grid.getCellIndex(i+1, j, k)->layer == i - 1)
                        fNe = true;
                }
                if(grid.cellInGrid(i,j+1,k))
                {
                    if (grid.getCellIndex(i, j+1, k)->layer == i - 1)
                        fNe = true;
                }
                if(grid.cellInGrid(i,j-1,k))
                {
                    if (grid.getCellIndex(i, j-1, k)->layer == i - 1)
                        fNe = true;
                }
                if(grid.cellInGrid(i,j,k-1))
                {
                    if (grid.getCellIndex(i, j, k-1)->layer == i - 1)
                        fNe = true;
                }
                if(grid.cellInGrid(i,j,k+1))
                {
                    if (grid.getCellIndex(i, j, k+1)->layer == i - 1)
                        fNe = true;
                }

                if (fNe)
                {
                    //velocity u value ex
                    if(grid.cellInGrid(i-1,j,k))
                    {
                        if (grid.getCellIndex(i-1,j,k)->type != Cell::FLUID)
                        {
                            int count = 0;
                            if(grid.cellInGrid(i-1,j,k))
                            {
                                if (grid.getCellIndex(i-1,j,k)->layer == i - 1)
                                {
                                    avgV[0] += grid.getCellIndex(i-1,j,k)->stagedVel[0];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i+1,j,k))
                            {
                                if (grid.getCellIndex(i+1,j,k)->layer == i - 1)
                                {
                                    avgV[0] += grid.getCellIndex(i+1,j,k)->stagedVel[0];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j+1,k))
                            {
                                if (grid.getCellIndex(i,j+1,k)->layer == i - 1)
                                {
                                    avgV[0] += grid.getCellIndex(i,j+1,k)->stagedVel[0];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j-1,k))
                            {
                                if (grid.getCellIndex(i,j-1,k)->layer == i - 1)
                                {
                                    avgV[0] += grid.getCellIndex(i,j-1,k)->stagedVel[0];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j,k-1))
                            {
                                if (grid.getCellIndex(i,j,k-1)->layer == i - 1)
                                {
                                    avgV[0] += grid.getCellIndex(i,j,k-1)->stagedVel[0];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j,k+1))
                            {
                                if (grid.getCellIndex(i,j,k+1)->layer == i - 1)
                                {
                                    avgV[0] += grid.getCellIndex(i,j,k+1)->stagedVel[0];
                                    count++;
                                }
                            }

                            if (count != 0)
                                avgV[0] /= count;
                        }
                    }
                    //velocity v value ex
                    if(grid.cellInGrid(i,j-1,k))
                    {
                        if (grid.getCellIndex(i,j-1,k)->type != Cell::FLUID)
                        {
                            int count = 0;
                            if(grid.cellInGrid(i-1,j,k))
                            {
                                if (grid.getCellIndex(i-1,j,k)->layer == i - 1)
                                {
                                    avgV[1] += grid.getCellIndex(i-1,j,k)->stagedVel[1];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i+1,j,k))
                            {
                                if (grid.getCellIndex(i+1,j,k)->layer == i - 1)
                                {
                                    avgV[1] += grid.getCellIndex(i+1,j,k)->stagedVel[1];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j+1,k))
                            {
                                if (grid.getCellIndex(i,j+1,k)->layer == i - 1)
                                {
                                    avgV[1] += grid.getCellIndex(i,j+1,k)->stagedVel[1];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j-1,k))
                            {
                                if (grid.getCellIndex(i,j-1,k)->layer == i - 1)
                                {
                                    avgV[1] += grid.getCellIndex(i,j-1,k)->stagedVel[1];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j,k-1))
                            {
                                if (grid.getCellIndex(i,j,k-1)->layer == i - 1)
                                {
                                    avgV[1] += grid.getCellIndex(i,j,k-1)->stagedVel[1];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j,k+1))
                            {
                                if (grid.getCellIndex(i,j,k+1)->layer == i - 1)
                                {
                                    avgV[1] += grid.getCellIndex(i,j,k+1)->stagedVel[1];
                                    count++;
                                }
                            }

                            if (count != 0)
                                avgV[1] /= count;
                        }
                    }
                    //velocity w value ex
                    if(grid.cellInGrid(i,j,k-1))
                    {
                        if (grid.getCellIndex(i,j,k-1)->type != Cell::FLUID)
                        {
                            int count = 0;
                            if(grid.cellInGrid(i-1,j,k))
                            {
                                if (grid.getCellIndex(i-1,j,k)->layer == i - 1)
                                {
                                    avgV[2] += grid.getCellIndex(i-1,j,k)->stagedVel[2];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i+1,j,k))
                            {
                                if (grid.getCellIndex(i+1,j,k)->layer == i - 1)
                                {
                                    avgV[2] += grid.getCellIndex(i+1,j,k)->stagedVel[2];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j+1,k))
                            {
                                if (grid.getCellIndex(i,j+1,k)->layer == i - 1)
                                {
                                    avgV[2] += grid.getCellIndex(i,j+1,k)->stagedVel[2];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j-1,k))
                            {
                                if (grid.getCellIndex(i,j-1,k)->layer == i - 1)
                                {
                                    avgV[2] += grid.getCellIndex(i,j-1,k)->stagedVel[2];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j,k-1))
                            {
                                if (grid.getCellIndex(i,j,k-1)->layer == i - 1)
                                {
                                    avgV[2] += grid.getCellIndex(i,j,k-1)->stagedVel[2];
                                    count++;
                                }
                            }
                            if(grid.cellInGrid(i,j,k+1))
                            {
                                if (grid.getCellIndex(i,j,k+1)->layer == i - 1)
                                {
                                    avgV[2] += grid.getCellIndex(i,j,k+1)->stagedVel[2];
                                    count++;
                                }
                            }

                            if (count != 0)
                                avgV[2] /= count;
                        }
                    }
                }
                it.second->stagedVel = avgV;
                it.second->layer = i;
            }
        }
    }
}

void FluidSimulation::SetSolidVelocity()
{
    for (auto it : grid.macCells)
    {
        if(it.second->type == Cell::SOLID)
        {
            vector3D ind = grid.findIndex(it.second->position);
            int j = ind.x();
            int i = ind.y();
            int k = ind.z();

            if(grid.cellInGrid(i-1,j,k))
            {
                if(grid.getCellIndex(i-1,j,k)->type != Cell::SOLID)
                {
                    if(grid.getCellIndex(i-1,j,k)->stagedVel[0] > 0.f)
                    {
                        grid.getCellIndex(i-1,j,k)->stagedVel[0] = 0.0f;
                    }
                }
            }
            if(grid.cellInGrid(i+1,j,k))
            {
                if(grid.getCellIndex(i+1,j,k)->type != Cell::SOLID)
                {
                    if(grid.getCellIndex(i+1,j,k)->stagedVel[0] < 0.f)
                    {
                        grid.getCellIndex(i+1,j,k)->stagedVel[0] = 0.0f;
                    }
                }
            }
            if(grid.cellInGrid(i,j+1,k))
            {
                if(grid.getCellIndex(i,j+1,k)->type != Cell::SOLID)
                {
                    if(grid.getCellIndex(i,j+1,k)->stagedVel[1] < 0.f)
                    {
                        grid.getCellIndex(i,j+1,k)->stagedVel[1] = 0.0f;
                    }
                }
            }
            if(grid.cellInGrid(i,j-1,k))
            {
                if(grid.getCellIndex(i,j-1,k)->type != Cell::SOLID)
                {
                    if(grid.getCellIndex(i,j-1,k)->stagedVel[1] > 0.f)
                    {
                        grid.getCellIndex(i,j-1,k)->stagedVel[1] = 0.0f;
                    }
                }
            }
            if(grid.cellInGrid(i,j,k-1))
            {
                if(grid.getCellIndex(i,j,k-1)->type != Cell::SOLID)
                {
                    if(grid.getCellIndex(i,j,k-1)->stagedVel[2] > 0.f)
                    {
                        grid.getCellIndex(i,j,k-1)->stagedVel[2] = 0.0f;
                    }
                }
            }
            if(grid.cellInGrid(i,j,k+1))
            {
                if(grid.getCellIndex(i,j,k+1)->type != Cell::SOLID)
                {
                    if(grid.getCellIndex(i,j,k+1)->stagedVel[2] < 0.f)
                    {
                        grid.getCellIndex(i,j,k+1)->stagedVel[2] = 0.0f;
                    }
                }
            }
        }
    }
}





