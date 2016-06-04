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

// Program and Shader Identifiers
//GLuint program, vertexShader, fragmentShader;
// Uniform Bindings Points
//GLuint matricesUniLoc = 1, materialUniLoc = 2;
// Uniform Buffer for Matrices
// this buffer will contain 3 matrices: projection, view and model
// each matrix is a float array with 16 components
//GLuint matricesUniBuffer;
//#define MatricesUniBufferSize sizeof(float) * 16 * 3
//#define ProjMatrixOffset 0
//#define ViewMatrixOffset sizeof(float) * 16
//#define ModelMatrixOffset sizeof(float) * 16 * 2
//#define MatrixSize sizeof(float) * 16
//A partir de aquí todo se necesita:
/* Variables globales*/
const struct aiScene* scene = NULL;
GLuint scene_list = 0;
// images / texture
// map image filenames to textureIds
std::map<std::string, GLuint*> textureIdMap;	
GLuint* textureIds;
// pointer to texture Array

// Create an instance of the Importer class
Assimp::Importer importer;

static std::string basepath = "./";
static std::string modelname = "avioneta.obj";

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 15.0f, 1.0f };
aiVector3D scene_min, scene_max, scene_center;
//rotación
static float angulo = 0.5;
static float movimiento = 0.f;
// Textura
static unsigned int texture[2]; // Array of texture indices.
static float d = 0.0; // Distance parameter in gluLookAt().
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


