//
// Created by antchr-4 on 2/16/17.
//

#include "pressuresolver.h"


/********************************************************************************
    VectorXd
********************************************************************************/

VectorXd::VectorXd() {
}

VectorXd::VectorXd(int size) : _vector(size, 0.0) {
}

VectorXd::VectorXd(int size, double fill) : _vector(size, fill) {
}

VectorXd::VectorXd(VectorXd &vector) {
    _vector.reserve(vector.size());
    for (unsigned int i = 0; i < vector.size(); i++) {
        _vector.push_back(vector[i]);
    }
}

VectorXd::~VectorXd() {
}

const double VectorXd::operator[](int i) const {
    return _vector[i];
}

double& VectorXd::operator[](int i) {
    return _vector[i];
}

void VectorXd::fill(double fill) {
    for (unsigned int i = 0; i < _vector.size(); i++) {
        _vector[i] = fill;
    }
}

double VectorXd::dot(VectorXd &vector) {
    double sum = 0.0;
    for (unsigned int i = 0; i < _vector.size(); i++) {
        sum += _vector[i] * vector._vector[i];
    }

    return sum;
}

double VectorXd::absMaxCoeff() {
    double max = -std::numeric_limits<double>::infinity();
    for (unsigned int i = 0; i < _vector.size(); i++) {
        if (fabs(_vector[i]) > max) {
            max = fabs(_vector[i]);
        }
    }

    return max;
}

/********************************************************************************
    MatrixCoefficients
********************************************************************************/

MatrixCoefficients::MatrixCoefficients() {
}

MatrixCoefficients::MatrixCoefficients(int size) : cells(size, MatrixCell()) {
}

MatrixCoefficients::~MatrixCoefficients() {
}

const MatrixCell MatrixCoefficients::operator[](int i) const
{
    return cells[i];
}

MatrixCell& MatrixCoefficients::operator[](int i)
{
    return cells[i];
}

/********************************************************************************
    PressureSolver
********************************************************************************/

PressureSolver::PressureSolver()
{}

void PressureSolver::solve(PressureSolverParameters params, VectorXd& pressure)     // Ap = B
{
    this->deltaTime = params.deltaTime;
    this->cellWidth = params.cellwidth;
    this->grid = params.velocityField;

    pressure.fill(0.0);

    IndexKeyMap();      //Give all fluid cells a index by "left, up, out"

    VectorXd b(indexKeyMap.size());
    CalculateNegativeDivergence(b);     //Setup negative divergence vector b

    MatrixCoefficients A(indexKeyMap.size());
    CalculateMatrixCoefficients(A);

    VectorXd precon(indexKeyMap.size());
    CalculatePreconditionerVector(A, precon);

    SolvePressureSystem(A, b, precon, pressure);


    return;
}

void PressureSolver::IndexKeyMap()
{
    for (int z = -grid->depthCount/2; z < grid->depthCount/2; ++z)
    {
        for (int y = -grid->colCount/2; y < grid->colCount/2; ++y)
        {
            for (int x = -grid->rowCount/2; x < grid->rowCount/2; ++x)
            {
                if (grid->cellInGrid(x,y,z))
                {
                    if (grid->getCellIndex(x, y, z)->type == Cell::FLUID)
                    {
                        fluidVIndex[541 * x + 79 * y + 31 * z] = indexKeyMap.size();
                        indexKeyMap.push_back(vector3D(x,y,z));
                    }
                }
            }
        }
    }
}

void PressureSolver::CalculateNegativeDivergence(VectorXd &b)
{
    double scale = 1.0f / (float)cellWidth;

    for (unsigned int idx = 0; idx < indexKeyMap.size(); idx++)
    {
        Cell* c = grid->getCellIndex(indexKeyMap[idx].x(), indexKeyMap[idx].y(), indexKeyMap[idx].z());
        double value = -scale * grid->getDivergenceInd(indexKeyMap[idx]);
        b[idx] = value;
    }

    float usolid = 0.0;
    float vsolid = 0.0;
    float wsolid = 0.0;
    for (unsigned int idx = 0; idx < indexKeyMap.size(); idx++)
    {
        int i = indexKeyMap[idx].x();
        int j = indexKeyMap[idx].y();
        int k = indexKeyMap[idx].z();

        if ( grid->getCellIndex(i+1, j, k))
        {
            b[idx] -= (float)scale*(grid->getCellIndex(i, j, k)->velocity.x() - usolid);
        }
        if ( grid->getCellIndex(i-1, j, k))
        {
            b[idx] -= (float)scale*(grid->getCellIndex(i+1, j, k)->velocity.x() - usolid);
        }
        if ( grid->getCellIndex(i, j+1, k))
        {
            b[idx] -= (float)scale*(grid->getCellIndex(i, j, k)->velocity.y() - vsolid);
        }
        if ( grid->getCellIndex(i, j-1, k))
        {
            b[idx] -= (float)scale*(grid->getCellIndex(i, j+1, k)->velocity.y() - vsolid);
        }
        if ( grid->getCellIndex(i, j, k+1))
        {
            b[idx] -= (float)scale*(grid->getCellIndex(i, j, k)->velocity.z() - wsolid);
        }
        if ( grid->getCellIndex(i, j, k-1))
        {
            b[idx] -= (float)scale*(grid->getCellIndex(i, j, k+1)->velocity.z() - wsolid);
        }
    }
}

