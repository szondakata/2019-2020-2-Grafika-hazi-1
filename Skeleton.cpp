//=============================================================================================
// Mintaprogram: Z�ld h�romsz�g. Ervenyes 2019. osztol.
//
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat, BOM kihuzando.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni a printf-et kiveve
// - Mashonnan atvett programresszleteket forrasmegjeloles nelkul felhasznalni es
// - felesleges programsorokat a beadott programban hagyni!!!!!!!
// - felesleges kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan OpenGL fuggvenyek hasznalhatok, amelyek az oran a feladatkiadasig elhangzottak
// A keretben nem szereplo GLUT fuggvenyek tiltottak.
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Szonda Katalin
// Neptun : FE17LH
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#include "framework.h"

// vertex shader in GLSL: It is a Raw string (C++11) since it contains new line characters
const char * const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

	uniform mat4 MVP;			// uniform variable, the Model-View-Projection transformation matrix
	layout(location = 0) in vec2 vp;	// Varying input: vp = vertex position is expected in attrib array 0

	void main() {
		gl_Position = vec4(vp.x, vp.y, 0, 1) * MVP;		// transform vp from modeling space to normalized device space
	}
)";

// fragment shader in GLSL
const char * const fragmentSource = R"(
	#version 330			// Shader 3.3
	precision highp float;	// normal floats, makes no difference on desktop computers

	uniform vec3 color;		// uniform variable, the color of the primitive
	out vec4 outColor;		// computed color of the current pixel

	void main() {
		outColor = vec4(color, 1);	// computed color is the color of the primitive
	}
)";

GPUProgram gpuProgram; // vertex and fragment shaders

const unsigned int nv = 100;
unsigned int resolution = 20; ///CSAK PAROS!!!
std::vector<vec2>  iv1, iv2, iv3;
std::vector<vec2> csucsok;

float distance(vec2, vec2);
float hajlasszog(vec2, vec2);

class Triangle{
private:
    unsigned int vaoTriangle{}, vboTriangle{};
    std::vector<vec2> trianglePoints;
    std::vector<float> vertexData;

    vec2 center = vec2(0.0f, 0.0f);
public:
    Triangle(std::vector<vec2> tP) : trianglePoints(tP) {
        glGenVertexArrays(1, &vaoTriangle);
        glBindVertexArray(vaoTriangle);

        glGenBuffers(1, &vboTriangle);	// Generate 1 buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
        glEnableVertexAttribArray(0); // AttribArray 0
        glVertexAttribPointer(0, // vbo -> AttribArray 0
                              2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
                              0, nullptr); // stride, offset: tightly packed //TODO stride?
        for (int i = 0; i < 3; i++){
            center = center + trianglePoints[resolution * i + (int) (resolution / 2)];
        }
        center = center / 3.0f;
    }
    void Draw(){
        for (int k = 0; k < 3; k++) {
            vertexData.clear();
            vertexData.push_back(center.x);
            vertexData.push_back(center.y);
            for (int i = 0; i < resolution + 2; i++) {
                if (i % 2 == 0) {
                    int idx = resolution * ((k + 2) % 3) + resolution / 2;
                    idx += i / 2;

                    vertexData.push_back(trianglePoints[idx].x);
                    vertexData.push_back(trianglePoints[idx].y);
                } else {
                    int idx = resolution * k + resolution / 2;
                    idx -= i / 2;

                    vertexData.push_back(trianglePoints[idx].x);
                    vertexData.push_back(trianglePoints[idx].y);
                }
            }

            mat4 MVPtransf = {1, 0, 0, 0,    // MVP matrix,
                              0, 1, 0, 0,    // row-major!
                              0, 0, 1, 0,
                              0, 0, 0, 1};
            gpuProgram.setUniform(MVPtransf, "MVP");

            if (!vertexData.empty()) {
                glBindVertexArray(vaoTriangle);

                glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
                gpuProgram.setUniform(vec3(0.3f * k + 0.1f, 0.3f * k + 0.1f, 0.3f * k + 0.1f), "color");

                glDrawArrays(GL_TRIANGLE_STRIP, 0, resolution);
            }
        }
    }
};

