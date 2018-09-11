
#define DENSITY (1.0/10000.0)
#define MAX_VERTEXES_PER_VORONOI 16

struct WorleyContex {
    uint32_t seed;
    cpFloat cellSize;
    int width, height;
    cpBB bb;
    cpVect focus;
};

static inline cpVect
HashVect(uint32_t x, uint32_t y, uint32_t seed)
{
//  cpFloat border = 0.21f;
    cpFloat border = 0.05f;
    uint32_t h = (x*1640531513 ^ y*2654435789) + seed;
    
    return cpv(
        cpflerp(border, 1.0f - border, (cpFloat)(      h & 0xFFFF)/(cpFloat)0xFFFF),
        cpflerp(border, 1.0f - border, (cpFloat)((h>>16) & 0xFFFF)/(cpFloat)0xFFFF)
    );
}

static cpVect
WorleyPoint(int i, int j, struct WorleyContex *context)
{
    cpFloat size = context->cellSize;
    int width = context->width;
    int height = context->height;
    cpBB bb = context->bb;
    
//  cpVect fv = cpv(0.5, 0.5);
    cpVect fv = HashVect(i, j, context->seed);
    
    return cpv(
        cpflerp(bb.l, bb.r, 0.5f) + size*(i + fv.x -  width*0.5f),
        cpflerp(bb.b, bb.t, 0.5f) + size*(j + fv.y - height*0.5f)
    );
}

static void 
DrawPoint(cpVect p) {
    nvgBeginPath(vg);
    nvgCircle(vg, p.x, p.y, 2.0);
    nvgStrokeWidth(vg, 1.0);
    nvgStrokeColor(vg, nvgRGBAf(1.0, 1.0, 1.0, 1.0));
    nvgStroke(vg);
}

static void 
DrawPoly(cpVect* p, int count) {
    nvgBeginPath(vg);
    nvgMoveTo(vg, p[0].x, p[0].y);
    for(int i=1; i<count; i++) {
        nvgLineTo(vg, p[i].x, p[i].y);
    }
    nvgLineTo(vg, p[0].x, p[0].y);
    nvgStrokeWidth(vg, 1.0);
    nvgStrokeColor(vg, nvgRGBAf(1.0, 1.0, 1.0, 1.0));
    nvgStroke(vg);
}

static int
ClipCell(
    cpVect center, 
    int i, int j, 
    struct WorleyContex *context, 
    cpVect *verts, 
    cpVect *clipped, 
    int count) {

    cpVect other = WorleyPoint(i, j, context);
    
    cpVect n = cpvsub(other, center);
    cpFloat dist = cpvdot(n, cpvlerp(center, other, 0.5f));
    int clipped_count = 0;
    for(int j=0, i=count-1; j<count; i=j, j++){
        
        cpVect a = verts[i];
        cpFloat a_dist = cpvdot(a, n) - dist;
        
        if(a_dist <= 0.0){
            clipped[clipped_count] = a;
            clipped_count++;
        }

        cpVect b = verts[j];
        cpFloat b_dist = cpvdot(b, n) - dist;
        
        if(a_dist*b_dist < 0.0f){
            cpFloat t = cpfabs(a_dist)/(cpfabs(a_dist) + cpfabs(b_dist));            
            clipped[clipped_count] = cpvlerp(a, b, t);
            clipped_count++;
        }
    }
    return clipped_count;
}

static void
ShatterCell(
    cpVect cell,
    int cell_i,
    int cell_j,
    struct WorleyContex *context) {
      
    cpVect *ping = (cpVect *) alloca(MAX_VERTEXES_PER_VORONOI*sizeof(cpVect));
    cpVect *pong = (cpVect *) alloca(MAX_VERTEXES_PER_VORONOI*sizeof(cpVect));
    
    int count = 4;
    count = (count > MAX_VERTEXES_PER_VORONOI ? MAX_VERTEXES_PER_VORONOI : count);

    ping[0] = (cpVect) {100, 100};
    ping[1] = (cpVect) {700, 100};
    ping[2] = (cpVect) {700, 700};
    ping[3] = (cpVect) {100, 700};
    
    for(int i=0; i<context->width; i++){
        for(int j=0; j<context->height; j++){
            if(!(i == cell_i && j == cell_j)) {
                count = ClipCell(cell, i, j, context, ping, pong, count);
                memcpy(ping, pong, count*sizeof(cpVect));
            }
        }
    }
    DrawPoly(ping, count);                                                                                                                                                                                                                                                                                    
}

static void
DrawWorley() {
    cpBB bb = {100,100,700,700};
    cpFloat cellSize = 100.0;
    int width  = (int)((bb.r - bb.l)/cellSize) + 1;
    int height = (int)((bb.t - bb.b)/cellSize) + 1;
    struct WorleyContex context = {0, cellSize, width, height, bb, cpv(400,400)};
    for(int i=0; i<context.width; i++){
        for(int j=0; j<context.height; j++){
            cpVect cell = WorleyPoint(i, j, &context);
            DrawPoint(cell);
            ShatterCell(cell, i, j, &context);
        }
    }
}
