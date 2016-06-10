/* -------------------------------------------------------------------
 * LoadModels.cpp
 * versión 1.0
 * Copyright (C) 2016 
 * José Ricardo Rodríguez Abreu,
 * Facultad de Ciencias,
 * Universidad Nacional Autónoma de México, Mexico.
 *
 * Este programa es software libre; se puede redistribuir
 * y/o modificar en los términos establecidos por la
 * Licencia Pública General de GNU tal como fue publicada
 * por la Free Software Foundation en la versión 2 o
 * superior.
 *
 * Este programa es distribuido con la esperanza de que
 * resulte de utilidad, pero SIN GARANTÍA ALGUNA; de hecho
 * sin la garantía implícita de COMERCIALIZACIÓN o
 * ADECUACIÓN PARA PROPÓSITOS PARTICULARES. Véase la
 * Licencia Pública General de GNU para mayores detalles.
 *
 * Con este programa se debe haber recibido una copia de la
 * Licencia Pública General de GNU, de no ser así, visite el
 * siguiente URL:
 * http://www.gnu.org/licenses/gpl.html
 * o escriba a la Free Software Foundation Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * -------------------------------------------------------------------
 */
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <fstream>
#include <il/il.h>
#include <string.h>
#include <map>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "getbmp.h"
#include <fstream>

/* Variables globales*/
const struct aiScene* scene = NULL;
GLuint scene_list = 0;
// imagenes / texturas
// mapea archivos de imagenes a las texturas.
std::map<std::string, GLuint*> textureIdMap;
//Un apuntador al arreglo de las texturas.
GLuint* textureIds;
// Creamos una instancia de una clase para importar al objeto.
Assimp::Importer importer;
//Son los archivos de entrada por default.
static std::string basepath = "../resources/";
static std::string modelname = "avioneta.obj";
// Estos arreglos nos ayudan a darle luz a cada textura:
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 15.0f, 1.0f };
// Estos vectores nos dan las posiciones de la escena actual.
aiVector3D scene_min, scene_max, scene_center;
//Con esto podemos generar la rotación del modelo.
static float anguloL = 0.5;
static float anguloC = 0.5;
static float anguloA = 0.5;
static float movimiento = 0.f;
static float moveW = 0.f;
static float escala = 1.5f;
static int mueve = 1, cambioW = 0;
//Un arreglo para los índices de las texturas.
static unsigned int texture[2];
//Un flotante para la distancia que recibe adelante el parámetro gluLookAt()
static float d = 0.0;
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


/**
 *----------------------------------------------------------------------------
 * Carga las texturas externas al modelo.
 *----------------------------------------------------------------------------
 */