class Kor{
    unsigned int vaoKor{}, vboKor{}; // vertex array object id
    std::vector<vec2> iv;

    vec2 _o;
    float _radius, _iv_len;
    vec2 _p1, _p2;

public:
    Kor(vec2 p1, vec2 p2) : _p1(p1), _p2(p2) {
        if(p1.x * p2.y - p2.x * p1.y != 0.0f) { //ha a ket pont pontosan atmerore esik, nincs megoldas
            _o = vec2((p2.y * (p1.x * p1.x + p1.y * p1.y + 1.0f) - p1.y * (p2.x * p2.x + p2.y * p2.y + 1.0f)) /
                      (2.0f * (p1.x * p2.y - p2.x * p1.y)),
                      (p1.x * (p2.x * p2.x + p2.y * p2.y + 1.0f) - p2.x * (p1.x * p1.x + p1.y * p1.y + 1.0f)) /
                      (2.0f * (p1.x * p2.y - p2.x * p1.y)));
        } else { exit; }
        _radius = sqrt(dot(_o, _o) - 1.0f);

        glGenVertexArrays(1, &vaoKor); // create 1 vertex array object
        glBindVertexArray(vaoKor); // make it active

        glGenBuffers(1, &vboKor);	// Generate 1 buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboKor);
        glEnableVertexAttribArray(0); // AttribArray 0
        glVertexAttribPointer(0, // vbo -> AttribArray 0
                              2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
                              0, nullptr); // stride, offset: tightly packed //TODO stride?
    }
    ~Kor(){
        glDeleteBuffers(1, &vboKor);
        glDeleteVertexArrays(1, &vaoKor);
    }

    float r() const {return _radius;}
    vec2 o() const {return _o;}
    float ivhossz() const {return _iv_len;}

    std::vector<vec2> koriv() {
        vec2 begin;
        vec2 end;

        float p1szog = atan2(_p1.x - _o.x, _p1.y - _o.y);
        float p2szog = atan2(_p2.x - _o.x, _p2.y - _o.y);

        _iv_len = 0;

        for (float i = 0; i < resolution; i++){
            vec2 v = _o + normalize(((_p1 - _o) * (1.0f - i / (resolution - 1)) ) + ((_p2 - _o) * i / (resolution - 1))) * _radius;
            if(i > 0) {
                _iv_len += distance(iv.back(), v);
            }
            iv.emplace_back(v);
        }

        iv.emplace_back(_p2);

        iv.pop_back();

        return iv;
    }

    void Draw(){
        std::vector<float> vertexData;
        for (int i = 0; i < iv.size(); i++){
            vertexData.push_back(iv[i].x);
            vertexData.push_back(iv[i].y);
        }
        mat4 MVPtransf = { 1, 0, 0, 0,    // MVP matrix,
                                   0, 1, 0, 0,    // row-major!
                                   0, 0, 1, 0,
                                   0, 0, 0, 1 };
        gpuProgram.setUniform(MVPtransf, "MVP");

        if (!vertexData.empty()) {
            glBindVertexArray(vaoKor);
            glBindBuffer(GL_ARRAY_BUFFER, vboKor);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
            gpuProgram.setUniform(vec3(1.0f, 0.0f, 0.0f), "color");

            glDrawArrays(GL_LINE_STRIP, 0, resolution);
        }
    }
};

class AlapKor{
    unsigned int vaoAlap{}, vboAlap{}; // vertex array object id
    vec2 vertices[nv];
public:
    AlapKor(){
        //az alapkor inicializalasa
        for (int i = 0; i < nv; i++){
            float fi = i * 2.0f * M_PI / nv;
            vertices[i] = vec2(cosf(fi), sin(fi));
        }

        glGenVertexArrays(1, &vaoAlap); // create 1 vertex array object
        glBindVertexArray(vaoAlap); // make it active

        glGenBuffers(1, &vboAlap);	// Generate 1 buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboAlap);

        glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
                     sizeof(vec2) * nv,  // # bytes
                     vertices,	      	// address
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0); // AttribArray 0
        glVertexAttribPointer(0, // vbo -> AttribArray 0
                              2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
                              0, NULL); // stride, offset: tightly packed //TODO stride?
    }

    ~AlapKor(){
        glDeleteBuffers(1, &vboAlap);
        glDeleteVertexArrays(1, &vaoAlap);
    }

    void Draw(){
        mat4 MVPtransf = { 1, 0, 0, 0,    // MVP matrix,
                           0, 1, 0, 0,    // row-major!
                           0, 0, 1, 0,
                           0, 0, 0, 1 };
        gpuProgram.setUniform(MVPtransf, "MVP");

        gpuProgram.setUniform(vec3(0.8f, 0.8f, 0.8f), "color");

        glBindVertexArray(vaoAlap);
        glDrawArrays(GL_TRIANGLE_FAN, 0, nv);
    }
};

