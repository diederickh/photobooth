#ifndef PHOTOBOOTH_SHADERS_H
#define PHOTOBOOTH_SHADERS_H

#include <opengl/Headers.h>
#include <math/Math.h>

// --------------------------------------------------
static const char* P_VS = ""
  "#version 150\n"
  "uniform mat4 u_pm;"
  "in vec4 a_pos;"
  "void main() {"
  "  gl_Position = u_pm * a_pos;"
  "}"
  "";

static const char* P_FS = ""
  "#version 150\n"
  "out vec4 fragcolor; "
  "void main() {"
  "  fragcolor = vec4(0.5, 0.3, 0.2, 1.0); "
  "}"
  "";

static const char* PT_VS = "" 
  "#version 150\n"
  "uniform mat4 u_pm;"
  "uniform mat4 u_mm;"
  "in vec4 a_pos;"
  "in vec2 a_tex;"
  "out vec2 v_tex;"
  "void main() {"
  "  gl_Position = u_pm * u_mm * a_pos;"
  "  v_tex = a_tex;" 
  "}"
  "";

static const char* PT_FS = "" 
  "#version 150\n"
  "uniform sampler2D u_tex;"
  "out vec4 fragcolor;"
  "in vec2 v_tex;"
  "void main() {"
  "  fragcolor = texture(u_tex, v_tex);"
  "}"
  "";

static const char* YUV_FS = ""
  "#version 150\n"
  "uniform sampler2D u_tex;"
  "in vec2 v_tex;"
  "out vec4 fragcolor; "
  ""
  "const vec3 R_cf = vec3(1.164383,  0.000000,  1.596027);"
  "const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968);"
  "const vec3 B_cf = vec3(1.164383,  2.017232,  0.000000);"
  "const vec3 offset = vec3(-0.0625, -0.5, -0.5);"
 
  "void main() {"
  "  vec3 tc =  texture(u_tex, v_tex).rgb;"
  "  vec3 yuv = vec3(tc.g, tc.b, tc.r);"
  "  yuv += offset;"
  "  fragcolor.r = dot(yuv, R_cf);"
  "  fragcolor.g = dot(yuv, G_cf);"
  "  fragcolor.b = dot(yuv, B_cf);"
  "  fragcolor.a = 1.0;"
  "}"
  "";

// attribute less fullscreen vertex shader
static const char* FULLSCREEN_PT_VS = ""
  "#version 150\n"
  "const vec2 verts[4] = vec2[] ("
  "  vec2(-1.0, 1.0), " 
  "  vec2(-1.0, -1.0), "
  "  vec2(1.0, 1.0), "
  "  vec2(1.0, -1.0)"
  ");"

  "const vec2 tex[4] = vec2[] ("
  "  vec2(0.0, 1.0), " 
  "  vec2(0.0, 0.0), "
  "  vec2(1.0, 1.0), "
  "  vec2(1.0, 0.0)"
  ");"

  "out vec2 v_tex; " 
  "void main() {"
  "  gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0); "
  "  v_tex = tex[gl_VertexID];"
  "}"
  "";

static const char* GRAYSCALE_FS = ""
  "#version 150\n"
  "uniform sampler2D u_tex; " 
  "in vec2 v_tex; "
  "out vec4 fragcolor;"
  ""
  "void main() {"
  "  vec4 tc = texture(u_tex, v_tex);"
  "  float gray = 0.2126 * tc.r + 0.7152 * tc.g + 0.0722 * tc.b; " 
  //"  float gray = 0.299 * tc.r + 0.587 * tc.g + 0.114 * tc.b; "
  "  fragcolor.rgb = vec3(gray);"
  "  fragcolor.a = 1.0; " 
  "}"
  "";