void loadExternalTextures()
{
  // Guarda los datos de la imagen bmp en este arreglo.
  BitMapFile *image[2];
  
  //Cargamos las imagenes.
  image[0] = getbmp("../resources/grass.bmp");
  image[1] = getbmp("../resources/sky.bmp");

  // Unimos las texturas de las imagenes con el objeto que simula el pasto en el texture[0]
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
	       GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //Unimos las texturas de las imagenes con el objeto que sumula el cierlo en el texture[1]
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
	       GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 *----------------------------------------------------------------------------
 * Función que inicializa todos los atributos de openGL.
 *----------------------------------------------------------------------------
 */
void setup(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  
  // Creamos los id de las texturas (2)
  glGenTextures(2, texture);
  
  // Cargamos las texturas.
  loadExternalTextures();

  // Aquí especificamos como las texturas son combinadas con los colores.
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  // Enciende las texturas de openGL
  glEnable(GL_TEXTURE_2D);
  
  // Aquí activamos esto para evitar gasto de recursos.
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

/**
 *----------------------------------------------------------------------------
 * Función que rota un objeto cada cierto tiempo. (DESACTIVADA)
 *----------------------------------------------------------------------------
 */
void rota_objeto (void)
{
  static GLint prev_time = 0;
  
  int time = glutGet(GLUT_ELAPSED_TIME);
  //Si la siguiente linea está comentada, la función no sirve de nada. (Desactivada)
  //angulo += (time-prev_time)*0.01;
  prev_time = time;
  
  glutPostRedisplay ();
}

/**
 *----------------------------------------------------------------------------
 * Función que mueve un objeto cada cierto tiempo por un cierto rango.
 *----------------------------------------------------------------------------
 */
void mueve_objeto (void)
{
  if(movimiento > 3.5)
    {
      movimiento = -3.5;
    }
  else
    {
      movimiento += .03;
    }
}


/**
 *----------------------------------------------------------------------------
 * En la estructa de datos que usamos para los vértices y las caras, en esta función
 * se conectan los nodos con un cluster de dicha estructura.
 *----------------------------------------------------------------------------
 */
void get_bounding_box_for_node(const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo)
{
  aiMatrix4x4 prev;
  unsigned int n = 0, t;
  
  prev = *trafo;
  aiMultiplyMatrix4(trafo, &nd->mTransformation);
  
  for (; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
    for (t = 0; t < mesh->mNumVertices; ++t) {
      
      aiVector3D tmp = mesh->mVertices[t];
      aiTransformVecByMatrix4(&tmp, trafo);
      
      min->x = aisgl_min(min->x, tmp.x);
      min->y = aisgl_min(min->y, tmp.y);
      min->z = aisgl_min(min->z, tmp.z);
      
      max->x = aisgl_max(max->x, tmp.x);
      max->y = aisgl_max(max->y, tmp.y);
      max->z = aisgl_max(max->z, tmp.z);
    }
  }
  
  for (n = 0; n < nd->mNumChildren; ++n) {
    get_bounding_box_for_node(nd->mChildren[n], min, max, trafo);
  }
  *trafo = prev;
}

/**
 *----------------------------------------------------------------------------
 * Lo que esto hace es crea un nuevo cluster en la estructura que usamos.
 *----------------------------------------------------------------------------
 */
void get_bounding_box(aiVector3D* min, aiVector3D* max)
{
  aiMatrix4x4 trafo;
  aiIdentityMatrix4(&trafo);
  
  min->x = min->y = min->z = 1e10f;
  max->x = max->y = max->z = -1e10f;
  get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
}

/**
 *----------------------------------------------------------------------------
 * Colorea en openGL con la función glColor4f
 *----------------------------------------------------------------------------
 */
void Color4f(const aiColor4D *color)
{
  glColor4f(color->r,color->g,color->b,color->a);
}

/**
 *----------------------------------------------------------------------------
 * Hace un "cast" de color a un arreglo que se le pase.
 *----------------------------------------------------------------------------
 */
void color4_to_float4(const aiColor4D *c, float f[4])
{
  f[0] = c->r;
  f[1] = c->g;
  f[2] = c->b;
  f[3] = c->a;
}

/**
 *----------------------------------------------------------------------------
 * Se le asigna valores a un arreglo que se le pase.
 *----------------------------------------------------------------------------
 */
void set_float4(float f[4], float a, float b, float c, float d)
{
  f[0] = a;
  f[1] = b;
  f[2] = c;
  f[3] = d;
}

/**
 *----------------------------------------------------------------------------
 * Esta función es de las 4 más importantes:
 * 1. CARGA el modelo:
 * En esta función se carga el modelo. Recibe una cadena que es el url de la imagen.
 *----------------------------------------------------------------------------
 */
bool Import3DFromFile(const std::string& pFile)
{
  
  //Revisa si el archivo existe.
  std::ifstream fin(pFile.c_str());
  if (!fin.fail()) {
    fin.close();
  }
  else{
    printf("No se pudo abrir el archivo: %s\n", pFile.c_str());
    printf("%s\n", importer.GetErrorString());
    return false;
  }
  
  scene = importer.ReadFile(pFile, aiProcessPreset_TargetRealtime_Quality);

  //Si falla el import, se reporta un error del sistema en la terminal
  if (!scene)
    {
      printf("%s\n", importer.GetErrorString());
      return false;
    }
  
  get_bounding_box(&scene_min, &scene_max);
  
  scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
  scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
  scene_center.z = (scene_min.z + scene_max.z) / 2.0f;

  //Ahora podemos acceder a el contenido del archivo:
  printf("Se importó exitosamente el archivo %s .", pFile.c_str());

  return true;
}


/**
 *----------------------------------------------------------------------------
 * Esta función es de las 4 más importantes:
 * 3. Aplicar el material en el modelo:
 * Esta función le aplica un material de formato mtl a un objeto en 3D.
 *----------------------------------------------------------------------------
 */
void apply_material(const aiMaterial *mtl)
{
  float c[4];
  
  GLenum fill_mode;
  int ret1, ret2;
  aiColor4D diffuse;
  aiColor4D specular;
  aiColor4D ambient;
  aiColor4D emission;
  float shininess, strength;
  int two_sided;
  int wireframe;
  unsigned int max;
  int texIndex = 0;
  //Tiene el nombre del archivo de las texturas.
  aiString texPath;
  
  if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
    {
      //Mapea en openGL las texturas.
      unsigned int texId = *textureIdMap[texPath.data];
      glBindTexture(GL_TEXTURE_2D, texId);
    }
  
  set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
    color4_to_float4(&diffuse, c);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
  
  set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
    color4_to_float4(&specular, c);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
  
  set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
    color4_to_float4(&ambient, c);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
  
  set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
    color4_to_float4(&emission, c);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
  
  max = 1;
  ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
  max = 1;
  ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
  if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
  else {
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
  }
  
  max = 1;
  if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
    fill_mode = wireframe ? GL_LINE : GL_FILL;
  else
    fill_mode = GL_FILL;
  glPolygonMode(GL_FRONT_AND_BACK, fill_mode);
  
  max = 1;
  if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);
}


/**
 *----------------------------------------------------------------------------
 * Esta función es de las 4 más importantes:
 * 2. CARGA el modelo:
 * Renderiza el modelo recursivamente en la pantalla. Muestra al objeto 3D.
 *----------------------------------------------------------------------------
 */
void recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale)
{
  unsigned int i;
  unsigned int n=0, t;
  aiMatrix4x4 m = nd->mTransformation;

    if(escala == 2.f)
      m.Scaling(aiVector3D(2*scale, 2*scale,2*scale), m);
    else
      m.Scaling(aiVector3D(scale, scale, scale), m);
  
  // Actualiza las transformaciones.
  m.Transpose();
  glPushMatrix();
  glMultMatrixf((float*)&m);
  
  // draw all meshes assigned to this node
  for (; n < nd->mNumMeshes; ++n)
    {
      const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
      
      apply_material(sc->mMaterials[mesh->mMaterialIndex]);
      
      
      if(mesh->mNormals == NULL)
	{
	  glDisable(GL_LIGHTING);
	}
      else
	{
	  glEnable(GL_LIGHTING);
	}
      
      if(mesh->mColors[0] != NULL)
	{
	  glEnable(GL_COLOR_MATERIAL);
	}
      else
	{
	  glDisable(GL_COLOR_MATERIAL);
	}
      
      
      
      for (t = 0; t < mesh->mNumFaces; ++t) {
	const struct aiFace* face = &mesh->mFaces[t];
	GLenum face_mode;
	
	switch(face->mNumIndices)
	  {
	  case 1: face_mode = GL_POINTS; break;
	  case 2: face_mode = GL_LINES; break;
	  case 3: face_mode = GL_TRIANGLES; break;
	  default: face_mode = GL_POLYGON; break;
	  }
	
	glBegin(face_mode);

	//Ahora avanzamos por todos los vértices de las caras
	for(i = 0; i < face->mNumIndices; i++)		
	  {
	    int vertexIndex = face->mIndices[i];	
	    if(mesh->mColors[0] != NULL)
	      Color4f(&mesh->mColors[0][vertexIndex]);
	    if(mesh->mNormals != NULL)

	      //Si tiene texturas en las coordenadas que lo pusimos.
	      if(mesh->HasTextureCoords(0))  
		{
		  //glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y);
		  glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1-mesh->mTextureCoords[0][vertexIndex].y);
		}
	    
	    glNormal3fv(&mesh->mNormals[vertexIndex].x);
	    glVertex3fv(&mesh->mVertices[vertexIndex].x);
	    
	  }
	glEnd();	
      }
    }
  
  // Ahora dibuja a todos los hijos de este nodo.
  for (n = 0; n < nd->mNumChildren; ++n)
    {
      recursive_render(sc, nd->mChildren[n], scale);
    }
  
  glPopMatrix();
}


