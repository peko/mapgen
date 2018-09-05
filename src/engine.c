#include <stdio.h>

#include <chipmunk/chipmunk_private.h>

#ifdef NANOVG_GLEW
#  include <GL/glew.h>
#endif
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg_gl.h"

#include "engine.h"

void (*engine_update)(float dt) = NULL;
void (*engine_render)(float dt) = NULL;
void (*engine_key)(int key, int action) = NULL;

void errorcb(int error, const char* desc) {
	printf("GLFW error %d: %s\n", error, desc);
}

int blowup  = 0;
int premult = 0;

GLFWwindow* window;
NVGcontext* vg = NULL;
int font = -1;
double prevt = 0;

static void key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	NVG_NOTUSED(scancode);
	NVG_NOTUSED(mods);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
    if(engine_key!=NULL) engine_key(key, action);
}

int engine_init () {

	if (!glfwInit()) {
		printf("Failed to init GLFW.");
		return -1;
	}

	glfwSetErrorCallback(errorcb);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

// #ifdef DEMO_MSAA
// 	glfwWindowHint(GLFW_SAMPLES, 4);
// #endif

	window = glfwCreateWindow(800, 800, "Ants SVM", NULL, NULL);
//	window = glfwCreateWindow(1000, 600, "NanoVG", glfwGetPrimaryMonitor(), NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key);

	glfwMakeContextCurrent(window);
#ifdef NANOVG_GLEW
    if(glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
		return -1;
	}
#endif

#ifdef DEMO_MSAA
	vg = nvgCreateGL2(NVG_STENCIL_STROKES | NVG_DEBUG);
#else
	vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif
	if (vg == NULL) {
		printf("Could not init nanovg.\n");
		return -1;
	}
    font = nvgCreateFont(vg, "sans", "Roboto-Regular.ttf");
	//glfwSwapInterval(0);

	glfwSetTime(0);
	prevt = glfwGetTime();

	return 0;
}

void 
engine_deinit() {
	nvgDeleteGL2(vg);
    glfwDestroyWindow(window);
	glfwTerminate();
}