// Load external textures.
void loadExternalTextures()
{
  // Local storage for bmp image data.
  BitMapFile *image[2];
  
  // Load the images.
  image[0] = getbmp("grass.bmp");
  image[1] = getbmp("sky.bmp");
  
  // Bind grass image to texture object texture[0]. 
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
	       GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  // Bind sky image to texture object texture[1]
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
	       GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setup(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  
  // Create texture ids.
  glGenTextures(2, texture);
  
  // Load external textures.
  loadExternalTextures();
  
  // Specify how texture values combine with current surface color values.
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  
  // Turn on OpenGL texturing.
  glEnable(GL_TEXTURE_2D);
  
  // Cull back faces.
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void rota_objeto (void)
{
  static GLint prev_time = 0;
  
  int time = glutGet(GLUT_ELAPSED_TIME);
  //angulo += (time-prev_time)*0.01;
  prev_time = time;
  
  glutPostRedisplay ();
}

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


/**/
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

/* ---------------------------------------------------------------------------- */
void get_bounding_box(aiVector3D* min, aiVector3D* max)
{
  aiMatrix4x4 trafo;
  aiIdentityMatrix4(&trafo);
  
  min->x = min->y = min->z = 1e10f;
  max->x = max->y = max->z = -1e10f;
  get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
}

void Color4f(const aiColor4D *color)
{
  glColor4f(color->r,color->g,color->b,color->a);
}

/* ---------------------------------------------------------------------------- */
void color4_to_float4(const aiColor4D *c, float f[4])
{
  f[0] = c->r;
  f[1] = c->g;
  f[2] = c->b;
  f[3] = c->a;
}

/* ---------------------------------------------------------------------------- */
void set_float4(float f[4], float a, float b, float c, float d)
{
  f[0] = a;
  f[1] = b;
  f[2] = c;
  f[3] = d;
}

/**/
bool Import3DFromFile(const std::string& pFile)
{
  
  //check if file exists
  std::ifstream fin(pFile.c_str());
  if (!fin.fail()) {
    fin.close();
  }
  else{
    printf("Couldn't open file: %s\n", pFile.c_str());
    printf("%s\n", importer.GetErrorString());
    return false;
  }
  
  scene = importer.ReadFile(pFile, aiProcessPreset_TargetRealtime_Quality);
  
  // If the import failed, report it
  if (!scene)
    {
      printf("%s\n", importer.GetErrorString());
      return false;
    }
  
  get_bounding_box(&scene_min, &scene_max);
  
  scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
  scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
  scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
  
  // Now we can access the file's contents.
  printf("Import of scene %s succeeded.", pFile.c_str());
  
  // We're done. Everything will be cleaned up by the importer destructor
  return true;
}


/** */
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
  unsigned int max;	// changed: to unsigned
  
  int texIndex = 0;
  aiString texPath;	//contains filename of texture
  
  if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
    {
      //bind texture
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

void recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale)
{
  unsigned int i;
  unsigned int n=0, t;
  aiMatrix4x4 m = nd->mTransformation;
  
  m.Scaling(aiVector3D(scale, scale, scale), m);
  
  // update transform
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
	
	for(i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
	  {
	    int vertexIndex = face->mIndices[i];	// get group index for current index
	    if(mesh->mColors[0] != NULL)
	      Color4f(&mesh->mColors[0][vertexIndex]);
	    if(mesh->mNormals != NULL)
	      
	      if(mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
		{
		  glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
		}
	    
	    glNormal3fv(&mesh->mNormals[vertexIndex].x);
	    glVertex3fv(&mesh->mVertices[vertexIndex].x);
	  }
	
	glEnd();
	
      }
      
    }
  
  
  // draw all children
  for (n = 0; n < nd->mNumChildren; ++n)
    {
      recursive_render(sc, nd->mChildren[n], scale);
    }
  
  glPopMatrix();
}


/** */
void display()
{
  //Merge
  
  // Comentada este ultimo intento:
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  //glLoadIdentity();
  //gluLookAt(0.0, 10.0, 15.0 + d, 0.0, 10.0, d, 0.0, 1.0, 0.0);
  
  // Map the grass texture onto a rectangle along the xz-plane.
  //Aquí quité el código
  //glutSwapBuffers();
  //Merge
  
  float tmp;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f,- 5.0f, 0.0f, 1.0f, 0.0f);
  //Aquí inserto el código.
  //Map the grass texture onto a rectangle along the xz-plane. 
   glBindTexture(GL_TEXTURE_2D, texture[0]);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -4.0, 50.0);
  glTexCoord2f(8.0, 0.0); glVertex3f(100.0, -4.0, 50.0);
  glTexCoord2f(8.0, 8.0); glVertex3f(100.0, -4.0, -10.0);
  glTexCoord2f(0.0, 8.0); glVertex3f(-100.0, -4.0, -10.0);
  glEnd();
  
  // Map the sky texture onto a rectangle parallel to the xy-plane.
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -25.0, -70.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(100.0, -25.0, -70.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, -70.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, -70.0);
  glEnd();
  
  //Aquí termina el codigo que inserté
  //Rotación alrededor del y
  glRotatef(angulo,0.f,1.f,0.f);
  //Translación del objeto en x.
  // Comentada este ultimo intento:
  glTranslated(-1*movimiento,1,-1);

  tmp = scene_max.x - scene_min.x;
  tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
  tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
  tmp = 1.f / tmp;
  
  glScalef(tmp, tmp, tmp);
  
  glTranslatef(-scene_center.x, -scene_center.y, scene_center.z);

  // if the display list has not been made yet, create a new one and
  // fill it with scene contents
  if(scene_list == 0) {
    scene_list = glGenLists(1);
    glNewList(scene_list, GL_COMPILE);
    // now begin at the root node of the imported data and traverse
    // the scenegraph by multiplying subsequent local transforms
    // together on GL's matrix stack.
    recursive_render(scene, scene->mRootNode, 1.0);
    glEndList();
  }
  
  glCallList(scene_list);
  //recursive_render(scene, scene->mRootNode);
  glutSwapBuffers();
  rota_objeto();
  mueve_objeto();
  }


GLboolean abortGLInit(const char* abortMessage)
{
  printf("ERROR(%s)\n",abortMessage);
  // quit and return False
  return false;  
}
/*
void ReSizeGLScene(GLsizei width, GLsizei height)				// Resize And Initialize The GL Window
{
	if (height==0)								// Prevent A Divide By Zero By
	{
		height=1;							// Making Height Equal One
	}

	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,10000.0f);

	glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
	glLoadIdentity();							// Reset The Modelview Matrix
}
*/