/**
 *----------------------------------------------------------------------------
 * Función para mostrar en pantalla el modelo ya cargando las texturas.
 *----------------------------------------------------------------------------
 */
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  float tmp;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f,- 5.0f, 0.0f, 1.0f, 0.0f);

  //Mapea el pasto en una figura rectangular en el xz-plano.
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -4.0, 50.0);
  glTexCoord2f(8.0, 0.0); glVertex3f(100.0, -4.0, 50.0);
  glTexCoord2f(8.0, 8.0); glVertex3f(100.0, -4.0, -10.0);
  glTexCoord2f(0.0, 8.0); glVertex3f(-100.0, -4.0, -10.0);
  glEnd();

  //Mapea el cielo en una figura rectangular en el xy-plano.
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -25.0, -70.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(100.0, -25.0, -70.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, -70.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, -70.0);
  glEnd();
  
  //Rotaciones en distintos angúlos del modelo.
  glRotatef(anguloL,0.f,1.f,0.f);
  glRotatef(anguloC,0.f,0.f,1.f);
  glRotatef(anguloA,1.f,0.f,0.f);

  //Traslación del objeto si se cumple una condición:
  if(mueve)
    glTranslated(-1*movimiento,moveW,-1);
  else
    {
    if(cambioW)
      {
	glTranslated(0,moveW,-1);
	cambioW = 0;
      }
    }
  
  tmp = scene_max.x - scene_min.x;
  tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
  tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
  tmp = 1.f / tmp;
  
  glScalef(tmp, tmp, tmp);
  
  //Lo transladamos al punto de referencia.
  glTranslatef(-scene_center.x, -scene_center.y, scene_center.z);

  //Si el display no se ha hecho, crea uno nuevo.
  if(scene_list == 0) {
    scene_list = glGenLists(1);
    glNewList(scene_list, GL_COMPILE);
    //Mostramos todo desde el nodo padre.
    recursive_render(scene, scene->mRootNode, escala);
    glEndList();
  }
  
  glCallList(scene_list);
  glutSwapBuffers();
  rota_objeto();
  mueve_objeto();
  }

