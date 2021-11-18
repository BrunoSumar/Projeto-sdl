#ifndef SCENE_H_
#define SCENE_H_

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>

/* glm::mat4 Model = glm::scale(glm::identity(), glm::vec3(0.5f)); */ //tralate, scale, rotate, identity
/* return glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 100.f); */

struct Scene{
  /* Camera */
  glm::vec3 eye;
  glm::vec3 center;
  glm::vec3 up;
  glm::mat4 view;

  glm::mat4 Projection;

  Scene(glm::vec3 e, glm::vec3 c, glm::vec3 u, glm::mat4 p): e{e}{
    eye = e;
    center = c;
    up = u;

    updateView();
  }

  void updateView(){
    view = glm::lookAt(eye, center, up);
  }

};



#endif // SCENE_H_