void PressureSolver::CalculateMatrixCoefficients(MatrixCoefficients &A)
{
    for (unsigned int idx = 0; idx < indexKeyMap.size(); idx++)
    {
        int i = indexKeyMap[idx].x();
        int j = indexKeyMap[idx].y();
        int k = indexKeyMap[idx].z();

        int n = grid->NumFluidAirNeighbours(indexKeyMap[idx]);
        A.cells[idx].diag = (char)n;

        if (grid->getCellIndex(i + 1, j, k)->type == Cell::FLUID)
        {
            A.cells[idx].plusi = 0x01;
        }
        if (grid->getCellIndex(i, j+1, k)->type == Cell::FLUID)
        {
            A.cells[idx].plusj = 0x01;
        }
        if (grid->getCellIndex(i, j, k+1)->type == Cell::FLUID)
        {
            A.cells[idx].plusk = 0x01;
        }
    }
}

void PressureSolver::CalculatePreconditionerVector(MatrixCoefficients &A, VectorXd &precon)
{
    double scale = deltaTime / (density * cellWidth * cellWidth);
    double negscale = -scale;

    double tau = 0.97; //Tuning constant?
    double sigma = 0.25;    //Safety constant?

    for (int idx = 0; idx < indexKeyMap.size(); ++idx)
    {
        int i = indexKeyMap[idx].x();
        int j = indexKeyMap[idx].y();
        int k = indexKeyMap[idx].z();

        int vidx_im1 = fluidVIndex[(541 * i-1) + (79 * j) + (31 * k)];
        int vidx_jm1 = fluidVIndex[(541 * i) + (79 * j-1) + (31 * k)];
        int vidx_km1 = fluidVIndex[(541 * i) + (79 * j) + (31 * k-1)];

        double diag = (double)A[idx].diag*scale;

        double plusi_im1 = vidx_im1 != -1 ? (double)A[vidx_im1].plusi * negscale : 0.0;
        double plusi_jm1 = vidx_jm1 != -1 ? (double)A[vidx_jm1].plusi * negscale : 0.0;
        double plusi_km1 = vidx_km1 != -1 ? (double)A[vidx_km1].plusi * negscale : 0.0;

        double plusj_im1 = vidx_im1 != -1 ? (double)A[vidx_im1].plusj * negscale : 0.0;
        double plusj_jm1 = vidx_jm1 != -1 ? (double)A[vidx_jm1].plusj * negscale : 0.0;
        double plusj_km1 = vidx_km1 != -1 ? (double)A[vidx_km1].plusj * negscale : 0.0;

        double plusk_im1 = vidx_im1 != -1 ? (double)A[vidx_im1].plusk * negscale : 0.0;
        double plusk_jm1 = vidx_jm1 != -1 ? (double)A[vidx_jm1].plusk * negscale : 0.0;
        double plusk_km1 = vidx_km1 != -1 ? (double)A[vidx_km1].plusk * negscale : 0.0;

        double precon_im1 = vidx_im1 != -1 ? precon[vidx_im1] : 0.0;
        double precon_jm1 = vidx_jm1 != -1 ? precon[vidx_jm1] : 0.0;
        double precon_km1 = vidx_km1 != -1 ? precon[vidx_km1] : 0.0;

        double v1 = plusi_im1 * precon_im1;
        double v2 = plusj_jm1 * precon_jm1;
        double v3 = plusk_km1 * precon_km1;
        double v4 = precon_im1 * precon_im1;
        double v5 = precon_jm1 * precon_jm1;
        double v6 = precon_km1 * precon_km1;

        double e = diag - v1*v1 - v2*v2 - v3*v3 -
                   tau*(plusi_im1*(plusj_im1 + plusk_im1)*v4 +
                        plusj_jm1*(plusi_jm1 + plusk_jm1)*v5 +
                        plusk_km1*(plusi_km1 + plusj_km1)*v6);

        if (e < sigma*diag) {
            e = diag;
        }

        if (fabs(e) > 10e-9) {
            precon[idx] = 1.0 / sqrt(e);
        }

    }
}

void PressureSolver::SolvePressureSystem(MatrixCoefficients &A, VectorXd &b, VectorXd &precon, VectorXd &pressure)
{

    double tol = pressureSolveTolerance;
    if (b.absMaxCoeff() < tol) {
        return;
    }



    VectorXd residual(b);
    VectorXd auxillary(indexKeyMap.size());
    ApplyPreconditioner(A, precon, residual, auxillary);

    VectorXd search(auxillary);

    double alpha = 0.0;
    double beta = 0.0;
    double sigma = auxillary.dot(residual);
    double sigmaNew = 0.0;
    int iterationNumber = 0;

    while (iterationNumber < maxCGIterations) {
        ApplyMatrix(A, search, auxillary);
        alpha = sigma / auxillary.dot(search);
        AddScaledVector(pressure, search, alpha);
        AddScaledVector(residual, auxillary, -alpha);

        ApplyPreconditioner(A, precon, residual, auxillary);
        sigmaNew = auxillary.dot(residual);
        beta = sigmaNew / sigma;
        AddScaledVectors(auxillary, 1.0, search, beta, search);
        sigma = sigmaNew;

        iterationNumber++;

        if (iterationNumber % 10 == 0) {
            std::ostringstream ss;
            ss << "\tIteration #: " << iterationNumber <<
               "\tEstimated Error: " << ((b.absMaxCoeff())) << std::endl;
            //_logfile->print(ss.str());
        }
    }

    //_logfile->log("Iterations limit reached.\t Estimated error : "residual.absMaxCoeff(), 1);
}

