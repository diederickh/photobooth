#include <photobooth/Graphics.h>
#include <photobooth/PhotoBooth.h>

Graphics::Graphics(PhotoBooth& booth)
  :booth(booth)
  ,vert_p(0)
  ,frag_p_debug(0)
  ,prog_p_debug(0)
  ,frag_yuv(0)
  ,prog_yuv(0)
  ,vert_pt(0)
  ,frag_pt(0)
  ,prog_pt(0)
  ,vert_fullscreen_pt(0)
  ,prog_fullscreen_pt(0)
  ,img_vbo(0)
  ,img_vao(0)
{
}

Graphics::~Graphics() {
  printf("@TODO cleanup Graphics.\n");
}

bool Graphics::setup() {
  // Matrices
  ortho_matrix.orthoBottomLeft(booth.win_w, booth.win_h, 0.0f, 10.0f);

  // Debug program
  vert_p = rx_create_shader(GL_VERTEX_SHADER, P_VS);
  frag_p_debug = rx_create_shader(GL_FRAGMENT_SHADER,  P_FS);
  prog_p_debug = rx_create_program(vert_p, frag_p_debug);
  glBindAttribLocation(prog_p_debug, 0, "a_pos");
  glLinkProgram(prog_p_debug); 
  eglGetShaderLinkLog(prog_p_debug);

  // VertexPT
  vert_pt = rx_create_shader(GL_VERTEX_SHADER, PT_VS);
  frag_pt = rx_create_shader(GL_FRAGMENT_SHADER, PT_FS);
  prog_pt = rx_create_program(vert_pt, frag_pt);
  glBindAttribLocation(prog_pt, 0, "a_pos");
  glBindAttribLocation(prog_pt, 1, "a_tex");
  glLinkProgram(prog_pt);
  eglGetShaderLinkLog(prog_pt);
  
  // YUYV program
  frag_yuv = rx_create_shader(GL_FRAGMENT_SHADER, YUV_FS);
  prog_yuv = rx_create_program(vert_pt, frag_yuv);
  glBindAttribLocation(prog_yuv, 0, "a_pos");
  glBindAttribLocation(prog_yuv, 1, "a_tex");
  glLinkProgram(prog_yuv);
  eglGetShaderLinkLog(prog_yuv);

  // Fullscreen attribute less 
  vert_fullscreen_pt = rx_create_shader(GL_VERTEX_SHADER, FULLSCREEN_PT_VS);
  prog_fullscreen_pt = rx_create_program(vert_fullscreen_pt, frag_pt);
  glLinkProgram(prog_fullscreen_pt);
  eglGetShaderLinkLog(prog_fullscreen_pt);

  // Grayscale effect
  frag_grayscale = rx_create_shader(GL_FRAGMENT_SHADER, GRAYSCALE_FS);
  prog_grayscale = rx_create_program(vert_fullscreen_pt, frag_grayscale);
  glLinkProgram(prog_grayscale);
  eglGetShaderLinkLog(prog_grayscale);

  // Sepia effect
  frag_sepia = rx_create_shader(GL_FRAGMENT_SHADER, SEPIA_FS);
  prog_sepia = rx_create_program(vert_fullscreen_pt, frag_sepia);
  glLinkProgram(prog_sepia);
  eglGetShaderLinkLog(prog_sepia);

  // Mirror effect
  frag_mirror = rx_create_shader(GL_FRAGMENT_SHADER, MIRROR_FS);
  prog_mirror = rx_create_program(vert_fullscreen_pt, frag_mirror);
  glLinkProgram(prog_mirror);
  eglGetShaderLinkLog(prog_mirror);

  // Bulge effect
  frag_bulge = rx_create_shader(GL_FRAGMENT_SHADER, BULGE_FS);
  prog_bulge = rx_create_program(vert_fullscreen_pt, frag_bulge);
  glLinkProgram(prog_bulge);
  eglGetShaderLinkLog(prog_bulge);

  // Setup VBO + VAO for images
  {
    glGenVertexArrays(1, &img_vao);
    glBindVertexArray(img_vao);
    glGenBuffers(1, &img_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, img_vbo);
    glEnableVertexAttribArray(0); // pos;
    glEnableVertexAttribArray(1); // tex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid*)12);

    Vertices<VertexPT> verts;
    verts.push_back(VertexPT(Vec3(-0.5f, -0.5f, 0.0f), Vec2(0.0f, 0.0f)));  // bottom left
    verts.push_back(VertexPT(Vec3(-0.5f, 0.5f, 0.0f), Vec2(0.0f, 1.0f)));   // top left
    verts.push_back(VertexPT(Vec3(0.5f, -0.5f, 0.0f), Vec2(1.0f, 0.0f)));   // bottom right
    verts.push_back(VertexPT(Vec3(0.5f, 0.5f, 0.0f), Vec2(1.0f, 1.0f)));    // top right

    glBufferData(GL_ARRAY_BUFFER, verts.getNumBytes(), verts.getPtr(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  return true;
}