Triangle *triangle;
Kor *c1, *c2, *c3;
AlapKor *k;

// Initialization, create an OpenGL context
void onInitialization() {
    if (resolution % 2 != 0){
        resolution += 1;
    }
	glViewport(0, 0, windowWidth, windowHeight);
	glLineWidth(2.0f);
	glPointSize(10.0f);

	k = new AlapKor();

	// create program for the GPU
	gpuProgram.create(vertexSource, fragmentSource, "outColor");
}

// Window has become invalid: Redraw
void onDisplay() {
	glClearColor(0, 0, 0, 0);     // background color
	glClear(GL_COLOR_BUFFER_BIT); // clear frame buffer

	k->Draw();

	if (csucsok.size() == 3){
	    c1->Draw();
	    c2->Draw();
	    c3->Draw();

	    triangle->Draw();
	}

	glutSwapBuffers(); // exchange buffers for double buffering
}

// Convert to normalized device space
vec2 toNDC(const int pX, const int pY) {
    vec2 cPoint;
    cPoint.x = 2.0F * static_cast<float>(pX) / windowWidth - 1; // flip y axis
    cPoint.y = 1.0F - 2.0F * static_cast<float>(pY) / windowHeight;
    return cPoint;
}

// Mouse click event
void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	if (state == GLUT_DOWN) {
        if (csucsok.size() < 3) {
            csucsok.push_back(toNDC(pX, pY));
            printf("(%3.2f, %3.2f) added\n", toNDC(pX, pY).x, toNDC(pX, pY).y);
        }

        if (csucsok.size() == 3) {
            std::vector<vec2> haromszog;

            c1 = new Kor(csucsok[0], csucsok[1]);
            c2 = new Kor(csucsok[1], csucsok[2]);
            c3 = new Kor(csucsok[2], csucsok[0]);

            iv1 = c1->koriv();
            iv2 = c2->koriv();
            iv3 = c3->koriv();

            haromszog.insert(haromszog.end(), iv1.begin(), iv1.end());
            haromszog.insert(haromszog.end(), iv2.begin(), iv2.end());
            haromszog.insert(haromszog.end(), iv3.begin(), iv3.end());

            triangle = new Triangle(haromszog);

            printf("Alpha: %f, ", hajlasszog(c1->o() - csucsok[0], c3->o() - csucsok[0]));
            printf("Beta: %f, ",  hajlasszog(c1->o() - csucsok[1], c2->o() - csucsok[1]));
            printf("Gamma: %f, \n", hajlasszog(c3->o() - csucsok[2], c2->o() - csucsok[2]));

            printf("a: %1.6f, ", c1->ivhossz());
            printf("b: %1.6f, ", c2->ivhossz());
            printf("c: %1.6f \n", c3->ivhossz());
        }
        glutPostRedisplay(); // redraw
    }
}

// Idle event indicating that some time elapsed: do animation here
void onIdle() {
	long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program
}

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY) {
}

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY) {
}

// Move mouse with key pressed
void onMouseMotion(int pX, int pY) {
}

float distance(vec2 v1, vec2 v2){
    auto d = v2 - v1;
    return length(d) / (1 - dot(v1, v1));
}

float hajlasszog(vec2 v1, vec2 v2){
    return acosf((dot (v1 - v2, v1 - v2) - dot(v1, v1) - dot (v2, v2)) / 2.0f / length(v1) / length(v2));
}