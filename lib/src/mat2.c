/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "cglm/cglm.h"
#include "cglm/call.h"

CGLM_EXPORT
void
glmc_mat2_make(const float * __restrict src, mat2 dest) {
  glm_mat2_make(src, dest);
}

CGLM_EXPORT
void
glmc_mat2_copy(mat2 mat, mat2 dest) {
  glm_mat2_copy(mat, dest);
}

CGLM_EXPORT
void
glmc_mat2_identity(mat2 m) {
  glm_mat2_identity(m);
}

CGLM_EXPORT
void
glmc_mat2_identity_array(mat2 * __restrict mats, size_t count) {
  glm_mat2_identity_array(mats, count);
}

CGLM_EXPORT
void
glmc_mat2_zero(mat2 m) {
  glm_mat2_zero(m);
}

CGLM_EXPORT
void
glmc_mat2_mul(mat2 m1, mat2 m2, mat2 dest) {
  glm_mat2_mul(m1, m2, dest);
}

CGLM_EXPORT
void
glmc_mat2_mulv(mat2 m, vec2 v, vec2 dest) {
  glm_mat2_mulv(m, v, dest);
}

CGLM_EXPORT
void
glmc_mat2_transpose_to(mat2 mat, mat2 dest) {
  glm_mat2_transpose_to(mat, dest);
}

CGLM_EXPORT
void
glmc_mat2_transpose(mat2 m) {
  glm_mat2_transpose(m);
}

CGLM_EXPORT
void
glmc_mat2_scale(mat2 m, float s) {
  glm_mat2_scale(m, s);
}

CGLM_EXPORT
void
glmc_mat2_inv(mat2 mat, mat2 dest) {
  glm_mat2_inv(mat, dest);
}

CGLM_EXPORT
void
glmc_mat2_swap_col(mat2 mat, int col1, int col2) {
  glm_mat2_swap_col(mat, col1, col2);
}

CGLM_EXPORT
void
glmc_mat2_swap_row(mat2 mat, int row1, int row2) {
  glm_mat2_swap_row(mat, row1, row2);
}

CGLM_EXPORT
float
glmc_mat2_det(mat2 m) {
  return glm_mat2_det(m);
}

CGLM_EXPORT
float
glmc_mat2_trace(mat2 m) {
  return glm_mat2_trace(m);
}

CGLM_EXPORT
float
glmc_mat2_rmc(vec2 r, mat2 m, vec2 c) {
  return glm_mat2_rmc(r, m, c);
}