int LoadGLTextures(const aiScene* scene)
{
  ILboolean success;
  
  /* Before calling ilInit() version should be checked. */
  if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
    {
      ILint test = ilGetInteger(IL_VERSION_NUM);
      /// wrong DevIL version ///
      std::string err_msg = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
      char* cErr_msg = (char *) err_msg.c_str();
      abortGLInit(cErr_msg);
      return -1;
    }
  
  ilInit(); /* Initialization of DevIL */
  
  if (scene->HasTextures()) abortGLInit("Support for meshes with embedded textures is not implemented");
  
  /* getTexture Filenames and Numb of Textures */
  for (unsigned int m=0; m<scene->mNumMaterials; m++)
    {
      int texIndex = 0;
      aiReturn texFound = AI_SUCCESS;
      
      aiString path;	// filename
      
      while (texFound == AI_SUCCESS)
	{
	  texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
	  textureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
	  texIndex++;
	}
    }
  
  int numTextures = textureIdMap.size();
  
  /* array with DevIL image IDs */
  ILuint* imageIds = NULL;
  imageIds = new ILuint[numTextures];
  
  /* generate DevIL Image IDs */
  ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */
  
  /* create and fill array with GL texture ids */
  textureIds = new GLuint[numTextures];
  glGenTextures(numTextures, textureIds); /* Texture name generation */
  
  /* define texture path */
  //std::string texturepath = "../../../test/models/Obj/";
  
  /* get iterator */
  std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();
  
  for (int i=0; i<numTextures; i++)
    {
      
      //save IL image ID
      std::string filename = (*itr).first;  // get filename
      (*itr).second =  &textureIds[i];	  // save texture id for filename in map
      itr++;								  // next texture
      
      if( filename == "" )
	{
	  continue;
	}
      
      ilBindImage(imageIds[i]); /* Binding of DevIL image name */
      std::string fileloc = basepath + filename;	/* Loading of image */
      
      //Charactor code translate.
      int pos = fileloc.find("\\" );
      if( pos >= 0 )
	{
	  fileloc.replace( pos , 1 , "/" );
	}
      
      success = ilLoadImage(fileloc.c_str());
      
      if (success) /* If no error occured: */
	{
	  /* Convert every colour component into
	     unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
	  success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	  
	  if (!success)
	    {
	      /* Error occured */
	      abortGLInit("Couldn't convert image");
	      return -1;
	    }
	  //glGenTextures(numTextures, &textureIds[i]); /* Texture name generation */
	  glBindTexture(GL_TEXTURE_2D, textureIds[i]); /* Binding of texture name */
	  //redefine standard texture values
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
									       interpolation for magnification filter */
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
									       interpolation for minifying filter */
	  glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		       ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		       ilGetData()); /* Texture specification */
	}
      else
	{
	  /* Error occured */
	  printf("%s(%d)Couldn't load Image: %s\n",__FILE__,__LINE__,fileloc.c_str() );
	}
      
      
    }
  
  ilDeleteImages(numTextures, imageIds); /* Because we have already copied image data into texture data
					    we can release memory used by image. */
  
  //Cleanup
  delete [] imageIds;
  imageIds = NULL;
  
  //return success;
  return true;
}



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


void reshape(int width, int height)
{
  const double aspectRatio = (float) width / height, fieldOfView = 45.0;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fieldOfView, aspectRatio,
		 1.0, 1000.0);  /* Znear and Zfar */
  glViewport(0, 0, width, height);
}

/**
 * Son las configuarciones para OpenGL
 */
int InitGL()
{
  if(!LoadGLTextures(scene))
    {
      return false;
    }

  	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);

	return true;
  
}

int main(int argc, char **argv)
{
  //bool b = Import3DFromFile("Sony_DSC_W170_obj.obj");
  //bool b = Import3DFromFile("avioneta.obj");
  
  aiLogStream stream;
  ilInit(); //Inicializamos DevIL (OpenIL)

  glutInitWindowSize(900,600);
  glutInitWindowPosition(100,100);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInit(&argc, argv);

  glutCreateWindow("UV - Mapping y Texturas");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
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
