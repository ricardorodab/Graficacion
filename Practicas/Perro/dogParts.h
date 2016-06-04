//#ifdef WIN32
//#include <windows.h>
//#endif

#include <OpenGL/gl.h>
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


// Function prototypes
	void OGL_Set_Current_Material_Parameters(float shininess, float r_diffuseColor, float g_diffuseColor, float b_diffuseColor, float r_specularColor, float g_specularColor, float b_specularColor);
	void OGL_Set_Current_Texture_Map(char *texture_map_name, float u_scale, float v_scale, float u_offset, float v_offset);
	void dog_body();
	void dog_ear();
	void dog_eye();
	void dog_face();
	void dog_leg_fr_l0();
	void dog_leg_fr_l1();
	void dog_leg_fr_l2();
	void dog_leg_fr_r0();
	void dog_leg_fr_r1();
	void dog_leg_fr_r2();
	void dog_leg_hd_l0();
	void dog_leg_hd_l1();
	void dog_leg_hd_l2();
	void dog_leg_hd_r0();
	void dog_leg_hd_r1();
	void dog_leg_hd_r2();
	void dog_tail();
