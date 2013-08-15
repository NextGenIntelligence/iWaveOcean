#pragma once
#include "Grid.h"
#include <object.h>
#include <iparamb2.h>
#include "IConvolution.h"

#define P 6 /* The radius of the iWave kernel. */

class Ocean
{
    int frame_num;

    INode* parent_node;
    INode** collision_nodes;
    int collision_nodes_count;

    IParamBlock2* ambient_pblock2;

    float dt;       /* Time between each frame of the simulation, i.e. 24 fps => dt = 0.4 */
    float alpha;    /* Wave damping factor. Realistic damping is 0.2 <= alpha <= 0.4, suggested is 0.3 */
    float gravity;  /* 9.8 m/s^2 * dt * dt */
    float sigma;    /* Used for strength of Gaussian smoothing of obstructions. sigma > 0.0, suggested is 1.0 */
    float wake_exp; /* Power of waves generated by an object's wake. wave_exp >= 1.0, suggested is 2.0 */

    int vertices_x;
    int vertices_y;
    int vertices_total;

    float width;
    float length;
    float height_scale;

    IConvolution<2>* gaussianConvolution;
    IConvolution<P>* verticalDerivConvolution;

    float *ambient;                 /* Ambient waves calculated by a FFT simulation. */
    float *obstruction_raw;         /* Water obstruction(s) before Gaussian smoothing. */
    float *obstruction;             /* Water obstruction(s). 1.0 = no obstruction, 0.0 = total obstruction. */
    float *source;                  /* Water source(s). 0.0 = no source. */
    float *height;                  /* Height map of waves. 0.0 = no height. */
    float *previous_height;         /* Height map of waves at previous time. */
    float *vertical_derivative;     /* Used for calculating convolution. */

    /* Propagates the waves in the simulation one step (one dt). */
    void PropagateWaves();

    /* Gets the Grid representation of the ocean waves at the current simulation time. */
    Grid* GetDisplayGrid();
public:

    /*
    Constructs a new Ocean object.
    \param startFrame the first frame number
    \param verticesX the number of vertices along the width; must be >= 2P+1; otherwise failure will occur while simulating
    \param verticesY the number of vertices along the length; must be >= 2P+1
    \param width the width of the plane
    \param length the length of the plane
    \param heightScale a factor that scales the height of the waves
    \param dt the difference in time between frames (e.g. for 24 fps, a normal dt is 1/24)
    \param alpha the wave damping factor; 0.2 <= alpha <= 0.4, suggested is 0.3
    \param sigma the Gaussian smoothing factor; sigma > 0.0, suggested is 1.0
    \param wakePower the strength of an object's wake; wavePower >= 1.0, suggested is 2.0
    \param parentNode the node hosting the wave simulation
    \param collisionNodes the nodes creating collisions and dynamics in the water
    \param numCollisionNodes the size of the collisionNodes array
    \param ambientPblock2 the parameter block containing the ambient wave parameters
    */
    Ocean(int startFrame, int verticesX, int verticesY, float width, float length, float heightScale, float dt, float alpha, float sigma, float wakePower, INode* parentNode, INode** collisionNodes, int numCollisionNodes, IParamBlock2* ambientPblock2);
    ~Ocean(void);

    /* Updates the obstructions and sources for the current simulation time. */
    void UpdateObstructions();

    /* Advances the simulation one step and returns the resultant grid. The Grid returned needs to be destroyed once it is unused. */
    Grid* NextGrid();
};

