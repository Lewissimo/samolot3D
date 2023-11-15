/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
/*#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
*/
#include <iostream>
float speed = PI;//[radiany/s]
float speed_x = 0;
float speed_y = 0;
float z = 0.0f;
GLuint tex1, tex2;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -40.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}


void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	float cameraSpeed = 0.55f; 
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			speed_y = -PI;

		}
		if (key == GLFW_KEY_RIGHT) {
			speed_y = PI;

		}
		if (key == GLFW_KEY_UP) {
			speed_x = -PI;
		}

		if (key == GLFW_KEY_DOWN) {
				speed_x = PI;
		}
		
			
			if (key == GLFW_KEY_W) {

				cameraUp += cameraSpeed;
			}
			if (key == GLFW_KEY_S) {
				cameraUp -= cameraSpeed;
			}
			if (key == GLFW_KEY_A) {
				cameraFront -= cameraSpeed;
			}
			if (key == GLFW_KEY_D) {
				cameraFront += cameraSpeed;
			}
			if (key == GLFW_KEY_Z) {
				cameraPos -= cameraSpeed;
			}
			if (key == GLFW_KEY_X) {
				cameraPos += cameraSpeed;
			}
		}
	
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) {
			speed_y = 0;

		}
		if ((key == GLFW_KEY_RIGHT)) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			speed_x = 0;
		}
	}
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glfwSetKeyCallback(window, key_callback);
	tex1 = readTexture("bricks.png");
	tex2 = readTexture("Kamo.png");

	glClearColor(0, 0, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	glDeleteTextures(1, &tex1);
	glDeleteTextures(1, &tex2);
}