/**
 *----------------------------------------------------------------------------
 * Metodo genérico para abortar una ejecución.
 *----------------------------------------------------------------------------
 */
GLboolean abortGLInit(const char* abortMessage)
{
  printf("ERROR(%s)\n",abortMessage);
  return false;  
}

/**
 *----------------------------------------------------------------------------
 * Esta función es de las 4 más importantes:
 * 4. CARGA las texturas:
 * Función para cargar las texturas de un objeto.
 * Aquí hacemos la magia del mapping de texturas al objeto.
 *----------------------------------------------------------------------------
 */
int LoadGLTextures(const aiScene* scene)
{
  ILboolean success;
  
  //Antes de cargar DevIL revisamos la versión. 
  if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
    {
      ILint test = ilGetInteger(IL_VERSION_NUM);
      std::string err_msg = "Versión de DevIL incorrecta.";
      char* cErr_msg = (char *) err_msg.c_str();
      abortGLInit(cErr_msg);
      return -1;
    }

  //Iniciamos DevIL
  ilInit(); 
  
  if (scene->HasTextures()) abortGLInit("Posible problema con la carga de texturas");

  //Obtenemos el nombre del archivo y el número de las texturas.
  for (unsigned int m=0; m<scene->mNumMaterials; m++)
    {
      int texIndex = 0;
      aiReturn texFound = AI_SUCCESS;

      //Nombre del archivo.
      aiString path;   
      
      while (texFound == AI_SUCCESS)
	{
	  texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
	  //Llenamos el map con texturas, apuntadores son todavía NULL
	  textureIdMap[path.data] = NULL;
	  texIndex++;
	}
    }
  
  int numTextures = textureIdMap.size();

  //Arreglo con id imagenes de DevIL
  ILuint* imageIds = NULL;
  imageIds = new ILuint[numTextures];
  
  // Generamos las imágenes con DevIL
  ilGenImages(numTextures, imageIds);

  //Generamos y llenamos el arreglo el arreglo con texturas de openGL ids
  textureIds = new GLuint[numTextures];
  glGenTextures(numTextures, textureIds);
    
  //Obtenemos un iterador sobre cada uno de los id de la textura.
  std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();
  
  for (int i=0; i<numTextures; i++)
    {
      
      //save IL image ID
      //Obtenemos el nombre del archivo.
      std::string filename = (*itr).first;
      //Guardamos el id de la textura.
      (*itr).second =  &textureIds[i];	 
      itr++;				
      
      if( filename == "" )
	{
	  continue;
	}

      //Unimos las imagenes que cargamos con DevIL con los id de la textura.
      ilBindImage(imageIds[i]);
      //Cargamos la imagen.
      std::string fileloc = basepath + filename;	
      

      int pos = fileloc.find("\\" );
      if( pos >= 0 )
	{
	  fileloc.replace( pos , 1 , "/" );
	}
      
      success = ilLoadImage(fileloc.c_str());

      //Si no ocurren errores:
      if (success) 
	{
	  //Convierte cada columna en un byte unsigned.
	  //Si tu imagen contiene canales alfa se deben remplanzar IL_RGB con IL_RGBA 
	  success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	  
	  if (!success)
	    {
	      //Ocurrió un error.
	      abortGLInit("No pudimos cargar la imagen");
	      return -1;
	    }

	  glBindTexture(GL_TEXTURE_2D, textureIds[i]); 

	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	  glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		       ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		       ilGetData()); 
	}
      else
	{
	  // Error
	  printf("%s(%d)No se pudo carga la imagen: %s\n",__FILE__,__LINE__,fileloc.c_str() );
	}
      
      
    }

  //Limpiamos un poco:
  ilDeleteImages(numTextures, imageIds); 
  delete [] imageIds;
  imageIds = NULL;
  
  return true;
}