void PressureSolver::ApplyPreconditioner(MatrixCoefficients &A, VectorXd &precon, VectorXd &residual, VectorXd &vect)
{
    double scale = deltaTime / (density*cellWidth*cellWidth);
    double negscale = -scale;

    // Solve A*q = residual
    VectorXd q(indexKeyMap.size());
    for (unsigned int idx = 0; idx < indexKeyMap.size(); idx++)
    {
        int i = indexKeyMap[idx].x();
        int j = indexKeyMap[idx].y();
        int k = indexKeyMap[idx].z();

        int vidx_im1 = fluidVIndex[(541 * i-1) + (79 * j) + (31 * k)];
        int vidx_jm1 = fluidVIndex[(541 * i) + (79 * j-1) + (31 * k)];
        int vidx_km1 = fluidVIndex[(541 * i) + (79 * j) + (31 * k-1)];

        double plusi_im1 = 0.0;
        double precon_im1 = 0.0;
        double q_im1 = 0.0;
        if (vidx_im1 != -1) {
            plusi_im1  = (double)A[vidx_im1].plusi * negscale;
            precon_im1 = precon[vidx_im1];
            q_im1      = q[vidx_im1];
        }

        double plusj_jm1 = 0.0;
        double precon_jm1 = 0.0;
        double q_jm1 = 0.0;
        if (vidx_jm1 != -1) {
            plusj_jm1  = (double)A[vidx_jm1].plusj * negscale;
            precon_jm1 = precon[vidx_jm1];
            q_jm1      = q[vidx_jm1];
        }

        double plusk_km1 = 0.0;
        double precon_km1 = 0.0;
        double q_km1 = 0.0;
        if (vidx_km1 != -1) {
            plusk_km1  = (double)A[vidx_km1].plusk * negscale;
            precon_km1 = precon[vidx_km1];
            q_km1      = q[vidx_km1];
        }

        double t = residual[idx] - plusi_im1 * precon_im1 * q_im1 -
                   plusj_jm1 * precon_jm1 * q_jm1 -
                   plusk_km1 * precon_km1 * q_km1;

        t = t*precon[idx];
        q[idx] = t;
    }

}

void PressureSolver::ApplyMatrix(MatrixCoefficients &A, VectorXd &x, VectorXd &result)
{
    double scale = deltaTime / (density * cellWidth * cellWidth);
    double negscale = -scale;

    for (unsigned int idx = 0; idx < indexKeyMap.size(); idx++)
    {
        int i = indexKeyMap[idx].x();
        int j = indexKeyMap[idx].y();
        int k = indexKeyMap[idx].z();


        // val = dot product of column vector x and idxth row of matrix A
        double val = 0.0;
        int vidx = this->fluidVIndex[(541 * i-1) + (79 * j) + (31 * k)];
        if (vidx != -1) { val += x[vidx]; }

        vidx = this->fluidVIndex[(541 * i+1) + (79 * j) + (31 * k)];
        if (vidx != -1) { val += x[vidx]; }

        vidx = this->fluidVIndex[(541 * i) + (79 * j-1) + (31 * k)];
        if (vidx != -1) { val += x[vidx]; }

        vidx = this->fluidVIndex[(541 * i) + (79 * j+1) + (31 * k)];
        if (vidx != -1) { val += x[vidx]; }

        vidx = this->fluidVIndex[(541 * i) + (79 * j) + (31 * k-1)];
        if (vidx != -1) { val += x[vidx]; }

        vidx = this->fluidVIndex[(541 * i-1) + (79 * j) + (31 * k+1)];
        if (vidx != -1) { val += x[vidx]; }

        val *= negscale;

        vidx = this->fluidVIndex[(541 * i) + (79 * j) + (31 * k)];
        val += (double)A.cells[vidx].diag * scale * x[vidx];

        result[vidx] = val;
    }

}

void PressureSolver::AddScaledVector(VectorXd &v1, VectorXd &v2, double scale)
{
    for (unsigned int idx = 0; idx < v1.size(); idx++) {
        v1[idx] += v2[idx]*scale;
    }
}

void PressureSolver::AddScaledVectors(VectorXd &v1, double s1, VectorXd &v2, double s2, VectorXd &result)
{
    for (unsigned int idx = 0; idx < v1.size(); idx++) {
        result[idx] = v1[idx]*s1 + v2[idx]*s2;
    }
}