//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float x, float y, float z) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości
	//printf("%f\n", z);
	
	float verts[] = {
		/*
	-15,0,0,1, 0,5,2,1, 0,-5,2,1, 0,-5,-2,1, 0,5,-2,1,	0,5,2,1,//AFCDBF
	 2,9,0,1, 0,5,2,1, 4,5,2,1, 4,5,-2,1, 0,5,-2,1, 0,5,2,1, //IFGHB
	19,0,0,1,4,5,2,1, 4,5,-2,1, 4,-5,-2,1,4,-5,2,1, 4,5,2,1, //LGHJK
	2,-17.5,-7,1, 4,-15,-2,1, 0,-15,-2,1, 0,-20,-2,1, 4,-20,-2,1, 4,-15,-2,1,//TRSOPR
	4,-20,-2,1, 4,5,2,1, 4,5,-2,1, 0,5,-2,1, 4,5,2,1,  	//PGHBG
	4,5,2,1, 4,-20,-2,1, 4,-20,2,1, 0,-20,2,1, 4,-20,-2,1, 	//GPMNP
	0,-20,2,1, 0,5,-2,1, 0,5,2,1, 4,5,2,1, 0,5,-2,1, 	//NBFGB
	0,5,-2,1, 4,-20,-2,1, 0,-20,-2,1, 0,-20,2,1, 4,-20,-2,1,	//BPONP
	2, 0, -5,1, 4,5,-2,1, 0,5,-2,1, 0,5,-2,1, 0,-5,-2,1, 4,-5,-2,1, 4,5,-2,1 //UHBDJH
			*/												 /*
	-15,0,0,1,			//A
	0,5,-2,1,         //B
	0,-5,2,1,			//C
	0,-5,-2,1,    //D
	0,5,2,1,			//F
	4,5,2,1,   //G
	4,5,-2,1,			//H
	2,0,0,1,   //I
	4,-5,-2,1,			//J
	4,-5,2,1,   //K
	19,0,0,1,		//L
	4,-20,2,1,  //M
	0,-20,2,1,//N
	0,-20,-2,1,//O
	4,-20,-2,1,//P
	4,-15,-2,1,//R
	0,-15,-2,1,//S
	2,-17.5,-7,1.//T
	2, 0, -5,1 //U
	*/
	
	-15,0,0,1, 0,2,5,1, 0,2,-5,1, 0,-2,-5,1, 0,-2, 5,1, 0,2,5,1,//AFCDBF
	2,0,10,1, 0,2,5,1, 4,2,5,1, 4,-2,5,1, 0,-2, 5,1, 0,2,5,1,//IFGHBf
	19,0,0,1, 4,2,5,1, 4,-2,5,1, 4,-2,-5,1, 4,2,-5,1,4,2,5,1,//LGHJKg
	2,-7,-17.5,1, 4,-2,-15,1, 0,-2,-15,1, 0,-2,-20,1, 4,-2,-20,1, 4,-2,-15,1,//TRSOPR
	4,-2,-20,1, 4,2,5,1, 4,-2,5,1, 0,-2, 5,1, 4,2,5,1,//PGHBG
	4,2,5,1, 4,-2,-20,1, 4,2,-20,1, 0,2,-20,1, 4,-2,-20,1,//GPMNP
	0,2,-20,1, 0,-2, 5,1, 0,2,5,1, 4,2,5,1, 0,-2, 5,1,//NBFGB
	0,-2, 5,1, 4,-2,-20,1, 0,-2,-20,1, 0,2,-20,1, 4,-2,-20,1,//BPONP
	




		/*
	-15,0,0,1,			//A
	0,-2, 5,1,         //B
	0,2,-5,1,			//C
	0,-2,-5,1,    //D
	0,2,5,1,			//F
	4,2,5,1,   //G
	4,-2,5,1,			//H
	2,0,0,1,   //I
	4,-2,-5,1,			//J
	4,2,-5,1,   //K
	19,0,0,1,		//L
	4,2,-20,1,  //M
	0,2,-20,1,//N
	0,-2,-20,1,//O
	4,-2,-20,1,//P
	4,-2,-15,1,//R
	0,-2,-15,1,//S
	2,-7,-17.5,1,//T
	2, -5, 0,1 //U
	*/
	
	};
	float vv[] = { 2,-5,0,1, 4,-2,5,1, 0,-2, 5,1, 0,-2,-5,1, 4,-2,-5,1, 4,-2,5,1   //UHBDJH
	};
	
	float texCoords[] = {
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //AFCDBF
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //IFGHBf
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //LGHJKg
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //TRSOPR
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  //PGHBG
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, //GPMNP
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, //NBFGB
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f, //BPONP

	};
	
	

	float colors[] = {
			0.33,0.33,0.92,1, 0.33,0.33,0.92,1,0.33,0.33,0.92,1,0.33,0.33,0.92,1,0.33,0.33,0.92,1,0.33,0.33,0.92,1
 };
 
	int vertexCount1 = 6;
	int vertexCount2 = 5;
	float normals[] = {
0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, //AFCDBF
	0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, //IFGHBf
	0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, //LGHJKg
	0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, //TRSOPR
	0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, //PGHBG
	0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f,  //GPMNP
	0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, //NBFGB
	0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, //BPONP
	0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f,0.0f  //UHBDJH
	};
	using namespace glm;
	
	mat4 M2 = mat4(1.0f);
	mat4 M1 = mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	//M = glm::rotate(M, angle, glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
	M1 = rotate(M1, y, vec3(0.0f, 1.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
	M1 = rotate(M1, x, vec3(1.0f, 0.0f, 0.0f));
	//M1 = rotate(M1, y, vec3(0.0f, 0.0f, z));
	
	glm::mat4 V = lookAt(cameraPos, cameraFront, cameraUp);
	glm::mat4 P = glm::perspective(glm::radians(100.0f), -1.0f, 1.0f, 1100.0f); //Wylicz macierz rzutowania
	
	/*
	spLambertTextured->use();
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M1));
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, verts);
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, normals);
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex1);
	glUniform1i(spLambertTextured->u("tex"), 0);

	*/

	spTextured->use();
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M1));
	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, verts);
	glEnableVertexAttribArray(spTextured->a("normal"));
	glVertexAttribPointer(spTextured->a("normal"), 4, GL_FLOAT, false, 0, normals);
	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex1);
	glUniform1i(spTextured->u("tex"), 0);






	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount1);
	glDrawArrays(GL_TRIANGLE_FAN, 6, vertexCount1);
	glDrawArrays(GL_TRIANGLE_FAN, 12, vertexCount1);
	glDrawArrays(GL_TRIANGLE_FAN, 18, vertexCount1);
	glDrawArrays(GL_TRIANGLE_FAN, 24, vertexCount2);
	glDrawArrays(GL_TRIANGLE_FAN, 29, vertexCount2);
	glDrawArrays(GL_TRIANGLE_FAN, 34, vertexCount2);
	glDrawArrays(GL_TRIANGLE_FAN, 39, vertexCount2);
	/*
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	*/


	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("normal"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));



	spConstant->use();
	glUniformMatrix4fv(spConstant->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spConstant->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spConstant->u("M"), 1, false, glm::value_ptr(M1));
	glUniform4f(spConstant->u("color"), 0.33, 0.33, 0.92, 1);
	//loadModel("models\piano\piano.obj");
	glEnableVertexAttribArray(spConstant->a("vertex"));
	glVertexAttribPointer(spConstant->a("vertex"), 4, GL_FLOAT, false, 0, vv);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount1);
	float vert[] = {
		1000, -25, -1000,1,
		-1000, -25, 1000,1,
		-1000, -25, -1000,1,
		1000, -25, -1000,1,
		1000, -25, 1000,1,
		-1000, -25, 1000,1
	};
	float texCoords2[] = {
10.0f, 0.0f, //A
0.0f, 10.0f, //B
0.0f, 0.0f, //C
10.0f, 0.0f, //A
10.0f, 10.0f, //D
0.0f, 10.0f, //B
	};

	int vertexCount = 6;
	spTextured->use();
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M2));
	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, vert);
	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords2);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_REPEAT);
	glUniform1i(spTextured->u("tex"), 0);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));


	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące
	float x = 0;
	float y = 0; 
	//Główna pętla
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		x += speed_x * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		y += speed_y * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		glfwSetTime(0); //Wyzeruj licznik czasu
	
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window,x, y, z); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