/**
 *----------------------------------------------------------------------------
 * Metodo para controlar el comportamiento del resize
 *----------------------------------------------------------------------------
 */
// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/**
 *----------------------------------------------------------------------------
 * Metodo para controlar el comportamiento del resize
 *----------------------------------------------------------------------------
 */
void reshape(int width, int height)
{
  const double aspectRatio = (float) width / height, fieldOfView = 45.0;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fieldOfView, aspectRatio,
		 1.0, 1000.0);  
  glViewport(0, 0, width, height);
}

/**
 *----------------------------------------------------------------------------
 * Son las configuarciones para OpenGL
 *----------------------------------------------------------------------------
 */
int InitGL()
{
  if(!LoadGLTextures(scene))
    {
      return false;
    }

  	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);			
	glEnable(GL_DEPTH_TEST);		
	glDepthFunc(GL_LEQUAL);			
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);

	return true;
  
}

/**
 *----------------------------------------------------------------------------
 * Las funcionalidades del teclado para rotar y detener el modelo
 *----------------------------------------------------------------------------
 */
// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
   case '.':
     anguloA -= 5.0;
     if (anguloA < 0.0) anguloA += 360.0;     
     break;
   case ',':
     anguloA += 5.0;
     if (anguloA > 360.0) anguloA -= 360.0;     
     break;
      case 27:
         exit(0);
         break;
      case ' ':
	 anguloL = 0.5f;
	 anguloA = 0.5f;
	 anguloC = 0.5f;
	 moveW = 0.f;
         break;
   case 'p':
   case 'P':
     if(mueve == 1)
       mueve = 0;
     else
       mueve = 1;
     if(escala == 2.f)
       escala = 1.f;
     else
       escala = 2.f;
     break;
   case 'w':
   case 'W':
     moveW += .03;
     cambioW = 1;
     break;
   case 's':
   case 'S':
     moveW -= .03;
     break;

      default:
         break;
   }
}