static const char* SEPIA_FS = ""
  "#version 150\n"
  "uniform sampler2D u_tex; " 
  "in vec2 v_tex; "
  "out vec4 fragcolor;"
  ""
  "void main() {"
  "  vec4 tc = texture(u_tex, v_tex);"
  "  float gray = 0.2126 * tc.r + 0.7152 * tc.g + 0.0722 * tc.b; " 
  "  vec3 sepia = vec3(0.44, 0.26, 0.078); " 
  "  if(gray <= 0.5) { "
  "     fragcolor.rgb = 2.0 * gray * sepia; "
  "  } "
  "  else { "
  "     fragcolor.rgb = 1.0 - 2.0 * (1.0 - gray) * (vec3(1.0) - sepia); "
  "  }"
  "  fragcolor.a = 1.0; " 
  "}"
  "";

static const char* MIRROR_FS = ""
  "#version 150\n"
  "uniform sampler2D u_tex; " 
  "in vec2 v_tex; "
  "out vec4 fragcolor;"
  "void main() {"
  "  vec2 coord = v_tex; " 
  "  if(v_tex.s >= 0.5) { "
  "    coord.s = 1.0 - v_tex.s; "
  "  }"
  "  vec4 tc = texture(u_tex, coord);"
  "  fragcolor.rgb = tc.rgb;"
  "  fragcolor.a = 1.0;"
  " } "
  "";

static const char* BULGE_FS = ""
  "#version 150\n"
  "uniform sampler2D u_tex; " 
  "in vec2 v_tex; "
  "out vec4 fragcolor;"
  ""
  " void main() {"

  // bulge, pinch with a smooth transition, based on: https://github.com/evanw/glfx.js/blob/master/src/filters/warp/bulgepinch.js
  "  vec2 center_coord = vec2(0.5, 0.5);"
  "  vec2 dir_coord = v_tex - center_coord; "
  "  float dist = length(dir_coord);"
  "  float radius = 0.4;"
  "  float s = v_tex.s;"
  "  float t = v_tex.t;"
  "  if(dist < radius) {"
  "    float p = dist / radius; "
  "    dir_coord *= mix(1.0, smoothstep(0.0, (radius / dist), p), 1.0 * 0.75);"
  "    s = dir_coord.s + center_coord.s; " 
  "    t = dir_coord.t + center_coord.t; " 
  "  }"
  "  vec4 tc = texture(u_tex, vec2(s, t)); " 
  "  fragcolor.rgb = tc.rgb; "
  "  fragcolor.a = 1.0; "
  "}"

  // mathematical correct bulge
#if 0
  "  vec2 center_coord = vec2(0.5, 0.5);"
  "  vec2 dir_coord = center_coord - v_tex; "
  "  float dist = length(dir_coord);"
  "  float r = pow(dist, 2.3) * 2.0;"
  "  float u = r * ((v_tex.s - 0.5) / dist) + 0.5; "
  "  float v = r * ((v_tex.t - 0.5) / dist) + 0.5; "
  "  tc = texture(u_tex, vec2(u,v)); " 
  "  fragcolor.rgb = tc.rgb;"
#endif

  "";

// --------------------------------------------------

class PhotoBooth;

class Graphics {
 public:
  Graphics(PhotoBooth& booth);
  ~Graphics();
  bool setup();

 public:
  PhotoBooth& booth;

  /* VertexP shader */
  GLuint vert_p;
  GLuint frag_p_debug;
  GLuint prog_p_debug;

  /* VertexPT shader */
  GLuint vert_pt;
  GLuint frag_pt;
  GLuint prog_pt;

  /* Fullscreen attribute less shader */
  GLuint vert_fullscreen_pt;
  GLuint prog_fullscreen_pt;

  /* YUYV shader */
  GLuint frag_yuv;
  GLuint prog_yuv;

  /* Grayscale effect */
  GLuint frag_grayscale;
  GLuint prog_grayscale;

  /* Sepia effect */
  GLuint frag_sepia;
  GLuint prog_sepia;

  /* Mirror effect */
  GLuint frag_mirror;
  GLuint prog_mirror;

  /* Bulge effect */
  GLuint frag_bulge;
  GLuint prog_bulge;

  Mat4 ortho_matrix;

  /* Used to draw a texture at the center */
  GLuint img_vbo;
  GLuint img_vao;
};

#endif