void 
engine_start() {
    while (!glfwWindowShouldClose(window)) {
		double mx, my, t, dt;
		int winWidth, winHeight;
		int fbWidth, fbHeight;
		float pxRatio;

		t = glfwGetTime();
		dt = t - prevt;
		prevt = t;
          
		if(engine_update!=NULL) engine_update(dt);
		
		glfwGetCursorPos(window, &mx, &my);
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// Calculate pixel ration for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;

		// Update and render
		glViewport(0, 0, fbWidth, fbHeight);
		if (premult) glClearColor(0,0,0,0);
		else glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
		nvgFontSize(vg, 20.0);
		nvgFontFace(vg, "sans");
		nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

        // char fps[32];
        // sprintf(fps, "%4.1f fps", 1.0/dt);
		// nvgText(vg, 10.0, 10.0, fps, NULL);

        if(engine_render!=NULL) engine_render(dt);
		nvgEndFrame(vg);
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


// CHIPMUNK //

static void 
DrawCircle(
    cpVect pos, 
    cpFloat angle, 
    cpFloat radius, 
    cpSpaceDebugColor outlineColor, 
    cpSpaceDebugColor fillColor, 
    cpDataPointer data) {

    nvgBeginPath(vg);
    nvgCircle(vg, pos.x, pos.y, radius);
    nvgMoveTo(vg, pos.x, pos.y);
    nvgLineTo(vg, pos.x+cos(angle)*radius, pos.y+sin(angle)*radius);
    nvgStrokeWidth(vg, 1.0);
    nvgStrokeColor(vg, nvgRGBAf(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
    nvgStroke(vg);
        
};

void 
DrawSegment(
    cpVect a, 
    cpVect b, 
    cpSpaceDebugColor color, 
    cpDataPointer data) {

    nvgBeginPath(vg);
    nvgMoveTo(vg, a.x, a.y);
    nvgLineTo(vg, b.x, b.y);
    nvgStrokeWidth(vg, 1.0);
    nvgStrokeColor(vg, nvgRGBAf(color.r, color.g, color.b, color.a));
    nvgStroke(vg);
};

static void 
DrawFatSegment(
    cpVect a, 
    cpVect b,
    cpFloat radius,
    cpSpaceDebugColor outlineColor,
    cpSpaceDebugColor fillColor,
    cpDataPointer data) {

    nvgBeginPath(vg);
    nvgMoveTo(vg, a.x, a.y);
    nvgLineTo(vg, b.x, b.y);
    nvgStrokeWidth(vg, 4.0);
    nvgStrokeColor(vg, nvgRGBAf(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
    nvgStroke(vg);
};
    
static void 
DrawPolygon(
    int count,
    const cpVect *verts,
    cpFloat radius,
    cpSpaceDebugColor outlineColor,
    cpSpaceDebugColor fillColor,
    cpDataPointer data) {

    nvgBeginPath(vg);
    nvgMoveTo(vg, verts[0].x, verts[0].y);
    for(int i=1; i<count; i++) {
        nvgLineTo(vg, verts[i].x, verts[i].y);
    }
    nvgLineTo(vg, verts[0].x, verts[0].y);
    nvgStrokeColor(vg, nvgRGBAf(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
    nvgStroke(vg);
};

void 
DrawDot(
    cpFloat size,
    cpVect pos,
    cpSpaceDebugColor color,
    cpDataPointer data) {

    nvgBeginPath(vg);
    nvgCircle(vg, pos.x, pos.y, size);
    nvgStrokeWidth(vg, 1.0);
    nvgStrokeColor(vg, nvgRGBAf(color.r, color.g, color.b, color.a));
    nvgStroke(vg);
};

static inline cpSpaceDebugColor 
RGBAColor(float r, float g, float b, float a){
	cpSpaceDebugColor color = {r, g, b, a};
	return color;
}

static inline cpSpaceDebugColor 
LAColor(float l, float a){
	cpSpaceDebugColor color = {l, l, l, a};
	return color;
}

static cpSpaceDebugColor
ColorForShape(cpShape *shape, cpDataPointer data) {
	if(cpShapeGetSensor(shape)){
		return LAColor(1.0f, 0.1f);
	} else {
		cpBody *body = cpShapeGetBody(shape);
		
		if(cpBodyIsSleeping(body)){
			return LAColor(0.2f, 1.0f);
		} else if(body->sleeping.idleTime > shape->space->sleepTimeThreshold) {
			return LAColor(0.66f, 1.0f);
		} else {
			uint32_t val = (uint32_t)shape->hashid;
			
			// scramble the bits up using Robert Jenkins' 32 bit integer hash function
			val = (val+0x7ed55d16) + (val<<12);
			val = (val^0xc761c23c) ^ (val>>19);
			val = (val+0x165667b1) + (val<<5);
			val = (val+0xd3a2646c) ^ (val<<9);
			val = (val+0xfd7046c5) + (val<<3);
			val = (val^0xb55a4f09) ^ (val>>16);
			
			float r = (float)((val>>0) & 0xFF);
			float g = (float)((val>>8) & 0xFF);
			float b = (float)((val>>16) & 0xFF);
			
			float max = (float)cpfmax(cpfmax(r, g), b);
			float min = (float)cpfmin(cpfmin(r, g), b);
			float intensity = (cpBodyGetType(body) == CP_BODY_TYPE_STATIC ? 0.15f : 0.75f);
			
			// Saturate and scale the color
			if(min == max){
				return RGBAColor(intensity, 0.0f, 0.0f, 1.0f);
			} else {
				float coef = (float)intensity/(max - min);
				return RGBAColor(
					(r - min)*coef,
					(g - min)*coef,
					(b - min)*coef,
					1.0f
				);
			}
		}
	}
}

static cpSpaceDebugDrawOptions drawOptions = {
	DrawCircle,
	DrawSegment,
	DrawFatSegment,
	DrawPolygon,
	DrawDot,
	
	(cpSpaceDebugDrawFlags)(CP_SPACE_DEBUG_DRAW_SHAPES | CP_SPACE_DEBUG_DRAW_CONSTRAINTS | CP_SPACE_DEBUG_DRAW_COLLISION_POINTS),
	
	{200.0f/255.0f, 210.0f/255.0f, 230.0f/255.0f, 1.0f},
	ColorForShape,
	{0.0f, 0.75f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	NULL,
};

void spaceDraw(cpSpace* space) {
	cpSpaceDebugDraw(space, &drawOptions);
}