/**
 *----------------------------------------------------------------------------
 * Funcionalidades de teclas especiales.
 *----------------------------------------------------------------------------
 */
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_RIGHT) 
   {
      anguloL -= 5.0;
      if (anguloL < 0.0) anguloL += 360.0;
   }
   if (key == GLUT_KEY_LEFT) 
   {
      anguloL += 5.0;
      if (anguloL > 360.0) anguloL -= 360.0;
   }
   if (key == GLUT_KEY_UP)
     {
       anguloC += 5.0;
       if (anguloC > 360.0) anguloC -= 360.0;
     }
   if (key == GLUT_KEY_DOWN)
     {
       anguloC -= 5.0;
       if (anguloC < 0.0) anguloC += 360.0;
     }
}


/**
 *----------------------------------------------------------------------------
 * Función principal del programa.
 *----------------------------------------------------------------------------
 */
int main(int argc, char **argv)
{
  
  aiLogStream stream;
  ilInit(); //Inicializamos DevIL (OpenIL)

  glutInitWindowSize(900,600);
  glutInitWindowPosition(100,100);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInit(&argc, argv);

  glutCreateWindow("UV - Mapping y Texturas");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyInput);
  glutSpecialFunc(specialKeyInput);
  stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
  aiAttachLogStream(&stream);
  stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
  aiAttachLogStream(&stream);

  //El modelo se especifica como argumento en la linea de comando,
  //Si no se hace, se toma avioneta.obj
  if( argc == 2 )
    {
      std::string input = argv[1];
      int sep = input.find_last_of( "/" );
      basepath = input.substr(0,sep+1);
      modelname = input.substr( sep+1 );
      
      if (!Import3DFromFile(argv[1]))
	{
	  printf("ERROR EN LA LINEA %d: Tuvimos problemas al cargar su archivo.\n",__LINE__);
	  return -1;
	}
    }
  else
    {
      if (!Import3DFromFile(basepath+modelname))
	{
	  printf("%d ERROR\n",__LINE__);
	  return -1;
	}
    }

  if(!InitGL() )
    {
      printf("ERROR EN LA LINEA %d.",__LINE__);
      return -1;
    }
  glClearColor(0.1f,0.1f,0.1f,1.f);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0); 
  glEnable(GL_DEPTH_TEST);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_NORMALIZE);

  setup();
  
  //En caso de que una cara esté mal dibujada:
  if(getenv("MODEL_IS_BROKEN"))  
    glFrontFace(GL_CW);
  
  glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);  
  glutGet(GLUT_ELAPSED_TIME);
  glutMainLoop();

  //Función para liberar recursos que no estemos ocupando:
  aiReleaseImport(scene);
  //Desactivamos los log stream que activamos previamente.
  aiDetachAllLogStreams();
  return 0;
}//Fin de LoadModel.cpp
